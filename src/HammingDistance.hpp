#ifndef __FireflyAssemblerHammingDistance__
#define __FireflyAssemblerHammingDistance__
#include <memory>
#include "DistanceMetric.hpp"

namespace FireflyAssembler
{
    class HammingDistance : public DistanceMetric
    {
        public:
            virtual double distance(const Path & a, const Path & b);
            virtual ~HammingDistance() {}
    };
    typedef std::shared_ptr<HammingDistance> HammingDistancePointer;
    typedef std::shared_ptr<const HammingDistance> HammingDistanceConstPtr;
}

#endif
