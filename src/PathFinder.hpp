#ifndef __FireflyAssemblerPathFinder__
#define __FireflyAssemblerPathFinder__

#include "Types.hpp"
#include "Path.hpp"
#include "IGraph.hpp"
#include "FitnessFunction.hpp"

namespace FireflyAssembler
{
    class PathFinder
    {
        public:
            virtual FireflyAssembler::PathPointer findPath(IGraphConstPointer graph,
                    FitnessFunctionPointer ff) = 0;
            virtual ~PathFinder() {}
    };
    typedef std::shared_ptr<PathFinder>
        PathFinderPointer;
}
#endif
