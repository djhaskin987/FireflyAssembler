#ifndef __FireflyAssemblerBiggerSequencesFixture__
#define __FireflyAssemblerBiggerSequencesFixutre__

#include <vector>
#include "../src/Sequence.hpp"
#include "../src/Types.hpp"

namespace FireflyAssembler
{

class BiggerSequencesFixture
{

    private:
        VectorPointer<FireflyAssembler::SequencePointer> sequences;
        FireflyAssembler::Sequence fullSequence;
    public:
        BiggerSequencesFixture() :
            fullSequence("ATGTGTGCAACTGTGTGTCTTATGTATTCTGTTATGTGTATATGAGATAGGCTCTTGCTCTGTTGCCCAGGCTGGA")
        {
            sequences.reset(new std::vector<FireflyAssembler::SequencePointer>());

            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CACTGGCTCCAAGAGAG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("TCTGACTGCCCCT")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CTGGCTCCAAGA")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("GCAGTGGGTG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("GCCATGTCTGAC")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("TGGCTCCAAGA")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("GACTGCCCCT")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("ATGTCTGACTGCCCC")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CAAGCACTGGCTCCAAGA")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("ACTGCCCCTTCAA")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CAAGCCATGTC")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CAAGAGAGCAGTG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("TGCCCCTTCAAGCACTGGC")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("ACTGGCTCCAAGAGAGC")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("AGCAGTGGGTGATGG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CTGCCCCTTCAAG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CTTCAAGCACT")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("AAGCACTGGCTCCAAG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("CCCCTTCAAG")));
            sequences->push_back(SequencePointer(new FireflyAssembler::Sequence("AGAGCAGTGGGTGATG")));

        }
        const int size() const
        {
            return sequences->size();
        }

        const FireflyAssembler::Sequence & operator [] (int index)
        {
            return (*((*sequences)[index]));
        }
};
}
#endif
