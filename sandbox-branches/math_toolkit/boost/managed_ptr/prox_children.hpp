//  (C) Copyright Larry Evans 2003.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_MANAGED_PTR_PROX_CHILDREN_HPP_LJE20031018
#define BOOST_MANAGED_PTR_PROX_CHILDREN_HPP_LJE20031018
#include "boost/iterator_default/range.hpp"
#include <vector>
#include <map>
#define THE_MAKER_CONTAINER_TYPEINFO
#ifdef THE_MAKER_CONTAINER_TYPEINFO
#include <typeinfo>
#endif
namespace boost
{
namespace managed_ptr
{

      inline
    char*
  cp_cast(void* vp)
    //.<Purpose:
    //.<  A "convenience" function to convert
    //.<  T* to char* for any T.
    { return static_cast<char*>(vp);
    } 
    
  struct mem_buffer
   /**@class mem_buffer
    * @brief
    *  Memory buffer for use by placement new in
    *  a descriptor_builder<Subject>, for some Subject.
    */
    {
    public:
          typedef
        mem_buffer
      my_type
        ;
      ~mem_buffer(void)
        { delete [] m_buffer;
        }  
      mem_buffer(std::size_t a_size)
        : m_size(a_size)
        , m_buffer(m_size?new char[m_size]:0)
        {}
        my_type const&
      operator=(my_type const& a_buf)
        { resize(a_buf.size());
          std::copy(a_buf.m_buffer, a_buf.m_buffer+a_buf.size(), m_buffer);
          return *this;
        } 
        void
      resize(std::size_t a_size)
        {
          if(a_size > m_size)
          { 
            delete [] m_buffer;
            m_size = a_size;
            m_buffer = new char[m_size];
          } ;
        } 
        char const*
      start_con(void)const
        { return m_buffer;
        } 
        char*
      start_mut(void)const
        { return m_buffer;
        } 
        std::size_t
      size(void)const
        { return m_size;
        } 
    private:
        std::size_t
      m_size
        ;
        char*
      m_buffer
        ;
    };//end mem_buffer struct
    
template
  < typename ProxVisitor
  >
  class 
prox_children
{
 public:

      typedef 
    ProxVisitor 
  prox_visitor_type
  ;
      typedef 
    void 
  prox_subj_type
  ;
  
    class
  prox_iterator_acceptor_abs
    //.<Purpose:
    //.<  An abstract "iterator-acceptor" over proxies 
    //.<  contained in a subject.  
    //.<Rationale:
    //.<  The combination of iterator with acceptor is done
    //.<  to avoid having the acceptor method as part
    //.<  of the proxy.  This was done to avoid the
    //.<  extra memory overhead of a virtual function
    //.<  table pointer in each proxy instead of just
    //.<  in the iterator.  Concrete derived classes
    //.<  of this class will be specialized on the
    //.<  type of proxy.
    //.<Pragmatics:
    //.<  The reason why this class is defined here
    //.<  instead of inside the prox_children<> class is
    //.<  to allow the actual ProxVisitor class interface
    //.<  to be defined without needing to reference a
    //.<  prox_chilren<> nested class.
  {
   public:
          virtual
      ~prox_iterator_acceptor_abs(void)
      {}
          virtual
        void
      accept(prox_visitor_type& a_visitor)const
      /**
       *@brief apply a_visitor to current
       *  proxy and its children
       */
      =0
      ;
          virtual
        void
      increment(void)
      /**
       *@brief go to the next proxy
       */     
       =0
       ;
          
          virtual
        std::size_t
      size(void)const
      //.Purpose:
      //.  Indicates number of elements accessible
      //.  by this iterator.
      =0
      ;
          virtual
        bool
      empty(void)const
      //.Purpose:
      //.  Indicates whether no more increments
      //.  are valid and accept is undefined.
      =0
      ;
        void
      operator++(void)
      { increment();
      } 
  }
  ;//end prox_iterator_acceptor_abs struct
  
  struct maker_proxiter_abs
    {
          typedef
        std::size_t
      offset_type
        ;
          typedef
        std::vector<offset_type>
      prox_offsets_type
        ;
          virtual
        prox_iterator_acceptor_abs*
      make_proxiter_subj
        ( prox_subj_type* a_subj
        , prox_offsets_type const& a_prox_offsets
        , mem_buffer& a_buffer
        )const
       /**
        * @brief Return prox_iterator_acceptor_abs for a_subj at a_prox_offsets.
        * 
        *  Return a prox_iterator_acceptor_abs for a
        *  proxy (either prox_scalar or
        *  prox_indirect) at the given offset
        *  from a subject containing the proxy.
        *
        *  The returned prox_iterator_acceptor_abs is created in
        *  a_buffer
        */
        =0
        ;
          virtual
        std::size_t
      iter_size(void)const
       /**
        * @brief Return the maximum size of iterator.
        */
        =0
        ;
    };//end maker_proxiter_abs struct
  struct prox_descriptor
    : public std::map<maker_proxiter_abs const*, typename maker_proxiter_abs::prox_offsets_type>
   /**@class prox_descriptor
    * @brief Describes the location of proxies in a class.
    *
    *  For each such proxy, there's a maker_proxiter_abs which
    *  creates a prox_iterator_acceptor_abs for proxies contained in that
    *  proxy.  For example, the prox_iterator_acceptor_abs for a prox_scalar
    *  merely iterates over a single proxy, the prox_scalar.  
    *  OTOH, the prox_iterator_acceptor_abs for a prox_indirect iterates over
    *  all the proxies contained in the container where the
    *  prox_indirect is located.
    */
    {
          typedef
        typename maker_proxiter_abs::prox_offsets_type
      offsets_type
        ;
          typedef
        std::map<maker_proxiter_abs const*, offsets_type>
      super_type
        ;
          typedef
        typename super_type::value_type
      mker_offs_type
        ;
          typedef
        prox_descriptor
      my_type
        ;
      prox_descriptor(void)
        : m_iter_max_size(0)
        {}
    public:
        void
      append_maker_at
        ( maker_proxiter_abs const* a_maker
        , typename offsets_type::value_type a_offset
        )
       /**@brief 
        *  Append a_maker at a_offset
        */
        { 
          offsets_type& a_offsets = super_type::operator[](a_maker);
          a_offsets.push_back(a_offset);
          std::size_t l_size = a_maker->iter_size();
          if(l_size > m_iter_max_size) m_iter_max_size = l_size;
        } 
        offsets_type const*
      find_maker_offsets
        ( maker_proxiter_abs const* a_maker
        )const
        { typename super_type::const_iterator found(super_type::find(a_maker));
          if(found == super_type::end()) return 0;
          mker_offs_type const& l_mker_offs = *found;
          offsets_type const& l_offs = l_mker_offs.second;
          return &l_offs;
        } 
        std::size_t
      iter_max_size(void)const
        { return m_iter_max_size;
        } 
    private:
        std::size_t
      m_iter_max_size
      /**@brief
        * maximum size of iterators created by
        * maker_proxiter_abs's in the map.
        */   
        ;
    };//end prox_descriptor struct
    
    struct 
  builder_materials
   /**@class builder_materials
    * @brief Provide a reference to the "building materials"
    *   for building the prox_descriptor superclass of
    *   a descriptor_builder<Subject>, for some Subject.
    *
    *   This reference is used by proxies contained by
    *   Subject to record their presence.
    */
    {
      struct build_buffer
        : public mem_buffer
        {
        public:
          build_buffer(std::size_t a_size)
            : mem_buffer(a_size)
            {}
            bool
          contains(void* a_loc)const
          /**@brief
            * Is a_loc within this mem_buffer?
            */
            { char* c_loc = static_cast<char*>(a_loc);
              char const* c_start = start_con();
              bool r =(c_start <= c_loc) && (c_loc < c_start+size());
              return r;
            } 
        };//end build_buffer struct
      struct build_materials
      /**@class build_materials
        * @brief
        *  Simply a convenience class
        *  to hold all build materials
        *  for building a prox_descriptor.
        */
        {
        public:
            prox_descriptor*
          m_descriptor
            ;
            build_buffer*
          m_buffer
            ;
        public:
          build_materials
            ( prox_descriptor* a_descriptor=0
            , build_buffer* a_buffer=0
            )
            : m_descriptor(a_descriptor)
            , m_buffer(a_buffer)
            {}
        };//end build_materials struct
          static
        build_materials&
      the_materials(void)
      /**@brief
        * The single instance of build_materials
        * used to make all prox_descriptor's.
        */
        { static build_materials a_materials;
          return a_materials;
        } 
        
      template
        < typename Container
        >
        struct 
      the_container_maker
        {
            struct
          maker_proxiter_con
            ;
              static
            maker_proxiter_abs const*
          the_maker(void)
            ;
        };//end the_container_maker<Container>

      struct make_recorder
        {
        template
          < typename Container
          >
        make_recorder
          ( Container* a_container
          )
         /**
          * Record the maker_proxiter_abs for given Container
          * in the descriptor for the Subject currently being
          * constructed in a descriptor_builder<Subject> 
          * instantiation.
          */
          {
            if
            (  the_materials().m_buffer
            && the_materials().m_buffer->contains(this)
            )
            {
              maker_proxiter_abs const* the_maker = the_container_maker<Container>::the_maker();
              #if 0 && defined(TRACE_SCOPE_HPP)
              utility::trace_scope ts("template<typename Container>make_recorder(Container*)::CTOR");
              #ifdef THE_MAKER_CONTAINER_TYPEINFO
              //This output helps determine whether correct Container is being used.
              //A Container is "correct" if there's some method:
              //  ProxVisitor::visit_prox(Container&)
              ; mout()<<"Container="<<typeid(Container).name()<<"\n";
              #endif
              #endif
              if(the_maker != 0)
              {
                char const* c_subj = the_materials().m_buffer->start_con();
                char const* c_cont = cp_cast(a_container);
                maker_proxiter_abs::prox_offsets_type::value_type offset_cont_subj 
                = c_cont - c_subj;
                the_materials().m_descriptor->append_maker_at
                ( the_maker 
                , offset_cont_subj
                );
              } ;
            } ;
          } //end make_recorder CTOR
        };//end make_recorder struct
    };//end builder_materials struct
  template<typename Subject>
  struct prox_description_of
  /**
    * @class prox_descriptor_of
    * @brief
    *   Return null prox_descriptor* of Subject class.
    *   Specializations for each Subject which
    *   has children should return non-null values.
    */
    {
    public:
          static
        prox_descriptor const*
      ptr(void)
         { return 0 /** default value is null, i.e. no children.*/;
         } 
    };//end prox_description_of<Subject> template
    
  template<typename Subject>
  struct descriptor_builder
    //.<Purpose:
    //.<  "Build" the prox_descriptor for Subject class
    //.<  which MIGHT have children.
    : public prox_descriptor
    , private builder_materials
    {
    private:
          static descriptor_builder the_builder;
          
    public:
      descriptor_builder(void)
      {
      #if defined(TRACE_SCOPE_HPP)
          utility::trace_scope ts("descriptor_builder::CTOR");
      #endif
          typename builder_materials::build_buffer subj_mem(sizeof(Subject));
          builder_materials::the_materials() = builder_materials::build_materials(this,&subj_mem);
          Subject* a_subj = new (subj_mem.start_mut()) Subject;
          {
          #if defined(TRACE_SCOPE_HPP)
              utility::trace_scope ts("descriptor_builder::CTOR.~Subject()");
          #endif
          //    a_subj->~Subject();
          }
          builder_materials::the_materials() = builder_materials::build_materials();
      }
          
          static
        prox_descriptor const*
      ptr(void)
      { 
          return &the_builder;
      } 
    };//end descriptor_builder struct template
    
  struct detail_iterators
    {
    
    struct maker_iterator_described
     /**@class maker_iterator_described
      * @brief 
      *  Iterator over elements in a prox_descriptor.
      *  Deref returns the corresponding prox_iterator_acceptor_abs* 
      *  for that element and m_subj.
      */
      {
      public:
            typedef
          typename prox_descriptor::const_iterator
        iter_desc_type
          ;
            typedef
          iterator_default::range<iter_desc_type>
        iter_type
          ;
            typedef
          maker_iterator_described
        my_type
          ;
        maker_iterator_described
          ( void
          )
          : m_subj(0)
      	  , m_iter()
          {}
        maker_iterator_described
          ( void* a_subj
          , prox_descriptor const*const a_desc //the prox_descriptor for a_subj
          )
          : m_subj(a_subj)
          , m_iter
            ( (m_subj && a_desc)
              ? iter_type(a_desc->begin(),a_desc->end())
              : iter_type() 
            )
          {}
          my_type const&
        operator=(my_type const& a_iter)
          { m_subj = a_iter.m_subj;
            m_iter = a_iter.m_iter;
            return *this;
          }  
          prox_iterator_acceptor_abs*
        deref(mem_buffer& a_buffer)const
          { typename prox_descriptor::mker_offs_type const& l_mker_offs = m_iter.deref();
            prox_iterator_acceptor_abs* l_iter 
            = l_mker_offs.first->make_proxiter_subj
              ( m_subj
              , l_mker_offs.second
              , a_buffer
              );
            return l_iter;
          } 
          void
        increment(void)
          { m_iter.increment();
          } 
          std::size_t
        size(void)const
          { iter_type mt(m_iter);
            std::size_t n=0;
            for(;!mt.empty(); mt.increment(),++n);
            return n;
          } 
          bool
        empty(void)const
          { 
            return m_iter.empty();
          } 
      private:
          prox_subj_type*
        m_subj
          ;
          iter_type
        m_iter
          ;
      };//end maker_iterator_described struct
    struct prox_iterator_described
      : public maker_iterator_described
     /**@class prox_iterator_described
      * @brief 
      *  A prox_iterator_acceptor_abs for a subject described by
      *  a prox_descriptor.
      */
      {
        struct prox_iterator_acceptor_fwd
         /**@class prox_iterator_acceptor_fwd
          * @brief 
          *  Simple smart pointer for prox_iterator_acceptor_abs
          *  which just forwards calls to the pointee.
          *  Also, defines empty for null pointer in super_type.
          */
          {
          public:
                typedef
              prox_iterator_acceptor_fwd
            my_type
              ;
            prox_iterator_acceptor_fwd
              ( std::size_t a_size
              )
              : m_buffer(a_size)
              , m_piter(0)
              {}
              void
            accept(prox_visitor_type& a_visitor)const
              { 
                m_piter->accept(a_visitor);
              } 
              void
            increment(void)
              {
                m_piter->increment();
              } 
              std::size_t
            size(void)const
             /**@class prox_iterator_acceptor_fwd
              * @pre-conditions
              *  neither this->increment or some derived class increment
              *  is executing.
              */
              { std::size_t r = 0;
                if(!empty())
                { r = m_piter->size();
                } ;
                return r;
              } 
              bool
            empty(void)const
              { 
                bool r = (m_piter == 0);
                if( !r )
                { r =  m_piter->empty();
                } ;
                return r;
              } 
              void
            operator++(void)
              { increment();
              } 
              my_type const&
            operator=( maker_iterator_described& a_maker)
              { m_piter = a_maker.deref(m_buffer);
                return *this;
              } 
          private:
              my_type const&
            operator=( my_type const& a_iter)
              //no need to define because not used
              ;
              mem_buffer
            m_buffer
              ;
              prox_iterator_acceptor_abs*
            m_piter
              ;
          };//end prox_iterator_acceptor_fwd
      
            typedef
          prox_iterator_acceptor_fwd
        iter_type
          ;
            typedef
          maker_iterator_described
        super_type
          ;
            typedef 
          prox_subj_type
        subj_type
          ;
            typedef
          prox_iterator_described
        my_type
          ;
      public:
        prox_iterator_described
          ( subj_type* a_subj=0 //the subject for which the iterator is made.
          , prox_descriptor const*const a_desc=0 //the prox_descriptor for a_subj
          )
          : super_type(a_subj, a_desc)
          , m_iter((a_subj && a_desc)?a_desc->iter_max_size():0)
          {
            first_non_empty();
          } 
          void
        accept(prox_visitor_type& a_visitor)const
          { m_iter.accept(a_visitor);
          } 
          void
        increment(void)
          { 
            m_iter.increment();
            next_non_empty();
          } 
      private:
          my_type const&
        operator=(my_type const& a_iter)
          //No need to define because not used
          ;
          void
        first_non_empty(void)
         /**Find first non empty iter_type in super_type
          */
          {
            super_type& l_super = *this;
            if (!l_super.empty())
            { 
              m_iter = l_super;
              next_non_empty();
            } ;
          } 
          void
        next_non_empty(void)
         /**Find next non empty iter_type in super_type
          */
          {
            super_type& l_super = *this;
            while (m_iter.empty())
            { 
              l_super.increment();
              if(l_super.empty()) break;
              m_iter = l_super;
            } ;
          } 
          iter_type
        m_iter
          //the current prox_iterator_acceptor_abs*
          ;
      };//end prox_iterator_described
    struct prox_iterator_con_described
     /**@class prox_iterator_described
      * @brief 
      *  A prox_iterator_acceptor_abs implemented by
      *  a prox_iterator_described.
      */
      : public prox_iterator_acceptor_abs
      , public prox_iterator_described
      {
      public:
            typedef
          prox_iterator_described
        super_type
          ;
            typedef
          typename super_type::subj_type
        subj_type
          ;
        prox_iterator_con_described
          ( subj_type* a_subj
          , prox_descriptor const*const a_desc //the prox_descriptor for a_subj
          )
          : super_type(a_subj, a_desc)
          {
          } 
        prox_iterator_con_described
          ( prox_iterator_con_described const& a_piter
          )
          : super_type(a_piter)
          {
          } 
          void
        accept(prox_visitor_type& a_visitor)const
          { super_type::accept(a_visitor);
          } 
          void
        increment(void)
          { super_type::increment();
          } 
          std::size_t
        size(void)const
          { return super_type::size();
          } 
          bool
        empty(void)const
          { return super_type::empty();
          } 
      };//end prox_iterator_con_described
    template
      < typename Subject
      >
    struct prox_iterator_of
      : public prox_iterator_described
     /**@class prox_iterator_of<Subject>
      * A "general" template producing a prox_iterator_described 
      * for the given Subject.
      */
      {
      public:
            typedef
          Subject
        subj_type
          ;
            typedef
          prox_iterator_described
        super_type
          ;
            typedef
          prox_iterator_of<subj_type>
        my_type
          ;
        prox_iterator_of
          ( void
          )
          : prox_iterator_described()
          {}
        prox_iterator_of
          ( subj_type* a_subj
          )
          : prox_iterator_described(a_subj, prox_description_of<subj_type>::ptr())
          {}
          my_type const&
        operator=(my_type const& a_iter)
          { super_type::operator=(a_iter);
            return *this;
          }  
          my_type const&
        operator=(subj_type& a_subj)
          {
            prox_descriptor const* prox_desc(prox_description_of<subj_type>::ptr());
            super_type::operator=(prox_iterator_described(&a_subj, prox_desc));
            mout()<<"prox_iterator_of<Subject>::operator=(Subject& a_subj):prox_desc="<<prox_desc<<"\n";
            return *this;
          }  
      };//end prox_iterator_of<Subject> template
    template
      < typename Subject
      >
    struct prox_iterator_con_of
     /**@class prox_iterator_con_of<Subject>
      * @brief
      *  A  prox_iterator_acceptor_abs implemented by
      *  a prox_iterator_of<Subject>
      */
      : public prox_iterator_acceptor_abs
      , public prox_iterator_of<Subject>
      {
      public:
            typedef
          prox_iterator_of<Subject>
        super_type
          ;
            typedef
          typename super_type::subj_type
        subj_type
          ;
        prox_iterator_con_of
          ( void
          )
          : super_type()
          {}
        prox_iterator_con_of
          ( subj_type* a_subj
          )
          : super_type(a_subj)
          {}
          void
        accept(prox_visitor_type& a_visitor)const
          { super_type::accept(a_visitor);
          } 
          void
        increment(void)
          { super_type::increment();
          } 
          std::size_t
        size(void)const
          { return super_type::size();
          } 
          bool
        empty(void)const
          { return super_type::empty();
          } 
      };//end prox_iterator_con_of<Subject>
    template
      < typename Container
      >
    struct prox_container_iterator
      : public iterator_default::range<typename Container::iterator>
      {
      public:
            typedef
          iterator_default::range<typename Container::iterator>
        super_type
          ;
            typedef
          typename super_type::value_type
        value_type
          ;
        prox_container_iterator
          ( void
          )
          {}
        prox_container_iterator
          ( Container& a_container
          )
          : super_type(range_iter_type(a_container.begin(),a_container.end()))
          {
          } 
          void
        accept(prox_visitor& a_visitor)const
          { 
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("prox_container_iterator::accept(prox_visitor&)");
            #endif
            value_type& l_val = super_type::deref();
            a_visitor.visit_prox(l_val);
          } 
      };//end prox_container_iterator<Container> template
    struct offsets_iterator
     /**@class offsets_iterator
      * @brief iterator over offsets_type in a prox_offsets_type.
      */
      : public iterator_default::range<maker_proxiter_abs::prox_offsets_type::const_iterator>
      {
            typedef
          maker_proxiter_abs::prox_offsets_type::const_iterator
        iter_rng_type
          ;
            typedef
          iterator_default::range<iter_rng_type>
        super_type
          ;
      public:
        offsets_iterator(typename maker_proxiter_abs::prox_offsets_type const& a_prox_offsets)
          : super_type(a_prox_offsets.begin(), a_prox_offsets.end())
          {}
      };//end offsets_iterator struct
    template
      < typename Member
      >
    struct offset_member_iterator
      : public offsets_iterator
     /**
      *@class offset_member_iterator
      *@brief An iterator over instances of Member located 
      *  at offsets in superclass from subject, m_subj,
      *  of this class.
      *
      *  Each such member is either a prox_offset_typed<T>,
      *  for some T, or a container of some type with a member
      *  of type prox_indirect_typed<T>, for some T.
      */
      {
      public:
            typedef
          Member
        value_type
          ;
            typedef
          offsets_iterator
        super_type
          ;
        offset_member_iterator
          ( prox_subj_type* a_subj //the object containing Members
          , typename maker_proxiter_abs::prox_offsets_type const& 
            a_prox_offsets //the offsets of Members from a_subj
          )
          : super_type(a_prox_offsets)
          , m_subj(static_cast<char*>(a_subj))
          {}
          value_type&
        deref(void)const
          { 
            maker_proxiter_abs::prox_offsets_type::value_type l_offset = super_type::deref();
            void const* v_memb = m_subj + l_offset;
            value_type const* p_memb = static_cast<value_type const*>(v_memb);
            value_type& r_memb = *const_cast<value_type*>(p_memb);
            return r_memb;
          } 
      private:
          char const*
        m_subj
          ;
      };//end offset_member_iterator template struct
    template
      < typename Member
      >
    struct prox_member_iterator
      : public prox_iterator_acceptor_abs
      , public offset_member_iterator<Member>
     /**
      *@class prox_member_iterator
      *@brief
      *  An iterator over proxies of all Member's iterated over
      *  by superclass.
      */
      {
      public:
            typedef
          Member
        value_type
          ;
            typedef
          offset_member_iterator<value_type>
        super_type
          ;
            typedef
          prox_member_iterator<value_type>
        my_type
          ;
        prox_member_iterator
          ( prox_subj_type* a_subj
          , typename maker_proxiter_abs::prox_offsets_type const& a_prox_offsets
          )
          : super_type(a_subj,a_prox_offsets)
          {}
          void
        accept(prox_visitor_type& a_visitor)const
          { value_type& l_val = this->super_type::deref();
            a_visitor.visit_prox(l_val);
          } 
          void
        increment(void)
          { super_type::increment();
          } 
          std::size_t
        size(void)const
          { my_type mt(*this);
            std::size_t n=0;
            for(;!mt.empty(); mt.increment(),++n);
            return n;
          } 
          bool
        empty(void)const
          { return super_type::empty();
          } 
      };//end prox_member_iterator general template
      
    };//end detail_iterators struct

};//end prox_children struct    

template<typename ProxVisitor>
template<typename Subject>
prox_children<ProxVisitor>::
  descriptor_builder<Subject>
prox_children<ProxVisitor>::
  descriptor_builder<Subject>
    ::the_builder
    //Causes building of a ProxVisitor descriptor for Subject.
    ;

template
  < typename ProxVisitor
  >
      template
        <typename Container
        >
            struct
prox_children
  < ProxVisitor
  >::
  builder_materials::
      the_container_maker
        < Container
        >::
          maker_proxiter_con
            : public maker_proxiter_abs
            {
                  typedef
                ProxVisitor
              visitor_type
              ;
                  typedef
                Container
              container_type
              ;
                  typedef
                detail_iterators::prox_member_iterator<container_type>
              prox_iterator_con
              ;
                typename prox_children<visitor_type>::prox_iterator_acceptor_abs*
              make_proxiter_subj
                ( typename prox_children<visitor_type>::prox_subj_type* a_subj
                , typename maker_proxiter_abs::prox_offsets_type const& a_prox_offsets
                , mem_buffer& a_buffer
                )const
                {
                ; assert(iter_size() <= a_buffer.size() )
                ; return new (a_buffer.start_mut()) prox_iterator_con(a_subj,a_prox_offsets)
                ;}
                std::size_t
              iter_size(void)const
                { return sizeof(prox_iterator_con)
                ;}
            };//end maker_proxiter_con struct

template
  < typename ProxVisitor
  >
      template
        <typename Container
        >
//              static
typename prox_children<ProxVisitor>::
            maker_proxiter_abs const*
prox_children<ProxVisitor>::
  builder_materials::
      the_container_maker
        < Container
        >::
          the_maker(void)
            {
            ; static maker_proxiter_con a_maker
            ; return &a_maker
            ;}
            
}//exit managed_ptr namespace
}//exit boost namespace

//The following declares that class, REFERENT, has a proxy children
//that can be visited by VISITOR.
#define DECLARE_PROX_CHILDREN_FOR_REFERENT_VISITOR(REFERENT,VISITOR) \
namespace boost{namespace managed_ptr{ \
template<> template<> \
  struct prox_children<VISITOR> \
    ::prox_description_of<REFERENT> \
  : public prox_children<VISITOR> \
    ::descriptor_builder<REFERENT> \
  {}; \
}}//exit boost::managed_ptr namespace

#endif
