#include "FireflyAssemblerGraph.hpp"

using namespace FireflyAssembler;

void Graph::init()
{

}

void Graph::free()
{

}

void Graph::copy()
{

}

Graph::Graph()
{
}

Graph::Graph(const Graph & other)
{
    copy(other);
}

Graph & Graph::operator = Graph(const Graph & other)
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
