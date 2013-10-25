#include "Graph.hpp"
#include <stdexcept>
using namespace FireflyAssembler;
using namespace std;


void Graph::free()
{

}

void Graph::copy(const Graph & other)
{
    sequences = other.sequences;
    overlaps = other.overlaps;
}

Graph::Graph()
{
}

Graph::Graph(const Graph & other)
{
    copy(other);
}

Graph & Graph::operator = (const Graph & other)
{
    if (this != &other)
    {
        free();
        copy(other);
    }
}

Graph::~Graph()
{
    free();
}

int Graph::sequenceCount() const
{
    return sequences.size();
}

void Graph::getSequence(Sequence & seq, int index) const
{
    seq.clear();
    seq.append(sequences[index]);
}

bool Graph::hasOverlapsFor(int first) const
{
    unordered_map<int,unordered_map<int,int> >::const_iterator firstIter =
        overlaps.find(first);
    if (firstIter == overlaps.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Graph::hasOverlap(int first, int second) const
{
    unordered_map<int,unordered_map<int,int> >::const_iterator firstIter =
        overlaps.find(first);
    if (firstIter == overlaps.end())
    {
        return false;
    }
    else
    {
        unordered_map<int,int>::const_iterator secondIter =
            (firstIter->second).find(second);
        if (secondIter == (firstIter->second).end())
        {
            return false;
        }
        return true;
    }

}

int Graph::getOverlap(int first, int second) const
{
    unordered_map<int,unordered_map<int,int> >::const_iterator firstIter =
        overlaps.find(first);
    if (firstIter == overlaps.end())
    {
        throw runtime_error("No such overlap");
    }
    else
    {
        unordered_map<int,int>::const_iterator secondIter =
            (firstIter->second).find(second);
        if (secondIter == (firstIter->second).end())
        {
            throw runtime_error("No such overlap");
        }
        return secondIter->second;
    }

}

const std::unordered_map<int,int> & Graph::getOverlapsFor(int first) const
{
    unordered_map<int,unordered_map<int,int> >::const_iterator firstIter =
        overlaps.find(first);
    if (firstIter == overlaps.end())
    {
        throw runtime_error("No such overlap index");
    }
    else
    {
        return firstIter->second;
    }
}

void Graph::addOverlap(int aIndex, int bIndex)
{
    int overlap = sequences[aIndex].determineOverlap(sequences[bIndex]);
    if (overlap > 0)
    {
        overlaps[aIndex][bIndex] = overlap;
    }
}

void Graph::addSequence(const Sequence & sequence)
{
    int newIndex = sequences.size();
    sequences.push_back(sequence);

    for (int i = 0; i < newIndex; i++)
    {
        addOverlap(i,newIndex);
        addOverlap(newIndex,i);
    }
}
