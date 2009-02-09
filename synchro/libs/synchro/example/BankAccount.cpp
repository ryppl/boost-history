//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/synchro/monitor.hpp"
#include "boost/thread.hpp"
#include <iostream>

using namespace boost::synchro;
using namespace boost;

/*<< Note that BanckAcount is not a Lockable outside the BankAccount hierarchy >>*/

//[BankAccount
class BankAccount : protected exclusive_monitor<>
{
protected:
    int balance_;
public:
    BankAccount() : balance_(0) {}
    BankAccount(const BankAccount &rhs) {
        synchronizer _(*rhs.mutex());
        balance_=rhs.balance_;
    }

    BankAccount& operator=(BankAccount &rhs)
    {
        if(&rhs == this) return *this;

        int balance=0;
        {
            synchronizer _(*rhs.mutex());
            balance=rhs.balance_;
        }
        synchronizer _(*this->mutex());
        balance_=balance;
        return *this;
    }
#if 0
    BankAccount& operator=(BankAccount &rhs)
    {
        if(&rhs == this) return *this;
        int balance=0;
        synchronize (*rhs.mutex()) balance=rhs.balance_;
        synchronize (*this->mutex()) balance_=balance;
        return *this;
    }
#endif

    void Deposit(int amount) {
        synchronizer _(*this->mutex());
        balance_ += amount;
    }
    int Withdraw(int amount) {
        synchronizer _(*this->mutex());
        balance_ -= amount;
        return amount;
    }
    int GetBalance() {
        synchronizer _(*this->mutex());
        return balance_;
    }
};
//]

#if 0
//[BankAccount_raii
class BankAccount {
    boost::mutex mtx_; /*< explicit mutex declaration >*/
    int balance_;
public:
    void Deposit(int amount) {
        boost::lock_guard<boost::mutex> guard(mtx_);
        balance_ += amount;
    }
    int Withdraw(int amount) {
        boost::lock_guard<boost::mutex> guard(mtx_);
        balance_ -= amount;
        return amount
    }
    int GetBalance() {
        boost::lock_guard<boost::mutex> guard(mtx_);
        return balance_;
    }
};
//]
#endif

//[BankAccount_ussage
class BankAccount;

BankAccount JoesAccount;

void bankAgent()
{
    for (int i =10; i>0; --i) {
        //...
        JoesAccount.Deposit(500);
        //...
    }
}

void Joe() {
    for (int i =10; i>0; --i) {
        //...
        int myPocket = JoesAccount.Withdraw(100);
        std::cout << myPocket << std::endl;
        //...
    }
}

int main() {
    //...
    boost::thread thread1(bankAgent); // start concurrent execution of bankAgent
    boost::thread thread2(Joe); // start concurrent execution of Joe
    thread1.join();
    thread2.join();
    return 0;
}
//]
