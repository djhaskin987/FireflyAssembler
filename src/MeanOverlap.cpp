#include <iostream>
#include "MeanOverlap.hpp"

using namespace FireflyAssembler;
using namespace std;

double MeanOverlap::rate(const Path & path) const
{
    IGraphConstPointer graph = path.getGraph();
    double meanOverlap = 0.0;
    for (int i = 1; i < path.size(); i++)
    {
        if (graph->hasOverlap(path[i-1],path[i]))
        {
            meanOverlap += ((double)graph->getOverlap(path[i-1],path[i]));
        }
    }
    meanOverlap /= (double)(path.size()-1);
    return meanOverlap;
}
