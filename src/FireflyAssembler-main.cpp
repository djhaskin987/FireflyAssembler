#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "Sequence.hpp"
#include "Types.hpp"
#include "Graph.hpp"
#include "Path.hpp"
#include "FitnessFunction.hpp"
#include "GreedyPathFinder.hpp"
#include "LocalSearchPathFinder.hpp"
#include "MeanOverlap.hpp"
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
    CAYLEY, HAMMING
} distance_type;

typedef enum
{
    MEAN_OVERLAP,
    CONTIG_LENGTH
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
        << "    -d, --distance-metric {cayley|hamming}" << endl
        << "        Specify distance metric (default is hamming)" << endl
        << "    -f, --fitness-function {meanoverlap|contiglength}" << endl
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
            if (val == "cayley")
            {
                distanceMeasure = CAYLEY;
            }
            else if (val == "hamming")
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
            else if (val == "contiglength")
            {
                ffType = CONTIG_LENGTH;
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

/*
    DistanceMetricPointer dm;
    switch (distanceMeasure)
    {
        case HAMMING:
            dm = new HammingDistanceMetric();
            break;
        case CAYLEY:
            dm = new CayleyDistanceMetric();
            break;
        default:
            throw logic_error("internal state is faulty. Cannot continue.");
            break;
    };
    */
    FitnessFunctionPointer fitnessFunction;
    switch (ffType)
    {
        // TODO: Fully implement this
        case CONTIG_LENGTH:
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
    SequenceVectorPointer
        contigs(deserializeSequences(inputFile));

    SequenceVectorPointer sequences;
    cout << "Starting consolidation run." << endl;
    do {
        if (contigs->size() == 1)
        {
            cout << "  Only one sequence left." << endl;
            break;
        }
        sequences = contigs;
        // preprocess sequences here
        cout << "  Eliminating 'contain' duplicates, if any..." << endl;
        cout << "    Number of sequences prior to elimination: "
             << sequences->size() << endl;
        sequences = eliminateContains(*sequences);
        cout << "    Number of sequences after contains elimination: "
             << sequences->size() << endl;
        cout << "    Done eliminating contain duplicates." << endl;
        cout << "  Merging Overlaps..." << endl;
        IGraphPointer graph(new Graph());
        cout << "    Loading Graph..." << endl;
        // load graph
        for (int i = 0; i < sequences->size(); i++)
        {
            cout << "      Loading Sequence #" << (i + 1) << " into graph..." << endl;
            graph->addSequence((*sequences)[i]);
            cout << "        Done." << endl;
        }
        cout << "      Graph size: " << graph->sequenceCount() << endl;
        cout << "      Done loading Graph." << endl;
        cout << "  Getting contigs..." << endl;

        contigs = pathFinder->findPath(graph, fitnessFunction)->getContigs();
        cout << "  Done getting contigs.  Found " << contigs->size()
                << " contigs." << endl;
    } while (sequences->size() > contigs->size());
    cout << "Done consolidating." << endl;
    output(outputFile,contigs);

    return 0;
}
