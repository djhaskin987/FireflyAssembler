#include <boost/test/minimal.hpp>
#include "../src/Sequence.hpp"
using namespace FireflyAssembler;
using namespace std;
int test_main(int argc, char *argv[])
{
    Sequence a("The quick brown fox jumped over the lazy dog.");
    Sequence b("lazy dog. The British are coming!");
    BOOST_CHECK(a.toString() == string("aaccttggaaccttgg"));
    BOOST_CHECK(a.at(6) == 'i');
    BOOST_CHECK(a.at(8) == 'k');
    BOOST_CHECK(a.length() == 45);
    cout << "Expected a-b overlap: " << 9 << endl;
    Sequence n("nnnnnnnnnnnnnn");
    cout << "Determining overlap of a and b" << endl;
    int abOverlap = a.determineOverlap(b);
    cout << "Actual a-b overlap: " << abOverlap << endl;
    cout << "Expected a-n overlap: " << 0 << endl;
    cout << "Determining overlap of a and n" << endl;
    int anOverlap = a.determineOverlap(n);
    cout << "Actual a-n overlap: " << anOverlap << endl;
    BOOST_CHECK(abOverlap == 9);
    BOOST_CHECK(anOverlap == 0);
    // check that errors OK
    Sequence d("The quick brown fox jumped over the lazy dog.");
    Sequence e("d over the lazy og.aaccttggaaccttggaaccttggctgactg");
    int deOverlap = d.determineOverlap(e);
    cout << "Expected noisy overlap: " << 20 << endl;
    cout << "Actual noisy overlap: " << deOverlap << endl;
    BOOST_CHECK(deOverlap == 8);
    Sequence c("aaccttggaaccttggactgactg");
    a.merge(b,abOverlap);
    cout << "C: " << c << endl;
    cout << "A: " << a << endl;
    BOOST_CHECK(a == c);
    BOOST_CHECK(a.length() == 24);
    return 0;
}
