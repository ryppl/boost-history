// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef BOOST_PARALLEL_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP
#define BOOST_PARALLEL_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP

#include <boost/parallel/mpi/datatype_fwd.hpp>
#include <boost/parallel/mpi/detail/mpi_datatype_oarchive.hpp>
#include <boost/parallel/mpi/exception.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <typeinfo>

namespace boost { namespace parallel { namespace mpi { namespace detail {

/// @brief comparison function object for two std::type_info pointers
///
/// is implemented using the before() member function of the std::type_info
/// class

struct type_info_compare
{
  bool operator()(std::type_info const* lhs, std::type_info const* rhs) const
  {
    return lhs->before(*rhs);
  }
};


/// @brief a map of MPI data types, indexed by their type_info
///
///
class mpi_datatype_map
 : private std::map<std::type_info const*,MPI_Datatype,type_info_compare>,
   public boost::noncopyable
{
public:
  mpi_datatype_map()
  {}

  ~mpi_datatype_map()
  {
    // do not free after call to MPI_FInalize
    int finalized=0;
    BOOST_MPI_CHECK_RESULT(MPI_Finalized,(&finalized));
    if (!finalized)
      free();
  }

  template <class T>
  MPI_Datatype datatype(const T& = T(), typename boost::enable_if<is_mpi_builtin_datatype<T> >::type* =0)
  {
    return get_mpi_datatype<T>();
  }

  template <class T>
  MPI_Datatype datatype(const T& x=T(), typename boost::disable_if<is_mpi_builtin_datatype<T> >::type* =0 )
  {
    BOOST_MPL_ASSERT((is_mpi_datatype<T>));

    // check whether the type already exists
    std::type_info const* t = &typeid(T);
    const_iterator it = find(t);
    if(it ==end())
    {
      // need to create a type
      mpi_datatype_oarchive ar(x);
      insert(std::make_pair(t,ar.get_mpi_datatype()));
      it = find(t);
    }

  return it->second;
  }

private:
  // free all MPI data types
  void free()
  {
    // ignore errors in the destructor
    for (iterator it=begin(); it !=end(); ++it)
          MPI_Type_free(&(it->second));
  }

};

extern mpi_datatype_map mpi_datatype_cache;

} } } } // end namespace boost::parallel::mpi::detail


#endif // BOOST_PARALLEL_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP
