//----------------------------------------------------------------------------
// conversion.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_CONVERSION_HPP
#define BOOST_CONVERSION_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class CheckingPolicy>
    class allow_conversion_ : public CheckingPolicy
    {
    public:             // Types
        typedef CheckingPolicy                          checking_policy;
        typedef checking_policy                         base_type;
        typedef typename checking_policy::pointer_type  pointer_type;
        typedef pointer_type                            result_type;

    protected:          // Protected Interface
                        allow_conversion_(void)         { }

                        allow_conversion_(allow_conversion_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        allow_conversion_(allow_conversion_<U> const& rhs)
                            : base_type(static_cast<
                                typename allow_conversion_<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        allow_conversion_(U& p)
                            : base_type(p)              { }

                        template <typename U>
                        allow_conversion_(U const& p)
                            : base_type(p)              { }

                        template <typename U, typename V>
                        allow_conversion_(U& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        allow_conversion_(U const& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        allow_conversion_(U& p, V const& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        allow_conversion_(U const& p, V const& d)
                            : base_type(p, d)           { }

        static
        void            swap(allow_conversion_& lhs, allow_conversion_& rhs)
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    template <class CheckingPolicy>
    class disallow_conversion_ : public CheckingPolicy
    {
    public:             // Types
        typedef conversion_policy_tag                   policy_category;
        typedef CheckingPolicy                          checking_policy;
        typedef checking_policy                         base_type;
        typedef typename checking_policy::pointer_type  pointer_type;
        struct disallow_conversion_result
        {
            disallow_conversion_result(pointer_type const&) { }
        };
        typedef disallow_conversion_result              result_type;

    protected:          // Protected Interface
                        disallow_conversion_(void)      { }

                        disallow_conversion_(disallow_conversion_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        disallow_conversion_(disallow_conversion_<U> const& rhs)
                            : base_type(static_cast<typename
                                disallow_conversion_<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        disallow_conversion_(allow_conversion_<U> const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        disallow_conversion_(U& p)
                            : base_type(p)              { }

                        template <typename U>
                        disallow_conversion_(U const& p)
                            : base_type(p)              { }

                        template <typename U, typename V>
                        disallow_conversion_(U& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        disallow_conversion_(U const& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        disallow_conversion_(U& p, V const& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        disallow_conversion_(U const& p, V const& d)
                            : base_type(p, d)           { }

        static
        void            swap(
                            disallow_conversion_& lhs,
                            disallow_conversion_& rhs
                        )
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    struct disallow_conversion
    {
        typedef conversion_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef disallow_conversion_<T> type;
        };
    };
    //------------------------------------------------------------------------
    struct allow_conversion
    {
        typedef conversion_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef allow_conversion_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_CONVERSION_HPP

