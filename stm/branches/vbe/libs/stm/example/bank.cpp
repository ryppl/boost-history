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
    int balance_;
public:
    BankAccount(int nb) : nb_(nb), balance_(0) {}

    void Deposit(int amount) {
        balance_ += amount;
    }
    int Withdraw(int amount) {
        balance_ -= amount;
        return amount;
    }
    int Balance() const {
        return balance_;
    }
    int Nb() const {
        return nb_;
    }
};

typedef BankAccount account;

struct bank{
    vector<tx_ptr<BankAccount> > accounts;
    int overall_balance() const{
        int tmp=0;
        foreach(tx_ptr<BankAccount> const &a, this->accounts){
            tmp+=a->Balance();
        }
        return tmp;
    }
    void  print_balance() const{
        foreach(tx_ptr<BankAccount> const &a, this->accounts){
            cerr << a->Nb()  << "=" << a->Balance() << endl;
        }
    }
};


struct teller {
    teller(bank* b)
        : bank_(b){
            cerr << "teller this=" <<this<< endl;
            cerr << "ptr=" <<bank_<< endl;
            cerr << "size=" <<bank_->accounts.size()<< endl;
            }
            ~teller() {
            cerr << "~this=" <<this<< endl;
                }
    void operator()(){  //thread start
        cerr << "this=" <<this<< endl;
        cerr << "start"<< endl;
        thread_initializer thi;
        cerr << "initialize_thread"<< endl;
        //while(!exit)
        for(int i=10; i>0;--i)
        {
            cerr << "c="<< i<<endl;
            atomic(_) {
                int amount=random() % 1000;
                #if 0
                tx_ptr<bank> rd_bank(bank_);
                #else
                //rd_ptr<bank> rd_bank(_,bank_);
                bank* rd_bank = bank_;
                //cerr << "rd_bank"<< endl;
                #endif
                //cerr << "ptr=" <<rd_bank<< endl;
                //cerr << "size=" <<rd_bank->accounts.size()<< endl;
                int acc1=random() % rd_bank->accounts.size();
                int acc2=random() % rd_bank->accounts.size();
                rd_bank->accounts[acc1]->Withdraw(amount);
                rd_bank->accounts[acc2]->Deposit(amount+1);
            } catch(...) {
                cerr << "aborted"<< endl;
            }
        }
            cerr << "ptr=" <<bank_<< endl;
            cerr << "size=" <<bank_->accounts.size()<< endl;
            cerr << "balance=" <<bank_->overall_balance()<< endl;
        cerr << "end"<< endl;
    }
    bank* bank_;
    static bool volatile exit;
};

bool volatile teller::exit=false;


void create_db(bank* b, int nr_of_accounts){
    //use_atomic(_)
    {
        for(int c=0;c<nr_of_accounts;++c){
    //cerr << "c=" <<c<< endl;
            tx_ptr<BankAccount> acc(make_tx_ptr<BankAccount>(c));
    //cerr << "ptr=" <<acc.ptr_<< endl;
            b->accounts.push_back(acc);
    //cerr << "size=" <<mybank->accounts.size()<< endl;
        }
    }
}

tx_ptr<BankAccount> a;
void account_withdraw_thr_basic() {
    thread_initializer thi;
    use_atomic(_) {
        a->Withdraw(10);
    }
}
void account_withdraw_thr() {
    thread_initializer thi;
    use_atomic(_) {
        make_wr_ptr(_,a)->Withdraw(10);
    }
}

void account_deposit_thr_basic() {
    thread_initializer thi;
    use_atomic(_) {
        a->Deposit(10);
    }
}
void account_deposit_thr() {
    thread_initializer thi;
    use_atomic(_) {
        make_wr_ptr(_,a)->Deposit(10);
    }
}

int test_account() {
    a=make_tx_ptr<account>(1);
    thread  th1(account_withdraw_thr);
    thread  th2(account_deposit_thr);
    thread  th3(account_withdraw_thr_basic);
    thread  th4(account_deposit_thr_basic);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    cerr << "ptr=" << a.ptr_ << " nb=" << a->Nb() << " bal=" << a->Balance() << endl;
    int res = (a->Balance()==0?0:1);
    //boost::stm::delete_ptr(a);
    return res;
}

tx_ptr<std::vector<int> > v;

void vector_int_assign_basic() {
    thread_initializer thi;
    use_atomic(_) {
        (*v)[0]+=10;
    }
}

void vector_int_assign() {
    thread_initializer thi;
    use_atomic(_) {
        wr_ptr<std::vector<int> > wrv(_,v);
        (*wrv)[0]+=10;
    }
}

int test_vector_int() {
    v=make_tx_ptr<std::vector<int> >();
    cerr << "ptr=" << v.ptr_ << endl;
    v->push_back(10);
    cerr << "ptr=" << v.ptr_ << " v[0]="  <<  v.ptr_->value[0] << endl;

    thread  th1(vector_int_assign);
    thread  th2(vector_int_assign_basic);
    thread  th3(vector_int_assign);

    th1.join();
    th2.join();
    th3.join();
    cerr << "ptr=" << v.ptr_ << " v[0]=" << (*v)[0] << endl;
    //cerr << "ptr=" << v.ptr_ << " v[0]="  <<  v.ptr_->value[0] << endl;
    //cerr << "ptr=" << v.ptr_ << " v[0]="  <<  0 << endl;
    int res = ((*v)[0]==40?0:1);

    //boost::stm::delete_ptr(v);
    return res;
}


int test_xxxx() {
    string wait;

    //int nr_of_threads=10;
    int nr_of_accounts=200;

    cerr << __LINE__ << endl;
    //cin >> wait;teller::exit=true;
    bank abank;
    bank* mybank=&abank;
    create_db(mybank,nr_of_accounts);
    cerr << __LINE__ << endl;
    //cin >> wait;teller::exit=true;
    //use_atomic(_)
    {

    //cerr << "size=" <<mybank.ptr_->value.accounts.size()<< endl;
    cerr << __LINE__ << " size=" <<mybank->accounts.size()<< endl;
    cerr << __LINE__ << " overall balance before: " << mybank->overall_balance() << endl;
    }

#if 0
    mybank->accounts[0]->Withdraw(10);
    mybank->accounts[1]->Deposit(100);
#endif
#if 0
    teller t(mybank);
    t();
    //cerr << "size=" <<mybank->accounts.size()<< endl;
    //cerr << "overall balance before: " << mybank->overall_balance() << endl;
    cerr << __LINE__ << endl;
#endif
#if 1
    teller t(mybank);
    cerr << "t=" <<&t<< endl;
    thread*  th1= new thread(boost::ref(t));
    //thread*  th2= new thread(teller(mybank));

    cin >> wait;teller::exit=true;
    cerr << __LINE__ << endl;
    th1->join();
    delete th1;
    //th2->join();
    //delete th2;
            use_atomic(_) {
    cerr << "overall balance after: " << mybank->overall_balance() << endl;
    cerr << "balances after: " <<endl;
    mybank->print_balance();
            }
#endif
#if 0


    list<shared_ptr<thread> > threads;
    for(int c=0;c<nr_of_threads;++c){
        thread *newthread=new thread(teller(mybank));
        threads.push_back(shared_ptr<thread>(newthread));
    }

    cin >> wait;teller::exit=true;
    for(int c=0;c<nr_of_threads;++c){
        shared_ptr<thread> th=threads.front();
        th->join();
        threads.pop_front();
    }

    cerr << "overall balance after: " << mybank->overall_balance() << endl;
    cerr << "balances after: " << endl;
    mybank->print_balance();
#endif

    return 0;
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;
    srand(time(0));

    int res=0;
    res+=test_account();
    //res+=test_vector_int();

    return res;

}
