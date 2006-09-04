// ----------------------------------------------------------------------------
// ******
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_TRANSLATOR_IMPLEMENTATION_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_TRANSLATOR_IMPLEMENTATION_HPP_INCLUDED

namespace boost { namespace property_tree
{

    namespace detail
    {
        
        // Data-to-string converter for std::string
        std::string data_to_string(const std::string &data)
        {
            return data;
        }

        // Data-to-string converter for std::basic_string<Ch>
        template<class Ch>
        std::string data_to_string(const std::basic_string<Ch> &data)
        {
            return narrow(data.c_str());
        }

        ////////////////////////////////////////////////////////////////////////////
        // Helpers

        template<class T>
        struct array_to_pointer_decay
        {
            typedef T type;
        };

        template<class T, std::size_t N>
        struct array_to_pointer_decay<T[N]>
        {
            typedef const T *type;
        };

        ////////////////////////////////////////////////////////////////////////////
        // Extractor and inserter

        template<class Ch, class T>
        struct extractor
        {
            inline bool operator()(const std::basic_string<Ch> &data, 
                                   T &extracted,
                                   const std::locale &loc) const
            {
                std::basic_istringstream<Ch> stream(data);
                stream.imbue(loc);
                stream.unsetf(std::ios::skipws);
                stream >> extracted >> std::ws;
                return stream.eof() && !stream.fail() && !stream.bad();
            }
        };

        template<class Ch>
        struct extractor<Ch, std::basic_string<Ch> >
        {
            inline bool operator()(const std::basic_string<Ch> &data, 
                                   std::basic_string<Ch> &extracted,
                                   const std::locale &loc) const
            {
                extracted = data;
                return true;
            }
        };

        template<class Ch, class T>
        struct inserter
        {
            inline bool operator()(std::basic_string<Ch> &data, 
                                   const T &to_insert,
                                   const std::locale &loc) const
            {
                typedef typename detail::array_to_pointer_decay<T>::type T2;
                std::basic_ostringstream<Ch> stream;
                stream.imbue(loc);
                if (std::numeric_limits<T2>::is_specialized)
                    stream.precision(std::numeric_limits<T2>::digits10 + 1);
                stream << to_insert;
                data = stream.str();
                return !stream.fail() && !stream.bad();
            }
        };

        template<class Ch>
        struct inserter<Ch, std::basic_string<Ch> >
        {
            inline bool operator()(std::basic_string<Ch> &data, 
                                   const std::basic_string<Ch> &to_insert,
                                   const std::locale &loc) const
            {
                data = to_insert;
                return true;
            }
        };

    }

    translator::translator()
    {
    }

    translator::translator(const std::locale &loc):
        m_locale(loc)
    {
    }

    template<class Ptree, class T> 
    bool translator::get_value(const Ptree &pt, T &value) const
    {
        typedef typename Ptree::data_type::value_type Ch;
        return detail::extractor<Ch, T>()(pt.data(), value, m_locale);
    }

    template<class Ptree, class T> 
    bool translator::put_value(Ptree &pt, const T &value) const
    {

        typedef typename Ptree::data_type::value_type Ch;

        // Make sure that no pointer other than char_type * is allowed
        BOOST_STATIC_ASSERT((is_pointer<T>::value == false ||
                             is_same<Ch, typename remove_const<typename remove_pointer<T>::type>::type>::value == true));

        return detail::inserter<Ch, T>()(pt.data(), value, m_locale);

    }

} }

#endif
