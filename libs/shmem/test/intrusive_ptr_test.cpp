//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Peter Dimov 2002-2005.
// (C) Copyright Ion Gazta�aga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/offset_ptr.hpp>
#include <boost/shmem/smart_ptr/intrusive_ptr.hpp>
#include <boost/shmem/named_shared_object.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <boost/detail/atomic_count.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <functional>

#if defined(BOOST_MSVC)

#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#pragma warning(disable: 4355)  // 'this' : used in base member initializer list
#pragma warning(disable: 4511)  // copy constructor could not be generated
#pragma warning(disable: 4512)  // assignment operator could not be generated

#if (BOOST_MSVC >= 1310)
#pragma warning(disable: 4675)  // resolved overload found with Koenig lookup
#endif

#endif

typedef boost::shmem::offset_ptr<void> VP;

namespace N
{

class base
{
   private:

   boost::detail::atomic_count use_count_;

   base(base const &);
   base & operator=(base const &);

   protected:

   base(): use_count_(0)
   {
   }

   virtual ~base()
   {
   }

   public:

   long use_count() const
   {
      return use_count_;
   }

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)

   inline friend void intrusive_ptr_add_ref
      (base * p)
   {
      ++p->use_count_;
   }

   inline friend void intrusive_ptr_release
      (base * p)
   {
      if(--p->use_count_ == 0) delete p;
   }

#else

   void add_ref()
   {
      ++use_count_;
   }

   void release()
   {
      if(--use_count_ == 0) delete this;
   }

#endif
};

} // namespace N

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)

namespace boost
{

inline void intrusive_ptr_add_ref
   (const boost::shmem::offset_ptr<N::base> &p)
{
   p->add_ref();
}

inline void intrusive_ptr_release
   (const boost::shmem::offset_ptr<N::base> &p)
{
   p->release();
}

} // namespace boost

#endif

//

struct X: public virtual N::base
{
};

struct Y: public X
{
};

//

namespace n_element_type
{

void f(X &)
{
}

void test()
{
   typedef boost::shmem::intrusive_ptr<X, VP>::element_type T;
   T t;
   f(t);
}

} // namespace n_element_type

namespace n_constructors
{

void default_constructor()
{
   boost::shmem::intrusive_ptr<X, VP> px;
   BOOST_TEST(px.get() == 0);
}

void pointer_constructor()
{
   {
      boost::shmem::intrusive_ptr<X, VP> px(0);
      BOOST_TEST(px.get() == 0);
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(0, false);
      BOOST_TEST(px.get() == 0);
   }

   {
      boost::shmem::offset_ptr<X> p = new X;
      BOOST_TEST(p->use_count() == 0);

      boost::shmem::intrusive_ptr<X, VP> px(p);
      BOOST_TEST(px.get() == p);
      BOOST_TEST(px->use_count() == 1);
   }

   {
      boost::shmem::offset_ptr<X> p = new X;
      BOOST_TEST(p->use_count() == 0);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
      using boost::intrusive_ptr_add_ref;
#endif
      intrusive_ptr_add_ref(get_pointer(p));
      BOOST_TEST(p->use_count() == 1);

      boost::shmem::intrusive_ptr<X, VP> px(p, false);
      BOOST_TEST(px.get() == p);
      BOOST_TEST(px->use_count() == 1);
   }
}

void copy_constructor()
{
   {
      boost::shmem::intrusive_ptr<X, VP> px;
      boost::shmem::intrusive_ptr<X, VP> px2(px);
      BOOST_TEST(px2.get() == px.get());
   }

   {
      boost::shmem::intrusive_ptr<Y, VP> py;
      boost::shmem::intrusive_ptr<X, VP> px(py);
      BOOST_TEST(px.get() == py.get());
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(0);
      boost::shmem::intrusive_ptr<X, VP> px2(px);
      BOOST_TEST(px2.get() == px.get());
   }

   {
      boost::shmem::intrusive_ptr<Y, VP> py(0);
      boost::shmem::intrusive_ptr<X, VP> px(py);
      BOOST_TEST(px.get() == py.get());
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(0, false);
      boost::shmem::intrusive_ptr<X, VP> px2(px);
      BOOST_TEST(px2.get() == px.get());
   }

   {
      boost::shmem::intrusive_ptr<Y, VP> py(0, false);
      boost::shmem::intrusive_ptr<X, VP> px(py);
      BOOST_TEST(px.get() == py.get());
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(new X);
      boost::shmem::intrusive_ptr<X, VP> px2(px);
      BOOST_TEST(px2.get() == px.get());
   }

   {
      boost::shmem::intrusive_ptr<Y, VP> py(new Y);
      boost::shmem::intrusive_ptr<X, VP> px(py);
      BOOST_TEST(px.get() == py.get());
   }
}

void test()
{
   default_constructor();
   pointer_constructor();
   copy_constructor();
}

} // namespace n_constructors

namespace n_destructor
{

void test()
{
   boost::shmem::intrusive_ptr<X, VP> px(new X);
   BOOST_TEST(px->use_count() == 1);

   {
      boost::shmem::intrusive_ptr<X, VP> px2(px);
      BOOST_TEST(px->use_count() == 2);
   }

   BOOST_TEST(px->use_count() == 1);
}

} // namespace n_destructor

namespace n_assignment
{

void copy_assignment()
{
}

void conversion_assignment()
{
}

void pointer_assignment()
{
}

void test()
{
   copy_assignment();
   conversion_assignment();
   pointer_assignment();
}

} // namespace n_assignment

namespace n_access
{

void test()
{
   {
      boost::shmem::intrusive_ptr<X, VP> px;
      BOOST_TEST(px? false: true);
      BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
      using boost::get_pointer;
#endif

      BOOST_TEST(get_pointer(px) == px.get());
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(0);
      BOOST_TEST(px? false: true);
      BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
      using boost::get_pointer;
#endif

      BOOST_TEST(get_pointer(px) == px.get());
   }

   {
#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
      using boost::get_pointer;
#endif
      boost::shmem::intrusive_ptr<X, VP> px
         (boost::shmem::offset_ptr<X>(new X));
      BOOST_TEST(px? true: false);
      BOOST_TEST(!!px);
      BOOST_TEST(&*px == get_pointer(px.get()));
      BOOST_TEST(px.operator ->() == px.get());
      BOOST_TEST(get_pointer(px) == px.get());
   }
}

} // namespace n_access

namespace n_swap
{

void test()
{
   {
      boost::shmem::intrusive_ptr<X, VP> px;
      boost::shmem::intrusive_ptr<X, VP> px2;

      px.swap(px2);

      BOOST_TEST(px.get() == 0);
      BOOST_TEST(px2.get() == 0);

      using std::swap;
      swap(px, px2);

      BOOST_TEST(px.get() == 0);
      BOOST_TEST(px2.get() == 0);
   }

   {
      boost::shmem::offset_ptr<X> p = new X;
      boost::shmem::intrusive_ptr<X, VP> px;
      boost::shmem::intrusive_ptr<X, VP> px2(p);
      boost::shmem::intrusive_ptr<X, VP> px3(px2);

      px.swap(px2);

      BOOST_TEST(px.get() == p);
      BOOST_TEST(px->use_count() == 2);
      BOOST_TEST(px2.get() == 0);
      BOOST_TEST(px3.get() == p);
      BOOST_TEST(px3->use_count() == 2);

      using std::swap;
      swap(px, px2);

      BOOST_TEST(px.get() == 0);
      BOOST_TEST(px2.get() == p);
      BOOST_TEST(px2->use_count() == 2);
      BOOST_TEST(px3.get() == p);
      BOOST_TEST(px3->use_count() == 2);
   }

   {
      boost::shmem::offset_ptr<X> p1 = new X;
      boost::shmem::offset_ptr<X> p2 = new X;
      boost::shmem::intrusive_ptr<X, VP> px(p1);
      boost::shmem::intrusive_ptr<X, VP> px2(p2);
      boost::shmem::intrusive_ptr<X, VP> px3(px2);

      px.swap(px2);

      BOOST_TEST(px.get() == p2);
      BOOST_TEST(px->use_count() == 2);
      BOOST_TEST(px2.get() == p1);
      BOOST_TEST(px2->use_count() == 1);
      BOOST_TEST(px3.get() == p2);
      BOOST_TEST(px3->use_count() == 2);

      using std::swap;
      swap(px, px2);

      BOOST_TEST(px.get() == p1);
      BOOST_TEST(px->use_count() == 1);
      BOOST_TEST(px2.get() == p2);
      BOOST_TEST(px2->use_count() == 2);
      BOOST_TEST(px3.get() == p2);
      BOOST_TEST(px3->use_count() == 2);
   }
}

} // namespace n_swap

namespace n_comparison
{

template<class T, class U, class VP>
void test2(boost::shmem::intrusive_ptr<T, VP> const & p,
           boost::shmem::intrusive_ptr<U, VP> const & q)
{
   BOOST_TEST((p == q) == (p.get() == q.get()));
   BOOST_TEST((p != q) == (p.get() != q.get()));
}

template<class T, class VP>
void test3(boost::shmem::intrusive_ptr<T, VP> const & p,
           boost::shmem::intrusive_ptr<T, VP> const & q)
{
   BOOST_TEST((p == q) == (p.get() == q.get()));
   BOOST_TEST((p.get() == q) == (p.get() == q.get()));
   BOOST_TEST((p == q.get()) == (p.get() == q.get()));
   BOOST_TEST((p != q) == (p.get() != q.get()));
   BOOST_TEST((p.get() != q) == (p.get() != q.get()));
   BOOST_TEST((p != q.get()) == (p.get() != q.get()));

   // 'less' moved here as a g++ 2.9x parse error workaround
   std::less<boost::shmem::offset_ptr<T> > less;
   BOOST_TEST((p < q) == less(p.get(), q.get()));
}

void test()
{
   {
      boost::shmem::intrusive_ptr<X, VP> px;
      test3(px, px);

      boost::shmem::intrusive_ptr<X, VP> px2;
      test3(px, px2);

      boost::shmem::intrusive_ptr<X, VP> px3(px);
      test3(px3, px3);
      test3(px, px3);
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px;

      boost::shmem::intrusive_ptr<X, VP> px2(new X);
      test3(px, px2);
      test3(px2, px2);

      boost::shmem::intrusive_ptr<X, VP> px3(new X);
      test3(px2, px3);

      boost::shmem::intrusive_ptr<X, VP> px4(px2);
      test3(px2, px4);
      test3(px4, px4);
   }

   {
      boost::shmem::intrusive_ptr<X, VP> px(new X);

      boost::shmem::intrusive_ptr<Y, VP> py(new Y);
      test2(px, py);

      boost::shmem::intrusive_ptr<X, VP> px2(py);
      test2(px2, py);
      test3(px, px2);
      test3(px2, px2);
   }
}

} // namespace n_comparison

namespace n_static_cast
{

void test()
{
}

} // namespace n_static_cast

namespace n_dynamic_cast
{

void test()
{
}

} // namespace n_dynamic_cast

namespace n_transitive
{

struct X: public N::base
{
   boost::shmem::intrusive_ptr<X, VP> next;
};

void test()
{
   boost::shmem::intrusive_ptr<X, VP> p(new X);
   p->next = boost::shmem::intrusive_ptr<X, VP>(new X);
   BOOST_TEST(!p->next->next);
   p = p->next;
   BOOST_TEST(!p->next);
}

} // namespace n_transitive

namespace n_report_1
{

class foo: public N::base
{ 
   public:

   foo(): m_self(this)
   {
   } 

   void suicide()
   {
      m_self = 0;
   }

   private:

   boost::shmem::intrusive_ptr<foo, VP> m_self;
}; 

void test()
{
   boost::shmem::offset_ptr<foo> foo_ptr = new foo;
   foo_ptr->suicide();
} 

} // namespace n_report_1

int main()
{
   n_element_type::test();
   n_constructors::test();
   n_destructor::test();
   n_assignment::test();
   n_access::test();
   n_swap::test();
   n_comparison::test();
   n_static_cast::test();
   n_dynamic_cast::test();

   n_transitive::test();
   n_report_1::test();

   return boost::report_errors();
}
