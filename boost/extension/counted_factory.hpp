/*
 * Boost.Extension / counted factory:
 *         factory to register the implementations and create them 
 *        (with a reference count)
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_COUNTED_FACTORY_HPP
#define BOOST_EXTENSION_COUNTED_FACTORY_HPP
#include <string>

#ifdef BOOST_EXTENSION_USE_PP

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>

#ifndef BOOST_EXTENSION_MAX_FUNCTOR_PARAMS
#define BOOST_EXTENSION_MAX_FUNCTOR_PARAMS 6
#endif

#endif

namespace boost{namespace extensions{

#ifdef BOOST_EXTENSION_USE_PP

/// Declaration of functor class template.
template <class Interface, class Info,
    BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC(\
                                        BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                                        class Param, void)>\
                                             class counted_factory;
/**/


#define BOOST_EXTENSION_COUNTED_FACTORY_CLASS(Z, N, _) \
template<class Interface, class Info BOOST_PP_COMMA_IF(N) \
         BOOST_PP_ENUM_PARAMS(N, class Param) > \
class counted_factory<Interface, Info BOOST_PP_COMMA_IF(N) \
              BOOST_PP_ENUM_PARAMS(N, Param) > { \
protected: \
  int * counter_; \
  std::string library_; \
  class generic_factory_function \
  { \
  public: \
    virtual ~generic_factory_function(){} \
    virtual Interface * operator()(int * counter \
             BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param) ) = 0; \
    virtual generic_factory_function * copy() const = 0; \
  }; \
  template <class T> \
  class factory_function : public generic_factory_function \
  { \
  public: \
    class counted_object : public T \
    { \
    private: \
      int * counter_; \
    public: \
      counted_object(int * counter BOOST_PP_COMMA_IF(N) \
                     BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
                             : T(BOOST_PP_ENUM_PARAMS(N, p)), \
                               counter_(counter) \
      { \
        ++(*counter_); \
      } \
      ~counted_object() \
      { \
        --(*counter_); \
      } \
    }; \
    virtual ~factory_function(){} \
    virtual Interface * operator() \
      (int * counter BOOST_PP_COMMA_IF(N) \
       BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
    { \
      return static_cast<Interface*>(new counted_object(counter \
                                     BOOST_PP_COMMA_IF(N) \
                                     BOOST_PP_ENUM_PARAMS(N,p)));} \
    virtual generic_factory_function * copy() const { \
      return new factory_function<T>; \
    } \
  }; \
  std::auto_ptr<generic_factory_function> factory_func_ptr_; \
  Info info_; \
public: \
  void set_library(const char * library_name) \
  { \
    library_ = library_name; \
  } \
  void set_counter(int * counter) \
  { \
    counter_ = counter; \
  } \
  const char * library() \
  { \
    return library_.c_str(); \
  } \
  template <class Actual> \
    void set_type_special(Actual *){ \
    factory_func_ptr_.reset(new factory_function<Actual>());} \
  template <class Actual> \
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();} \
  counted_factory(Info info) \
    :factory_func_ptr_(0), \
    info_(info) \
  {} \
  counted_factory(const counted_factory & first) \
    :counter_(first.counter_), \
    library_(first.library_), \
    factory_func_ptr_(first.factory_func_ptr_->copy()), \
    info_(first.info_) \
                       {} \
  Interface * operator()(int * counter BOOST_PP_COMMA_IF(N) \
                         BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
  {return create(counter BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, p));} \
  Interface * create(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return (*factory_func_ptr_) (counter_ BOOST_PP_COMMA_IF(N) \
                                 BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
  Info & get_info(){return info_;} \
};
/**/

/// Functor template specializations.
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_COUNTED_FACTORY_CLASS, _)


#undef BOOST_EXTENSION_COUNTED_FACTORY_CLASS
#else
  template <class Interface, class Info, class Param1 = void, 
            class Param2 = void, class Param3 = void, class Param4 = void, 
            class Param5 = void, class Param6 = void>
class counted_factory
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1, Param2, Param3, 
                                   Param4, Param5, Param6) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                     Param4 p4, Param5 p5, Param6 p6) 
        : T(p1, p2, p3, p4, p5, p6),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, 
       Param6 p6)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1, p2, p3, 
                                                        p4, p5, p6));}
    virtual generic_factory_function * copy() const 
    {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
  void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                         Param4 p4, Param5 p5, Param6 p6)
    {return create(counter, p1, p2, p3, p4, p5, p6);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, 
                     Param6 p6){return (*factory_func_ptr_)
  (counter_, p1, p2, p3, p4, p5, p6);}
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3, class Param4, class Param5>
class counted_factory<Interface, Info, Param1, Param2, Param3, Param4, Param5>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1, Param2, Param3, 
                                   Param4, Param5) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                     Param4 p4, Param5 p5) : T(p1, p2, p3, p4, p5),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1, p2, p3, 
                                                        p4, p5));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
    void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());}
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                         Param4 p4, Param5 p5)
    {return create(counter, p1, p2, p3, p4, p5);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
    return (*factory_func_ptr_) (counter_, p1, p2, p3, p4, p5);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3, class Param4>
class counted_factory<Interface, Info, Param1, Param2, Param3, Param4>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1, Param2, Param3, 
                                   Param4) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                     Param4 p4) : T(p1, p2, p3, p4),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1, p2, p3, 
                                                        p4));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
  void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1, Param2 p2, Param3 p3, 
                         Param4 p4)
    {return create(counter, p1, p2, p3, p4);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
    return (*factory_func_ptr_) (counter_, p1, p2, p3, p4);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3>
class counted_factory<Interface, Info, Param1, Param2, Param3>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1, Param2, Param3) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1, Param2 p2, Param3 p3) 
        : T(p1, p2, p3),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1, Param2 p2, Param3 p3)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1, p2, p3));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
    void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1, Param2 p2, Param3 p3)
    {return create(counter, p1, p2, p3);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3){
    return (*factory_func_ptr_) (counter_, p1, p2, p3);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2>
class counted_factory<Interface, Info, Param1, Param2>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1, Param2) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1, Param2 p2) : T(p1, p2),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1, Param2 p2)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1, p2));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
    void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1, Param2 p2)
    {return create(counter, p1, p2);}
  Interface * create(Param1 p1, Param2 p2){return (*factory_func_ptr_)
  (counter_, p1, p2);}
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1>
class counted_factory<Interface, Info, Param1>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter, Param1) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter, Param1 p1) : T(p1),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter, Param1 p1)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter, p1));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
    void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter, Param1 p1)
    {return create(counter, p1);}
  Interface * create(Param1 p1){return (*factory_func_ptr_)
  (counter_, p1);}
  Info & get_info(){return info_;}
};
template <class Interface, class Info>
class counted_factory<Interface, Info>
{
protected:
  int * counter_;
  std::string library_;
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(int * counter) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    class counted_object : public T
    {
    private:
      int * counter_;
    public:
      counted_object(int * counter) : T(),
      counter_(counter)
      {
        ++(*counter_);
      }
      ~counted_object()
      {
        --(*counter_);
      }  
    };
    virtual ~factory_function(){}
    virtual Interface * operator()
      (int * counter)
    {  // A compilation error here usually indicates that the
       // class you are adding is not derived from the base class
       // that you indicated.
      return static_cast<Interface*>(new counted_object(counter));}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  void set_library(const char * library_name)
  {
    library_ = library_name;
  }
  void set_counter(int * counter)
  {
    counter_ = counter;
  }
  const char * library()
  {
    return library_.c_str();
  }
  template <class Actual>
    void set_type_special(Actual *){
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  counted_factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  counted_factory(const counted_factory & first)
    :counter_(first.counter_),
    library_(first.library_),
    factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(int * counter)
    {return create(counter);}
  Interface * create(){return (*factory_func_ptr_)
  (counter_);}
  Info & get_info(){return info_;}
};
#endif // BOOST_EXTENSION_USE_PP
}}
#endif
