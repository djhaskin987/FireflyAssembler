#ifndef __FireflyAssemblerMeanOverlap__
#define __FireflyAssemblerMeanOverlap__
#include <memory>
#include "FitnessFunction.hpp"

namespace FireflyAssembler
{
    class MeanOverlap : public FitnessFunction
    {
        private:
            double getOverlapPercentage(const Path & path, int a, int b) const;
        public:
            virtual double rate(const Path & path) const;
    };
}

#endif
