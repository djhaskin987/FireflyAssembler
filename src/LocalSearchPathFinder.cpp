#include "LocalSearchPathFinder.hpp"
#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
#include <random>
#include "Types.hpp"

using namespace FireflyAssembler;
using namespace std;

PathPointer LocalSearchPathFinder::findPath(IGraphConstPointer graph,
        FitnessFunctionPointer ff)
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
        vector<int> path;
        HashSet<int> visited;
        default_random_engine generator;
        uniform_int_distribution<int> distribution(0,graph->sequenceCount()-1);
        auto random = bind(distribution, generator);
        while (visited.size() < graph->sequenceCount())
        {
            int node = random();
            if (visited.find(node) == visited.end())
            {
                visited.insert(node);
                path.push_back(node);
            }
        }
        PathPointer initial(new Path(graph, path));

        //PathFinderPointer greedy(new GreedyPathFinder());
        //PathPointer initial = greedy->findPath(graph,ff);
        //for (int i = 0; i < graph->sequenceCount(); i++)
        //{
        //    path.push_back((*initial)[i]);
        //}

        double currentScore = ff->rate(*initial);
        double bestScore = -numeric_limits<double>::infinity();
        vector<int> currentPath = path;
        while (currentScore > bestScore)
        {
            bestScore = currentScore;
            for (int i = 1; i < graph->sequenceCount()-1; i++)
            {
                vector<int> triedPath(currentPath.begin()+i,currentPath.end());
                triedPath.insert(triedPath.end(),currentPath.begin(),
                        currentPath.begin()+i);
                double score = ff->rate(Path(graph,triedPath));
                if (score > bestScore)
                {
                    cout << "Improved!" << endl;
                    currentScore = score;
                    currentPath = triedPath;
                    break;
                }
            }
        }
        return PathPointer(new Path(graph, currentPath));
    }
}

