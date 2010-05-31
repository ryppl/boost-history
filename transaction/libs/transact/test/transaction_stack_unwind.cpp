//          Copyright Stefan Strasser 2010.
//      Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifdef _MSC_VER
#  pragma warning (disable : 4535) // calling _set_se_translator() requires /EHa
#endif

#include <boost/transact/simple_transaction_manager.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/test/minimal.hpp>

using namespace boost;
using namespace transact;

struct my_rm{
    typedef int transaction;
    typedef mpl::vector<nested_transaction_service_tag> services;
    transaction begin_transaction(){ return 0; }
    transaction begin_nested_transaction(transaction){ return 0; }
    void commit_transaction(transaction){}
    void rollback_transaction(transaction){}
};


typedef simple_transaction_manager<my_rm> my_tm;
#define BOOST_TRANSACT_CONFIGURATION my_tm

#include <boost/transact/language.hpp>


int test(int contextnr){
    my_tm::transaction *txs[5];
    begin_transaction{
        txs[0]=my_tm::current_transaction();
        begin_transaction{
            txs[1]=my_tm::current_transaction();
            begin_transaction{
                txs[2]=my_tm::current_transaction();
                begin_transaction{
                    txs[3]=my_tm::current_transaction();
                    begin_transaction{
                        txs[4]=my_tm::current_transaction();
                        my_rm::transaction &rtx=my_tm::resource_transaction(*txs[contextnr]);
                        throw resource_isolation_exception<my_rm>(my_tm::resource(),rtx);
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
    return -1;
}


int test_main(int, char *[]){
    my_rm rm;
    my_tm::connect_resource(rm);
    for(int c=0;c<5;++c){
        BOOST_REQUIRE( test(c) == c );
    }
    return 0;
}
