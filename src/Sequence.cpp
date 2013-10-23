#include "Sequence.hpp"
#include <stdexcept>
#include <climits>

using namespace FireflyAssembler;
using namespace std;

const int Sequence::TOLERANCE_SCORE = 0;
const int Sequence::INSERT_SCORE = 5;
const int Sequence::DELETE_SCORE = 5;
const int Sequence::SUBST_SCORE = 1;
const int Sequence::MATCH_SCORE = -3;

void Sequence::free()
{

}

void Sequence::copy(const Sequence & other)
{
    sequence = other.sequence;
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
            oldSequence.end() - overlap);
    sequence.insert(sequence.end(),
            other.sequence.begin(),
            other.sequence.end());

}

int Sequence::getScore(const vector<char> & a,
        const vector<char> & b)
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
    return matrix[b.size()-1][a.size()-1];
}

int Sequence::determineOverlap(const Sequence & other)
{
    int maxPossibleOverlap = sequence.size();
    int maxOverlapOffset = 0;
    int minScore = INT_MAX;
    for (int aOffset = 0; aOffset < sequence.size(); aOffset++)
    {
        vector<char> aSegment;
        vector<char> bSegment;
        int bEnd = other.sequence.size();
        if (bEnd > maxPossibleOverlap)
        {
            bEnd = maxPossibleOverlap;
        }
        aSegment.insert(aSegment.end(),
                sequence.begin() + aOffset,
                sequence.end());
        bSegment.insert(bSegment.end(),
                other.sequence.begin(),
                other.sequence.begin() + bEnd);

        int segmentScore = getScore(aSegment, bSegment);
        if (segmentScore < minScore)
        {
            minScore = segmentScore;
            maxOverlapOffset = aOffset;
        }
        maxPossibleOverlap--;
    }
    if (minScore > Sequence::TOLERANCE_SCORE)
    {
        return 0;
    }
    return sequence.size() - maxOverlapOffset;
}

int Sequence::length()
{
    return sequence.size();
}

char Sequence::at(int index)
{
    return sequence[index];
}

string Sequence::toString()
{
    return string(sequence.begin(),
            sequence.end());
}

bool Sequence::operator == (const Sequence & other)
{
    return sequence == other.sequence;
}

bool Sequence::operator < (const Sequence & other)
{
    return sequence < other.sequence;
}


ostream & operator << (ostream & o,
        Sequence & s)
{
    o << s.toString();
}
