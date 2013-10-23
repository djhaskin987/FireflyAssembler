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
    cout << "Determining first indel overlap" << endl;
    int deOverlap = d.determineOverlap(e);
    cout << "Expected first indel overlap: " << 0 << endl;
    cout << "Actual first indel overlap: " << deOverlap << endl;

    // test noisy large vs single-overlap.
    Sequence h("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy.");
    Sequence i(". The british are coming This is the tail end of the thingy. BABOOSHKA!");
    cout << "Determining second indel overlap" << endl;
    int hiOverlap = h.determineOverlap(i);
    cout << "Expected second indel overlap: " << 60 << endl;
    cout << "Actual second indel overlap: " << hiOverlap << endl;
    BOOST_CHECK(hiOverlap == 60);

    // test noisy large.

    Sequence j("The quick brown fox jumped over the lazy dog. The british are coming! This is the tail end of the thingy?");
    Sequence k(". The british are coming This is the tail end of the thingy? BABOOSHKA!");
    cout << "Determining third indel overlap" << endl;
    int jkOverlap = j.determineOverlap(k);
    cout << "Expected third indel overlap: " << 60 << endl;
    cout << "Actual second indel overlap: " << jkOverlap << endl;
    BOOST_CHECK(jkOverlap == 60);


    cout << "Determining sub overlap" << endl;
    Sequence f("The quick brown fox jumped over the lazy dog.");
    Sequence g("d over the lazy mog.aaccttggaaccttggaaccttggctgactg");
    int fgOverlap = f.determineOverlap(g);
    cout << "Expected sub overlap: " << 20 << endl;
    cout << "Actual sub overlap: " << fgOverlap << endl;
   // BOOST_CHECK(fgOverlap == 20);
    Sequence c("aaccttggaaccttggactgactg");
    a.merge(b,abOverlap);
    cout << "A: " << a << endl;
    //BOOST_CHECK(a == c);
    //BOOST_CHECK(a.length() == 24);
    return 0;
}
