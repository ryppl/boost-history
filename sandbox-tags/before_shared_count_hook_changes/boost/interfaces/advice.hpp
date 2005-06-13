// (C) Copyright Jonathan Turkanis 2004.
// (C) Copyright Christopher Diggins 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Based on the class CBaseAspect from the code accompanying the 
// article "Aspect-Oriented Programming & C++", by Christopher Diggins, 
// Dr. Dobb's Journal, Aug 2004, p. 53.

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_ADVICE_HPP_INCLUDED
#define BOOST_IDL_ADVICE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <exception>
#include <boost/implicit_cast.hpp>
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/rebind.hpp>

namespace boost { namespace interfaces {

namespace detail {

template<typename Category> struct advice_impl;

template<>
struct advice_impl<null_advice_tag> {
    template<typename Advice, typename Tuple>
    static void before( const Advice&, Tuple&, const char*, 
                        int arity, const char* params[] ) { }

    template<typename Advice, typename Tuple>
    static void after( const Advice&, Tuple&, const char*, 
                       int, const char*[] ) { }

    template<typename Advice, typename Tuple>
    static void finally( const Advice&, Tuple&, const char*, 
                         int, const char*[] ) { }

    template<typename Advice, typename Tuple>
    static void error( const Advice&, std::exception&, Tuple&, 
                       const char*, int, 
                       const char*[] ) { }

    template<typename Advice, typename Tuple>
    static bool cancel( const Advice&, Tuple&, const char*, 
                        int, const char*[] )
    { return false; }

    template<typename Advice, typename Result, typename Tuple>
    static Result transform( const Advice& adv, const Result& result, Tuple&, 
                             const char*, int, const char*[] )
    { return result; }
};

template<>
struct advice_impl<basic_advice_tag> {
    template<typename Advice, typename Tuple>
    static void before( const Advice& adv, Tuple& args, const char* function, 
                        int arity, const char* params[] )
    { adv.before(args, function, arity, params); }

    template<typename Advice, typename Tuple>
    static void after( const Advice& adv, Tuple& args, const char* function, 
                       int arity, const char* params[] )
    { adv.after(args, function, arity, params); }

    template<typename Advice, typename Tuple>
    static void finally( const Advice& adv, Tuple& args, const char* function, 
                         int arity, const char* params[] )
    { adv.finally(args, function, arity, params); }

    template<typename Advice, typename Tuple>
    static void error( const Advice& adv, std::exception& e, Tuple& args, 
                       const char* function, int arity, const char* params[] ) 
    { adv.error(e, args, function, arity, params); }

    template<typename Advice, typename Tuple>
    static bool cancel( const Advice& adv, Tuple& args, const char* function, 
                        int arity, const char* params[] )
    { return adv.cancel(args, function, arity, params); }

    template<typename Advice, typename Result, typename Tuple>
    static Result transform( const Advice&, const Result& result, Tuple&, 
                             const char*, int, const char*[] ) 
    { return result; }
};

template<>
struct advice_impl<transforming_advice_tag> 
    : advice_impl<basic_advice_tag> 
{
    template<typename Advice, typename Result, typename Tuple>
    static Result transform( const Advice& adv, const Result& result, 
                             Tuple& args, const char* function, int arity, 
                             const char* params[] ) 
    { return adv.transform(result, args, function, arity, params); }
};

} // End namespace detail.

template<typename Advice, typename Tuple>
void before( const Advice& adv, Tuple& args, const char* function, 
             int arity, const char* params[] )
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;
    impl::before(adv, args, function, arity, params);
}

template<typename Advice, typename Tuple>
void after( const Advice& adv, Tuple& args, const char* function, 
            int arity, const char* params[] )
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;
    impl::after(adv, args, function, arity, params);
}

template<typename Advice, typename Tuple>
void finally( const Advice& adv, Tuple& args, const char* function, 
              int arity, const char* params[] ) 
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;
    impl::finally(adv, args, function, arity, params);
}


template<typename Advice, typename Tuple>
void error( const Advice& adv, std::exception& e, Tuple& args, 
            const char* function, int arity, const char* params[] ) 
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;;
    impl::error(adv, e, args, function, arity, params);
}


template<typename Advice, typename Tuple>
bool cancel( const Advice& adv, Tuple& args, const char* function, 
             int arity, const char* params[] )
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;
    return impl::cancel(adv, args, function, arity, params);
}

template<typename Advice, typename Result, typename Tuple>
static Result transform( const Advice& adv, const Result& result, Tuple& args, 
                         const char* function, int arity, const char* params[] )
{ 
    typedef detail::advice_impl<
                typename advice_category<Advice>::type
            > impl;
    return impl::transform(adv, result, args, function, arity, params);
}

namespace detail {

template<typename Category, typename Base>
struct advice_base : Base {
    typedef Category category;

    template<typename Tuple>
    void before(Tuple&, const char*, int, const char*[]) const { }

    template<typename Tuple>
    void after(Tuple&, const char*, int, const char*[]) const { }

    template<typename Tuple>
    void finally(Tuple&, const char*, int arity, const char*[]) const { }

    template<typename Tuple>
    void error( std::exception&, Tuple&, const char*, 
                int, const char*[] ) const { }

    template<typename Tuple>
    bool cancel(Tuple&, const char*, int, const char*[]) const
    { return false; }

    template<typename Result, typename Tuple>
    Result transform( const Result& result, Tuple&, 
                      const char*, int, const char*[] ) const
    { return result; }
};

} // End namespace detail.

template<typename Base = boost::interfaces::detail::empty_base>
struct basic_advice 
    : detail::advice_base<basic_advice_tag, Base> 
    { };

template<typename Base = boost::interfaces::detail::empty_base>
struct transforming_advice 
    : detail::advice_base<transforming_advice_tag, Base> 
    { };

namespace detail {

template<bool IsTransforming1, bool IsTransforming2>
struct transform_impl;

template<>
struct transform_impl<false, false> {
    template<typename MergedAdvice, typename Result, typename Tuple>
    static Result execute( const MergedAdvice&, const Result& result, 
                           Tuple&, const char*, int, const char*[] )
    { return result; }
};

template<>
struct transform_impl<true, false> {
    template<typename MergedAdvice, typename Result, typename Tuple>
    static Result execute( const MergedAdvice& adv, const Result& result, 
                           Tuple& args, const char* function, int arity, 
                           const char* params[] )
    { 
        return transform(adv.first(), result, args, function, arity, params);
    }
};

template<>
struct transform_impl<false, true> {
    template<typename MergedAdvice, typename Result, typename Tuple>
    static Result execute( const MergedAdvice& adv, const Result& result, 
                           Tuple& args, const char* function, int arity, 
                           const char* params[] )
    { 
        return transform(adv.second(), result, args, function, arity, params); 
    }
};

template<>
struct transform_impl<true, true> {
    template<typename MergedAdvice, typename Result, typename Tuple>
    static Result execute( const MergedAdvice& adv, const Result& result, 
                           Tuple& args, const char* function, int arity, 
                           const char* params[] )
    { 
        return transform( 
                  adv.second(),
                  transform(adv.first(), result, args, function, arity, params),
                  args, function, arity, params 
               ); 
    }
};

} // End namespace detail.

template<typename First, typename Second>
struct merge_advice { 
    struct type : detail::rebind_advice<First, Second>::type {
        typedef typename
                combine_categories<
                    typename advice_category<First>::type,
                    typename advice_category<Second>::type
                >::type                                     category;
        typedef typename
                detail::rebind_advice<
                    First, Second
                >::type                                     first_type;
        typedef Second                                      second_type;

        const first_type& first() const { return *this; }
        const second_type& second() const { return *this; }

        template<typename Tuple>
        void before( Tuple& args, const char* function, 
                     int arity, const char* params[] ) const
        { 
            interfaces::before(second(), args, function, arity, params);
            interfaces::before(first(), args, function, arity, params);
        }

        template<typename Tuple>
        void after( Tuple& args, const char* function, 
                    int arity, const char* params[] ) const
        { 
            interfaces::after(first(), args, function, arity, params);
            interfaces::after(second(), args, function, arity, params);
        }

        template<typename Tuple>
        void finally( Tuple& args, const char* function, 
                      int arity, const char* params[] ) const
        { 
            interfaces::finally(first(), args, function, arity, params);
            interfaces::finally(second(), args, function, arity, params);
        }

        template<typename Tuple>
        void error( std::exception& e, Tuple& args, 
                    const char* function, int arity, 
                    const char* params[] ) const
        { 
            interfaces::error(first(), e, args, function, arity, params);
            interfaces::error(second(), e, args, function, arity, params);
        }

        template<typename Tuple>
        bool cancel( Tuple& args, const char* function, 
                     int arity, const char* params[] ) const
        { 
            return interfaces::cancel(second(), args, function, arity, params) ||
                   interfaces::cancel(first(), args, function, arity, params);
        }

        template<typename Result, typename Tuple>
        Result transform( const Result& result, Tuple& args, 
                          const char* function, int arity, 
                          const char* params[] ) const
        { 
            static const bool is_trans1 = 
                is_transforming_advice<First>::value;
            static const bool is_trans2 = 
                is_transforming_advice<Second>::value;
            typedef detail::transform_impl<is_trans1, is_trans2> impl;
            return impl::execute(*this, result, args, function, arity, params);
        }
    };
};

template<typename Advice>
struct merge_advice<Advice, detail::empty_base> {
    typedef Advice type;
};

template<typename Advice>
struct merge_advice<detail::empty_base, Advice> {
    typedef Advice type;
};

template<>
struct merge_advice<detail::empty_base, detail::empty_base>  {
    typedef detail::empty_base type;
};

} } // End namespace interfaces, aspects.

#endif // #ifndef BOOST_IDL_ADVICE_HPP_INCLUDED
