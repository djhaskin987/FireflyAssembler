#ifndef __FireflyAssemblerGraph__
#define __FireflyAssemblerGraph__
#include "Types.hpp"
#include "Sequence.hpp"
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace FireflyAssembler
{
    class Graph
    {
        private:
            std::vector<Sequence> sequences;
            // from -> (to -> overlap)
            std::unordered_map<int,std::unordered_map<int,int> > overlaps;

            void free();
            void copy(const Graph & other);

            void addOverlap(int aIndex, int bIndex);
        public:
            Graph();
            Graph(const Graph & other);

            Graph & operator = (const Graph & other);

            ~Graph();

            int sequenceCount() const;
            const Sequence & getSequence(int index) const;
            bool hasOverlap(int first, int second) const;
            bool hasOverlapsFor(int first) const;
            int getOverlap(int first, int second) const;
            const std::unordered_map<int,int> & getOverlapsFor(int first) const;

            void addSequence(const Sequence & sequence);

            HashSetPointer<int> getSources() const;
            HashSetPointer<int> getSinks() const;
    };
    typedef std::shared_ptr<Graph> GraphPointer;
}

#endif
