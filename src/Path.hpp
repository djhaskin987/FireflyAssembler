#ifndef __FireflyAssemblerPath__
#define __FireflyAssemblerPath__
#include "Types.hpp"
#include "Sequence.hpp"
#include "IGraph.hpp"
#include <ostream>
#include <memory>
#include <vector>



namespace FireflyAssembler
{
    class Path
    {
        private:
            IGraphConstPointer pathGraph;
            std::vector<int> path;

            void free();
            void copy(const Path & other);

            void validatePath() const;

        public:

            Path(IGraphConstPointer g,
                    const std::vector<int> & p);
            Path(const Path & other);

            Path & operator = (const Path & other);

            virtual ~Path();

            virtual IGraphConstPointer getGraph() const;
            int size() const;
            int operator [] (int index) const;

            VectorPointer<Sequence> getContigs() const;
    };
    std::ostream & operator << (std::ostream & s, const Path & p);

    typedef std::shared_ptr<Path> PathPointer;
}

#endif
