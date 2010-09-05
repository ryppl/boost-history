#ifndef BOOST_THREAD_CV_STATUS_HPP
#define BOOST_THREAD_CV_STATUS_HPP

//  (C) Copyright 2010 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ENUM_CLASS
#if defined(BOOST_NO_SCOPED_ENUMS)
#define BOOST_ENUM_CLASS(T) T::type
#else
#define BOOST_ENUM_CLASS(T) T
#endif
#endif

namespace boost
{
#if defined(BOOST_NO_SCOPED_ENUMS)
    namespace cv_status {
        enum type { no_timeout, timeout };
    }
#else
    enum class cv_status { no_timeout, timeout };
#endif

}    

#endif
