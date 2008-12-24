//  boost/filesystem/convenience.hpp  ----------------------------------------//

//  Copyright Beman Dawes, 2002-2005
//  Copyright Vladimir Prus, 2002
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

//----------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_CONVENIENCE_HPP
#define BOOST_FILESYSTEM_CONVENIENCE_HPP

#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>
#include <vector>
#include <stack>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace filesystem
  {

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED

    BOOST_FS_FUNC_STRING extension(const Path& ph)
    {
      typedef BOOST_FS_TYPENAME Path::string_type string_type;
      string_type filename = ph.filename();

      BOOST_FS_TYPENAME string_type::size_type n = filename.rfind('.');
      if (n != string_type::npos)
        return filename.substr(n);
      else
        return string_type();
    }

    BOOST_FS_FUNC_STRING basename(const Path& ph)
    {
      typedef BOOST_FS_TYPENAME Path::string_type string_type;
      string_type filename = ph.filename();
      BOOST_FS_TYPENAME string_type::size_type n = filename.rfind('.');
      return filename.substr(0, n);
    }


    BOOST_FS_FUNC(Path) change_extension( const Path & ph,
      const BOOST_FS_TYPENAME Path::string_type & new_extension )
      { return ph.parent_path() / (basename(ph) + new_extension); }

    inline std::string extension(const path& ph)
      { return extension<path>(ph); }
    inline std::wstring extension(const wpath& ph)
      { return extension<wpath>(ph); }

    inline std::string basename(const path& ph)
      { return basename<path>( ph ); }
    inline std::wstring basename(const wpath& ph)
      { return basename<wpath>( ph ); }

    inline path change_extension( const path & ph, const std::string& new_ex )
      { return change_extension<path>( ph, new_ex ); }
    inline wpath change_extension( const wpath & ph, const std::wstring& new_ex )
      { return change_extension<wpath>( ph, new_ex ); }
# endif


    //  basic_recursive_directory_iterator helpers  --------------------------//

    namespace detail
    {
      struct recur_dir_itr_imp
      {
        typedef directory_iterator element_type;
        std::stack< element_type, std::vector< element_type > > m_stack;
        int  m_level;
        bool m_no_push;
        bool m_no_throw;

        recur_dir_itr_imp() : m_level(0), m_no_push(false), m_no_throw(false) {}
      };

    } // namespace detail

    //  recursive_directory_iterator  ----------------------------------------//

    class recursive_directory_iterator
      : public boost::iterator_facade<
          recursive_directory_iterator,
          directory_entry,
          boost::single_pass_traversal_tag >
    {
    public:

      recursive_directory_iterator(){}  // creates the "end" iterator

      explicit recursive_directory_iterator( const path & dir_path )
        : m_imp( new detail::recur_dir_itr_imp )
      {
        m_imp->m_stack.push( directory_iterator( dir_path ) );
        if ( m_imp->m_stack.top () == directory_iterator() )
          { m_imp.reset (); }
      }

      recursive_directory_iterator( const path & dir_path,
        system::error_code & ec )
      : m_imp( new detail::recur_dir_itr_imp )
      {
        m_imp->m_no_throw = true;
        m_imp->m_stack.push( directory_iterator( dir_path, ec ) );
        if ( m_imp->m_stack.top () == directory_iterator() )
          { m_imp.reset (); }
      }

      int level() const { return m_imp->m_level; }

      void pop();
      void no_push()
      {
        BOOST_ASSERT( m_imp.get() && "no_push() on end recursive_directory_iterator" );
        m_imp->m_no_push = true;
      }

      file_status status() const
      {
        BOOST_ASSERT( m_imp.get()
          && "status() on end recursive_directory_iterator" );
        return m_imp->m_stack.top()->status();
      }

      file_status symlink_status() const
      {
        BOOST_ASSERT( m_imp.get()
          && "symlink_status() on end recursive_directory_iterator" );
        return m_imp->m_stack.top()->symlink_status();
      }

    private:

      // shared_ptr provides shallow-copy semantics required for InputIterators.
      // m_imp.get()==0 indicates the end iterator.
      boost::shared_ptr< detail::recur_dir_itr_imp >  m_imp;

      friend class boost::iterator_core_access;

      boost::iterator_facade< 
        recursive_directory_iterator,
        directory_entry,
        boost::single_pass_traversal_tag >::reference
      dereference() const 
      {
        BOOST_ASSERT( m_imp.get() && "dereference end recursive_directory_iterator" );
        return *m_imp->m_stack.top();
      }

      void increment();

      bool equal( const recursive_directory_iterator & rhs ) const
        { return m_imp == rhs.m_imp; }

    };

# if  !defined(BOOST_FILESYSTEM_NARROW_ONLY) && !defined(BOOST_FILESYSTEM_NO_DEPRECATED)
    typedef recursive_directory_iterator wrecursive_directory_iterator;
# endif

    //  basic_recursive_directory_iterator implementation  -------------------//

    //  increment
    inline void recursive_directory_iterator::increment()
    {
      BOOST_ASSERT( m_imp.get() && "increment end recursive_directory_iterator" );
      
      static const directory_iterator end_itr;

      if ( m_imp->m_no_push )
        { m_imp->m_no_push = false; }
      else if ( is_directory( m_imp->m_stack.top()->status() ) )
      {
        system::error_code ec;
        m_imp->m_stack.push(
          m_imp->m_no_throw
            ? directory_iterator( *m_imp->m_stack.top(), ec )
            : directory_iterator( *m_imp->m_stack.top() ) );
        if ( m_imp->m_stack.top() != end_itr )
        {
          ++m_imp->m_level;
          return;
        }
        m_imp->m_stack.pop();
      }

      while ( !m_imp->m_stack.empty()
        && ++m_imp->m_stack.top() == end_itr )
      {
        m_imp->m_stack.pop();
        --m_imp->m_level;
      }

      if ( m_imp->m_stack.empty() ) m_imp.reset(); // done, so make end iterator
    }

    //  pop
    inline void recursive_directory_iterator::pop()
    {
      BOOST_ASSERT( m_imp.get() && "pop end recursive_directory_iterator" );
      BOOST_ASSERT( m_imp->m_level > 0 && "pop recursive_directory_iterator with level < 1" );

      static const directory_iterator end_itr;

      do
      {
        m_imp->m_stack.pop();
        --m_imp->m_level;
      }
      while ( !m_imp->m_stack.empty()
        && ++m_imp->m_stack.top() == end_itr );

      if ( m_imp->m_stack.empty() ) m_imp.reset(); // done, so make end iterator
    }

  } // namespace filesystem
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif // BOOST_FILESYSTEM_CONVENIENCE_HPP
