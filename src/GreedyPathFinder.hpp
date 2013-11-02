#ifndef __FireflyAssemblerGreedyPathFinder__
#define __FireflyAssemblerGreedyPathFinder__
#include "Types.hpp"
#include "PathFinder.hpp"

namespace FireflyAssembler
{
    class GreedyPathFinder : public PathFinder
    {
        private:
            VectorPointer<int> getPathFromMap(const HashMap<int,int> & map, int start);

        public:
            virtual PathPointer findPath(const Graph & graph,
                    FitnessFunctionPointer ff);
    };
}

#endif
