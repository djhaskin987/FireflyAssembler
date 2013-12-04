#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <ctime>
#include "StraightMatcher.hpp"
#include "DPMatcher.hpp"
#include "Sequence.hpp"
#include "Types.hpp"
#include "Graph.hpp"
#include "Path.hpp"
#include "FitnessFunction.hpp"
#include "GreedyPathFinder.hpp"
#include "LocalSearchPathFinder.hpp"
#include "FireflyPathFinder.hpp"
#include "MeanOverlap.hpp"
#include "N50Rating.hpp"
#include "DistanceMetric.hpp"
#include "HammingDistance.hpp"

using namespace std;
using namespace FireflyAssembler;
typedef VectorPointer<FireflyAssembler::Sequence>
    SequenceVectorPointer;
typedef vector<FireflyAssembler::Sequence> SequenceVector;
using Sequence = FireflyAssembler::Sequence;

SequenceVectorPointer deserializeSequences(string fileName, MatcherConstPointer m)
{
    SequenceVectorPointer sequences(new SequenceVector());

    // Open file and create RecordReader.
    ifstream in(fileName.c_str());
    if (!in.is_open())
    {
        cerr << "Problem occurred in opening file for input." << endl;
        exit(1);
    }

    // Read file record-wise.
    string line;

    while (!in.eof())
    {
        getline(in,line);
        if ( line[0] == '>' )
        {
            sequences->push_back(FireflyAssembler::Sequence(m));
        }
        else
        {
            // trim the string
            while (line.back() == '\r' || line.back() == '\n' ||
                    line.back() == ' ' || line.back() == '\t')
            {
                line.pop_back();
            }
            if (line.size() == 0)
            {
                continue;
            }
            SequenceVector::iterator s = sequences->end() - 1;
            s->append(line);
        }
    }
    in.close();
    if (sequences->size() == 0)
    {
        cerr << "No sequences were found in file '" << fileName.c_str() << "'!"
             << endl;
        exit(1);
    }
    if (sequences->size() == 1)
    {
        cerr << "Only one sequence was found in file '"
             << fileName.c_str() << "'. No assembly is needed." << endl;
        exit(1);
    }
    return sequences;
}

typedef enum
{
    HAMMING
} distance_type;

typedef enum
{
    MEAN_OVERLAP,
    N50
} fitness_function_type;

typedef enum
{
    GREEDY,
    FIREFLY,
    LOCAL
} path_finder_type;

typedef enum
{
    STRAIGHT,
    DP
} matcher_type;

void usage(int exitCode)
{
    cout << "Usage: FireflyAssembler [<options>...] <input-fasta-file> <output-file>" << endl
        << "  Where:" << endl
        << "    -d, --distance-metric {hamming}" << endl
        << "        Specify distance metric. Currently only 'hamming' is supported." << endl
        << "        ('hamming' is the default value)" << endl
        << "    -f, --fitness-function {meanoverlap|n50}" << endl
        << "        Specify fitness funciton (default is 'meanoverlap')" << endl
        << "    -p, --path-finder {greedy|firefly|localsearch}" << endl
        << "        Specify path finder (default is greedy)" << endl
        << "    -m, --matcher {straight|dynamicprogramming}" << endl
        << "        Specify overlap/container matcher (straight is default)" << endl;
    exit(exitCode);
}

void getArgs(distance_type & distanceMeasure,
        fitness_function_type & ffType,
        path_finder_type & pfType,
        matcher_type & mType,
        string & inputFile,
        string & outputFile,
        int argc,
        char *argv[])
{
    int currentArgIndex = 1;
    while (currentArgIndex < argc)
    {
        string key;
        string val;
		key = argv[currentArgIndex];
		if (currentArgIndex + 1 < argc)
		{
			val = argv[currentArgIndex + 1];
		}
		else
		{
			val = "";
		}
        if (key == "-h" || key == "--help")
        {
            usage(0);
        }
        else if (key == "-d" || key == "--distance-metric")
        {
            if (val == "hamming")
            {
                distanceMeasure = HAMMING;
            }
            else
            {
                cerr << "Invalid distance measure type '"
                     << argv[++currentArgIndex] << "'." << endl;
                usage(1);
            }
            currentArgIndex += 2;
        }
        else if (key == "-f" || key == "--fitness-function")
        {
            if (val == "meanoverlap")
            {
                ffType = MEAN_OVERLAP;
            }
            else if (val == "n50")
            {
                ffType = N50;
            }
            else
            {
                cerr << "Invalid fitness function type '"
                     << argv[++currentArgIndex] << "'." << endl;
                usage(1);
            }
            currentArgIndex += 2;
        }
        else if (key == "-m" || key == "--matcher")
        {
            if (val == "straight")
            {
                mType = STRAIGHT;
            }
            else if (val == "dynamicprogramming")
            {
                mType = DP;
            }
            else
            {
                cerr << "Invalid matcher type '"
                     << argv[++currentArgIndex] << "'." << endl;
                usage(1);
            }
            currentArgIndex += 2;
        }
        else if (key == "-p" || key == "--path-finder")
        {
            if (val == "greedy")
            {
                pfType = GREEDY;
            }
            else if (val == "firefly")
            {
                pfType = FIREFLY;
            }
            else if (val == "localsearch")
            {
                pfType = LOCAL;
            }
            else
            {
                cerr << "Invalid path finder type '"
                     << argv[++currentArgIndex] << "'."
                     << endl;
                usage(1);
            }
            currentArgIndex += 2;
        }
        else
        {
            inputFile = key;
            outputFile = val;
            currentArgIndex += 2;
        }
    }
    if (inputFile.length() == 0)
    {
        cerr << "Please specify input fasta file name." << endl;
        usage(1);
    }
    if (outputFile.length() == 0)
    {
        cerr << "Please specify output file name." << endl;
        usage(1);
    }
}



SequenceVectorPointer
    eliminateContains(const SequenceVector & sequences)
{
    SequenceVectorPointer returned(new SequenceVector());
    for (int i = 0; i < sequences.size(); i++)
    {
        bool isIContainedAnywhere = false;
        for (int j = 0; j < sequences.size(); j++)
        {
            if (i == j)
                continue;
            if (sequences[j].containsSize(sequences[i]) > 0)
            {
                isIContainedAnywhere = true;
                break;
            }
        }
        if (!isIContainedAnywhere)
        {
            returned->push_back(sequences[i]);
        }
    }
    return returned;
}

void output(const string & filename, SequenceVectorPointer contigs)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Problem occurred in opening file for output." << endl;
        exit(1);
    }
    for (int j = 0; j < contigs->size(); j++)
    {
        file << ">Contig " << j << endl;
        file << (*contigs)[j] << endl;
    }
    file.close();
}

int main(int argc, char * argv[])
{
    cout.precision(6);
    string inputFile = "";
    string outputFile = "";
    distance_type distanceMeasure = HAMMING;
    fitness_function_type ffType = MEAN_OVERLAP;
    path_finder_type pfType = GREEDY;
    matcher_type mType = STRAIGHT;
    getArgs(distanceMeasure,
            ffType,
            pfType,
            mType,
            inputFile,
            outputFile,
            argc,
            argv);
    PathFinderPointer pathFinder;
    switch (pfType)
    {
        case FIREFLY:
            cout << "Using the firefly path finder." << endl;
        	pathFinder.reset(new FireflyPathFinder());
        	break;
        case GREEDY:
            cout << "Using the greedy path finder." << endl;
            pathFinder.reset(new GreedyPathFinder());
            break;
        case LOCAL:
            cout << "Using the local search path finder." << endl;
            pathFinder.reset(new LocalSearchPathFinder());
            break;
        default:
            throw logic_error("Internal state is faulty. Cannot continue.");
            break;
    };

    DistanceMetricPointer dm;
    switch (distanceMeasure)
    {
        case HAMMING:
            cout << "Using Hamming distance (this only applies for the firefly" << endl
                << "  pathfinder)." << endl;
            dm.reset(new HammingDistance());
            break;
        default:
            throw logic_error("Internal state is faulty. Cannot continue.");
            break;
    };
    FitnessFunctionPointer fitnessFunction;
    switch (ffType)
    {
        case N50:
            cout << "Using N50 longest length as the fitness function" << endl
                 << "  (this only applies to the local search and firefly path" << endl
                 << "  finders)." << endl;
            fitnessFunction.reset(new N50Rating());
            break;
        case MEAN_OVERLAP:
            cout << "Using largest mean overlap as the fitness function" << endl
                 << "  (this only applies to the local search and firefly path" << endl
                 << "  finders)." << endl;
            fitnessFunction.reset(new MeanOverlap());
            break;
        default:
            throw logic_error("Internal state is faulty. Cannot continue.");
            break;
    };

    // This do loop is how we deal with 'contains' problem, where one sequence
    // may contain another.
    // We pre-process for contigs containing other contigs and get rid of those
    // ones which are contained.
    // Then we merge based on overlaps.
    // We continue until the merge process stops giving good results.
    //
    // This do loop also is nice because some of the algorithms (local search,
    // firefly) use random initialization and so could do better if we tried
    // them over and over.
    MatcherConstPointer theMatcher;
    switch (mType)
    {
        case STRAIGHT:
            cout << "Using the straight (hamming-distance-like)" << endl
                 << "  matching algorithm." << endl;
            theMatcher.reset((Matcher*)new StraightMatcher());
            break;
        case DP:
            cout << "Using the dynamic programming (edit-distance-like)" << endl
                << "  matching algorithm." << endl;
            theMatcher.reset((Matcher*)new DPMatcher());
            break;
        default:
            throw logic_error("Internal state is faulty. Cannot continue.");
            break;
    };
    cout << "Loading sequences from file into memory..." << endl;
    clock_t loadTime = clock();
    SequenceVectorPointer
        contigs(deserializeSequences(inputFile,theMatcher));
    loadTime = clock() - loadTime;
    cout << "Loading took " << (((float)loadTime)/CLOCKS_PER_SEC) << " seconds, "
         << loadTime << " clock ticks."
         << endl;

    SequenceVectorPointer sequences;
    cout << "Starting consolidation run." << endl;
    clock_t consolidation_time = clock();
    clock_t graphLoadingTime = 0.0;
    clock_t containsEliminationTime = 0.0;
    clock_t solvingTime = 0.0;
    int iterations = 0;
    do {
        if (contigs->size() == 1)
        {
            cout << "  Only one sequence left." << endl;
            break;
        }
        iterations++;
        sequences = contigs;
        // preprocess sequences here
        cout << "  Eliminating 'contain' duplicates, if any..." << endl;
        cout << "    Number of sequences prior to elimination: "
             << sequences->size() << endl;
        clock_t currentContainsEliminationTime = clock();
        sequences = eliminateContains(*sequences);
        currentContainsEliminationTime = clock() - currentContainsEliminationTime;
        cout << "    Elimination took " <<
            (((double)currentContainsEliminationTime) / CLOCKS_PER_SEC)
            << " seconds, " << currentContainsEliminationTime << " clock ticks."
            << endl;
        containsEliminationTime += currentContainsEliminationTime;
        cout << "    Number of sequences after contains elimination: "
             << sequences->size() << endl;
        cout << "    Done eliminating contain duplicates." << endl;
        cout << "  Merging Overlaps..." << endl;
        IGraphPointer graph(new Graph());
        cout << "    Loading Graph..." << endl;
        // load graph
        clock_t currentGraphLoadingTime = clock();
        for (int i = 0; i < sequences->size(); i++)
        {
            cout << "      Loading Sequence #" << (i + 1) << " into graph..." << endl;
            graph->addSequence((*sequences)[i]);
            cout << "        Done." << endl;
        }
        currentGraphLoadingTime = clock() - currentGraphLoadingTime;
        cout << "    Loading graph took " <<
            (((double)currentGraphLoadingTime)/CLOCKS_PER_SEC) <<
            " seconds, " << currentGraphLoadingTime << " clock ticks." << endl;
        graphLoadingTime += currentGraphLoadingTime;
        cout << "    Done loading Graph." << endl;
        cout << "Getting contigs..." << endl;
        clock_t currentSolvingTime = clock();
        contigs = pathFinder->findPath(graph, fitnessFunction, dm)->getContigs();
        currentSolvingTime = clock() - currentSolvingTime;
        cout << "  Getting contigs took " <<
            (((double)currentSolvingTime) / CLOCKS_PER_SEC) <<
            " seconds, " << currentSolvingTime << " clock ticks." << endl;
        solvingTime += currentSolvingTime;
        cout << "  Done getting contigs.  Found " << contigs->size()
                << " contigs." << endl;
    } while (sequences->size() > contigs->size());
    consolidation_time = clock() - consolidation_time;
    cout << "Done consolidating. Consolidation took " <<
       ( ((double)consolidation_time)/CLOCKS_PER_SEC) << " seconds, "
       << consolidation_time << " clock ticks." << endl;
    cout << "Total contains elimination time: "
         << (((double)containsEliminationTime)/CLOCKS_PER_SEC) << " seconds, "
         << containsEliminationTime << " clock ticks." << endl;
    cout << "Total graph loading time: "
         << (((double)graphLoadingTime)/CLOCKS_PER_SEC) << " seconds, "
         << graphLoadingTime << " clock ticks."
         << endl;
    cout << "Total contig finding time: " << (((double)solvingTime)/CLOCKS_PER_SEC)
         << " seconds, " << solvingTime << " clock ticks." << endl;
    cout << "Total iterations: " << iterations << endl;
    cout << "Total contigs: " << contigs->size() << endl;
    output(outputFile,contigs);

    return 0;
}
