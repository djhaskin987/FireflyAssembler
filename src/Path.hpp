#ifndef __FireflyAssemblerPath__
#define __FireflyAssemblerPath__

#include "FireflyAssemblerGraph.hpp"
#include <stdexcept>

namespace FireflyAssembler
{
    class Path
    {
        private:
            void free();
            void copy(const Path & other);
            void verifyPath() throw(runtime_error);

            Graph * graph;
            vector<int> path;

        public:
            Path(Graph * graph, list<path>);
            Path(const Path & other);
            Path & operator = (const Path & other);

            int getNodeIndex(int pathIndex);
            int size();

            ~Path();
    };
}

#endif
