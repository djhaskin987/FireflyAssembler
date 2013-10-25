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
                    const std::vector<char> & b) const;
            std::vector<char> sequence;
        public:
            const static int MINIMUM_OVERLAP;
            const static double TOLERANCE_SCORE;
            const static int INSERT_SCORE;
            const static int DELETE_SCORE;
            const static int SUBST_SCORE;
            const static int MATCH_SCORE;
            Sequence();
            Sequence(const char * s);
            Sequence(const std::string & s);
            Sequence(const Sequence & other);

            Sequence & operator = (const Sequence & other);

            ~Sequence();

            void merge(const Sequence & other,
                    int thisOverlap);
            int determineOverlap(const Sequence & other) const;
            int length() const;
            void clear();
            void append(const Sequence & other);
            std::string str() const;
            char operator [] (int index) const;
            bool operator == (const Sequence & other) const;
            bool operator < (const Sequence & other) const;
    };
    std::ostream & operator << (std::ostream & o, const Sequence & s);
}

#endif
