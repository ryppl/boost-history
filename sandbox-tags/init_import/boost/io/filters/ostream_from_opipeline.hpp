#ifndef BOOST_IO_FILTERS_OSTREAM_FROM_OPIPELINE_HPP
#define BOOST_IO_FILTERS_OSTREAM_FROM_OPIPELINE_HPP
//  (C) Copyright Larry Joe Evans 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#include "ostreambuf_from_opipeline.hpp"
#include <ostream>
namespace boost{namespace io{namespace filters{
  
  struct void_ptr
  //Purpose:
  //  In combination with following class's privitization
  //  of operator<<(void*) and operator<<(void_ptr), 
  //  enforces more explicit indication of desire to
  //  print value of void*.
  {
    typedef void referent_type;
    explicit void_ptr(referent_type const*a_ptr)
    : my_ptr(a_ptr)
    {}
    explicit void_ptr(referent_type*a_ptr)
    : my_ptr(a_ptr)
    {}
    void_ptr(void_ptr const& a_ptr)
    : my_ptr(a_ptr.get())
    {}
      referent_type const*
    get(void)const
    { return my_ptr
    ;}
  private:
      referent_type const*
    my_ptr
      ;
  };

  template<typename Referent>      
  struct typed_ptr
  : public void_ptr
  {
    typedef Referent referent_type;
    explicit typed_ptr(referent_type const*a_ptr)
    : void_ptr(a_ptr)
    {}
    explicit typed_ptr(referent_type*a_ptr)
    : void_ptr(a_ptr)
    {}
    typed_ptr(typed_ptr const& a_ptr)
    : void_ptr(a_ptr.get())
    {}
      referent_type const*
    get(void)const
    { return static_cast<referent_type const*>(void_ptr::get())
    ;}
  };
      
  template<typename Referent>
    typed_ptr<Referent>
  mk_typed_ptr
    ( Referent* a_ref
    )
    { return typed_ptr<Referent>(a_ref)
    ;}
    
  template<typename Referent>
    typed_ptr<Referent>
  mk_typed_ptr
    ( Referent const* a_ref
    )
    { return typed_ptr<Referent>(a_ref)
    ;}
    
  template
    < typename OutputPipeline
    >
    class 
  ostream_from_opipeline
    //.Purpose:
    //.  Adapt OutputPipeline to appear as a ostream
    : public OutputPipeline
    , private ostreambuf_from_opipeline<OutputPipeline> // adapt pipeline->ostreambuf
    , private std::basic_ostream //then adapt ostreambuf->ostream
      < typename OutputPipeline::contents_type::fluid_type
      , typename OutputPipeline::contents_type::traits_type
      >
    {
    public:
          typedef
        OutputPipeline
      opipeline_super_type
        ;
          typedef
        typename opipeline_super_type::sink_type
      sink_type
        ;
          typedef
        ostreambuf_from_opipeline<opipeline_super_type>
      obuf_super_type
        ;
          typedef
        typename opipeline_super_type::contents_type::fluid_type
      fluid_type
        ;
          typedef
        typename opipeline_super_type::contents_type::traits_type
      traits_type
        ;
          typedef
        std::basic_ostream
          < fluid_type
          , traits_type
          >
      ostream_super_type
        ;
          typedef
        std::basic_string<fluid_type,traits_type>
      string_type
        ;
          typedef
        ostream_from_opipeline<opipeline_super_type>
      my_type
        ;
      ostream_from_opipeline(sink_type* a_sink)
        : obuf_super_type(static_cast<opipeline_super_type*>(this))
        , ostream_super_type(static_cast<obuf_super_type*>(this))
        { this->opipeline_super_type::attach_sink(a_sink)
        ;}
        
        ostream_super_type&
      as_super_ostream()
        { return *this
        ;}
        
      //  Make super_insert available to allow user coding of any
      //  operator<<(my_type&, T) for which there's a corresponding 
      //  operator<<(ostream_super_type&, T) or
      //  ostream_super_type::operator<<(T): 
      template<typename T>
        my_type&
      super_insert(T n)
        { as_super_ostream()<<n
        ; return *this
        ;}
        
      my_type& operator<<(my_type& (*n)(my_type&))
        {return n(*this);}
        
      //  Provide my_type::operator<<(T) for each
      //  ostream_super_type::operator<<(T) or
      //  operator<<(ostream_super_type&, T):
      
            typedef
         ostream_super_type& (*
       super_io_manip_type
         )(ostream_super_type&) 
         ;
      
      my_type& operator<<(super_io_manip_type n){return super_insert(n);}
      my_type& operator<<(bool n){return super_insert(n);}
      my_type& operator<<(short n){return super_insert(n);}
      my_type& operator<<(unsigned short n){return super_insert(n);}
      my_type& operator<<(int n){return super_insert(n);}
      my_type& operator<<(unsigned int n){return super_insert(n);}
      my_type& operator<<(long n){return super_insert(n);}
      my_type& operator<<(unsigned long n){return super_insert(n);}
      my_type& operator<<(float n){return super_insert(n);}
      my_type& operator<<(double n){return super_insert(n);}
      my_type& operator<<(long double n){return super_insert(n);}
      my_type& operator<<(void* n){return super_insert(n);}
      my_type& operator<<(const char* n){return super_insert(n);}
      my_type& operator<<(const unsigned char* n){return super_insert(n);}
      my_type& operator<<(char n){return super_insert(n);}
      my_type& operator<<(const string_type& n){return super_insert(n);}
      
    };//end ostream_from_opipeline<OutputPipeline> template

}}}//exit namespace boost::io::filters

#endif
