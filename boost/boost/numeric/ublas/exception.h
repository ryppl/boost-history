//  
//  Copyright (c) 2000-2001
//  Joerg Walter, Mathias Koch
//  
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.  
//  It is provided "as is" without express or implied warranty.
//  
//  The authors gratefully acknowledge the support of 
//	GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_EXCEPTION_H
#define NUMERICS_EXCEPTION_H

#include <exception>

#include "config.h"

namespace numerics {

    struct divide_by_zero:
        public std::exception {
        virtual const char *what () const {
            return "divide by zero";
        }
    };
    struct internal_logic:
        public std::exception {
        virtual const char *what () const {
            return "internal logic";
        }
    };
    struct external_logic:
        public std::exception {
        virtual const char *what () const {
            return "external logic";
        }
    };
    struct bad_size:
        public std::exception {
        virtual const char *what () const {
            return "bad size";
        }
    };
    struct bad_index:
        public std::exception {
        virtual const char *what () const {
            return "bad index";
        }
    };
    struct singular:
        public std::exception {
        virtual const char *what () const {
            return "singular";
        }
    };
    struct non_triangular:
        public std::exception {
        virtual const char *what () const {
            return "non triangular";
        }
    };
    struct non_band:
        public std::exception {
        virtual const char *what () const {
            return "non band";
        }
    };

    template<class E>
    struct check {
        static 
        NUMERICS_INLINE
        void precondition (bool expression) {
            if (! expression)
                throw E ();
        }
        static 
        NUMERICS_INLINE
        void postcondition (bool expression) {
            if (! expression)
                throw E ();
        }
        static 
        NUMERICS_INLINE
        void invariant (bool expression) {
            if (! expression)
                throw E ();
        }
        static 
        NUMERICS_INLINE
        void nop () {}
    };

#ifndef NUMERICS_BOUNDS_CHECK
#define precondition(expression) nop()
#define postcondition(expression) nop()
#define invariant(expression) nop()
#endif
    
}

#endif 


