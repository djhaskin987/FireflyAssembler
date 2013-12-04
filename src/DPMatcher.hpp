#ifndef __FireflyAssemblerDPMatcher__
#define __FireflyAssemblerDPMatcher__
#include <vector>
#include <memory>
#include "Matcher.hpp"

namespace FireflyAssembler
{
    class DPMatcher : Matcher
    {
        private:
            int getScore(const std::vector<char> & a,
                    const std::vector<char> & b) const;
            double getMinEntryScore(int entry, int size, double minScore) const;
            double scoreShortcut(const std::vector<char> & a,
                    const std::vector<char> & b) const;
        public:
            const static int INSERT_SCORE;
            const static int DELETE_SCORE;
            const static int SUBST_SCORE;
            const static int MATCH_SCORE;
            virtual int overlapSize(const std::vector<char> & a,
                    const std::vector<char> & b) const;
            virtual int containSize(const std::vector<char> & a,
                    const std::vector<char> & b) const;
            virtual ~DPMatcher();
    };
    typedef std::shared_ptr<DPMatcher> DPMatcherPointer;
    typedef std::shared_ptr<const DPMatcher> DPMatcherConstPtr;
}

#endif
