//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_DC4208C6417811DEBF11E1EC55D89593
#define UUID_DC4208C6417811DEBF11E1EC55D89593

#if defined(__GNUC__) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif

namespace
boost
    {
        namespace
        exception_detail
                {
                class
                exception_ptr_base
                        {
                        public:

                        virtual void _rethrow() const=0;
                        virtual bool _empty() const=0;

                        protected:

                        ~exception_ptr_base() throw()
                            {
                            }
                        };
                }
    }

#endif
