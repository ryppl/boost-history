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

#include <iostream>
#include <stdio.h>
#include <boost/validate/loki_xt/Tuple.h>
#include <boost/itl/set.hpp>
#include <boost/itl/map.hpp>
#include <boost/validate/gentor/randomgentor.h>

#include <boost/validate/laws/monoid.h>
#include <boost/validate/lawviolations.h>

namespace boost{namespace itl
{
    typedef itl::map<std::string, int> ValidationCounterT;
    typedef itl::map<std::string, int> ViolationCounterT;
    typedef itl::map<std::string, PolyLawViolations> ViolationMapT;

    class LawValidaterI
    {
    public:
        virtual ~LawValidaterI(){}
        virtual void init()=0;
        virtual void run()=0;
        virtual void addFrequencies(ValidationCounterT&)=0;
        virtual void addViolations(ViolationCounterT&, ViolationMapT&)=0;
    };


    template <class LawT, template<typename>class GentorT>
    class LawValidater : public LawValidaterI
    {
    public:
        typedef typename LawT::input_types    input_types;
        typedef typename LawT::output_types   output_types;
        typedef typename LawT::input_tuple    input_tuple;
        typedef typename LawT::output_tuple   output_tuple;
        
        typedef typename Loki::TL::MapType<GentorT, input_types>::Result gentor_types;
        typedef typename Loki::tuple<gentor_types> input_gentor;

    public:
        LawValidater(){ setTrialsCount(1000); } //1000(std)//JODO config at only ONE location

        void setTrialsCount(int trials) 
        {
            _trialsCount = trials;
            _silentTrialsCount = std::max(1, _trialsCount / 10);
        }

        void setSilentTrialsCount(int trials) { _trialsCount = trials; }

        void init();
        void run();
        void addFrequencies(ValidationCounterT& summary) { summary += _frequencies; }
        
        void addViolations(ViolationCounterT& summary, ViolationMapT& collector)   
        {
            summary += ViolationCounterT::value_type(lawType(), static_cast<int>(_lawViolations.size())); 
            if(!_lawViolations.empty())
                collector += ViolationMapT::value_type(lawType(), PolyLawViolations(new LawViolationsT(_lawViolations)));
        }

        std::string lawType()const{ return _law.typeString(); }

        void reportLawInfo()const;
        void reportProgress()const;
        void reportViolations()const;
        void reportSuccess()const;

    private:
        typedef LawViolations<LawT> LawViolationsT;

    private:
        input_gentor _gentor;
        LawT         _law;

        int _trialsCount;
        int _silentTrialsCount;

        LawViolationsT     _lawViolations;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
    }; //class LawValitater


    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::init()
    {
        _frequencies.clear();
        _lawViolations.clear();
        _violations.clear();
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::run()
    {
        // apply Calibrater.apply to each element of the generator-tuple _gentor
        // This is just an initialisation of the generators according to system
        // defaults for convenience purposes.
        _gentor.template apply<GentorT, Calibrater, input_tuple>();

        // Input values that are to be generated on every iteration
        input_tuple values;

        for(int idx=0; idx<_trialsCount; idx++)
        {
            // Apply the function SomeVale to each component of the input tuple
            _gentor.template map_template<GentorT, SomeValue>(values);
            _law.setInstance(values);

            if(!_law.holds())
                _lawViolations.insert(_law);
        }

        if(!_lawViolations.empty())
        {
            reportViolations();
            LawT violation = *(_lawViolations.begin());

            input_tuple  inVars;
            output_tuple outVars;
            violation.getInstance(inVars, outVars);
            std::cout << "Violation of: " << _law.typeString() << std::endl;
            std::cout << _law.formula()      << std::endl;
            std::cout <<  inVars.as_string() << std::endl;
            std::cout << outVars.as_string() << std::endl;

            violation.debug_holds();
        }
        else
        {
            //reportSuccess();
            _frequencies.insert(ValidationCounterT::value_type(lawType(), 1));
        }

    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportLawInfo()const
    {
        std::cout << "----------------------------------------------------------" << std::endl;
        std::cout << "Validation of law: " << std::endl;
        std::cout << _law.name() << std::endl;
        std::cout << _law.formula() << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportProgress()const
    {
        typename LawT::input_tuple input_values;
        _law.getInputInstance(input_values);
        //JODO think about more general mechanics of progress reporting here
        std::cout << "SomeValues " << input_values.as_string() << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportViolations()const
    {
        std::cout << static_cast<int>(_lawViolations.getViolationsCount()) << " violations" << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportSuccess()const
    {
        std::cout << "Law successfully validated for " << _trialsCount << " cases" << std::endl;
    }

}} // namespace itl boost

// ----------------------------------------------------------------------------

