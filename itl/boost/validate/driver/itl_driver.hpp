/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/config/warning_disable.hpp>
#include <boost/validate/validater/algebra_validater.hpp>
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
        enum CodomainTypes { Nat, Int, Double, set_int, raw_bitset, CodomainTypes_size };
    }

    namespace NeutronHandlerType 
    {
        enum NeutronHandlerTypes { partial_absorber, partial_enricher, total_absorber, total_enricher, NeutronHandlerTypes_size };
    }

    
    class itl_driver
    {
    public:
        itl_driver()
        {
            _laws_per_cycle = GentorProfileSgl::it()->laws_per_cycle();
        }

        bool hasValidProfile()const { return _isValid; }

        virtual void setProfile() = 0;
        virtual algebra_validater* chooseValidater() = 0;

        void validate()
        {
            //srand(static_cast<unsigned>(time(NULL))); //Different numbers each run
            srand(static_cast<unsigned>(1)); //Same numbers each run (std)
            //srand(static_cast<unsigned>(4711)); //Same numbers each run (varying)

            for(int idx=0; hasValidProfile(); idx++)
            {
                if(idx>0 && idx % _laws_per_cycle == 0)
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
            double avg_evaluation_time = 0.0;
            long   instance_count      = 0;
            FORALL(ValidationCounterT, it, _frequencies)
            {
                long law_validation_count = it->CONT_VALUE.count();
                double avg_law_evaluation_time = 
                    it->CONT_VALUE.time()/(law_validation_count);
                printf("%3d %-58s%9d%7.0lf\n", 
                    valid_count, it->KEY_VALUE.c_str(), law_validation_count, avg_law_evaluation_time);

                avg_evaluation_time += avg_law_evaluation_time;
                instance_count      += law_validation_count;
                valid_count++;
            }

            std::cout << "------------------------------------------------------------------------------" << std::endl;
            // Summary for the current cycle
            double avg_evaluation_time_per_law = avg_evaluation_time/_frequencies.size();
            printf( "    %10.3lf%-50s%7d%7.0lf\n", 
                    avg_evaluation_time_per_law, " ", instance_count, avg_evaluation_time_per_law);

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
        void setValid(bool truth) 
        { 
            _isValid = truth;
        }

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
            type_names[NeutronHandlerType::partial_absorber]        = "partial_absorber"; 
            type_names[NeutronHandlerType::partial_enricher]        = "partial_enricher"; 
            type_names[NeutronHandlerType::total_absorber]          = "total_absorber"; 
            type_names[NeutronHandlerType::total_enricher]          = "total_enricher"; 
            _neutronizerChoice.setTypeNames(type_names);
        }

        algebra_validater* choiceError(const std::string& location, int value, const ChoiceT& choice)
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
        algebra_validater* _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
        bool               _isValid;

        int _laws_per_cycle; // After _laws_per_cycle times a cycle is
                             // done and times and frequencies of law 
                             // validations are reported for all instances.
    };


}} // namespace itl boost

