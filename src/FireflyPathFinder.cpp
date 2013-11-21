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
    	for(vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    		cout << *it << ' ';
    	cout << endl;

    	fireflies.push_back(PathPointer(new Path(graph, numbers)));
    }

    // fireflies.push_back(greedyFinder->findPath(graph, ff, dm));


    for(int i=0 ; i<NUM_ITERATIONS ; i++) 
    {
        for(vector<PathPointer>::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1) 
        {
            for(vector<PathPointer>::iterator f2 = fireflies.begin(); f2 != fireflies.end(); ++f2) 
            {
                if (f1 != f2) {
                    double distance = dm->distance(**f1, **f2)+.0001;
                    double intensity = ff->rate(**f2)-ff->rate(**f1);
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
    printFireflies();


	return NULL;
}

void FireflyPathFinder::printFireflies()
{
	for(vector<PathPointer>::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1)
		cout << ff->rate(**f1) << endl;

}

