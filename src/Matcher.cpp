#include "Matcher.hpp"

using namespace std;
using namespace FireflyAssembler;

const int Matcher::MINIMUM_MATCH = 5;
// allow 5% error in the overlap (ish)
double Matcher::TOLERANCE_SCORE = .05;
