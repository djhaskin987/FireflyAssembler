#include <iostream>
#include "MeanOverlap.hpp"

using namespace FireflyAssembler;
using namespace std;

double MeanOverlap::getOverlapPercentage(const Path & path, int a, int b) const
{
    double returned = 0.0;
    IGraphConstPointer g = path.getGraph();

    if (g->hasOverlap(a,b))
    {
        returned = (double)g->getOverlap(a,b);
        // cout << g->sequenceCount() << endl;
    }
    return returned;
}


double MeanOverlap::rate(const Path & path) const
{
    double meanOverlap = 0.0;
    for (int i = 1; i < path.size(); i++)
    {
        meanOverlap += getOverlapPercentage(path, path[i-1], path[i]);
        // cout << path[i-1] << ", " << path[i] << ", " << meanOverlap << endl;
    }
    meanOverlap /= (double)(path.size()-1);
    return meanOverlap;
}
