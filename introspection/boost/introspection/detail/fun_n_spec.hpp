#define n BOOST_PP_ITERATION()

template<>
struct fun_n<n>
{
  template<typename FUNCTION_TYPE>
  class free_fun : public member
  {
  public:
    typedef typename FUNCTION_TYPE::return_type return_type;
    typedef typename FUNCTION_TYPE::arglist_type arglist_type;
    typedef return_type (*function_pointer_type)(BOOST_PP_REPEAT(n, BOOST_INTROSPECTION_DISPATCHER_SPECIALIZATION_PARAM, unused));
    
    free_fun(const std::string& name, function_pointer_type fun) : member(name), fun_(fun) { }
    
    function_pointer_type function() const { return fun_; }
      
  private:
    function_pointer_type fun_;
  };

  template<typename POINTER_TYPE, typename FUNCTION_TYPE>
  inline static free_fun<FUNCTION_TYPE>*
  make_free_fun(const std::string& name, POINTER_TYPE pointer)
  { return new free_fun<FUNCTION_TYPE>(name, pointer); }
  
  template<typename CLASS, typename FUNCTION_TYPE>
  static void* make(BOOST_PP_REPEAT(n, BOOST_INTROSPECTION_DISPATCHER_SPECIALIZATION_PARAM, unused))
  {
    CLASS* object(new CLASS(BOOST_PP_ENUM_PARAMS(n, a)));
    return object;
  }
  
  template<typename FUNCTION_TYPE>
  class mem_fun : public member
  {
  public:
    typedef typename FUNCTION_TYPE::return_type return_type;
    typedef typename FUNCTION_TYPE::arglist_type arglist_type;
    
    mem_fun(const std::string& name) : member(name) { }
    
    virtual return_type
    call(void* obj BOOST_PP_COMMA_IF(n)
      BOOST_PP_REPEAT(n, BOOST_INTROSPECTION_DISPATCHER_SPECIALIZATION_PARAM, unused)) const = 0;
  };
  
  template<typename CLASS, typename POINTER_TYPE, typename FUNCTION_TYPE>
  class direct_mem_fun : public mem_fun<FUNCTION_TYPE>
  {
  public:
    direct_mem_fun(const std::string& name, POINTER_TYPE pm)
      : mem_fun<FUNCTION_TYPE>(name), pm_(pm) { }
      
    typedef typename FUNCTION_TYPE::return_type return_type;
    typedef typename FUNCTION_TYPE::arglist_type arglist_type;
    
    return_type call(void* obj BOOST_PP_COMMA_IF(n)
      BOOST_PP_REPEAT(n, BOOST_INTROSPECTION_DISPATCHER_SPECIALIZATION_PARAM, unused)) const {
        return (reinterpret_cast<CLASS*>(obj)->*pm_)(BOOST_PP_ENUM_PARAMS(n, a)); }

  private:
    POINTER_TYPE pm_;
  };

  template<typename CLASS, typename POINTER_TYPE, typename FUNCTION_TYPE>
  inline static direct_mem_fun<CLASS, POINTER_TYPE, FUNCTION_TYPE>*
  make_direct_mem_fun(const std::string& name, POINTER_TYPE pointer)
  { return new direct_mem_fun<CLASS, POINTER_TYPE, FUNCTION_TYPE>(name, pointer); }
  

}; // struct fun_n<n>

#undef n
