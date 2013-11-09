#!/bin/sh
normalClass=1
interface=0
algorithm=0

while [ -n "${1}" ]
do
    case "${1}" in
        -i|--interface)
            normalClass=0
            interface=1
            shift
            ;;
        -c|--normal-class)
            normalClass=1
            interface=0
            shift
            ;;
        -a|--algorithm)
            algorithm=1
            interface=0
            normalClass=0
            shift
            ;;
        *)
            if [ -z "${namespace}" ]
            then
                namespace="${1}"
            elif [ -z "${class}" ]
            then
                class="${1}"
            fi
            shift
            ;;
    esac
done

cat > ${class}.hpp << HERE
#ifndef __${namespace}${class}__
#define __${namespace}${class}__
#include <memory>

namespace ${namespace}
{
    class ${class}
    {
HERE
if [ ${interface} -ne 0 -o ${algorithm} -ne 0 ]
then
    cat >> ${class}.hpp << HERE
        public:
    };
    typedef std::shared_ptr<${class}> ${class}Pointer;
    typedef std::shared_ptr<const ${class}> ${class}ConstPointer;
}
#endif
HERE
    if [ ${algorithm} -ne 0 ]
    then
        cat > ${class}.cpp << HERE
#include "${class}.hpp"

using namespace ${namespace};
using namespace std;

HERE
    fi
else
    cat >> ${class}.hpp << HERE
        private:
            void free();
            void copy(const ${class} & other);

        public:
            ${class}();
            ${class}(const ${class} & other);

            ${class} & operator = (const ${class} & other);

            virtual ~${class}();
    };
    typedef std::shared_ptr<${class}> ${class}Pointer;
    typedef std::shared_ptr<const ${class}> ${class}ConstPtr;
}

#endif
HERE
cat > ${class}.cpp << HERE
#include "${class}.hpp"

using namespace ${namespace};
using namespace std;

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

${class} & ${class}::operator = (const ${class} & other)
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
fi
