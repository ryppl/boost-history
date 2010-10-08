/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTORS_HPP_JOFA_080315
#define BOOST_ITL_FUNCTORS_HPP_JOFA_080315

#include <functional>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/unon.hpp>
#include <boost/itl/type_traits/is_set.hpp>
#include <boost/itl/type_traits/has_set_semantics.hpp>

namespace boost{namespace itl
{
    // ------------------------------------------------------------------------
    template <typename Type> struct neutron_based_inplace_combine 
        : public std::binary_function<Type&, const Type&, void>
    {
        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    // ------------------------------------------------------------------------
    template <typename Type> struct unon_based_inplace_combine 
        : public std::binary_function<Type&, const Type&, void>
    {
        static Type neutron() { return boost::itl::unon<Type>::value(); }
    };

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_identity 
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_identity<Type> type;
        void operator()(Type& object, const Type& operand)const{}
    };

    template<>
    inline std::string unary_template_to_string<inplace_identity>::apply() 
    { return "i="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_erasure 
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_erasure<Type> type;

        void operator()(Type& object, const Type& operand)const
        { 
            if(object == operand)
                object = Type();
        }
    };

    template<>
    inline std::string unary_template_to_string<inplace_erasure>::apply() 
    { return "0="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_plus 
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_plus<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object += operand; }

        static void version(Type& object){}
    };

    template<>
    inline std::string unary_template_to_string<inplace_plus>::apply() { return "+="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_minus 
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_minus<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object -= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_minus>::apply() { return "-="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_add
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_bit_add<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object |= operand; }

        static void version(Type& object){}
    };

    template<>
    inline std::string unary_template_to_string<inplace_bit_add>::apply() { return "b|="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_subtract
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_bit_subtract<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object &= ~operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_bit_subtract>::apply() { return "b-="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_and
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_bit_and<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object &= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_bit_and>::apply() { return "b&="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_xor
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_bit_xor<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object ^= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_et
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_et<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object &= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_et>::apply() { return "&="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_caret
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_caret<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object ^= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_caret>::apply() { return "^="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_insert
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_insert<Type> type;

        void operator()(Type& object, const Type& operand)const
        { insert(object,operand); }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_insert>::apply() { return "ins="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_erase
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_erase<Type> type;

        void operator()(Type& object, const Type& operand)const
        { erase(object,operand); }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_erase>::apply() { return "ers="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_star
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_star<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object *= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_star>::apply() { return "*="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_slash
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_slash<Type> type;

        void operator()(Type& object, const Type& operand)const
        { object /= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_slash>::apply() { return "/="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_max
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_max<Type> type;

        void operator()(Type& object, const Type& operand)const
        {
            if(object < operand)
                object = operand;
        }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_max>::apply() { return "max="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_min
        : public neutron_based_inplace_combine<Type>
    {
        typedef inplace_min<Type> type;

        void operator()(Type& object, const Type& operand)const
        {
            if(object > operand)
                object = operand;
        }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_min>::apply() { return "min="; }

    //--------------------------------------------------------------------------
    // Inter_section functor
    //--------------------------------------------------------------------------
    template<class Type> struct inter_section
        : public neutron_based_inplace_combine<Type>
    {
        typedef typename boost::mpl::
            if_<has_set_semantics<Type>,
                itl::inplace_et<Type>, 
                itl::inplace_plus<Type> 
               >::type
            type;

        void operator()(Type& object, const Type& operand)const
        { 
            type()(object, operand);
        }
    };

    //--------------------------------------------------------------------------
    // Inverse functor
    //--------------------------------------------------------------------------
    template<class Functor> struct inverse;

    template<class Type> 
    struct inverse<itl::inplace_plus<Type> >
    { typedef itl::inplace_minus<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_minus<Type> >
    { typedef itl::inplace_plus<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_bit_add<Type> >
    { typedef itl::inplace_bit_subtract<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_bit_subtract<Type> >
    { typedef itl::inplace_bit_add<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_et<Type> >
    { typedef itl::inplace_caret<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_caret<Type> >
    { typedef itl::inplace_et<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_bit_and<Type> >
    { typedef itl::inplace_bit_xor<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_bit_xor<Type> >
    { typedef itl::inplace_bit_and<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_star<Type> >
    { typedef itl::inplace_slash<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_slash<Type> >
    { typedef itl::inplace_star<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_max<Type> >
    { typedef itl::inplace_min<Type> type; };

    template<class Type> 
    struct inverse<itl::inplace_min<Type> >
    { typedef itl::inplace_max<Type> type; };


    //--------------------------------------------------------------------------
    // Inverse inter_section functor
    //--------------------------------------------------------------------------
    template<class Type> 
    struct inverse<itl::inter_section<Type> >
        : public neutron_based_inplace_combine<Type>
    {
        typedef typename boost::mpl::
            if_<has_set_semantics<Type>,
                itl::inplace_caret<Type>, 
                itl::inplace_minus<Type> 
               >::type
            type;

        void operator()(Type& object, const Type& operand)const
        { 
            type()(object, operand);
        }
    };


    //--------------------------------------------------------------------------
    // Positive or negative functor trait
    //--------------------------------------------------------------------------

    // A binary operation - is negative (or inverting) with respect to the
    // neutral element iff it yields the inverse element if it is applied to the 
    // identity element:
    // 0 - x = -x
    // For a functor that wraps the inplace of op-assign version this is 
    // equivalent to
    //
    // T x = ..., y;
    // y = Functor::neutron();
    // Functor()(y, x); // y == inverse_of(x) 

    template<class Functor> struct is_negative;

    template<class Functor> 
    struct is_negative
    {
        typedef is_negative<Functor> type;
        BOOST_STATIC_CONSTANT(bool, value = false);
    };

    template<class Type> 
    struct is_negative<itl::inplace_minus<Type> >
    {
        typedef is_negative type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<class Type> 
    struct is_negative<itl::inplace_bit_subtract<Type> >
    {
        typedef is_negative type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    //--------------------------------------------------------------------------
    // Pro- or in-version functor
    //--------------------------------------------------------------------------
    template<class Combiner> struct conversion;

    template<class Combiner> 
    struct conversion
    { 
        typedef conversion<Combiner> type;
        typedef typename
            remove_const<
                typename remove_reference<typename Combiner::first_argument_type
                >::type
            >::type
            argument_type;
        // The proversion of an op-assign functor o= lets the value unchanged
        // (0 o= x) == x;
        // Example += :  (0 += x) == x
        static argument_type proversion(const argument_type& value)
        { 
            return value; 
        } 

        // The inversion of an op-assign functor o= inverts the value x
        // to it's inverse element -x
        // (0 o= x) == -x;
        // Example -= :  (0 -= x) == -x
        static argument_type inversion(const argument_type& value)
        {
            argument_type inverse = Combiner::neutron();
            Combiner()(inverse, value);
            return inverse;
        }
    };

    template<class Combiner> struct version : public conversion<Combiner>
    {
        typedef    version<Combiner> type;
        typedef conversion<Combiner> base_type;
        typedef typename base_type::argument_type argument_type;

        argument_type operator()(const argument_type& value)
        { return base_type::proversion(value); } 
    };

    template<>struct version<itl::inplace_minus<short      > >{short       operator()(short       val){return -val;}};
    template<>struct version<itl::inplace_minus<int        > >{int         operator()(int         val){return -val;}};
    template<>struct version<itl::inplace_minus<long       > >{long        operator()(long        val){return -val;}};
    template<>struct version<itl::inplace_minus<long long  > >{long long   operator()(long long   val){return -val;}};
    template<>struct version<itl::inplace_minus<float      > >{float       operator()(float       val){return -val;}};
    template<>struct version<itl::inplace_minus<double     > >{double      operator()(double      val){return -val;}};
    template<>struct version<itl::inplace_minus<long double> >{long double operator()(long double val){return -val;}};

    template<class Type> 
    struct version<itl::inplace_minus<Type> > : public conversion<itl::inplace_minus<Type> >
    { 
        typedef    version<itl::inplace_minus<Type> > type;
        typedef conversion<itl::inplace_minus<Type> > base_type;
        typedef typename base_type::argument_type argument_type;

        Type operator()(const Type& value)
        {
            return base_type::inversion(value);
        } 
    };

}} // namespace itl boost

#endif


