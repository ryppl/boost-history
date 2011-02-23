#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
//#define MULTIPLE_DISPATCH_DEBUG
#ifdef MULTIPLE_DISPATCH_DEBUG
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/demangled_type_name.hpp>
#endif
#include <iostream>
#include <string>
#include <boost/mpl/assert.hpp>
typedef std::ostream ind_ostream;
static std::ostream& ind_out=std::cout;
struct trace_scope
{
    trace_scope(std::string const& a_where)
    : my_where(a_where)
    {
        ind_out<<"[[[ENTERING:"<<my_where<<"\n";
        ind_out<<indent_buf_in;
    }
    ~trace_scope(void)
    {
        ind_out<<indent_buf_out;
        ind_out<<"]]]EXITING:"<<my_where<<"\n";
    }
 private:
    std::string my_where;    
};  
  #if 1
    #define FUNCTOR_CONSTANCY const
  #else
    #define FUNCTOR_CONSTANCY
  #endif
  #if 1
    #define ARG_CONSTANCY const
  #else
    #define ARG_CONSTANCY
  #endif
#define APPLY_UNPACK_USER_CHECKED_ARGS
#include <boost/composite_storage/pack/multiple_dispatch/reify_apply.hpp>
#define REIFIER_VISITOR
#ifdef REIFIER_VISITOR
  #include <boost/composite_storage/pack/multiple_dispatch/reifier_visitor.hpp>
#else
  #include <boost/composite_storage/pack/multiple_dispatch/reifier_switch.hpp>
#endif
#include <boost/composite_storage/pack/container_one_of_maybe.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
namespace testing
{
static unsigned object_id=0;
  
static int object_number=0;
  
template <unsigned I, typename ResultType=int>
struct host_concrete;

template <typename Indices, typename ResultType>
struct hosts_indices_concrete;

template <unsigned... Indices, typename ResultType>
struct hosts_indices_concrete< mpl::package_c<unsigned, Indices...>, ResultType>
{
    typedef mpl::package<host_concrete<Indices,ResultType>...> type;
};

template <typename ResultType=int>
struct host_abstract;

}//exit testing namespace

#ifdef REIFIER_VISITOR
template <typename ResultType>
struct hosts_concrete<testing::host_abstract<ResultType> >  
  : testing::hosts_indices_concrete
    < mpl::package_range_c<unsigned, 0,3>::type
    , ResultType
    >
{
};
#endif

namespace testing
{
template <typename ResultType >
struct host_abstract
{
    unsigned const my_id;
    
    host_abstract(void)
      : my_id(++object_id)
    {
        ++object_number;
    }
    
    host_abstract(host_abstract const&)
      : my_id(++object_id)
    {
        ++object_number;
    }
    
    ~host_abstract(void)
    {
        --object_number;
    }
    
    void operator=(host_abstract const&)
    {}
    
  #ifdef REIFIER_VISITOR  
    typedef reifier_visit_abstract_seq< ResultType
      , typename hosts_concrete<host_abstract>::type>
    visitor_abstract;
    
    virtual ResultType accept( visitor_abstract const&)const=0;
  #endif
};

template <unsigned I, typename ResultType>
struct host_concrete
  : host_abstract<ResultType>
{
    char v[2*(I+1)];
    unsigned tag(void)const
    {
        return I;
    }
    
    host_concrete(void)
    {
        v[0]='a';
        v[1]='\0';
    }
  #ifdef REIFIER_VISITOR  
    typedef typename host_abstract<ResultType>::visitor_abstract
    visitor_abstract;
    
    ResultType accept( visitor_abstract const& a_visitor)const
    {
        return a_visitor.visit(*this);
    }
  #endif
};

template <unsigned I, typename ResultType>
ind_ostream& operator<<( ind_ostream& sout
  , host_concrete<I,ResultType>const& x)
{
    sout<<":host_concrete-yes-const<"<<I<<">(my_id="<<x.my_id<<")";
    return sout;
}
  
template <unsigned I, typename ResultType>
ind_ostream& operator<<( ind_ostream& sout
  , host_concrete<I,ResultType>& x)
{
    sout<<":host_concrete-non-const<"<<I<<">(my_id="<<x.my_id<<")";
    return sout;
}
  
enum index_numerals
{ index_0,
  index_1,
  index_2,
  index_3,
  index_4
};

struct functor_any
{
    functor_any(void)
    {}

    typedef int result_type;
    
    static void print(unsigned index)
    {
        ind_out<<":index(last)="<<index<<"\n";
    }
    
    template<typename Head, typename... Tail>
    static void print(unsigned index, Head const& a_head, Tail const&... a_tail)
    {
        ind_out<<":index="<<index<<":a_head=";
        ind_out<<a_head<<"\n";
        print(index+1,a_tail...);
    }
    
    template<typename... Args>
    int operator()(Args const&... args)const
    {
        unsigned const arity=sizeof...(Args);
        ind_out<<"functor_any:arity="<<arity<<"\n";
        print(0,args...);
        return arity;
    }
}; 

struct functor3
#ifdef APPLY_UNPACK_USER_CHECKED_ARGS
: functor_bad_args
  < functor3
  , int
  >
#endif
{
  #ifdef APPLY_UNPACK_USER_CHECKED_ARGS
        typedef
      functor_bad_args
      < functor3
      , int
      >
    super_type;
    
    using super_type::operator();
    
    typedef typename super_type::result_type result_type;
  #endif
          
    functor3(void)
    {}
    
    int operator()(void)FUNCTOR_CONSTANCY
    {
        ind_out<<"functor3:arity=0.\n";
        return 0;
    }
  #if 1
    int operator()( host_concrete<0> ARG_CONSTANCY &a0)FUNCTOR_CONSTANCY
    {
        ind_out<<"functor3:arity=1:\n";
        ind_out<<":a0="<<a0<<"\n";
        return 0;
    }
    int operator()( host_concrete<1> ARG_CONSTANCY &a0)FUNCTOR_CONSTANCY
    {
        ind_out<<"functor3:arity=1:";
        ind_out<<":a0="<<a0<<"\n";
        return 0;
    }
    int operator()( host_concrete<2> ARG_CONSTANCY &a0)FUNCTOR_CONSTANCY
    {
        ind_out<<"functor3:arity=1:";
        ind_out<<":a0="<<a0<<"\n";
        return 0;
    }
    int operator()( host_concrete<0> ARG_CONSTANCY &a0
      , host_concrete<1> ARG_CONSTANCY &a1)FUNCTOR_CONSTANCY
    {
        ind_out<<"functor3:arity=2:\n";
        ind_out<<":a0="<<a0<<"\n";
        ind_out<<":a1="<<a1<<"\n";
        return 0;
    }
  #endif
};

void test(void)
{
#if 1
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("one_of_multiple_dispatch TESTS");
        
        host_concrete<0> hc0;
        host_concrete<1> hc1;
        host_concrete<2> hc2;
      #ifdef REIFIER_VISITOR
      #else
        typedef pack::container < tags::one_of_maybe
          , mpl::integral_c<index_numerals,index_0>, host_concrete<0>
          , host_concrete<1>, host_concrete<2> >
        tagged_type;
        
        unsigned const arity=3;
        
        tagged_type tagged_v[arity];
        
        ind_out<<"v before inject:\n";
        for(unsigned i=0; i<arity; ++i)
        {
            ind_out<<"v["<<i<<"].which="<<tagged_v[i].which()<<"\n";
        }
        tagged_v[0].inject<index_0>(hc0);
        tagged_v[1].inject<index_1>(hc1);
        tagged_v[2].inject<index_2>(hc2);
        ind_out<<"v after inject:\n";
        for(unsigned i=0; i<arity; ++i)
        {
            ind_out<<"v["<<i<<"].which="<<tagged_v[i].which()<<"\n";
        }
      #endif
      #define FUNCTOR_T functor3
            typedef
          FUNCTOR_T
        FUNCTOR_CONSTANCY functor_t;
        functor_t functor_v;
        int result=0;
      #if 0
        ind_out<<"functor_v().result="<<result<<"\n";
        result =functor_v(tagged_v[0].project<index_0>());
        ind_out<<"functor_v(c<0>).result="<<result<<"\n";
        typedef function_types::can_be_called
          < functor_t
            ( host_concrete<0>
            , host_concrete<1>
            )
          >::type
        is_functor_args_callable;
        ind_out
          <<"callable<functor_t(u<0>,u<1> >::type::value="
          <<is_functor_args_callable::type::value
          <<"\n";
      #endif
      #if 1
        #ifdef REIFIER_VISITOR
          ind_out<<"***  defined(REIFIER_VISITOR)\n";
          ind_out<<"hc0.my_id="<<hc0.my_id<<"\n";
          ind_out<<"hc1.my_id="<<hc1.my_id<<"\n";
          ind_out<<"hc2.my_id="<<hc2.my_id<<"\n";
        #else
          ind_out<<"*** !defined(REIFIER_VISITOR)\n";
          ind_out<<"tagged_v[0].my_id="<<tagged_v[0].project<index_0>().my_id<<"\n";
          ind_out<<"tagged_v[1].my_id="<<tagged_v[1].project<index_1>().my_id<<"\n";
          ind_out<<"tagged_v[2].my_id="<<tagged_v[2].project<index_2>().my_id<<"\n";
        #endif
        result = pack::multiple_dispatch::reify_apply
          #ifdef REIFIER_VISITOR
            < pack::multiple_dispatch::reifier_visitor
          #else
            < pack::multiple_dispatch::reifier_switch
          #endif
            >( functor_v
          #ifdef REIFIER_VISITOR
            , static_cast<host_abstract<> ARG_CONSTANCY &>(hc0)
            , static_cast<host_abstract<> ARG_CONSTANCY &>(hc1)
          #else
            , static_cast<tagged_type ARG_CONSTANCY &>(tagged_v[0])
            , static_cast<tagged_type ARG_CONSTANCY &>(tagged_v[1])
          #endif
          //#define FUNCTOR_INVALID_ARGS
          #if defined(FUNCTOR_INVALID_ARGS)
            //This should fail compilation with error message
            //something about invalid args.
            #ifdef REIFIER_VISITOR
              , static_cast<host_abstract<> ARG_CONSTANCY &>(hc0)
            #else        
              , tagged_v[0]
            #endif
          #endif
            );
      #ifdef FUNCTOR_INVALID_ARGS
        ind_out<<"functor_v(v0,v1,v0).result="<<result<<"\n";
      #else
        ind_out<<"functor_v(v0,v1).result="<<result<<"\n";
      #endif
      #endif
    }
#endif    
    ind_out<<"final:object_number="<<object_number<<"\n";
}

}//exit testing
}//exit multiple_dispatch
}//exit pack
}//exit composite_storage
}//exit boost namespace

int main(void)
{
    boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(std::cout,4);
    
    boost::composite_storage::pack::multiple_dispatch::testing::test();
    return 0;
}    
