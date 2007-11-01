/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_IMPL_HPP_
#define _BOOST_OVERLOAD_IMPL_HPP_

#include "detail/overload_base.hpp"
#include "detail/trait_by_index.hpp"
#include "detail/trait_by_signature.hpp"
#include "detail/trait_by_functor.hpp"
#include "detail/member_function_form.hpp"

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4127) // conditional expression is constant.
#endif

// Workaround for MSVC < 8.0 tested on MSVC 7.1 
#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)
    #include "detail/function_trait.hpp"
#endif


namespace boost{ namespace overloads{

using detail::extent;
using detail::signature;
using detail::function;
using detail::index;
using detail::has_signature;
using detail::has_functor_signature;

using detail::const_;
using detail::non_const;

///////////////////////////////////////////////////////////////////////////////
// overload class template

template< 
    typename Sig0, 
    BOOST_PP_ENUM_SHIFTED(BOOST_OVERLOAD_LIMIT, OVL_TEMPL_PARAMS, Sig)
>
//template< typename Sig0, typename Sig1 = no_signature, ... >
class overload
    : public detail::overload_base<0,BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)>

{
  public:
    ///////////////////////////////////////////////////////////////////////////
    // type traits
    typedef overload<BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)> this_type;
    typedef 
        detail::overload_base<0,BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)>
        base_type;

    // static const unsigned int extent = overloads::extent<this_type>::value;
    static const unsigned int extent;

    template< unsigned int N >
    struct signature
    {
        typedef typename overloads::signature<N, this_type>::type type;
    };

    template< unsigned int N >
    struct function
    {
        typedef typename overloads::function< N, this_type >::type type;
    };

    template< typename Sig >
    struct index
    {
        static const unsigned int value 
            = overloads::index<Sig, this_type>::value;
    };

    template< typename Sig >
    struct has_signature
    {
        static const bool value 
            = overloads::has_signature<Sig, this_type>::value;
    };

    template< typename F >
    struct has_functor_signature
    {
        static const bool value 
            = overloads::has_functor_signature<F, this_type>::value;
    };

  private:
    ///////////////////////////////////////////////////////////////////////////
    // Workaround for MSVC < 8.0 tested on MSVC 7.1 
    // types used in the declaration of the function call operators
    
#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)  
    typedef
        typename detail::zero_arity_signature<
            BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)
        >::type
        zero_arity_signature_type;

    BOOST_PP_REPEAT(BOOST_OVERLOAD_LIMIT, OVL_REAL_OR_DUMMY_SIG_M, unused)
#endif

  public:
    overload()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    // constructors and methods that supports signature deduction
 
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_OVERLOAD_LIMIT, OVL_CTOR, unused)

    template< typename F >
    this_type& set( F const& _f,
                    typename boost::disable_if<
                        boost::is_pointer<F>
                    >::type* = 0 )
    {
        set_impl(_f);
        return *this;
    }

    BOOST_PP_REPEAT_FROM_TO(2, BOOST_OVERLOAD_LIMIT, OVL_SET, unused)

    ///////////////////////////////////////////////////////////////////////////
    // Workaround for MSVC < 8.0 tested on MSVC 7.1 
    // function call operators
 
#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)  
	typename detail::func_trait< zero_arity_signature_type >::result_type
	operator() ()
    {
		return boost::function<zero_arity_signature_type>::operator()();
	}

	BOOST_PP_REPEAT_FROM_TO(0, BOOST_OVERLOAD_LIMIT, OVL_FUNC_CALL_OPERATORS, Sig)
#endif

    ///////////////////////////////////////////////////////////////////////////
    // swap two boost::functions

    template< typename Sig >
    void swap_function( boost::function<Sig>& _f )
    {
        boost::function<Sig>::swap(_f);
    }

    ///////////////////////////////////////////////////////////////////////////
    // clear all the callable targets

    void clear_all()
    {
        clear_all_impl<typename this_type::base_type>(this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // return true if all the callable targets are empty

    bool empty_all()
    {
        return empty_all_impl<typename this_type::base_type>(this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // signature based methods

    template< typename Sig >
    const boost::function<Sig>& get() const
    {
         return *this;
    }

    template< typename Sig >
    boost::function<Sig>& get()
    {
         return *this;
    }

    template<typename Sig>
    this_type& set( Sig* _f_ptr )
    {
        get<Sig>() = _f_ptr;
        return *this;
    }

    template<typename Sig>
    this_type& set( typename detail::memb_func_form<Sig>::type _mf_ptr )
    {
        get<Sig>() = _mf_ptr;
        return *this;
    }

    template<typename Sig>
    this_type& set( typename detail::memb_func_form<Sig, const_>::type _mf_ptr )
    {
        get<Sig>() = _mf_ptr;
        return *this;
    }

    template<typename Sig, typename const_qlf >
    this_type& set( typename detail::memb_func_form<Sig, const_qlf>::type _mf_ptr )
    {
        get<Sig>() = _mf_ptr;
        return *this;
    }

    template< typename Sig, typename F >
    this_type& set( F const& _f, 
                    typename boost::disable_if<
                        detail::is_ptr_or_memb_ptr<F>
                    >::type* = 0 )
    {
        get<Sig>() = _f;
        return *this;
    }

    template< typename Sig >
    bool empty() const
    {
        return boost::function<Sig>::empty();
    }

    template< typename Sig >
    void clear()
    {
        boost::function<Sig>::clear();
    }

    ///////////////////////////////////////////////////////////////////////////
    // index based methods

    template< unsigned int N >
    const typename function<N>::type& get() const
    {
        return *this;
    }

    template< unsigned int N >
    typename function<N>::type& get()
    {
        return *this;
    }

    template< unsigned int N, typename F >
    this_type& set( F const& _f )
    {
        get<N>() = _f;
        return *this;
    }

    template< unsigned int N >
    bool empty() const
    {
        typedef typename function<N>::type function_type;
        return function_type::empty();
    }

    template< unsigned int N >
    void clear()
    {
        typedef typename function<N>::type function_type;
        function_type::clear();
    }

  private:
    ///////////////////////////////////////////////////////////////////////////
    // function set_impl
    // 

    template< typename Sig >
    void set_impl( boost::function<Sig> const& _f )
    {
        boost::function<Sig>::operator=(_f);
    }

    template< typename F >
    void set_impl( 
        F const& _f, 
        typename boost::enable_if< has_functor_signature<F> >::type* = 0
    )
    {
        typedef
            typename
            detail::base_by_functor<F, this_type>::type
            base_type;

        set_impl<F, base_type>(_f, this);
    }

    template< typename F, typename Base >
    void set_impl( F const& _f, Base const* )
    {
        //std::cout << "set_impl" << std::endl;
        typedef
            typename
            detail::base_by_functor<F, Base>::type
            base_type;
        typedef typename Base::signature_type signature_type;

        set<signature_type>(_f);
        set_impl<F, base_type>(_f, this);
    }

    template< typename F, typename Base>
    void set_impl( F const& , 
                   detail::final_overload_base const* )
    {
        //std::cout << "final" << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    // function clear_all__impl

    template< typename Base >
    void clear_all_impl(Base const* )
    {
        typedef typename Base::base_type          base_type;
        typedef typename Base::signature_type     signature_type;

        clear<signature_type>();
        clear_all_impl<base_type>(this);
    }

    template<typename Base>
    void clear_all_impl(detail::final_overload_base const* )
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    // function empty_all__impl

    template< typename Base >
    bool empty_all_impl(Base const* )
    {
        typedef typename Base::base_type          base_type;
        typedef typename Base::signature_type     signature_type;

        return ( empty<signature_type>() && empty_all_impl<base_type>(this) );
    }

    template<typename Base>
    bool empty_all_impl(detail::final_overload_base const*)
    {
        return true;
    }
    ///////////////////////////////////////////////////////////////////////////
    // return a pointer to this object casted to the correct base pointer 
    // for a generic functor

    template< typename F >
    boost::function<typename detail::signature_by_functor<F, this_type>::type>*
    base_ptr( F const& )
    {
        return this;
    }

}; // end class overload

// defined out of the class for compatibility with gcc 3.4 and intel 9.1
template< BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, typename Sig) >
const unsigned int 
overload<BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)>::extent 
    = overloads::extent< 
          overload<BOOST_PP_ENUM_PARAMS(BOOST_OVERLOAD_LIMIT, Sig)> 
      >::value;

} // end overloads namespace

using overloads::overload;

} // end boost namespace


#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif // _BOOST_OVERLOAD_IMPL_HPP_


