#ifndef __FireflyAssemblerTypes__
#define __FireflyAssemblerTypes__

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace FireflyAssembler
{
    template <typename t>
    using HashSet = std::unordered_set<t>;

    template <typename t>
    using HashSetPointer =  std::shared_ptr<std::unordered_set<t> >;
    template <typename k, typename v>
    using HashMap = std::unordered_map<k,v>;

    template <typename k, typename v>
    using HashMapPointer = std::shared_ptr<HashMap<k,v> >;
    template <typename T>
    using VectorPointer = std::shared_ptr<std::vector<T> >;

    typedef std::shared_ptr<std::string> StringPointer;
}


#endif
