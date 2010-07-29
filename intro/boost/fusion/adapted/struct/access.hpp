/*=============================================================================
    Copyright (c) 2010 Stefan Strasser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_FUSION_ADAPTED_STRUCT_ACCESS)
#define BOOST_FUSION_ADAPTED_STRUCT_ACCESS

#include <boost/fusion/adapted/struct/extension.hpp>

namespace boost{
namespace fusion{

class access{
private:
    template<typename Struct,int N>
    friend struct fusion::extension::struct_member;
    template<typename Struct,int N>
    struct result;
    template<typename Struct,int N>
    static typename result<Struct,N>::type &call(Struct &);
    template<typename To,typename From>
    static To &upcast(From &from){ //for private inheritance
        return from;
    }
};

}
}


#endif
