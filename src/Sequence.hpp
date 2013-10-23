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
            const static double TOLERANCE_SCORE;
            const static int MAX_INDELS;
            const static int INSERT_SCORE;
            const static int DELETE_SCORE;
            const static int SUBST_SCORE;
            const static int MATCH_SCORE;
            Sequence(const char * s);
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