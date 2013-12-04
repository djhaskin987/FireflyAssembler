#include <limits>
#include <iostream>
#include "StraightMatcher.hpp"

using namespace FireflyAssembler;
using namespace std;

StraightMatcher::~StraightMatcher()
{
}

double StraightMatcher::distance(const vector<char> & a, const vector<char> & b) const
{
    int sum = 0;
    int stop = (int)(Matcher::TOLERANCE_SCORE * (double)a.size());
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            sum++;
            if (sum > stop)
            {
                return numeric_limits<double>::infinity();
            }
        }
    }
    // they're both the same in StraightMatcher, a and b size
    return (double)sum / (double)a.size();
}

int StraightMatcher::overlapSize(const std::vector<char> & a,
        const std::vector<char> & b) const
{
    int stop = 0;
    if (b.size() < a.size())
    {
        stop = a.size() - b.size();
    }

    int minAOffset = a.size();
    int minBEnd = b.size();
    int maxOverlap = 0;
    double minScore = numeric_limits<double>::infinity();
    for (int aOffset = a.size() - Matcher::MINIMUM_MATCH  - 1; aOffset >= stop;  aOffset--)
    {
        int overlapSize = a.size() - aOffset;
        vector<char> aSegment;
        vector<char> bSegment;

        aSegment.insert(aSegment.end(),
                a.begin() + aOffset,
                a.end());
        bSegment.insert(bSegment.end(),
                b.begin(),
                b.begin() + overlapSize);
        double currentScore = distance(aSegment, bSegment);
        if (currentScore < Matcher::TOLERANCE_SCORE)
        {
            if (minScore > currentScore)
            {
                minAOffset = aOffset;
                minScore = currentScore;
            }
        }
    }
    return a.size() - minAOffset;
}

int StraightMatcher::containSize(const std::vector<char> & a,
        const std::vector<char> & b) const
{
    if (b.size() > a.size())
    {
        return 0;
    }
    int stop = a.size() - b.size();
    for (int aOffset = 0; aOffset < stop; aOffset++)
    {
        vector<char> aSegment;

        aSegment.insert(aSegment.end(),
                a.begin() + aOffset,
                a.begin() + aOffset + b.size());
        double currentScore = distance(aSegment, b);
        if (currentScore < Matcher::TOLERANCE_SCORE)
        {
            return b.size();
        }
    }
    return 0;
}
