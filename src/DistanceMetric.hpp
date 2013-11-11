#ifndef __FireflyAssemblerDistanceMetric__
#define __FireflyAssemblerDistanceMetric__
#include <memory>
#include "Path.hpp"

namespace FireflyAssembler
{
    class DistanceMetric
    {
        public:
            virtual double distance(const Path & a, const Path & b) = 0;
    };
    typedef std::shared_ptr<DistanceMetric> DistanceMetricPointer;
    typedef std::shared_ptr<const DistanceMetric> DistanceMetricConstPointer;
}
#endif
