#include "FireflyAssemblerPath.hpp"

using namespace FireflyAssembler;

void Path::free()
{
    // we don't "own" this pointer
    graph = NULL;
}

void Path::copy(const Path & other)
{
    // shallow copy of the graph,
    // deep copy of the path
    graph = other.graph;
    path = other.path;
    verifyPath();
}

Path::Path(Graph * g, vector<int> p) :
    graph(g), path(p)
{
    verifyPath();
}

void Path::verifyPath()
{
    // Verify the path length is the graph size
    if (path.size() != graph->sequenceCount())
    {
        throw new runtime_error("Need to specify a path for the given graph");
    }
    set<int> visited;
    // Verify path specifies legitimate nodes in the graph and that
    // each node is visited.
    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] >= graph->sequenceCount())
        {
            throw new runtime_error("Path must specify nodes in the graph!");
        }
        if (visited.find(path[i]) != visited.end())
        {
            throw new runtime_error("Path must not visit a node twice!");
        }
        visited.insert(path[i]);
    }
}

Path::Path(const Path & other)
{
    copy(other);
}

Path & Path::operator = Path(const Path & other)
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

void Path::assemble(Sequence & seq)
{
    seq.clear();

    // TODO
}

int Path::getSequenceIndex(int pathIndex)
{
    return path[pathIndex];
}

int Path::size()
{
    return path.size();
}
