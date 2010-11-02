/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_VALIDATE_VALIDATER_ICL_INDUCED_RELATIONS_HPP_JOFA_090304
#define BOOST_ICL_VALIDATE_VALIDATER_ICL_INDUCED_RELATIONS_HPP_JOFA_090304

#include <boost/icl/functors.hpp>
#include <boost/icl/predicates/element_equal.hpp>
#include <boost/icl/predicates/sub_super_set.hpp>
#include <boost/icl/detail/interval_morphism.hpp>
#include <boost/validate/laws/induced_relation.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

namespace boost{namespace icl
{

    template <typename Type>
    class icl_induced_relations_validater : public concept_validater
    {
    public:
        enum Laws 
        { 
            atomize_subset,
            atomize_superset,
            atomize_element_equal,
            Laws_size 
        };

        icl_induced_relations_validater() {setProfile();}

        void setProfile()
        {
            const int volume = 100;
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(volume);
            _lawChoice[atomize_subset]          = 33;
            _lawChoice[atomize_superset]        = 33;
            _lawChoice[atomize_element_equal]   = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case atomize_subset:        return new LawValidater<InducedRelation<Type, typename Type::atomized_type, segmental::atomizer, sub_super_set>, RandomGentor>();
            case atomize_superset:      return new LawValidater<InducedRelation<Type, typename Type::atomized_type, segmental::atomizer, super_sub_set>, RandomGentor>();
            case atomize_element_equal: return new LawValidater<InducedRelation<Type, typename Type::atomized_type, segmental::atomizer, element_equal>, RandomGentor>();
            default: return NULL;
            }
        }

    private:
        ChoiceT        _lawChoice;
    }; //class icl_induced_relations_validater


}} // namespace icl boost

#endif BOOST_ICL_VALIDATE_VALIDATER_ICL_INDUCED_RELATIONS_HPP_JOFA_090304

