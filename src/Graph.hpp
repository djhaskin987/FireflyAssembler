#ifndef __FireflyAssemblerGraph__
#define __FireflyAssemblerGraph__
#include "IGraph.hpp"
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace FireflyAssembler
{
    class Graph : public IGraph
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

            virtual ~Graph();

            virtual int sequenceCount() const;
            virtual const Sequence & getSequence(int index) const;
            virtual bool hasOverlap(int first, int second) const;
            virtual bool hasOverlapsFor(int first) const;
            virtual int getOverlap(int first, int second) const;
            virtual const std::unordered_map<int,int> & getOverlapsFor(int first) const;

            virtual void addSequence(const Sequence & sequence);

            virtual HashSetPointer<int> getSources() const;
            virtual HashSetPointer<int> getSinks() const;
    };
    typedef std::shared_ptr<Graph> GraphPointer;
}

#endif
