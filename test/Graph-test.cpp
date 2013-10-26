#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include "../src/Sequence.hpp"
#include "../src/Graph.hpp"

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
    GraphFixture g = new GraphFixture();
    Graph graph;
    

}
