/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_NUIDG_RECOVERABLE_HPP_
#define BOOST_NUIDG_RECOVERABLE_HPP_

//#include <queue>

namespace boost
{
namespace luid
{
namespace iccl
{
    
/**
 */    
template <typename Inner>
class recoverable_distance : public Inner
{
public:
    typedef Inner super_type;
    typedef typename super_type::config config;
    typedef typename config::final final;
    typedef typename config::numeric_type numeric_type;
    typedef typename config::distance_type distance_type;

    typedef typename config::void_pointer_type void_pointer_type;

    // forwards construction
    template <typename ArgsParam>              
    recoverable_distance(const ArgsParam& args) 
    : super_type(args)
    , distance_(args[distance|0]) 
    {}

    void release(numeric_type n) {
            if (distance_>queue_.size()) {
              distance_type required = distance_-queue_.size();
              numeric_type r = super_type::make(required);
                for (std::size_t d=required; d>0; --d) {
                    super_type::release(r-d+1);
                }
          }
            super_type::release(n);
    }
private:
    std::size_t distance_;
};

}
}
}
#endif /*RECOVERABLE_HPP_*/
