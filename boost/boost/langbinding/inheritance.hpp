// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Copyright (c) 2003 Daniel Wallin

#ifndef INHERITANCE_DWA200216_HPP
# define INHERITANCE_DWA200216_HPP

# include <boost/langbinding/config.hpp>
# include <boost/langbinding/type_id.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/mpl/if.hpp>
# include <boost/type_traits/object_traits.hpp>
# include <boost/type_traits/is_polymorphic.hpp>
# include <boost/detail/workaround.hpp>

namespace boost { namespace langbinding {

   typedef type_info class_id;

   // Types used to get address and id of most derived type
   typedef std::pair<void*,class_id> dynamic_id_t;
   typedef dynamic_id_t (*dynamic_id_function)(void*);

   class BOOST_LANGBINDING_DECL inheritance_graph_base
   {
   public: 
      inheritance_graph_base(const inheritance_graph_base&);
      inheritance_graph_base& operator=(const inheritance_graph_base&);

      void register_dynamic_id(
            class_id static_id, dynamic_id_function get_dynamic_id);

      void add_cast(class_id src_t, class_id dst_t, 
            void*(*cast)(void*), bool is_downcast);

      void* find_static_type(void* p, type_info src, type_info dst) const;
      void* find_dynamic_type(void* p, type_info src, type_info dst) const;

      void link_with(inheritance_graph_base& that);

   protected:
      inheritance_graph_base();
      virtual ~inheritance_graph_base();

   private:
      struct implementation;
      boost::shared_ptr<implementation> m_pimpl;
   };

   class inheritance_graph : public inheritance_graph_base
   {
   public:
      static inheritance_graph& instance();

   private:
      inheritance_graph() {}
   };

//
// a generator with an execute() function which, given a source type
// and a pointer to an object of that type, returns its most-derived
// /reachable/ type identifier and object pointer.
//

// first, the case where T has virtual functions
template <class T>
struct polymorphic_id_generator
{
    static dynamic_id_t execute(void* p_)
    {
        T* p = static_cast<T*>(p_);
        return std::make_pair(dynamic_cast<void*>(p), 
              dynamic_type_id(p));
    }
};

// now, the non-polymorphic case.
template <class T>
struct non_polymorphic_id_generator
{
    static dynamic_id_t execute(void* p_)
    {
        return std::make_pair(p_, static_type_id<T>());
    }
};

// Now the generalized selector
template <class T>
struct dynamic_id_generator
  : mpl::if_<
        is_polymorphic<T>
        , polymorphic_id_generator<T>
        , non_polymorphic_id_generator<T>
    >
{};

// Register the dynamic id function for T with the type-conversion
// system.
template <class T>
void register_dynamic_id(T* = 0)
{
    typedef typename dynamic_id_generator<T>::type generator;
    inheritance_graph::instance().register_dynamic_id(
        static_type_id<T>(), &generator::execute);
}

//
// a generator with an execute() function which, given a void*
// pointing to an object of type Source will attempt to convert it to
// an object of type Target.
//

template <class Source, class Target>
struct dynamic_cast_generator
{
    static void* execute(void* source)
    {
        return dynamic_cast<Target*>(
            static_cast<Source*>(source));
    }
};

template <class Source, class Target>
struct implicit_cast_generator
{
    static void* execute(void* source)
    {
        Target* result = static_cast<Source*>(source);
        return result;
    }
};

template <class Source, class Target>
struct cast_generator
  : mpl::if_<
        is_base_and_derived<Target,Source>
      , implicit_cast_generator<Source,Target>
      , dynamic_cast_generator<Source,Target>
    >
{
};

template <class Source, class Target>
inline void register_conversion(
    bool is_downcast = ::boost::is_base_and_derived<Source,Target>::value
    // These parameters shouldn't be used; they're an MSVC bug workaround
    , Source* = 0, Target* = 0)
{
    typedef typename cast_generator<Source,Target>::type generator;

    inheritance_graph::instance().add_cast(
        static_type_id<Source>()
      , static_type_id<Target*>()
      , &generator::execute
      , is_downcast
    );
}

}} // namespace boost::langbinding

#endif // INHERITANCE_DWA200216_HPP
