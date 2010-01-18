//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_BASIC_LOC_HPP
#define BOOST_PERSISTENT_BASIC_LOC_HPP

...

namespace boost{

namespace persistent{

...

namespace{
template<class> class loc;
}

template<class T,class TxMgr>
class basic_loc : public detail::loc_base{
public:
	//conversions for template pseudo-alias loc
	operator loc<T> &(){ return *static_cast<loc<T> *>(this); }
	operator loc<T> const &() const{ return *static_cast<loc<T> const *>(this); }
	...
};


}
}



#endif // BOOST_PERSISTENT_BASIC_LOC_HPP

