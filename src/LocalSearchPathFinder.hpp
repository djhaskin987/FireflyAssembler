#ifndef __FireflyAssemblerLocalSearchPathFinder__
#define __FireflyAssemblerLocalSearchPathFinder__
#include "Types.hpp"
#include "PathFinder.hpp"

namespace FireflyAssembler
{
    class LocalSearchPathFinder : public PathFinder
    {
        public:
            virtual PathPointer findPath(IGraphConstPointer graph,
                    FitnessFunctionPointer ff);
    };
}

#endif
