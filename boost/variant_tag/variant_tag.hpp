/***************************************************************************
                        variant.hpp  -  description
                        ---------------------------
    begin                : Fri Jun 21 10:11:47 IDT 2002
    copyright            : (C) 2002 by Itay Maman
    email                : itay_maman@yahoo.com
 ***************************************************************************/

//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
//



#ifndef _VARIANT_HPP_INC_
#define _VARIANT_HPP_INC_

//////////////////////////////////////////////////////////////////////


#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/pending/ct_if.hpp>

#include <boost/preprocessor/enum_params_with_defaults.hpp>
#include <boost/preprocessor/enum_params_with_a_default.hpp>
#include <boost/preprocessor/enum_params.hpp>


#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <assert.h>
#include <cstring>



#ifdef BOOST_MSVC

#define BOOST_MPL_AUX_CONFIG_USE_PREPROCESSED_HPP_INCLUDED
#define BOOST_VARIANT_MSVC_TYPENAME  
#else

#define BOOST_VARIANT_MSVC_TYPENAME typename
#endif


//////////////////////////////////////////////////////////////////////


//#if defined(BOOST_MSVC) | defined(__COMO__) | defined(__MWERKS__)
#if defined(BOOST_MSVC) | defined(__COMO__)
#define BOOST_VARIANT_NO_ASSIGN_HELPER
#endif


//////////////////////////////////////////////////////////////////////


#include <boost/variant_tag/seqadaptor.hpp>


//////////////////////////////////////////////////////////////////////



namespace boost {



template<typename T>
struct static_visitor
{
   typedef T result_type;
}; 


namespace detail {
namespace variant {

  
   struct incomplete_tag
   {
      typedef incomplete_tag type;
   };
     
#ifdef __MWERKS__
   struct is_incomplete
   {        
      typedef detail::variant::tag_1 yes;
      typedef detail::variant::tag_2 no;
      
      static yes check(const detail::variant::incomplete_tag*);
      static no check(...);
   }; //is_variant

#else
   template<typename T>
   struct is_incomplete
   {
   private:
      typedef detail::variant::incomplete_tag t_tag;
      typedef is_base_and_derived
      <
         t_tag, 
         typename remove_const<T>::type
      > t_temp;

   public:
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //is_incomplete

#endif   //__MWERKS__

} //namespace variant
} //namespace detail


//////////////////////////////////////////////////////////////////////


template<typename T>
struct type_to_type
{
   typedef T type;
}; //type_to_type


template<typename T>
struct incomplete : detail::variant::incomplete_tag
{
   typedef incomplete t_self;
   typedef T t_held;

   ~incomplete() { }

   incomplete() : impl_p_(new t_held) { }

   incomplete(const t_held& value) : impl_p_(new t_held(value)) { }

   incomplete(const t_self& other)
      :  impl_p_( new t_held(*other.get_ptr()) )  { }

   void swap(t_self& other)
   {
      std::swap(impl_p_, other.impl_p_);
   }

   t_self& operator=(const t_self& rhs)
   {
      t_self temp(rhs);
      swap(temp);

      return *this;
   }

   t_held* get_ptr()
   {
      return impl_p_.get();
   }

   const t_held* get_ptr() const
   {
      return impl_p_.get();
   }

private:
   std::auto_ptr<t_held> impl_p_;

}; //incomplete


//////////////////////////////////////////////////////////////////////


namespace detail {
namespace variant {

   //
   //Intenrally used visitors:
   //

   //(1) copy construction
   template<typename TrgType>
   struct variant_to_variant_copier
   {
      typedef void result_type;

      template<class Value>
      void operator()(Value& value)
      {
         *(trg_p_) = value;
      }

      TrgType* trg_p_;
   }; //variant_to_variant_copier


   //(2) variant::type()
   struct type_info_extractor
   {
      typedef void result_type;

      type_info_extractor() : info_p_(0) { }

      template <typename U>
      void operator()(const U&)
      {
         info_p_ = &typeid(U);
      }

      const std::type_info* info_p_;
   }; //type_info_extractor


   //Used to designate member functions as 'private'
   struct private_tag
   {
      typedef private_tag type;
   };

   struct variant_tag
   {
      typedef variant_tag type;
   };


   template<typename ReturnType, class V, typename Sequence, int SeqSize, 
      bool HostIsConst = false>
   struct accept_table
   {
      typedef Sequence t_sequence;
      typedef ReturnType t_return;

      typedef V& t_a1;
      typedef void* t_a2;
      typedef t_return (*t_accept_f)(V&, const void*);
      t_accept_f funcs_[SeqSize];

      template<class T>
      struct accept_impl_simple
      {

         typedef typename add_const<T>::type t_with_const;
         typedef typename ct_if<
            HostIsConst,
            t_with_const,
            T
         >::type t_held;
         
         static t_return on_accept(V& vis, const void* raw_ptr)
         {
            t_held* ptr = static_cast<t_held*>(const_cast<void*>(raw_ptr));
            
            //
            // [Compiler error]
            //
            // Indicates an inadequate visitor type
            return vis(*ptr);
         }

      }; //accept_impl_simple

      template<class T>
      struct accept_impl_incomplete
      {
         typedef typename T::t_held t_temp;
         typedef typename add_const<t_temp>::type t_with_const;
         typedef typename ct_if<
            HostIsConst,
            t_with_const,
            t_temp
         >::type t_held;

         static t_return on_accept(V& vis, const void* raw_ptr)
         {
            incomplete<t_held>* incom_p =
               static_cast<incomplete<t_held>*>(const_cast<void*>(raw_ptr) );
            t_held* ptr = incom_p->get_ptr();

            //
            // [Compiler error]
            //
            // Indicates an inadequate visitor type
            return vis(*ptr);
         }

      }; //accept_impl_incomplete



      template<typename CurrSequence>
      void init(CurrSequence, t_accept_f* , detail::variant::int_to_type<false>)
      { }


      template<typename CurrSequence>
      void init(CurrSequence, t_accept_f* pos, detail::variant::int_to_type<true>)
      {
         typedef typename BOOST_VARIANT_TL_NS::get_head<CurrSequence>::type 
            t_curr;

         
#ifdef __MWERKS__
         using detail::variant::is_incomplete;

         BOOST_STATIC_CONSTANT( bool, is_incom = 
            sizeof(is_incomplete::check(static_cast<t_curr*>(0))) 
            == sizeof(is_incomplete::yes));
            
#else
         BOOST_STATIC_CONSTANT(bool, is_incom = 
            detail::variant::is_incomplete<t_curr>::value);

#endif //__MWERKS__

         typedef typename ct_if
         <
            is_incom,
            accept_impl_incomplete<t_curr>,
            accept_impl_simple<t_curr>
         >::type t_temp;

         *pos = t_temp::on_accept;
                
         typedef typename BOOST_VARIANT_TL_NS::get_rest<CurrSequence>::type
            t_rest;

         BOOST_STATIC_CONSTANT(bool, end_of_seq = 
            BOOST_VARIANT_TL_NS::is_empty<t_rest>::value);

         init(t_rest(), pos+1, 
            detail::variant::int_to_type<!end_of_seq>() );
      }


   public:
      t_accept_f operator[](unsigned int index)
      {
         return funcs_[index];
      }


      accept_table()
      {
         init(t_sequence(), funcs_, detail::variant::int_to_type<true>());
      }
   }; //accept_table

   //////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////

#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER

#define BOOST_VARIANT_IGNORE_TYPE_MOLD(Z, N, D) \
   typedef nu< N > BOOST_PP_CAT(ignore_type_, N);

BOOST_PP_REPEAT(BOOST_VARIANT_LIMIT_TYPES, 
   BOOST_VARIANT_IGNORE_TYPE_MOLD, aaa)

#else
#define BOOST_VARIANT_IGNORE_TYPE_MOLD(Z, N, D) \
   typedef plain_never_used BOOST_PP_CAT(ignore_type_, N);

BOOST_PP_REPEAT(BOOST_VARIANT_LIMIT_TYPES, 
   BOOST_VARIANT_IGNORE_TYPE_MOLD, aaa)



#endif //BOOST_VARIANT_NO_ASSIGN_HELPER




   //////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////


#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER

#define BOOST_VARIANT_FLAT_HELPER_PARAMS                                \
   BOOST_PP_ENUM_PARAMS_WITH_DEFAULTS(BOOST_VARIANT_LIMIT_TYPES,        \
   typename T, ignore_type_)



#define BOOST_VARIANT_SET_ITEM_MOLD(Z, N, D)                            \
   static void set_item(t_self* inst_p, const BOOST_PP_CAT(AT, N) a)   \
   {                                                                    \
      inst_p->assign(a, detail::variant::private_tag());                \
   }                       

   //////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////


#else //BOOST_VARIANT_NO_ASSIGN_HELPER


   template<int CurrIndex, typename Sequence, typename ParentType>
   struct assign_helper;

   template<typename Sequence, typename ParentType>
   struct assign_helper<0, Sequence, ParentType>
   {
      static void set_item() { }

   }; //assign_helper<0, *, *>



   template<int CurrIndex, typename Sequence, typename ParentType>
   struct assign_helper
      :  assign_helper
         <
            CurrIndex-1,
            typename BOOST_VARIANT_TL_NS::get_rest<Sequence>::type,
            ParentType
         >
   {
      typedef assign_helper
      <
         CurrIndex-1, 
         typename BOOST_VARIANT_TL_NS::get_rest<Sequence>::type,
         ParentType
      > t_base;

      using t_base::set_item;

      typedef typename BOOST_VARIANT_TL_NS::get_head<Sequence>::type t_arg;

      //TODO: t_arg - pass by reference ??
      static void set_item(ParentType* parent_p, t_arg a)
      {
         parent_p->assign(a, private_tag());
      }

/*
      static void set_item(ParentType* parent_p,
         typename add_const<t_arg>::type& a)
      {
         parent_p->assign(a, private_tag());
      }
*/
      
   }; //assign_helper


#endif //BOOST_VARIANT_NO_ASSIGN_HELPER


   //////////////////////////////////////////////////////////////////////


   template<class Sequence>
   class stack_holder
   {
      typedef Sequence t_sequence;

      typedef stack_holder<t_sequence> t_self;
      typedef typename BOOST_VARIANT_TL_NS::get_head<t_sequence>::type t_first;

      BOOST_STATIC_CONSTANT(int, max_size = 
         BOOST_VARIANT_TL_NS::max_size<t_sequence>::value);

      BOOST_STATIC_CONSTANT(int, align = 
         BOOST_VARIANT_TL_NS::needed_alignment<t_sequence>::value);


      union inner_union;
      friend union inner_union;

      union inner_union
      {
         typedef stack_holder<Sequence> t_outer;

#ifdef BOOST_MSVC
         typedef typename type_with_alignment<(align > 8 ? 8 : align)>::type
            t_aligner;
#else
         typedef typename type_with_alignment<t_outer::align>::type t_aligner;
   
#endif //BOOST_MSVC

         typedef char t_buffer[2][max_size];

         t_aligner aligner_;
         t_buffer buf_;

      }; //inner_union

      typedef typename inner_union::t_buffer t_buffer;



      template<typename T>
      void assign(const T& t)
      {      
         void* ptr = &(holder_.buf_[0]);
         new(ptr) T(t);

         static type_desc_helper<T> tdh;
         desc_p_ = &(tdh.desc1_);
         assert(desc_p_);
      }

      void flip()
      {
         desc_p_ = desc_p_->other_;
      }

      void destroy()
      {
         destroy(get_ptr());
      }

      void destroy(void *ptr)
      {
         desc_p_->destroy_func_(ptr);
      }


      void clone(void* trg_p)
      {
         desc_p_->clone_func_(get_ptr(), trg_p);
      }

      bool is_2nd_active() const
      {
         return desc_p_->second_is_active_;
      }


   public:
      stack_holder()
      {
         assign(t_first());
      }

      ~stack_holder()
      {
         destroy();
      }


      template<class ValueType>
      stack_holder(const ValueType& v)
      {
         assign(v);
      }


      void* get_ptr()
      {
         if(is_2nd_active())
            return &(holder_.buf_[1][0]);
         else
            return &(holder_.buf_[0][0]);
      }

      // TODO: Make private
      void* get_non_active_ptr()
      {
         if(is_2nd_active())
            return &(holder_.buf_[0][0]);
         else
            return &(holder_.buf_[1][0]);
      }

      const void* get_ptr() const
      {
         if(is_2nd_active())
            return &(holder_.buf_[1][0]);
         else
            return &(holder_.buf_[0][0]);
      }

      int tid() const
      {
         return desc_p_->tid_;
      }

      void swap(t_self& other)
      {      	 
#ifndef BOOST_NO_STDC_NAMESPACE
         using std::memcpy;
#endif      
         try
         {
            other.clone(get_non_active_ptr());
         }
         catch(...)
         {
            throw;
         }


         try
         {
            clone(other.get_non_active_ptr());
         }
         catch(...)
         {
            other.destroy(get_non_active_ptr());
            throw;
         }


         flip();
         other.flip();

         bool my_2nd = is_2nd_active();
         bool other_2nd = other.is_2nd_active();


         std::swap(desc_p_, other.desc_p_);
         if(is_2nd_active() != my_2nd)
            flip();

         if(other.is_2nd_active() != other_2nd)
            other.flip();
      }

      template<typename ValueType>
      t_self& operator=(const ValueType& v)
      {
         t_self temp(v);
         swap(temp);

         return *this;
      }


      struct type_desc
      {
         bool second_is_active_;
         int tid_;
         void (*destroy_func_)(void*);
         void (*clone_func_)(void*, void*);

         type_desc* other_;
      };

      //type_desc_helper implements the vtable idiom
      //See [1] at the bottom for more details
      template<typename T>
      struct type_desc_helper
      {
         static void on_destroy(void* ptr)
         {
            T* inst_p = static_cast<T*>(ptr);
            inst_p->~T();
         }

         static void on_clone(void* src_p, void* trg_p)
         {
            T& src = *static_cast<T*>(src_p);
            new (trg_p) T(src);
         }

         static void on_switch()
         {
         }

         type_desc_helper()
         {
            BOOST_STATIC_CONSTANT( int, 
               tid = (BOOST_VARIANT_TL_NS::type_to_tid<Sequence, T>::value) );           
            BOOST_STATIC_ASSERT(tid >= 0);

            desc1_.second_is_active_ = false;
            desc1_.tid_ = tid;
            desc1_.destroy_func_ = on_destroy;
            desc1_.clone_func_ = on_clone;
            desc1_.other_ = &desc2_;


            desc2_.second_is_active_ = true;
            desc2_.tid_ = tid;
            desc2_.destroy_func_ = on_destroy;
            desc2_.clone_func_ = on_clone;
            desc2_.other_ = &desc1_;

         };

         type_desc desc1_;
         type_desc desc2_;


      }; //type_desc_helper


      //desc_p_ should be accessible from variant<>

   private:
      type_desc* desc_p_;
      inner_union holder_;

   }; //stack_holder


   //////////////////////////////////////////////////////////////////////


#ifdef __MWERKS__
   struct is_variant
   {
      typedef detail::variant::tag_1 yes;
      typedef detail::variant::tag_2 no;
      
      static yes check(const detail::variant::variant_tag*);
      static no check(...);
   }; //is_variant

#else
        

   template<typename T>
   struct is_variant
   {
   private:
      typedef detail::variant::variant_tag t_tag;
      typedef is_base_and_derived<t_tag, T> t_temp;

   public:
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);
   }; //is_variant
           
#endif   //__MWERKS__


} //namespace variant
} //namespace detail


//////////////////////////////////////////////////////////////////////


//
// Visitation
//

template<typename VisitorType>
struct visitor_to_functor_adaptor
{
   typedef VisitorType visitor_type;
   typedef typename visitor_type::result_type result_type;
   
   explicit visitor_to_functor_adaptor(visitor_type& visitor)
      : visitor_(visitor) { }


   template<typename VariantType>
   result_type operator()(VariantType& x)
   {
      return call_vtable_entry(&x);
   }
   
private:
   template<typename VariantType>
   result_type call_vtable_entry(VariantType* x)
   {
      typedef VariantType t_variant;

      static detail::variant::accept_table<result_type, visitor_type, 
            BOOST_VARIANT_MSVC_TYPENAME t_variant::t_sequence, 
            t_variant::seq_size, boost::is_const<VariantType>::value> table;
      return table[x->tid()](visitor_, x->holder_.get_ptr());      

   }
   
private:      
   visitor_type& visitor_;

}; // visitor_to_functor_adaptor



struct void_like
{
   void_like() { };

}; //void_like


template<typename VisitorType>
struct visitor_to_void_functor_adaptor
{
   typedef VisitorType visitor_type;
   typedef void_like result_type;
   
   explicit visitor_to_void_functor_adaptor(visitor_type& visitor)
      : wrapper_(visitor) { }


   template<typename VariantType>
   void_like operator()(VariantType& x)
   {
      boost::apply_visitor(wrapper_, x);
      return void_like();
   }

   struct visitor_wrapper
   {
      typedef int result_type;

      explicit visitor_wrapper(visitor_type& visitor)
         : visitor_(visitor) { }

      template<typename OperandType>
      int operator()(OperandType& x)
      {
         visitor_(x);
         return 0;
      }

      visitor_type& visitor_;
   }; 
   
private:
   visitor_wrapper wrapper_;

}; // visitor_to_void_functor_adaptor


template<typename VisitorType>
struct visitaion_adaptor_selector
{
   typedef VisitorType visitor_type;
   typedef typename visitor_type::result_type result_type;

   typedef typename ct_if
   <
      is_same<result_type, void>::value,
      visitor_to_void_functor_adaptor<visitor_type>,
      visitor_to_functor_adaptor<visitor_type>
   >::type adaptor_type;

   // typedef typename adaptor_type::result_type impl_result_type;

}; //visitaion_adaptor_selector

template<typename VisitorType>
struct result_type_selector
{
   typedef typename VisitorType::result_type in_type;

   typedef typename ct_if
   <
      is_same<in_type, void>::value,
      void_like,
      in_type    
   >::type type;

}; // result_type_selector


//TODO: Move all adaptors + adaptor_selector to namespace detail


//
// apply_visitor (unary)
//
template<typename VisitorType>
typename visitaion_adaptor_selector<VisitorType>::adaptor_type
apply_visitor(VisitorType& visitor)
{
   typedef typename visitaion_adaptor_selector
      <VisitorType>::adaptor_type temp;

   return temp(visitor);
}

template<typename VisitorType>
typename visitaion_adaptor_selector<const VisitorType>::adaptor_type
apply_visitor(const VisitorType& visitor)
{
   typedef typename visitaion_adaptor_selector<const VisitorType>::adaptor_type 
      temp;

   return temp(visitor);
}


//
// apply_visitor (binary)
//

template<typename VisitorType, typename VariantType>
typename result_type_selector<VisitorType>::type
apply_visitor(VisitorType& vis, VariantType& var_inst)
{
   return apply_visitor(vis)(var_inst);
}


template<typename VisitorType, typename VariantType>
typename result_type_selector<const VisitorType>::type
apply_visitor(const VisitorType& vis, VariantType& var_inst)
{
   return apply_visitor(vis)(var_inst);
}


/*


template<typename VisitorType, typename VariantType>
typename VisitorType::result_type 
apply_visitor(VisitorType& vis, VariantType& var_inst)
{
   typedef VariantType t_variant;
   typedef typename VisitorType::result_type t_return;

   static detail::variant::accept_table<t_return, VisitorType, 
         typename t_variant::t_sequence, t_variant::seq_size> table;
   return table[var_inst.tid()](vis, var_inst.holder_.get_ptr());
}

template<typename VisitorType, typename VariantType>
typename VisitorType::result_type 
apply_visitor(const VisitorType& vis, VariantType& var_inst)
{
   typedef VisitorType t_concrete_visitor;
   typedef typename t_concrete_visitor::result_type t_return;
   typedef VariantType t_variant;

   static detail::variant::accept_table<t_return, t_concrete_visitor, 
      typename t_variant::t_sequence, t_variant::seq_size> table;

   return table[var_inst.tid()](vis, var_inst.holder_.get_ptr());
   //TODO: possible optimization. tid() incurs overhead
}

#ifndef BOOST_MSVC

//Visit a const variant
template<typename VisitorType, typename VariantType>
typename VisitorType::result_type 
apply_visitor(const VisitorType& vis, const VariantType& var_inst)
{
   typedef VariantType t_variant;
   typedef typename VisitorType::result_type t_return;

   //Strip away the constness. It will be restored by
   //accept_table::on_accept(). 
   //The 'true' value passed to accept_table<> takes care of that
   void* ptr = const_cast<void*>(var_inst.holder_.get_ptr());

   typedef const VisitorType t_concrete_visitor;

   static detail::variant::accept_table<t_return, t_concrete_visitor, 
      BOOST_VARIANT_MSVC_TYPENAME t_variant::t_sequence, t_variant::seq_size, 
      true> table;

   return table[var_inst.tid()](vis, ptr);
}


template<typename VisitorType, typename VariantType>
typename VisitorType::result_type 
apply_visitor(VisitorType& vis, const VariantType& var_inst)
{
   typedef VariantType t_variant;
   typedef typename VisitorType::result_type t_return;
   

   static detail::variant::accept_table<t_return, VisitorType, 
      BOOST_VARIANT_MSVC_TYPENAME t_variant::t_sequence, t_variant::seq_size> 
      table;

   return table[var_inst.tid()](vis, var_inst.holder_.get_ptr());

//   return vis;
}

#endif
*/

//////////////////////////////////////////////////////////////////////


template< BOOST_VARIANT_PARAMS_DEF >
struct variant : detail::variant::variant_tag
{
   typedef variant t_self;

private:
   typedef detail::variant::unused_tag t_unused;
   typedef detail::variant::plain_never_used t_plain_never_used;

#ifdef BOOST_VARIANT_USE_MPL_SEQ
   typedef BOOST_VARIANT_IMPL_TL
   <
      AT0, AT1, AT2, AT3, AT4, AT5, AT6, AT7
//      , AT8, AT9
//      ,AT10, AT11, AT12, AT13, AT14, AT15, AT16, AT17, AT18, AT19
   > t_raw_sequence;

   typedef typename 
      BOOST_VARIANT_TL_NS::erase_all_derived<t_raw_sequence, t_unused>::type 
      t_chopped_sequence;

#else
   typedef BOOST_VARIANT_IMPL_TL
   (
      AT0, AT1, AT2, AT3, AT4, AT5, AT6, AT7, AT8, AT9
      ,AT10, AT11, AT12, AT13, AT14, AT15, AT16, AT17, AT18, AT19
   ) t_raw_sequence;

#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER
   BOOST_STATIC_CONSTANT(int, first_unused = 
      (BOOST_VARIANT_TL_NS::first_derived
      <t_raw_sequence, t_unused>::value) );
#else
   BOOST_STATIC_CONSTANT(int, first_unused = 
      (BOOST_VARIANT_TL_NS::type_to_tid
      <t_raw_sequence, t_plain_never_used>::value) );
#endif //BOOST_VARIANT_NO_ASSIGN_HELPER
      
   typedef typename BOOST_VARIANT_TL_NS::copy_first_n<t_raw_sequence,
      first_unused>::type t_chopped_sequence;


#endif //BOOST_VARIANT_USE_MPL_SEQ

public:
   typedef typename ct_if
   <
#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER
      is_base_and_derived<t_unused, AT1>::value,
#else
      is_same<t_plain_never_used, AT1>::value,
#endif 
      AT0,
      t_chopped_sequence
   >::type t_sequence;

   BOOST_STATIC_CONSTANT(int,
      seq_size = BOOST_VARIANT_TL_NS::get_size<t_sequence>::value);


   int tid() const
   {
      return holder_.tid();
   }

private:
   typedef detail::variant::stack_holder<t_sequence> t_storage;
   typedef typename BOOST_VARIANT_TL_NS::get_head<t_sequence>::type t_first;


//These functions are always needed (in the assign_helper_flat impl.)
//#ifndef BOOST_VARIANT_NO_ASSIGN_HELPER

   //Assign complete types
   template<typename Value>
   void assign(Value& value, detail::variant::int_to_type<true> )     //ES
   {
      holder_ = value;
   }

   //Assign incomplete types
   template<typename Value>
   void assign(const Value& value, detail::variant::int_to_type<false> ) //ES
   {
      holder_ = incomplete<Value>(value);
   }

public:

   //variant::assign should be private, but was made public as a workaround
   //the private_tag parameter is a dummy value, which
   //serves as a (pseudo) private qualifier
   template<typename ValueType>
   void assign(const ValueType& value, detail::variant::private_tag )  //ES
   {
      typedef BOOST_VARIANT_TL_NS::type_to_tid<t_sequence, ValueType> t_temp_1;
      BOOST_STATIC_CONSTANT(int, simple_index = t_temp_1::value);

      typedef BOOST_VARIANT_TL_NS::type_to_tid<t_sequence, incomplete<ValueType> >
         t_temp_2;
      BOOST_STATIC_CONSTANT(int, incomplete_index = t_temp_2::value);

      BOOST_STATIC_CONSTANT(bool, is_simple = simple_index >= 0);
      
      BOOST_STATIC_ASSERT(simple_index >= 0 || incomplete_index >= 0);
      assign(value, detail::variant::int_to_type<is_simple>());

   }
//#endif // !BOOST_VARIANT_NO_ASSIGN_HELPER


private:

   enum assign_vv_opt { e_extract, e_simple, e_incomplete };

   template<class SrcType>
   void assign_vv(const SrcType& src, 
      detail::variant::int_to_type<e_extract> ) //ES
   {
      detail::variant::variant_to_variant_copier<t_self> copier;
      copier.trg_p_ = this;

      apply_visitor(copier, src);
   }

   template<class SrcType>
   void assign_vv(const SrcType& src, 
      detail::variant::int_to_type<e_simple> )  //ES
   {
      holder_ = src;
   }

   template<class SrcType>
   void assign_vv(const SrcType& src, 
      detail::variant::int_to_type<e_incomplete> ) //ES
   {
      holder_ = incomplete<SrcType>(src);
   }

   template<class OtherType>
   void assign_vv(const OtherType& other)  //ES
   {
   
        
#ifdef __MWERKS__
      using detail::variant::is_variant;
      BOOST_STATIC_ASSERT(
         sizeof(is_variant::check(&other)) == sizeof(is_variant::yes) );
        
#else
      BOOST_STATIC_ASSERT(detail::variant::is_variant<OtherType>::value);
      
#endif //__MWERKS__

      typedef OtherType t_other;

      typedef BOOST_VARIANT_TL_NS::type_to_tid<t_sequence, t_other> t_temp_1;
      BOOST_STATIC_CONSTANT(int, simple_index = t_temp_1::value);

      typedef BOOST_VARIANT_TL_NS::type_to_tid<t_sequence, incomplete<t_other> > 
         t_temp_2;
      BOOST_STATIC_CONSTANT(int, incomplete_index = t_temp_2::value);

      //cond_b:
      //
      //true -    t_other does not appear on t_sequence. Hence, other's
      //          held value should be copied into *this
      //
      //false -   t_other does appear on t_sequence. other will be copied,
      //          as-is, into *this
      BOOST_STATIC_CONSTANT(bool,
         cond_b = (simple_index < 0) &&  (incomplete_index < 0));

      const assign_vv_opt opt = cond_b ? e_extract 
         : simple_index >= 0 ? e_simple : e_incomplete;

     assign_vv(other, detail::variant::int_to_type<opt>());
   }


#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER

   //
   // Define the flat_assign_helper structure
   // Simulates overloading resolution thru BOOST_PP_* hacks
   //

//   template<typename ParentType, BOOST_VARIANT_FLAT_HELPER_PARAMS>

   struct flat_assign_helper
   {
      BOOST_PP_REPEAT(BOOST_VARIANT_LIMIT_TYPES, 
         BOOST_VARIANT_SET_ITEM_MOLD, aaa)
   }; //flat_assign_helper

#endif //BOOST_VARIANT_NO_ASSIGN_HELPER


   template<class ValueType>
   void ctor_helper(const ValueType& other, 
      detail::variant::int_to_type<true> )
   {
      //Variant to variant initialization
      assign_vv(other);
   }


   template<class ValueType>
   void ctor_helper(const ValueType& value, 
      detail::variant::int_to_type<false> )
   {
      //Value to variant initialization, 

#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER

      flat_assign_helper::set_item(this, value);     
      
#else

      //
      //[Compile time error on the following line:]
      //
      //The given value cannot be converted to any acceptable type
      //
      //Note that a compiler *warning* on that line, may 
      //indicate loss of data (e.g: double -> int)
      //
      //
      
      detail::variant::assign_helper<seq_size, t_sequence, t_self>
         ::set_item(this, value);



#endif //BOOST_VARIANT_NO_ASSIGN_HELPER
   }


public:
   ~variant() { }

   variant()  { }


   template<class ValueType>
   variant(const ValueType& value) 
   {
#ifdef __MWERKS__
      using detail::variant::is_variant;

      BOOST_STATIC_CONSTANT( bool, value_is_variant =  
        (sizeof(is_variant::check(&value)) == sizeof(is_variant::yes)) );
#else
      BOOST_STATIC_CONSTANT(bool, value_is_variant = 
        detail::variant::is_variant<ValueType>::value);

#endif //__MWERKS__
         
      ctor_helper(value, detail::variant::int_to_type<value_is_variant>());
   }


   variant(const t_self& other) //ES
   {
      //variant to variant copy.

      //'e_extract' -> copy other's held value
      //This makes sure that a variant will never hold itself, effectively
      //prohibiting direct recursion
      assign_vv(other, detail::variant::int_to_type<e_extract>());
   }


   void swap(t_self& other)   //ES
   {
      holder_.swap(other.holder_);
   }


   template<class ValueType> //ES
   t_self& operator=(const ValueType& v)
   {
      t_self temp(v);
      swap(temp);

      return *this;
   }


   t_self& operator=(const t_self& rhs)  //ES
   {
      std::cout << "****** copy assignment ******\n";
      t_self temp(rhs);
      swap(temp);

      return *this;
   }



   //Again, private_tag is used to make this member function private
   template<typename T>
   T* get_ptr(type_to_type<T>, detail::variant::private_tag ) const
   {
      typedef BOOST_VARIANT_TL_NS::type_to_tid<t_sequence, T> t_temp;
      BOOST_STATIC_CONSTANT(int, index = t_temp::value);


      //
      //[Static assertion failure:]
      //
      //The specified type is not part of this variant's set of types
      //
      BOOST_STATIC_ASSERT(index >= 0);

      if(index != tid())
         return 0;

      return static_cast<T*>(const_cast<void*>(holder_.get_ptr()));
   }

public:

   const std::type_info& type() const
   {
      detail::variant::type_info_extractor tie;
      apply_visitor(tie, *this);

      assert(tie.info_p_ != 0);
      return *tie.info_p_;
   }

   bool empty() const
   {
      return true;
   }

// private:

   t_storage holder_;

}; //variant



struct variant_cast_error : std::bad_cast
{
   virtual const char * what() const throw()
   {
      return "boost::variant_cast_error: "
         "failed conversion using boost::variant_cast";
   }  

}; //variant_cast_error


//
//Use type_to_type<T> to overcome MSVC related problems
//

template<typename T, BOOST_VARIANT_PARAMS>
T* variant_cast(type_to_type<T>, variant<BOOST_VARIANT_PARAMS_USAGE>* var_p)
{
   return var_p->get_ptr(type_to_type<T>(), detail::variant::private_tag());
}

template<typename T, BOOST_VARIANT_PARAMS>
const T* variant_cast(type_to_type<T>, const variant<BOOST_VARIANT_PARAMS_USAGE>* var_p)
{
   typedef variant<BOOST_VARIANT_PARAMS_USAGE> t_temp;
   return variant_cast<T>(const_cast<t_temp*>(var_p));
}

template<typename T, BOOST_VARIANT_PARAMS>
T variant_cast(type_to_type<T>, const variant<BOOST_VARIANT_PARAMS_USAGE>& var)
{
    const T* result = variant_cast<T>(&var);
    if(!result)
        throw variant_cast_error();
    return *result;
}


//
//Same functions with a more natural syntax
//

template<typename T, BOOST_VARIANT_PARAMS>
T* variant_cast(variant<BOOST_VARIANT_PARAMS_USAGE>* var_p)
{
   return variant_cast(type_to_type<T>(), var_p);
}

template<typename T, BOOST_VARIANT_PARAMS>
const T* variant_cast(const variant<BOOST_VARIANT_PARAMS_USAGE>* var_p)
{
   return variant_cast(type_to_type<T>(), var_p);
}

template<typename T, BOOST_VARIANT_PARAMS>
T variant_cast(const variant<BOOST_VARIANT_PARAMS_USAGE>& var)
{
   return variant_cast(type_to_type<T>(), var);
}



} //namespace boost


//////////////////////////////////////////////////////////////////////

/*
 * This source code is based on the following sources:
 *
 * [1] Andrei Alexandrescu, "An implementation of discriminated unions in C++",
 *    http://www.oonumerics.org/tmpw01/alexandrescu.pdf
 *
 * [2] Doug Gregor, "BOOST_USER: variant"
 *    http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?variant
 *
 * [3] Kevlin Henney, boost::any
 *    http://www.boost.org
 *
 * [4] Eric Friedman, "Variant (aka Discriminated Union) implementation"
 *    http://groups.yahoo.com/group/boost/files/variant-20020701.zip
 *                                                     -yyyymmdd.zip
 *
 */


//////////////////////////////////////////////////////////////////////

/*
 * Change log:
 *
 * 1. Various fixes suggested by Giovanni Bajo due to Comeau/MSVC 7 errors 
 *    (10-Jul-02)
 *
 *
 */


#endif //_VARIANT_HPP_INC_
