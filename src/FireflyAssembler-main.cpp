#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <ctime>
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

SequenceVectorPointer deserializeSequences(string fileName)
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
            sequences->push_back(FireflyAssembler::Sequence());
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
        << "        Specify path finder (default is greedy)" << endl;
    exit(exitCode);
}

void getArgs(distance_type & distanceMeasure,
        fitness_function_type & ffType,
        path_finder_type & pfType,
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
    string inputFile = "";
    string outputFile = "";
    distance_type distanceMeasure = HAMMING;
    fitness_function_type ffType = MEAN_OVERLAP;
    path_finder_type pfType = GREEDY;
    getArgs(distanceMeasure,
            ffType,
            pfType,
            inputFile,
            outputFile,
            argc,
            argv);

    DistanceMetricPointer dm;
    switch (distanceMeasure)
    {
        case HAMMING:
            dm.reset(new HammingDistance());
            break;
        default:
            throw logic_error("internal state is faulty. Cannot continue.");
            break;
    };
    FitnessFunctionPointer fitnessFunction;
    switch (ffType)
    {
        case N50:
            fitnessFunction.reset(new N50Rating());
            break;
        case MEAN_OVERLAP:
            fitnessFunction.reset(new MeanOverlap());
            break;
        default:
            throw logic_error("internal state is faulty. Cannot continue.");
            break;
    };
    PathFinderPointer pathFinder;
    switch (pfType)
    {
        case FIREFLY:
        	pathFinder.reset(new FireflyPathFinder());
        	break;
        case GREEDY:
            pathFinder.reset(new GreedyPathFinder());
            break;
        case LOCAL:
            pathFinder.reset(new LocalSearchPathFinder());
            break;
        default:
            throw logic_error("internal state is faulty. Cannot continue.");
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
    SequenceVectorPointer
        contigs(deserializeSequences(inputFile));

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
            (((double)currentContainsEliminationTime) / CLOCKS_PER_SEC) <<
            " seconds." << endl;
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
            " seconds." << endl;
        graphLoadingTime += currentGraphLoadingTime;
        cout << "    Done loading Graph." << endl;
        cout << "Getting contigs..." << endl;
        clock_t currentSolvingTime = clock();
        contigs = pathFinder->findPath(graph, fitnessFunction, dm)->getContigs();
        currentSolvingTime = clock() - currentSolvingTime;
        cout << "  Getting contigs took " <<
            (((double)currentSolvingTime) / CLOCKS_PER_SEC) <<
            " seconds." << endl;
        solvingTime += currentSolvingTime;
        cout << "  Done getting contigs.  Found " << contigs->size()
                << " contigs." << endl;
    } while (sequences->size() > contigs->size());
    consolidation_time = clock() - consolidation_time;
    cout << "Done consolidating. Consolidation took " <<
       ( ((double)consolidation_time)/CLOCKS_PER_SEC) << " seconds." << endl;
    cout << "Total contains elimination time: " << (((double)containsEliminationTime)/CLOCKS_PER_SEC) << endl;
    cout << "Total graph loading time: " << (((double)graphLoadingTime)/CLOCKS_PER_SEC) << endl;
    cout << "Total contig finding time: " << (((double)solvingTime)/CLOCKS_PER_SEC) << endl;
    cout << "Total iterations: " << iterations << endl;
    output(outputFile,contigs);

    return 0;
}
