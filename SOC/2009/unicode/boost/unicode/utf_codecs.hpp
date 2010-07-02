#ifndef BOOST_UTF_CODECS_HPP
#define BOOST_UTF_CODECS_HPP

#include <boost/assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/mpl/int.hpp>
#include <stdexcept>
#ifndef BOOST_NO_STD_LOCALE
#include <sstream>
#include <ios>
#endif

#include <boost/unicode/surrogates.hpp>

#include <boost/iterator/convert_iterator.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/detail/unspecified.hpp>

namespace boost
{
namespace unicode
{

namespace detail
{

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
	ss << "Invalid UTF-32 code point U+" << std::showbase << std::hex << (uint_least32_t)val << " encountered";
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
	boost::throw_exception(e);
}

} // namespace detail

/** Model of \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly
 * that converts a code point to a sequence of UTF-16 code units. */
struct u16_encoder : one_many_converter<u16_encoder>
{
    typedef char32 input_type;
	typedef char16 output_type;
    typedef mpl::int_<2> max_output;
	
    /** Throws \c std::out_of_range if \c v is not a valid code point. */
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

/** Model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly that
 * converts a sequence of UTF-16 code units into a single code point. */
struct u16_decoder
{
    typedef char16 input_type;
	typedef char32 output_type;
    typedef mpl::int_<1> max_output;
	
    /** Throws \c std::out_of_range if [<tt>begin</tt>, <tt>end</tt>[ is not a valid UTF-16 range. */
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		In it = begin;
		char32 value = *it;
		
		if(unicode::is_high_surrogate(value))
		{
            // precondition; next value must have be a low-surrogate:
			if(++it == end)
				detail::invalid_utf_sequence(begin, end);
			
         	char16 lo = *it;
         	if(!unicode::is_low_surrogate(lo))
            	detail::invalid_code_point(lo);
				
         	value = code_point((char16)value, lo);
      	}
      	// postcondition; result must not be a surrogate:
      	if(unicode::is_surrogate(value))
			detail::invalid_code_point(static_cast<char16>(value));
		
		*out++ = value;
		return std::make_pair(++it, out);
	}
	
    /** Throws \c std::out_of_range if [<tt>begin</tt>, <tt>end</tt>[ is not a valid UTF-16 range. */
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
        BOOST_ASSERT(!is_surrogate(*begin) || is_high_surrogate(*begin));
		
		In it = --end;
		char32 value = *it;
		
		if(unicode::is_low_surrogate(value))
		{
            // precondition; next value must have be a high-surrogate:
			if(it == begin)
				detail::invalid_utf_sequence(begin, end);
			--it;
        
			char16 hi = *it;
         	if(!unicode::is_high_surrogate(hi))
            	detail::invalid_code_point(hi);
			
			value = code_point(hi, (char16)value);
      	}
      	// postcondition; result must not be a surrogate:
      	if(unicode::is_surrogate(value))
			detail::invalid_code_point(static_cast<char16>(value));
			
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

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * that tells whether a position lies on a code point boundary within
 * a range of UTF-16 code units. */
struct u16_boundary
{
    typedef char16 input_type;
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        BOOST_ASSERT(begin != end);
        BOOST_ASSERT(pos != begin);
        BOOST_ASSERT(pos != end);
        
        return !is_surrogate(*pos) || is_high_surrogate(*pos);
    }
};

/** Model of \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly
 * that converts a code point to a sequence of UTF-8 code units. */
struct u8_encoder : one_many_converter<u8_encoder>
{
    typedef char32 input_type;
	typedef char output_type;
    typedef mpl::int_<4> max_output;
	
    /** Throws \c std::out_of_range if \c c is not a valid code point. */
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

/** Model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly that
 * converts a sequence of UTF-8 code units into a single code point. */
struct u8_decoder
{
    typedef char input_type;
	typedef char32 output_type;
    typedef mpl::int_<1> max_output;

private:
    template<typename In>
    void check(bool test, In begin, In end)
    {
        if(!test)
            detail::invalid_utf_sequence(begin, end);
    }

public:	
    /** Throws \c std::out_of_range if [<tt>begin</tt>, <tt>end</tt>[ is not a valid UTF-8 range. */
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		In p = begin;
        
        unsigned char b0 = *(p++);
        if((b0 & 0x80) == 0)
        {
            char32 r = b0;
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check(p != end, begin, end);
        unsigned char b1 = *(p++);
        check((b1 & 0xc0) == 0x80, begin, end);
        if((b0 & 0xe0) == 0xc0)
        {
            char32 r = (b1 & 0x3f) | ((b0 & 0x1f) << 6);
            check(r >= 0x80, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check(p != end, begin, end);
        unsigned char b2 = *(p++);
        check((b2 & 0xc0) == 0x80, begin, end);
        if((b0 & 0xf0) == 0xe0)
        {
            char32 r = (b2 & 0x3f) | ((b1 & 0x3f) << 6) | ((b0 & 0x0f) << 12);
            check(r >= 0x800, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check(p != end, begin, end);
        unsigned char b3 = *(p++);
        check((b3 & 0xc0) == 0x80, begin, end);
        if((b0 & 0xf8) == 0xf0)
        {
            char32 r = (b3 & 0x3f) | ((b2 & 0x3f) << 6) | ((b1 & 0x3f) << 12) | ((b0 & 0x07) << 18);
            check(r >= 0x10000, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        detail::invalid_utf_sequence(begin, end);
        return std::make_pair(p, out);
	}

    /** Throws \c std::out_of_range if [<tt>begin</tt>, <tt>end</tt>[ is not a valid UTF-8 range. */	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		In p = end;
        
		unsigned char b0 = *(--p);
        if((b0 & 0x80) == 0)
        {
            char32 r = b0;
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check((b0 & 0xc0) == 0x80, begin, end);
        check(p != begin, begin, end);
        unsigned char b1 = *(--p);
        if((b1 & 0xe0) == 0xc0)
        {
            char32 r = (b0 & 0x3f) | ((b1 & 0x1f) << 6);
            check(r >= 0x80, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check((b1 & 0xc0) == 0x80, begin, end);
        check(p != begin, begin, end);
        unsigned char b2 = *(--p);
        if((b2 & 0xf0) == 0xe0)
        {
            char32 r = (b0 & 0x3f) | ((b1 & 0x3f) << 6) | ((b2 & 0x0f) << 12);
            check(r >= 0x800, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        check((b2 & 0xc0) == 0x80, begin, end);
        check(p != begin, begin, end);
        unsigned char b3 = *(--p);
        if((b3 & 0xf8) == 0xf0)
        {
            char32 r = (b0 & 0x3f) | ((b1 & 0x3f) << 6) | ((b2 & 0x3f) << 12) | ((b3 & 0x07) << 18);
            check(r >= 0x10000, begin, end);
            *out++ = r;
            return std::make_pair(p, out);
        }
        
        detail::invalid_utf_sequence(begin, end);
        return std::make_pair(p, out);
	}
};

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * that tells whether a position lies on a code point boundary within
 * a range of UTF-8 code units. */
struct u8_boundary
{
    typedef char input_type;
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        BOOST_ASSERT(begin != end);
        BOOST_ASSERT(pos != begin);
        BOOST_ASSERT(pos != end);
        
        unsigned char c = *pos;
        return (c & 0x80) == 0 || (c & 0xc0) == 0xc0;
    }
};

namespace detail
{

    template<typename T, typename Enable = void>
    struct is_u32 : mpl::false_ {};
    template<> struct is_u32<char32> : mpl::true_ {};
    template<typename T>
    struct is_u32<T, typename enable_if<
        mpl::and_<
            is_same<T, wchar_t>,
            mpl::bool_<sizeof(T) == 4>
        >
    >::type> : mpl::true_ {};

    template<typename T, typename Enable = void>
    struct is_u16 : mpl::false_ {};
    template<> struct is_u16<char16> : mpl::true_ {};
    template<typename T>
    struct is_u16<T, typename enable_if<
        mpl::and_<
            is_same<T, wchar_t>,
            mpl::bool_<sizeof(T) == 2>
        >
    >::type> : mpl::true_ {};

    template<typename T, typename Enable = void>
    struct is_u8 : mpl::false_ {};
    template<> struct is_u8<char> : mpl::true_ {};

    template<typename ValueType, typename Enable = void>
    struct select_encoder
    {
    };
    
    template<typename ValueType>
    struct select_encoder<ValueType, typename enable_if<
        detail::is_u32<ValueType>
    >::type>
    {
        typedef cast_converter<char32> type;
    };
    
    template<typename ValueType>
    struct select_encoder<ValueType, typename enable_if<
        detail::is_u16<ValueType>
    >::type>
    {
        typedef u16_encoder type;
    };
    
    template<typename ValueType>
    struct select_encoder<ValueType, typename enable_if<
        detail::is_u8<ValueType>
    >::type>
    {
        typedef u8_encoder type;
    };

} // namespace detail

/** Model of \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly,
 * either behaves like \c u16_encoder, \c u8_encoder or nothing
 * depending on the targeted element type \c ValueType. */
template<typename ValueType>
struct utf_encoder : detail::select_encoder<ValueType>::type
{
};

/** Model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly,
 * either behaves like \c u16_decoder, a \c u8_decoder or nothing depending on the
 * value type of the input range. */
struct utf_decoder
{
    typedef char32 input_type;
    typedef char32 output_type;
    typedef mpl::int_<1> max_output;

#ifndef BOOST_UNICODE_DOXYGEN_INVOKED
private:
    template<typename Iterator, typename Enable = void>
    struct decoder
    {
    };
    
    template<typename Iterator>
    struct decoder<Iterator, typename enable_if<
        detail::is_u32<
            typename std::iterator_traits<Iterator>::value_type
        >
    >::type>
    {
        typedef cast_converter<char32> type;
    };
    
    template<typename Iterator>
    struct decoder<Iterator, typename enable_if<
        detail::is_u16<
            typename std::iterator_traits<Iterator>::value_type
        >
    >::type>
    {
        typedef u16_decoder type;
    };
    
    template<typename Iterator>
    struct decoder<Iterator, typename enable_if<
        detail::is_u8<
            typename std::iterator_traits<Iterator>::value_type
        >
    >::type>
    {
        typedef u8_decoder type;
    };
#endif

public:
    template<typename In, typename Out>
    std::pair<In, Out>
	ltr(In begin, In end, Out out)
    {
        return typename decoder<In>::type().ltr(begin, end, out);
    }
    
    template<typename In, typename Out>
    std::pair<In, Out>
	rtl(In begin, In end, Out out)
    {
        return typename decoder<In>::type().rtl(begin, end, out);
    }
};

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly,
 * either behaves like \c u16_boundary or \c u8_boundary depending on
 * the value type of the input range. */
struct utf_boundary
{
    typedef char32 input_type;
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        return impl(begin, end, pos, (void*)0);
    }
    
#ifndef BOOST_UNICODE_DOXYGEN_INVOKED
private:
    template<typename In>
    bool impl(
        In begin, In end, In pos,
        typename enable_if<
            detail::is_u32<
                typename std::iterator_traits<In>::value_type
            >
        >::type*
    )
    {
        return true;
    }
    
    template<typename In>
    bool impl(
        In begin, In end, In pos,
        typename enable_if<
            detail::is_u16<
                typename std::iterator_traits<In>::value_type
            >
        >::type*
    )
    {
        return u16_boundary()(begin, end, pos);
    }
    
    template<typename In>
    bool impl(
        In begin, In end, In pos,
        typename enable_if<
            detail::is_u8<
                typename std::iterator_traits<In>::value_type
            >
        >::type*
    )
    {
        return u8_boundary()(begin, end, pos);
    }
#endif
};

/** Model of \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly
 * that converts from UTF-32 to ISO-8859-1 alias latin-1. */
typedef boost::detail::unspecified< cast_converter<char> >::type latin1_encoder;

} // namespace unicode

} // namespace boost

#endif
