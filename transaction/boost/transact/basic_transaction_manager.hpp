//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_BASIC_TRANSACTION_MANAGER_HEADER_HPP
#define BOOST_TRANSACT_BASIC_TRANSACTION_MANAGER_HEADER_HPP

#include <boost/thread/locks.hpp>
#include <boost/thread/tss.hpp>
#include <boost/transact/detail/mutex.hpp>
#include <boost/transact/exception.hpp>
#include <boost/transact/detail/static_tss.hpp>
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
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty_sequence.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/optional/optional.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/insert.hpp>


namespace boost{
namespace transact{

namespace detail{

template<class Resource>
struct get_tag{
	typedef typename Resource::tag type;
};
template<class Resources>
struct default_lazy_resources{
	typedef typename mpl::if_c<
		mpl::size<Resources>::value==1,
		mpl::empty_sequence,
		typename mpl::transform<Resources,get_tag<mpl::_1> >::type
	>::type type;
};

template<class State,class F>
struct runtime_folder{
	explicit runtime_folder(State &state,F &f)
		: state(state),f(f){}
	template<class U>
	void operator()(U x){
		state=f(state,x);
	}
private:
	State &state;
	F &f;
};

template<class Sequence,class State,class F>
State runtime_fold(State state,F f){
	mpl::for_each<Sequence>(runtime_folder<State,F>(state,f));
	return state;
}

}

/// Model of TransactionManager. Only members that are not part of that concept are documented here.
///
/// Template parameters:
/// \li \c Resources A MPL Sequence containing the types of the resource managers used.
/// \li \c Threads \c true if multiple threads are used to access this transaction manager.
/// \li \c TThreads \c true if multiple threads are used to access the same transaction. Can be \c false if multiple threads are used to access the transaction manager, but not to access the same transaction.
/// \li \c Lazy A MPL Sequence of resource tags of those resource managers whose transactions
///        ought to be started lazily, i.e. the local transaction of the resource manager
///        is not started when the global transaction is started but on first
///        access of the resource transaction. This can be beneficial when 2 or more
///        resource managers are used but not every resource is accessed in a global
///        transaction.
///        By default, all resource transactions
///        are started lazily, unless there is only one resource manager used.
/// \brief A transaction manager
template<
	class Resources,
	bool Threads=true,
	bool TThreads=true,
	class Lazy=typename detail::default_lazy_resources<Resources>::type
>
class basic_transaction_manager : noncopyable{
/// \cond
	BOOST_STATIC_ASSERT(Threads || !TThreads);
private:
	struct detail{
		typedef typename mpl::if_<
			mpl::is_sequence<Resources>,
			Resources,
			mpl::vector1<Resources>
		>::type resource_types;

		typedef typename mpl::transform<
			typename detail::resource_types,
			transact::detail::get_tag<mpl::_1>
		>::type resource_tags;

		template<class Resource>
		struct make_resource_pair{
			typedef mpl::pair<typename Resource::tag,Resource> type;
		};
		typedef typename mpl::fold<
			resource_types,
			mpl::map0<>,
			mpl::insert<mpl::_1,make_resource_pair<mpl::_2> >
		>::type resource_types_by_tag;

		template<class Tag>
		struct resource_type{
			typedef typename mpl::at<resource_types_by_tag,Tag>::type type;
		};

		template<class Resource>
		struct get_services{
			typedef typename Resource::services type;
		};
		template<class Service>
		struct default_resource{
			typedef typename mpl::deref<
				typename mpl::find_if<
					resource_types,
					mpl::contains<get_services<mpl::_1>,Service>
				>::type
			>::type::tag type;
		};

		class transaction;
		class transaction_construct_t{
			explicit transaction_construct_t(transaction *parent)
				: parent(parent){}
			template<class R,bool Th,bool TTh,class L> 
			friend class basic_transaction_manager;
			transaction *parent;
		};

		class transaction : noncopyable{
		public:
			explicit transaction(transaction_construct_t const &c) : parent(c.parent){
				mpl::for_each<resource_tags>(beginner(*this));
			}
		private:
			template<class R,bool Th,bool TTh,class L> 
			friend class basic_transaction_manager;
	
			template<class Resource>
			struct make_resource_transaction_pair{
				typedef fusion::pair<typename Resource::tag,optional<typename Resource::transaction> > type;
			};
			typedef typename fusion::result_of::as_map<
				typename mpl::transform<
					resource_types,
					make_resource_transaction_pair<mpl::_1>
				>::type
			>::type resource_transactions_type;
			resource_transactions_type resource_transactions;

			transaction * const parent;

			typedef typename mpl::if_c<
				TThreads,
				transact::detail::mutex_type,
				transact::detail::null_mutex
			>::type mutex_type;
	
			mutex_type mutex;
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
	typedef typename detail::resource_types resource_types;
	template<class ServiceTag>
	struct default_resource{
		typedef typename detail::template default_resource<ServiceTag>::type type;
	};

	/// A basic_transaction_manager constructed using this constructor
	/// is not able to commit transactions that involve two or more persistent
	/// resource managers, i.e. that require a two phase commit protocol.
	/// \brief Constructs a basic_transaction_manager
	basic_transaction_manager(){}

	/// TODO doc, not part of the concept
	template<class Resource>
	static void connect_resource(Resource &newres){
		Resource *&res=fusion::at_key<typename Resource::tag>(resources);
		if(res) throw resource_error();
		res=&newres;
	}

	/// TODO doc, not part of the concept
	template<class ResourceTag>
	static void disconnect_resource(ResourceTag tag=ResourceTag()){
		fusion::at<ResourceTag>(resources)=0;		
	}

	template<class Tag>
	static typename detail::resource_type<Tag>::type &resource(Tag tag=Tag()){
		typename detail::resource_type<Tag>::type *res
			=fusion::at_key<Tag>(resources);
		if(res) return *res;
		else throw resource_error();
	}

	template<class Tag>
	static typename detail::resource_type<Tag>::type::transaction &
	resource_transaction(transaction &tx,Tag tag=Tag()){
		return resource_transaction(tx,tag,typename mpl::contains<Lazy,Tag>());
	}

	static typename detail::transaction_construct_t begin_transaction(){
		return typename detail::transaction_construct_t(activetx::get());
	}

	static void commit_transaction(transaction &tx){
		typedef typename detail::resource_tags tags;
		bind_transaction(tx);

		//call finish_transaction until all transactions return false
		while(transact::detail::runtime_fold<tags>(false,finisher(tx)));

		mpl::for_each<tags>(preparer<false>(tx)); //prepare transient two-phase transactions
		//count persistent transactions and transactions that only support one-phase commit:
		std::size_t pers=transact::detail::runtime_fold<tags>(0,persistent_counter(tx));
		if(pers > 1){
			mpl::for_each<tags>(preparer<true>(tx)); //prepare persistent transactions
			//TODO write commit message to log
			mpl::for_each<tags>(committer<true>(tx)); //commit persistent transactions
		}else{
			mpl::for_each<tags>(committer<true>(tx)); //commit persistent transaction unprepared
		}
		mpl::for_each<tags>(committer<false>(tx)); //commit transient two-phase transactions
	}

	static void rollback_transaction(transaction &tx){
		bind_transaction(tx);
		mpl::for_each<detail::resource_tags>(rollbacker(tx));
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
	struct persistent_counter{
		explicit persistent_counter(transaction &tx) : tx(tx){}
		template<class Tag>
		std::size_t operator()(std::size_t c,Tag){
			typedef typename detail::resource_type<Tag>::type res_type;
			if(is_persistent<res_type>::type::value){
				if(fusion::at_key<Tag>(tx.resource_transactions)){
					return c+1;
				}
			}
			return c;
		}
	private:
		transaction &tx;
	};
	struct finisher{
		explicit finisher(transaction &tx) : tx(tx){}
		template<class Tag>
		bool operator()(bool repeat,Tag tag){
			typedef typename detail::resource_type<Tag>::type res_type;
			optional<typename res_type::transaction> &rtx=
				fusion::at_key<Tag>(tx.resource_transactions);
			if(rtx){
				res_type *rmgr=fusion::at_key<Tag>(resources);
				BOOST_ASSERT(rmgr);
				return rmgr->finish_transaction(*rtx) || repeat;
			}
			return repeat;
		}
	private:
		transaction &tx;
	};
	struct beginner{
		explicit beginner(transaction &tx) : tx(tx){}
		template<class Tag>
		void operator()(Tag tag){
			this->begin(tag,typename mpl::contains<Lazy,Tag>::type());
		}
	private:
		template<class Tag>
		void begin(Tag,mpl::true_ lazy){}
		template<class Tag>
		void begin(Tag,mpl::false_ lazy){
			typedef typename detail::resource_type<Tag>::type res_type;
			res_type *rmgr=fusion::at_key<Tag>(resources);
			if(rmgr){
				optional<typename res_type::transaction> &rtx=
					fusion::at_key<Tag>(tx.resource_transactions);
				BOOST_ASSERT(!rtx);
				if(tx.parent){
					typename res_type::transaction &parentrtx=
						resource_transaction<Tag>(*tx.parent);
					rtx=in_place(rmgr->begin_nested_transaction(parentrtx));
				}else rtx=in_place(rmgr->begin_root_transaction());
			}
		}
		transaction &tx;
	};
	template<class Tag>
	static typename detail::resource_type<Tag>::type::transaction &
	resource_transaction(transaction &tx,Tag tag,mpl::true_ lazy){
		typedef typename detail::resource_type<Tag>::type res_type;
		optional<typename res_type::transaction> &rtx=
			fusion::at_key<Tag>(tx.resource_transactions);

		lock_guard<typename transaction::mutex_type> l(tx.mutex);
		if(!rtx){
			res_type &res=resource(tag);
			if(tx.parent){
				//TODO optimization: if 10 nested transactions were created in one resource manager,
				//and then a second resource manager is called for the first time in the innermost
				//transaction, 10 nested transactions are created in the second resource manager,
				//even though it would be enough to create one transaction
				//that gets committed only when the outermost global transaction is committed.
				//(or is moved to the parent on commit instead of performing an actual commit?)

				typename res_type::transaction &parentrtx=resource_transaction<Tag>(*tx.parent);
				rtx=in_place(res.begin_nested_transaction(parentrtx));
			}else{
				rtx=in_place(res.begin_root_transaction());
			}
		}
		return *rtx;
	}
	template<class Tag>
	static typename detail::resource_type<Tag>::type::transaction &
	resource_transaction(transaction &tx,Tag tag,mpl::false_ lazy){
		typedef typename detail::resource_type<Tag>::type res_type;
		optional<typename res_type::transaction> &rtx=
			fusion::at_key<Tag>(tx.resource_transactions);
		BOOST_ASSERT(rtx);
		return *rtx;
	}
	template<class Resource>
	struct is_persistent{
		typedef mpl::true_ type; //FIXME
	};
	template<class Resource>
	struct is_two_phase{
		typedef mpl::false_ type; //FIXME
	};
	
	template<bool Persistent>
	struct committer{
		explicit committer(transaction &tx) : tx(tx){}
		template<class Tag>
		void operator()(Tag){
			typedef typename detail::resource_type<Tag>::type res_type;
			if(Persistent == is_persistent<res_type>::type::value){
				optional<typename res_type::transaction> &rtx=
					fusion::at_key<Tag>(tx.resource_transactions);
				if(rtx){
					res_type *res=fusion::at_key<Tag>(resources);
					BOOST_ASSERT(res);
					res->commit_transaction(*rtx);
				}
			}
		}
	private:
		transaction &tx;
	};
	template<bool Persistent>
	struct preparer{
		explicit preparer(transaction &tx) : tx(tx){}
		template<class Tag>
		void operator()(Tag){
			typedef typename detail::resource_type<Tag>::type res_type;
			if(Persistent == is_persistent<res_type>::type::value){
				typedef typename detail::resource_type<Tag>::type res_type;
				optional<typename res_type::transaction> &rtx=
					fusion::at_key<Tag>(tx.resource_transactions);
				if(rtx){
					res_type *res=fusion::at_key<Tag>(resources);
					BOOST_ASSERT(res);
					this->prepare(*res,*rtx,typename is_two_phase<res_type>::type());
				}
			}
		}
	private:
		template<class Resource>
		void prepare(Resource &res,typename Resource::transaction &rtx,mpl::true_){
			res.prepare_transaction(rtx);
		}
		template<class Resource>
		void prepare(Resource &res,typename Resource::transaction &rtx,mpl::false_){
			//a resource that does not support two-phase-commit was used
			//together with other persistent resources
			throw unsupported_exception();
		}
		transaction &tx;
	};
	struct rollbacker{
		explicit rollbacker(transaction &tx) : tx(tx){}
		template<class Tag>
		void operator()(Tag){
			typedef typename detail::resource_type<Tag>::type res_type;
			optional<typename res_type::transaction> &rtx=
				fusion::at_key<Tag>(tx.resource_transactions);
			if(rtx){
				res_type *res=fusion::at_key<Tag>(resources);
				BOOST_ASSERT(res);
				res->rollback_transaction(*rtx);
			}
		}
	private:
		transaction &tx;
	};
	template<class Resource>
	struct make_resource_ptr_pair{
		typedef fusion::pair<typename Resource::tag,Resource *> type;
	};
	typedef typename fusion::result_of::as_map<
		typename mpl::transform<
			resource_types,
			make_resource_ptr_pair<mpl::_1>
		>::type
	>::type resources_type;
	static resources_type resources;

	/// \endcond
};

template<class Res,bool Thr,bool TThr,class Lazy>
typename basic_transaction_manager<Res,Thr,TThr,Lazy>::resources_type
basic_transaction_manager<Res,Thr,TThr,Lazy>::resources;

}
}



#endif