#include "Path.hpp"
#include <sstream>
#include <stdexcept>
#include <unordered_set>

using namespace FireflyAssembler;
using namespace std;

void Path::free()
{
    pathGraph = NULL;

}

void Path::copy(const Path & other)
{
    pathGraph = other.pathGraph;
    path = other.path;
}

void Path::validatePath() const
{
    if (pathGraph == NULL)
    {
        throw logic_error("path's graph pointer is null.");
    }

    if (path.size() != pathGraph->sequenceCount())
    {
        throw logic_error("Path length must equal graph sequence count");
    }
    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] < 0 || path[i] >= pathGraph->sequenceCount())
        {
            stringstream ss;
            ss << "Path at index '" << i << "' contains invalid index '"
                << path[i] << "' for given graph." << endl;

            throw out_of_range(ss.str());
        }
    }
    unordered_set<int> pathIndexes(path.begin(), path.end());
    if (pathIndexes.size() != path.size())
    {
        throw logic_error("Each index must be visited in the graph exactly once");
    }

}

Path::Path(const Path & other)
{
    copy(other);
}

Path & Path::operator = (const Path & other)
{
    if (this != &other)
    {
        free();
        copy(other);
    }
}

Path::~Path()
{
    free();
}

const Graph & Path::getGraph() const
{
    return *pathGraph;
}

int Path::size() const
{
    return path.size();
}

int Path::operator [] (int index) const
{
    return path[index];
}

VectorPointer<Sequence> Path::getContigs() const
{
    VectorPointer<Sequence> ss(new vector<Sequence>());
    if (path.size() <= 0)
    {
        return ss;
    }
    int currentIndex = 0;
    ss->push_back(pathGraph->getSequence(path[0]));
    for (int index = 1; index < path.size(); index++)
    {
        const Sequence & nextSequence = pathGraph->getSequence(index);
        if (pathGraph->hasOverlap(path[index-1],path[index]))
        {
            (*ss)[currentIndex].merge(nextSequence,
                    pathGraph->getOverlap(path[index]-1,path[index]));
        }
        else
        {
            ss->push_back(nextSequence);
            currentIndex++;
        }
    }
    return ss;
}

ostream & operator << (ostream & s, const Path & p)
{
    if (p.size() == 0)
    {
        s << "<empty-path>";
    }
    else
    {
        s << "( ";
        s << p[0];
        for (int i = 1; i < p.size(); i++)
        {
            s << " -> " << p[i];
        }
        s << " )";
    }
    return s;
}
