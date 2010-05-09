//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_EXCEPTION_HEADER_HPP
#define BOOST_TRANSACT_EXCEPTION_HEADER_HPP

#include <exception>
#include <boost/assert.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/optional/optional.hpp>


namespace boost{
namespace transact{

///\brief Exception base class.
struct exception : std::exception{};

///\brief Indicates that a persistent resource recovery failed.
struct recovery_failure : transact::exception{};

///\brief Indicates that an internal operation reading from/writing to files failed.
struct io_failure : transact::exception{};

///\brief Indicates that this operation required a transaction but there was no transaction bound to this thread, or that the operation
///required an active transaction but the transaction bound to this thread was inactive.
struct no_transaction : transact::exception{};

///\brief Indicates an error with regard to connecting a resource to a transaction manager
struct resource_error : transact::exception{};

///\brief Indicates that no transaction manager was constructed.
struct no_transaction_manager : transact::exception{};

///\brief Indicates that this operation is not supported by this implementation
struct unsupported_operation : transact::exception{};

struct isolation_exception;
template<class ResMgr>
struct resource_isolation_exception;

namespace detail{

template<class TxMgr>
struct tag_types{
    typedef typename mpl::fold<
        typename TxMgr::resource_types,
        mpl::vector0<>,
        mpl::push_back<mpl::_1,mpl::first<mpl::_2> >
    >::type type;
};

}

///\brief Indicates that the operation conflicted with another transaction.
///
///\c isolation_exception is an abstract base class. The derived class
///\c resource_isolation_exception can be used to throw this exception.
struct isolation_exception : transact::exception{
    ///Rethrows the exception if the current transaction is a nested transaction but the isolation exception was caused by a parent transaction of it,
    ///or if the isolation_exception was caused independently of a transaction.
    ///\pre TxMgr::current_transaction() must be a rolled back transaction
    template<class TxMgr>
    void unwind() const{ //pseudo-virtual
        //TODO optimization: stop iteration when correct resource_isolation_exception is found
        mpl::for_each<typename detail::tag_types<TxMgr>::type>(unwinder<TxMgr>(*this));
    }
    virtual ~isolation_exception() throw(){}
protected:
    isolation_exception(){}
private:
    template<class TxMgr>
    struct unwinder{
        explicit unwinder(isolation_exception const &e) : e(e){}
        template<class Tag>
        void operator()(Tag){
            typedef typename mpl::at<typename TxMgr::resource_types,Tag>::type resource_type;
            typedef resource_isolation_exception<resource_type> der_type;
            if(der_type const *der=dynamic_cast<der_type const *>(&this->e)){
                der->template unwind<TxMgr>();
            }
        }
    private:
        isolation_exception const &e;
    };
};


///\brief Indicates that the operation conflicted with another transaction.
///
///The base class \c isolation_exception should be used to catch this exception,
///in order to catch isolation exceptions of all resource managers.
template<class ResMgr>
struct resource_isolation_exception : isolation_exception{
    ///\brief Constructs a resource_isolation_exception
    resource_isolation_exception(){}

    ///\brief Constructs a resource_isolation_exception
    ///\param res The resource manager that is throwing the exception
    ///\param retry The transaction that caused the isolation_exception and ought to be repeated.
    ///Must be a transaction on the nested transaction stack.
    explicit resource_isolation_exception(ResMgr const &res,typename ResMgr::transaction const &retry)
        : retry(std::pair<ResMgr const &,typename ResMgr::transaction const &>(res,retry)){}

    ///Throws: thread_resource_error. no_transaction if \c retry was not on the nested transaction stack or it was removed before unwind() was called.
    ///\brief Equivalent to <tt>isolation_exception::unwind<TxMgr>()</tt>
    ///\pre TxMgr::current_transaction() must be a rolled back transaction
    template<class TxMgr>
    void unwind() const{ //pseudo-virtual
        //FIXME: does not work correctly with flat nested transaction emulation. possibly make TxMgr::restart_transaction throw accordingly?
        if(this->retry){
            //TODO optimization: stop iteration when correct resource manager is found
            mpl::for_each<typename detail::tag_types<TxMgr>::type>(is_current<TxMgr>(*this->retry));
        }else throw;
    }
    virtual ~resource_isolation_exception() throw(){}
private:
    template<class TxMgr>
    struct is_current{
        explicit is_current(std::pair<ResMgr const &,typename ResMgr::transaction const &> const &retry) : retry(retry){}
        template<class Tag>
        void operator()(Tag const &tag) const{
            typedef typename mpl::at<typename TxMgr::resource_types,Tag>::type resource_type;
            return this->operator()(tag,typename is_same<resource_type,ResMgr>::type());
        }
    private:
        template<class Tag>
        void operator()(Tag const &,mpl::true_) const{
            typedef typename TxMgr::resource_iterator iterator;
            std::pair<iterator,iterator> range=TxMgr::template resources<Tag>();
            for(iterator it=range.first;it != range.second;++it){
                if(&it->second == &this->retry.first){
                    typename TxMgr::transaction *tx=TxMgr::current_transaction();
                    BOOST_ASSERT(tx);
                    if(&TxMgr::resource_transaction(*tx,it->first) != &this->retry.second) throw;
                }
            }
        }
        template<class Tag>
        void operator()(Tag,mpl::false_) const{
            return false;
        }
        std::pair<ResMgr const &,typename ResMgr::transaction const &> retry;
    };
    optional<std::pair<ResMgr const &,typename ResMgr::transaction const &> > retry;
};

}
}



#endif
