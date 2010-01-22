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
#include <boost/type_traits/add_pointer.hpp>
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
		typedef typename mpl::transform<resource_types,add_pointer<mpl::_1> >::type resource_pointers;
		typedef typename fusion::result_of::as_vector<resource_pointers>::type resources_tuple;

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
	basic_transaction_manager();
	/// \endcond
public:
	typedef typename detail::transaction transaction;
	typedef typename detail::resource_types resource_types;
	template<class ServiceTag>
	struct default_resource{
		typedef typename detail::template default_resource<ServiceTag>::type type;
	};

	/// TODO doc, not part of the concept
	template<class Resource>
	static void connect_resource(Resource &newres){
		typedef typename Resource::tag tag;
		typedef typename persistent::detail::index_by_tag<resource_types,tag>::type index;
		Resource *&res=fusion::at<index>(resources);
		if(res) persistent::detail::throw_(resource_error());
		res=&newres;
	}

	/// TODO doc, not part of the concept
	template<class ResourceTag>
	static void disconnect_resource(ResourceTag tag=ResourceTag()){
		typedef typename persistent::detail::index_by_tag<resource_types,ResourceTag>::type index;
		fusion::at<index>(resources)=0;		
	}

	template<class Tag>
	static typename persistent::detail::type_by_tag<resource_types,Tag>::type &resource(Tag tag=Tag()){
		typedef typename persistent::detail::type_by_tag<resource_types,Tag>::type resource_type;
		typedef typename persistent::detail::index_by_tag<resource_types,Tag>::type index;
		resource_type *res=fusion::at<index>(resources);
		if(res) return *res;
		else persistent::detail::throw_(resource_error());
	}

	template<class Tag>
	static typename persistent::detail::type_by_tag<resource_types,Tag>::type::transaction &
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
			resource_type &res=resource(tag);
			if(tx.parent){
				//TODO optimization: if 10 nested transactions were created in one resource manager,
				//and then a second resource manager is called for the first time in the innermost
				//transaction, 10 nested transactions are created in the second resource manager,
				//even though it would be enough to create one transaction
				//that gets committed only when the outermost global transaction is committed.
				//(or is moved to the parent on commit instead of performing an actual commit?)

				typename resource_type::transaction &parentrtx=resource_transaction<Tag>(*tx.parent);
				rtx=in_place(res.begin_nested_transaction(parentrtx));
			}else{
				rtx=in_place(res.begin_root_transaction());
			}
		}
		return *rtx;
	}

	static typename detail::transaction_construct_t begin_transaction(){
		return typename detail::transaction_construct_t(active_transaction(mpl::bool_<Threads>()));
	}

	static void commit_transaction(transaction &tx){
		bind_transaction(tx);

		std::size_t nr=fusion::count_if(tx.resource_transactions,is());
		if(nr > 0){
			if(nr == 1){
				//FIXME the local transaction commit might access other resource managers
				//(e.g. for reference counting). that can start another resource transaction
				//and a two-phase-commit must take place.
				//finish-phase must be introduced to make sure the commit itself doesn't
				//access other resource managers
				for_each_transaction(tx,committer());
			}else{
				persistent::detail::throw_(unsupported_exception()); //TODO distributed transaction
			}
		}
	}

	static void rollback_transaction(transaction &tx){
		bind_transaction(tx);

		for_each_transaction(tx,rollbacker());
	}

	static void bind_transaction(transaction &tx){
		active_transaction(&tx,mpl::bool_<Threads>());
	}
	static void unbind_transaction(){
		active_transaction(0,mpl::bool_<Threads>());
	}

	static transaction &active_transaction(){
		if(transaction *tx=active_transaction(mpl::bool_<Threads>())) return *tx;
		else persistent::detail::throw_(no_active_transaction());
	}
	static bool has_active_transaction(){
		return active_transaction(mpl::bool_<Threads>()) ? true : false;
	}

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
		explicit for_each_helper(transaction &tx,F const &f) : tx(tx),f(f){}
		template<class U>
		void operator()(U) const{
			typedef typename mpl::at<resource_types,U>::type resource_type;
			optional<typename resource_type::transaction> &rtx=fusion::at<U>(tx.resource_transactions);
			if(rtx){
				resource_type *rmgr=fusion::at<U>(resources);
				BOOST_ASSERT(rmgr);
				f(*rmgr,*rtx);
			}
		}
	private:
		transaction &tx;
		F f;
	};
	template<class F>
	static void for_each_transaction(transaction &tx,F const &f){
		static unsigned int const size=mpl::size<resource_types>::type::value;
		typedef mpl::range_c<unsigned int,0,size> range;
		mpl::for_each<range>(for_each_helper<F>(tx,f));
	}
#if defined(__GNUG__) || defined(BOOST_MSVC)
	template<class T>
	static void active_transaction(transaction *newtx,T){ activetx=newtx; }
	template<class T>
	static transaction *active_transaction(T){ return activetx; }
	typedef transaction *activetx_type;
	#ifdef BOOST_MSVC
		static __declspec(thread) transaction *activetx; //TODO MSVC untested
	#else
		static __thread transaction *activetx;
	#endif
#else
	static void active_transaction(transaction *newtx,mpl::true_){ activetx.reset(newtx); }
	static void active_transaction(transaction *newtx,mpl::false_){ activetx=newtx; }
	static transaction *active_transaction(mpl::true_){ return activetx.get(); }
	static transaction *active_transaction(mpl::false_){ return activetx; }
	typedef typename mpl::if_c<
		Threads,
		thread_specific_ptr<transaction>,
		transaction *>::type activetx_type;	
	activetx_type activetx;
#endif


	typedef typename detail::resources_tuple resources_type;
	static resources_type resources;
	/// \endcond
};

template<class Res,bool Thr,bool TThr>
typename basic_transaction_manager<Res,Thr,TThr>::resources_type
basic_transaction_manager<Res,Thr,TThr>::resources;

template<class Res,bool Thr,bool TThr>
#if defined(__GNUG__)
__thread
#elif defined(BOOST_MSVC)
__declspec(thread)
#endif
typename basic_transaction_manager<Res,Thr,TThr>::activetx_type
basic_transaction_manager<Res,Thr,TThr>::activetx(0); //the 0-argument either initializes
//the pointer to 0, or passes a 0 cleanup function to the thread_specific_ptr constructor.


}
}



#endif // BOOST_PERSISTENT_BASIC_TRANSACTION_MANAGER_HEADER_HPP