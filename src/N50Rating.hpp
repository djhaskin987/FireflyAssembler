#ifndef __FireflyAssemblerN50Rating__
#define __FireflyAssemblerN50Rating__
#include <memory>
#include "FitnessFunction.hpp"

namespace FireflyAssembler
{
		struct compare {
		    bool operator()(const Sequence& first, const Sequence& second) {
		        return first.length() > second.length();
		    }
		};

    class N50Rating : public FitnessFunction
    {
    		private:
    			compare c; 
        public:
            virtual double rate(const Path & path) const;
    };
}

#endif
