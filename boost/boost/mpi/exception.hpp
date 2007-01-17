// Copyright (C) 2005-2006 Douglas Gregor <doug.gregor -at- gmail.com>.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file exception.hpp
 *
 *  This header provides exception classes that report MPI errors to
 *  the user and macros that translate MPI error codes into Boost.MPI
 *  exceptions.
 */
#ifndef BOOST_MPI_EXCEPTION_HPP
#define BOOST_MPI_EXCEPTION_HPP

#include <mpi.h>
#include <exception>
#include <boost/config.hpp>
#include <boost/throw_exception.hpp>

namespace boost { namespace mpi {

/** @brief Catch-all exception class for MPI errors.
 *
 * Instances of this class will be thrown when an MPI error
 * occurs. MPI failures that trigger these exceptions may or may not
 * be recoverable, depending on the underlying MPI
 * implementation. Consult the documentation for your MPI
 * implementation to determine the effect of MPI errors.
 */
class exception : public std::exception
{
 public:
  /**
   * Build a new @c exception exception.
   *
   *   @param routine The MPI routine in which the error
   *   occurred. This should be a pointer to a string constant: it
   *   will not be copied.
   *
   *   @param result_code The result code returned from the MPI
   *   routine that aborted with an error.
   */
  exception(const char* routine, int result_code)
    : routine_(routine), result_code_(result_code) { }

  /**
   * A description of the error that occured. At present, this refers
   * only to the name of the MPI routine that failed.
   */
  virtual const char * what () const throw ()
  {
    return routine_;
  }

  /** Retrieve the name of the MPI routine that reported the error. */
  const char* routine() const { return routine_; }

  /**
   * Retrieve the result code returned from the MPI routine that
   * reported the error.
   */
  int result_code() const { return result_code_; }

 protected:
  /// The MPI routine that triggered the error
  const char* routine_;

  /// The failed result code reported by the MPI implementation.
  int result_code_;
};

/**
 * Call the MPI routine MPIFunc with arguments Args (surrounded by
 * parentheses). If the result is not MPI_SUCCESS, use
 * boost::throw_exception to throw an exception or abort, depending on
 * BOOST_NO_EXCEPTIONS.
 */
#define BOOST_MPI_CHECK_RESULT( MPIFunc, Args )                         \
 {                                                                      \
   int _check_result = MPIFunc Args;                                    \
   if (_check_result != MPI_SUCCESS)                                    \
     boost::throw_exception(boost::mpi::exception(#MPIFunc,   \
                                                             _check_result)); \
 }

} } // end namespace boost::mpi

#endif // BOOST_MPI_EXCEPTION_HPP
