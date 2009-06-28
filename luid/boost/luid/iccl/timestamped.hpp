/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_TIMESTAMPED_HPP_
#define BOOST_LUID_TIMESTAMPED_HPP_

namespace boost { namespace luid { namespace iccl {    
    
template <typename time_traits, typename super_element_type>
class timestamped : public super_element_type {
public:
          typedef typename time_traits::time_type time_type;
          typedef typename time_traits::duration_type duration_type;
    timestamped(const duration_type delay, 
                const typename super_element_type::numeric_type e) 
    : super_element_type(e)
    , dead_on_(time_traits::now()+delay)
    {
    }

    bool alive() const {
//            std::cout << "now=" << time_traits::now() << " dead_on=" << dead_on_ << std::endl;        
        return time_traits::now() < dead_on_;  
    }        
    time_type dead_on() const {
        return dead_on_;  
    }        
private:
    const time_type dead_on_;
};
} } }
#endif /*BOOST_LUID_TIMESTAMPED_HPP_*/
