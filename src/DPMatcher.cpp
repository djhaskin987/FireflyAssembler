#include "DPMatcher.hpp"
#include <stdexcept>
#include <limits>
#include <iostream>

using namespace FireflyAssembler;
using namespace std;

const int DPMatcher::INSERT_SCORE = 3;
const int DPMatcher::DELETE_SCORE = 3;
const int DPMatcher::SUBST_SCORE = 1;
const int DPMatcher::MATCH_SCORE = 0;

DPMatcher::~DPMatcher()
{
}

int DPMatcher::overlapSize(const vector<char> & a,
        const vector<char> & b) const
{
    int minAOffset = 0;
    int minBEnd = b.size();
    int maxOverlap = 0;
    double minScore = numeric_limits<double>::infinity();
    for (int aOffset = 0; aOffset < a.size(); aOffset++)
    {
        for (int bEnd = b.size();
                bEnd > 0;
                bEnd--)
        {
            vector<char> aSegment;
            vector<char> bSegment;

            aSegment.insert(aSegment.end(),
                    a.begin() + aOffset,
                    a.end());
            bSegment.insert(bSegment.end(),
                    b.begin(),
                    b.begin() + bEnd);

            int aOverlapSize = a.size() - aOffset;
            int bOverlapSize = bEnd;
            int overlapSize = aOverlapSize > bOverlapSize ? aOverlapSize :
                bOverlapSize;
            double segmentScore = scoreShortcut(aSegment,
                    bSegment);
            // take the max of the two overlap sizes

            // code for under-the-bar longest
            // if (segmentScore < Matcher::TOLERANCE_SCORE)
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
                    overlapSize > Matcher::MINIMUM_MATCH)

            {
                minScore = segmentScore;
                minAOffset = aOffset;
                minBEnd = bEnd;
            }
        }
    }
    if (minScore > Matcher::TOLERANCE_SCORE)
    {
        return 0;
    }
    // returns the number of codons in THIS a in the overlap.
    // we intentionally omit returning bEnd here, since it is not used
    return a.size() - minAOffset;
}

int DPMatcher::containSize(const vector<char> & a,
        const vector<char> & b) const
{
    int minAOffset = 0;
    int minAEnd = 0;
    int maxContains = 0;
    double minScore = numeric_limits<double>::infinity();
    for (int aOffset = 0; aOffset < a.size(); aOffset++)
    {
        for (int aEnd = a.size();
                aEnd > aOffset;
                aEnd--)
        {
            vector<char> aSegment;
            vector<char> bSegment = b;

            aSegment.insert(aSegment.end(),
                    a.begin() + aOffset,
                    a.begin() + aEnd);


            int aContainSize = aEnd - aOffset;
            int bContainSize = b.size();
            // take the max of the two contain sizes
            int containSize = aContainSize > bContainSize ? aContainSize :
                bContainSize;
            double segmentScore = scoreShortcut(aSegment, bSegment);

            // code for under-the-bar longest
            // if (segmentScore < Matcher::TOLERANCE_SCORE)
            // {
            //     if (overlapSize > maxContains)
            //     {
            //         maxContains = overlapSize;
            //         minAOffset = aOffset;
            //         minAEnd = aEnd;
            //     }
            // }

            // code for smallest error, which says
            // that it is more unlikely that overlaps of size 'n'
            // exist and are under tolerance are the correct overlap
            // than that long segments under tolerance are the correct
            // overlap
            if (segmentScore < minScore &&
                    containSize > Matcher::MINIMUM_MATCH)

            {
                minScore = segmentScore;
                minAOffset = aOffset;
                minAEnd = aEnd;
            }
        }
    }
    if (minScore > Matcher::TOLERANCE_SCORE)
    {
        return 0;
    }
    // returns the number of codons in THIS sequence in the overlap.
    // we intentionally omit returning bEnd here, since it is not used
    return minAEnd - minAOffset;
}

double DPMatcher::getMinEntryScore(int entry, int size, double minScore) const
{
    double minNormalizedScore = (double)entry /
                                (double)size;
    if (minScore > minNormalizedScore)
    {
        minScore = minNormalizedScore;
    }
    return minScore;
}

int DPMatcher::getScore(const vector<char> & a,
        const vector<char> & b) const
{
    vector<vector<int> > matrix;
    matrix.push_back(vector<int>());
    int largerSize = a.size() > b.size() ? a.size() : b.size();
    double minScore = numeric_limits<double>::infinity();
    matrix[0].push_back(DPMatcher::MATCH_SCORE);
    minScore = getMinEntryScore(matrix[0][0], largerSize, minScore);
    for (int i = 1; i <= a.size(); i++)
    {
        matrix[0].push_back(matrix[0][i-1]+DPMatcher::DELETE_SCORE);
        minScore = getMinEntryScore(matrix[0][i], largerSize, minScore);
    }
    if (minScore > Matcher::TOLERANCE_SCORE)
    {
        // We're already too big!
        // And since no score incrementer is negative
        // (INSERT_SCORE, DELETE_SCORE, and MATCHSUB_SCORE are non-negative),
        // We know that anything produced in the next row is also going to be
        // too big, since it can only get bigger from here (this row).
        // We may as well return now.
        return numeric_limits<int>::max();
    }

    int row;
    for (row = 1; row <= b.size(); row++)
    {
        matrix.push_back(vector<int>());
        minScore = numeric_limits<double>::infinity();
        matrix[row].push_back(matrix[row-1][0]+DPMatcher::INSERT_SCORE);
        minScore = getMinEntryScore(matrix[row][0], largerSize, minScore);
        for (int col = 1; col <= a.size(); col++)
        {
            int insertScore = matrix[row-1][col] + DPMatcher::INSERT_SCORE;
            int deleteScore = matrix[row][col-1] + DPMatcher::DELETE_SCORE;
            int matchSubScore = matrix[row-1][col-1];
            if (a[col-1] == b[row-1])
            {
                matchSubScore += DPMatcher::MATCH_SCORE;
                if (matchSubScore <= insertScore &&
                        matchSubScore <= deleteScore)
                {
                    matrix[row].push_back(matchSubScore);
                    minScore = getMinEntryScore(matrix[row][col], largerSize, minScore);
                    continue;
                }
            }
            else
            {
                matchSubScore += DPMatcher::SUBST_SCORE;
                if (matchSubScore <= insertScore &&
                        matchSubScore <= deleteScore)
                {
                    matrix[row].push_back(matchSubScore);
                    minScore = getMinEntryScore(matrix[row][col], largerSize, minScore);
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
            minScore = getMinEntryScore(matrix[row][col], largerSize, minScore);
        }
        if (minScore > Matcher::TOLERANCE_SCORE)
        {
            // We're already too big!
            // And since no score incrementer is negative
            // (INSERT_SCORE, DELETE_SCORE, and MATCHSUB_SCORE are non-negative),
            // We know that anything produced in the next row is also going to be
            // too big, since it can only get bigger from here (this row).
            // We may as well return now.
            return numeric_limits<int>::max();
        }
    }
    return matrix[b.size()][a.size()];
}

double DPMatcher::scoreShortcut(const vector<char> & a,
        const vector<char> & b) const
{
    int indels;
    if (a.size() > b.size())
    {
       indels = b.size() - a.size();
    }
    else
    {
       indels = a.size() - b.size();
    }
    int overlaySize = a.size() > b.size() ? a.size() : b.size();
    double minScore = (double)(indels * DPMatcher::INSERT_SCORE) / overlaySize;
    if (minScore >= Matcher::TOLERANCE_SCORE)
    {
        return numeric_limits<double>::infinity();
    }
    int segmentRawScore = getScore(a, b);
    // decision made: score with smallest error, or
    // score with under-tolerance error that's the longest?

    double segmentScore = ((double)segmentRawScore) /
        ((double)overlaySize);
    return segmentScore;
}
