#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include "../src/Sequence.hpp"
using namespace FireflyAssembler;
using namespace std;
BOOST_AUTO_TEST_CASE(Sequence_basic_test)
{
    Sequence a("The quick brown fox jumped over the lazy dog.");
    string aString = "The quick brown fox jumped over the lazy dog.";
    BOOST_MESSAGE("Checking basics...");
    BOOST_CHECK((*a.str()) == aString);
    BOOST_CHECK(a[6] == 'i');
    BOOST_CHECK(a[12] == 'o');
    BOOST_CHECK(a.length() == 45);
    stringstream out;
    out << a;
    BOOST_CHECK(out.str().length() == 45);
}

BOOST_AUTO_TEST_CASE(Sequence_basic_overlap_test)
{
    Sequence a("The quick brown fox jumped over the lazy dog.");
    Sequence b("lazy dog. The British are coming!");
    BOOST_MESSAGE("Checking overlap...");
    Sequence n("nnnnnnnnnnnnnn");
    int abOverlap = a.determineOverlap(b);
    int anOverlap = a.determineOverlap(n);
    BOOST_CHECK(abOverlap == 9);
    BOOST_CHECK(anOverlap == 0);
}

BOOST_AUTO_TEST_CASE(Sequence_tolerance_overlap_test)
{
    // check that errors OK
    // First check: reads over 5% error are a no-go (20-length read with
    // in-del is 15%.
    Sequence d("The quick brown fox jumped over the lazy dog.");
    Sequence e("d over the lazy og.aaccttggaaccttggaaccttggctgactg");
    int deOverlap = d.determineOverlap(e);
    BOOST_CHECK(deOverlap == 0);

    // test indel overlap.
    Sequence j("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy?");
    Sequence k(" The british are coming This is the tail end of the thingy? BABOOSHKA!");
    int jkOverlap = j.determineOverlap(k);
    BOOST_CHECK(jkOverlap == 60);

    // test in-del overlap must be over length 5 (there is a one-character
    // overlap here, but we want the longer overlap instead)
    Sequence h("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy.");
    Sequence i(" The british are coming This is the tail end of the thingy. BABOOSHKA!");
    int hiOverlap = h.determineOverlap(i);
    BOOST_CHECK(hiOverlap == 60);

    // determine substitute overlap works.
    Sequence f("The quick brown fox jumped over the lazy dog.");
    Sequence g("d over the lazy mog.aaccttggaaccttggaaccttggctgactg");
    int fgOverlap = f.determineOverlap(g);
    BOOST_CHECK(fgOverlap == 20);
}

BOOST_AUTO_TEST_CASE(Sequence_merge_test)
{
    Sequence a("The quick brown fox jumped over the lazy dog.");
    Sequence b("lazy dog. The British are coming!");
    int abOverlap = a.determineOverlap(b);
    Sequence c("The quick brown fox jumped over the lazy dog. The British are coming!");
    a.merge(b,abOverlap);
    BOOST_CHECK(a == c);
    BOOST_CHECK(a.length() == 69);

    Sequence j("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy?");
    Sequence k(" The british are coming This is the tail end of the thingy? BABOOSHKA!");
    int jkOverlap = j.determineOverlap(k);
    j.merge(k,jkOverlap);
    BOOST_CHECK(j.length() == 115);
}
