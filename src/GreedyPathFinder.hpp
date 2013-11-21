#ifndef __FireflyAssemblerGreedyPathFinder__
#define __FireflyAssemblerGreedyPathFinder__
#include "Types.hpp"
#include "PathFinder.hpp"
#include <list>

namespace FireflyAssembler
{
    class GreedyPathFinder : public PathFinder
    {
        private:
            VectorPointer<int> getPathFromMap(const HashMap<int,int> & map, int start);
            std::list<HashSet<int> >::iterator getLeg(int point,
                    std::list<HashSet<int> > & legs);

        public:
            virtual PathPointer findPath(IGraphConstPointer graph,
                    FitnessFunctionPointer ff);
    };
}

#endif
