#include <boost/synchro/make_lockable.hpp>
#include <boost/synchro/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>

using namespace boost::synchro;
using namespace boost;
//[IL_Rec_Lockable_BancAccount_BankAccount
class BankAccount
: public make_exclusive_lockable<boost::recursive_mutex>
{
/*<-*/
    int balance_;
public:
    void Deposit(int amount) {
        lock_guard<BankAccount> guard(*this);
        balance_ += amount;
    }
    void Withdraw(int amount) {
        lock_guard<BankAccount> guard(*this);
        balance_ -= amount;
    }
    int GetBalance() {
        lock_guard<BankAccount> guard(*this);
        return balance_;
    }
/*->*/
    // ...
};
//]

void ATMWithdrawal(BankAccount& acct, int sum) {
    lock_guard<BankAccount> guard(acct);
    acct.Withdraw(sum);
    acct.Withdraw(2);
}

int main() {
    BankAccount acct;
    ATMWithdrawal(acct, 100);
    return 0;
}


