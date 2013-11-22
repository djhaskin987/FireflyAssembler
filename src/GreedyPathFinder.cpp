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
        list<HashSet<int> > legs;
        for (int i = 0; i < graph->sequenceCount(); i++)
        {
            HashSet<int> leg;
            leg.insert(i);
            legs.push_back(leg);
        }

        HashMap<int,int> solution;
        int from;
        int to;
        int overlap;

        while (edgeQueue.size() > 0)
        {
            pair<int,pair<int,int> > queueTop = edgeQueue.top();

            overlap = queueTop.first;
            from = queueTop.second.first;
            to = queueTop.second.second;
            list<HashSet<int> >::iterator fromLeg = getLeg(from,legs);
            list<HashSet<int> >::iterator toLeg = getLeg(to,legs);
            if (fromLeg == legs.end())
            {
                throw std::logic_error("'from' not in legs.");
            }
            if (toLeg == legs.end())
            {
                throw std::logic_error("'to' not in legs.");
            }
                // If I haven't come from there yet...
            if (fromVisited.find(from) == fromVisited.end() &&
                    // and I've not already visited there...
                    toVisited.find(to) == toVisited.end() &&
                    // and we're not going in circles...
                    fromLeg != toLeg)
            {
                // then I must be an edge on a hamiltonian path
                fromVisited.insert(from);
                toVisited.insert(to);
                solution.insert(make_pair(from,to));
                fromLeg->insert(toLeg->begin(),toLeg->end());
                legs.erase(toLeg);
            }
            edgeQueue.pop();
        }

        // Find the last edge (connect the path)
        int start = -1;
        int end = -1;

        for (int i = 0; i < graph->sequenceCount(); i++)
        {
            if (fromVisited.find(i) == fromVisited.end())
            {
                end = i;
            }
            else if (toVisited.find(i) == toVisited.end())
            {
                start = i;
            }
        }
        // Complete the TSP tour for our solver.
        solution.insert(make_pair(end, start));
        // solution is now fully populated with a greedy solution. Now to find
        // where to start.
        VectorPointer<int> pathIndices = getPathFromMap(solution, start);

        PathPointer returnedPath(new Path(graph, *pathIndices));

        return returnedPath;
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

list<HashSet<int> >::iterator GreedyPathFinder::getLeg(int point,
        list<HashSet<int> > & legs)
{
    for (list<HashSet<int> >::iterator iter = legs.begin();
            iter != legs.end(); iter++)
    {
        if (iter->find(point) != iter->end())
        {
            return iter;
        }
    }
    return legs.end();
}
