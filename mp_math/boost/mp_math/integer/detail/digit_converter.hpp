// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_DIGIT_CONVERTER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_DIGIT_CONVERTER_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <typeinfo>

namespace boost {
namespace mp_math {
namespace detail {

struct eq_tag{}; // equal digit type and equal radix
struct ne_tag{}; // not equal


// This class converts only the magnitude, sign needs to be handled by caller
template<class DstType, class SrcType>
struct digit_converter
{
  typedef DstType dst_type;
  typedef SrcType src_type;
  typedef typename dst_type::traits_type dst_traits;
  typedef typename src_type::traits_type src_traits;

  static void convert(dst_type& dst, const src_type& src);
  static void convert(dst_type& dst, const src_type& src, eq_tag);
  static void convert(dst_type& dst, const src_type& src, ne_tag);
};


template<class DstType, class SrcType>
void
digit_converter<DstType,SrcType>::
convert(dst_type& dst, const src_type& src)
{
  typedef typename mpl::if_c<
    sizeof(typename dst_type::digit_type) == sizeof(typename src_type::digit_type) &&
    dst_traits::radix_bits == src_traits::radix_bits,
    eq_tag,
    ne_tag
  >::type tag_type;

  convert(dst, src, tag_type());
}

template<class DstType, class SrcType>
void
digit_converter<DstType,SrcType>::
convert(dst_type& dst, const src_type& src, eq_tag)
{
  dst.reserve(src.size());
  std::memcpy(dst.digits(),
              src.digits(),
              sizeof(typename dst_type::digit_type) * src.size());
  dst.set_size(src.size());
}

template<class DstType, class SrcType>
void
digit_converter<DstType,SrcType>::
convert(dst_type& dst, const src_type& src, ne_tag)
{
  dst.reserve(src.size()
      * (src_traits::radix_bits + (dst_traits::radix_bits - 1))
      /  dst_traits::radix_bits);

  typedef typename src_type::digit_type src_digit_type;
  typedef typename dst_type::digit_type dst_digit_type;

  unsigned dst_offset = 0;

  dst_digit_type d = 0;
  for (typename src_type::const_iterator s = src.begin(); s != src.end(); ++s)
  {
    unsigned bits_read = 0;
    while (bits_read < dst_traits::radix_bits)
    {
      unsigned bits = std::min(dst_traits::radix_bits - dst_offset,
                               src_traits::radix_bits - bits_read);

      d |= static_cast<dst_digit_type>(*s >> (src_traits::radix_bits - bits))
           << dst_offset;

      dst_offset += bits;
      bits_read += bits;

      if (bits_read >= src_traits::radix_bits)
        break;

      if (dst_offset >= dst_traits::radix_bits)
      {
        dst.push(d);
        d = 0;
      }
    }
  }

  if (d)
    dst.push(d);
}




} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

