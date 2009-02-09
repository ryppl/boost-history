#include <boost/synchro/make_lockable.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/locks.hpp>
#include <boost/synchro/lockers/strict_locker.hpp>
#include <boost/synchro/lockers/externally_locked.hpp>


using namespace boost::synchro;
using namespace boost;

class BankAccount
{
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

//[AccountManager
class AccountManager
    : public make_exclusive_lockable<boost::mutex>
{
public:
    typedef make_exclusive_lockable<boost::mutex> lockable_base_type;
    AccountManager()
        : checkingAcct_(*this)
        , savingsAcct_(*this)
    {}
    void Checking2Savings(int amount);
    void AccountManager::AMoreComplicatedChecking2Savings(int amount);
private:
    /*<-*/ bool some_condition() { return true; } /*->*/
    externally_locked_any<BankAccount, AccountManager> checkingAcct_;
    externally_locked_any<BankAccount, AccountManager> savingsAcct_;
};
//]

//[Checking2Savings
void AccountManager::Checking2Savings(int amount) {
    strict_locker<AccountManager> guard(*this);
    checkingAcct_.get(guard).Withdraw(amount);
    savingsAcct_.get(guard).Deposit(amount);
}
//]

#if DO_NOT_COMPILE
//[AMoreComplicatedChecking2Savings_DO_NOT_COMPILE
void AccountManager::AMoreComplicatedChecking2Savings(int amount) {
    unique_lock<AccountManager> guard(*this);
    if (some_condition()) {
        guard.lock();
    }
    checkingAcct_.get(guard).Withdraw(amount);
    savingsAcct_.get(guard).Deposit(amount);
    guard1.unlock();
}
//]
#elif DO_NOT_COMPILE_2
//[AMoreComplicatedChecking2Savings_DO_NOT_COMPILE2
void AccountManager::AMoreComplicatedChecking2Savings(int amount) {
    unique_lock<AccountManager> guard1(*this);
    if (some_condition()) {
        guard1.lock();
    }
    {
        strict_locker<AccountManager> guard(guard1);
        checkingAcct_.get(guard).Withdraw(amount);
        savingsAcct_.get(guard).Deposit(amount);
    }
    guard1.unlock();
}
//]
#else
//[AMoreComplicatedChecking2Savings
void AccountManager::AMoreComplicatedChecking2Savings(int amount) {
    unique_lock<AccountManager> guard1(*this);
    if (some_condition()) {
        guard1.lock();
    }
    {
        nested_strict_locker<unique_lock<AccountManager> > guard(guard1);
        checkingAcct_.get(guard).Withdraw(amount);
        savingsAcct_.get(guard).Deposit(amount);
    }
    guard1.unlock();
}
//]
#endif

int main() {
    AccountManager mgr;
    mgr.Checking2Savings(100);
    return 0;
}


