#include <boost/transact/simple_transaction_manager.hpp>
#include <boost/mpl/empty_sequence.hpp>
#include <boost/assert.hpp>

using namespace boost;
using namespace transact;

struct my_rm{
    typedef int transaction;
    typedef mpl::empty_sequence services;
    struct tag{};
    transaction begin_root_transaction(){ return 0; }
    transaction begin_nested_transaction(transaction){ return 0; }
    void commit_transaction(transaction){}
    bool finish_transaction(transaction){ return false; }
    void rollback_transaction(transaction){}
};


typedef simple_transaction_manager<my_rm> my_tm;
#define BOOST_TRANSACT_CONFIGURATION my_tm

#include <boost/transact/language.hpp>


int test(int contextnr){
    my_tm::transaction *txs[5];
    int retried=-1;
    begin_transaction{
        txs[0]=&my_tm::current_transaction();
        begin_transaction{
            txs[1]=&my_tm::current_transaction();
            begin_transaction{
                txs[2]=&my_tm::current_transaction();
                begin_transaction{
                    txs[3]=&my_tm::current_transaction();
                    begin_transaction{
                        txs[4]=&my_tm::current_transaction();
                        my_rm::transaction &rtx=my_tm::resource_transaction(*txs[contextnr]);
                        throw resource_isolation_exception<my_rm>(rtx);
                    }retry{
                        return 4;
                    }end_retry;
                }retry{
                    return 3;
                }end_retry;
            }retry{
                return 2;
            }end_retry;
        }retry{
            return 1;
        }end_retry;
    }retry{
        return 0;
    }end_retry;
}


int main(){
    my_rm rm;
    my_tm::connect_resource(rm);
    for(int c=0;c<5;++c){
        BOOST_ASSERT( test(c) == c );
    }
}
