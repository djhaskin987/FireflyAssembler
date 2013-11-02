#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include "../src/Sequence.hpp"
#include "../src/Graph.hpp"
#include "SmallSequencesFixture.hpp"

using namespace FireflyAssembler;
using namespace std;

//-- to test --
//class Graph
//{
//    public:
//        Graph();
//        Graph(const Graph & other);
//
//        Graph & operator = (const Graph & other);
//
//        ~Graph();
//
//        int sequenceCount() const;
//        void getSequence(Sequence & seq, int index) const;
//        bool hasOverlap(int first, int second) const;
//        bool hasOverlapsFor(int first) const;
//        int getOverlap(int first, int second) const;
//        const std::unordered_map<int,int> & getOverlapsFor(int first) const;
//
//        void addSequence(const Sequence & sequence);
//};


BOOST_AUTO_TEST_CASE(Graph_basic_test)
{
    SmallSequencesFixture g;
    Graph graph;

    graph.addSequence(g[0]);
    graph.addSequence(g[1]);
    graph.addSequence(g[2]);
    graph.addSequence(g[3]);
    graph.addSequence(g[4]);
    BOOST_CHECK(graph.getSequence(0) == g[0]);
    BOOST_CHECK(graph.getSequence(1) == g[1]);
    BOOST_CHECK(graph.getSequence(2) == g[2]);
    BOOST_CHECK(graph.getSequence(3) == g[3]);
    BOOST_CHECK(graph.getSequence(4) == g[4]);
    BOOST_CHECK(graph.sequenceCount() == 5);
    HashSetPointer<int> sources = graph.getSources();
    BOOST_CHECK(sources->find(0) != sources->end());
    HashSetPointer<int> sinks = graph.getSinks();
    BOOST_CHECK(sinks->find(4) != sinks->end());
    BOOST_CHECK(graph.hasOverlap(0,1));
    BOOST_CHECK(graph.getOverlap(0,1) == 21);
    BOOST_CHECK(graph.hasOverlap(1,2));
    BOOST_CHECK(graph.getOverlap(1,2) == 21);
    BOOST_CHECK(graph.hasOverlap(2,3));
    BOOST_CHECK(graph.getOverlap(2,3) == 21);
    BOOST_CHECK(graph.hasOverlap(0,3));
    BOOST_CHECK(graph.getOverlap(0,3) == 11);
    BOOST_CHECK(graph.getOverlapsFor(0).size() == 3);


}
