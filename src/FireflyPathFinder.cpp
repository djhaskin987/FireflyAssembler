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

    int populationSize = NUM_FIREFLIES * graph->sequenceCount();
    // generate random paths
	srand(time(0));
    for(int i=0 ; i<populationSize ; i++)
    {
    	vector<int> numbers;
    	for(int i=0 ; i<graph->sequenceCount() ; i++)
    		numbers.push_back(i);

    	random_shuffle(numbers.begin(), numbers.end());

        PathPointer path = PathPointer(new Path(graph, numbers));
        path->setRating(ff->rate(*path));
    	fireflies.push_back(path);
    }
    int majority = (populationSize / 2) + 1;

    int equalFireflies = 0;
    int neededEqualities = (((graph->sequenceCount() * graph->sequenceCount()) -
        graph->sequenceCount()) * 2) / 3;
    for(int i=0 ; equalFireflies < neededEqualities && i<NUM_ITERATIONS ; i++)
    {
        equalFireflies = 0;

        for(vector<PathPointer>::iterator f1 = fireflies.begin();
                f1 != fireflies.end(); ++f1)
        {
            for(vector<PathPointer>::iterator f2 = fireflies.begin();
                    f2 != fireflies.end(); ++f2)
            {
                if (f1 != f2) {
                    double distance = dm->distance(**f1, **f2)+.0001;
                    double intensity = (*f2)->getRating()-(*f1)->getRating();
                    int movement = (int)floor(intensity/distance*MOVEMENT_RATE);
                    int moves_completed = 0;
                    int j=1;
                    while( j < graph->sequenceCount() && moves_completed < movement )
                    {
                        if((**f1)[j] != (**f2)[j] 
                            && (*f2)->getGraph()->hasOverlap(j-1, j)  
                            && (!(*f1)->getGraph()->hasOverlap(j-1, j) || (*f2)->getGraph()->getOverlap(j-1, j) > (*f1)->getGraph()->hasOverlap(j-1, j)))
                        {
                            (*f1)->swapSequences(j, (**f2)[j]);
                            (*f1)->setRating(ff->rate(**f1));
                            moves_completed++;
                        }
                        j++;
                    }
                    if (**f1 == **f2)
                    {
                        equalFireflies++;
                    }
                }
            }
        }
    }
    printFireflies();
    vector<PathPointer>::iterator fittest = fireflies.begin();
    double fittest_rating = ff->rate(**fittest);
    for(vector<PathPointer>::iterator it = fireflies.begin() ; it != fireflies.end() ; ++it)
        if (ff->rate(**it) > fittest_rating)
        {
            fittest_rating = ff->rate(**it);
            fittest = it;
        }

	return *fittest;
}

void FireflyPathFinder::printFireflies()
{
	for(vector<PathPointer>::iterator firefly = fireflies.begin(); firefly != fireflies.end(); ++firefly)
    {
		cout << **firefly << "(rating: " << ff->rate(**firefly) << ") (contigs: " << (*firefly)->getContigs()->size() << ")" << endl;
    }
    cout << *fireflies[7] << endl;
}

