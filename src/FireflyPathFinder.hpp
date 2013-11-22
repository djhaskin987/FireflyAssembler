/*
 * FireflyPathFinder.hpp
 *
 *  Created on: Nov 19, 2013
 *      Author: kyle
 */

#ifndef FIREFLYPATHFINDER_HPP_
#define FIREFLYPATHFINDER_HPP_

#include "Types.hpp"
#include "PathFinder.hpp"
#include <vector>

#define NUM_FIREFLIES 20
#define NUM_ITERATIONS 100
#define MOVEMENT_RATE 10

namespace FireflyAssembler
{
    class FireflyPathFinder : public PathFinder
    {
    	private:
    		std::vector<PathPointer> fireflies;
    		FitnessFunctionPointer ff;
    		void printFireflies();
        public:
            virtual PathPointer findPath(IGraphConstPointer graph,
                    FitnessFunctionPointer ff, DistanceMetricPointer dm);
    };
}


#endif /* FIREFLYPATHFINDER_HPP_ */
