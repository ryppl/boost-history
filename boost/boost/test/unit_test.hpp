/**********************************************************************/
/*                                                                    */
/* a simple unit test framework, similar to Kent Beck's JUnit         */
/*                                                                    */
/*              (C) Copyright Ullrich Koethe 2001                     */
/* Permission to copy, use, modify, sell and distribute this software */
/* is granted provided this copyright notice appears in all copies.   */
/* This software is provided "as is" without express or implied       */
/* warranty, and with no claim as to its suitability for any purpose. */
/*                                                                    */
/**********************************************************************/
 
#ifndef BOOST_UNIT_TEST_HPP
#define BOOST_UNIT_TEST_HPP

#include <iostream>           // for cout as default log stream
#include <vector>
#include <string>
#include <stdexcept>
#include <strstream>
#include <boost/test/execution_monitor.hpp>

#define BOOST_TEST_CASE(function)  boost::create_test_case(function, #function "()")
#define BOOST_TEST_SUITE(testsuite)  ( new testsuite )

#define BOOST_CHECKPOINT(message) \
    boost::detail::checkpoint_impl(message, __FILE__, __LINE__)

#define BOOST_TEST(predicate) \
    boost::detail::test_and_continue_impl((predicate), \
    boost::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_CHECK(predicate) BOOST_TEST(predicate)

#define BOOST_CRITICAL_TEST(predicate) \
    boost::detail::test_and_throw_impl((predicate), \
    boost::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_REQUIRE(predicate) BOOST_CRITICAL_TEST(predicate)

#define BOOST_MESSAGE(message) \
    boost::detail::message_impl((message), __FILE__, __LINE__)

#define BOOST_CHECK_MESSAGE(predicate, message) \
    boost::detail::test_and_continue_impl((predicate), \
    boost::detail::wrapstrstream() << message, __FILE__, __LINE__)
    
#define BOOST_REQUIRE_MESSAGE(predicate, message) \
    boost::detail::test_and_throw_impl((predicate), \
    boost::detail::wrapstrstream() << message, __FILE__, __LINE__)
    
#define BOOST_CHECK_EQUAL(left, right) \
    boost::detail::equal_and_continue_impl((left), (right), \
    boost::detail::wrapstrstream() << #left " == " #right, __FILE__, __LINE__)
    
#define BOOST_REQUIRE_EQUAL(left, right) \
    boost::detail::equal_and_throw_impl((left), (right), \
    boost::detail::wrapstrstream() << #left " == " #right, __FILE__, __LINE__)
    
#define BOOST_CHECK_REAL(left, right, epsilon) \
    boost::detail::real_and_continue_impl((left), (right), (epsilon), \
    boost::detail::wrapstrstream() << #left " == " #right, __FILE__, __LINE__)
    
#define BOOST_REQUIRE_REAL(left, right, epsilon) \
    boost::detail::real_and_throw_impl((left), (right), (epsilon), \
    boost::detail::wrapstrstream() << #left " == " #right, __FILE__, __LINE__)
    
#define BOOST_ERROR(message) \
    boost::detail::test_and_continue_impl(false, \
    boost::detail::wrapstrstream() << message, __FILE__, __LINE__)

#define BOOST_CRITICAL_ERROR(message) \
    boost::detail::test_and_throw_impl(false, \
    boost::detail::wrapstrstream() << message, __FILE__, __LINE__)

#define BOOST_FAIL(message) BOOST_CRITICAL_ERROR(message)

namespace boost
{

  namespace log
  {
    //  each reporting level includes all subsequent higher reporting levels
    enum report_level {
        report_successful_tests = 0,
        report_test_suites = 1,
        report_all_errors = 2,           // reported by unit test macros
        report_cpp_exception_errors = 3, // uncaught C++ exceptions
        report_system_errors = 4,        // including timeouts, signals, traps
        report_fatal_errors = 5,         // including unit test macros or
                                         // fatal system errors
        report_nothing = 6
    };
  }

  class test_suite;

  namespace detail
  {
    struct null_streambuf
    : public std::streambuf
    {};
    
    inline std::string & exception_checkpoint()
    {
        static std::string test_checkpoint_;
        return test_checkpoint_;
    }

    inline std::ostream *& logstream_impl()
    {
        static std::ostream * logstream = &std::cout;
        return logstream;
    }

    inline log::report_level & loglevel_impl()
    {
        static log::report_level loglevel = log::report_test_suites;
        return loglevel;
    }
  } // namespace detail
  
  inline std::ostream & set_logstream(std::ostream & l)
  {
     std::ostream * old = detail::logstream_impl();
     detail::logstream_impl() = &l;
     return *old;
  }

  inline log::report_level set_loglevel(int l)
  {
     log::report_level old = detail::loglevel_impl();
     detail::loglevel_impl() = static_cast<log::report_level>(l);
     return old;
  }

  inline std::ostream & logstream(log::report_level loglevel)
  {
     static std::ostream null_stream(new detail::null_streambuf);
     
     if(loglevel >= detail::loglevel_impl())
        return *detail::logstream_impl();
     else
        return null_stream;
  }
  
  namespace detail
  {
    class test_monitor
    {
      public:
        static int *& passed()
        {
            static int initial = 0;
            static int * p = &initial;
            return p;
        }
        
        static int *& failed()
        {
            static int initial = 0;
            static int * f = &initial;
            return f;
        }
        
        static std::string & prefix_message()
        {
            static std::string m;
            return m;
        }
        
        static void reset(int & f, int & p)
        {
            f = 0;
            p = 0;
            failed() = &f;
            passed() = &p;
            prefix_message() = "";
        }
        
        static void inc_failed_assertions()
        {
            ++(*failed());
        }
        
        static void inc_passed_assertions()
        {
            ++(*passed());
        }
    };

//  Internal error levels.
   
    enum error_level { 
        test_failed = 1,
        test_ok = 0,
        constructor_error = -1, 
        unexpected_exception = -2, 
        os_exception = -3, 
        os_timeout = -4, 
        fatal_error = -5,  // includes both system and user
        destructor_error = -6
    };

    inline bool critical_error(int i)
    { return i <= fatal_error; }

    inline bool unexpected_error(int i)
    { return i < 0; }

struct wrapstrstream
{
    mutable std::strstream buf;
    
    template <class T>
    wrapstrstream & operator<<(T t)
    {
        buf << t;
        return *this;
    }
    
    char const * str() const
    {
        buf << char();
        return buf.str();
    }
};

struct unit_test_failed 
: public std::exception
{
    unit_test_failed(char const * message)
    : what_(message)
    {}
    
    virtual const char * what() const
    {
        return what_.c_str();
    }
    
    std::string what_;
};

inline void 
checkpoint_impl(const char * message, const char * file, int line)
{
    detail::wrapstrstream buf;
    buf << message << " (" << file <<":" << line << ")";
    exception_checkpoint() = buf.str();
}

inline void 
message_impl(const char * message, const char * file, int line)
{
    boost::logstream(log::report_test_suites) << // Borland 5.5.1 requires boost::
            message << " (" << file <<":" << line << ")" << std::endl;
}

inline bool  // return true if error detected 
test_and_continue_impl(bool predicate, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_all_errors)
{
    char const * m = message.str();
    
    checkpoint_impl(m, file, line);
    if(!predicate)
    { 
        test_monitor::inc_failed_assertions();
        
        boost::logstream(loglevel) << // Borland 5.5.1 requires boost::
                test_monitor::prefix_message() << 
                " Test failed: " << m <<
                " (" << file <<":" << line << ")" << std::endl;
                            
        return true;
    }
    else
    {
        test_monitor::inc_passed_assertions();
        boost::logstream(log::report_successful_tests) // Borland 5.5.1 requires boost:: here
                << "Test passed: " << m <<
                " (" << file <<":" << line << ")" << std::endl;
        return false;
    }
}

template <class Left, class Right>
inline void 
equal_and_continue_impl(Left left, Right right, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_all_errors)
{
    bool predicate = left == right;
    if(!predicate)
        const_cast<wrapstrstream&>(message) << " [" << left << " != " << right << "]";
    test_and_continue_impl(predicate, message, file, line, loglevel); 
}

inline void 
real_and_continue_impl(double left, double right, double epsilon, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_all_errors)
{
    double diff = left - right;
    bool predicate = -epsilon <= diff && diff <= epsilon;
    if(!predicate)
        const_cast<wrapstrstream&>(message) << " [" << left << " != " << right << "]";
    test_and_continue_impl(predicate, message, file, line, loglevel); 
}

inline void 
test_and_throw_impl(bool predicate, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_fatal_errors)
{
    if(test_and_continue_impl(predicate, message, file, line, loglevel))
    {
        // test_amd_continue_impl() has already issued the error msg, so it's unclear
        // why a second message is constructed only be be discarded when caught (BGD)
        detail::wrapstrstream buf;
        buf << " Test failed: " << 
            message.str() << " (" << file <<":" << line << ")";
        throw unit_test_failed(buf.str());       
    }
}

template <class Left, class Right>
inline void 
equal_and_throw_impl(Left left, Right right, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_fatal_errors)
{
    bool predicate = left == right;
    if(!predicate)
        const_cast<wrapstrstream&>(message) << " [" << left << " != " << right << "]";
    test_and_throw_impl(predicate, message, file, line, loglevel); 
}

inline void 
real_and_throw_impl(double left, double right, double epsilon, wrapstrstream const & message, 
                       const char * file, int line,
                       log::report_level loglevel = log::report_fatal_errors)
{
    double diff = left - right;
    bool predicate = -epsilon <= diff && diff <= epsilon;
    if(!predicate)
        const_cast<wrapstrstream&>(message) << " [" << left << " != " << right << "]";
    test_and_throw_impl(predicate, message, file, line, loglevel); 
}

//  As an aid to understanding the relationship between functions, the call stack
//  is shown for the unit_test_example.cpp test program, at point of entry
//  into the account_test::test_deposit() member function:
//
//    main(int 1, char * * 0x002f1070)
//    boost::test_suite::run()
//    boost::test_suite::run()
//    boost::detail::class_test_case_base::run()
//    boost::execution_monitor::execute(int 0)
//    boost::detail::catch_signals(boost::execution_monitor & {...}, int 0)
//    boost::detail::unit_test_functor::function()
//    boost::detail::class_test_case<account_test>::do_run()
//    account_test::test_deposit()

class test_case
{
  protected:
  
    friend class boost::test_suite;
    
    test_case(char const * name = "Unnamed")
    : name_(name), timeout(0),
      failed_assertions(0), passed_assertions(0)
    {}

    virtual int run() = 0;
    // Returns: if >= 0, value is the number of tests failed, possibly 0
    //          if < 0, a fatal error was detected, value unspecified  
    
    virtual void do_init() {}
    virtual void do_run() {}
    virtual void do_destroy() {}
    
    virtual int reset()
    {
        exception_checkpoint() = "";
        test_monitor::reset(failed_assertions, passed_assertions);
        return 0;
    }
    

    virtual char const * name() { return name_.c_str(); } 
    virtual int size() const { return 1; }

    std::string name_;
    int timeout;
    int failed_assertions, passed_assertions;
};

class unit_test_functor : public execution_monitor
{
    typedef void (test_case::*test_function)();
    test_case *   test_case_;
    test_function test_function_;
  public:   
    unit_test_functor(test_case * tc, test_function tf)
      : test_case_(tc), test_function_(tf) {}
    
    error_level execute_and_translate(int timeout)
    {
      try
      {  
        execute(timeout);
      }

      catch ( const execution_exception & exex )
      {
        log::report_level level = log::report_fatal_errors;
        if (exex.code() <= execution_exception::timeout_error)
          { level = log::report_system_errors; }
        if (exex.code() <= execution_exception::cpp_exception_error)
          { level = log::report_cpp_exception_errors; }

        if (exception_checkpoint().size() > 0)
        {
            boost::logstream(level) << "\nCheckpoint: "
                                    << exception_checkpoint() << std::endl;
        }
        boost::logstream(level)
            << test_monitor::prefix_message()
            << " Error: " << exex.what() << std::endl;
               
        // translate execution_exception::error_code to detail::error_level
        switch (exex.code())
        {
          case execution_exception::no_error:             return test_ok;
          case execution_exception::cpp_exception_error:  return unexpected_exception;
          case execution_exception::system_error:         return os_exception;
          case execution_exception::timeout_error:        return os_timeout;
          case execution_exception::user_fatal_error:
          case execution_exception::system_fatal_error:   return fatal_error;
          default:                                        return unexpected_exception;
        }
      }
      return test_ok;
    }

    int function()
    {
        try
        {
            (test_case_->*test_function_)();
        }
        catch(unit_test_failed & /*e*/) // e not used; error already reported
        {
          // nothing to do
        }
        return 0;
    }

}; // unit_test_functor

} // namespace detail

class test_suite
: public detail::test_case
{
  public:
    test_suite(char const * name = "TopLevel")
    : detail::test_case(name),
      size_(0)
    {}
    
    virtual ~test_suite()
    {
        for(vec_type_::size_type i=0; i != testcases_.size(); ++i) 
            delete testcases_[i];        
    }
    
    virtual void add(detail::test_case * t, int timeout = 0)
    {
        t->timeout = timeout;
        testcases_.push_back(t);
        size_ += t->size();
    }
    
    virtual int run()
    {
        reset();
        int failed = 0;
        logstream(log::report_test_suites) << 
                "Entering test suite \"" << name() << "\"" << std::endl;
        
        for(int i=0; i != testcases_.size(); ++i) 
        {
            int result = testcases_[i]->run();
            
            //  if result >= 0, it is the number of tests failed, possibly 0,
            //  else result is detail::error_level  

            if(detail::critical_error(result))
            {
                logstream(log::report_fatal_errors) << 
                    "\nFatal error - aborting test suite " << name() << "." << std::endl;
                return result;
            }
            else if(result < 0)
                failed++;
            else if(result > 0)
                failed += result;
            else if(testcases_[i]->failed_assertions != 0)
                failed++;
            
            passed_assertions += testcases_[i]->passed_assertions;
            failed_assertions += testcases_[i]->failed_assertions;
        }
        
        if(failed) 
        {
            logstream(log::report_test_suites) << 
                    "\nFailed tests in test suite \"" << name() << "\" (" <<
                    failed << " of " << size() << " test cases, " <<
                    failed_assertions << " of " << (failed_assertions + passed_assertions) <<
                    " assertions)" << std::endl;
        }
        else 
        {
            logstream(log::report_test_suites) << 
                    "All tests passed in test suite \"" << name() << "\" (" <<
                    size() << " test cases, " << passed_assertions << " assertions)" << std::endl;
        }
        
        logstream(log::report_test_suites) << 
                "Leaving test suite \"" << name() << "\"" << std::endl;
        
        return failed;
    }
    
  private:
    
    virtual int size() const { return size_; }
    
    typedef std::vector<detail::test_case *> vec_type_;
    vec_type_ testcases_;
    int size_;
};

namespace detail {

class class_test_case_base
: public test_case
{
    struct lower_loglevel
    {
        log::report_level old;
        
        lower_loglevel()
        : old(boost::set_loglevel(log::report_all_errors))
        {
          if (old < log::report_all_errors) // bug fix: lowering might have raised level!
            { boost::set_loglevel(old); }  // Borland 5.5.1 requires boost::
        }
        
        ~lower_loglevel()
        { boost::set_loglevel(old); }  // Borland 5.5.1 requires boost::
    };
    
  protected:
    
    class_test_case_base(char const * name)
    : test_case(name)
    {}
    
    virtual void do_init() = 0;
    virtual void do_run() = 0;
    virtual void do_destroy() = 0;
    virtual bool may_execute() const = 0;
    
    int reset()
    {
        exception_checkpoint() = "";
        test_monitor::reset(failed_assertions, passed_assertions);
        
        std::string prefix("\nFailure in initialization of \"");
        prefix += name(); 
        prefix += "\":\n"; 
        test_monitor::prefix_message() = prefix;
        if(!may_execute())
        {
            boost::logstream(log::report_cpp_exception_errors) << prefix << 
                "Test case failed to clean up after previous run." << std::endl;
            return constructor_error;
        }   
        else
        {
            unit_test_functor ut(this, &test_case::do_init);
            return ut.execute_and_translate( timeout );
        }
    }
    
    virtual int run()
    {
        int result = reset();
        if(result != test_ok)
            return result;
            
        std::string prefix("\nFailure in \"");
        prefix += name(); 
        prefix += "\":\n"; 
        test_monitor::prefix_message() = prefix;
        
        unit_test_functor ut(this, &test_case::do_run);
        result = ut.execute_and_translate( timeout );
        
        if(critical_error(result))
            return result;
            
        int destruction_result = destroy();
        
//        return (destruction_result != test_ok) ? 
//                destructor_error : 
//                result;
        return destruction_result;
    }
    
    virtual int destroy()
    {
        std::string prefix("\nFailure in destruction of \"");
        prefix += name(); 
        prefix += "\":\n"; 
        test_monitor::prefix_message() = prefix;
        
//        lower_loglevel l; // report all errors while executing destructor
        
        unit_test_functor ut(this, &test_case::do_destroy);
        return ut.execute_and_translate( timeout );
    }
};
    
template <class TESTCASE>
class class_test_case
: public class_test_case_base
{
  public:
    
    class_test_case(void (TESTCASE::*fct)(), char const * name)
    : class_test_case_base(name),
      fct_(fct),
      testcase_(0)
    {}
    
    virtual ~class_test_case()
    {
        delete testcase_;
    }
    
  private:
    
    virtual bool may_execute() const
        { return testcase_ == 0; }
    
    virtual void do_init()
    {
        testcase_ = new TESTCASE;
    }
    
    virtual void do_run()
    {
        if(testcase_ != 0) 
            (testcase_->*fct_)();
    }
    
    virtual void do_destroy()
    {
        delete testcase_;
        testcase_ = 0;
    }
    
    void (TESTCASE::*fct_)();
    TESTCASE * testcase_;
};
    
class function_test_case
: public test_case
{
  public:
    
    function_test_case(void (*fct)(), char const * name)
    : test_case(name),
      fct_(fct)
    {}
    
    virtual int run()
    {
        reset();
        
        std::string prefix("\nFailure in \"");
        prefix += name(); 
        prefix += "\":\n"; 
        test_monitor::prefix_message() = prefix;
        
        unit_test_functor ut(this, &test_case::do_run);
        return ut.execute_and_translate( timeout );
    }
    
  private:
    
    virtual void do_run()
    {
        (*fct_)();
    }
    
    void (*fct_)();
};
    
} // namespace detail

template <class TESTCASE>
inline 
detail::test_case * 
create_test_case(void (TESTCASE::*fct)(), char const * name)
{
    if(*name == '&') ++name;
    return new detail::class_test_case<TESTCASE>(fct, name);
};

inline 
detail::test_case * 
create_test_case(void (*fct)(), char const * name)
{
    if(*name == '&') ++name;
    return new detail::function_test_case(fct, name);
};

} // namespace boost

#endif /* BOOST_UNIT_TEST_HPP */
