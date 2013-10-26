#ifndef __FireflyAssemblerPath__
#define __FireflyAssemblerPath__

namespace FireflyAssembler
{
    class Path
    {
        private:
            Graph * pathGraph;
            std::vector<int> path;

            void free();
            void copy(const Path & other);

            void validatePath();

        public:
            template <typename inputIterator>
            Path(const Graph & g,
                    inputIterator first,
                    inputIterator last);
            Path(const Path & other);

            Path & operator = (const Path & other);

            ~Path();

            const Graph & getGraph() const;
            int size() const;
            int operator [] (int index) const;
            void getContigs(std::vector<Sequence> & ss);
    };
    ostream & operator << (ostream & s, const Path & p);
}

#endif
