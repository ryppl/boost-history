/*
 *
 * Copyright (c) 2003
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

class fileview
{
public:
   // types:
   typedef const char&                               reference;
   typedef reference                                 const_reference;
   typedef const char*                               iterator;       // See _lib.container.requirements_
   typedef iterator                                  const_iterator; // See _lib.container.requirements_
   typedef std::size_t                               size_type;
   typedef std::ptrdiff_t                            difference_type;
   typedef char                                      value_type;
   typedef const char*                               pointer;
   typedef pointer                                   const_pointer;
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
   typedef std::reverse_iterator<iterator>           reverse_iterator;
   typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;
#endif

   // construct:
   fileview();
   fileview(const boost::filesystem::path& p);
   ~fileview();
   fileview(const fileview& that);
   fileview& operator=(const fileview& that);
   void close();
   void open(const boost::filesystem::path& p);

   // iterators:
   const_iterator         begin() const;
   const_iterator         end() const;
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
   const_reverse_iterator rbegin() const;
   const_reverse_iterator rend() const;
#endif

   // capacity:
   size_type size() const;
   size_type max_size() const;
   bool      empty() const;

   // element access:
   const_reference operator[](size_type n) const;
   const_reference at(size_type n) const;
   const_reference front() const;
   const_reference back() const;
   void            swap(fileview& that);

private:
   void cow();
   struct implementation;
   boost::shared_ptr<implementation> pimpl;
};


