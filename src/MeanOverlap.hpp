#ifndef __FireflyAssemblerMeanOverlap__
#define __FireflyAssemblerMeanOverlap__
#include <memory>
#include "FitnessFunction.hpp"

namespace FireflyAssembler
{
    class MeanOverlap : public FitnessFunction
    {
        public:
            virtual double rate(const Path & path) const;
    };
}

#endif
