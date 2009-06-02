#ifndef BOOST_UTF_CONVERSION_HPP
#define BOOST_UTF_CONVERSION_HPP

#include <boost/assert.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>
#ifndef BOOST_NO_STD_LOCALE
#include <sstream>
#include <ios>
#endif

#include <boost/iterator/pipe_iterator.hpp>
#include <boost/unicode/unicode_properties.hpp>

namespace boost
{

namespace detail
{
    
static const char16 high_surrogate_base = 0xD7C0u;
static const char16 low_surrogate_base = 0xDC00u;
static const char32 ten_bit_mask = 0x3FFu;

inline unsigned utf8_byte_count(uint8_t c)
{
   // if the most significant bit with a zero in it is in position
   // 8-N then there are N bytes in this UTF-8 sequence:
   uint8_t mask = 0x80u;
   unsigned result = 0;
   while(c & mask)
   {
      ++result;
      mask >>= 1;
   }
   return (result == 0) ? 1 : ((result > 4) ? 4 : result);
}

inline unsigned utf8_trailing_byte_count(uint8_t c)
{
   return utf8_byte_count(c) - 1;
}

inline void invalid_code_point(char32 val)
{
#ifndef BOOST_NO_STD_LOCALE
	std::stringstream ss;
	ss << "Invalid UTF-32 code point U+" << std::showbase << std::hex << (uint_least32_t)val << " encountered while trying to encode UTF-16 sequence";
	std::out_of_range e(ss.str());
#else
	std::out_of_range e("Invalid UTF-32 code point encountered while trying to encode UTF-16 sequence");
#endif
	throw_exception(e);
}

template<typename Iterator>
inline void invalid_utf_sequence(Iterator begin, Iterator end)
{
#ifndef BOOST_NO_STD_LOCALE
	std::stringstream ss;
	ss << "Invalid UTF sequence " << std::showbase << std::hex;
	for(Iterator it = begin; it != end; ++it)
		ss << *it << " ";
	ss << "encountered while trying to decode UTF-32 sequence";
	std::out_of_range e(ss.str());
#else
	std::out_of_range e("Invalid UTF sequence encountered while trying to decode UTF-32 sequence");
#endif
	throw_exception(e);
}

struct u16_packer
{
	typedef char16 output_type;
    static const int max_output = 2;
	
	template<typename OutputIterator>
	OutputIterator operator()(char32 v, OutputIterator out)
	{
		if(v >= 0x10000u)
		{
			if(v > 0x10FFFFu)
				detail::invalid_code_point(v);
				
			// split into two surrogates:
			output_type hi = static_cast<output_type>(v >> 10) + detail::high_surrogate_base;
			output_type lo = static_cast<output_type>(v & detail::ten_bit_mask) + detail::low_surrogate_base;
			
			BOOST_ASSERT(unicode::is_high_surrogate(hi));
			BOOST_ASSERT(unicode::is_low_surrogate(lo));
			
			*out++ = hi;
			*out++ = lo;
		}
		else
		{
			// 16-bit code point:
			output_type cp = static_cast<output_type>(v);

			// value must not be a surrogate:
			if(unicode::is_surrogate(cp))
				detail::invalid_code_point(v);
				
			*out++ = cp;
		}
		
		return out;
	}
};

struct u16_unpacker
{
	typedef char32 output_type;
    static const int max_output = 1;
	
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		In it = begin;
		char32 value = *it;
		
		if(unicode::is_high_surrogate(value))
		{
			if(++it == end)
				detail::invalid_utf_sequence(begin, end);
			
			// precondition; next value must have be a low-surrogate:
         	char16 lo = *it;
         	if(!unicode::is_low_surrogate(lo))
            	detail::invalid_code_point(lo);
				
         	value = code_point(value, lo);
      	}
      	// postcondition; result must not be a surrogate:
      	if(unicode::is_surrogate(value))
			detail::invalid_code_point(static_cast<char16>(value));
		
		*out++ = value;
				
		return std::make_pair(++it, out);
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		In it = --end;
		char32 value = *it;
		
		if(unicode::is_low_surrogate(value))
		{
			if(it == begin)
				invalid_utf_sequence(begin, end);
			--it;
			
			char16 hi = *it;
         	if(!unicode::is_high_surrogate(hi))
            	invalid_code_point(hi);
			
			value = code_point(hi, value);
      	}
      	// postcondition; result must not be a surrogate:
      	if(unicode::is_surrogate(value))
			invalid_code_point(static_cast<char16>(value));
			
		*out++ = value;
		
		return std::make_pair(it, out);
	}
	
private:
	char32 code_point(char16 hi, char16 lo)
	{
		return   ((hi - detail::high_surrogate_base) << 10)
		       | (static_cast<char32>(static_cast<char16>(lo)) & detail::ten_bit_mask);
	}
};

struct u8_packer
{
	typedef char output_type;
    static const int max_output = 4;
	
	template<typename OutputIterator>
	OutputIterator operator()(char32 c, OutputIterator out)
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
	typedef char32 output_type;
    static const int max_output = 1;
	
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		In it = begin;
		char32 value = *it;
		
		if((value & 0xC0u) == 0x80u)
			detail::invalid_utf_sequence(begin, end);
			
		// see how many extra byts we have:
		unsigned extra = detail::utf8_trailing_byte_count(value);
		// extract the extra bits, 6 from each extra byte:
		for(unsigned c = 0; c < extra; ++c)
		{
			if(++it == end)
				detail::invalid_utf_sequence(begin, end);
			
			value <<= 6;
			value += static_cast<unsigned char>(*it) & 0x3Fu;
		}
		
		// we now need to remove a few of the leftmost bits, but how many depends
		// upon how many extra bytes we've extracted:
		static const char32 masks[4] = 
		{
			0x7Fu,
			0x7FFu,
			0xFFFFu,
			0x1FFFFFu,
		};
		value &= masks[extra];
		
		// check the result:
		if(value > static_cast<char32>(0x10FFFFu))
			invalid_utf_sequence(begin, end);
		
		*out++ = value;
				
		return std::make_pair(++it, out);
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		In it = --end;
		char32 value = *it;
		
		// Keep backtracking until we don't have a trailing character:
		unsigned count = 0;
		while((*it & 0xC0u) == 0x80u)
		{					
			if(count >= 4 || it == begin)
				invalid_utf_sequence(begin, end);
				
			--it;
			++count;
		}

		// now check that the sequence was valid:
		if(count != detail::utf8_trailing_byte_count(value))
			invalid_utf_sequence(begin, end);
		
		out = ltr(it, end, out).second;
		return std::make_pair(it, out);
	}
};

} // namespace detail

template<typename Range>
std::pair<
	pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<detail::u16_packer> >,
	pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<detail::u16_packer> >
> make_u32_to_u16_range(const Range& range)
{
	return make_pipe_range(range, make_one_many_pipe(detail::u16_packer()));
}

template<typename Range>
std::pair<
	pipe_iterator<typename range_iterator<const Range>::type, detail::u16_unpacker>,
	pipe_iterator<typename range_iterator<const Range>::type, detail::u16_unpacker>
> make_u16_to_u32_range(const Range& range)
{
	return make_pipe_range(range, detail::u16_unpacker());
}

template<typename Range>
std::pair<
	pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<detail::u8_packer> >,
	pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<detail::u8_packer> >
> make_u32_to_u8_range(const Range& range)
{
	return make_pipe_range(range, make_one_many_pipe(detail::u8_packer()));
}

template<typename Range>
std::pair<
	pipe_iterator<typename range_iterator<const Range>::type, detail::u8_unpacker>,
	pipe_iterator<typename range_iterator<const Range>::type, detail::u8_unpacker>
> make_u8_to_u32_range(const Range& range)
{
	return make_pipe_range(range, detail::u8_unpacker());
}

template<typename OutputIterator>
pipe_output_iterator<OutputIterator, one_many_pipe<detail::u8_packer> >
make_u8_output_iterator(OutputIterator out)
{
	return make_pipe_output_iterator(out, make_one_many_pipe(detail::u8_packer()));
}

template<typename OutputIterator>
pipe_output_iterator<OutputIterator, one_many_pipe<detail::u16_packer> >
make_u16_output_iterator(OutputIterator out)
{
	return make_pipe_output_iterator(out, make_one_many_pipe(detail::u16_packer()));
}

} // namespace boost

#endif
