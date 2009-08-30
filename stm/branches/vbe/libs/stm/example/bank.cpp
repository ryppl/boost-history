//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009. 
// (C) Copyright Vicente J. Botet Escriba 2009. 
// Distributed under the Boost
// Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
//#include <string>
#include <vector>
#include <list>
//#include <iostream>
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

struct bank{ //persistent object
    vector<tx_ptr<BankAccount> > accounts;
    int overall_balance() const{
        int tmp=0;
        foreach(BankAccount const* a,this->accounts){
            tmp+=a->Balance();
        }
        return tmp;
    }
    void  print_balance() const{
        foreach(BankAccount const* a,this->accounts){
            cerr << a->Nb()  << "=" << a->Balance() << endl;

        }
    }
};

struct teller {
    teller(tx_ptr<bank> const &b)
        : bank_(b){}
    void operator()(){  //thread start
        //thread_initializer thi;
        transaction::initialize_thread();
        //while(!exit)
        for(int i=100; i>0;--i) 
        {
            atomic(_) {
                int amount=random() % 1000;
                #if 0
                tx_ptr<bank> rd_bank(bank_);
                #else
                rd_ptr<bank> rd_bank(_,bank_);
                #endif
                int acc1=random() % rd_bank->accounts.size();
                int acc2=random() % rd_bank->accounts.size();
                rd_bank->accounts[acc1]->Withdraw(amount);
                rd_bank->accounts[acc2]->Deposit(amount);
            } catch(...) {
                cerr << "aborted"<< endl;
            }
        }
        transaction::terminate_thread();
    }
    tx_ptr<bank> const & bank_;
    static bool volatile exit;
};

bool volatile teller::exit=false;


void create_db(tx_ptr<bank> &mybank, int nr_of_accounts){
    use_atomic(_) {
        for(int c=0;c<nr_of_accounts;++c){
            tx_ptr<BankAccount> acc(make_tx_ptr<BankAccount>(c));
            mybank->accounts.push_back(acc);
        }
    }
}

   
int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    transaction::initialize_thread();
    //thread_initializer thi;
    srand(time(0));
     
    int nr_of_threads=10;
    int nr_of_accounts=200;

    tx_ptr<bank> mybank(make_tx_ptr<bank>());
    create_db(mybank,nr_of_accounts);

    cerr << "overall balance before: " << mybank->overall_balance() << endl;
    string wait; 
#if 0
    thread*  th1= new thread(teller(mybank));
    thread*  th2= new thread(teller(mybank));

    cin >> wait;teller::exit=true;
    th1->join(); 
    delete th1;
    th2->join(); 
    delete th2;
#endif
    cerr << "overall balance after: " << mybank->overall_balance() << endl;
    cerr << "balances after: " <<endl;
    mybank->print_balance();


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

    transaction::terminate_thread();
    return 0;
    
}    
    
