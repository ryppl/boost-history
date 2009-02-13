//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/lockable_adapter.hpp>
#include <boost/synchro/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>

using namespace boost::synchro;
using namespace boost;
//[IL_Rec_Lockable_BancAccount_BankAccount
class BankAccount
: public exclusive_lockable_adapter<thread_recursive_mutex>
{
/*<-*/
    int balance_;
public:
    BankAccount() {}
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


