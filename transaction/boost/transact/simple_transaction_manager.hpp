//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP
#define BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP

#include <boost/mpl/vector.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/optional/optional.hpp>
#include <boost/transact/exception.hpp>
#include <boost/noncopyable.hpp>
#include <boost/transact/detail/static_tss.hpp>

namespace boost{
namespace transact{

/// Model of TransactionManager. Only members that are not part of that concept are documented here.
///
/// Equivalent to <tt>basic_transaction_manager<mpl::vector<Resource>,Threads></tt>.
///
/// Template parameters:
/// \li \c Resource The type of the resource manager used.
/// \li \c Threads \c true if multiple threads are used to access this transaction manager.
// \brief A transaction manager that only supports one resource manager.
template<class Resource,bool Threads=true>
class simple_transaction_manager : noncopyable{
private:
	struct detail{
		class transaction;
		class transaction_construct_t{
			explicit transaction_construct_t(transaction *parent)
				: parent(parent){}
			template <class R, bool Th> friend class simple_transaction_manager;
			friend class transaction;
			transaction *parent;
		};

		class transaction : noncopyable{
		public:
			explicit transaction(transaction_construct_t const &c)
				: parent(c.parent){
				if(res){
					if(this->parent){
						this->rtx=in_place(res->begin_nested_transaction(*this->parent->rtx));
					}else{
						this->rtx=in_place(res->begin_root_transaction());
					}
				}
			}
		private:
			template <class R, bool Th> friend class simple_transaction_manager;
			optional<typename Resource::transaction> rtx;
			transaction * const parent;
		};
	};
	struct activetx_tag{};
	typedef transact::detail::static_thread_specific_ptr<
		typename detail::transaction,
		activetx_tag,
		Threads> activetx;
	/// \endcond
public:
	typedef typename detail::transaction transaction;
	typedef mpl::vector<Resource> resource_types;
	template<class ServiceTag>
	struct default_resource{
		typedef typename Resource::tag type;
	};

	/// \brief Constructs a simple_transaction_manager
	simple_transaction_manager(){}

	/// TODO doc, not part of the concept
	static void connect_resource(Resource &newres){
		if(res) throw resource_error();
		res=&newres;
	}

	/// TODO doc, not part of the concept
	static void disconnect_resource(typename Resource::tag tag=typename Resource::tag()){
		res=0;
	}

	static Resource &resource(typename Resource::tag tag=typename Resource::tag()){
		if(res) return *res;
		else throw resource_error();
	}

	static typename Resource::transaction &
	resource_transaction(transaction &tx,typename Resource::tag tag=typename Resource::tag()){
		if(tx.rtx) return *tx.rtx;
		else throw resource_error();
	}

	static typename detail::transaction_construct_t begin_transaction(){
		return typename detail::transaction_construct_t(activetx::get());
	}

	static void commit_transaction(transaction &tx){
		bind_transaction(tx);
		if(res){
			BOOST_ASSERT(tx.rtx);
			res->finish_transaction(*tx.rtx);
			res->commit_transaction(*tx.rtx);
		}
	}

	static void rollback_transaction(transaction &tx){
		bind_transaction(tx);
		if(res){
			BOOST_ASSERT(tx.rtx);
			res->rollback_transaction(*tx.rtx);
		}
	}

	static void bind_transaction(transaction &tx){
		activetx::reset(&tx);
	}
	static void unbind_transaction(){
		activetx::reset(0);
	}
	static transaction &active_transaction(){
		if(transaction *tx=activetx::get()) return *tx;
		else throw no_active_transaction();
	}
	static bool has_active_transaction(){
		return activetx::get() ? true : false;
	}

	/// \cond
private:
	static Resource *res;
	/// \endcond
};

template<class Res,bool Thr>
Res *simple_transaction_manager<Res,Thr>::res=0;


}
}



#endif
