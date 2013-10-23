#ifndef __FireflyAssemblerSequence__
#define __FireflyAssemblerSequence__

#include <vector>
#include <string>
#include <ostream>

namespace FireflyAssembler
{
    class Sequence
    {
        private:
            void free();
            void copy(const Sequence & other);
            int getScore(const std::vector<char> & a,
                    const std::vector<char> & b);
            std::vector<char> sequence;
        public:
            const static int TOLERANCE_SCORE=4;
            const static int INSERT_SCORE=3;
            const static int DELETE_SCORE=3;
            const static int SUBST_SCORE=1;
            const static int MATCH_SCORE=0;
            Sequence(char * s);
            Sequence(const std::string & s);
            Sequence(const Sequence & other);

            Sequence & operator = (const Sequence & other);

            ~Sequence();

            void merge(const Sequence & other,
                    int overlap);
            int determineOverlap(const Sequence & other);
            int length();
            char at(int index);
            std::string toString();
            bool operator == (const Sequence & other);
            bool operator < (const Sequence & other);
    };
    std::ostream & operator << (std::ostream & o, Sequence & s);
}

#endif
