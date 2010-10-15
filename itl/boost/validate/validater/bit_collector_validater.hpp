/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_VALIDATER_BIT_COLLECTOR_VALIDATER_HPP_JOFA_091009
#define BOOST_VALIDATE_VALIDATER_BIT_COLLECTOR_VALIDATER_HPP_JOFA_091009

#include <boost/itl/functors.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/laws/symmetric_difference.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

namespace boost{namespace icl
{

template <typename Type>
class collector_validater : public concept_validater
{
public:

    enum Laws 
    { 
        inplacePlusAssociativity,
        inplacePlusNeutrality,
        inplacePlusCommutativity,
        inplaceEtAssociativity,
        inplaceEtCommutativity,
        inplaceNaturalInversion,
        inplaceSymmetricDifference,
        inplaceFlip,
        inplacePlusDistributivity,
        inplaceEtDistributivity,
        inplacePlusDashRightDistrib,
        inplaceEtDashRightDistrib,
        inplacePlusDeMorgan,
        inplaceEtDeMorgan,
        Laws_size 
    };

    collector_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;
        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[inplacePlusAssociativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusNeutrality]      = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusCommutativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtAssociativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtCommutativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceNaturalInversion]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceSymmetricDifference] = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceFlip]                = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDistributivity]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDistributivity]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDashRightDistrib]= share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDashRightDistrib]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDeMorgan]        = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDeMorgan]          = share(Laws_size, item_index, rest_shares);
        _lawChoice.init();
    }


    //JODO DEL complete: spezielle instanzen werden nicht benoetigt! 
    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case inplacePlusAssociativity:  return new LawValidater<InplaceAssociativity<Type, inplace_bit_add> >;
        case inplacePlusNeutrality:     return new LawValidater<InplaceNeutrality   <Type, inplace_bit_add> >;
        case inplacePlusCommutativity:  return new LawValidater<InplaceCommutativity<Type, inplace_bit_add> >;
        case inplaceEtAssociativity:    return new LawValidater<InplaceAssociativity<Type, inplace_bit_and> >;
        case inplaceEtCommutativity:    return new LawValidater<InplaceCommutativity<Type, inplace_bit_and> >;
        case inplaceSymmetricDifference:return new LawValidater<InplaceSymmetricDifference<Type, inplace_bit_add, inplace_bit_subtract, inplace_bit_and> >;
        case inplaceFlip:               return new LawValidater<InplaceFlip<Type> >;
        case inplaceEtDistributivity:  
            if(icl::is_interval_splitter<Type>::value && absorbs_identities<Type>::value && !is_total<Type>::value)
                                        return new LawValidater<InplaceDistributivity<Type, inplace_bit_and, inplace_bit_add, element_equal> >;
            else                        return new LawValidater<InplaceDistributivity<Type, inplace_bit_and, inplace_bit_add, std_equal> >;
        case inplacePlusDashRightDistrib:
            if(icl::is_interval_splitter<Type>::value && absorbs_identities<Type>::value && !is_total<Type>::value)
                                        return new LawValidater<InplaceRightDistributivity<Type, inplace_bit_add, inplace_bit_subtract, element_equal> >;
            else                        return new LawValidater<InplaceRightDistributivity<Type, inplace_bit_add, inplace_bit_subtract, std_equal> >;
        case inplaceEtDashRightDistrib: return new LawValidater<InplaceRightDistributivity<Type, inplace_bit_and, inplace_bit_subtract> >;
        case inplacePlusDeMorgan:       return new LawValidater<InplaceDeMorgan<Type, inplace_bit_add, inplace_bit_and, inplace_bit_subtract, icl::std_equal> >;
        case inplaceEtDeMorgan:        
            if(icl::is_interval_splitter<Type>::value || icl::is_interval_separator<Type>::value)
                                        return new LawValidater<InplaceDeMorgan<Type, inplace_bit_and, inplace_bit_add, inplace_bit_subtract, icl::element_equal> >;
            else                        return new LawValidater<InplaceDeMorgan<Type, inplace_bit_and, inplace_bit_add, inplace_bit_subtract, icl::std_equal> >;

        default: return NULL;
        }
    }

private:
    ChoiceT        _lawChoice;
};


}} // namespace icl boost

#endif // BOOST_VALIDATE_VALIDATER_BIT_COLLECTOR_VALIDATER_HPP_JOFA_091009
