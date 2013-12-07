#include "FireflyPathFinder.hpp"
#include "GreedyPathFinder.hpp"
#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Types.hpp"

using namespace FireflyAssembler;
using namespace std;

int FireflyPathFinder::bitSizeOf(int number)
{
    int mask = 1UL;
    int size = 0;
    for (int i = 0; mask < number && i < sizeof(int) * 8; i++)
    {
        mask <<= 1;
        size++;
    }
    return size;
}


PathPointer FireflyPathFinder::findPath(IGraphConstPointer graph,
        FitnessFunctionPointer ff, DistanceMetricPointer dm)
{
	this->ff = ff;
	PathFinderPointer greedyFinder;
	greedyFinder.reset(new GreedyPathFinder());
    fireflies.clear();

    // generate random paths
    double fittest_rating = -numeric_limits<double>::infinity();
    PathPointer fittest;
	srand(time(0));
    int fireflyCount = NUM_FIREFLIES;
    int bits = bitSizeOf(graph->sequenceCount());
    fireflyCount *= (bits * bits);
    for(int i=0 ; i<fireflyCount ; i++)
    {
    	vector<int> numbers;
    	for(int i=0 ; i<graph->sequenceCount() ; i++)
    		numbers.push_back(i);

    	random_shuffle(numbers.begin(), numbers.end());
        PathPointer newPath(new Path(graph, numbers));
        double fitness = ff->rate(*newPath);
    	fireflies.push_back(
                make_pair(newPath,fitness));
        if (fitness > fittest_rating)
        {
            fittest_rating = fitness;
            fittest.reset(new Path(*newPath));
        }
    }

    // fireflies.push_back(greedyFinder->findPath(graph, ff, dm));

    int iterations = NUM_ITERATIONS * graph->sequenceCount();
    for(int i=0 ; i< iterations ; i++)
    {
        for(vector<pair<PathPointer,double> >::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1)
        {
            double maxIntensity = -numeric_limits<double>::infinity();
            PathPointer maxIntensityFirefly;

            for(vector<pair<PathPointer, double> >::iterator f2 = fireflies.begin(); f2 != fireflies.end(); ++f2)
            {
                if (f1 != f2) {
                    double distance = dm->distance(*(f1->first), *(f2->first))+1.0;
                    double f1Fitness = f1->second;
                    double f2Fitness = f2->second;
                    if (f1Fitness > fittest_rating)
                    {
                        fittest_rating = f1Fitness;
                        fittest.reset(new Path(*(f1->first)));
                    }
                    double intensity = (f2Fitness - f1Fitness) / distance;
                    if (intensity > maxIntensity)
                    {
                        maxIntensity = intensity;
                        maxIntensityFirefly = f2->first;
                    }
                }

            }
            for (int j = 0; j < graph->sequenceCount(); j++)
            {
                if((*(f1->first))[j] != (*maxIntensityFirefly)[j])
                {
                    (f1->first)->swapSequences(j, (*maxIntensityFirefly)[j]);
                    f1->second = ff->rate(*(f1->first));
                    if (f1->second > fittest_rating)
                    {
                        fittest_rating = f1->second;
                        fittest.reset(new Path(*(f1->first)));
                    }
                    break;
                }
            }
        }
    }

	return fittest;
}

void FireflyPathFinder::printFireflies()
{
	for(vector<pair<PathPointer,double> >::iterator firefly = fireflies.begin(); firefly != fireflies.end(); ++firefly)
    {
		cout << *(firefly->first) << "(" << firefly->second << ")"
             << "{" << firefly->first->getGraph()->sequenceCount() << "}" << endl;
    }
}
