#ifndef __FireflyAssemblerMatcher__
#define __FireflyAssemblerMatcher__
#include <memory>
#include <vector>

namespace FireflyAssembler
{
    class Matcher
    {
        public:
            static double TOLERANCE_SCORE;
            const static int MINIMUM_MATCH;
            virtual int overlapSize(const std::vector<char> & a,
                    const std::vector<char> & b) const = 0;
            virtual int containSize(const std::vector<char> & a,
                    const std::vector<char> & b) const = 0;
    };
    typedef std::shared_ptr<Matcher> MatcherPointer;
    typedef std::shared_ptr<const Matcher> MatcherConstPointer;
}
#endif
