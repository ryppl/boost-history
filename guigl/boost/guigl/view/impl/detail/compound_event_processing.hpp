/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__DETAIL__COMPOUND_EVENT_PROCESSING_HPP
#define BOOST__GUIGL__VIEW__IMPL__DETAIL__COMPOUND_EVENT_PROCESSING_HPP



namespace boost { namespace guigl { namespace view {

namespace detail {
    
    template<typename StaticCompound>
    struct draw
    {
        template<typename View>
        void operator()(View &view) const
        {
            StaticCompound::draw(view);
        }
    };
    
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__DETAIL__COMPOUND_EVENT_PROCESSING_HPP
