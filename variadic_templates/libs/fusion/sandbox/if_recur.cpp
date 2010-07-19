//Purpose:
//  demonstrate if_recur.
//
#include <iostream>
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>

#define IF_RECUR_TRACE

      struct indent_undent_cout
      {
          indent_undent_cout(void)
          {
              std::cout<<indent_buf_in;
          }
          ~indent_undent_cout(void)
          {
              std::cout<<indent_buf_out;
          }
      };
      
#include <boost/fusion/algorithm/if_recur.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
// For doing I/O
#include <boost/fusion/sequence/io.hpp>

namespace boost
{
namespace fusion
{
namespace example
{

  void show_default(void)
  {
      typedef if_recur<int> ir_t;
      std::cout<<"show_default="<<ir_t::call( 9)<<"\n";
  }
  
  
        template
        < int Max
        >
      struct less
      {
          static aux::if_recur::recur_kind const our_recur=aux::if_recur::recur_value;
          typedef bool result_type;
          static result_type call(int const& state)
          {
              return state<Max;
          }
      };

      struct increment
      {
          typedef int result_type;
          static result_type call(int const& state)
          {
              std::cout<<"increment:state="<<state<<"\n";
              return state+1;
          }
      };
      
      struct print_pair
      {
          typedef int result_type;
          static result_type call(int const& state_now, int const& state_up)
          {
              std::cout
              <<"print_pair"
              <<":state_now="<<state_now
              <<":state_up="<<state_up
              <<"\n";
              return state_up;
          }
      };
  
      struct print_btm
      {
          typedef int result_type;
          static result_type call(int const& state_btm)
          {
              std::cout
              <<"print_btm"
              <<":state_btm="<<state_btm
              <<"\n";
              return state_btm;
          }
      };
  
  void show_less_increment(void)
  {
      typedef less<4> recur_t;
      typedef increment then_down_t;
      typedef aux::if_recur::identity<int> now_now_t;
      typedef print_pair now_up_t;
      typedef print_btm else_btm_t;
          typedef
        if_recur
        < int
        , recur_t
        , then_down_t
        , now_now_t
        , now_up_t
        , else_btm_t
        > 
      ir_t;
      std::cout<<"show_less_increment="<<ir_t::call( 0)<<"\n";
  }
  
  namespace show_recur_type
  {
      template
      < int TypeId
      >
    struct int_val
    {
        int val_;
        int_val(void):val_(TypeId){}
        void reverse(void)
        {
            val_ = -val_;
        }
            friend
          std::ostream&
        operator<<
          ( std::ostream& sout
          , int_val<TypeId> const& self
          )
        {
            sout<<"int_val<"<<TypeId<<">("<<self.val_<<")";
            return sout;
        }
    };
    
      template
      < typename IterEnd
      >
    struct iter_print
    {
          template
          < typename IterNow
          >
          void
        operator()(IterNow const& a_now)const
        {
            typedef typename result_of::equal_to<IterNow,IterEnd>::type at_end_t;
            at_end_t at_end_v;
            this->call(a_now, at_end_v);
        }
        
     private:   
          template
          < typename IterNow
          >
          void
        call
          ( IterNow const& a_now
          , mpl::true_
          )const
        {
            std::cout<<"\n";
        }
          template
          < typename IterNow
          >
          void
        call
          ( IterNow const& a_now
          , mpl::false_
          )const
        {
            std::cout<<deref(a_now)<<" ";
            this->operator()(next(a_now));
        }
        
    };
    
      template
      < typename IterEnd
      >
    struct iter_not_end
    {
        struct recur
        {
            static aux::if_recur::recur_kind const our_recur=aux::if_recur::recur_type;
            
              template
              < typename IterNow
              >
            struct result_of
            : mpl::not_<fusion::result_of::equal_to<IterNow,IterEnd> >
            {};
        };
    };
    
      template
      < typename IterEnd
      >
    struct iter_next
    {
          template
          < typename IterNow
          >
        struct result_of
        : fusion::result_of::next<IterNow>
        {};
        
          template
          < typename IterNow
          >
            static 
          typename result_of<IterNow>::type
        call(IterNow const& iter_now)
        {
            std::cout<<"iter_next:\n";
            iter_print<IterEnd> printer;
            std::cout<<"iter_now=";
            printer(iter_now);
            return fusion::next(iter_now);
        };
    };
    
    struct iter_identity
    {
          template
          < typename IterNow
          >
        struct result_of
        {
            typedef IterNow type;
        };
        
          template
          < typename IterNow
          >
            static 
          typename result_of<IterNow>::type
        call(IterNow const& iter_now)
        {
            return iter_now;
        };
    };
    
      template
      < typename IterEnd
      >
    struct iter_btm
    {
          template
          < typename IterNow
          >
        struct result_of
        {
            typedef IterNow type;
        };
        
          template
          < typename IterNow
          >
            static 
          typename result_of<IterNow>::type
        call(IterNow const& iter_now)
        {
            std::cout<<"iter_btm:\n";
            iter_print<IterEnd> printer;
            std::cout<<"iter_now=";
            printer(iter_now);
            return iter_now;
        };
    };
    
      template
      < typename IterEnd
      , typename Target
      >
    struct iter_find_target
    {
        struct now_up
        {
              template
              < typename IterNow
              , typename IterUp
              >
            struct result_of
            : mpl::eval_if
              < fusion::result_of::equal_to
                < IterUp
                , IterEnd
                >
              , typename mpl::eval_if
                < is_same
                  < Target&
                  , typename fusion::result_of::deref<IterNow>::type
                  >
                , mpl::identity<IterNow>
                , mpl::identity<IterUp>
                >
              , mpl::identity<IterUp>
              >
            {
            };
                
              template
              < typename IterNow
              , typename IterUp
              >
                static 
              IterNow
            call_result
              ( IterNow const& iter_now
              , IterUp  const& iter_up
              , IterNow*
              )
            {
                return iter_now;
            }
              template
              < typename IterNow
              , typename IterUp
              >
                static 
              IterUp
            call_result
              ( IterNow const& iter_now
              , IterUp  const& iter_up
              , IterUp*
              )
            {
                return iter_up;
            }
              
              template
              < typename IterNow
              , typename IterUp
              >
                static 
              typename result_of<IterNow,IterUp>::type
            call
              ( IterNow const& iter_now
              , IterUp  const& iter_up
              )
            {
                std::cout<<"now_up:\n";
                iter_print<IterEnd> printer;
                std::cout<<"iter_now=";
                printer(iter_now);
                std::cout<<"iter_up=";
                printer(iter_up);
                typedef typename result_of<IterNow,IterUp>::type result_type;
                return call_result
                  ( iter_now
                  , iter_up
                  , static_cast<result_type*>(0)
                  );
            }
            
        };//end now_up
      
    };
    
    void show(void)
    {
            typedef
          int_val<-9>
        target_val
        ;
            typedef 
          list
          < target_val
          , int_val<0>
          , target_val
          , int_val<2>
          >
        list_t
        ;
        // Let's specify our own tuple delimeters for nicer printing
        std::cout << tuple_open('[');
        std::cout << tuple_close(']');
        std::cout << tuple_delimiter(", ");
        list_t list_v;
        std::cout<<"list_v="<<list_v<<"\n";
        std::cout<<"target_v="<<target_val()<<"\n";
        typedef result_of::end<list_t>::type end_t;
        
        typedef result_of::begin<list_t>::type state_t;
        typedef iter_not_end<end_t>::recur recur_t;
        typedef iter_next<end_t> then_down_t;
        typedef iter_identity now_now_t;
        typedef iter_find_target<end_t,target_val>::now_up now_up_t;
        typedef iter_btm<end_t> else_btm_t;
        
            typedef
          if_recur
          < state_t
          , recur_t
          , then_down_t
          , now_now_t
          , now_up_t
          , else_btm_t
          > 
        ir_t;
        {
            state_t state_v(begin(list_v));
            auto result_v=ir_t::call(state_v);
            end_t end_v(end(list_v));
            iter_print<end_t> printer;
            std::cout<<"result_v=";
            printer(result_v);
        }
    }
    
  }//exit show_recur_type
  
}//exit example namespace
}//exit fusion namespace
}//exit boost namespace

int main(void)
{
    boost::iostreams::indent_scoped_ostreambuf<char> iob(std::cout);
    //boost::fusion::example::show_default();
    //boost::fusion::example::show_less_increment();
    boost::fusion::example::show_recur_type::show();
    return 0;
}    
