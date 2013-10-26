#ifndef __FireflyAssemblerSmallSequencesFixture__
#define __FireflyAssemblerSmallSequencesFixutre__


namespace FireflyAssember
{

class SmallSequencesFixture
{

    private:
        vector<Sequence> sequences;
        Sequence fullSequence;
    public:
        GraphFixture() : sequences(),
        fullSequence("ATGTGTGCAACTGTGTGTCTTATGTATTCTGTTATGTGTATATGAGATAGGCTCTTGCTCTGTTGCCCAGGCTGGA")
        {
            sequences.push_back(Sequence("ATGTGTGCAACTGTGTGTATTATGTA"));
            sequences.push_back(Sequence("TGCAACTGTGTGTATTATGTATTCTG"));
            sequences.push_back(Sequence("CTGTGTGTATTATGTATTCTGTTATG"));
            sequences.push_back(Sequence("TGTATTATGTATTCTGTTATGTGTAT"));
            sequences.push_back(Sequence("GGCTCTTGCCCTGTTGCCCAGGCTGG"));
        }
        const int size() const
        {
            return sequences.size();
        }

        const Sequence & operator [] (int index)
        {
            return sequences[index];
        }
};
}
