//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __BOOST_DELETED_FUNCTIONS__HPP
#define __BOOST_DELETED_FUNCTIONS__HPP

#include <memory>
#include <boost/config.hpp>

#if defined(BOOST_HAS_DELETED_FUNCTIONS)

#define BOOST_DESTRUCTOR_DELETE(T)                  \
  ~T()=delete;

#define BOOST_DEFAULT_CONSTRUCTOR_DELETE(T)         \
  T()=delete;

#define BOOST_COPY_CONSTRUCTOR_DELETE(T)            \
  T(const T&)=delete;

#define BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(T)            \
  T(T&)=delete;

#define BOOST_COPY_ASSIGNEMENT_DELETE(T)            \
  T& operator=(const T&)=delete;

#define BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(T)            \
  const T& operator=(T&)=delete;

#define BOOST_SEQUENCE_DELETE(T)                    \
  T operator,(T)=delete;

#define BOOST_ADRESS_OF_DELETE(T)                   \
  const T* operator&() const =delete;

#define BOOST_NON_CONST_ADRESS_OF_DELETE(T)         \
  T* operator&()=delete;

#define BOOST_INDIRECTION_DELETE(T)                 \
  const T& operator*() const =delete;

#define BOOST_NON_CONST_INDIRECTION_DELETE(T)       \
  T& operator*()=delete;

#define BOOST_MEMBER_ACCESS_DELETE(T)               \
  const T* operator->() const =delete;

#define BOOST_NON_CONST_MEMBER_ACCESS_DELETE(T)     \
  T* operator->()=delete;

#define BOOST_NEW_DELETE(T)                         \
  void* operator new(unsigned)=delete;

#define BOOST_NEW_ARRAY_DELETE(T)                   \
  void* operator new[](unsigned)=delete;

#define BOOST_NEW_PLACEMENT_DELETE(T)               \
  void operator delete(void*, unsigned)=delete;

#define BOOST_NEW_ARRAY_PLACEMENT_DELETE(T)         \
  void operator delete[](void*, unsigned)=delete;

#else // !defined(BOOST_HAS_DELETED_FUNCTIONS)

#define BOOST_DESTRUCTOR_DELETE(T)                  \
private:                                            \
  ~T();                                             \
public:

#define BOOST_DEFAULT_CONSTRUCTOR_DELETE(T)         \
private:                                            \
  T();                                              \
public:

#define BOOST_COPY_CONSTRUCTOR_DELETE(T)            \
private:                                            \
  T(const T&);                                      \
public:

#define BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(T)            \
private:                                            \
  T(T&);                                            \
public:

#define BOOST_COPY_ASSIGNEMENT_DELETE(T)            \
private:                                            \
  T& operator=(const T&);                     \
public:

#define BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(T)            \
private:                                            \
  T& operator=(T&);                           \
public:

#define BOOST_SEQUENCE_DELETE(T)                    \
private:                                            \
  T operator,(T);                                   \
public:

#define BOOST_ADRESS_OF_DELETE(T)                   \
private:                                            \
  const T* operator&() const;                       \
public:

#define BOOST_NON_CONST_ADRESS_OF_DELETE(T)         \
private:                                            \
  T* operator&();                                   \
public:

#define BOOST_INDIRECTION_DELETE(T)                 \
private:                                            \
  const T& operator*() const;                       \
public:

#define BOOST_NON_CONST_INDIRECTION_DELETE(T)       \
private:                                            \
  T& operator*();                                   \
public:

#define BOOST_MEMBER_ACCESS_DELETE(T)               \
private:                                            \
  const T* operator->() const;                      \
public:

#define BOOST_NON_CONST_MEMBER_ACCESS_DELETE(T)     \
private:                                            \
  T* operator->();                                  \
public:

#define BOOST_NEW_DELETE(T)                         \
private:                                            \
  void* operator new(unsigned);                     \
public:

#define BOOST_NEW_ARRAY_DELETE(T)                   \
private:                                            \
  void* operator new[](unsigned);                   \
public:

#define BOOST_NEW_PLACEMENT_DELETE(T)               \
private:                                            \
  void operator delete(void*, unsigned);            \
public:

#define BOOST_NEW_ARRAY_PLACEMENT_DELETE(T)         \
private:                                            \
  void operator delete[](void*, unsigned);          \
public:

#endif

#define BOOST_HEAP_ALLOCATEION_DELETE(T)            \
    BOOST_NEW_DELETE(T)                             \
    BOOST_NEW_ARRAY_DELETE(T)                       \
    BOOST_NEW_PLACEMENT_DELETE(T)                   \
    BOOST_NEW_ARRAY_PLACEMENT_DELETE(T)

#endif // !defined(BOOST_HAS_DELETED_FUNCTIONS)


