///////////////////////////////////////////////////////////////////////////////
//  ref.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_GENERATOR_REF_HPP_ER_2009
#define BOOST_GENERATOR_REF_HPP_ER_2009
#include <boost/shared_ptr.hpp>
#include <boost/utility/remove_qualifier.hpp>

namespace boost{
namespace generator{

        // A generator is likely to contain its own seed
        // But as_random_distribution makes a copy
        // hence the need for a ref wrapper


        template<typename G>
        class ref{
            typedef typename utility::remove_qualifier<G>::type noqual;
            BOOST_MPL_ASSERT((
                is_same<G,noqual>
            ));
            typedef shared_ptr<G> pimpl_t;
            public:
            typedef typename G::result_type result_type;

            ref(pimpl_t g):pimpl_(g){}
            ref(const ref& that):pimpl_(that.pimpl_){}
            ref& operator=(const ref& that){
                if(&that!=this){
                    pimpl_ = that.pimpl_;
                }
                return *this;
            }

            result_type operator()(){
                return pimpl_->operator()();
            }

            private:
            ref();
            pimpl_t pimpl_;
        };


}
}

#endif // REF_HPP_INCLUDED
