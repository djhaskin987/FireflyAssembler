#!/bin/sh

namespace="${1}"
shift
class="${1}"
shift

cat > ${class}.hpp << HERE
#ifndef __${namespace}${class}__
#define __${namespace}${class}__

namespace ${namespace}
{

    class ${class}
    {
        private:
            void init();
            void free();
            void copy(const ${class} & other);

        public:
            ${class}();
            ${class}(const ${class} & other);

            ${class} & operator = (const ${class} & other);

            ~${class}();
    };
}

#endif
HERE

cat > ${class}.cpp << HERE
#include "${namespace}${class}.hpp"

using namespace ${namespace};

void $class::init()
{

}

void ${class}::free()
{

}

void ${class}::copy(const ${class} & other)
{

}

${class}::${class}()
{
}

${class}::${class}(const ${class} & other)
{
    copy(other);
}

${class} & ${class}::operator = ${class}(const ${class} & other)
{
    if (this != &other)
    {
        free();
        copy(other);
    }
}

${class}::~${class}()
{
    free();
}
HERE
