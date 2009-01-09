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
#include <time.h>
#include <boost/validate/typevalidater.h>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    namespace RootType 
    {
        enum RootTypes 
        { 
            itl_set, interval_set, separate_interval_set, split_interval_set, 
            itl_map, interval_map, split_interval_map, 
            Types_size 
        };
    }

    namespace DomainType 
    {
        enum DomainTypes { Int, Double, DomainTypes_size };
    }

    namespace CodomainType 
    {
        enum CodomainTypes { Int, Double, set_int, raw_bitset, CodomainTypes_size };
    }

    namespace NeutronHandlerType 
    {
        enum NeutronHandlerTypes { absorber, enricher, emitter, polluter, NeutronHandlerTypes_size };
    }

    
    class itl_validater
    {
    public:
        bool hasValidProfile()const { return _isValid; }

        virtual void setProfile() = 0;
        virtual AlgebraValidater* chooseValidater() = 0;

        void validate()
        {
            srand(static_cast<unsigned>(time(NULL))); //Different numbers each run
            //srand(static_cast<unsigned>(1)); //Same numbers each run (std)
            //srand(static_cast<unsigned>(4711)); //Same numbers each run (varying)

            for(int idx=0; hasValidProfile(); idx++)
            {
                if(idx>0 && idx % 100 == 0)
                    reportFrequencies();
                validateType();
            }
        }

        void validateType()
        {
            _validater = chooseValidater();
            if(_validater)
            {
                _validater->validate();
                _validater->addFrequencies(_frequencies);
                _validater->addViolations(_violationsCount, _violations);
                delete _validater;
            }
        }

        void reportFrequencies()
        {
            std::cout << "------------------------------------------------------------------------------" << std::endl;
            int valid_count = 1;
            FORALL(ValidationCounterT, it, _frequencies)
            {
                printf("%3d %-66s%8d\n", valid_count, it->KEY_VALUE.c_str(), it->CONT_VALUE);
                valid_count++;
            }
            std::cout << "------------------------------------------------------------------------------" << std::endl;
            int violation_count = 1;
            FORALL(ViolationMapT, it, _violations)
            {
                printf("%3d %-66s%8d\n", violation_count, it->KEY_VALUE.c_str(), it->CONT_VALUE.getViolationsCount());
                violation_count++;
            }
            if(!_violations.empty())
                std::cout << "------------------------------------------------------------------------------" << std::endl;
            FORALL(ViolationMapT, it, _violations)
            {
                PolyLawViolations violas = it->CONT_VALUE;
                violas.reportFirst();
            }
            if(!_violations.empty())
                std::cout << "------------------------------------------------------------------------------" << std::endl;
        }

		void reportFrequencies(const std::string& filename)
        {
			FILE* fp = fopen(filename.c_str(), "w");
            int valid_count = 1;
            FORALL(ValidationCounterT, it, _frequencies)
            {
                fprintf(fp, "%3d %-66s\n", valid_count, it->KEY_VALUE.c_str());
                valid_count++;
            }
        }

        void reportTypeChoiceError(const std::string& location, int rootChoice, const ChoiceT& chooser)const
        {
            std::cout << location
                << "Type choice: " << rootChoice << " is out of range or unselectable in switch clause.\n"
                << "Expected types and their weights are:\n"
                << chooser.asString();
        }

	protected:
        void setValid(bool truth) { _isValid = truth; }

        void setRootTypeNames()
        {
            std::vector<std::string> type_names(RootType::Types_size);
            type_names[RootType::itl_set]               = "itl_set"; 
            type_names[RootType::interval_set]          = "interval_set"; 
            type_names[RootType::separate_interval_set] = "separate_interval_set"; 
            type_names[RootType::split_interval_set]    = "split_interval_set"; 
            type_names[RootType::itl_map]               = "itl_map"; 
            type_names[RootType::interval_map]          = "interval_map"; 
            type_names[RootType::split_interval_map]    = "split_interval_map"; 
            _rootChoice.setTypeNames(type_names);
        }
        void setDomainTypeNames()
        {
            std::vector<std::string> type_names(CodomainType::CodomainTypes_size);
            type_names[CodomainType::Int]               = "Int"; 
            type_names[CodomainType::Double]            = "Double"; 
            _domainChoice.setTypeNames(type_names);
        }
        void setCodomainTypeNames()
        {
            std::vector<std::string> type_names(CodomainType::CodomainTypes_size);
            type_names[CodomainType::Int]               = "Int"; 
            type_names[CodomainType::Double]            = "Double"; 
            type_names[CodomainType::set_int]           = "set_int"; 
            type_names[CodomainType::raw_bitset]        = "raw_bitset"; 
            _codomainChoice.setTypeNames(type_names);
        }
        void setNeutronHandlerTypeNames()
        {
            std::vector<std::string> type_names(NeutronHandlerType::NeutronHandlerTypes_size);
            type_names[NeutronHandlerType::absorber]          = "absorber"; 
            type_names[NeutronHandlerType::enricher]          = "enricher"; 
            type_names[NeutronHandlerType::emitter]           = "emitter"; 
            type_names[NeutronHandlerType::polluter]          = "polluter"; 
            _neutronizerChoice.setTypeNames(type_names);
        }

        AlgebraValidater* choiceError(const std::string& location, int value, const ChoiceT& choice)
        {
            reportTypeChoiceError(location, value, choice); 
            setValid(false);
            return NULL; 
        }

    protected:
        ChoiceT            _rootChoice;
        ChoiceT            _domainChoice;
        ChoiceT            _codomainChoice;
        ChoiceT            _neutronizerChoice;

    private:
        AlgebraValidater*  _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
        bool               _isValid;
    };


}} // namespace itl boost

