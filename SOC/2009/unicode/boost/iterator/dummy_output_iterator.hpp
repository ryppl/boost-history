#ifndef BOOST_DUMMY_OUTPUT_ITERATOR_HPP
#define BOOST_DUMMY_OUTPUT_ITERATOR_HPP

#include <iterator>

namespace boost
{
    struct dummy_output_iterator
    {
      typedef std::output_iterator_tag iterator_category;
      typedef void value_type;
      typedef void pointer;
      typedef void difference_type;

      struct reference {
        template<typename T>
        reference& operator=(const T&) { return *this; }
      };

      reference operator*() const { return reference(); }
      dummy_output_iterator& operator++() { return *this; }
      dummy_output_iterator operator++(int) { return *this; }
    };
} // namespace boost

#endif
