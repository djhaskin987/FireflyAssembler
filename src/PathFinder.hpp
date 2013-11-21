#ifndef __FireflyAssemblerPathFinder__
#define __FireflyAssemblerPathFinder__

#include "Types.hpp"
#include "Path.hpp"
#include "IGraph.hpp"
#include "FitnessFunction.hpp"
#include "DistanceMetric.hpp"

namespace FireflyAssembler
{
    class PathFinder
    {
        public:
            virtual FireflyAssembler::PathPointer findPath(IGraphConstPointer graph,
                    FitnessFunctionPointer ff, DistanceMetricPointer dm) = 0;
            virtual ~PathFinder() {}
    };
    typedef std::shared_ptr<PathFinder>
        PathFinderPointer;
}
#endif
