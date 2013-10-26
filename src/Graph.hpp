#ifndef __FireflyAssemblerGraph__
#define __FireflyAssemblerGraph__
#include "Sequence.hpp"
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
            void getSequence(Sequence & seq, int index) const;
            bool hasOverlap(int first, int second) const;
            bool hasOverlapsFor(int first) const;
            int getOverlap(int first, int second) const;
            const std::unordered_map<int,int> & getOverlapsFor(int first) const;

            void addSequence(const Sequence & sequence);

            void getSources(std::unordered_set<int> & s);
            void getSinks(std::unordered_set<int> & s);
    };
}

#endif
