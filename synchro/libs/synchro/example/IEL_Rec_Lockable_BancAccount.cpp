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
#include <boost/synchro/lockers/strict_locker.hpp>

using namespace boost::synchro;
using namespace boost;

class BankAccount_TN {
    int balance_;
public:
    void Deposit(int amount) {
        balance_ += amount;
    }
    void Withdraw(int amount) {
        balance_ -= amount;
    }
    int GetBalance() {
        return balance_;
    }
};

class BankAccount
: protected BankAccount_TN
, public exclusive_lockable_adapter<thread_recursive_mutex>
{
//        int balance_;
    void throw_if_not_owned(strict_locker<BankAccount>&locker) {
        if (locker.mutex() != this)
            throw "Locking Error: Wrong Object Locked";
    }

public:
    typedef exclusive_lockable_adapter<boost::recursive_mutex> lockable_base_type;

    BankAccount() {}
    void Deposit(int amount) {
        lock_guard<BankAccount> guard(*this);
//        lock_guard<boost::recursive_mutex> guard(*this->mutex());
        BankAccount_TN::Deposit(amount);
    }
    void Deposit(int amount, strict_locker<BankAccount>&locker) {
        throw_if_not_owned(locker);
        BankAccount_TN::Deposit(amount);
    }
    void Withdraw(int amount) {
        lock_guard<BankAccount> guard(*this);
//        lock_guard<boost::recursive_mutex> guard(*this->mutex());
        BankAccount_TN::Withdraw(amount);
    }
    void Withdraw(int amount, strict_locker<BankAccount>&locker) {
        throw_if_not_owned(locker);
        BankAccount_TN::Withdraw(amount);
    }
    int GetBalance() {
        lock_guard<BankAccount> guard(*this);
//        lock_guard<boost::recursive_mutex> guard(*this->mutex());
        return BankAccount_TN::GetBalance();
    }
    int GetBalance(strict_locker<BankAccount>&locker) {
        throw_if_not_owned(locker);
        return BankAccount_TN::GetBalance();
    }
};

void ATMWithdrawal(BankAccount& acct, int sum) {
    strict_locker<BankAccount> guard(acct);
    acct.Withdraw(sum, guard);
    acct.Withdraw(2, guard);
}

int main() {
    BankAccount acct;
    ATMWithdrawal(acct, 100);
    return 0;
}


