
// Copyright (C) 2001, 2002 Roland Richter (roland@flll.jku.at)
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef VIEW_OWNERSHIP_DETAIL_HPP
#define VIEW_OWNERSHIP_DETAIL_HPP

#include <algorithm>
#include <boost/config.hpp>
#include <boost/smart_ptr.hpp>


namespace boost {
  namespace view {
    namespace ownership {

// Forward declarations
template<class T> class unique;
template<class T> class shared;
template<class T> class external;


template<class T> class unique {
public:
  typedef T element_type;

  explicit unique( const T& theT )
    : ptr( new T( theT ) )
  { }

  explicit unique( const unique& rhs )
    : ptr( new T( *(rhs.ptr) ) )
  { }

  explicit unique( const shared<T>& rhs );
  explicit unique( const external<T>& rhs );

  ~unique()
  { delete ptr; }

  unique& swap( unique& rhs )
  {
    std::swap( ptr, rhs.ptr );
    return *this;
  }

  template<class S> unique& operator=( const S& rhs )
  {
    unique(rhs).swap(*this);
    return *this;
  }

  long use_count() const { return 1; }

  T& operator*()  const { return *ptr; }
  T* operator->() const { return ptr; }
  operator T*()   const { return ptr; }
  T* get()        const { return ptr; }

private:
  T* ptr;
};

// Remark: The reason why these are all stored as pointers
// (instead of references) is that I wanted to use shared_ptr!

template<class T> class shared {
public:
  typedef T element_type;

  explicit shared( const T& theT )
    : ptr( new T( theT ) )
  { }

  explicit shared( const shared& rhs )
    : ptr( rhs.ptr )
  { }

  explicit shared( const unique<T>& rhs );
  explicit shared( const external<T>& rhs );

  ~shared()
  { }

  shared& swap( shared& rhs )
  {
    ptr.swap( rhs.ptr );
    return *this;
  }

  /// Swap to allow 'cheap conversion' of unique -> shared ownership.
  shared& swap( unique<T>& rhs )
  {
    //###

    return *this;
  }


  template<class S> shared& operator=( const S& rhs )
  {
    shared(rhs).swap(*this);
    return *this;
  }

  long use_count() const { return ptr.use_count(); }

  /// @name Methods to access the owned contents.
  ///  In order to avoid problems, these should be const.
  ///  In order to avoid other problems, these are non-const.
  /// In a strict sense, a shared entity might be writeable only if use_count() == 1.
  //@{
  T& operator*()  const { return *(ptr.get()); }
  T* operator->() const { return ptr.get(); }
  operator T*()   const { return ptr.get(); }
  T* get()        const { return ptr.get(); }
  //@}

private:
  boost::shared_ptr<T> ptr;
};


template<class T> class external {
public:
  typedef T element_type;

  explicit external( const T& theT )
    : ptr( const_cast<T*>(&theT) )   // Eeewww, that's ugly
  { }

  explicit external( const external& rhs )
    : ptr( rhs.ptr )
  { }

  ~external()
  { }

  external& swap( external& rhs )
  {
    std::swap( ptr, rhs.ptr );
    return *this;
  }

  external& operator=( const external& rhs )
  {
    if( this != &rhs ) ptr = rhs.ptr;
    return *this;
  }

  long use_count() const { return 0; }

  T& operator*()  const { return *ptr; }
  T* operator->() const { return ptr; }
  operator T*()   const { return ptr; }
  T* get()        const { return ptr; }

private:
  T* ptr;
};



  template<class T> unique<T>::unique( const shared<T>& rhs )
    : ptr( new T( *(rhs.get()) ) )
  { }

  template<class T> unique<T>::unique( const external<T>& rhs )
    : ptr( new T( *(rhs.get()) ) )
  { }



  template<class T> shared<T>::shared( const unique<T>& rhs )
    : ptr( new T( *(rhs.get()) ) )
  { }

  template<class T> shared<T>::shared( const external<T>& rhs )
    : ptr( new T( *(rhs.get()) ) )
  { }



template<class T> struct wrap
{
  typedef shared<T> type;  // Ownership is "shared" by default
  typedef T         domain;
};


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<class T> struct wrap< unique<T> >
{
  typedef unique<T> type;
  typedef T         domain;
};

template<class T> struct wrap< shared<T> >
{
  typedef shared<T> type;
  typedef T         domain;
};

template<class T> struct wrap< external<T> >
{
  typedef external<T> type;
  typedef T           domain;
};
#endif


    } // namespace ownership
  } // namespace view
} // namespace boost

#endif
