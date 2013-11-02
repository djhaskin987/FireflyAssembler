#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include "Types.hpp"
#include "Sequence.hpp"
#include "Graph.hpp"
#include "Path.hpp"
#include "FitnessFunction.hpp"
#include "GreedyPathFinder.hpp"
#include "MeanOverlap.hpp"

using namespace std;
using namespace FireflyAssembler;

VectorPointer<Sequence> deserializeSequences(string fileName)
{
    boost::regex beginningOfSequence("^>.*");
    VectorPointer<Sequence> sequences(new vector<Sequence>());

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
            sequences->push_back(Sequence());
        }
        else
        {
            vector<Sequence>::iterator s = sequences->end() - 1;
            s->append(line);
        }
    }
    in.close();
}

void usage()
{
    cout << "Usage: FireflyAssembler <filename>" << endl
         << "  This prints the contigs found in <filename> to standard out."
            << endl;
};

int main(int argc, char * argv[])
{
    // TODO: Use Boost.program_options here later
    // Add greedy-or-firefly assembler options
    // Add fitness function options
    // Add distance metric options
    if (argc != 1)
    {
        usage();
    }
    string filename(argv[1]);
    VectorPointer<Sequence>
        sequences(deserializeSequences(filename));
    Graph graph;
    // load graph
    for (int i = 0; i < sequences->size(); i++)
    {
        graph.addSequence((*sequences)[i]);
    }

    PathFinderPointer pathFinder(new GreedyPathFinder());
    FitnessFunctionPointer ff(new MeanOverlap());
    VectorPointer<Sequence> contigs =
        pathFinder->findPath(graph, ff)->getContigs();
    for (int j = 0; j < contigs->size(); j++)
    {
        cout << (*contigs)[j] << endl;
    }

    return 0;
}
