/**********************************************************************/
/*                                                                    */
/* preliminary example program for the unit test framework            */
/*                                                                    */
/*              (C) Copyright Ullrich Koethe 2001                     */
/* Permission to copy, use, modify, sell and distribute this software */
/* is granted provided this copyright notice appears in all copies.   */
/* This software is provided "as is" without express or implied       */
/* warranty, and with no claim as to its suitability for any purpose. */
/*                                                                    */
/**********************************************************************/
 
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <boost/config.hpp>

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::getenv; using ::atoi; }
# endif

//  "account" is the user class to be tested.  In a real application,
//  it would almost always be contained in a user supplied header file.

struct account
{
    account()
    : m_amount(0.0)
    {}
    
    void deposit(double amount) { m_amount += amount; }
    void withdraw(double amount)
    {
        if(amount > m_amount)
        {
            throw std::runtime_error("You don't have that much money!");
        }
        m_amount -= amount;
    }
    double balance() const { return m_amount; }
    
    
    double m_amount;
};

//  "account_test" supplies tests of "account", in the form of member functions
//  which utilize test macros supplied by the unit test framework.  An object
//  of class account_test is constructed by the unit test framework each
//  time a member function test case is run.  See "account_test_suite" below. 
//  Because the unit test framework is what instantiates account_test,
//  a construtor and destructor could also be added if desired, and they too
//  could contain test macros. 

struct account_test
{
    account account_;  // a very simple fixture
    
    void test_init()
    {
        // different kinds of non-critical tests
        // they report the error and continue
        
        // standard assertion
        // reports "account_.balance() == 0.0" on error
        BOOST_CHECK(account_.balance() == 0.0);
        
        // customized assertion
        // reports "Initial balance should be 0, was actual_value" on error
        BOOST_CHECK_MESSAGE(account_.balance() == 0.0, 
                            "Initial balance should be 0, was " << account_.balance());
                            
        // equality assertion
        // reports "account_.balance() == 0.0 [actual_value != 0]" on error
        BOOST_CHECK_EQUAL(account_.balance(), 0.0);
        
        // equality assertion for real numbers
        // checks "-epsilon <= left - right && left - right <= epsilon"
        // reports "account_.balance() == 0.0 [actual_value != 0]" on error
        BOOST_CHECK_REAL(account_.balance(), 0.0, /* epsilon */ 0.0);
    }
    
    void test_deposit()
    {
        account_.deposit(5.5);
        BOOST_TEST(account_.balance() == 5.5);
        account_.deposit(100.0);
        
        // different kinds of critical tests
        // meaning equals the non-critical counterparts, but they throw
        BOOST_REQUIRE(account_.balance() == 105.5);
        BOOST_REQUIRE_MESSAGE(account_.balance() == 105.5, 
                            "Balance should be 105.5, was " << account_.balance());
        BOOST_REQUIRE_EQUAL(account_.balance(), 105.5);
        BOOST_REQUIRE_REAL(account_.balance(), 105.5, /* epsilon */ 0.0);
    }
    
    void test_withdraw()
    {
        account_.deposit(5.5);
        account_.withdraw(2.5);
        BOOST_TEST(account_.balance() == 3.0);
        account_.withdraw(3.0);
        BOOST_TEST(account_.balance() == 0.0);
    }
    
    void test_failure()
    {
        // test that an exception is thrown when it should be
        try
        {
            account_.withdraw(2.5); // this should throw
            
            // Unreachable code if exception was thrown
            // => Uniconditional failure if executed
            BOOST_ERROR("no exception thrown!");
        }
        catch(std::runtime_error & e) 
        {
            // Check that we received the expected exception
            BOOST_CHECK_EQUAL(std::string("You don't have that much money!"), e.what());
        }
        // other exception will be reported, though
    }  
};

//  In addition to test cases for class member functions, test cases can be
//  run on non-member functions.  Several examples follow. 

void free_test_function()
{
    // test functions can be free functions or member functions
    BOOST_TEST(2 == 1); // non-critical test => continue after failure
}

void force_division_by_zero()
{
    // unit_test.hpp can catch operating system signals
    BOOST_CHECKPOINT("About to force division by zero!");
    int i = 1, j = 0;
    i = i / j; // divide by zero
//    int * p = 0;
//    i = *p;
}

void infinite_loop()
{
    // unit_test.hpp can break infinite loops by timeout
#ifdef __unix  // don't have timeout on other platforms
    BOOST_CHECKPOINT("About to enter an infinite loop!");
    while(1);
#endif
}

//  test_suites contain the individual test cases

struct account_test_suite
: public boost::test_suite
{
    account_test_suite()
    : boost::test_suite("account_test_suite")
    {
        // add member function test cases to a test suite
        add( BOOST_TEST_CASE(&account_test::test_init));
        add( BOOST_TEST_CASE(&account_test::test_deposit));
        add( BOOST_TEST_CASE(&account_test::test_withdraw));
        add( BOOST_TEST_CASE(&account_test::test_failure));
    }
};

struct demo_test_suite
: public boost::test_suite
{
    demo_test_suite()
    : boost::test_suite("demo_test_suite")
    {
        // add free function test cases to a test suite
        add( BOOST_TEST_CASE(&free_test_function));
        add( BOOST_TEST_CASE(&force_division_by_zero));
        add( BOOST_TEST_CASE(&infinite_loop), /* timeout */ 2);
    }
};

int main(int argc, char ** argv)
{
    // set the log level (default: 1)
    //   each level also includes levels numerically greater
    // 0 ... report everything, even successful tests
    // 1 ... show which test suite is executing 
    //       and give summary for each test suite
    // 2 ... report all failures
    // 3 ... report uncaught C++ exceptions
    // 4 ... report operating system errors (signals, traps, timeouts, etc)
    // 5 ... report fatal errors (failures in destructors and memory access violation)
    // 6 ... don't report anything
    // these constants are also defined as enums in boost::log

    const char * lglv = std::getenv( "BOOST_LOGLEVEL" );
    if (lglv)
        boost::set_loglevel(std::atoi(lglv));
    if(argc > 1)
        boost::set_loglevel(std::atoi(argv[1]));
    
    // uncomment the following code to direct the output to a
    // different stream (default: std::cout)
    //   boost::set_logstream(std::cerr);
    
    // construct test suite object
    boost::test_suite test("Master");
    // add the two test suites defined above
    // (test suites may form a tree of arbitrary depth)
    test.add(new account_test_suite);
    test.add(new demo_test_suite);

    // run the tests, non-zero return upon failure
    int failed = test.run();

    if(failed)
        std::cerr << "\n*** Failures in test programm \"" << 
                argv[0] << "\"" << std::endl;

    return (failed != 0);
}
                                           