#include "FireflyPathFinder.hpp"
#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Types.hpp"

using namespace FireflyAssembler;
using namespace std;

PathPointer FireflyPathFinder::findPath(IGraphConstPointer graph,
        FitnessFunctionPointer ff, DistanceMetricPointer dm)
{
	this->ff = ff;
	PathFinderPointer greedyFinder;
	greedyFinder.reset(new GreedyPathFinder());

//    cout << ff->rate(*greedy) << endl;

    // generate random paths
	srand(time(0));
    for(int i=0 ; i<NUM_FIREFLIES ; i++)
    {
    	vector<int> numbers;
    	for(int i=0 ; i<graph->sequenceCount() ; i++)
    		numbers.push_back(i);

    	random_shuffle(numbers.begin(), numbers.end());

    	fireflies.push_back(PathPointer(new Path(graph, numbers)));
    }

    // fireflies.push_back(greedyFinder->findPath(graph, ff, dm));

    double fittest_rating = -numeric_limits<double>::infinity();
    PathPointer fittest;
    for(int i=0 ; i<NUM_ITERATIONS ; i++)
    {
        for(vector<PathPointer>::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1)
        {
            for(vector<PathPointer>::iterator f2 = fireflies.begin(); f2 != fireflies.end(); ++f2)
            {
                if (f1 != f2) {
                    double distance = dm->distance(**f1, **f2)+.0001;
                    double f1Fitness = ff->rate(**f1);
                    double f2Fitness = ff->rate(**f2);
                    if (f1Fitness > fittest_rating)
                    {
                        fittest_rating = f1Fitness;
                        fittest.reset(new Path(**f1));
                    }
                    double intensity = f2Fitness - f1Fitness;
                    int movement = (int)floor(intensity/distance*MOVEMENT_RATE);
                    int moves_completed = 0;
                    int j=0;
                    while( j<graph->sequenceCount() && moves_completed<movement )
                    {
                        if((**f1)[j] != (**f2)[j])
                        {
                            (*f1)->swapSequences(j, (**f2)[j]);
                            moves_completed++;
                        }
                        j++;
                    }
                }
            }
        }
    }

    for(vector<PathPointer>::iterator it = fireflies.begin() ; it != fireflies.end() ; ++it)
    {
        if (ff->rate(**it) > fittest_rating)
        {
            fittest_rating = ff->rate(**it);
            fittest = *it;
        }
    }
	return fittest;
}

void FireflyPathFinder::printFireflies()
{
	for(vector<PathPointer>::iterator firefly = fireflies.begin(); firefly != fireflies.end(); ++firefly)
    {
		cout << **firefly << "(" << ff->rate(**firefly) << ")" << endl;
    }
    cout << *fireflies[7] << endl;
}
