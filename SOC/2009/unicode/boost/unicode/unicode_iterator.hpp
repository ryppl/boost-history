#ifndef BOOST_UNICODE_ITERATOR_HPP
#define BOOST_UNICODE_ITERATOR_HPP

#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>
#ifndef BOOST_NO_STD_LOCALE
#include <sstream>
#include <ios>
#endif
#include <limits.h> // CHAR_BIT

#include <boost/iterator/pack_iterator.hpp>

namespace boost
{

namespace detail
{
	
static const ::boost::uint16_t high_surrogate_base = 0xD7C0u;
static const ::boost::uint16_t low_surrogate_base = 0xDC00u;
static const ::boost::uint32_t ten_bit_mask = 0x3FFu;

inline bool is_high_surrogate(::boost::uint16_t v)
{
   return (v & 0xFC00u) == 0xd800u;
}
inline bool is_low_surrogate(::boost::uint16_t v)
{
   return (v & 0xFC00u) == 0xdc00u;
}
template <class T>
inline bool is_surrogate(T v)
{
   return (v & 0xF800u) == 0xd800;
}

inline unsigned utf8_byte_count(boost::uint8_t c)
{
   // if the most significant bit with a zero in it is in position
   // 8-N then there are N bytes in this UTF-8 sequence:
   boost::uint8_t mask = 0x80u;
   unsigned result = 0;
   while(c & mask)
   {
      ++result;
      mask >>= 1;
   }
   return (result == 0) ? 1 : ((result > 4) ? 4 : result);
}

inline unsigned utf8_trailing_byte_count(boost::uint8_t c)
{
   return utf8_byte_count(c) - 1;
}

inline void invalid_code_point(::boost::uint32_t val)
{
#ifndef BOOST_NO_STD_LOCALE
	std::stringstream ss;
	ss << "Invalid UTF-32 code point U+" << std::showbase << std::hex << val << " encountered while trying to encode UTF-16 sequence";
	std::out_of_range e(ss.str());
#else
	std::out_of_range e("Invalid UTF-32 code point encountered while trying to encode UTF-16 sequence");
#endif
	boost::throw_exception(e);
}

template<typename Range>
inline void invalid_utf_sequence(const Range& r)
{
#ifndef BOOST_NO_STD_LOCALE
	std::stringstream ss;
	ss << "Invalid UTF sequence " << std::showbase << std::hex;
	for(typename boost::range_iterator<const Range>::type it = boost::begin(r); it != boost::end(r); ++it)
		ss << *it << " ";
	ss << "encountered while trying to decode UTF-32 sequence";
	std::out_of_range e(ss.str());
#else
	std::out_of_range e("Invalid UTF sequence encountered while trying to decode UTF-32 sequence");
#endif
	boost::throw_exception(e);
}

struct u16_packer
{
	typedef boost::uint16_t output_type;
	
	template<typename OutputIterator>
	OutputIterator operator()(boost::uint32_t v, OutputIterator out)
	{
		if(v >= 0x10000u)
		{
			if(v > 0x10FFFFu)
				detail::invalid_code_point(v);
				
			// split into two surrogates:
			output_type hi = static_cast<output_type>(v >> 10) + detail::high_surrogate_base;
			output_type lo = static_cast<output_type>(v & detail::ten_bit_mask) + detail::low_surrogate_base;
			
			BOOST_ASSERT(detail::is_high_surrogate(hi));
			BOOST_ASSERT(detail::is_low_surrogate(lo));
			
			*out++ = hi;
			*out++ = lo;
		}
		else
		{
			// 16-bit code point:
			output_type cp = static_cast<output_type>(v);

			// value must not be a surrogate:
			if(detail::is_surrogate(cp))
				detail::invalid_code_point(v);
				
			*out++ = cp;
		}
		
		return out;
	}
};

struct u16_unpacker
{
	typedef boost::uint32_t output_type;
	
	template<typename Range, typename It>
	std::pair<
		typename boost::range_iterator<Range>::type,
		It
	>
	left(const Range& in, It out)
	{
		assert(!boost::empty(in));
		
		typename boost::range_iterator<const Range>::type it(boost::begin(in));
		
		boost::uint32_t value = *it;
		
		if(detail::is_high_surrogate(value))
		{
			if(++it == boost::end(in))
				detail::invalid_utf_sequence(in);
			
			// precondition; next value must have be a low-surrogate:
         	boost::uint16_t lo = *it;
         	if(!detail::is_low_surrogate(lo))
            	detail::invalid_code_point(lo);
				
         	value = code_point(value, lo);
      	}
      	// postcondition; result must not be a surrogate:
      	if(detail::is_surrogate(value))
			detail::invalid_code_point(static_cast<boost::uint16_t>(value));
		
		*out++ = value;
				
		return std::make_pair(++it, out);
	}
	
	template<typename Range, typename OutputIterator>
	std::pair<
		typename boost::range_iterator<Range>::type,
		OutputIterator
	>
	right(const Range& in, OutputIterator out)
	{
		assert(!boost::empty(in));
		
		typename boost::range_iterator<const Range>::type it = --boost::end(in);
		
		boost::uint32_t value = *it;
		
		if(detail::is_low_surrogate(value))
		{
			if(it == boost::begin(in))
				invalid_utf_sequence(in);
			--it;
			
			boost::uint16_t hi = *it;
         	if(!detail::is_high_surrogate(hi))
            	invalid_code_point(hi);
			
			value = code_point(hi, value);
      	}
      	// postcondition; result must not be a surrogate:
      	if(detail::is_surrogate(value))
			invalid_code_point(static_cast<boost::uint16_t>(value));
			
		*out++ = value;
		
		return std::make_pair(it, out);
	}
	
private:
	boost::uint32_t code_point(boost::uint16_t hi, boost::uint16_t lo)
	{
		return   ((hi - detail::high_surrogate_base) << 10)
		       | (static_cast<boost::uint32_t>(static_cast<boost::uint16_t>(lo)) & detail::ten_bit_mask);
	}
};

struct u8_packer
{
	typedef boost::uint8_t output_type;
	
	template<typename OutputIterator>
	OutputIterator operator()(boost::uint32_t c, OutputIterator out)
	{
		if(c > 0x10FFFFu)
			detail::invalid_code_point(c);

		if(c < 0x80u)
		{
			*out++ = static_cast<unsigned char>(c);
		}
		else if(c < 0x800u)
		{
			*out++ = static_cast<unsigned char>(0xC0u + (c >> 6));
			*out++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
		}
		else if(c < 0x10000u)
		{
			*out++ = static_cast<unsigned char>(0xE0u + (c >> 12));
			*out++ = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
			*out++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
		}
		else
		{
			*out++ = static_cast<unsigned char>(0xF0u + (c >> 18));
			*out++ = static_cast<unsigned char>(0x80u + ((c >> 12) & 0x3Fu));
			*out++ = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
			*out++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
		}
		
		return out;
	}
};

struct u8_unpacker
{
	typedef boost::uint32_t output_type;
	
	template<typename Range, typename It>
	std::pair<
		typename boost::range_iterator<Range>::type,
		It
	>
	left(const Range& in, It out)
	{
		assert(!boost::empty(in));
		
		typename boost::range_iterator<const Range>::type it(boost::begin(in));
		
		boost::uint32_t value = *it;
		
		if((value & 0xC0u) == 0x80u)
			detail::invalid_utf_sequence(in);
			
		// see how many extra byts we have:
		unsigned extra = detail::utf8_trailing_byte_count(value);
		// extract the extra bits, 6 from each extra byte:
		
		for(unsigned c = 0; c < extra; ++c)
		{
			if(++it == boost::end(in))
				detail::invalid_utf_sequence(in);
			
			value <<= 6;
			value += static_cast<boost::uint8_t>(*it) & 0x3Fu;
		}
		
		// we now need to remove a few of the leftmost bits, but how many depends
		// upon how many extra bytes we've extracted:
		static const boost::uint32_t masks[4] = 
		{
			0x7Fu,
			0x7FFu,
			0xFFFFu,
			0x1FFFFFu,
		};
		value &= masks[extra];
		
		// check the result:
		if(value > static_cast<boost::uint32_t>(0x10FFFFu))
			invalid_utf_sequence(in);
		
		*out++ = value;
				
		return std::make_pair(++it, out);
	}
	
	template<typename Range, typename OutputIterator>
	std::pair<
		typename boost::range_iterator<Range>::type,
		OutputIterator
	>
	right(const Range& in, OutputIterator out)
	{
		assert(!boost::empty(in));
		
		typename boost::range_iterator<const Range>::type it = --boost::end(in);
		
		boost::uint32_t value = *it;
		
		// Keep backtracking until we don't have a trailing character:
		unsigned count = 0;
		while((*it & 0xC0u) == 0x80u)
		{					
			if(count >= 4 || it == boost::begin(in))
				invalid_utf_sequence(in);
				
			--it;
			++count;
		}

		// now check that the sequence was valid:
		if(count != detail::utf8_trailing_byte_count(value))
			invalid_utf_sequence(in);
		
		out = left(std::make_pair(it, boost::end(in)), out).second;
		return std::make_pair(it, out);
	}
};

} // namespace detail

template<typename Range>
std::pair<
	pack_iterator<typename boost::range_iterator<const Range>::type, packer<detail::u16_packer> >,
	pack_iterator<typename boost::range_iterator<const Range>::type, packer<detail::u16_packer> >
> make_u32_to_u16_range(const Range& range)
{
	return make_pack_range(range, make_packer(detail::u16_packer()));
}

template<typename Range>
std::pair<
	pack_iterator<typename boost::range_iterator<const Range>::type, detail::u16_unpacker>,
	pack_iterator<typename boost::range_iterator<const Range>::type, detail::u16_unpacker>
> make_u16_to_u32_range(const Range& range)
{
	return make_pack_range(range, detail::u16_unpacker());
}

template<typename Range>
std::pair<
	pack_iterator<typename boost::range_iterator<const Range>::type, packer<detail::u8_packer> >,
	pack_iterator<typename boost::range_iterator<const Range>::type, packer<detail::u8_packer> >
> make_u32_to_u8_range(const Range& range)
{
	return make_pack_range(range, make_packer(detail::u8_packer()));
}

template<typename Range>
std::pair<
	pack_iterator<typename boost::range_iterator<const Range>::type, detail::u8_unpacker>,
	pack_iterator<typename boost::range_iterator<const Range>::type, detail::u8_unpacker>
> make_u8_to_u32_range(const Range& range)
{
	return make_pack_range(range, detail::u8_unpacker());
}

//TODO: output iterators

} // namespace boost

#endif
