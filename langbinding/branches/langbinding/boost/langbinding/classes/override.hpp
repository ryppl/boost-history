// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef OVERRIDE_DWA2004922_HPP
# define OVERRIDE_DWA2004922_HPP

# include <boost/langbinding/util/type_id.hpp>
# include <boost/langbinding/backend/call_xxx_data.hpp>
# include <boost/langbinding/backend/plugin.hpp>
# include <boost/implicit_cast.hpp>
# include <boost/aligned_storage.hpp>
# include <boost/type_traits/alignment_of.hpp>
# include <boost/mpl/vector/vector10.hpp>
# include <boost/mpl/for_each.hpp>
# include <boost/mpl/size.hpp>
# include <boost/mpl/int.hpp>
# include <boost/mpl/min_max.hpp>
# include <algorithm>

namespace boost { namespace langbinding { namespace classes { 

namespace aux
{
  struct write_id
  {
      write_id(util::type_info* types)
        : types(types) {}
      
      template <class T>
      void operator()(mpl::identity<T>)
      {
          *++types = util::type_id<T>();
      }
   private:
      util::type_info* types;
  };

  template <class Result, class Args>
  struct call_xxx_data : backend::call_xxx_data
  {
      static std::size_t const arity = mpl::size<Args>::value;

      static call_xxx_data instance;

   private:
      call_xxx_data()
        : backend::call_xxx_data(arity, this->types)
      {
          types[0] = util::type_id<Result>();
          write_id processor(types);
          mpl::for_each<Args, mpl::identity<mpl::_> >(processor);
      }

      util::type_info types[mpl::size<Args>::value + 1];
  };

  template <class Result, class Args>
  call_xxx_data<Result,Args> call_xxx_data<Result,Args>::instance;

  template <class Args>
  struct proxy
  {
      typedef void* arg_array[
          mpl::max<mpl::size<Args>,mpl::int_<1> >::type::value
      ];

      proxy(backend::plugin const& back_end, void* impl, arg_array const& args)
        : back_end(back_end)
        , impl(impl)
      {
          std::copy(args, args + sizeof(args)/sizeof(*args), this->args);
      }

      template <class T>
      operator T() const
      {
          boost::aligned_storage<sizeof(T), boost::alignment_of<T>::value> storage;
          
          return back_end.call(
              impl
            , call_xxx_data<T,Args>::instance
            , storage.address()
            , this->args);
      }

      template <class T>
      operator T*() 
      {
          return &implicit_cast<T&>(*this); // dispatch to the T& case below.
      }

      template <class T>
      operator T&() const volatile
      {
          return *static_cast<T*>(
              back_end.call(
                  impl
                , call_xxx_data<typename remove_cv<T>::type,Args>::instance
                , 0     // No need to store the result
                , const_cast<void* const*>(this->args))
          );
      }

   private:
      backend::plugin const& back_end;
      void* impl;
      arg_array args;
  };
}

// This class is movable but not copyable.  Preventing copies saves us
// from going through indirect calls for incrementing and decrementing
// reference counts when it is returned.
struct override
{
    typedef void* const override::* safe_bool;
 public:
    
    operator safe_bool() const
    {
        return this->impl == 0 ? 0 : &override::impl;
    }
    
    override(backend::plugin const& back_end, backend::override const& x)
      : back_end(back_end)
      , impl(x.data)
      , destroy(*x.destroy)
    {}
 
    ~override()
    {
        if (this->impl)
            this->destroy(this->impl);
    }

    aux::proxy<mpl::vector0<> > operator()() const
    {
        void* const dummy[] = { 0 };
        return aux::proxy<mpl::vector0<> >(back_end, impl, dummy);
    }

    template <class T0>
    aux::proxy<mpl::vector1<T0> > operator()(T0 const& a0) const
    {
        void* const addresses[] = { &a0 };
        return aux::proxy<mpl::vector1<T0> >(back_end, impl, addresses);
    }
    
    template <class T0, class T1>
    aux::proxy<mpl::vector2<T0,T1> > operator()(T0 const& a0, T1 const& a1) const
    {
        void* const addresses[] = { &a0, &a1 };
        return aux::proxy<mpl::vector2<T0,T1> >(back_end, impl, addresses);
    }

    // ...
    
 private: // movable/noncopyable support
    override(override&);
    override& operator=(override&);
    
    struct rvalue
    {
        rvalue(override* source) : source(source) {}
        
        override* source;
    };
    
 public: // movable/noncopyable support
    operator rvalue() { return rvalue(this); }

    override(rvalue rhs)
      : back_end(rhs.source->back_end)
      , impl(rhs.source->impl)
      , destroy(rhs.source->destroy)
    {
        rhs.source->impl = 0;
    }
    
 private: // data members
    backend::plugin const& back_end;
    void* impl;
    void (&destroy)(void*);
};

}}} // namespace boost::langbinding::classes

#endif // OVERRIDE_DWA2004922_HPP
