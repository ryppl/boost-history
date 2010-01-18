//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_DEFAULT_LOC_HPP
#define BOOST_PERSISTENT_DEFAULT_LOC_HPP

#include <boost/persistent/basic_loc.hpp>
#include <boost/persistent/transaction_manager.hpp>

namespace boost{
namespace persistent{

#ifdef DOXYGEN

/// \brief A template alias of \c basic_loc using the default transaction manager
///
/// \code
/// template<class T>
/// using loc = basic_loc<T,transaction_manager>;
/// \endcode
/// In C++98, \c loc ist implemented as a class that acts like a template alias.
template<class T>
class loc{};

#else

namespace{

template<class T>
class loc : public basic_loc<T,transaction_manager>{
private:
	typedef basic_loc<T,transaction_manager> base;
public:
	//forwarding constructors:
	loc(){}
	template<class Y>
	loc(loc<Y> const &o) : base(o){}
	template<class Y>
	loc(basic_loc<Y,transaction_manager> const &o) : base(o){}
	explicit loc(typename remove_const<T>::type *p) : base(p){}
	template<class Tag>
	loc(typename remove_const<T>::type *p,Tag tag) : base(p,tag){}
};

}

#endif

}

...

}



#endif