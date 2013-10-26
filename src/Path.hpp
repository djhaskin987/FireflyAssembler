#ifndef __FireflyAssemblerPath__
#define __FireflyAssemblerPath__
#include "Sequence.hpp"
#include "Graph.hpp"
#include <ostream>
#include <vector>


namespace FireflyAssembler
{
    class Path
    {
        private:
            const Graph * pathGraph;
            std::vector<int> path;

            void free();
            void copy(const Path & other);

            void validatePath() const;

        public:
            template <typename inputIterator>
            Path(const Graph & g,
                    inputIterator first,
                    inputIterator last);
            Path(const Path & other);

            Path & operator = (const Path & other);

            ~Path();

            const Graph & getGraph() const;
            int size() const;
            int operator [] (int index) const;
            void getContigs(std::vector<Sequence> & ss);
    };
    std::ostream & operator << (std::ostream & s, const Path & p);

    template <typename inputIterator>
    Path::Path(const Graph & g,
                        inputIterator first,
                        inputIterator last) : pathGraph(&g),
                        path(first, last)
    {
    }

}

#endif
