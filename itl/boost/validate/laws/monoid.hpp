/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_MONOID_HPP_JOFA_070411
#define BOOST_ICL_MONOID_HPP_JOFA_070411

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace icl
{

    /*  Monoid: (M,+,0) with .+.: M x M -> M   has these axioms
        (1) Associativity
        (2) Neutral element
        A commutative monoid or abelian monoid has also
        (3) Commutativity
    */

    template <typename Type>
    class AdditionNeutrality 
        : public Law<AdditionNeutrality<Type>, LOKI_TYPELIST_1(Type), LOKI_TYPELIST_1(Type)> 
    {
    public:
        std::string name()const { return "Addition Neutrality"; }
        std::string formula()const { return "a + 0 == 0"; }

        std::string typeString()const
        {
            return "Neutrality<"+type_to_string<Type>::apply()+",+,0>";
        }

    public:
        bool holds()
        {
            Type inVal = this->template getInputValue<0>();
            Type outVal = inVal + Type();
            this->template setOutputValue<0>(outVal);
            return inVal == outVal;
        }

        bool debug_holds(){ return holds(); }

        size_t size()const;

        void setValue(const Type& inVal) { this->template setInputValue<0>(inVal); }
        Type getResult()const { return this->template getOutputValue<0>(); }
    };

    template <> size_t AdditionNeutrality<int>::size()const 
    { return getInputValue<0>(); }

    template <class Type> size_t AdditionNeutrality<Type>::size()const 
    { return this->template getInputValue<0>().size(); };

    template <typename Type>
    class AdditionCommutativity 
        : public Law<AdditionCommutativity<Type>, LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)> 
    {
        /** a + b == b + a 
        Input  = (a := inVal1, b := inVal2)
        Output = (sum_lhs, sum_rhs)
        */
    public:
        std::string name()const { return "Addition Commutativity"; }
        std::string formula()const { return "a + b == b + a"; }

        std::string typeString()const
        {
            return "Commutativity<"+type_to_string<Type>::apply()+",+>";
        }

    public:
        enum InputVarIndex  { operand_a, operand_b };
        enum OutputVarIndex { lhs_sum, rhs_sum };

        void setOperand_a(const Type& inVal) { this->template setInputValue<operand_a>(inVal); }
        void setOperand_b(const Type& inVal) { this->template setInputValue<operand_b>(inVal); }
        void setOperands(const Type& inVal_a, const Type& inVal_b) 
        { this->template setInputValue<operand_b>(inVal_a); this->template setInputValue<operand_b>(inVal_b); }

        Type getLhsSum()const { return this->template getOutputValue<lhs_sum>(); }
        Type getRhsSum()const { return this->template getOutputValue<rhs_sum>(); }

        bool holds()
        {
            this->template setOutputValue<lhs_result>(this->template getInputValue<operand_a>() + this->template getInputValue<operand_b>());
            this->template setOutputValue<rhs_result>(this->template getInputValue<operand_b>() + this->template getInputValue<operand_a>());
            return this->template getOutputValue<lhs_result>() == this->template getOutputValue<rhs_result>();
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };


    template <typename TypeA, typename TypeB>
    class MixedAdditionCommutativity 
        : public Law<MixedAdditionCommutativity<TypeA, TypeB>, 
                     LOKI_TYPELIST_2(TypeA,TypeB), LOKI_TYPELIST_2(TypeB,TypeB)> 
    {
        /** a + b == b + a 
        Input  = (a := inVal1, b := inVal2)
        Output = (sum_lhs, sum_rhs)
        */
    public:
        std::string name()const { return "Mixed Addition Commutativity"; }
        std::string formula()const { return "a + b == b + a for A a; B b; +: A x B -> B"; }

        std::string typeString()const
        {
            return "Commutativity<"+type_to_string<TypeA>::apply()+","
                                   +type_to_string<TypeB>::apply()+",+>";
        }

    public:
        enum InputVarIndex  { operand_a, operand_b };
        enum OutputVarIndex { lhs_sum, rhs_sum };

        void setOperand_a(const TypeA& inVal) { this->template setInputValue<operand_a>(inVal); }
        void setOperand_b(const TypeB& inVal) { this->template setInputValue<operand_b>(inVal); }
        void setOperands(const TypeA& inVal_a, const TypeB& inVal_b) 
        { this->template setInputValue<operand_b>(inVal_a); this->template setInputValue<operand_b>(inVal_b); }

        TypeB getLhsSum()const { return this->template getOutputValue<lhs_sum>(); }
        TypeB getRhsSum()const { return this->template getOutputValue<rhs_sum>(); }

        bool holds()
        {
            this->template setOutputValue<lhs_result>(this->template getInputValue<operand_a>() + this->template getInputValue<operand_b>());
            this->template setOutputValue<rhs_result>(this->template getInputValue<operand_b>() + this->template getInputValue<operand_a>());
            return this->template getOutputValue<lhs_result>() == this->template getOutputValue<rhs_result>();
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return value_size<TypeA>::get(this->template getInputValue<operand_a>())+
                   value_size<TypeB>::get(this->template getInputValue<operand_b>());
        }

    };


    // ---------------------------------------------------------------------------
    // Inplace variant of laws for operator o=
    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Accumulator = inplace_plus, template<class>class IdentityElement = identity_element>
    class InplaceNeutrality 
        : public Law<InplaceNeutrality<Type,Accumulator,IdentityElement>, 
                     LOKI_TYPELIST_1(Type), LOKI_TYPELIST_1(Type)>
    {
        /** a o 0 == a computed as
        l=a; l o= 0; => l==a 
        Input  = (a := inVal1)
        Output = (lhs_result)
        */
    public:
        std::string name()const { return "Inplace Op Neutrality"; }
        std::string formula()const { return "a o 0 == a 'inplace'"; }

        std::string typeString()const
        {
            return "Neutrality<"+type_to_string<Type>::apply()+","
                                +unary_template_to_string<Accumulator>::apply()+","
                                +unary_template_to_string<IdentityElement>::apply()+">";
        }

    public:

        bool holds()
        {
            Type lhs = this->template getInputValue<operand_a>();
            Accumulator<Type>()(lhs, identity_element<Type>()());
            this->template setOutputValue<lhs_result>(lhs);
            return lhs == this->template getInputValue<operand_a>();
        }

        bool debug_holds(){ return holds(); }

    };


    template <typename Type, 
              template<class>class Accumulator = inplace_plus, 
              template<class>class Equality = icl::std_equal>
    class InplaceAssociativity 
        : public Law<InplaceAssociativity<Type,Accumulator,Equality>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** (a o b) o c == a o (b o c) 'inplace'
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (sum_lhs, sum_rhs)
        */
    public:
        std::string name()const { return "Inplace Associativity"; }
        std::string formula()const { return "(a o b) o c == a o (b o c) 'inplace'"; }

        std::string typeString()const
        {
            return "Associativity<"+type_to_string<Type>::apply()+","
                                   +unary_template_to_string<Accumulator>::apply()+","
                                   +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>());
        }

        bool holds()
        {
            Type lhs = this->template getInputValue<operand_a>();
            Accumulator<Type>()(lhs, this->template getInputValue<operand_b>());
            Accumulator<Type>()(lhs, this->template getInputValue<operand_c>());

            Type rhs = this->template getInputValue<operand_a>();
            Type b_plus_c = this->template getInputValue<operand_b>();
            Accumulator<Type>()(b_plus_c, this->template getInputValue<operand_c>());
            Accumulator<Type>()(rhs, b_plus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        {
            std::cout << "--- function debug_holds -----------------------------\n";
            std::cout << "          a = " << this->template getInputValue<operand_a>() << std::endl;
            std::cout << "          b = " << this->template getInputValue<operand_b>() << std::endl;
            std::cout << "          c = " << this->template getInputValue<operand_c>() << std::endl;

            Type lhs = this->template getInputValue<operand_a>();
            Accumulator<Type>()(lhs, this->template getInputValue<operand_b>());
            std::cout << " a o b      = " << lhs << std::endl;

            Accumulator<Type>()(lhs, this->template getInputValue<operand_c>());
            std::cout << "(a o b) o c = " << lhs << std::endl;

            Type rhs      = this->template getInputValue<operand_a>();
            Type b_plus_c = this->template getInputValue<operand_b>();
            Accumulator<Type>()(b_plus_c, this->template getInputValue<operand_c>());
            std::cout << "     b o c  = " << b_plus_c << std::endl;
            Accumulator<Type>()(rhs, b_plus_c);
            std::cout << "a o (b o c) = " << rhs << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);
            std::cout << "------------------------------------------------------\n";

            return Equality<Type>()(lhs, rhs);
        }

    };


    template <typename Type, template<class>class Accumulator = inplace_plus>
    class InplaceCommutativity 
        : public Law<InplaceCommutativity<Type,Accumulator>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a o b == b o a computed as
        lhs=a; lhs+=b; rhs=b; rhs+=a => lhs==rhs 
        Input  = (a := inVal1, b := inVal2)
        Output = (sum_lhs, sum_rhs)
        */
    public:
        std::string name()const { return "Commutativity"; }
        std::string formula()const { return "a o b == b o a 'inplace'"; }

        std::string typeString()const
        {
            return "Commutativity<"+type_to_string<Type>::apply()+","
                                   +unary_template_to_string<Accumulator>::apply()+">";
        }

    public:

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }

        bool holds()
        {
            Type lhs = this->template getInputValue<operand_a>();
            Accumulator<Type>()(lhs, this->template getInputValue<operand_b>());
            Type rhs = this->template getInputValue<operand_b>();
            Accumulator<Type>()(rhs, this->template getInputValue<operand_a>());

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
        { 
            std::cout << "--- function debug_holds -----------------------------\n";
            std::cout << "    a = " << this->template getInputValue<operand_a>() << std::endl;
            std::cout << "    b = " << this->template getInputValue<operand_b>() << std::endl;

            Type lhs = this->template getInputValue<operand_a>();
            Accumulator<Type>()(lhs, this->template getInputValue<operand_b>());
            std::cout << "a o b = " << lhs << std::endl;

            Type rhs = this->template getInputValue<operand_b>();
            Accumulator<Type>()(rhs, this->template getInputValue<operand_a>());
            std::cout << "b o a = " << rhs << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);
            std::cout << "------------------------------------------------------\n";

            return lhs == rhs;
        }

    };

}} // namespace icl boost

#endif // BOOST_ICL_MONOID_HPP_JOFA_070411

