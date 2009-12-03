/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_BACK_DRIVER_HPP_JOFA_091202
#define BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_BACK_DRIVER_HPP_JOFA_091202

#include <iostream>
#include <stdio.h>
#include <boost/validate/validater/sorted_associative_back_validater.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
    class sorted_associative_bitset_back_driver : public itl_driver
    {
    public:
        sorted_associative_bitset_back_driver() { setProfile(); }

        void setProfile()
        {
            const int used_choices = 4;
            setValid(true);
            _freeChoice.setSize(used_choices);
            _freeChoice.setMaxWeights(100);
            _freeChoice[FreeChoice::_1] = 25;
            _freeChoice[FreeChoice::_2] = 25;
            _freeChoice[FreeChoice::_3] = 25;
            _freeChoice[FreeChoice::_4] = 25;
            setRootTypeNames();
            _freeChoice.init();

            if(!_freeChoice.is_consistent())
            {
                setValid(false);
                std::cout << _freeChoice.inconsitencyMessage("interval_bitset_driver::setProfile()") << std::endl;
            }
        }

        algebra_validater* chooseValidater()
        {
            int freeChoice         = _freeChoice.some();

            switch(freeChoice)
            {
            case FreeChoice::_1:
                return new sorted_associative_back_validater< interval_bitset<int, bits<unsigned char> >,  itl::list<int> >; 
            case FreeChoice::_2:
                return new sorted_associative_back_validater< interval_bitset<int, bits<unsigned short> >, itl::list<int> >; 
            case FreeChoice::_3:
                return new sorted_associative_back_validater< interval_bitset<int, bits<unsigned long> >,  itl::list<int> >; 
            case FreeChoice::_4:
                return new sorted_associative_back_validater< interval_bitset<int, bits<unsigned long long> >, itl::list<int> >; 
            default: return choiceError(ITL_LOCATION("freeChoice:\n"), freeChoice, _freeChoice);
            } //switch()

        }
    };

}} // namespace itl boost

#endif // BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_BACK_DRIVER_HPP_JOFA_091202
