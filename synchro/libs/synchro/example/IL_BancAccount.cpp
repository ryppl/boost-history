//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#if  IL_BankAccount_BankAccount_mutex
//[IL_BankAccount_BankAccount_mutex
class BankAccount {
    boost::mutex mtx_;
    int balance_;
public:
    void Deposit(int amount) {
        mtx_.lock();
        balance_ += amount;
        mtx_.unlock();
    }
    void Withdraw(int amount) {
        mtx_.lock();
        balance_ -= amount;
        mtx_.unlock();
    }
    int GetBalance() {
        mtx_.lock();
        int b = balance_;
        mtx_.unlock();
        return balance_;
    }
};
//]
#endif
//[IL_BankAccount_BankAccount
class BankAccount {
    boost::mutex mtx_; /*< explicit mutex declaration >*/
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
    int GetBalance() {
        boost::lock_guard<boost::mutex> guard(mtx_);
        return balance_;
    }
};
//]
#ifndef BOOST_SYNCHRO_DO_NOT_COMPILE
//[IL_BankAccount_ATMWithdrawal
void ATMWithdrawal(BankAccount& acct, int sum) {
    acct.Withdraw(sum);
    /*<preemption possible>*/
    acct.Withdraw(2);
}
//]
#else
//[IL_BankAccount_ATMWithdrawal_do_not_compile
void ATMWithdrawal(BankAccount& acct, int sum) {
    boost::lock_guard<boost::mutex> guard(acct.mtx_); /*<mtx_ field is private>*/
    acct.Withdraw(sum);
    acct.Withdraw(2);
//]
#endif

int main() {
    BankAccount acct;
    ATMWithdrawal(acct, 100);
    return 0;
}

