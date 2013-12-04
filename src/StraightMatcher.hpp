#ifndef __FireflyAssemblerStraightMatcher__
#define __FireflyAssemblerStraightMatcher__
#include <memory>
#include <vector>
#include "Matcher.hpp"

namespace FireflyAssembler
{
    class StraightMatcher : Matcher
    {
        private:

            double distance(const std::vector<char> & a, const std::vector<char> & b) const;
        public:
            virtual int overlapSize(const std::vector<char> & a,
                    const std::vector<char> & b) const;
            virtual int containSize(const std::vector<char> & a,
                    const std::vector<char> & b) const;
            virtual ~StraightMatcher();
    };
    typedef std::shared_ptr<StraightMatcher> StraightMatcherPointer;
    typedef std::shared_ptr<const StraightMatcher> StraightMatcherConstPtr;
}

#endif
