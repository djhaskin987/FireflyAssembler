#ifndef __FireflyAssemblerIGraph__
#define __FireflyAssemblerIGraph__
#include <memory>
#include "Types.hpp"
#include "Sequence.hpp"
#include <memory>

namespace FireflyAssembler
{
    class IGraph
    {
        public:
            virtual int sequenceCount() const = 0;
            virtual const Sequence & getSequence(int index) const = 0;
            virtual bool hasOverlap(int first, int second) const = 0;
            virtual bool hasOverlapsFor(int first) const = 0;
            virtual int getOverlap(int first, int second) const = 0;
            virtual const std::unordered_map<int,int> & getOverlapsFor(int first) const = 0;

            virtual void addSequence(const Sequence & sequence) = 0;

            virtual HashSetPointer<int> getSources() const = 0;
            virtual HashSetPointer<int> getSinks() const = 0;
            virtual ~IGraph() {}
    };
    typedef std::shared_ptr<IGraph> IGraphPointer;
    typedef std::shared_ptr<const IGraph> IGraphConstPointer;
}
#endif
