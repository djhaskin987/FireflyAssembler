#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include "Sequence.hpp"
#include "Types.hpp"
#include "Graph.hpp"
#include "Path.hpp"
#include "FitnessFunction.hpp"
#include "GreedyPathFinder.hpp"
#include "MeanOverlap.hpp"

using namespace std;
using namespace FireflyAssembler;
using namespace boost;
typedef VectorPointer<FireflyAssembler::Sequence>
    SequenceVectorPointer;
typedef vector<FireflyAssembler::Sequence> SequenceVector;
using Sequence = FireflyAssembler::Sequence;


SequenceVectorPointer deserializeSequences(string fileName)
{
    boost::regex beginningOfSequence("^>.*");
    SequenceVectorPointer sequences(new SequenceVector());

    // Open file and create RecordReader.
    ifstream in(fileName.c_str());
    if (!in.is_open())
    {
        cerr << "Problem occurred in opening file." << endl;
    }

    // Read file record-wise.
    string line;

    while (!in.eof())
    {
        getline(in,line);
        if (boost::regex_match(line, beginningOfSequence))
        {
            sequences->push_back(FireflyAssembler::Sequence());
        }
        else
        {
            SequenceVector::iterator s = sequences->end() - 1;
            s->append(line);
        }
    }
    in.close();
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
    FIREFLY
} path_finder_type;

void usage(int exitCode)
{
    cout << "Usage: FireflyAssembler [<options>...] <input-fasta-file> <output-file>" << endl
        << "  Where:" << endl
        << "    -d, --distance-metric {cayley|hamming}" << endl
        << "        Specify distance metric (default is hamming)" << endl
        << "    -f, --fitness-function {meanoverlap|contiglength}" << endl
        << "        Specify fitness funciton (default is 'meanoverlap')" << endl
        << "    -p, --path-finder {greedy|firefly}" << endl
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
    if (argc < 3)
    {
        cerr << "Not enough arguments." << endl;
        usage(1);
    }
    int currentArgIndex = 1;
    cmatch parts;
    regex keyValOption("^([^=]*)=([^=]*)$");
    cmatch key_val_parts;
    while (currentArgIndex != argc)
    {
        string key;
        string val;
        if (regex_match(argv[currentArgIndex], key_val_parts, keyValOption))
        {
            key = key_val_parts[1];
            val = key_val_parts[2];
        }
        else
        {
            key = argv[currentArgIndex];
            if (currentArgIndex + 1 < argc)
            {
                val = argv[currentArgIndex + 1];
            }
            else
            {
                val = "";
            }
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
}



SequenceVectorPointer
    eliminateContains(const SequenceVector & sequences)
{

    SequenceVectorPointer returned(new SequenceVector(sequences));
    for (int i = 0; i < sequences.size(); i++)
    {
        bool isIContainedAnywhere = false;
        for (int j = 0; j < sequences.size(); j++)
        {
            if (i == j)
                continue;
            if (sequences[i].containsSize(sequences[j]) > 0)
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
    file << contigs;
    for (int j = 0; j < contigs->size(); j++)
    {
        cout << (*contigs)[j] << endl;
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
    do {
        sequences = contigs;
        // preprocess sequences here
        cout << "Eliminating 'contain' duplicates, if any..." << endl;
        sequences = eliminateContains(*sequences);
        cout << "  Done eliminating contain duplicates." << endl;
        cout << "Merging Overlaps..." << endl;
        Graph graph;
        cout << "  Loading Graph..." << endl;
        // load graph
        for (int i = 0; i < sequences->size(); i++)
        {
            cout << "    Loading Sequence #" << (i + 1) << " into graph..." << endl;
            graph.addSequence((*sequences)[i]);
            cout << "      Done." << endl;
        }
        cout << "    Done loading Graph." << endl;
        cout << "  Getting contigs..." << endl;
        contigs = pathFinder->findPath(graph, fitnessFunction)->getContigs();
        cout << "    Done getting contigs.  Found " << contigs->size()
                << " contigs." << endl;
    } while (sequences->size() > contigs->size());

    output(outputFile,contigs);

    return 0;
}
