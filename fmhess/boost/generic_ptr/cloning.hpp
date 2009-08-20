//
//  generic_ptr/cloning.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_CLONING_HPP_INCLUDED
#define BOOST_GENERIC_PTR_CLONING_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/generic_ptr/detail/util.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  namespace generic_ptr
  {
    template<typename T>
    T* construct_clone(void *location, T *p)
    {
      if(p == 0) return 0;
      // based on boost::new_clone
      T* result = new(location) T(*p);
      BOOST_ASSERT
      (
        typeid(*p) == typeid(*result) &&
        "Default construct_clone() sliced object!"
      );
      return result;
    }
    template<typename GenericPointer>
    GenericPointer construct_clone
    (
      void *location,
      const GenericPointer &p,
      typename pointer_traits<GenericPointer>::value_type * = 0
    )
    {
      return GenericPointer(construct_clone(location, get_pointer(p)));
    }
    template<typename T>
    void destroy_clone(T *p)
    {
      if(p == 0) return;
      // make sure type is complete: based on boost::checked_delete
      typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
      (void) sizeof(type_must_be_complete);
      p->~T();
    }
    template<typename GenericPointer>
    void destroy_clone
    (
      const GenericPointer &p,
      typename pointer_traits<GenericPointer>::value_type * = 0
    )
    {
      return destroy_clone(get_pointer(p));
    }

    namespace detail
    {
      template<typename GenericVoidPointer>
      class clone_factory_impl_base
      {
      public:
        virtual ~clone_factory_impl_base() {}
        virtual GenericVoidPointer get_pointer() = 0;
        virtual clone_factory_impl_base* make_clone() = 0;
      };

      template<typename GenericPointer, typename Cloner>
      class clone_factory_impl: public
        clone_factory_impl_base
        <
          typename rebind<GenericPointer, void>::other
        >
      {
      public:
        clone_factory_impl(GenericPointer p, Cloner c):
          cloner(c),
          px(cloner.allocate_clone(p))
        {}
        ~clone_factory_impl()
        {
          cloner.deallocate_clone(px);
        }
        virtual typename rebind<GenericPointer, void>::other get_pointer()
        {
          return const_pointer_cast
            <
              typename remove_const
              <
                typename pointer_traits<GenericPointer>::value_type
              >::type
            >(px);
        }
        virtual clone_factory_impl* make_clone()
        {
          return new clone_factory_impl(px, cloner);
        }
      private:
        Cloner cloner;
        GenericPointer px;
      };

      template<typename GenericVoidPointer>
      class clone_factory
      {
      public:
        clone_factory(): _impl()
        {}
        template<typename T, typename Cloner>
        clone_factory(T p, Cloner c): _impl(new clone_factory_impl<T, Cloner>(p, c))
        {}
        clone_factory(const clone_factory &other): _impl(other._impl->make_clone())
        {}
#ifndef BOOST_NO_RVALUE_REFERENCES
        clone_factory(clone_factory && other)
        {
          swap(other);
        }
#endif
        GenericVoidPointer get_pointer()
        {
          if(_impl.get() == 0) return GenericVoidPointer();
          return _impl->get_pointer();
        }
        void swap(clone_factory &other)
        {
          boost::swap(_impl, other._impl);
        }
      private:
        clone_factory& operator=(const clone_factory &);  // could be implemented and made public if we needed it

        scoped_ptr<clone_factory_impl_base<GenericVoidPointer> > _impl;
      };
      template<typename T>
      void swap(clone_factory<T> &a, clone_factory<T> &b)
      {
        a.swap(b);
      }
    }

    template<typename T>
    class default_cloner
    {
    public:
      default_cloner(): _allocated(false)
      {}
      default_cloner(const default_cloner &): _allocated(false)
      {}
      default_cloner & operator=(const default_cloner &)
      {}
      template<typename GenericPointer>
      GenericPointer allocate_clone(const GenericPointer & p)
      {
        BOOST_ASSERT(_allocated == false);
        using boost::generic_ptr::construct_clone;
        GenericPointer result(construct_clone(storage(), p));
        _allocated = true;
        return result;
      }
      template<typename GenericPointer>
      void deallocate_clone(const GenericPointer & p)
      {
        typename pointer_traits<GenericPointer>::value_type *pop = get_plain_old_pointer(p);
        if(pop == 0) return;
        BOOST_ASSERT(pop == storage());
        BOOST_ASSERT(_allocated);
        _allocated = false;
        using boost::generic_ptr::destroy_clone;
        destroy_clone(p);
      }
    private:
      void * storage() {return boost::addressof(_storage);}
      boost::aligned_storage<sizeof(T), boost::alignment_of<T>::value> _storage;
      bool _allocated;
    };

    template<typename T>
    class cloning
    {
      typedef cloning this_type; // for detail/operator_bool.hpp
      template<typename U>
      friend class cloning;
      typedef detail::clone_factory<typename generic_ptr::rebind<T, void>::other> clone_factory_type;
    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;

      template<typename ValueType>
      struct rebind
      {
        typedef cloning<typename generic_ptr::rebind<pointer, ValueType>::other> other;
      };

      cloning(): _clone_factory(), px()
      {}
      template<typename U>
      cloning
      (
        U p
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          default_cloner<typename pointer_traits<U>::value_type>()
        ),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}
#ifndef BOOST_NO_SFINAE
      template<typename U>
      explicit cloning
      (
        U p
        , typename disable_if<is_convertible<U, T> >::type * = 0
      ):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          default_cloner<typename pointer_traits<U>::value_type>()
        ),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}
#endif // BOOST_NO_SFINAE
      template<typename U, typename Deleter>
      cloning
      (
        U p,
        Deleter d
      ):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          d,
          default_cloner<typename pointer_traits<U>::value_type>()
        ),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}
      template<typename U, typename Deleter, typename Cloner>
      cloning(U p, Deleter d, Cloner c):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          c
        ),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}
      cloning(const cloning & other):
        _clone_factory(other._clone_factory),
        px
        (
          static_pointer_cast<value_type>(_clone_factory.get_pointer())
        )
      {}
      template<typename U>
      cloning
      (
        const cloning<U> & other
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ):
        _clone_factory(other._clone_factory),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}

      // casts
      template<typename U>
      cloning(const cloning<U> & other, detail::static_cast_tag):
        _clone_factory(other._clone_factory),
        px
        (
          static_pointer_cast
          <
            value_type
          >
          (
            static_pointer_cast
            <
              typename pointer_traits<U>::value_type
            >(_clone_factory.get_pointer())
          )
        )
      {}
      template<typename U>
      cloning(const cloning<U> & other, detail::const_cast_tag):
        _clone_factory(other._clone_factory),
        px
        (
          const_pointer_cast
          <
            value_type
          >
          (
            static_pointer_cast
            <
              typename pointer_traits<U>::value_type
            >(_clone_factory.get_pointer())
          )
        )
      {}
      template<typename U>
      cloning(const cloning<U> & other, detail::dynamic_cast_tag):
        _clone_factory(other._clone_factory),
        px
        (
          dynamic_pointer_cast
          <
            value_type
          >
          (
            static_pointer_cast
            <
              typename pointer_traits<U>::value_type
            >(_clone_factory.get_pointer())
          )
        )
      {
        // reset _clone_factory if dynamic cast failed
        if(get_plain_old_pointer(px) == 0)
        {
          clone_factory_type().swap(_clone_factory);
        }
      }

#ifndef BOOST_NO_RVALUE_REFERENCES
      cloning(cloning && other): _clone_factory(std::move(other._clone_factory)), px(std::move(other.px))
      {
        detail::set_plain_old_pointer_to_null(other.px);
      }
      template<typename U>
      cloning
      (
        cloning<U> && other
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ): _clone_factory(std::move(other._clone_factory)), px(std::move(other.px))
      {
        detail::set_plain_old_pointer_to_null(other.px);
      }
#endif

      void swap(cloning & other)
      {
        boost::swap(px, other.px);
        boost::swap(_clone_factory, other._clone_factory);
      }

      cloning & operator=(const cloning & other)
      {
        cloning(other).swap(*this);
        return *this;
      }

      template<typename U>
      cloning & operator=(const cloning<U> & other)
      {
        cloning(other).swap(*this);
        return *this;
      }
#ifndef BOOST_NO_RVALUE_REFERENCES
      cloning & operator=(cloning && other)
      {
        cloning(std::move(other)).swap(*this);
        return *this;
      }
      template<typename U>
      cloning & operator=(cloning<U> && other)
      {
        cloning(std::move(other)).swap(*this);
        return *this;
      }
#endif
      void reset()
      {
        cloning().swap(*this);
      }
      template<typename U> void reset(U p)
      {
        cloning(p).swap(*this);
      }
      template<typename U, typename D> void reset(U p, D d)
      {
        cloning(p, d).swap(*this);
      }
      template<typename U, typename D, typename C> void reset(U p, D d, C c)
      {
        cloning(p, d, c).swap(*this);
      }

      pointer get() const {return px;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      pointer operator->() const
      {
        detail::assert_plain_old_pointer_not_null(px);
        return px;
      }

      reference operator*() const
      {
        detail::assert_plain_old_pointer_not_null(px);
        return *px;
      }

    private:
      clone_factory_type _clone_factory;
      pointer px;
    };

    template<typename T>
    T get_pointer(const cloning<T> &p)
    {
      return p.get();
    }

    // casts
    template<typename ToValueType, typename U>
    typename rebind<cloning<U>, ToValueType>::other static_pointer_cast
    (
      cloning<U> const & cp,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
      typedef typename rebind<cloning<U>, ToValueType>::other result_type;
      return result_type(cp, detail::static_cast_tag());
    }
    template<typename ToValueType, typename U>
    typename rebind<cloning<U>, ToValueType>::other const_pointer_cast
    (
      cloning<U> const & cp,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
      typedef typename rebind<cloning<U>, ToValueType>::other result_type;
      return result_type(cp, detail::const_cast_tag());
    }
    template<typename ToValueType, typename U>
    typename rebind<cloning<U>, ToValueType>::other dynamic_pointer_cast
    (
      cloning<U> const & cp,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
      typedef typename rebind<cloning<U>, ToValueType>::other result_type;
      return result_type(cp, detail::dynamic_cast_tag());
    }

    // comparisons
    template<class T, class U> inline bool operator==(cloning<T> const & a, cloning<U> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(cloning<T> const & a, cloning<U> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U> inline bool operator==(cloning<T> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U> inline bool operator!=(cloning<T> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U> inline bool operator==(T const & a, cloning<U> const & b)
    {
      return a == b.get();
    }
    template<class T, class U> inline bool operator!=(T const & a, cloning<U> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T> inline bool operator!=(cloning<T> const & a, cloning<T> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T> inline bool operator<(cloning<T> const & a, cloning<T> const & b)
    {
      return std::less<typename cloning<T>::pointer>()(a.get(), b.get());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_CLONING_HPP_INCLUDED
