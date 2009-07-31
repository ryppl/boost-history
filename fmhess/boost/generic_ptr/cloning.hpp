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
    //FIXME: add similar delete_clone functions

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

      template<typename GenericPointer, typename Deleter, typename Cloner>
      class clone_factory_pdc_impl: public
        clone_factory_impl_base
        <
          typename rebind<GenericPointer, void>::other
        >
      {
      public:
        clone_factory_pdc_impl(GenericPointer p, Deleter d, Cloner c): px(p), deleter(d), cloner(c)
        {}
        ~clone_factory_pdc_impl()
        {
          deleter(px);
        }
        virtual typename rebind<GenericPointer, void>::other get_pointer()
        {
          //FIXME: remove const from px's value_type
          return px;
        }
        virtual clone_factory_pdc_impl* make_clone()
        {
          return new clone_factory_pdc_impl(cloner(px), deleter, cloner);
        }
      private:
        GenericPointer px;
        Deleter deleter;
        Cloner cloner;
      };

      template<typename GenericVoidPointer>
      class clone_factory
      {
      public:
        clone_factory(): _impl()
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
      cloning( U p ):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          default_cloning_deleter(),
          default_cloner()
        ),
        px
        (
          static_pointer_cast
          <
            typename pointer_traits<U>::value_type
          >(_clone_factory.get_pointer())
        )
      {}
      template<typename U, typename Deleter>
      cloning(U p, Deleter d):
        _clone_factory
        (
          typename generic_ptr::rebind<T, typename pointer_traits<U>::value_type>::other(p),
          d,
          default_cloner()
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
          d,
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
      cloning(const cloning<U> & other):
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
      cloning(cloning<U> && other): _clone_factory(std::move(other._clone_factory)), px(std::move(other.px))
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
