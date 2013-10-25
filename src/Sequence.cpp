#include "Sequence.hpp"
#include <stdexcept>
#include <limits>
#include <iostream>

using namespace FireflyAssembler;
using namespace std;

// allow 5% error in the overlap (ish)
const int Sequence::MINIMUM_OVERLAP = 5;
const double Sequence::TOLERANCE_SCORE = .05;
const int Sequence::INSERT_SCORE = 3;
const int Sequence::DELETE_SCORE = 3;
const int Sequence::SUBST_SCORE = 1;
const int Sequence::MATCH_SCORE = 0;

void Sequence::free()
{

}

void Sequence::copy(const Sequence & other)
{
    sequence = other.sequence;
}

Sequence::Sequence() : sequence()
{
}

Sequence::Sequence(const char * s) : sequence()
{
    while (*s)
    {
        sequence.push_back(*s++);
    }
}

Sequence::Sequence(const string & s)  : sequence()
{
    sequence.insert(sequence.begin(),
            s.begin(),
            s.end());
}

Sequence::Sequence(const Sequence & other)
{
    copy(other);
}

Sequence & Sequence::operator = (const Sequence & other)
{
    if (this != &other)
    {
        free();
        copy(other);
    }
}

Sequence::~Sequence()
{
    free();
}

void Sequence::append(const Sequence & other)
{
    sequence.insert(sequence.end(),
            other.sequence.begin(),
            other.sequence.end());
}

void Sequence::merge(const Sequence & other,
        int overlap)
{
    if (overlap > other.sequence.size())
    {
        throw runtime_error("overlap exceeds other sequence argument's length");
    }
    if (overlap > sequence.size())
    {
        throw runtime_error("overlap exceeds this sequence's length");
    }

    vector<char> oldSequence = sequence;
    sequence.clear();
    sequence.insert(sequence.end(),
            oldSequence.begin(),
            oldSequence.begin() + (oldSequence.size() - overlap));
    sequence.insert(sequence.end(),
            other.sequence.begin(),
            other.sequence.end());
}

int Sequence::getScore(const vector<char> & a,
        const vector<char> & b) const
{
    vector<vector<int> > matrix;
    matrix.push_back(vector<int>());
    matrix[0].push_back(Sequence::MATCH_SCORE);
    for (int i = 1; i <= a.size(); i++)
    {
        matrix[0].push_back(matrix[0][i-1]+Sequence::DELETE_SCORE);
    }

    int row;
    for (row = 1; row <= b.size(); row++)
    {
        matrix.push_back(vector<int>());

        matrix[row].push_back(matrix[row-1][0]+Sequence::INSERT_SCORE);
        for (int col = 1; col <= a.size(); col++)
        {
            int insertScore = matrix[row-1][col] + Sequence::INSERT_SCORE;
            int deleteScore = matrix[row][col-1] + Sequence::DELETE_SCORE;
            int matchSubScore = matrix[row-1][col-1];
            if (a[col-1] == b[row-1])
            {
                matchSubScore += Sequence::MATCH_SCORE;
                if (matchSubScore <= insertScore &&
                        matchSubScore <= deleteScore)
                {
                    matrix[row].push_back(matchSubScore);
                    continue;
                }
            }
            else
            {
                matchSubScore += Sequence::SUBST_SCORE;
                if (matchSubScore <= insertScore &&
                        matchSubScore <= deleteScore)
                {
                    matrix[row].push_back(matchSubScore);
                    continue;
                }
            }
            if (insertScore <= deleteScore)
            {
                matrix[row].push_back(insertScore);
            }
            else
            {
                matrix[row].push_back(deleteScore);
            }
        }
    }
    return matrix[b.size()][a.size()];
}

int Sequence::determineOverlap(const Sequence & other) const
{
    int minAOffset = 0;
    int minBEnd = other.sequence.size();
    int maxOverlap = 0;
    double minScore = numeric_limits<double>::infinity();
    for (int aOffset = 0; aOffset < sequence.size(); aOffset++)
    {
        for (int bEnd = other.sequence.size();
                bEnd > 0;
                bEnd--)
        {
            vector<char> aSegment;
            vector<char> bSegment;

            aSegment.insert(aSegment.end(),
                    sequence.begin() + aOffset,
                    sequence.end());
            bSegment.insert(bSegment.end(),
                    other.sequence.begin(),
                    other.sequence.begin() + bEnd);
            int segmentRawScore(getScore(aSegment, bSegment));
            // decision made: score with smallest error, or
            // score with under-tolerance error that's the longest?

            double segmentScore = ((double)segmentRawScore) /
                ((double)aSegment.size());
            int aOverlapSize = sequence.size() - aOffset;
            int bOverlapSize = bEnd;
            // take the max of the two overlap sizes
            int overlapSize = aOverlapSize > bOverlapSize ? aOverlapSize :
                bOverlapSize;

            // code for under-the-bar longest
            // if (segmentScore < Sequence::TOLERANCE_SCORE)
            // {
            //     if (overlapSize > maxOverlap)
            //     {
            //         maxOverlap = overlapSize;
            //         minAOffset = aOffset;
            //         minBEnd = bEnd;
            //     }
            // }

            // code for smallest error, which says
            // that it is more unlikely that overlaps of size 'n'
            // exist and are under tolerance are the correct overlap
            // than that long segments under tolerance are the correct
            // overlap
            if (segmentScore < minScore &&
                    overlapSize > Sequence::MINIMUM_OVERLAP)

            {
                minScore = segmentScore;
                minAOffset = aOffset;
                minBEnd = bEnd;
            }
        }
    }
    if (minScore > Sequence::TOLERANCE_SCORE)
    {
        return 0;
    }
    // returns the number of codons in THIS sequence in the overlap.
    // we intentionally omit returning bEnd here, since it is not used
    return sequence.size() - minAOffset;
}

int Sequence::length() const
{
    return sequence.size();
}

void Sequence::clear()
{
    sequence.clear();
}

char Sequence::operator [](int index) const
{
    return sequence[index];
}

string Sequence::str() const
{
    return string(sequence.begin(),
            sequence.end());
}

bool Sequence::operator == (const Sequence & other) const
{
    return sequence == other.sequence;
}

bool Sequence::operator < (const Sequence & other) const
{
    return sequence < other.sequence;
}

ostream & FireflyAssembler::operator << (ostream & o,
        const Sequence & s)
{
    for (int i = 0; i < s.length(); i++)
    {
        o << s[i];
    }
    return o;
}
