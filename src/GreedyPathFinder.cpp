#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
#include "Types.hpp"

using namespace FireflyAssembler;
using namespace std;

PathPointer GreedyPathFinder::findPath(IGraphConstPointer graph,
        FitnessFunctionPointer ff)
{
    cout << "Graph size: " << graph->sequenceCount() << endl;
    cout << flush;
    if (graph->sequenceCount() == 1)
    {
        vector<int> path(1);
        path.push_back(0);
        PathPointer returned(new Path(graph, path));
        return returned;
    }
    else
    {
        priority_queue<pair<int,pair<int,int> > >
            edgeQueue;
        for (int a = 0; a < graph->sequenceCount(); a++)
        {
            for (int b = 0; b < graph->sequenceCount(); b++)
            {
                if (a == b) continue;
                if (graph->hasOverlap(a,b))
                {
                    edgeQueue.push(make_pair(graph->getOverlap(a,b),
                                make_pair(a,b)));
                }
                else
                {
                    edgeQueue.push(make_pair(0,
                                make_pair(a,b)));
                }
            }
        }
        HashSet<int> fromVisited;
        HashSet<int> toVisited;
        HashMap<int,int> solution;

        while (edgeQueue.size() > 0)
        {
            pair<int,pair<int,int> > queueTop = edgeQueue.top();

            int overlap = queueTop.first;
            int from = queueTop.second.first;
            int to = queueTop.second.second;
                // If I haven't come from there yet...
            if (fromVisited.find(from) == fromVisited.end() &&
                    // and I've not already visited there...
                    toVisited.find(to) == toVisited.end() &&
                    // and either I'm going nowhere I've been, or
                    // I'm coming from somewhere I've not seen yet...
                    (fromVisited.find(to) == fromVisited.end() ||
                     toVisited.find(from) == toVisited.end()))
            {
                // then I must be an edge on a hamiltonian path
                fromVisited.insert(from);
                toVisited.insert(to);
                solution.insert(make_pair(from,to));
            }
            edgeQueue.pop();
        }

        // Find the last edge (connect the path)
        int from = -1;
        int to = -1;

        for (int i = 0; i < graph->sequenceCount(); i++)
        {
            if (fromVisited.find(i) == fromVisited.end())
            {
                from = i;
            }
            else if (toVisited.find(i) == toVisited.end())
            {
                to = i;
            }
        }
        solution.insert(make_pair(from, to));
        // solution is now fully populated with a greedy solution. Now to find
        // where to start.

        PathPointer bestPath;
        double maxScore = -numeric_limits<double>::infinity();
        for (int i = 0; i < graph->sequenceCount(); i++)
        {
            VectorPointer<int> pathIndices = getPathFromMap(solution, i);

            PathPointer currentPath(new Path(graph, *pathIndices));
            double currentScore = ff->rate(*currentPath);
            if (currentScore > maxScore)
            {
                maxScore = currentScore;
                bestPath = currentPath;
            }
        }
        return bestPath;
    }
}

VectorPointer<int> GreedyPathFinder::getPathFromMap(const HashMap<int,int> & map, int start)
{
    int pathSize = map.size();
    VectorPointer<int> vecPtr(new vector<int>());
    int currentIndex = start;
    for (int i = 0; i < pathSize; i++)
    {
        vecPtr->push_back(currentIndex);
        currentIndex = map.find(currentIndex)->second;
    }
    return vecPtr;
}

