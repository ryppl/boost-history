/*
 * Boost.Extension / factory:
 *         factory to register the implementations and create them
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_FACTORY_HPP
#define BOOST_EXTENSION_FACTORY_HPP

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
                                        class Param, void)> class factory;



#define BOOST_EXTENSION_FACTORY_CLASS(Z, N, _) \
template<class Interface, class Info BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, \
         class Param) > \
class factory<Interface, Info BOOST_PP_COMMA_IF(N) \
              BOOST_PP_ENUM_PARAMS(N, Param) > { \
  protected: \
    class generic_factory_function \
    { \
      public: \
        virtual ~generic_factory_function(){} \
	    virtual Interface * operator()(BOOST_PP_ENUM_PARAMS(N, Param)) = 0; \
            virtual generic_factory_function * copy() const = 0; \
    }; \
    template <class T> \
    class factory_function : public generic_factory_function \
    { \
      public: \
        virtual ~factory_function(){} \
        virtual Interface * operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
        { \
          return new T(BOOST_PP_ENUM_PARAMS(N, p)); \
        } \
        virtual generic_factory_function * copy() const \
        { \
          return new factory_function<T>; \
        } \
    }; \
    std::auto_ptr<generic_factory_function> factory_func_ptr_; \
    Info info_; \
  public: \
    template <class Actual> \
  void set_type_special(Actual *) { \
    factory_func_ptr_.reset(new factory_function<Actual>()); \
  } \
  template <class Actual> \
    void set_type(){ \
      factory_func_ptr_ = new factory_function<Actual>(); \
    } \
    factory(Info info) :factory_func_ptr_(0), info_(info) {} \
    factory(const factory & first) \
      :factory_func_ptr_(first.factory_func_ptr_->copy()), info_(first.info_) {} \
    Interface * operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
    { \
      return create(BOOST_PP_ENUM_PARAMS(N, p)); \
    } \
    Interface * create(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) \
    { \
      return (*factory_func_ptr_) (BOOST_PP_ENUM_PARAMS(N, p)); \
    } \
    Info & get_info(){return info_;} \
};
/**/


/// Functor template specializations.
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_FACTORY_CLASS, _)

#undef BOOST_EXTENSION_FACTORY_CLASS
#else

  template <class Interface, class Info, class Param1 = void, 
            class Param2 = void, class Param3 = void, class Param4 = void, 
            class Param5 = void, class Param6 = void>
class factory
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1, Param2, Param3, Param4, Param5, 
                                   Param6) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
    {return new T(p1, p2, p3, p4, p5, p6);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, 
                         Param5 p5, Param6 p6)
    {return create(p1, p2, p3, p4, p5, p6);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, 
                     Param6 p6)
  {
    return (*factory_func_ptr_) (p1, p2, p3, p4, p5, p6);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3, class Param4, class Param5>
class factory<Interface, Info, Param1, Param2, Param3, Param4, Param5>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1, Param2, Param3, Param4, Param5) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {return new T(p1, p2, p3, p4, p5);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {return create(p1, p2, p3, p4, p5);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
    return (*factory_func_ptr_) (p1, p2, p3, p4, p5);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3, class Param4>
class factory<Interface, Info, Param1, Param2, Param3, Param4>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1, Param2, Param3, Param4) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {return new T(p1, p2, p3, p4);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {return create(p1, p2, p3, p4);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
    return (*factory_func_ptr_) (p1, p2, p3, p4);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2, 
          class Param3>
class factory<Interface, Info, Param1, Param2, Param3>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1, Param2, Param3) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1, Param2 p2, Param3 p3)
    {return new T(p1, p2, p3);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1, Param2 p2, Param3 p3)
    {return create(p1, p2, p3);}
  Interface * create(Param1 p1, Param2 p2, Param3 p3) {
    return (*factory_func_ptr_) (p1, p2, p3);
  }
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1, class Param2>
class factory<Interface, Info, Param1, Param2>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1, Param2) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1, Param2 p2)
    {return new T(p1, p2);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
  void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1, Param2 p2)
    {return create(p1, p2);}
  Interface * create(Param1 p1, Param2 p2){return (*factory_func_ptr_)
  (p1, p2);}
  Info & get_info(){return info_;}
};
template <class Interface, class Info, class Param1>
class factory<Interface, Info, Param1>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()(Param1) = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      (Param1 p1)
    {return new T(p1);}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
    void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()(Param1 p1)
    {return create(p1);}
  Interface * create(Param1 p1){return (*factory_func_ptr_)
  (p1);}
  Info & get_info(){return info_;}
};
template <class Interface, class Info>
class factory<Interface, Info>
{
protected:
  class generic_factory_function
  {
  public:
    virtual ~generic_factory_function(){}
    virtual Interface * operator()() = 0;
    virtual generic_factory_function * copy() const = 0;
  };
  template <class T>
  class factory_function : public generic_factory_function
  {
  public:
    virtual ~factory_function(){}
    virtual Interface * operator()
      ()
    {return new T();}
    virtual generic_factory_function * copy() const {
      return new factory_function<T>;
    }
  };
  std::auto_ptr<generic_factory_function> factory_func_ptr_;
  Info info_;
public:
  template <class Actual>
    void set_type_special(Actual *) {
    factory_func_ptr_.reset(new factory_function<Actual>());
  }
  template <class Actual>
    void set_type(){factory_func_ptr_ = new factory_function<Actual>();}
  factory(Info info)
    :factory_func_ptr_(0),
    info_(info)
  {}
  factory(const factory & first)
    :factory_func_ptr_(first.factory_func_ptr_->copy()),
    info_(first.info_)
                       {}
  Interface * operator()()
    {return create();}
  Interface * create(){return (*factory_func_ptr_)
  ();}
  Info & get_info(){return info_;}
};
#endif // BOOST_EXTENSION_USE_PP
}}


#endif
