//          Copyright Stefan Strasser 2010.
//      Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP
#define BOOST_TRANSACT_SIMPLE_TRANSACTION_MANAGER_HEADER_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/bool.hpp>
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
/// \li \c Threads \c true if multiple threads are used to access this transaction manager.
// \brief A transaction manager that only supports one resource manager.
template<class Resource,bool Threads=true>
class simple_transaction_manager : noncopyable{
public:
    class transaction;
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
    };
    /// \endcond
public:
    class transaction : noncopyable{
    /// \cond
    public:
        explicit transaction(transaction_construct_t const &c)
            : parent(c.parent){
            if(res){
                if(this->parent){
                    BOOST_ASSERT(this->parent->rtx);
                    this->rtx=in_place(res->begin_nested_transaction(*this->parent->rtx));
                }else{
                    this->rtx=in_place(res->begin_root_transaction());
                }
            }
        }
    private:
        friend class simple_transaction_manager;
        optional<typename Resource::transaction> rtx;
        transaction * const parent;
    /// \endcond
    };
    typedef mpl::vector1<Resource> resource_types;
    template<class ServiceTag>
    struct default_resource{
        typedef typename Resource::tag type;
    };

    typedef typename Resource::tag resource_tag_t;
    
    /// \brief Constructs a simple_transaction_manager
    simple_transaction_manager(){}

    /// TODO doc, not part of the concept
    static void connect_resource(Resource &newres){
        if(res) throw resource_error();
        res=&newres;
    }

    /// TODO doc, not part of the concept
    static void disconnect_resource(resource_tag_t tag= resource_tag_t()){
        res=0;
    }

    static Resource &resource(resource_tag_t tag= resource_tag_t()){
        if(res) return *res;
        else throw resource_error();
    }

    static typename Resource::transaction &
    resource_transaction(transaction &tx,resource_tag_t tag= resource_tag_t()){
        if(tx.rtx) return *tx.rtx;
        else throw resource_error();
    }

    static typename detail::transaction_construct_t begin_transaction(){
        return transaction_construct_t(currenttx::get());
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

    static void restart_transaction(transaction &tx){
        if(res){
            BOOST_ASSERT(tx.rtx);
            restart_transaction(tx,typename mpl::contains<typename Resource::services,transaction_restart_service_tag>::type());
        }
    }

    static void bind_transaction(transaction &tx){
        currenttx::reset(&tx);
    }
    static void unbind_transaction(){
        currenttx::reset(0);
    }
    static transaction &current_transaction(){
        if(transaction *tx=currenttx::get()) return *tx;
        else throw no_transaction();
    }
    static bool has_current_transaction(){
        return currenttx::get() ? true : false;
    }

    /// \cond
private:
    static void restart_transaction(transaction &tx,mpl::true_ service){
        res->restart_transaction(*tx.rtx);
    }
    static void restart_transaction(transaction &tx,mpl::false_ service){
        if(tx.parent){
            BOOST_ASSERT(tx.parent->rtx);
            tx.rtx=in_place(res->begin_nested_transaction(*tx.parent->rtx));
        }else{
            tx.rtx=in_place(res->begin_root_transaction());
        }
    }

    static Resource *res;
    /// \endcond
};

template<class Res,bool Thr>
Res *simple_transaction_manager<Res,Thr>::res=0;


}
}



#endif
