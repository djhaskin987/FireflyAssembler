#ifndef __FireflyAssemblerSequence__
#define __FireflyAssemblerSequence__
#include "Types.hpp"
#include "Matcher.hpp"
#include <string>
#include <vector>
#include <string>
#include <memory>
#include <ostream>

namespace FireflyAssembler
{
    class Sequence
    {
        private:
            void free();
            void copy(const Sequence & other);
            std::vector<char> sequence;
            MatcherConstPointer matcher;
        public:
            Sequence(MatcherConstPointer m);
            Sequence(MatcherConstPointer m, const char * s);
            Sequence(MatcherConstPointer m, const std::string & s);
            Sequence(const Sequence & other);

            Sequence & operator = (const Sequence & other);

            ~Sequence();

            void merge(const Sequence & other,
                    int thisOverlap);
            int determineOverlap(const Sequence & other) const;
            int containsSize(const Sequence & other) const;
            int length() const;
            void clear();
            void append(const Sequence & other);
            void append(const std::string & s);
            StringPointer str() const;
            char operator [] (int index) const;
            bool operator == (const Sequence & other) const;
            bool operator < (const Sequence & other) const;
    };
    std::ostream & operator << (std::ostream & o, const Sequence & s);
    typedef std::shared_ptr<Sequence> SequencePointer;
}

#endif
