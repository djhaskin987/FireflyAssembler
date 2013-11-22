#include <iostream>
#include <algorithm>
#include "N50Rating.hpp"

using namespace FireflyAssembler;
using namespace std;


double N50Rating::rate(const Path & path) const
{
    VectorPointer<Sequence> contigs = path.getContigs();

    int total_length = 0;
    for(vector<Sequence>::iterator it = contigs->begin() ; it != contigs->end() ; ++it)
    {
        total_length += it->length();
    }

    sort(contigs->begin(), contigs->end(), c);
    int half_length = total_length/2;
    int total_so_far = 0;
    for(vector<Sequence>::iterator it = contigs->begin() ; it != contigs->end() ; ++it)
    {
        total_so_far += it->length();
        if (total_so_far > half_length)
        {
            return it->length();
        }
    }
}