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
#include <boost/generic_ptr/detail/util.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  namespace generic_ptr
  {
    template<typename T>
    T* new_clone(T *p)
    {
      if(p == 0) return 0;
      using boost::new_clone;
      return new_clone(*p);
    }
    template<typename GenericPointer>
    GenericPointer new_clone(const GenericPointer &p, typename pointer_traits<GenericPointer>::value_type * = 0)
    {
      return GenericPointer(new_clone(get_pointer(p)));
    }

    namespace detail
    {
      class clone_factory_impl_base
      {
      public:
        virtual ~clone_factory_impl_base() {}
        //FIXME: this isn't adequate.  We need to return a generic pointer to void instead
        virtual void * get_pointer() = 0;
        virtual clone_factory_impl_base* make_clone() = 0;
      };

      template<typename T>
      class clone_factory_impl: public clone_factory_impl_base
      {
      public:
        explicit clone_factory_impl(T *p): px(p)
        {}
        ~clone_factory_impl()
        {
          delete_clone(px);
        }
        virtual void * get_pointer() { return px; }
        virtual clone_factory_impl* make_clone()
        {
          if(px == 0) return new clone_factory_impl(0);
          return new clone_factory_impl(new_clone(px));
        }
      private:
        T * px;
      };

      template<typename GenericPointer, typename Deleter, typename Cloner>
      class clone_factory_pdc_impl: public clone_factory_impl_base
      {
      public:
        clone_factory_pdc_impl(GenericPointer p, Deleter d, Cloner c): px(p), deleter(d), cloner(c)
        {}
        ~clone_factory_pdc_impl()
        {
          deleter(px);
        }
        virtual void * get_pointer() { return get_plain_old_pointer(px); }
        virtual clone_factory_pdc_impl* make_clone()
        {
          return new clone_factory_pdc_impl(cloner(px), deleter, cloner);
        }
      private:
        GenericPointer px;
        Deleter deleter;
        Cloner cloner;
      };

      class clone_factory
      {
      public:
        clone_factory(): _impl()
        {}
        template<typename T>
        explicit clone_factory(T * p): _impl(new clone_factory_impl<T>(p))
        {}
        template<typename T, typename Deleter, typename Cloner>
        clone_factory(T p, Deleter d, Cloner c): _impl(new clone_factory_pdc_impl<T, Deleter, Cloner>(p, d, c))
        {}
        clone_factory(const clone_factory &other): _impl(other._impl->make_clone())
        {}
#ifndef BOOST_NO_RVALUE_REFERENCES
        clone_factory(clone_factory && other)
        {
          swap(other);
        }
#endif
        void * get_pointer()
        {
          if(_impl.get() == 0) return 0;
          return _impl->get_pointer();
        }
        void swap(clone_factory &other)
        {
          boost::swap(_impl, other._impl);
        }
      private:
        clone_factory& operator=(const clone_factory &);  // could be implemented and made public if we needed it

        scoped_ptr<clone_factory_impl_base> _impl;
      };
      void swap(clone_factory &a, clone_factory &b)
      {
        a.swap(b);
      }
    }

    class default_cloning_deleter
    {
    public:
      template<typename GenericPointer>
      void operator()(const GenericPointer &p) const
      {
        delete_clone(get_plain_old_pointer(p));
      }
    };

    class default_cloner
    {
    public:
      template<typename GenericPointer>
      GenericPointer operator()(const GenericPointer & p) const
      {
        if(get_plain_old_pointer(p) == 0) return p;
        return new_clone(p);
      }
    };

    template<typename T>
    class cloning
    {
      typedef cloning this_type; // for detail/operator_bool.hpp
      template<typename U>
      friend class cloning;
    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;

      template<typename ValueType>
      struct rebind
      {
        typedef cloning<typename generic_ptr::rebind<pointer, ValueType>::other> other;
      };

      cloning(): _cloner(), px()
      {}
      template<typename U>
      cloning( U p ): _cloner(get_plain_old_pointer(p)), px( p )
      {}
      template<typename U, typename Deleter, typename Cloner = default_cloner>
      cloning(U p, Deleter d, Cloner c = default_cloner()): _cloner(p, d, c), px( p )
      {}
      cloning(const cloning & other):
        _cloner(other._cloner),
        px
        (
          static_cast<value_type *>(_cloner.get_pointer())
        )
      {}
      template<typename U>
      cloning(const cloning<U> & other):
        _cloner(other._cloner),
        px
        (
          static_cast
          <
            typename pointer_traits<U>::value_type *
          >(_cloner.get_pointer())
        )
      {}

      // casts
      template<typename U>
      cloning(const cloning<U> & other, detail::static_cast_tag):
        _cloner(other._cloner),
        px
        (
          static_cast
          <
            value_type *
          >
          (
            static_cast
            <
              typename pointer_traits<U>::value_type *
            >(_cloner.get_pointer())
          )
        )
      {}
      template<typename U>
      cloning(const cloning<U> & other, detail::const_cast_tag):
        _cloner(other._cloner),
        px
        (
          const_cast
          <
            value_type *
          >
          (
            static_cast
            <
              typename pointer_traits<U>::value_type *
            >(_cloner.get_pointer())
          )
        )
      {}
      template<typename U>
      cloning(const cloning<U> & other, detail::dynamic_cast_tag):
        _cloner(other._cloner),
        px
        (
          dynamic_cast
          <
            value_type *
          >
          (
            static_cast
            <
              typename pointer_traits<U>::value_type *
            >(_cloner.get_pointer())
          )
        )
      {
        // reset _cloner if dynamic cast failed
        if(get_plain_old_pointer(px) == 0)
        {
          detail::clone_factory().swap(_cloner);
        }
      }

#ifndef BOOST_NO_RVALUE_REFERENCES
      cloning(cloning && other): _cloner(std::move(other._cloner)), px(std::move(other.px))
      {}
      template<typename U>
      cloning(cloning<U> && other): _cloner(std::move(other._cloner)), px(std::move(other.px))
      {}
#endif

      void swap(cloning & other)
      {
        boost::swap(px, other.px);
        boost::swap(_cloner, other._cloner);
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

      pointer get() const {return px;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      pointer operator->() const
      {
        return px;
      }

      reference operator*() const
      {
        return *px;
      }

    private:
      detail::clone_factory _cloner;
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
