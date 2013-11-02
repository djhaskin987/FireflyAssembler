#ifndef __FireflyAssemblerPathFinder__
#define __FireflyAssemblerPathFinder__

#include "Types.hpp"
#include "Path.hpp"
#include "FitnessFunction.hpp"

namespace FireflyAssembler
{
    class PathFinder
    {
        public:
            virtual FireflyAssembler::PathPointer findPath(const Graph & graph,
                    FitnessFunctionPointer ff) = 0;
    };
    typedef std::shared_ptr<PathFinder>
        PathFinderPointer;
}
#endif
