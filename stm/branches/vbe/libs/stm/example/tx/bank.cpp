//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <list>
#include <stdlib.h>

#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace boost::stm;

class BankAccount
{
protected:
    int nb_;
    tx::int_t balance_;
public:
    BankAccount(int nb) : nb_(nb), balance_(0) {
        bind(&balance_);
    }

    void Deposit(int amount) {
        BOOST_STM_TRANSACTION(_) {
            balance_ += amount;
        } BOOST_STM_RETRY
    }
    int Withdraw(int amount) {
        BOOST_STM_TRANSACTION(_) {
            balance_ -= amount;
        } BOOST_STM_RETRY
        return amount;
    }
    int Balance() const {
        BOOST_STM_E_TRANSACTION {
            BOOST_STM_E_RETURN(balance_);
        } BOOST_STM_E_END_TRANSACTION

        return 0;
    }
    int Nb() const {
        return nb_;
    }
    std::list<base_transaction_object*>& binds() {
        return binds_;
    }
    std::list<base_transaction_object*> binds_;

    void bind(base_transaction_object* bto) {binds_.push_back(bto);}
};

typedef BankAccount account;

template <typename OSTREAM>
OSTREAM& operator<<(OSTREAM& os, account const& ) {
    return os;
}

struct Bank {
    std::vector<tx::pointer<BankAccount> > accounts;
    int overall_balance() const{
        int tmp=0;
        foreach(tx::pointer<BankAccount> const &a, this->accounts) {
            tmp+=a->Balance();
        }
        return tmp;
    }
    void  print_balance() const{
        foreach(tx::pointer<BankAccount> const &a, this->accounts){
            cerr << a->Nb()  << "=" << a->Balance() << endl;
        }
    }
};

template <typename OSTREAM>
OSTREAM& operator<<(OSTREAM& os, Bank const& ) {
    return os;
}

struct Teller {
    Teller(tx::pointer<const Bank> b)
        : bank_(b){}
    ~Teller() {}
    void operator()(){  //thread start
        thread_initializer thi;
        for(int i=10; i>0;--i)
        {
            BOOST_STM_OUTER_TRANSACTION(_) {
                int amount=random() % 1000;
                int acc1=random() % bank_->accounts.size();
                int acc2=random() % bank_->accounts.size();
                bank_->accounts[acc1]->Withdraw(amount);
                bank_->accounts[acc2]->Deposit(amount+1);
            } BOOST_STM_RETRY
            catch(...) {
                cerr << "aborted"<< endl;
            }
        }
    }
    tx::pointer<const Bank> bank_;
    static bool volatile exit;
};

bool volatile Teller::exit=false;


void create_db(tx::pointer<Bank> b, int nr_of_accounts){
    for(int c=0;c<nr_of_accounts;++c) {
        BOOST_STM_OUTER_TRANSACTION(_) {
            tx::pointer<BankAccount> acc(BOOST_STM_TX_NEW_PTR(_, BankAccount(c)));
            b->accounts.push_back(acc);
        } BOOST_STM_RETRY
    }
}

tx::pointer<BankAccount> a;
BankAccount b(1);
//tx::tx_ptr<BankAccount> a;
void account_withdraw_thr() {
    thread_initializer thi;
    BOOST_STM_OUTER_TRANSACTION(_) {
        a->Withdraw(10);
    } BOOST_STM_RETRY
}
void account_withdraw() {
    BOOST_STM_OUTER_TRANSACTION(_) {
        a->Withdraw(10);
    } BOOST_STM_RETRY
}

void account_deposit_thr() {
    thread_initializer thi;
    BOOST_STM_OUTER_TRANSACTION(_) {
        a->Deposit(10);
    } BOOST_STM_RETRY
}
void account_deposit() {
    BOOST_STM_OUTER_TRANSACTION(_) {
        a->Deposit(10);
    } BOOST_STM_RETRY
}

int test_account_0() {

    BOOST_STM_OUTER_TRANSACTION(_) {
        a=BOOST_STM_TX_NEW_PTR(_, BankAccount(2));
        a->Deposit(10);
    } BOOST_STM_RETRY

    BOOST_STM_E_TRANSACTION {
        int res = (a->Balance()==10?0:1);
        //BUG assertion "pthread_mutex_lock(&lockable)==0&&"synchro::lock<pthread_mutex_t>"" failed: file "../../../boost/synchro/pthread/mutex.hpp", line 46
        //~ BOOST_STM_TX_DELETE_PTR(_, a.value());
        BOOST_STM_E_RETURN(res);
    } BOOST_STM_E_END_TRANSACTION
    return 1;
}

int test_account_1() {

    BOOST_STM_OUTER_TRANSACTION(_) {
        a=BOOST_STM_TX_NEW_PTR(_, BankAccount(1));
    } BOOST_STM_RETRY
    thread  th1(account_withdraw_thr);

    th1.join();
    BOOST_STM_E_TRANSACTION {
        int res = (a->Balance()==-10?0:1);
        //~ BOOST_STM_TX_DELETE_PTR(_, a.value());
        BOOST_STM_E_RETURN(res);
    } BOOST_STM_E_END_TRANSACTION
    return 1;
}

int test_account_2() {

    BOOST_STM_OUTER_TRANSACTION(_) {
        a=BOOST_STM_TX_NEW_PTR(_, BankAccount(1));
    } BOOST_STM_RETRY
    thread  th1(account_withdraw_thr);
    thread  th2(account_deposit_thr);
    thread  th3(account_withdraw_thr);
    thread  th4(account_deposit_thr);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    BOOST_STM_E_TRANSACTION {
        int res = (a->Balance()==0?0:1);
        //~ BOOST_STM_TX_DELETE_PTR(_, a.value());
        BOOST_STM_E_RETURN(res);
    } BOOST_STM_E_END_TRANSACTION
    return 1;
}

bool test_bank_1() {
    string wait;

    //int nr_of_threads=10;
    int nr_of_accounts=200;

    //cin >> wait;Teller::exit=true;
    tx::object<Bank> mybank;
    create_db(address_of(mybank),nr_of_accounts);
    //cin >> wait;Teller::exit=true;

#if 0
    Teller t(mybank);
    t();
#else
    Teller t1(address_of(mybank));
    thread  th1(boost::ref(t1));
    //~ Teller t2(address_of(mybank));
    //~ thread  th2(boost::ref(t2));

    cin >> wait;Teller::exit=true;
    th1.join();
    //~ th2.join();
    BOOST_STM_OUTER_TRANSACTION(_) {
        address_of(mybank)->print_balance();
    } BOOST_STM_RETRY
#endif

    return true;
}

bool test_bank_2() {
    string wait;

    //int nr_of_threads=10;
    int nr_of_accounts=200;

    //cin >> wait;Teller::exit=true;
    tx::object<Bank> mybank;
    create_db(address_of(mybank),nr_of_accounts);
    //cin >> wait;Teller::exit=true;

#if 0
    Teller t(mybank);
    t();
#else
    Teller t1(address_of(mybank));
    thread  th1(boost::ref(t1));
    Teller t2(address_of(mybank));
    thread  th2(boost::ref(t2));

    cin >> wait;Teller::exit=true;
    th1.join();
    th2.join();
    BOOST_STM_OUTER_TRANSACTION(_) {
        address_of(mybank)->print_balance();
    } BOOST_STM_RETRY
#endif

    return true;
}


int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;
    srand(time(0));

    int res=0;
    res+=test_account_0();
    res+=test_account_1();
    //BUG assertion "pthread_mutex_lock(&lockable)==0&&"synchro::lock<pthread_mutex_t>"" failed: file "../../../boost/synchro/pthread/mutex.hpp", line 46
    res+=test_account_2();
    res+=test_bank_1();
    res+=test_bank_2();

    return res;

}
