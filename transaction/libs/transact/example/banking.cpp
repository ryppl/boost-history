//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <libs/transact/test/test_stm/default.hpp>
#include <boost/transact/language.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <iostream>
#include <boost/thread.hpp>

using namespace boost;
using namespace transact;
using namespace std;

//test_stm only supports static variables, create "Accounts" of them:
template<std::size_t N>
struct account_tag{};

template<std::size_t Accounts>
class bank : bank<Accounts - 1>{
    typedef bank<Accounts - 1> base_type;
    static std::size_t const N = Accounts - 1;
public:
    std::size_t size() const{ return Accounts; }
    void deposit(std::size_t accountnr,int amount){
        if(accountnr == N) this->account+=amount;
        else base_type::deposit(accountnr,amount);
    }
    void withdraw(std::size_t accountnr,int amount){
        if(accountnr == N) this->account-=amount;
        else base_type::withdraw(accountnr,amount);
    }
    int overall_balance() const{
        return this->account + base_type::overall_balance();
    }
private:
    object<int,account_tag<N> > account;
};

template<>
struct bank<0>{
    void deposit(std::size_t,int){
        BOOST_ASSERT(false);
    }
    void withdraw(std::size_t,int){
        BOOST_ASSERT(false);
    }
    int overall_balance() const{
        return 0;
    }
};

bank<100> mybank;

struct teller{
	teller()
		: transactions(0)
		, isolations(0){
		thread(boost::bind(&teller::run,this)).swap(this->thr);
	}

	void run(){
    	while(!this_thread::interruption_requested()){
			int amount=rand() % 1000;
			int acc1=rand() % mybank.size();
			int acc2=rand() % mybank.size();

			begin_transaction{
                mybank.withdraw(acc1,amount);
                mybank.deposit(acc2,amount);
			}retry{
				++isolations;
			}end_retry;
            ++transactions;
		}
	}
	int tellernr;
	unsigned int transactions;
	unsigned int isolations;
	thread thr;
};


int main(){
    test_stm rm;
    transaction_manager::connect_resource(rm);

    static unsigned int const nr_of_threads=10;

	cerr << "overall balance before: " << mybank.overall_balance() << endl;

    typedef ptr_vector<teller> tellers_type;
	tellers_type tellers;
	for(int c=0;c<nr_of_threads;++c){
		tellers.push_back(new teller);
	}
    this_thread::sleep(posix_time::seconds(5));
    
	unsigned int transactions=0,isolations=0;
    
    for(tellers_type::iterator it=tellers.begin();it != tellers.end();++it){
		it->thr.interrupt();
		it->thr.join();
		transactions+=it->transactions;
		isolations+=it->isolations;
	}
	cerr << "overall balance after: " << mybank.overall_balance() << endl;
	cerr << transactions << " transactions, " << float(isolations)/transactions*100 << "% isolation exceptions" << endl;
}


