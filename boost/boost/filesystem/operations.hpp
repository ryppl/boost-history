//  boost/filesystem/directory.hpp  ------------------------------------------//

//  Copyright � 2002-2005 Beman Dawes
//  Copyright � 2002 Jan Langer
//  Copyright � 2001 Dietmar K�hl                                        
//  
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
//  at http://www.boost.org/LICENSE_1_0.txt)                             

//  See library home page at http://www.boost.org/libs/filesystem

//----------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_OPERATIONS_HPP
#define BOOST_FILESYSTEM_OPERATIONS_HPP

#include <boost/filesystem/path.hpp>  // includes <boost/filesystem/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>

#include <string>
#include <utility> // for pair
#include <ctime>

#ifdef BOOST_WINDOWS_API
#include <fstream>
#endif

#include <boost/config/abi_prefix.hpp> // must be the last #include

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::time_t; }
# endif

//----------------------------------------------------------------------------//

namespace boost
{
  namespace filesystem
  {
    template<class Path> class basic_directory_iterator;

    namespace detail
    {
      typedef std::pair< boost::filesystem::system_error_type, bool >
        query_pair;

      typedef std::pair< boost::filesystem::system_error_type, boost::intmax_t >
        intmax_pair;

      typedef std::pair< boost::filesystem::system_error_type, std::time_t >
        time_pair;

      template< class Path >
      struct directory_pair
      {
        typedef std::pair< boost::filesystem::system_error_type,
          typename Path::external_string_type > type;
      };

      BOOST_FILESYSTEM_DECL bool
        exists_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL bool 
        is_accessible_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL bool 
        symbolic_link_exists_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL query_pair
        is_directory_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL query_pair
        is_empty_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL query_pair
        equivalent_api( const std::string & ph1, const std::string & ph2 );
      BOOST_FILESYSTEM_DECL intmax_pair
        file_size_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL time_pair 
        last_write_time_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        last_write_time_api( const std::string & ph, std::time_t new_value );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        get_current_path_api( std::string & ph );
      BOOST_FILESYSTEM_DECL query_pair
        create_directory_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        remove_api( const std::string & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        rename_api( const std::string & from, const std::string & to );

#   ifdef BOOST_WINDOWS_API
      BOOST_FILESYSTEM_DECL bool 
        exists_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL bool 
        is_accessible_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL bool 
        symbolic_link_exists_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL query_pair 
        is_directory_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL query_pair 
        is_empty_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL query_pair
        equivalent_api( const std::wstring & ph1, const std::wstring & ph2 );
      BOOST_FILESYSTEM_DECL intmax_pair 
        file_size_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        get_full_path_name_api( const std::string & ph, std::string & target );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        get_full_path_name_api( const std::wstring & ph, std::wstring & target );
      BOOST_FILESYSTEM_DECL time_pair 
        last_write_time_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        last_write_time_api( const std::wstring & ph, std::time_t new_value );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type 
        get_current_path_api( std::wstring & ph );
      BOOST_FILESYSTEM_DECL query_pair
        create_directory_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        remove_api( const std::wstring & ph );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        rename_api( const std::wstring & from, const std::wstring & to );
#   endif

      template<class Path>
      unsigned long remove_all_aux( const Path & ph );

    } // namespace detail

//  query functions  ---------------------------------------------------------//

    //  The non-template overloads enable automatic conversion from std and
    //  C-style strings. See basic_path constructors. The enable_if for the
    //  templates implements the famous "do-the-right-thing" rule.

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    exists( const Path & ph )
      { return detail::exists_api( ph.external_file_string() ); }
    inline bool exists( const path & ph ) { return exists<path>( ph ); }
    inline bool exists( const wpath & ph ) { return exists<wpath>( ph ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    is_accessible( const Path & ph )
      { return detail::is_accessible_api( ph.external_file_string() ); }
    inline bool is_accessible( const path & ph )
      { return is_accessible<path>( ph ); }
    inline bool is_accessible( const wpath & ph )
      { return is_accessible<wpath>( ph ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    symbolic_link_exists( const Path & ph )
      { return detail::symbolic_link_exists_api( ph.external_file_string() ); }
    inline bool symbolic_link_exists( const path & ph )
      { return symbolic_link_exists<path>( ph ); }
    inline bool symbolic_link_exists( const wpath & ph )
      { return symbolic_link_exists<wpath>( ph ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    is_directory( const Path & ph )
    {
      detail::query_pair result = detail::is_directory_api( ph.external_file_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::is_directory", ph, result.first ) );
      return result.second;
    }
    inline bool is_directory( const path & ph )
      { return is_directory<path>( ph ); }
    inline bool is_directory( const wpath & ph )
      { return is_directory<wpath>( ph ); }

    // VC++ 7.0 and earlier has a serious namespace bug that causes a clash
    // between boost::filesystem::is_empty and the unrelated type trait
    // boost::is_empty.

# if !defined( BOOST_MSVC ) || BOOST_MSVC > 1300
    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    is_empty( const Path & ph )
    {
      detail::query_pair result = detail::is_empty_api( ph.external_file_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::is_empty", ph, result.first ) );
      return result.second;
    }
    inline bool is_empty( const path & ph )
      { return is_empty<path>( ph ); }
    inline bool is_empty( const wpath & ph )
      { return is_empty<wpath>( ph ); }
# endif

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    equivalent( const Path & ph1, const Path & ph2 )
    {
      detail::query_pair result = detail::equivalent_api(
        ph1.external_file_string(), ph2.external_file_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::equivalent", ph1, ph2, result.first ) );
      return result.second;
    }
    inline bool equivalent( const path & ph1, const path & ph2 )
      { return equivalent<path>( ph1, ph2 ); }
    inline bool equivalent( const wpath & ph1, const wpath & ph2 )
      { return equivalent<wpath>( ph1, ph2 ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, boost::intmax_t>::type
    file_size( const Path & ph )
    {
      detail::intmax_pair result
        = detail::file_size_api( ph.external_file_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::file_size", ph, result.first ) );
      return result.second;
    }
    inline boost::intmax_t file_size( const path & ph )
      { return file_size<path>( ph ); }
    inline boost::intmax_t file_size( const wpath & ph )
      { return file_size<wpath>( ph ); }

    template<class Path>
    inline typename boost::enable_if<is_basic_path<Path>, std::time_t>::type
    last_write_time( const Path & ph )
    {
      detail::time_pair result
        = detail::last_write_time_api( ph.external_file_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::last_write_time", ph, result.first ) );
      return result.second;
    }
    inline std::time_t last_write_time( const path & ph )
      { return last_write_time<path>( ph ); }
    inline std::time_t last_write_time( const wpath & ph )
      { return last_write_time<wpath>( ph ); }

//  operations  --------------------------------------------------------------//

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    create_directory( const Path & dir_ph )
    {
      detail::query_pair result
        = detail::create_directory_api( dir_ph.external_directory_string() );
      if ( result.first != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::create_directory",
          dir_ph, result.first ) );
      return result.second;
    }
    inline bool create_directory( const path & dir_ph )
      { return create_directory<path>( dir_ph ); }
    inline bool create_directory( const wpath & dir_ph )
      { return create_directory<wpath>( dir_ph ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, bool>::type
    remove( const Path & ph )
    {
      if ( exists( ph )
        || symbolic_link_exists( ph ) ) // handle dangling symbolic links
        // note that the POSIX behavior for symbolic links is what we want;
        // the link rather than what it points to is deleted. Windows behavior
        // doesn't matter; symbolic_link_exists() is always false on Windows.
      {
        system_error_type result = detail::remove_api( ph.external_file_string() );
        if ( result != 0 )
          boost::throw_exception( basic_filesystem_error<Path>(
            "boost::filesystem::remove",
            ph, result ) );
        return true;
      }
      return false;
    }
    inline bool remove( const path & ph )
      { return remove<path>( ph ); }
    inline bool remove( const wpath & ph )
      { return remove<wpath>( ph ); }


    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, unsigned long>::type
    remove_all( const Path & ph )
    {
      return exists( ph )|| symbolic_link_exists( ph )
        ? detail::remove_all_aux( ph ) : 0;
    }
    inline unsigned long remove_all( const path & ph )
      { return remove_all<path>( ph ); }
    inline unsigned long remove_all( const wpath & ph )
      { return remove_all<wpath>( ph ); }

    template<class Path>
    typename boost::enable_if<is_basic_path<Path>, void>::type
    rename( const Path & from_path, const Path & to_path )
    {
      system_error_type result = detail::rename_api(
        from_path.external_directory_string(),
        to_path.external_directory_string() );
      if ( result != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::rename",
          from_path, to_path, result ) );
    }
    inline void rename( const path & from_path, const path & to_path )
      { return rename<path>( from_path, to_path ); }
    inline void rename( const wpath & from_path, const wpath & to_path )
      { return rename<wpath>( from_path, to_path ); }

    BOOST_FILESYSTEM_DECL void copy_file( const path & from_file_ph,
                    const path & to_file_ph );
    BOOST_FILESYSTEM_DECL void copy_file( const wpath & from_file_ph,
                    const wpath & to_file_ph );

    template< class Path >
    Path current_path()
    {
      typename Path::external_string_type ph;
      boost::filesystem::system_error_type result;
      if ( (result = detail::get_current_path_api( ph )) != 0 )
          boost::throw_exception( basic_filesystem_error<Path>(
            "boost::filesystem::current_path", result ) );
      return Path( Path::traits_type::to_internal( ph ) );
    }

    template< class Path >
    Path initial_path()
    {
      static Path init_path;
      if ( init_path.empty() ) init_path = current_path<Path>();
      return init_path;
    }

    template< class Path >
    typename boost::enable_if<is_basic_path<Path>, Path>::type
    system_complete( const Path & ph )
    {
# ifdef BOOST_WINDOWS_API
      if ( ph.empty() ) return ph;
      typename Path::external_string_type sys_ph;
      boost::filesystem::system_error_type result;
      if ( (result = detail::get_full_path_name_api( ph.external_file_string(),
              sys_ph )) != 0 )
          boost::throw_exception( basic_filesystem_error<Path>(
            "boost::filesystem::system_complete", ph, result ) );
      return Path( Path::traits_type::to_internal( sys_ph ) );
# else
      return (ph.empty() || ph.is_complete())
        ? ph : current_path<Path>() / ph;
# endif
    }
    inline path system_complete( const path & ph )
      { return system_complete<path>( ph ); }
    inline wpath system_complete( const wpath & ph )
      { return system_complete<wpath>( ph ); }

    template< class Path >
    typename boost::enable_if<is_basic_path<Path>, Path>::type
    complete( const Path & ph,
      const Path & base/* = initial_path<Path>() */)
    {
      BOOST_ASSERT( base.is_complete()
        && (ph.is_complete() || !ph.has_root_name()) ); // preconditions
#   ifdef BOOST_WINDOWS_PATH
      if (ph.empty() || ph.is_complete()) return ph;
      if ( !ph.has_root_name() )
        return ph.has_root_directory()
          ? Path( base.root_name() ) / ph
          : base / ph;
      return base / ph;
#   else
      return (ph.empty() || ph.is_complete()) ? ph : base / ph;
#   endif
    }
    inline path complete( const path & ph,
      const path & base/* = initial_path<path>()*/ )
      { return complete<path>( ph, base ); }
    inline wpath complete( const wpath & ph,
      const wpath & base/* = initial_path<wpath>()*/ )
      { return complete<wpath>( ph, base ); }

    // VC++ 7.1 had trouble with default arguments, so separate one argument
    // signatures are provided as workarounds; the effect is the same.
    template< class Path >
    inline typename boost::enable_if<is_basic_path<Path>, Path>::type
    complete( const Path & ph )
      { return complete( ph, initial_path<Path>() ); }

    inline path complete( const path & ph )
      { return complete<path>( ph, initial_path<path>() ); }
    inline wpath complete( const wpath & ph )
      { return complete<wpath>( ph, initial_path<wpath>() ); }


    template< class Path >
    typename boost::enable_if<is_basic_path<Path>, void>::type
    last_write_time( const Path & ph, const std::time_t new_time )
    {
      boost::filesystem::system_error_type result;
      if ( (result = detail::last_write_time_api( ph.external_file_string(),
          new_time )) != 0 )
        boost::throw_exception( basic_filesystem_error<Path>(
          "boost::filesystem::last_write_time", ph, result ) );
    }
    inline void last_write_time( const path & ph, const std::time_t new_time )
      { last_write_time<path>( ph, new_time ); }
    inline void last_write_time( const wpath & ph, const std::time_t new_time )
      { last_write_time<wpath>( ph, new_time ); }

    namespace detail
    {
      template<class Path>
      unsigned long remove_all_aux( const Path & ph )
      {
        static const boost::filesystem::basic_directory_iterator<Path> end_itr;
        unsigned long count = 1;
        if ( !boost::filesystem::symbolic_link_exists( ph ) // don't recurse symbolic links
          && boost::filesystem::is_directory( ph ) )
        {
          for ( boost::filesystem::basic_directory_iterator<Path> itr( ph );
                itr != end_itr; ++itr )
          {
            count += remove_all_aux( *itr );
          }
        }
        boost::filesystem::remove( ph );
        return count;
      }

//  test helper  -------------------------------------------------------------//

    // not part of the documented interface because false positives are possible;
    // there is no law that says that an OS that has large stat.st_size
    // actually supports large file sizes.
      BOOST_FILESYSTEM_DECL bool possible_large_file_size_support();

//  directory_iterator helpers  ----------------------------------------------//

//    forwarding functions avoid need for BOOST_FILESYSTEM_DECL for class
//    basic_directory_iterator, and so avoid iterator_facade DLL template
//    problems. They also overload to the proper external path character type.

      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        dir_itr_first(
          void *& handle, const std::string & dir_path, std::string & target );
      // eof: return==0 && handle==0

      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        dir_itr_increment( void *& handle, std::string & target );
      // eof: return==0 && handle==0

      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        dir_itr_close( void *& handle );
      // Effects: none if handle==0, otherwise close handle, set handle=0

#     ifdef BOOST_WINDOWS_API
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        dir_itr_first(
          void *& handle, const std::wstring & ph, std::wstring & target );
      BOOST_FILESYSTEM_DECL boost::filesystem::system_error_type
        dir_itr_increment( void *& handle, std::wstring & target );
#     endif

      template< class Path >
      class dir_itr_imp
      {
      public:  
        Path    m_path;
        void *  m_handle;

        dir_itr_imp() : m_handle(0) {}

        ~dir_itr_imp()
        {
          dir_itr_close( m_handle );
        }
      };

    } // namespace detail

//  basic_directory_iterator  ------------------------------------------------//

    template< class Path >
    class basic_directory_iterator
      : public boost::iterator_facade<
      basic_directory_iterator<Path>,
      Path,
      boost::single_pass_traversal_tag >
    {
    public:
      basic_directory_iterator(){}  // creates the "end" iterator

      // TODO: move definition out of class body
      explicit basic_directory_iterator( const Path & dir_path )
        : m_imp( new detail::dir_itr_imp<Path> )
      {
        system_error_type sys_err(0);
        typename Path::external_string_type name;

        if ( dir_path.empty()
          || (sys_err = detail::dir_itr_first( m_imp->m_handle,
          dir_path.external_directory_string(), name )) != 0 )
        {
          boost::throw_exception( basic_filesystem_error<Path>(  
            "boost::filesystem::basic_directory_iterator constructor",
            dir_path, sys_err ) );
        }
        
        if ( m_imp->m_handle == 0 ) m_imp.reset(); // eof, so make end iterator
        else // not eof
        {
          m_imp->m_path = dir_path;
          m_imp->m_path /= Path::traits_type::to_internal( name );
          if ( name[0] == path_relative<Path>::value // dot or dot-dot
            && (name.size() == 1
              || (name[1] == path_relative<Path>::value
                && name.size() == 2)) )
            {  increment(); }
        }
      }

    private:
      // shared_ptr provides shallow-copy semantics required for InputIterators.
      // m_imp.get()==0 indicates the end iterator.
      boost::shared_ptr< detail::dir_itr_imp< Path > >  m_imp;

      friend class boost::iterator_core_access;

      typename boost::iterator_facade< basic_directory_iterator<Path>, Path,
      boost::single_pass_traversal_tag >::reference dereference() const 
      {
        BOOST_ASSERT( m_imp.get() && "attempt to dereference end iterator" );
        return m_imp->m_path;
      }

      // TODO: move definition out of class body
      void increment()
      {
        BOOST_ASSERT( m_imp.get() && "attempt to increment end iterator" );
        BOOST_ASSERT( m_imp->m_handle != 0 && "internal program error" );
        
        system_error_type sys_err(0);
        typename Path::external_string_type name;

        for (;;)
        {
          if ( (sys_err = detail::dir_itr_increment( m_imp->m_handle, name ))
            != 0 )
          {
            boost::throw_exception( basic_filesystem_error<Path>(  
              "boost::filesystem::basic_directory_iterator increment",
              m_imp->m_path.branch_path(), sys_err ) );
          }
          if ( m_imp->m_handle == 0 ) { m_imp.reset(); return; } // eof, make end
          if ( !(name[0] == path_relative<Path>::value // !(dot or dot-dot)
            && (name.size() == 1
              || (name[1] == path_relative<Path>::value
                && name.size() == 2))) )
          {
            m_imp->m_path.remove_leaf();
            m_imp->m_path /= Path::traits_type::to_internal( name );
            return;
          }
        }
      }

      bool equal( const basic_directory_iterator & rhs ) const
        { return m_imp == rhs.m_imp; }
    };

    typedef basic_directory_iterator< path > directory_iterator;
    typedef basic_directory_iterator< wpath > wdirectory_iterator;

  } // namespace filesystem
} // namespace boost


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif // BOOST_FILESYSTEM_OPERATIONS_HPP
