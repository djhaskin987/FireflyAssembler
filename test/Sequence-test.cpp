#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include "../src/Sequence.hpp"
using namespace FireflyAssembler;
using namespace std;
BOOST_AUTO_TEST_CASE(Sequence_test)
{
    Sequence a("The quick brown fox jumped over the lazy dog.");
    Sequence b("lazy dog. The British are coming!");
    string aString = "The quick brown fox jumped over the lazy dog.";
    BOOST_MESSAGE("Checking basics...");
    BOOST_CHECK(a.str() == aString);
    BOOST_CHECK(a[6] == 'i');
    BOOST_CHECK(a[12] == 'k');
    BOOST_CHECK(a.length() == 45);
    stringstream out;
    out << a;
    BOOST_CHECK(out.str().length() == 45);
    BOOST_MESSAGE("Checking overlap...");
    Sequence n("nnnnnnnnnnnnnn");
    int abOverlap = a.determineOverlap(b);
    int anOverlap = a.determineOverlap(n);
    BOOST_CHECK(abOverlap == 9);
    BOOST_CHECK(anOverlap == 0);
    // check that errors OK
    Sequence d("The quick brown fox jumped over the lazy dog.");
    Sequence e("d over the lazy og.aaccttggaaccttggaaccttggctgactg");
    int deOverlap = d.determineOverlap(e);
    BOOST_CHECK(deOverlap == 20);

    // test noisy large vs single-overlap.
    Sequence h("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy.");
    Sequence i(" The british are coming This is the tail end of the thingy. BABOOSHKA!");
    int hiOverlap = h.determineOverlap(i);
    BOOST_CHECK(hiOverlap == 60);

    // test noisy large.
    Sequence j("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy?");
    Sequence k(" The british are coming This is the tail end of the thingy? BABOOSHKA!");
    int jkOverlap = j.determineOverlap(k);
    BOOST_CHECK(jkOverlap == 60);

    // determine substitute overlap works.
    Sequence f("The quick brown fox jumped over the lazy dog.");
    Sequence g("d over the lazy mog.aaccttggaaccttggaaccttggctgactg");
    int fgOverlap = f.determineOverlap(g);
    BOOST_CHECK(fgOverlap == 20);
    Sequence c("The quick brown fox jumped over the lazy dog. The British are coming!");
    BOOST_MESSAGE("Checking merge...");
    a.merge(b,abOverlap);
    BOOST_CHECK(a == c);
    BOOST_CHECK(a.length() == 69);
    j.merge(k,jkOverlap);
    BOOST_CHECK(j.length() == 116);
}
