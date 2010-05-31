//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TRANSACT_TEST_STM_OBJECT_HPP
#define BOOST_TRANSACT_TEST_STM_OBJECT_HPP

#include <boost/transact/default_tag.hpp>

template<class T,class Tag,class TxMgr,class ResourceTag=boost::transact::default_tag>
class basic_object{
public:
    explicit basic_object(ResourceTag const &tag=ResourceTag()) : tag(tag){}
    operator T() const{
        if(typename TxMgr::transaction *tx=TxMgr::current_transaction()){
            return TxMgr::resource(this->tag).template read<T,Tag>(TxMgr::resource_transaction(*tx,this->tag));
        }else return TxMgr::resource(this->tag).template read<T,Tag>();
    }
    basic_object &operator=(T const &t){
        typename TxMgr::transaction *tx=TxMgr::current_transaction();
        if(!tx) throw boost::transact::no_transaction();
        TxMgr::resource(this->tag).template write<T,Tag>(TxMgr::resource_transaction(*tx,this->tag),t);
        return *this;
    }
    template<class T2>
    basic_object &operator+=(T2 const &t){
        return *this = *this + t;
    }
    template<class T2>
    basic_object &operator-=(T2 const &t){
        return *this = *this - t;
    }
private:
    ResourceTag tag;
};

#include <boost/transact/transaction_manager.hpp>

template<class T,class Tag,class ResourceTag=boost::transact::default_tag>
class object : public basic_object<T,Tag,boost::transact::transaction_manager,ResourceTag>{
    typedef basic_object<T,Tag,boost::transact::transaction_manager,ResourceTag> base_type;
public:
    object &operator=(T const &t){
        base_type::operator=(t);
        return *this;
    }
};

#endif


