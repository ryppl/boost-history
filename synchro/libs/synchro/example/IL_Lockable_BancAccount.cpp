#include <boost/synchro/make_lockable.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/thread/locks.hpp>


using namespace boost::synchro;
#if 0
//[IL_Lockable_BancAccount_BankAccount_explicit
class BankAccount {
    boost::mutex mtx_;
    int balance_;
public:
    void Deposit(int amount) {
        boost::lock_guard<boost::mutex> guard(mtx_);
        balance_ += amount;
    }
    void Withdraw(int amount) {
        boost::lock_guard<boost::mutex> guard(mtx_);
        balance_ -= amount;
    }
    void lock() {
        mtx_.lock();
    }
    void unlock() {
        mtx_.unlock();
    }
};
//]
#else
//[IL_Lockable_BancAccount_BankAccount_inherit
class BankAccount
: public make_exclusive_lockable<boost::mutex>
{
    int balance_;
public:
    void Deposit(int amount) {
        boost::lock_guard<BankAccount> guard(*this);
//        boost::lock_guard<boost::mutex> guard(*this->mutex());
        balance_ += amount;
    }
    void Withdraw(int amount) {
        boost::lock_guard<BankAccount> guard(*this);
//        boost::lock_guard<boost::mutex> guard(*this->mutex());
        balance_ -= amount;
    }
    int GetBalance() {
        boost::lock_guard<BankAccount> guard(*this);
//        boost::lock_guard<boost::mutex> guard(*this->mutex());
        return balance_;
    }
};
//]
#endif
//[IL_Lockable_BancAccount_ATMWithdrawal
void ATMWithdrawal(BankAccount& acct, int sum) {
//    boost::lock_guard<boost::mutex> guard(*acct.mutex());
    boost::lock_guard<BankAccount> guard(acct);
    acct.Withdraw(sum);
    acct.Withdraw(2);
}
//]

int main() {
    BankAccount acct;
    ATMWithdrawal(acct, 100);
    return 0;
}

