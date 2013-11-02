#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include "../src/Path.hpp"
#include "../src/Sequence.hpp"
#include "../src/Graph.hpp"
#include "SmallSequencesFixture.hpp"

using namespace FireflyAssembler;
using namespace std;

//-- to test --
//class Path
//{
//        void validatePath() const;
//
//    public:
//        template <typename inputIterator>
//        Path(const Graph & g,
//                inputIterator first,
//                inputIterator last);
//        Path(const Path & other);
//
//        Path & operator = (const Path & other);
//
//        ~Path();
//
//        const Graph & getGraph() const;
//        int size() const;
//        int operator [] (int index) const;
//        void getContigs(std::vector<Sequence> & ss);
//};
//std::ostream & operator << (std::ostream & s, const Path & p);

BOOST_AUTO_TEST_CASE(Path_basic_test)
{
    SmallSequencesFixture g;
    Graph graph;

    graph.addSequence(g[0]);
    graph.addSequence(g[1]);
    graph.addSequence(g[2]);
    graph.addSequence(g[3]);
    graph.addSequence(g[4]);
    int forward[] = {0,1,2,3,4};
    int * ptr = &forward[0];
    int backward[] = {4,3,2,1,0};
    int * btr = &backward[0];
    Path p(graph,
            ptr,ptr+5);
    VectorPointer<Sequence> ss = p.getContigs();
    BOOST_CHECK(ss->size() == 2);
    Path r(graph,
            btr,
            btr+5);
    ss = r.getContigs();
    BOOST_CHECK(ss->size() == 5);
}