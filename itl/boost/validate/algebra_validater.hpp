/*----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#pragma once

#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/functors.hpp>
#include <boost/validate/lawvalidater.h>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;

    class AlgebraValidater
    {
    public:
        virtual ~AlgebraValidater(){}
        // the choice probability, and also the availability of laws is individual to each type
        // Somehow it has to be defined in conjunction to every type. So it is an aspect of
        // type traits.
        virtual void setProfile()=0;
        virtual void validate()=0;
        virtual void addFrequencies(ValidationCounterT&)=0;
        virtual void addViolations(ViolationCounterT&, ViolationMapT&)=0;

        virtual bool hasValidProfile()const{ return true; }
    };

}} // namespace itl boost

