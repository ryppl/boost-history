/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_MATCH_ATTR_TRAITS_IPP)
#define BOOST_SPIRIT_MATCH_ATTR_TRAITS_IPP

namespace boost { namespace spirit {

    namespace impl
    {
        template <typename T>
        struct match_attr_traits
        {
            //  case where src *IS* convertible to T (dest)
            static void
            convert(boost::optional<T>& dest, T const& src)
            { dest.reset(src); }

            //  case where src *IS NOT* convertible to T (dest)
            static void
            convert(boost::optional<T>& dest, .../*src*/)
            { dest.reset(); }

            template <typename OtherMatchT>
            static void
            copy(boost::optional<T>& dest, OtherMatchT const& src)
            {
                if (src.has_valid_attribute())
                    convert(dest, src.value());
            }

            template <typename OtherMatchT>
            static void
            assign(boost::optional<T>& dest, OtherMatchT const& src)
            {
                if (src.has_valid_attribute())
                    convert(dest, src.value());
                else
                    dest.reset();
            }
        };
    }

}} // namespace boost::spirit

#endif

