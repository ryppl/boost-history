//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_EXCEPTION_HEADER_HPP
#define BOOST_PERSISTENT_EXCEPTION_HEADER_HPP

#include <exception>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/assert.hpp>
#include <boost/exception/detail/attribute_noreturn.hpp>


namespace boost{
namespace persistent{

namespace detail{

template<class T>
BOOST_ATTRIBUTE_NORETURN void throw_(T const &e){
	throw e;
}

}

///Exception base class.
struct exception : std::exception{};

///Indicates that a database recovery using its transaction log failed.
struct recovery_failure : persistent::exception{};

///Indicates that internal operations reading from/writing to database files failed.
struct io_failure : persistent::exception{};

///Indicates that there was an attempt to modify the database from a user-supplied \c finalize() function.
struct finalize_error : persistent::exception{};

///Indicates that the operation required an active transaction but there was no active transaction set for this thread.
struct no_active_transaction : persistent::exception{};

///Indicates that the operation required an active transaction manager but there is none set.
struct no_active_transaction_manager : persistent::exception{};

///Indicates that the operation is not supported by this implementation
struct unsupported_exception : persistent::exception{};

struct isolation_exception;

namespace detail{

template<class ResMgr>
struct visolation_exception;

template<class TxMgr,class Iterator>
struct isolation_unwind_visitor{
	void operator()(TxMgr &txmgr,isolation_exception const &iso){
		typedef typename mpl::deref<Iterator>::type resource_type;
		if(visolation_exception<resource_type> const *viso=dynamic_cast<visolation_exception<resource_type> const *>(&iso)){
			viso->unwind(txmgr);
		}else{
			isolation_unwind_visitor<TxMgr,typename mpl::next<Iterator>::type> visit;
			visit(txmgr,iso);
		}
	}
};

template<class TxMgr>
struct isolation_unwind_visitor<TxMgr,typename mpl::end<typename TxMgr::resource_types>::type>{
	void operator()(TxMgr &,isolation_exception const &){
		BOOST_ASSERT(false);
	}
};

}

///Indicates that the operation conflicted with another transaction.
struct isolation_exception : persistent::exception{
	///Rethrows the exception if the active transaction is a nested transaction but the isolation exception was caused by a parent transaction of it.
	template<class TxMgr>
	void unwind(TxMgr &txmgr=TxMgr::active()) const{ //pseudo-virtual
		detail::isolation_unwind_visitor<TxMgr,typename mpl::begin<typename TxMgr::resource_types>::type> visit;
		visit(txmgr,*this);
	}
};


namespace detail{

template<class ResMgr>
struct visolation_exception : isolation_exception{
	visolation_exception(typename ResMgr::transaction *unwind_to) : to(unwind_to){}
	template<class TxMgr>
	void unwind(TxMgr &txmgr) const{ //pseudo-virtual
		if(this->to){
			typename ResMgr::transaction &tx=txmgr.template resource_transaction<typename ResMgr::tag>(txmgr.active_transaction());
			if(&tx != this->to) throw;
		}else if(txmgr.has_active_transaction()) throw;
	}
private:
	typename ResMgr::transaction *to;
};

}



}
}



#endif // BOOST_PERSISTENT_EXCEPTION_HEADER_HPP
