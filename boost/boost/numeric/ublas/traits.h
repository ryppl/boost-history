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

#ifndef NUMERICS_TRAITS_H
#define NUMERICS_TRAITS_H

#include <complex>

#include "config.h"

namespace numerics {

    template<class T>
    struct type_traits {
        typedef T norm_type;
        typedef T precision_type;
    };

    template<>
    struct type_traits<float> {
        typedef float norm_type;
	    typedef double precision_type;
    };
    template<>
    struct type_traits<double> {
        typedef double norm_type;
	    typedef double precision_type;
    };

    template<>
    struct type_traits<std::complex<float> > {
        typedef float norm_type;
	    typedef std::complex<double> precision_type;
    };
    template<>
    struct type_traits<std::complex<double> > {
        typedef double norm_type;
	    typedef std::complex<double> precision_type;
    };

    template<class T1, class T2>
    struct promote_traits {
        typedef T1 promote_type;
    };

    template<>
    struct promote_traits<float, double> {
	    typedef double promote_type;
    };
    template<>
    struct promote_traits<double, float> {
	    typedef double promote_type;
    };

    template<>
    struct promote_traits<std::complex<float>, std::complex<double> > {
	    typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, std::complex<float> > {
	    typedef std::complex<double> promote_type;
    };
}

#endif // NUMERICS_TRAITS_H