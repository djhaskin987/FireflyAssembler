#include "LocalSearchPathFinder.hpp"
#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
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
        PathFinderPointer greedy(new GreedyPathFinder());
        PathPointer initial = greedy->findPath(graph,ff,dm);
        vector<int> triedPath;
        for (int i = 0; i < graph->sequenceCount(); i++)
        {
            triedPath.push_back((*initial)[i]);
        }
        cout << "Graph size: " << graph->sequenceCount() << endl;
        cout << "path size: " << triedPath.size() << endl;

        double currentScore = ff->rate(Path(graph, triedPath));
        double bestScore = -numeric_limits<double>::infinity();
        while (currentScore > bestScore)
        {
            bestScore = currentScore;
            bool found = false;
            for (int i = 0; i < graph->sequenceCount(); i++)
            {
                for (int j = i + 1; j < graph->sequenceCount(); j++)
                {
                    int tmp = triedPath[i];
                    triedPath[i] = triedPath[j];
                    triedPath[j] = tmp;
                    double score = ff->rate(Path(graph,triedPath));
                    if (score > currentScore)
                    {
                        found = true;
                        currentScore = score;
                        break;
                    }
                    triedPath[j] = triedPath[i];
                    triedPath[i] = tmp;
                }
                if (found)
                {
                    break;
                }
            }
        }
        return PathPointer(new Path(graph, triedPath));
    }
}

