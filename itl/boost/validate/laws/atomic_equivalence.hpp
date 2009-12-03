/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_LAWS_ATOMIC_EQUIVALENCE_HPP_JOFA_091124
#define BOOST_VALIDATE_LAWS_ATOMIC_EQUIVALENCE_HPP_JOFA_091124

#include <iterator>
#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/itl/interval_morphism.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/validate/std/algorithm/copy.hpp>
#include <boost/validate/std/algorithm/set_algo.hpp>
#include <boost/validate/std/algorithm/relation.hpp>

namespace boost{namespace itl
{

// -----------------------------------------------------------------------------
// S: Segment container 
// f: Atomize function transforming an interval container into an element container
// E: Element container
// g: A function (specifically std::algorithm) applicable to S and E
// T: Target container
//
//         S
//        / \
//     f /   \ g
//      /  =  \
//     V       V  
//    E ------> T
//         g
// -----------------------------------------------------------------------------
template <typename SegmentsT, 
          typename TargetsT  = typename SegmentsT::atomized_type,
          template<class,class,template<class>class>class Algorithm = itl::std_copy,
          template<class>class       TargetIterator = std::insert_iterator,
          template<class,class>class Atomizer       = itl::Interval::Atomize,
          template<class>class       Equality       = itl::std_equal         > 
class UnaryAtomicEquivalence : 
    public Law<UnaryAtomicEquivalence<SegmentsT, TargetsT, 
                                      Algorithm, TargetIterator, 
                                      Atomizer, Equality>, 
               LOKI_TYPELIST_1(SegmentsT), 
               LOKI_TYPELIST_2(TargetsT,TargetsT)   >
{
    /** S: SegmentsT, T: TargetsT, I: InputIterator
    Alg<S,T,I>: an algortihm on interval containers S
                that results in a container T via iterator I. 
    For all S a: Alg<E,T,I>(atomize(a)) == Alg<S,T,I>(a)
    Input  = (a := inVal1)
    Output = (lhs_result, rhs_result)
    */
public:
    typedef typename SegmentsT::atomized_type ElementsT;

    std::string name()const { return "UnaryAtomicEquivalence"; }
    std::string formula()const { return "For all S a: Alg<E,T,I>(atomize(a)) == Alg<S,T,I>(a)"; }

    std::string typeString()const
    {
        return
            "EleEq<"
                + type_to_string<SegmentsT>::apply()+","
                + type_to_string<TargetsT>::apply() +","
                + Algorithm<ElementsT,TargetsT,TargetIterator>::struct_abbreviation() +">";
    }

public:

    bool holds()
    {
        // For all S a: Alg<E,T,I>(atomize(a)) == Alg<S,T,I>(a)
        // --- left hand side --------------------------------------------------
        TargetsT lhs;
        SegmentsT segmental_a = this->template getInputValue<operand_a>();
        ElementsT elemental_a;
        Atomizer<ElementsT,SegmentsT>()(elemental_a, segmental_a);
        Algorithm<ElementsT,TargetsT,TargetIterator>::apply(elemental_a, lhs); 

        // --- right hand side -------------------------------------------------
        TargetsT rhs;
        Algorithm<SegmentsT,TargetsT,TargetIterator>::apply_elemental(segmental_a, rhs); 

        this->template setOutputValue<lhs_result>(lhs);
        this->template setOutputValue<rhs_result>(rhs);

        return Equality<TargetsT>()(lhs, rhs);
    }

    bool debug_holds()
    { 
        return holds();
    }

    size_t size()const 
    { 
        return value_size<SegmentsT>::apply(this->template getInputValue<operand_a>());
    }
};



// -----------------------------------------------------------------------------
// S: Segment container 
// f: Atomize function transforming an interval container into an element container
// E: Element container
// .o.: U x U -> T A binary function (specifically std::algorithm) applicable to S or E
// T: Target container
//
//         S
//        / \
//     f /   \ o
//      /  =  \
//     V       V  
//    E ------> T
//         o
// -----------------------------------------------------------------------------
template <typename SegmentsT, 
          typename TargetsT  = typename SegmentsT::atomized_type,
          template<class,class,template<class>class>class Algorithm = itl::std_set_union,
          template<class>class       TargetIterator = std::insert_iterator,
          template<class,class>class Atomizer       = itl::Interval::Atomize,
          template<class>class       Equality       = itl::std_equal         > 
class BinaryAtomicEquivalence : 
    public Law<BinaryAtomicEquivalence<SegmentsT, TargetsT, 
                                       Algorithm, TargetIterator, 
                                       Atomizer, Equality>, 
               LOKI_TYPELIST_2(SegmentsT,SegmentsT), 
               LOKI_TYPELIST_2(TargetsT,TargetsT)   >
{
    /** S: SegmentsT, T: TargetsT, I: InputIterator
    Alg<S,T,I>: an algortihm on interval containers S
                that results in a container T via iterator I. 
    For all S a,b: Alg<S,T,I>(atomize(a), atomize(b)) == Alg<S,T,I>(a,b)
    Input  = (a := inVal1)
    Output = (lhs_result, rhs_result)
    */
public:
    typedef typename SegmentsT::atomized_type ElementsT;

    std::string name()const { return "BinaryAtomicEquivalence"; }
    std::string formula()const 
    { return "For all S a: Alg<E,T,I>(f(a),f(b)) == Alg<S,T,I>(a,b)"; }

    std::string typeString()const
    {
        return
            "EleEq<"
                + type_to_string<SegmentsT>::apply()+","
                + type_to_string<TargetsT>::apply() +","
                + Algorithm<ElementsT,TargetsT,TargetIterator>::struct_abbreviation() +">";
    }

public:

    bool holds()
    {
        // For all S a: Alg<E,T,I>(atomize(a)) == Alg<S,T,I>(a)
        // --- left hand side --------------------------------------------------
        TargetsT lhs;
        SegmentsT segmental_a = this->template getInputValue<operand_a>();
        SegmentsT segmental_b = this->template getInputValue<operand_b>();
        ElementsT elemental_a, elemental_b;
        Atomizer<ElementsT,SegmentsT>()(elemental_a, segmental_a);
        Atomizer<ElementsT,SegmentsT>()(elemental_b, segmental_b);
        Algorithm<ElementsT,TargetsT,TargetIterator>::apply(elemental_a, elemental_b, lhs); 

        // --- right hand side -------------------------------------------------
        TargetsT rhs;
        Algorithm<SegmentsT,TargetsT,TargetIterator>::apply_elemental(segmental_a, segmental_b, rhs); 

        this->template setOutputValue<lhs_result>(lhs);
        this->template setOutputValue<rhs_result>(rhs);

        return Equality<TargetsT>()(lhs, rhs);
    }

    bool debug_holds()
    { 
        return holds();
    }

    size_t size()const 
    { 
        return value_size<SegmentsT>::apply(this->template getInputValue<operand_a>())
             + value_size<SegmentsT>::apply(this->template getInputValue<operand_b>());
    }
};

}} // namespace itl boost

#endif BOOST_VALIDATE_LAWS_ATOMIC_EQUIVALENCE_HPP_JOFA_091124

