#include "LocalSearchPathFinder.hpp"
#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Types.hpp"

using namespace FireflyAssembler;
using namespace std;

PathPointer LocalSearchPathFinder::findPath(IGraphConstPointer graph,
        FitnessFunctionPointer ff, DistanceMetricPointer dm)
{
    if (graph->sequenceCount() == 1)
    {
        vector<int> path(1);
        path.push_back(0);
        PathPointer returned(new Path(graph, path));
        return returned;
    }
    else
    {
        VectorPointer<int> initialNodes(new vector<int>());

        srand(time(0));
        for(int i=0 ; i<graph->sequenceCount() ; i++)
            initialNodes->push_back(i);
        random_shuffle(initialNodes->begin(), initialNodes->end());

        PathPointer initial(new Path(graph,*initialNodes));
        VectorPointer<int> currentNodes = initialNodes;
        VectorPointer<int> triedNodes(new vector<int>(initialNodes->begin(),
                    initialNodes->end()));

        double currentScore = ff->rate(Path(graph, *currentNodes));
        double bestScore = -numeric_limits<double>::infinity();
        while (currentScore > bestScore)
        {
            bestScore = currentScore;
            bool found = false;
            for (int i = 0; i < graph->sequenceCount(); i++)
            {
                for (int j = i + 1; j < graph->sequenceCount(); j++)
                {
                    swap((*triedNodes)[i],(*triedNodes)[j]);
                    double score = ff->rate(Path(graph,*triedNodes));
                    if (score > currentScore)
                    {
                        found = true;
                        currentScore = score;
                        currentNodes = triedNodes;
                        triedNodes.reset(new vector<int>(currentNodes->begin(),
                            currentNodes->end()));
                        break;
                    }
                    swap((*triedNodes)[i],(*triedNodes)[j]);
                }
                if (found)
                {
                    break;
                }
            }
        }
        return PathPointer(new Path(graph, *currentNodes));
    }
}

