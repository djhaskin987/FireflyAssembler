#include "Path.hpp"
#include "FitnessFunction.hpp"
#include <sstream>
#include <stdexcept>
#include <unordered_set>

using namespace FireflyAssembler;
using namespace std;

void Path::free()
{
}

void Path::copy(const Path & other)
{
    pathGraph = other.pathGraph;
    path = other.path;
}

void Path::validatePath() const
{
    if (pathGraph.get() == NULL)
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
    HashSet<int> uniqueIndices(path.begin(),path.end());
    if (uniqueIndices.size() < path.size())
    {
        throw runtime_error("Each path must have unique indices");
    }
}

Path::Path(IGraphConstPointer g,
        const std::vector<int> & p) : pathGraph(g),
                    path(p)
{
    validatePath();
}

Path::Path(const Path & other) : pathGraph(other.pathGraph),
        path(other.path)
{
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

IGraphConstPointer Path::getGraph() const
{
    return pathGraph;
}

int Path::size() const
{
    return path.size();
}

int Path::operator [] (int index) const
{
    return path[index];
}
bool Path::operator ==(const Path & other)
{
    // Same sequence, same graph
    return path == other.path &&
        pathGraph.get() == other.pathGraph.get();
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
        const Sequence & nextSequence = pathGraph->getSequence(path[index]);
        if (pathGraph->hasOverlap(path[index-1],path[index]))
        {
            (*ss)[currentIndex].merge(nextSequence,
                    pathGraph->getOverlap(path[index-1],path[index]));
        }
        else
        {
            ss->push_back(nextSequence);
            currentIndex++;
        }
    }
    return ss;
}

void Path::setRating(double rating)
{
	this->rating = rating;
}

double Path::getRating()
{
	return rating;
}

void Path::swapSequences(int index, int new_sequence)
{
    int new_index = 0;
    for( int i=0 ; i<path.size() ; i++)
        if (path[i] == new_sequence)
            new_index = i;

    path[new_index] = path[index];
    path[index] = new_sequence;
}

ostream & FireflyAssembler::operator << (ostream & s, const Path & p)
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
