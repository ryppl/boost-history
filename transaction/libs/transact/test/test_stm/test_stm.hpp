//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//
//      horrible STM implementation, for testing purposes

#ifndef BOOST_TRANSACT_TEST_STM_HPP
#define BOOST_TRANSACT_TEST_STM_HPP

#include <boost/mpl/set.hpp>
#include <typeinfo>
#include <map>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/assert.hpp>
#include <boost/transact/resource_manager.hpp>
#include <boost/transact/exception.hpp>

struct test_stm{
public:
    class transaction;
private:
    typedef unsigned int transaction_id;
    typedef std::pair<test_stm const *,transaction_id> instance_id;
    struct tx_construct_t{
        tx_construct_t(test_stm &rm,transaction_id id,transaction *parent)
            : rm(rm),id(id),parent(parent){}
        test_stm &rm;
        transaction_id id;
        transaction *parent;
    };
public:
    test_stm() : next_tid(1){}

    typedef boost::mpl::set<
        boost::transact::nested_transaction_service_tag
    > services;

    tx_construct_t begin_transaction(){
        boost::lock_guard<mutex_type> lock(mutex);
        return tx_construct_t(*this,this->next_tid++,0);
    }
    tx_construct_t begin_nested_transaction(transaction &tx){
        boost::lock_guard<mutex_type> lock(mutex);
        return tx_construct_t(*this,this->next_tid++,&tx);
    }
    void commit_transaction(transaction &tx);
    void rollback_transaction(transaction &tx);

    template<class T,class Tag>
    T read(){
        boost::lock_guard<mutex_type> lock(mutex);
        return object<T,Tag>::read(*this);
    }
    template<class T,class Tag>
    T read(transaction &tx);
    template<class T,class Tag>
    void write(transaction &tx,T const &t);

    template<class T>
    class instance;
    template<class T,class Tag>
    class object;

    typedef boost::mutex mutex_type;
    static mutex_type mutex;

    transaction_id next_tid;
};


template<class T>
class test_stm::instance{
public:
    instance() : value(),last_update(){}
    instance(T const &value,transaction_id last_update) : value(value),last_update(last_update){}
    T value;
    transaction_id last_update;
};

class test_stm::transaction{
public:
    explicit transaction(tx_construct_t c)
        : rm(c.rm)
        , id_(c.id)
        , parent_(c.parent)
        , active(true){}
    transaction_id id() const{ return this->id_; }
    transaction *parent() const{ return this->parent_; }

    void check_active(){
        if(!this->active) throw boost::transact::no_transaction();
    }

    template<class T,class Tag>
    void register_access(transaction_id read_tid){
        this->accesses.push_back(new access<T,Tag>(read_tid));
    }
    void check(){
        for(accesses_type::iterator it=this->accesses.begin();it != this->accesses.end();++it){
            it->check(this->rm,*this);
        }
    }
    void update(){
        for(accesses_type::iterator it=this->accesses.begin();it != this->accesses.end();++it){
            it->update(this->rm,*this);
        }
    }
    void rollback(){
        this->active=false;
    }
    ~transaction(){
        boost::lock_guard<mutex_type> lock(mutex);
        for(accesses_type::iterator it=this->accesses.begin();it != this->accesses.end();++it){
            it->clear(this->rm,*this);
        }
    }
private:
    struct access_base{
        virtual void check(test_stm const &,transaction &) const = 0;
        virtual void update(test_stm const &,transaction &) const = 0;
        virtual void clear(test_stm const &,transaction &) const = 0;
        virtual ~access_base(){}
    };

    template<class T,class Tag>
    struct access : access_base{
        explicit access(transaction_id read_tid) : read_tid(read_tid){}
        virtual void check(test_stm const &rm,transaction &tx) const{
            object<T,Tag>::check(rm,tx,this->read_tid);
        }
        virtual void update(test_stm const &rm,transaction &tx) const{
            object<T,Tag>::update(rm,tx);
        }
        virtual void clear(test_stm const &rm,transaction &tx) const{
            object<T,Tag>::clear(rm,tx);
        }
    private:
        transaction_id read_tid;
    };


    test_stm &rm;
    transaction_id id_;
    transaction *parent_;
    bool active;
    typedef boost::ptr_vector<access_base> accesses_type;
    accesses_type accesses;
};


template<class T,class Tag>
class test_stm::object{
public:
    static T read(test_stm const &rm){
        iterator it=find(rm);
        if(it == instances.end()) return T();
        else return it->second.value;
    }
    static T read(test_stm const &rm,transaction &tx){
        return read(find(rm,tx),tx);
    }
    static void write(test_stm const &rm,transaction &tx,T const &t){
        iterator it=find(rm,tx);
        if(it == instances.end() || it->first.second != tx.id()){ //instance not found or does not belong to tx
            T value=read(it,tx);
            std::pair<iterator,bool> ret=instances.insert(std::make_pair(std::make_pair(&rm,tx.id()),instance<T>(value,tx.id())));
            BOOST_ASSERT(ret.second);
            it=ret.first;
        }
        BOOST_ASSERT(it->first.second == tx.id());
        it->second.value=t;
    }
    static void check(test_stm const &rm,transaction &tx,transaction_id read_tid){
        iterator it=tx.parent() ? find(rm,*tx.parent()) : find(rm);
        if(it != instances.end()){
            if(it->second.last_update != read_tid){
                throw boost::transact::resource_isolation_exception<test_stm>(rm,tx);
            }
        }else BOOST_ASSERT(read_tid == 0);
    }
    static void update(test_stm const &rm,transaction &tx){
        iterator source=instances.find(std::make_pair(&rm,tx.id()));
        if(source != instances.end()){
            transaction_id destid=tx.parent() ? tx.parent()->id() : 0;
            instances[std::make_pair(&rm,destid)]=source->second;
        }
    }
    static void clear(test_stm const &rm,transaction &tx){
        instances.erase(std::make_pair(&rm,tx.id()));
    }
private:
    typedef std::map<instance_id,instance<T> > instances_type;
    typedef typename instances_type::iterator iterator;

    static T read(iterator it,transaction &tx){
        if(it == instances.end()){
            tx.register_access<T,Tag>(0);
            return T();
        }else{
            if(it->first.second != tx.id()){ //don`t register reads of local copies
                tx.register_access<T,Tag>(it->second.last_update);
            }
            return it->second.value;
        }
    }
    static typename instances_type::iterator find(test_stm const &rm){
        return instances.find(std::make_pair(&rm,0));
    }
    static typename instances_type::iterator find(test_stm const &rm,transaction &tx){
        typename instances_type::iterator it=instances.find(std::make_pair(&rm,tx.id()));
        if(it == instances.end()){
            if(tx.parent()) return find(rm,*tx.parent());
            else return find(rm);
        }else return it;
    }

    static instances_type instances;
};

template<class T,class Tag>
typename test_stm::object<T,Tag>::instances_type test_stm::object<T,Tag>::instances;

boost::mutex test_stm::mutex;

inline void test_stm::commit_transaction(transaction &tx){
    tx.check_active();
    boost::lock_guard<mutex_type> lock(mutex);
    tx.check();
    tx.update();
}
inline void test_stm::rollback_transaction(transaction &tx){
    tx.check_active();
    boost::lock_guard<mutex_type> lock(mutex);
    tx.rollback();
}

template<class T,class Tag>
inline T test_stm::read(transaction &tx){
    tx.check_active();
    boost::lock_guard<mutex_type> lock(mutex);
    return object<T,Tag>::read(*this,tx);
}
template<class T,class Tag>
inline void test_stm::write(transaction &tx,T const &t){
    tx.check_active();
    boost::lock_guard<mutex_type> lock(mutex);
    object<T,Tag>::write(*this,tx,t);
}


#endif
