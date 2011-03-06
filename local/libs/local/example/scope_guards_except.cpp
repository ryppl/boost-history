
#include <boost/local/exit.hpp>
#include <iostream>
#include <stdexcept>

struct exception_error_interface {
    virtual operator bool() const = 0;
    virtual void throw_if() const = 0;
};

template<typename E>
struct exception_error: exception_error_interface {
    template<typename A0>
    explicit exception_error(A0 arg0): ex_(E(arg0)) {}
    template<typename A0, typename A1>
    exception_error(A0 arg0, A1 arg1): ex_(E(arg0, arg1)) {}
    // Add more constructors for more parameters if needed...

    operator bool() const { return true; }
    void throw_if() const { throw ex_; }

private:
    E ex_; // I need the exception type `E` here so I can throw it later.
};

template<>
struct exception_error<void> { // Use `void` for no error (cannot throw void).
    exception_error(): err_() {} // No error by default.

    template<typename E> // Construct with some error.
    /* implicit */ exception_error(exception_error<E> const& err): err_(&err) {}
    template<typename E> // Set some error.
    exception_error& operator=(exception_error<E> const& err)
        { err_ = &err; return *this; }

    operator bool() const { return err_ && bool(*err_); }
    // Select proper type to throw via polymorphism.
    void throw_if() const { if (err_) err_->throw_if(); }

private:
    exception_error_interface const* err_;
};

void f() {
    exception_error<void> error; // No error (i.e., `void`) to start with.
    // This scope exit is special -- it's used to throw on exit if error.
    BOOST_LOCAL_EXIT( (const bind& error) ) {
        std::cout << "throwing if error" << std::endl;
        error.throw_if(); // Throws if error set not to `exception_error<void>`.
    } BOOST_LOCAL_EXIT_END

    BOOST_LOCAL_EXIT( (void) ) { // Smae as D's `scope(exit)`.
        std::cout << "exit" << std::endl;
    } BOOST_LOCAL_EXIT_END

    BOOST_LOCAL_EXIT( (const bind& error) ) { // Same as D's `scope(success)`.
        if (!error) std::cout << "success" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    BOOST_LOCAL_EXIT( (const bind& error) ) { // Same as D's `scope(failure)`.
        if (error) std::cout << "failure" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    // Cannot use `throw` otherwise scope exits are not executed. Set error
    // and exit using `return` instead (the 1st scope exit will throw).
    error = exception_error<std::runtime_error>("some error"); return;
    error = exception_error<int>(-1); return;
}

int main() {
    f();
    return 0;
}

