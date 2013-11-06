#ifndef __FireflyAssemblerFitnessFunction__
#define __FireflyAssemblerFitnessFunction__
#include <memory>
#include <vector>
#include "Path.hpp"

namespace FireflyAssembler
{
    class FitnessFunction
    {
        public:
            virtual double rate(const Path & path) const = 0;
    };
    typedef std::shared_ptr<FitnessFunction> FitnessFunctionPointer;
    typedef std::shared_ptr<const FitnessFunction> FitnessFunctionConstPointer;
}
#endif
