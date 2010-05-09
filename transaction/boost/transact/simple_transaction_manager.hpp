//          Copyright Stefan Strasser 2010.
//      Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP
#define BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP

#include <boost/mpl/map.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/optional/optional.hpp>
#include <boost/transact/exception.hpp>
#include <boost/noncopyable.hpp>
#include <boost/transact/detail/static_tss.hpp>
#include <boost/transact/resource_manager.hpp>

namespace boost{
namespace transact{

/// Model of TransactionManager. Only members that are not part of that concept are documented here.
///
/// Equivalent to <tt>basic_transaction_manager<mpl::vector<Resource>,Threads></tt>.
///
/// Template parameters:
/// \li \c Resource The type of the resource manager used.
/// \li \c FlatNested Use emulated flat nested transactions instead of the NestedTransactionService offered by the ResourceManager.
/// \li \c Threads \c true if multiple threads are used to access this transaction manager.
/// \li \c ResourceTag The tag that identifies the Resource
// \brief A transaction manager that only supports one resource manager.
template<class Resource,bool FlatNested=false,bool Threads=true,class ResourceTag=default_tag>
class simple_transaction_manager : noncopyable{
public:
    class transaction;
    /// \cond
    friend class transaction;
private:
    class transaction_construct_t{
        explicit transaction_construct_t(transaction *parent)
            : parent(parent){}
        friend class simple_transaction_manager;
        friend class transaction;
        transaction *parent;
    };
    struct currenttx_tag{};
    typedef transact::detail::static_thread_specific_ptr<
        transaction,
        currenttx_tag,
        Threads> currenttx;

    struct detail{ //for QuickBook
        typedef typename simple_transaction_manager::transaction_construct_t transaction_construct_t;
        typedef std::pair<ResourceTag,Resource &> *resource_iterator;
    };
public:
    class transaction : noncopyable{
    public:
        explicit transaction(transaction_construct_t const &c)
            : rtx(0),rolled_back(false),parent(c.parent){
            simple_transaction_manager::begin_transaction(*this);
        }
    private:
        void reset(){
            this->rtx=0;
            this->rtx_storage=none;
            this->rolled_back=false;
        }
        friend class simple_transaction_manager;
        optional<typename Resource::transaction> rtx_storage;
        typename Resource::transaction *rtx; //if(FlatNested) == root transaction; else == &*rtx_storage
        bool rolled_back;
        transaction * const parent;
    };
    /// \endcond
    typedef mpl::map1<mpl::pair<ResourceTag,Resource> > resource_types;

    /// \brief Constructs a simple_transaction_manager
    simple_transaction_manager(){}

    /// TODO doc, not part of the concept
    static void connect_resource(Resource &newres,ResourceTag const &tag=ResourceTag()){
        if(res){
            if(tag_is_equal(tag)) throw resource_error();
            else throw unsupported_operation();
        }
        res=in_place(tag,boost::ref(newres));
    }

    /// TODO doc, not part of the concept
    static void disconnect_resource(ResourceTag const &tag=ResourceTag()){
        if(res && tag_is_equal(tag)) res=none;
        else throw resource_error();
    }

    static Resource &resource(ResourceTag const &tag=ResourceTag()){
        if(res && tag_is_equal(tag)) return res->second;
        else throw resource_error();
    }

    typedef typename detail::resource_iterator resource_iterator;
    template<class Tag>
    static std::pair<resource_iterator,resource_iterator> resources(){
        BOOST_STATIC_ASSERT((boost::is_same<Tag,ResourceTag>::value));
        if(res) return std::pair<resource_iterator,resource_iterator>(&*res,&*res + 1);
        else return std::pair<resource_iterator,resource_iterator>(0,0);
    }

    static typename Resource::transaction &
    resource_transaction(transaction &tx,ResourceTag const &tag=ResourceTag()){
        if(tx.rtx && tag_is_equal(tag)) return *tx.rtx;
        else throw resource_error();
    }

    static typename detail::transaction_construct_t begin_transaction(){
        return transaction_construct_t(currenttx::get());
    }

    static void commit_transaction(transaction &tx){
        bind_transaction(tx);
        if(res){
            BOOST_ASSERT(tx.rtx);
            res->second.finish_transaction(*tx.rtx);
            res->second.commit_transaction(*tx.rtx);
        }
    }

    static void rollback_transaction(transaction &tx){
        bind_transaction(tx);
        if(res){
            BOOST_ASSERT(tx.rtx);
            BOOST_ASSERT(FlatNested || !tx.rolled_back);
            if(!FlatNested || !tx.rolled_back){
                mark_rolled_back(tx);
                res->second.rollback_transaction(*tx.rtx);
            }
        }
    }

    static void restart_transaction(transaction &tx){
        if(res){
            BOOST_ASSERT(tx.rtx);
            if(FlatNested && tx.rtx != &*tx.rtx_storage) throw unsupported_operation();
            restart_transaction(tx,typename mpl::contains<typename Resource::services,transaction_restart_service_tag>::type());
        }
    }

    static void bind_transaction(transaction &tx){
        currenttx::reset(&tx);
    }
    static void unbind_transaction(){
        currenttx::reset(0);
    }
    static transaction *current_transaction(){
        return currenttx::get();
    }
    /// \cond
private:
    static void begin_transaction(transaction &tx){
        if(res){
            if(tx.parent){
                begin_nested_transaction(tx,mpl::bool_<FlatNested>(),typename has_service<Resource,nested_transaction_service_tag>::type());
            }else{
                tx.rtx_storage=in_place(res->second.begin_transaction());
                tx.rtx=&*tx.rtx_storage;
            }
        }
    }
    template<bool Service>
    static void begin_nested_transaction(transaction &tx,mpl::true_ flatnested,mpl::bool_<Service>){
        if(tx.parent->rtx) tx.rtx=tx.parent->rtx;
        else throw no_transaction();
    }
    static void begin_nested_transaction(transaction &tx,mpl::false_ flatnested,mpl::true_ service){
        BOOST_ASSERT(tx.parent->rtx);
        tx.rtx_storage=in_place(res->second.begin_nested_transaction(*tx.parent->rtx));
        tx.rtx=&*tx.rtx_storage;
    }
    static void begin_nested(transaction &,mpl::false_ flatnested,mpl::false_ service){
        throw unsupported_operation();
    }

    static void mark_rolled_back(transaction &tx){
        tx.rolled_back=true;
        if(FlatNested){
            for(transaction *current=tx.parent;current;current=current->parent){
                current->rolled_back=true;
            }
        }
    }
    static void restart_transaction(transaction &tx,mpl::true_ service){
        BOOST_ASSERT(res);
        res->second.restart_transaction(*tx.rtx);
    }
    static void restart_transaction(transaction &tx,mpl::false_ service){
        tx.reset();
        begin_transaction(tx);
    }
    static bool tag_is_equal(ResourceTag const &o){
        return tag_is_equal(o,boost::is_empty<ResourceTag>());
    }
    static bool tag_is_equal(ResourceTag const &,true_type empty){
        return true;
    }
    static bool tag_is_equal(ResourceTag const &o,false_type empty){
        BOOST_ASSERT(res);
        return res->first == o;
    }

    static optional<std::pair<ResourceTag,Resource &> > res;
    /// \endcond
};

template<class Resource,bool FlatNested,bool Threads,class ResourceTag>
optional<std::pair<ResourceTag,Resource &> > simple_transaction_manager<Resource,FlatNested,Threads,ResourceTag>::res;


}
}



#endif
