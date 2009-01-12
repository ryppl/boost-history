/*----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_set_laws_h_JOFA_071124__
#define __itl_set_laws_h_JOFA_071124__

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/law.h>
#include <boost/itl/functors.hpp>

namespace boost{namespace itl
{

    template <typename Type, 
              template<class>class Combiner = inplace_plus,
		      template<class>class Equality = itl::std_equal>
    class InplaceSelfRemovability 
        : public Law<InplaceSelfRemovability<Type,Combiner,Equality>, 
                     LOKI_TYPELIST_1(Type), LOKI_TYPELIST_2(Type,Type)>
    {
        //a - a == 0
        //computed using inplace operators +=
        //Input  = (a := inVal1, b := inVal2)
        //Output = (lhs_result, rhs_result)

    public:
		typedef typename inverse<Combiner<Type> >::type InverseCombinerT;

        std::string name()const { return "InplaceSelfRemovability"; }
        std::string formula()const { return "a -= a; a == 0"; }

        std::string typeString()const
        {
            return "SelfRemovability<"+type_to_string<Type>::apply()+","
                                      +unary_template_to_string<Combiner>::apply()+","
                                      +unary_template_to_string<Equality>::apply()
									  +">";
        }

    public:

        bool holds()
        {
            Type lhs = this->template getInputValue<operand_a>();
            InverseCombinerT()(lhs, this->template getInputValue<operand_a>());

            Type rhs = Combiner<Type>::neutron();

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
		{ 
			return holds();
		}

        size_t size()const { return value_size<Type>::apply(this->template getInputValue<operand_a>()); }
    };


    template <typename Type, 
              template<class>class Combiner = inplace_plus,
		      template<class>class Equality = itl::std_equal>
    class InplaceInverseRemovability 
        : public Law<InplaceInverseRemovability<Type,Combiner,Equality>, 
                     LOKI_TYPELIST_1(Type), LOKI_TYPELIST_2(Type,Type)>
    {
        //(0 - a) + a == 0
        //computed using inplace operators +=
        //Input  = (a := inVal1, b := inVal2)
        //Output = (lhs_result, rhs_result)

    public:
		typedef typename inverse<Combiner<Type> >::type InverseCombinerT;

        std::string name()const { return "InplaceInverseRemovability"; }
        std::string formula()const { return "(0-a) + a == 0"; }

        std::string typeString()const
        {
            return "InverseRemovability<"+type_to_string<Type>::apply()+","
                                      +unary_template_to_string<Combiner>::apply()+","
                                      +unary_template_to_string<Equality>::apply()
									  +">";
        }

    public:

        bool holds()
        {
			Type lhs = Combiner<Type>::neutron(); 
			Type value_a = this->template getInputValue<operand_a>();
			// lhs = (0 - a)
            InverseCombinerT()(lhs, value_a);
			// lhs = (0 - a) + a
			Combiner<Type>()(lhs, value_a);

            Type rhs = Combiner<Type>::neutron();

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
		{ 
			return holds();
		}

        size_t size()const { return value_size<Type>::apply(this->template getInputValue<operand_a>()); }
    };


    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Operator1 = inplace_plus, 
                             template<class>class Operator2 = inplace_star,
                             template<class>class Equality  = itl::std_equal>
    class InplaceDistributivity 
        : public Law<InplaceDistributivity<Type,Operator1,Operator2,Equality>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a + (b * c) == (a + b) * (a + c)
            a(1)(b(2)c) == (a(1)b)(2)(a(1)c)
        computed using inplace operators +=, += and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplaceDistributivity"; }
        std::string formula()const { return "a + (b * c) == (a + b) * (a + c) 'inplace'"; }

        std::string typeString()const
        {
            return "Distributivity<"+type_to_string<Type>::apply()+","
                                    +unary_template_to_string<Operator1>::apply()+","
                                    +unary_template_to_string<Operator2>::apply()+","
                                    +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            // a + (b * c) == (a + b) * (a + c)
            // --- left hand side ------------------------
            Type b_star_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_star_c, this->template getInputValue<operand_c>());

            // lhs := a + (b * c)
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, b_star_c);

            // --- right hand side -----------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_b, this->template getInputValue<operand_b>());

            Type a_plus_c = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_c, this->template getInputValue<operand_c>());

            // rhs := (a + b) * (a + c)
            Type rhs = a_plus_b;
            Operator2<Type>()(rhs, a_plus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        {
            // a + (b * c) == (a + b) * (a + c)
            std::cout << "a:" << this->template getInputValue<operand_a>().as_string() << std::endl;
            std::cout << "b:" << this->template getInputValue<operand_b>().as_string() << std::endl;
            std::cout << "c:" << this->template getInputValue<operand_c>().as_string() << std::endl;
            // --- left hand side ------------------------
            Type b_star_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_star_c, this->template getInputValue<operand_c>());
            std::cout << "b*c:" << b_star_c.as_string() << std::endl;

            // lhs := a + (b * c)
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, b_star_c);
            std::cout << "l=a+(b*c):" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_b, this->template getInputValue<operand_b>());
            std::cout << "a+b:" << a_plus_b.as_string() << std::endl;

            Type a_plus_c = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_c, this->template getInputValue<operand_c>());
            std::cout << "a+c:" << a_plus_c.as_string() << std::endl;

            // rhs := (a + b) * (a + c)
            Type rhs = a_plus_b;
            Operator2<Type>()(rhs, a_plus_c);
            std::cout << "r=(a+b)*(a+c):" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Operator1 = inplace_plus, 
                             template<class>class Operator2 = inplace_star, 
                             template<class>class Equality = itl::std_equal>
    class InplaceDeMorgan 
        : public Law<InplaceDeMorgan<Type,Operator1,Operator2,Equality>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a - (b + c) == (a - b) & (a - c)
		    a - (b(1)c) == (a - b)(2)(a - c)
        computed using inplace operators +=, += and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplacePlusDeMorgan"; }
        std::string formula()const { return "a - (b + c) == (a - b) & (a - c) 'inplace'"; }

        std::string typeString()const
        {
            return "DeMorgan<"+type_to_string<Type>::apply()+","
                              +unary_template_to_string<Operator1>::apply()+","
                              +unary_template_to_string<Operator2>::apply()+","
                              +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            // a - (b + c) == (a - b) & (a - c)
            // --- left hand side ------------------------
            Type b_plus_c = this->template getInputValue<operand_b>();
            Operator1<Type>()(b_plus_c, this->template getInputValue<operand_c>());

            // lhs := a - (b + c)
            Type lhs = this->template getInputValue<operand_a>();
            lhs -= b_plus_c;

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            a_minus_b -= this->template getInputValue<operand_b>();

            Type a_minus_c = this->template getInputValue<operand_a>();
            a_minus_c -= this->template getInputValue<operand_c>();

            // rhs := (a - b) & (a - c)
            Type rhs = a_minus_b;
            Operator2<Type>()(rhs, a_minus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, 
              template<class>class Operator1 = inplace_plus, 
              template<class>class Operator2 = inplace_minus,
              template<class>class Equality = itl::std_equal>
    class InplaceRightDistributivity 
        : public Law<InplaceRightDistributivity<Type,Operator1,Operator2>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** (a + b) - c == (a - c) + (b - c)
        computed using inplace operators +=, -= and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplaceRightDistributivity"; }
        std::string formula()const { return "(a + b) - c == (a - c) + (b - c) 'inplace'"; }

        std::string typeString()const
        {
            return "RightDistributivity<"+type_to_string<Type>::apply()+","
                                         +unary_template_to_string<Operator1>::apply()+","
                                         +unary_template_to_string<Operator2>::apply()+","
			                             +unary_template_to_string<Equality>::apply() +">";
        }

    public:

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }

        bool holds()
        {
            // (a + b) - c == (a - c) + (b - c)
            // --- left hand side ------------------------
            // lhs := (a + b) - c
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, this->template getInputValue<operand_b>());
            Operator2<Type>()(lhs, this->template getInputValue<operand_c>());

            // --- right hand side -----------------------
            Type a_minus_c = this->template getInputValue<operand_a>();
            Operator2<Type>()(a_minus_c, this->template getInputValue<operand_c>());

            Type b_minus_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_minus_c, this->template getInputValue<operand_c>());

            // rhs := (a - c) + (b - c)
            Type rhs = a_minus_c;
            Operator1<Type>()(rhs, b_minus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        {
            // (a + b) - c == (a - c) + (b - c)
            std::cout << "a:" << this->template getInputValue<operand_a>().as_string() << std::endl;
            std::cout << "b:" << this->template getInputValue<operand_b>().as_string() << std::endl;
            std::cout << "c:" << this->template getInputValue<operand_c>().as_string() << std::endl;
            // --- left hand side ------------------------
            // lhs := (a + b) - c
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, this->template getInputValue<operand_b>());
            std::cout << "a+b:" << lhs.as_string() << std::endl;
            Operator2<Type>()(lhs, this->template getInputValue<operand_c>());
            std::cout << "l=(a+b)-c:" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_minus_c = this->template getInputValue<operand_a>();
            Operator2<Type>()(a_minus_c, this->template getInputValue<operand_c>());
            std::cout << "a-c:" << a_minus_c.as_string() << std::endl;

            Type b_minus_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_minus_c, this->template getInputValue<operand_c>());
            std::cout << "b-c:" << b_minus_c.as_string() << std::endl;

            // rhs := (a - c) + (b - c)
            Type rhs = a_minus_c;
            Operator1<Type>()(rhs, b_minus_c);
            std::cout << "r=(a-c)+(b-c):" << b_minus_c.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Equality = itl::std_equal>
    class InplaceSymmetricDifference 
        : public Law<InplaceSymmetricDifference<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** (a + b) - (a & b) == (a - b) + (b - a)
        computed using inplace operators +=, += and &=
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "Inplace Symmetric Difference"; }
        std::string formula()const { return "(a+b) - (a&b) == (a-b) + (b-a) 'inplace'"; }

        std::string typeString()const
        {
            return "SymmetricDifference<"+type_to_string<Type>::apply()+","
                                         +unary_template_to_string<Equality>::apply()+">";
        }

    public:

		bool holds() 
        {
            // --- left hand side ------------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            a_plus_b += this->template getInputValue<operand_b>();

            Type a_sec_b = this->template getInputValue<operand_a>();
            a_sec_b OP_INPLACE_INTERSECT this->template getInputValue<operand_b>();

            Type lhs = a_plus_b;
            lhs -= a_sec_b;

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            a_minus_b -= this->template getInputValue<operand_b>();

            Type b_minus_a = this->template getInputValue<operand_b>();
            b_minus_a -= this->template getInputValue<operand_a>();

            Type rhs = a_minus_b;
            rhs += b_minus_a;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }


        bool debug_holds()
        {
            // --- left hand side ------------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            a_plus_b += this->template getInputValue<operand_b>();

            std::cout << "a_plus_b=" << a_plus_b.as_string() << std::endl;

            Type a_sec_b = this->template getInputValue<operand_a>();
            a_sec_b OP_INPLACE_INTERSECT this->template getInputValue<operand_b>();

            std::cout << "a_sec_b=" << a_sec_b.as_string() << std::endl;

            Type lhs = a_plus_b;
            lhs -= a_sec_b;

            std::cout << "lhs=" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            a_minus_b -= this->template getInputValue<operand_b>();
            std::cout << "a_minus_b=" << a_minus_b.as_string() << std::endl;

            Type b_minus_a = this->template getInputValue<operand_b>();
            b_minus_a -= this->template getInputValue<operand_a>();
            std::cout << "b_minus_a=" << b_minus_a.as_string() << std::endl;

            Type rhs = a_minus_b;
            rhs += b_minus_a;
            std::cout << "rhs=" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename MapT, template<class>class Equality = itl::std_equal>
    class SectionAbsorbtion 
        : public Law<SectionAbsorbtion<MapT>, 
                     LOKI_TYPELIST_2(MapT, typename MapT::set_type), LOKI_TYPELIST_2(MapT,MapT)>
    {
        /** a - (a & b) == a - b
        computed using inplace operators -= and &=
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "SectionAbsorbtion"; }
        std::string formula()const { return "map a, set b: a - (a & b) == a - b 'inplace'"; }

        std::string typeString()const
        {
            return "SectionAbsorbtion<"+type_to_string<MapT>::apply()+","
                                       +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            // a - (a & b) == a - b
            // --- left hand side ------------------------
            // lhs := a - (a & b)
            MapT a_sec_b = this->template getInputValue<operand_a>();
            a_sec_b OP_INPLACE_INTERSECT  this->template getInputValue<operand_b>();
            MapT lhs =  this->template getInputValue<operand_a>();
            lhs -= a_sec_b;

            // --- right hand side -----------------------
            // rhs := a - b
            MapT rhs = this->template getInputValue<operand_a>();
            rhs -= this->template getInputValue<operand_b>();

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<MapT>()(lhs, rhs);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return 
                value_size<MapT>::apply(this->template getInputValue<operand_a>())+
                value_size<typename MapT::set_type>::apply(this->template getInputValue<operand_b>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename MapT>
    class ProtonicEquality 
        : public Law<ProtonicEquality<MapT>, 
                     LOKI_TYPELIST_2(MapT,MapT), LOKI_TYPELIST_2(bool,bool)>
    {
        /** (a.absorb_neutrons() == b.absorb_neutrons()) == is_protonic_equal(a, b)
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "ProtonicEquality"; }
        std::string formula()const { return "(a.absorb_neutrons() == b.absorb_neutrons()) == is_protonic_equal(a, b)"; }

        std::string typeString()const
        {
            return "ProtonicEquality<"+type_to_string<MapT>::apply()+">";
        }

    public:

        bool holds()
        {
            // (a.absorb_neutrons() == b.absorb_neutrons()) == is_protonic_equal(a, b)
            // --- left hand side ------------------------
            // lhs := (a.absorb_neutrons() == b.absorb_neutrons())
            MapT a = this->template getInputValue<operand_a>();
            MapT a_protonic = a;
			a_protonic.absorb_neutrons();
            MapT b = this->template getInputValue<operand_b>();
            MapT b_protonic = b;
			b_protonic.absorb_neutrons();

            bool lhs = a_protonic == b_protonic;

            // --- right hand side -----------------------
            // rhs := is_protonic_equal(a, b)
            bool rhs = is_protonic_equal(a, b);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
		{ 
            // (a.absorb_neutrons() == b.absorb_neutrons()) == is_protonic_equal(a, b)
            // --- left hand side ------------------------
            // lhs := (a.absorb_neutrons() == b.absorb_neutrons())

			cout << name() << "::debug_holds():" << endl;
			cout << "Instance: " << typeString() << endl;
			cout << "Formula: " << formula() << endl;

            MapT a = this->template getInputValue<operand_a>();
            MapT b = this->template getInputValue<operand_b>();

			cout << "a: " << a.as_string() << endl;
			cout << "b: " << b.as_string() << endl;

            MapT a_protonic = a;
			a_protonic.absorb_neutrons();
            MapT b_protonic = b;
			b_protonic.absorb_neutrons();

			cout << "a.absorb_neutrons(): " << a_protonic.as_string() << endl;
			cout << "b.absorb_neutrons(): " << b_protonic.as_string() << endl;

            bool lhs = a_protonic == b_protonic;

			cout << "lhs := (a.absorb_neutrons() == b.absorb_neutrons()): " << lhs << endl;

            // --- right hand side -----------------------
            // rhs := is_protonic_equal(a, b)
            bool rhs = is_protonic_equal(a, b);

			cout << "rhs := is_protonic_equal(a, b): " << rhs << endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
		}

        size_t size()const 
        { 
            return 
                value_size<MapT>::apply(this->template getInputValue<operand_a>())+
                value_size<MapT>::apply(this->template getInputValue<operand_b>());
        }
    };

}} // namespace itl boost

#endif // __itl_set_laws_h_JOFA_071124__


