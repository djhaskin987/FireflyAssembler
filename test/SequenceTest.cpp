#include <boost/test/minimal.hpp>
#include "../src/Sequence.hpp"
using namespace FireflyAssembler;
using namespace std;
int test_main(int argc, char *argv[])
{
    Sequence a("aaccttggaaccttgg");
    BOOST_CHECK(a.toString() == string("aaccttggaaccttgg"));
    BOOST_CHECK(a.at(6) == 'g');
    BOOST_CHECK(b.at(8) == 'a');
    Sequence b("ccttggactgactg");
    Sequence narf("nasdlikjfasd;lkjasdf");
    BOOST_CHECK(a.determineOverlap(b) == 6);
    // check minimal score
    BOOST_CHECK(a.determineOverlap(narf) == Sequence::TOLERANCE_SCORE);
    Sequence c("aaccttggaaccttggactgactg");
    a.merge(b);
    BOOST_CHECK(a == c);
    a.clear();
    BOOST_CHECK(a.length() == 0);

    return 0;
}
