//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PERSISTENT_BASIC_TRANSACTION_MANAGER_HEADER_HPP
#define BOOST_PERSISTENT_BASIC_TRANSACTION_MANAGER_HEADER_HPP

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/tss.hpp>
#include <boost/persistent/detail/mutex.hpp>
#include <boost/persistent/exception.hpp>
#include <boost/persistent/detail/utility.hpp>
#include <boost/persistent/detail/null_mutex.hpp>
#include <boost/persistent/detail/mutex.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/count_if.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/optional/optional.hpp>
#include <boost/ref.hpp>



namespace boost{
namespace persistent{


/// Model of TransactionManager. Only members that are not part of that concept are documented here.
///
/// Template parameters:
/// \li \c Resources Either the type of the only resource manager used, or a MPL Sequence containing the types of the resource managers used.
/// \li \c Threads \c true if multiple threads are used to access this transaction manager.
/// \li \c TThreads \c true if multiple threads are used to access the same transaction. Can be \c false if multiple threads are used to access the transaction manager, but not to access the same transaction.
/// \brief A transaction manager
template<class Resources,bool Threads=true,bool TThreads=true>
class basic_transaction_manager : noncopyable{
/// \cond
	BOOST_STATIC_ASSERT(Threads || !TThreads);
private:
	struct detail{
		typedef typename mpl::if_<mpl::is_sequence<Resources>,Resources,mpl::vector<Resources> >::type resource_types;
		template<class Service>
		struct default_resource{
			typedef typename mpl::deref<
				typename mpl::find_if<
					resource_types,
					persistent::detail::resource_has_service<mpl::_1,Service>
				>::type
			>::type::tag type;
		};
		typedef typename mpl::transform<resource_types,add_reference<mpl::_1> >::type resource_references;
		typedef typename fusion::result_of::as_vector<resource_references>::type resources_tuple;

		class transaction;
		class transaction_construct_t{
			explicit transaction_construct_t(transaction *parent) : parent(parent){}
			friend class basic_transaction_manager;
			transaction *parent;
		};

		template<class Resource> struct get_optional_transaction{
			typedef optional<typename Resource::transaction> type;
		};
		typedef typename mpl::transform<resource_types,get_optional_transaction<mpl::_1> >::type optional_transaction_types;
		typedef typename fusion::result_of::as_vector<optional_transaction_types>::type optional_transaction_tuple;

		class transaction : noncopyable{
		public:
			transaction(transaction_construct_t const &c) : parent(c.parent){}
		private:
			friend class basic_transaction_manager;
	
			transaction * const parent;
			optional_transaction_tuple resource_transactions;
	
			typedef typename mpl::if_c<
				TThreads,
				persistent::detail::mutex_type,
				persistent::detail::null_mutex
			>::type mutex_type;
	
			mutex_type mutex;
		};
	};
	/// \endcond
public:
	typedef typename detail::transaction transaction;
	typedef typename detail::resource_types resource_types;
	template<class ServiceTag>
	struct default_resource{
		typedef typename detail::template default_resource<ServiceTag>::type type;
	};

	/// This constructor is only available if this transaction manager uses only one
	/// resource manager.
	/// 
	/// Throws: Nothing
	/// \brief Constructs a basic_transaction_manager using the passed resource manager
	/// \param resource The resource manager
	explicit basic_transaction_manager(typename mpl::front<resource_types>::type &resource)
		: resources(ref(resource))
		, activetx(){
		BOOST_STATIC_ASSERT(mpl::size<resource_types>::value==1);
		this->bind();
	}

	/// Throws: Nothing
	/// \brief Constructs a basic_transaction_manager using the passed resource managers
	/// \param resources A Boost.Fusion Sequence of non-const references to the resource
	///                  managers. For example, fusion::vector<res1_type &,res2_type &>
	explicit basic_transaction_manager(typename detail::resources_tuple const &resources)
		: resources(resources)
		, activetx(){
		this->bind();
	}

	template<class Tag>
	typename persistent::detail::type_by_tag<resource_types,Tag>::type &resource(Tag tag=Tag()){
		typedef typename persistent::detail::index_by_tag<resource_types,Tag>::type index;
		return fusion::at<index>(this->resources);
	}

	template<class Tag>
	typename persistent::detail::type_by_tag<resource_types,Tag>::type::transaction &
	resource_transaction(transaction &tx,Tag tag=Tag()){
		typedef typename persistent::detail::type_by_tag<resource_types,Tag>::type resource_type;
		typedef typename persistent::detail::index_by_tag<resource_types,Tag>::type index;
		optional<typename resource_type::transaction> &rtx=fusion::at<index>(tx.resource_transactions);

		//TODO optimization: mutex lock could be avoided if a resource transaction of this
		//resource is created in each global transaction, so the resource transaction can be
		//created in basic_transaction_manager::begin_transaction() instead.
		//e.g. determined by if this resource
		//manager is the only one, or according to user configuration.
		lock_guard<typename transaction::mutex_type> l(tx.mutex);
		if(!rtx){
			resource_type &resource=fusion::at<index>(this->resources);
			if(tx.parent){
				//TODO optimization: if 10 nested transactions were created in one resource manager,
				//and then a second resource manager is called for the first time in the innermost
				//transaction, 10 nested transactions are created in the second resource manager,
				//even though it would be enough to create one transaction
				//that gets committed only when the outermost global transaction is committed.

				typename resource_type::transaction &parentrtx=this->resource_transaction<Tag>(*tx.parent);
				rtx=in_place(resource.begin_nested_transaction(parentrtx));
			}else{
				rtx=in_place(resource.begin_root_transaction());
			}
		}
		return *rtx;
	}

	typename detail::transaction_construct_t begin_transaction(){
		if(this->has_active_transaction()) return typename detail::transaction_construct_t(&this->active_transaction());
		else return typename detail::transaction_construct_t(0);
	}

	void commit_transaction(transaction &tx){
		this->bind_transaction(tx);

		try{
			std::size_t nr=fusion::count_if(tx.resource_transactions,is());
			if(nr > 0){
				if(nr == 1){
					//FIXME the local transaction commit might access other resource managers
					//(e.g. for reference counting). that can start another resource transaction
					//and a two-phase-commit must take place.
					//finish-phase must be introduced to make sure the commit itself doesn't
					//access other resource managers
					this->for_each_transaction(tx,committer());
				}else{
					persistent::detail::throw_(unsupported_exception()); //TODO distributed transaction
				}
			}
		}catch(...){ //commit_transaction is called exactly once, even if something goes wrong. unbind transaction:
			if(tx.parent) this->bind_transaction(*tx.parent);
			else this->unbind_transaction();
			throw;
		}
		if(tx.parent) this->bind_transaction(*tx.parent);
		else this->unbind_transaction();
	}

	void rollback_transaction(transaction &tx){
		this->bind_transaction(tx);

		try{ this->for_each_transaction(tx,rollbacker()); }
		catch(...){
			if(tx.parent) this->bind_transaction(*tx.parent);
			else this->unbind_transaction();
			throw;
		}

		if(tx.parent) this->bind_transaction(*tx.parent);
		else this->unbind_transaction();
	}

	void bind_transaction(transaction &tx){
		this->active_transaction(&tx,mpl::bool_<Threads>());
	}
	void unbind_transaction(){
		this->active_transaction(0,mpl::bool_<Threads>());
	}

	transaction &active_transaction() const{
		if(transaction *tx=this->active_transaction(mpl::bool_<Threads>())) return *tx;
		else persistent::detail::throw_(no_active_transaction());
	}
	bool has_active_transaction() const{
		return this->active_transaction(mpl::bool_<Threads>()) ? true : false;
	}

	static bool has_active(){
		return active_;
	}

	static basic_transaction_manager &active(){
		if(active_) return *active_;
		else persistent::detail::throw_(no_active_transaction_manager());
	}
	void bind(){ active_=this; }
	static void unbind(){ active_=0; }

	/// \cond
private:
	struct is{
		typedef bool result_type;
		template<class T>
		bool operator()(T const &t) const{ return t; }
	};
	struct committer{
		template<class ResMgr,class Tx>
		void operator()(ResMgr &rmgr,Tx &tx) const{
			rmgr.commit_transaction(tx);
		}
	};
	struct rollbacker{
		template<class ResMgr,class Tx>
		void operator()(ResMgr &rmgr,Tx &tx) const{
			rmgr.rollback_transaction(tx);
		}
	};
	template<class F>
	struct for_each_helper{
		explicit for_each_helper(basic_transaction_manager *this_,transaction &tx,F const &f) : this_(this_),tx(tx),f(f){}
		template<class U>
		void operator()(U) const{
			typedef typename mpl::at<resource_types,U>::type resource_type;
			optional<typename resource_type::transaction> &rtx=fusion::at<U>(tx.resource_transactions);
			if(rtx){
				resource_type &rmgr=fusion::at<U>(this_->resources);
				f(rmgr,*rtx);
			}
		}
	private:
		basic_transaction_manager *this_;
		transaction &tx;
		F f;
	};
	template<class F>
	void for_each_transaction(transaction &tx,F const &f){
		static unsigned int const size=mpl::size<resource_types>::type::value;
		typedef mpl::range_c<unsigned int,0,size> range;
		mpl::for_each<range>(for_each_helper<F>(this,tx,f));
	}
	void active_transaction(transaction *newtx,mpl::true_){
		if(transaction **tx=this->activetx.get()) *tx=newtx;
		else this->activetx.reset(new transaction *(newtx));
	}
	void active_transaction(transaction *newtx,mpl::false_){
		this->activetx=newtx;
	}
	transaction *active_transaction(mpl::true_) const{
		if(transaction **tx=this->activetx.get()){
			if(*tx) return *tx;
		}
		return 0;	
	}
	transaction *active_transaction(mpl::false_) const{
		return this->activetx;
	}

	typename detail::resources_tuple const resources;
	//TODO optimization: use thread local variable instead of thread_specific_ptr
	typename mpl::if_c<Threads,thread_specific_ptr<transaction *>,transaction *>::type activetx;
	static basic_transaction_manager *active_; 
	/// \endcond
};

template<class Resources,bool Threads,bool TThreads>
basic_transaction_manager<Resources,Threads,TThreads> *basic_transaction_manager<Resources,Threads,TThreads>::active_=0;

}
}



#endif // BOOST_PERSISTENT_BASIC_TRANSACTION_MANAGER_HEADER_HPP