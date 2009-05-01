/**
 * \file boost/mirror/visitors/sample.hpp
 *
 *  Simple JSON serializer
 *
 *  NOTE: this implementation is to be taken only as an sample
 *  of using Mirror for this task. It is by no means complete.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_VISITORS_JSON
#define BOOST_MIRROR_VISITORS_JSON

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_path/next.hpp>
#include <boost/mirror/meta_path/empty.hpp>
//
#include <boost/type_traits/is_fundamental.hpp>

#include <stdexcept>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_JSON_SERIALIZER_STRING_RELATED_HELPER(R,DATA,I,CHAR_T)\
        static inline mpl::true_ is_base_type(::std::basic_string< CHAR_T >*) \
        { \
                return mpl::true_(); \
        } \
        inline void do_print_value( \
		const ::std::basic_string< CHAR_T >& value \
	) const \
        { \
                do_print_string(value); \
        }


template <class OutputStream, class EncodeMBCs>
class json_serializer 
{
private:
	OutputStream& out_stream;
public:
	typedef mpl::true_ works_on_instances;

	json_serializer(OutputStream& ostream)
	 : out_stream(ostream)
	{ }

	// enter a class/type
	template <class MetaClass, class Context>
	void enter_type(MetaClass, Context)
	{
		if(!is_base_type((typename MetaClass::reflected_type*)0).value)
			out_stream << '{';
	}

	// leave the class/type
	template <class MetaClass, class Context>
	void leave_type(MetaClass, Context)
	{
		if(!is_base_type((typename MetaClass::reflected_type*)0).value)
			out_stream << '}';
	}

	template <class MetaAttribute, class Context>
	inline void enter_attribute(MetaAttribute, Context)
	{
		out_stream << '"' << MetaAttribute::base_name() << '"' << ':';
	}

	template <class MetaAttribute, class Context>
	void leave_attribute(MetaAttribute, Context)
	{
		if(!meta_path::empty<meta_path::next<Context> >::value)
			out_stream << ',';
	}

	template <class MetaClass, class Context, typename InstanceType>
	void visit_instance(MetaClass, Context ctx, InstanceType* ptr_to_inst)
	{
		print_value(
			ptr_to_inst, 
			is_base_type(ptr_to_inst)
		);
	}

private:
	static inline void throw_exception(void)
	{
		throw ::std::runtime_error("JSON encoding error");
	}

	BOOST_CTS_FOR_EACH_CHAR_T(
		BOOST_MIRROR_JSON_SERIALIZER_STRING_RELATED_HELPER,
		_
	)

	template <typename Type>
	static inline is_fundamental<Type> is_base_type(Type*)
	{
		return is_fundamental<Type>();
	}

	template <typename Type>
	static inline mpl::true_ is_base_type(Type**)
	{
		return mpl::true_();
	}

	template <typename Type>
	inline void print_value(Type* ptr_to_inst, mpl::false_) const { }

	template <typename Type>
	inline void print_value(Type* ptr_to_inst, mpl::true_) const
	{
		assert(ptr_to_inst != 0);
		// call the proper overload
		do_print_value(*ptr_to_inst);
	}

	inline void do_print_value(bool value) const
	{
		out_stream << (value ? "true" : "false") ;
	}

	inline void do_print_value(char value) const
	{
		out_stream << '"' ;
		do_print_simple_char(value);
		out_stream << '"' ;
	}

	static inline char get_hex_digit(unsigned char d)
	{
		switch(d)
		{
			case  0: return '0';
			case  1: return '1';
			case  2: return '2';
			case  3: return '3';
			case  4: return '4';
			case  5: return '5';
			case  6: return '6';
			case  7: return '7';
			case  8: return '8';
			case  9: return '9';
			case 10: return 'a';
			case 11: return 'b';
			case 12: return 'c';
			case 13: return 'd';
			case 14: return 'e';
			case 15: return 'f';
		}
		throw_exception();
		return '\0';
	}

	template <typename CharT>
	inline void do_print_simple_char(CharT c) const
	{
		switch(c)
		{
			case '"':	out_stream << "\\\""; break;
			case '/':	out_stream << "\\/"; break;
			case '\\':	out_stream << "\\\\"; break;
			case '\b':	out_stream << "\\b"; break;
			case '\f':	out_stream << "\\f"; break;
			case '\n':	out_stream << "\\n"; break;
			case '\r':	out_stream << "\\r"; break;
			case '\t':	out_stream << "\\t"; break;
			default:        out_stream << c;
		}
	}

	template <int CharSize>
	inline void do_print_char(
		char c, 
		int&, 
		unsigned int&,
		mpl::false_,
		mpl::int_<CharSize>
	) const
	{
		do_print_simple_char(c);
	}

	inline void do_print_char(
		char c, 
		int& remaining, 
		unsigned int& status,
		mpl::true_,
		mpl::int_<1>
	) const
	{
		// unknown status
		if(remaining == 0)
		{
			// the MSb is not set
			// and this is a single byte character
			if((c & 0x80) == 0x00)
			{
				// just print it
				do_print_simple_char(c);
			}
			else // this is a multi-byte character
			{
				// 11110 a 4-byte character
				if((c & 0xF8) == 0xF0)
				{
					remaining = 3;
					status = (c & 0x07);
				}
				// 1110 a 3-byte character
				else if((c & 0xF0) == 0xE0)
				{
					remaining = 2;
					status = (c & 0x0F);
				}
				// 110 a 2-byte character
				else if((c & 0xE0) == 0xC0)
				{
					remaining = 1;
					status = (c & 0x1F);
				}
				else throw_exception();
				// print the leading 
				out_stream << "\\u";
			}
		}
		else 
		{
			// in this stage it must be a 10 
			if((c & 0xC0) == 0x80)
			{
				status <<= 6;
				status |= (c & 0x3F);
			}
			else throw_exception();
			// if this is the end of the sequence
			if(--remaining == 0)
			{
				out_stream << 
					get_hex_digit((status >> 12) & 0xF) <<
					get_hex_digit((status >>  8) & 0xF) <<
					get_hex_digit((status >>  4) & 0xF) <<
					get_hex_digit((status >>  0) & 0xF) ;
			}
		}
	}

	template <int CharSize>
	inline void do_print_char(
		wchar_t c, 
		int&,
		unsigned int&,
		mpl::true_,
		mpl::int_<CharSize>
	) const
	{
		out_stream << 
			"\\u" <<
			get_hex_digit((c >> 12) & 0xF) <<
			get_hex_digit((c >>  8) & 0xF) <<
			get_hex_digit((c >>  4) & 0xF) <<
			get_hex_digit((c >>  0) & 0xF) ;
	}

	template <class CharT>
	inline void do_print_string(
		const ::std::basic_string<CharT>& str
	) const
	{
		out_stream << '"' ;
		typedef ::std::basic_string<CharT> string_type;
		typedef typename string_type::const_iterator iter;
		int remaining = 0;
		unsigned int status = 0;
		for(iter i=str.begin(),e=str.end();i!=e;++i)
		{
			do_print_char(
				*i, 
				remaining, 
				status,
				EncodeMBCs(),
				mpl::int_<sizeof(CharT)>()
			);
		}
		out_stream << '"' ;
	}

	template <typename T>
	inline void do_print_value(T* pointer) const
	{
		if(pointer)
		{
			// TODO: pointer serialization ?
			throw_exception();
		}
		else out_stream << "null";
	}

	template <typename Type>
	inline void do_print_value(const Type& value) const
	{
		out_stream << value ;
	}
};

#undef BOOST_MIRROR_JSON_SERIALIZER_STRING_RELATED_HELPER

template <class OutputStream, class EncodeMBCs>
inline json_serializer<OutputStream, EncodeMBCs> 
make_json_serializer(OutputStream& out_stream, EncodeMBCs)
{
	return json_serializer<OutputStream, EncodeMBCs>(out_stream);
}


} // namespace mirror
} // namespace boost

#endif //include guard

