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
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/unon.hpp>

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
        void operator()(Type& object, const Type& operand)const{}
        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_identity>::apply() 
    { return "i="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_erasure 
        : public neutron_based_inplace_combine<Type>
    {
        void operator()(Type& object, const Type& operand)const
        { 
            if(object == operand)
                object = Type();
        }
        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_erasure>::apply() 
    { return "0="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_plus 
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;

        void operator()(Type& object, const Type& operand)const
        { object += operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_plus>::apply() { return "+="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_minus 
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;
        void operator()(Type& object, const Type& operand)const
        { object -= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_minus>::apply() { return "-="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_add
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;

        void operator()(Type& object, const Type& operand)const
        { object |= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_bit_add>::apply() { return "b|="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_subtract
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;
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
        typedef Type type;

        void operator()(Type& object, const Type& operand)const
        { object &= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_bit_and>::apply() { return "b&="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_bit_xor
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;
        void operator()(Type& object, const Type& operand)const
        { object ^= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_et
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;

        void operator()(Type& object, const Type& operand)const
        { object &= operand; }
    };

    template<>
    inline std::string unary_template_to_string<inplace_et>::apply() { return "&="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_caret
        : public neutron_based_inplace_combine<Type>
    {
        typedef Type type;
        void operator()(Type& object, const Type& operand)const
        { object ^= operand; }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inplace_caret>::apply() { return "^="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inserter
        : public neutron_based_inplace_combine<Type>
    {
        void operator()(Type& object, const Type& operand)const
        { insert(object,operand); }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<inserter>::apply() { return "ins="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct eraser
        : public neutron_based_inplace_combine<Type>
    {
        void operator()(Type& object, const Type& operand)const
        { erase(object,operand); }

        static Type neutron() { return boost::itl::neutron<Type>::value(); }
    };

    template<>
    inline std::string unary_template_to_string<eraser>::apply() { return "ers="; }

    // ------------------------------------------------------------------------
    template <typename Type> struct inplace_star
        : public neutron_based_inplace_combine<Type>
    {
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
    // Inverse functor
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


}} // namespace itl boost

#endif


