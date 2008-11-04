#ifndef BOOST_INTROSPECTION_HPP
#define BOOST_INTROSPECTION_HPP

#include <iostream>
#include <vector>
#include <map>
#include <memory> 
#include <stdexcept> 
#include <boost/type_traits.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/intrusive_ptr.hpp> 
#include <boost/introspection/detail.hpp> 
#include <boost/mpl/vector.hpp> 
#include <boost/mpl/at.hpp> 
#include <boost/mpl/size.hpp> 
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#ifdef _WIN32
#pragma warning(disable: 4355)
#endif

namespace boost { namespace introspection {

class member_type_error : public std::runtime_error
{
public:
  member_type_error(const std::string& class_name, const std::string& member_name, const std::type_info& tid) :
    std::runtime_error("member " + class_name + "::" + member_name + " cannot be converted to " + tid.name()),
    class_(class_name), member_(member_name), tid_(tid) { }

  virtual ~member_type_error() throw () { }
    
private:
  std::string class_, member_;
  const std::type_info& tid_;
};

class member_not_found : public std::runtime_error
{
public:
  member_not_found(const std::string& class_name, const std::string& member_name) :
    std::runtime_error("member " + class_name + "::" + member_name + " not found"),
    class_(class_name), member_(member_name) { }
    
  virtual ~member_not_found() throw () { }

private:
  std::string class_, member_;
};

class type_not_found : public std::runtime_error
{
public:
  type_not_found(const std::string& name) :
    std::runtime_error("type " + name + " not found"),
    class_(name) { }
    
  virtual ~type_not_found() throw () { }

private:
  std::string class_;
};

class ambiguous : public std::runtime_error
{
public:
  ambiguous(const std::string& class_name, const std::string& member_name) :
    std::runtime_error("member " + class_name + "::" + member_name + " is ambiguous"),
    class_(class_name), member_(member_name) { }
    
  virtual ~ambiguous() throw () { }

private:
  std::string class_, member_;
};

#define raw_name name

class user_defined_type
{
public:
  user_defined_type(const std::type_info& ti)
  {
    id_map()[ti.raw_name()] = this;
  }
  
  virtual ~user_defined_type() { }

  template<class T>  
  static const T* by_id(const std::type_info& ti)
  {
    return get_typed<T>(ti.raw_name(), id_map());
  }

  template<class T>  
  static const T* by_name(const std::string& name)
  {
    return get_typed<T>(name, name_map());
  }
  
  void name(const std::string& n)
  {
    name_ = n;
    name_map()[n] = this;
  }

  std::string name() const { return name_; }
  
private:

  std::string name_;

  template<class T>  
  static const T* get_typed(const std::string& name, const std::map<std::string, user_defined_type*>& index)
  {
    if (const T* p = dynamic_cast<const T*>(get_untyped(name, index)))
      return p;
    else
      throw type_not_found(name);
  }

  static const user_defined_type* get_untyped(const std::string& name, const std::map<std::string, user_defined_type*>& index)
  {
    std::map<std::string, user_defined_type*>::const_iterator iter(index.find(name));
    return iter == index.end() ? 0 : iter->second;
  }


  static std::map<std::string, user_defined_type*>& id_map()
  {
    static std::map<std::string, user_defined_type*> the_map;
    return the_map;
  }
  
  static std::map<std::string, user_defined_type*>& name_map()
  {
    static std::map<std::string, user_defined_type*> the_map;
    return the_map;
  }
};

class class_type;
class object;
class const_object;

enum inheritance_flag {
  public_ = 0, protected_ = 1, private_ = 2, virtual_ = 4,
  default_inheritance_ = public_
};

struct direct_base
{
  const class_type* class_;
  int flags_;
  void* (*upcast_)(void*);
  bool is_virtual() const { return (flags_ & virtual_) != 0; }
};

typedef boost::shared_ptr< const std::vector<const direct_base*> > inheritance_path;

class member
{
public:
  member(const std::string& name)
    : name_(name) { }
    
  virtual ~member() { }
    
  const std::string& name() const { return name_; }
private:
  std::string name_;
};

template<int N>
struct fun_n;

#define BOOST_INTROSPECTION_DISPATCHER_SPECIALIZATION_PARAM(z, n, unused) \
  BOOST_PP_COMMA_IF(n) typename mpl::at_c<typename FUNCTION_TYPE::arglist_type, n>::type BOOST_PP_CAT(a, n)

#define BOOST_INTROSPECTION_MAX_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_INTROSPECTION_MAX_SIZE - 1)
#define BOOST_PP_FILENAME_1 "boost/introspection/detail/fun_n_spec.hpp"
#include BOOST_PP_ITERATE()

template<typename T>
class mem_var : public member
{
public:
  mem_var(const std::string& name)
    : member(name) { }
    
  virtual void* address(void* object) const = 0;
  const void* address(const void* object) const { return address(const_cast<void*>(object)); }
};

template<class C, typename T>  
class direct_mem_var : public mem_var<T>
{
public:
  direct_mem_var(const std::string& name, T (C::*pointer))
    : mem_var<T>(name), pointer_(pointer) { }
    
  virtual void* address(void* object) const
  {
    return &(reinterpret_cast<C*>(object)->*pointer_);
  }
  
private:
  T (C::*pointer_);
};

struct base
{
  base() : refs_(0) { }
  int refs_;
  const class_type* class_;
  bool virtual_;
  inheritance_path path_;
  std::vector< boost::intrusive_ptr<base> > bases_;
};

inline void intrusive_ptr_add_ref(base* p)
{
  ++p->refs_;
}

inline void intrusive_ptr_release(base* p)
{
  if (--p->refs_ == 0)
    delete p;
}

template<class MEMBER_CLASS>
class qualified_member
{
public:
  qualified_member(const MEMBER_CLASS* member) : path_(new std::vector<const class_type*>), member_(member) { }
  qualified_member(inheritance_path path, const MEMBER_CLASS* member) : path_(path), member_(member) { }
  
  inheritance_path path() const { return path_; }
  const MEMBER_CLASS* direct_member() const { return member_; }

private:
  inheritance_path path_;
  const MEMBER_CLASS* member_;
};

typedef std::vector< qualified_member<member> > qualified_member_vector;

template<typename RETURN, typename ARGLIST, typename IS_CONST>
struct function_type
{
  typedef RETURN return_type;
  typedef ARGLIST arglist_type;
  typedef IS_CONST const_type;
  enum { arglist_size = mpl::size<arglist_type>::value };
  typedef fun_n<arglist_size> fun_n_type;
  typedef typename fun_n_type::template free_fun<function_type> free_fun_type;
  typedef typename fun_n_type::template mem_fun<function_type> mem_fun_type;
  typedef qualified_member< typename fun_n_type::template mem_fun<function_type> > qualified_member_type;
};

template<typename T>
struct function_type_helper;

#define BOOST_PP_ITERATION_LIMITS (0, BOOST_INTROSPECTION_MAX_SIZE - 1)
#define BOOST_PP_FILENAME_1 "boost/introspection/detail/function_type_helper_spec.hpp"
#include BOOST_PP_ITERATE()

template<typename T>
struct member_function : public function_type_helper<T>::function_type::qualified_member_type
{
  typedef typename function_type_helper<T>::function_type function_type;
  typedef typename function_type::return_type return_type;
  typedef typename function_type::arglist_type arglist_type;
  typedef typename function_type::const_type const_type;
  typedef typename function_type::qualified_member_type qualified_member_type;
  
  member_function(object& o, const std::string& name);
  member_function(const const_object& o, const std::string& name);
};

class class_type : public user_defined_type
{
public:
  class_type(const std::type_info& ti) : user_defined_type(ti), collect_qualified_members_done_(false) { }
  
  void add(member* pm)
  {
    std::auto_ptr<member> track(pm);
    direct_members_.reserve(direct_members_.size() + 1);
    direct_member_map_[pm->name()] = pm;
    direct_members_.push_back(pm);
    track.release();
  }

  const qualified_member_vector& candidates(const std::string& member_name) const;
  
  template<typename T>
  qualified_member< mem_var<T> > member_variable(const std::string& var_name) const
  {
    const qualified_member_vector& vars(candidates(var_name));
        
    if (vars.size() > 1)
      throw ambiguous(name(), var_name);

    const mem_var<T>* pm(dynamic_cast<const mem_var<T>*>(vars.front().direct_member()));
    
    if (!pm)
      throw member_type_error(name(), var_name, typeid(T));
      
    return qualified_member< mem_var<T> >(vars.front().path(), pm);
  }
  
  template<typename FUNCTION_TYPE>
  typename FUNCTION_TYPE::qualified_member_type
  member_function(const std::string& function_name) const
  {
    const qualified_member_vector& functions(candidates(function_name));
    
    qualified_member_vector::const_iterator iter(functions.begin()), last(functions.end());
    
    typedef typename FUNCTION_TYPE::mem_fun_type mem_fun_type;
    
    while (iter != last)
    {
      if (const mem_fun_type* pm = dynamic_cast<const mem_fun_type*>(iter->direct_member()))
        return typename FUNCTION_TYPE::qualified_member_type(iter->path(), pm);
        
      ++iter;
    }
    
    throw member_not_found(name(), function_name);
  }
  
  template<typename FUNCTION_TYPE>
  typename FUNCTION_TYPE::free_fun_type::function_pointer_type
  static_function(const std::string& function_name) const
  {
    const qualified_member_vector& functions(candidates(function_name));
    qualified_member_vector::const_iterator iter(functions.begin()), last(functions.end());
    typedef typename FUNCTION_TYPE::free_fun_type free_fun_type;
    
    while (iter != last)
    {
      if (const free_fun_type* fun = dynamic_cast<const free_fun_type*>(iter->direct_member()))
        return fun->function();
        
      ++iter;
    }
    
    throw member_not_found(name(), function_name);
  }
    
  void* adjust(const inheritance_path& path, void* address) const;
  
  const void* adjust(const inheritance_path& path, const void* address) const
  { return adjust(path, const_cast<void*>(address)); }

  void dump_inheritance_graph(std::ostream& os) const;
  void dump_inheritance_tree(std::ostream& os) const;
  
private:

  void build_inheritance_graph() const;
  void build_inheritance_graph(class_type* complete_class, std::vector< boost::intrusive_ptr<base> > & bases,
    std::vector<const direct_base*>& path, std::map<const class_type*, boost::intrusive_ptr<base> >& virtual_bases) const;
  static void dump_inheritance_tree(std::ostream& os, int indent, const std::vector< intrusive_ptr<base> >& bases);
  static void dump_inheritance_graph_bases(std::ostream& os, const std::vector< intrusive_ptr<base> >& bases);
    
  void collect_qualified_members() const;

  detail::auto_vector_ptr<direct_base> direct_bases_;
  detail::auto_vector_ptr<member> direct_members_;
  std::map<std::string, member*> direct_member_map_;
  mutable std::vector< boost::intrusive_ptr<base> > bases_;
  mutable std::vector< qualified_member<member> > qualified_members_;
  mutable std::map<std::string, std::vector< qualified_member<member> > > qualified_member_map_;
  mutable bool collect_qualified_members_done_;
  
  template<class C> friend class introspection_class;
};

class value
{
};

template<typename T>
struct member_variable;

template<typename T>
struct member_function;

template<typename OBJECT, typename FUNCTION>
struct bound_member_function;

class object : public value
{
public:
  template<class T>
  object(T* static_pointer)
    : address_(static_pointer), class_(user_defined_type::by_id<class_type>(typeid(T))) { }
  
  object(const class_type* cls, void* address)
    : class_(cls), address_(address) { }
    
  object(const std::string& class_name)
    : class_(user_defined_type::by_name<class_type>(class_name)),
      address_(class_->static_function< function_type_helper<void* (*)()>::function_type >("")()) { }
    
  object(const char* class_name)
    : class_(user_defined_type::by_name<class_type>(class_name)),
      address_(class_->static_function< function_type_helper<void* (*)()>::function_type >("")()) { }

  template<typename T>
  T& operator [](const member_variable<T>& var);

  template<typename T>
  bound_member_function< object, member_function<T> > operator [](const member_function<T>& fun);
  
  template<typename T>
  bound_member_function< object, member_function<T> > function(const std::string& name);
  
  const class_type* type() const { return class_; }
  void* address() const { return address_; }

private:
  const class_type* class_;
  void* address_;
};

class const_object
{
public:
  template<class T>
  const_object(const T* static_pointer)
    : address_(static_pointer), class_(user_defined_type::by_id<class_type>(typeid(T))) { }
  
  const_object(const class_type* cls, void* address) : class_(cls), address_(address) { }
  
  const_object(const object& obj) : class_(obj.type()), address_(obj.address()) { }

  template<typename T>
  const T& operator [](const member_variable<T>& var) const;

  template<typename T>
  typename enable_if<
    typename member_function<T>::const_type,
    bound_member_function< const_object, member_function<T> >
  >::type
  operator [](const member_function<T>& fun) const;
  
  template<typename T>
  typename enable_if<
    typename member_function<T>::const_type,
    bound_member_function< const_object, member_function<T> >
  >::type
  function(const std::string& name) const;
  
  const class_type* type() const { return class_; }
  
  const void* address() const { return address_; }

private:
  void* address_;
  const class_type* class_;
  bool is_const;
};

template<typename T>
member_function<T>::member_function(object& o, const std::string& name)
  : qualified_member_type(o.type()->class_type::member_function<typename function_type_helper<T>::function_type>(name)) { }

template<typename T>
member_function<T>::member_function(const const_object& o, const std::string& name)
  : qualified_member_type(o.type()->class_type::member_function<function_type>(name)) { }

template<typename T>
struct member_variable : public qualified_member< mem_var<T> >
{
  member_variable(object& o, const std::string& name)
    : qualified_member< mem_var<T> >(o.type()->class_type::member_variable<T>(name)) { }

  member_variable(const_object& o, const std::string& name)
    : qualified_member< mem_var<T> >(o.type()->class_type::member_variable<T>(name)) { }
};

template<typename T>
T& object::operator [](const member_variable<T>& var)
{
  return *reinterpret_cast<T*>(var.direct_member()->address(class_->adjust(var.path(), address_)));
}

template<typename T>
const T& const_object::operator [](const member_variable<T>& var) const
{
  return *reinterpret_cast<T*>(var.direct_member()->address(class_->adjust(var.path(), address_)));
}

template<typename OBJECT, typename FUNCTION>
struct bound_member_function
{
  typedef typename FUNCTION::function_type::return_type return_type;
  
  bound_member_function(const OBJECT* po, FUNCTION member_function) : object_(po), function_(member_function) { }
  
  const OBJECT* object_;
  FUNCTION function_;

  return_type operator ()() const
  {
    return function_.direct_member()->call(
      object_->type()->adjust(function_.path(), const_cast<void*>(object_->address())));
  }

  #define BOOST_INTROSPECTION_MAX_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE
  #define BOOST_PP_ITERATION_LIMITS (1, BOOST_INTROSPECTION_MAX_SIZE - 1)
  #define BOOST_PP_FILENAME_1 "boost/introspection/detail/object_call_spec.hpp"
  #include BOOST_PP_ITERATE()
};

template<typename T>
bound_member_function< object, member_function<T> > object::operator [](const member_function<T>& fun)
{
  return bound_member_function<object, member_function<T> >(this, fun);
}

template<typename T>
bound_member_function< object, member_function<T> > object::function(const std::string& name)
{
  return (*this)[member_function<T>(*this, name)];
}

template<typename T>
typename enable_if<
  typename member_function<T>::const_type,
  bound_member_function< const_object, member_function<T> >
>::type
const_object::operator [](const member_function<T>& fun) const
{
  return bound_member_function< const_object, member_function<T> >(this, fun);
}

template<typename T>
typename enable_if<
  typename member_function<T>::const_type,
  bound_member_function< const_object, member_function<T> >
>::type
const_object::function(const std::string& name) const
{
  return (*this)[member_function<T>(*this, name)];
}

template<class T>
object downcast(const T* static_pointer)
{
  return const_object(
    user_defined_type::by_id<class_type>(typeid(*static_pointer)),
    introspection_class<typename boost::remove_cv<T>::type>::base_address(static_pointer));
}

template<class T>
object downcast(T* static_pointer)
{
  return object(
    user_defined_type::by_id<class_type>(typeid(*static_pointer)),
    introspection_class<typename boost::remove_cv<T>::type>::base_address(static_pointer));
}

// =========================================================

template<class C>
class introspection_class
{
  public:
  
    static class_type* instance()
    {
      static class_type cl(typeid(C));
      return &cl;
    }

  static typename enable_if<is_polymorphic<C>, void*>::type base_address(C* obj)
    {
      return dynamic_cast<void*>(obj);
    }

    static const void* base_address(const C* obj)
    {
      return base_address(const_cast<C*>(obj), is_polymorphic<C>::type());
    }
    
    static void name(const std::string& name)
    {
      instance()->name(name);
    }
  
    template<typename T>
    static void variable(const std::string& name, T (C::*pointer))
    {
      instance()->add(new direct_mem_var<C, T>(name, pointer));
    }
  
    template<typename T>
    static void function(const std::string& name, T pointer)
    {
      typedef typename function_type_helper<T>::function_type function_type;
      instance()->add(function_type::fun_n_type::
        template make_direct_mem_fun<C, T, function_type>(name, pointer));
    }
  
    template<typename T>
    static void constructor()
    {
      typedef typename function_type_helper<T>::function_type function_type;
      typedef typename function_type::fun_n_type fun_n_type;
      instance()->add(fun_n_type::template make_free_fun<T, function_type>("", fun_n_type::template make<C, function_type>));
    }
  
    template<typename B>
    static void base(inheritance_flag flag1 = default_inheritance_, inheritance_flag flag2 = default_inheritance_)
    {
      std::auto_ptr<direct_base> base(new direct_base);
      base->class_ = introspection_class<B>::instance();
      base->flags_ = static_cast<int>(flag1) | static_cast<int>(flag2);
      struct local { static void* upcast(void* p) { return static_cast<B*>(reinterpret_cast<C*>(p)); } };
      base->upcast_ = local::upcast;
      instance()->direct_bases_.push_back(base.get());
      direct_base* p = instance()->direct_bases_.front();
      base.release();
    }
};

#define BOOST_INTROSPECTION_MAX_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE
#define BOOST_PP_ITERATION_LIMITS (1, BOOST_INTROSPECTION_MAX_SIZE - 1)
#define BOOST_PP_FILENAME_1 "boost/introspection/detail/make_spec.hpp"
#include BOOST_PP_ITERATE()

} }

// =========================================================

#define BOOST_INTROSPECTION_CLASS(_cls) \
  introspection_class<_cls>::name(#_cls)

#define BOOST_INTROSPECTION_BASE(_cls, _base) \
  introspection_class<_cls>::base<_base>()

#define BOOST_INTROSPECTION_VIRTUAL_BASE(_cls, _base) \
  introspection_class<_cls>::base<_base>(virtual_);

#define BOOST_INTROSPECTION_DEFAULT_CONSTRUCTOR(_cls) \
  introspection_class<_cls>::constructor<void* (*)()>();

#define BOOST_INTROSPECTION_CONSTRUCTOR_1(_cls, _type1) \
  introspection_class<_cls>::constructor<void* (*)(_type1)>();
  
#define BOOST_INTROSPECTION_MEMBER_VARIABLE(cls, name) \
  introspection_class<cls>::variable(#name, &cls::name)

#define BOOST_INTROSPECTION_MEMBER_FUNCTION(cls, name) \
  introspection_class<cls>::function(#name, &cls::name)

#define BOOST_INTROSPECTION_OVERLOADED_MEMBER_FUNCTION(_cls, _name, _type) \
  introspection_class<_cls>::function<_type>(#_name, &_cls::_name)

#endif // BOOST_INTROSPECTION_HPP
