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
    class FitnessFunction;
    typedef std::shared_ptr<FitnessFunction> FitnessFunctionPointer;
    class Path
    {
        private:
            IGraphConstPointer pathGraph;
            std::vector<int> path;
            double rating;

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

            void setRating(double rating);
            double getRating();

            void swapSequences(int index, int new_sequence);
            bool operator ==(const Path & other);
    };
    std::ostream & operator << (std::ostream & s, const Path & p);

    typedef std::shared_ptr<Path> PathPointer;
    typedef std::shared_ptr<const Path> PathConstPointer;
}

#endif
