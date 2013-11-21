#include "Graph.hpp"
#include <stdexcept>
#include <sstream>
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

const Sequence & Graph::getSequence(int index) const
{
    return sequences[index];
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
        return (firstIter->second).size() > 0;
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
    if (firstIter == overlaps.end() || (firstIter->second).size() == 0)
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
    if (aIndex == bIndex)
    {
        stringstream ss;
        ss << "Overlaps should never be looked for from one index ('"
            << aIndex << "') to itself ('" << bIndex << "')." << endl;
        throw std::logic_error(ss.str());
    }
    else
    {
        int overlap = sequences[aIndex].determineOverlap(sequences[bIndex]);
        if (overlap > 0)
        {
            overlaps[aIndex][bIndex] = overlap;
        }
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

HashSetPointer<int> Graph::getSources() const
{
    HashSetPointer<int> s(new HashSet<int>());

    for (int tested = 0; tested < sequences.size(); tested++)
    {
        bool isSource = true;
        for (int index = 0; index < sequences.size(); index++)
        {
            if (tested == index)
            {
                continue;
            }
            isSource = isSource && !hasOverlap(index, tested);
            if (!isSource)
            {
                break;
            }
        }
        if (isSource)
        {
            s->insert(tested);
        }
    }
    return s;
}

HashSetPointer<int> Graph::getSinks() const
{
    HashSetPointer<int> s(new HashSet<int>());

    for (int tested = 0; tested < sequences.size(); tested++)
    {
        if (!hasOverlapsFor(tested))
        {
            s->insert(tested);
        }
    }
    return s;
}
