#include <boost/test/minimal.hpp>
#include "../src/Sequence.hpp"
using namespace FireflyAssembler;
using namespace std;
int test_main(int argc, char *argv[])
{
    Sequence a("aaccttggaaccttgg");
    BOOST_CHECK(a.toString() == string("aaccttggaaccttgg"));
    BOOST_CHECK(a.at(6) == 'g');
    BOOST_CHECK(a.at(8) == 'a');
    BOOST_CHECK(a.length() == 16);
    Sequence b("ccttggactgactg");
    cout << "Expected a-b overlap: " << 6 << endl;
    Sequence narf("nasdlikjfasd;lkjasdf");
    int abOverlap = a.determineOverlap(b);
    cout << "Actual a-b overlap: " << abOverlap << endl;
    cout << "Expected a-n overlap: " << 0 << endl;
    int anOverlap = a.determineOverlap(narf);
    cout << "Actual a-n overlap: " << anOverlap << endl;
    BOOST_CHECK(abOverlap == 6);
    BOOST_CHECK(anOverlap == 0);
    Sequence c("aaccttggaaccttggactgactg");
    a.merge(b,abOverlap);
    cout << "C: " << c << endl;
    cout << "A: " << a << endl;
    BOOST_CHECK(a == c);
    BOOST_CHECK(a.length() == 24);
    return 0;
}
