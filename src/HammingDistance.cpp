#include "HammingDistance.hpp"
#include <stdexcept>

using namespace FireflyAssembler;
using namespace std;

double HammingDistance::distance(const Path & a, const Path & b)
{
    double returned = 0;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            returned = returned + 1.0;
        }
    }
    returned = returned / (double)a.size();
    return returned;
}
