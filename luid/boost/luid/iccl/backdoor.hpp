//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/luid for documentation.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef BOOST_LUID_BACKDOOR_HPP_
#define BOOST_LUID_BACKDOOR_HPP_

namespace boost { namespace luid { namespace iccl {
    
/**
 */

struct backdoor {
    
    template <typename node_pointer_type, typename FINAL, typename numeric_type>
    static node_pointer_type make_node(FINAL& f, numeric_type e) {
        return f.make_node(e);
    }

    template <typename FINAL, typename node_pointer_type>
    static bool valid_node(const FINAL& f, const node_pointer_type e) {
        return f.valid_node(e);
    }
    
    
};

} } }
#endif // BOOST_LUID_BACKDOOR_HPP_
