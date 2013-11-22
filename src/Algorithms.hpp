#ifndef __FireflyAssemblerAlgorithms__
#define __FireflyAssemblerAlgorithms__
#include <limits>
#include <vector>

namespace FireflyAssembler
{
    double getMinEntryScore(int entry, int size, double minScore)
    {
        double minNormalizedScore = (double)entry /
                                    (double)size;
        if (minScore > minNormalizedScore)
        {
            minScore = minNormalizedScore;
        }
        return minScore;
    }

    template <typename v>
    int getScore(const v & a,
            const v & b,
            int insertScore,
            int deleteScore,
            int matchScore,
            int substScore,
            double tolerance)
    {
        std::vector<std::vector<int> > matrix;
        matrix.push_back(std::vector<int>());
        int largerSize = a.size() > b.size() ? a.size() : b.size();
        double minScore = std::numeric_limits<double>::infinity();
        bool safePrune = insertScore >= 0 &&
            deleteScore >= 0 &&
            matchScore >= 0 &&
            substScore >= 0;
        matrix[0].push_back(matchScore);
        minScore = getMinEntryScore(matrix[0][0], largerSize, minScore);
        for (int i = 1; i <= a.size(); i++)
        {
            matrix[0].push_back(matrix[0][i-1]+deleteScore);
            minScore = getMinEntryScore(matrix[0][i], largerSize, minScore);
        }
        if (safePrune && minScore > tolerance)
        {
            // We're already too big!
            // And since no score incrementer is negative
            // (insertScore, deleteScore, and match/substScore are non-negative),
            // We know that anything produced in the next row is also going to be
            // too big, since it can only get bigger from here (this row).
            // We may as well return now.
            return std::numeric_limits<int>::max();
        }

        int row;
        for (row = 1; row <= b.size(); row++)
        {
            matrix.push_back(std::vector<int>());
            minScore = std::numeric_limits<double>::infinity();
            matrix[row].push_back(matrix[row-1][0]+insertScore);
            minScore = getMinEntryScore(matrix[row][0], largerSize, minScore);
            for (int col = 1; col <= a.size(); col++)
            {
                int insertScore = matrix[row-1][col] + insertScore;
                int deleteScore = matrix[row][col-1] + deleteScore;
                int matchSubScore = matrix[row-1][col-1];
                if (a[col-1] == b[row-1])
                {
                    matchSubScore += matchScore;
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
                    matchSubScore += substScore;
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
            if (safePrune && minScore > tolerance)
            {
                // We're already too big!
                // And since no score incrementer is negative
                // (insertScore, deleteScore, and match/substScore are non-negative),
                // We know that anything produced in the next row is also going to be
                // too big, since it can only get bigger from here (this row).
                // We may as well return now.
                return std::numeric_limits<int>::max();
            }
        }
    return matrix[b.size()][a.size()];
    }
}

#endif
