/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_MAP_LAWS_HPP_JOFA_071124
#define BOOST_ICL_MAP_LAWS_HPP_JOFA_071124

#include <boost/icl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/icl/functors.hpp>

namespace boost{namespace icl
{

    // ---------------------------------------------------------------------------
    template <typename MapT, template<class>class Equality = icl::std_equal>
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
            a_sec_b &=  this->template getInputValue<operand_b>();
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
        /** (a.absorb_identities() == b.absorb_identities()) == is_distinct_equal(a, b)
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "ProtonicEquality"; }
        std::string formula()const { return "(a.absorb_identities() == b.absorb_identities()) == is_distinct_equal(a, b)"; }

        std::string typeString()const
        {
            return "ProtonicEquality<"+type_to_string<MapT>::apply()+">";
        }

    public:

        bool holds()
        {
            // (a.absorb_identities() == b.absorb_identities()) == is_distinct_equal(a, b)
            // --- left hand side ------------------------
            // lhs := (a.absorb_identities() == b.absorb_identities())
            MapT a = this->template getInputValue<operand_a>();
            MapT a_protonic = a;
            a_protonic.absorb_identities();
            MapT b = this->template getInputValue<operand_b>();
            MapT b_protonic = b;
            b_protonic.absorb_identities();

            bool lhs = a_protonic == b_protonic;

            // --- right hand side -----------------------
            // rhs := is_distinct_equal(a, b)
            bool rhs = is_distinct_equal(a, b);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
        { 
            // (a.absorb_identities() == b.absorb_identities()) == is_distinct_equal(a, b)
            // --- left hand side ------------------------
            // lhs := (a.absorb_identities() == b.absorb_identities())

            std::cout << name() << "::debug_holds():" << std::endl;
            std::cout << "Instance: " << typeString() << std::endl;
            std::cout << "Formula: " << formula() << std::endl;

            MapT a = this->template getInputValue<operand_a>();
            MapT b = this->template getInputValue<operand_b>();

            std::cout << "a: " << a.as_string() << std::endl;
            std::cout << "b: " << b.as_string() << std::endl;

            MapT a_protonic = a;
            a_protonic.absorb_identities();
            MapT b_protonic = b;
            b_protonic.absorb_identities();

            std::cout << "a.absorb_identities(): " << a_protonic.as_string() << std::endl;
            std::cout << "b.absorb_identities(): " << b_protonic.as_string() << std::endl;

            bool lhs = a_protonic == b_protonic;

            std::cout << "lhs := (a.absorb_identities() == b.absorb_identities()): " << lhs << std::endl;

            // --- right hand side -----------------------
            // rhs := is_distinct_equal(a, b)
            bool rhs = is_distinct_equal(a, b);

            std::cout << "rhs := is_distinct_equal(a, b): " << rhs << std::endl;

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

}} // namespace icl boost

#endif // BOOST_ICL_MAP_LAWS_HPP_JOFA_071124


