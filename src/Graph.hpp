#ifndef __FireflyAssemblerGraph__
#define __FireflyAssemblerGraph__

#include <vector>
#include <map>

namespace FireflyAssembler
{

    class Graph
    {
        private:
            std::vector<Sequence> sequences;
            // from -> (to -> overlap)
            std::map<int,map<int,int> > overlaps;

            void free();
            void copy(const Graph & other);

        public:
            Graph();
            Graph(const Graph & other);

            Graph & operator = (const Graph & other);

            ~Graph();

            void assemble(Sequence & seq);
            void getSequence(Sequence & seq, int index);

            int sequenceCount();
            void addSequence(const Sequence & sequence);
            int getOverlap(int first, int second);
            const map<int,int> & getOverlapsFor(int first) const;
    };
}

#endif
