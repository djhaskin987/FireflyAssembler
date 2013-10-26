#ifndef __FireflyAssemblerSmallSequencesFixture__
#define __FireflyAssemblerSmallSequencesFixutre__

#include <vector>
#include "../src/Sequence.hpp"

namespace FireflyAssembler
{

class SmallSequencesFixture
{

    private:
        std::vector<FireflyAssembler::Sequence> sequences;
        FireflyAssembler::Sequence fullSequence;
    public:
        SmallSequencesFixture() : sequences(),
        fullSequence("ATGTGTGCAACTGTGTGTCTTATGTATTCTGTTATGTGTATATGAGATAGGCTCTTGCTCTGTTGCCCAGGCTGGA")
        {
            sequences.push_back(FireflyAssembler::Sequence("ATGTGTGCAACTGTGTGTATTATGTA"));
            sequences.push_back(FireflyAssembler::Sequence("TGCAACTGTGTGTATTATGTATTCTG"));
            sequences.push_back(FireflyAssembler::Sequence("CTGTGTGTATTATGTATTCTGTTATG"));
            sequences.push_back(FireflyAssembler::Sequence("TGTATTATGTATTCTGTTATGTGTAT"));
            sequences.push_back(FireflyAssembler::Sequence("GGCTCTTGCCCTGTTGCCCAGGCTGG"));
        }
        const int size() const
        {
            return sequences.size();
        }

        const FireflyAssembler::Sequence & operator [] (int index)
        {
            return sequences[index];
        }
};
}
#endif
