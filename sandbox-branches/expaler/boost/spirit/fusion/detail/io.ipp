/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_TUPLEIO_IPP)
#define BOOST_FUSION_TUPLE_TUPLEIO_IPP

#include <iostream>
#include <cctype>

namespace boost { namespace fusion { namespace tuple_detail
{
    template <class F, int Start, int Len, class T>
    struct printread_tuple_type
    {
        static void run(F& f, T& x)
        {

            // print/read the current elem
            f.var(get<Start>(x));

            // if not the last elem, print/read a delimiter
            if (Start+1 != Len)
                f.constant(string_ios_manip<
	                tuple_delimiter_tag
	              , typename F::stream_type>(f.stream).get(" "));

            printread_tuple_type<F,Start+1, Len, T>::run(f,x);
        }
    };

    template <class F, int Len, class T>
    struct printread_tuple_type<F, Len, Len, T>
    {
        static void run(F& f, T&)
        {
            // print the closing delimiter
            f.constant(string_ios_manip<
	            tuple_close_tag
	          , typename F::stream_type>(f.stream).get(")"));
        }
    };

    template <class F, class T>
    void printread_tuple(F& f, T& x)
    {
        // print the opening delimiter
        f.constant(tuple_detail::string_ios_manip<
	        tuple_detail::tuple_open_tag
	      , typename F::stream_type>(f.stream).get("("));

        tuple_detail::printread_tuple_type<F, 0, tuple_size<T>::value, T>::
            run(f,x);
    }

    // helper functor for printing

    template <class O>
    struct print_tuple_functor
    {
        O& stream;

        typedef O stream_type;

        print_tuple_functor(O& o_): stream(o_) {}

        // print out an actual tuple element
        template <class T>
        void var(const T& x) {stream << x;}

        // print out a delimiter
        template <class T>
        void constant(const T& x) {stream << x;}
    };

    // helper functor for reading

    template <class I>
    struct read_tuple_functor
    {
        I& stream;

        typedef I stream_type;
        read_tuple_functor(I& i_): stream(i_) {}

        // read an actual tuple element
        template <class T>
        void var(T& x)
        {
            stream >> x;
        }

        // try to read a particular delimiter string
        template <class T>
        void constant(const T& x)
        {
            ws(stream);
            for (typename T::const_iterator i = x.begin(); i != x.end(); ++i)
            {
                if (!std::isspace(*i))
                {
                    if (stream.get() != *i)
                    {
	                    stream.unget();
	                    stream.setstate(std::ios::failbit);
                    }
                }
            }
        }
    };

}}} // namespace boost::fusion::tuple_detail

#endif // defined(BOOST_FUSION_TUPLE_TUPLEIO_IPP)
