/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_VALIDATE_CONCEPT_VALIDATER_HPP_JOFA_080203
#define BOOST_ITL_VALIDATE_CONCEPT_VALIDATER_HPP_JOFA_080203

#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/functors.hpp>
#include <boost/validate/validater/law_validater.hpp>

namespace boost{namespace icl
{
    typedef WeightedNumberGentor<int> ChoiceT;

    class concept_validater
    {
    public:
        virtual ~concept_validater(){}
        // the choice probability, and also the availability of laws is individual to each type
        // Somehow it has to be defined in conjunction to every type. So it is an aspect of
        // type traits.
        virtual LawValidaterI* chooseValidater()=0;
        virtual bool hasValidProfile()const{ return true; }

        void validate()
        {
            LawValidaterI* law_validater = chooseValidater();
            if(law_validater)
            {
                law_validater->run();
                law_validater->addFrequencies(_frequencies);
                law_validater->addViolations(_violationsCount, _violations);
                delete law_validater;
            }
        }

        void addFrequencies(ValidationCounterT& summary) { summary += _frequencies; }

        void addViolations(ViolationCounterT& summary, ViolationMapT& collector)
        { 
            summary   += _violationsCount; 
            collector += _violations;  
        }

        static int share(int total, int& index, int& rest_shares);

    private:
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
    };

    inline int concept_validater::share(int total, int& index, int& rest_shares)
    {
        int count = total - index;
        int share = rest_shares / count;
        rest_shares -= share;
        ++index;
        return share;
    }

}} // namespace icl boost

#endif // BOOST_ITL_VALIDATE_CONCEPT_VALIDATER_HPP_JOFA_080203
