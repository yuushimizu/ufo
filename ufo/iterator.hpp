#ifndef ufo_iterator
#define ufo_iterator

#include <utility>
#include <type_traits>
#include <iterator>

namespace ufo {
    template <typename Range>
    constexpr auto adl_begin(Range &&range) {
        using std::begin;
        return begin(std::forward<Range>(range));
    }
    
    template <typename Range>
    constexpr auto adl_end(Range &&range) {
        using std::end;
        return end(std::forward<Range>(range));
    }
    
    template <typename Range>
    constexpr auto adl_cbegin(Range &&range) {
        using std::cbegin;
        return cbegin(std::forward<Range>(range));
    }
    
    template <typename Range>
    constexpr auto adl_cend(Range &&range) {
        using std::cend;
        return cend(std::forward<Range>(range));
    }
    
    template <typename Iterator>
    using iterator_category_t = typename std::iterator_traits<Iterator>::iterator_category;
    
    template <typename Iterator>
    using iterator_value_type_t = typename std::iterator_traits<Iterator>::value_type;
    
    template <typename Iterator>
    using iterator_difference_type_t = typename std::iterator_traits<Iterator>::difference_type;
    
    template <typename Iterator>
    using iterator_pointer_t = typename std::iterator_traits<Iterator>::pointer;
    
    template <typename Iterator>
    using iterator_reference_t = typename std::iterator_traits<Iterator>::reference;
}

#endif
