//defines fields_visitor namespace
#ifndef BOOST_FIELDS_VISITOR_FIELDS_VISITOR_HPP_LJE20031018
#define BOOST_FIELDS_VISITOR_FIELDS_VISITOR_HPP_LJE20031018
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
/**@file fields_visitor.hpp
 *  @brief
 *    Defines namespace which enables "visiting" of selected "fields" 
 *    in a "record".  See Terminology under namespace fields_visitor
 *    for definition of these terms.
 */
#include "boost/rangelib/range.hpp"
//^ from http://www.torjo.com/code/rangelib15.zip on 2004-11-09
#include <boost/type_traits/is_base_and_derived.hpp>
#include <vector>
#include <map>
namespace boost
{
namespace fields_visitor
 /**@brief
  *  Enables "visiting" "selected fields" of instances of a "record" class.
  *
  *  Terminology:
  *    record - a aggregate containing "field"s.
  *    field - an instance variable forming part of a "record".
  */
{
      typedef 
    void
  record_top_type
   /**@brief
    *  A more meaningful name than simply void.
    *  Void is used since the record's selected fields are
    *  accessed simply by using offsets from the
    *  start of the record.
    */
  ;
  
namespace detail
{
      inline
    char*
  cp_cast(void* vp)
   /**@brief
    *  A convenience function to convert T* to char* for any T.
    */
  { 
      return static_cast<char*>(vp);
  } 
      inline
    char const*
  cp_cast(void const* vp)
   /**@brief
    *  A convenience function to convert T const* to char const* for any T.
    */
  { 
      return static_cast<char const*>(vp);
  } 
      typedef
    std::size_t
  offset_type
  ;
      inline
    offset_type
  offset_from_to(void const* a_from, void const* a_to)
   /**@brief
    *  A convenience function to calculate the offset, a_from - a_to.
    */
  { 
      offset_type l_offs= cp_cast(a_from) - cp_cast(a_to);
      return l_offs;
  }
  
  struct mem_buffer
   /**@brief
    *  Simple memory buffer.
    */
  {
   public:
          typedef
        mem_buffer
      my_type
      ;
      ~mem_buffer(void)
      { 
          delete [] m_buffer;
      }  
      mem_buffer(std::size_t a_size)
        : m_size(a_size)
        , m_buffer(m_size?new char[m_size]:0)
      {}
        my_type const&
      operator=(my_type const& a_buf)
      { 
          resize(a_buf.size());
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
          }
      } 
        char const*
      start_con(void)const
      { 
          return m_buffer;
      } 
        char*
      start_mut(void)const
      { 
          return m_buffer;
      } 
        std::size_t
      size(void)const
      { 
          return m_size;
      } 
   private:
        std::size_t
      m_size
      ;
        char*
      m_buffer
      ;
  };//end mem_buffer struct
    
  struct build_buffer
    : public mem_buffer
  {
      build_buffer(std::size_t a_size)
        : mem_buffer(a_size)
        , c_tail_start(start_con())
        , c_tail_end(c_tail_start+a_size)
      {
      }
        template
        < typename Field
        >
        bool
      contains(Field const* a_field)
      /**@brief
        * Is a_field within this mem_buffer?
        */
      { 
          char const*const c_fld_start = cp_cast(a_field);
          bool r =(c_tail_start <= c_fld_start) && (c_fld_start < c_tail_end);
          if(r)
          {
              c_tail_start=c_fld_start+sizeof(Field);//move past this field
          }
          return r;
      } 
   private:
        char const*
      c_tail_start
      //Start of remaining part of Record memory which may contain 
      //fields not already processed by contains(Field*).
      ;
        char const*const
      c_tail_end
      //End of Record memory+1.
      ;
  };//end build_buffer struct
          
      typedef
    std::vector<offset_type>
  field_offsets_type
  ;
      typedef
    rangelib:: crange<detail::field_offsets_type const>
  offsets_iterator
  ;
  template
    < typename SelectedField
    >
  struct offset_field_iterator
   /**@brief An iterator over instances of SelectedField located 
    *  at given offsets ( enumerated by superclass ) 
    *  from record, this->m_record.
    *
    */
    : private offsets_iterator
  {
   public:
          typedef
        SelectedField
      value_type
      ;
          typedef
        offsets_iterator
      super_type
      ;
      offset_field_iterator
        ( record_top_type* a_record //the object containing SelectedFields
        , detail::field_offsets_type const& 
          a_field_offsets //the offsets of SelectedFields from a_record start
        )
        : super_type(a_field_offsets)
        , m_record(static_cast<char*>(a_record))
      {}
        value_type&
      current(void)const
      { 
          detail::field_offsets_type::value_type l_offset 
            = super_type::operator*();
          void* v_memb = m_record + l_offset;
          value_type* p_memb = static_cast<value_type*>(v_memb);
          return *p_memb;
      }
        using super_type::
      operator++
      ;
        using super_type::
      operator bool
      ;
   private:
        char*const
      m_record
       /**@brief
        *  The "record" containing selected fields at offsets 
        *  enumerated by superclass.
        */
      ;
  };//end offset_field_iterator template struct
    
}//exit detail namespace

/** @brief
 *  Define the set of classes used to define the "general" 
 *  version of the template: 
 *    selected_fields_description_of<FieldsVisitor,Record>
 */
template
  < typename FieldsVisitor
  >
struct names_descriptor
{
    class field_iterator_acceptor_abs
    /**@class field_iterator_acceptor_abs
     * @brief
     *   An abstract "iterator-acceptor" over selected
     *   record fields.
     * 
     * Rationale:
     *   The combination of iterator with acceptor is done
     *   to avoid having the acceptor method as part
     *   of the record containing the field.  This was done to avoid the
     *   extra memory overhead of a virtual function
     *   table pointer in each record instead of just
     *   in the iterator.  Concrete derived classes
     *   of this class will be specialized on the
     *   type of record.
     */
    {
     public:
            virtual
        ~field_iterator_acceptor_abs(void)
        {}
            virtual
          void
        accept(FieldsVisitor& a_visitor)const
        /**
         * @brief apply a_visitor to current field.
         */
        =0
        ;
            virtual
          void
        increment(void)
        /**
         * @brief 
         *  Go to the next field.
         */     
         =0
         ;
            
            virtual
          std::size_t
        size(void)const
        /**
         * @brief
         *  Indicates number of elements accessible
         *  by this iterator.
         */
        =0
        ;
            virtual
          bool
        empty(void)const
        /**
         * @brief
         *  Indicates whether no more increments
         *  are valid and accept is undefined.
         */
        =0
        ;
          void
        operator++(void)
        { increment();
        } 
    };//end field_iterator_acceptor_abs struct
    
    class maker_iterator_acceptor_abs
     /**
      * @brief Abstract class for creating field_iterator_acceptor_abs*
      *  which iterates over all fields of a particular type in a record.
      *
      *  IOW, there a 1-to-1 correspondence between the selected field
      *  types in a record and the number of maker_iterator_acceptor_abs* in
      *  that record's selected_fields_descriptor (see below).
      */
    {
     public:
         /**
          * @brief Return field_iterator_acceptor_abs for a_record at a_field_offsets.
          * 
          *  Return a field_iterator_acceptor_abs for the fields, all of the same
          *  type, located at the given offsets from the containing record.
          *
          *  The returned field_iterator_acceptor_abs is created in
          *  a_buffer.
          */
            virtual
          field_iterator_acceptor_abs*
        make_field_iter_from_record_offsets
          ( record_top_type* a_record
          , detail::field_offsets_type const& a_field_offsets
          , detail::mem_buffer& a_buffer
          )const
          =0
          ;
         /**
          * @brief Return the size of mem_buffer needed by
          *  make_field_iter_from_record_offsets.
          */
            virtual
          std::size_t
        iter_size(void)const
          =0
          ;
    };//end maker_iterator_acceptor_abs struct
      
     /**@brief 
      *  Describes the location of SelectedFields in a class.
      *
      *  For a given RECORD class ( a class used as an argument 1
      *  to the SELECTED_FIELDS_DESCRIPTION_OF_RECORD_VISITOR macro )
      *  there's a single instance of this class.  This instance
      *  "describes" the SelectedField's (the arg to CTOR) located within 
      *  the record.
      *
      *  This description contains a maker_iterator_acceptor_abs const*, M, for
      *  each class , SelectedField, contained by RECORD, at the offsets 
      *  given by the value part of the map for that M. M depends on 
      *  SelectedField since its value is:
      *
      *    maker_iterator_acceptor_typed<SelectedField>::the_singleton() 
      *
      *  since it's "registered" in the descriptor by:
      *
      *    field_registrar<FieldsVisitor>::field_registrar<SelectedField>(SelectedField*)
      *
      *  defined below.
      */
    class selected_fields_descriptor
      : public std::map
        < maker_iterator_acceptor_abs const*
        , detail::field_offsets_type 
        >
    {
     public:
            typedef
          detail::field_offsets_type
        offsets_type
        ;
            typedef
          std::map<maker_iterator_acceptor_abs const*, offsets_type>
        super_type
        ;
            typedef
          typename super_type::value_type
        mker_offs_type
        ;
            typedef
          selected_fields_descriptor
        my_type
        ;
        selected_fields_descriptor(void)
          : m_iter_max_size(0)
        {}
            static
          selected_fields_descriptor const&
        the_default(void)
        {
            static selected_fields_descriptor const a_desc;
            return a_desc;
        }
          void
        append_maker_at
          ( maker_iterator_acceptor_abs const* a_maker
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
          ( maker_iterator_acceptor_abs const* a_maker
          )const
        { 
            typename super_type::const_iterator found(super_type::find(a_maker));
            if(found == super_type::end()) return 0;
            mker_offs_type const& l_mker_offs = *found;
            offsets_type const& l_offs = l_mker_offs.second;
            return &l_offs;
        } 
          std::size_t
        iter_max_size(void)const
        { 
            return m_iter_max_size;
        } 
          std::size_t
        num_offsets(void)const
        /**@brief
         *  return sum of sizes of values in map.
         */    
        {
            std::size_t l_total=0; 
            typedef typename super_type::const_iterator iter_type;
            for(iter_type cur=super_type::begin(); cur != super_type::end(); ++cur)
            {
                mker_offs_type const& l_mker_offs = *cur;
                offsets_type const& l_offs = l_mker_offs.second;
                l_total+= l_offs.size();
            }
            return l_total;
        }
     private:
          std::size_t
        m_iter_max_size
        /**@brief
          * maximum size of iterators created by
          * maker_iterator_acceptor_abs's in the map.
          */   
        ;
    };//end selected_fields_descriptor struct
     
};//end names_descriptor

template
  < typename FieldsVisitor
  , typename Record
  >
struct selected_fields_description_of
/**
  * @class selected_fields_description_of
  * @brief
  *   Default (i.e. a general template ) definition of class template 
  *   for accessing the selected_fields_descriptor* for Record.
  *   selected_fields_descriptor is defined in
  *   names_descriptor<FieldsVisitor>.
  *
  *   This default value, null, indicates Record has no 
  *   selected fields.
  *
  *   The class method, ptr(), returns the default value which
  *   can be overridden by defining specializations for Record
  *   with an invokation of macro:
  *
  *     SELECTED_FIELDS_DESCRIPTION_OF_RECORD_VISITOR
  *
  *   which is defined below.
  */
{
 public:
        static
      typename names_descriptor<FieldsVisitor>
        ::selected_fields_descriptor const*
    ptr(void)
    { 
        return 0 /** default value is null, i.e. no children.*/;
    } 
};//end selected_fields_description_of<FieldsVisitor,Record> template
    
/** @brief
 *  Define the set of classes used to define the "general"
 *  version of the template:
 *    descriptor_builder<FieldsVisitor,Record>
 */
template
  < typename FieldsVisitor
  >
  struct
names_builder
  : public names_descriptor<FieldsVisitor>
{
        typedef
      names_descriptor<FieldsVisitor>
    super_ns
    ;
        typedef
      typename super_ns::selected_fields_descriptor
    selected_fields_descriptor
    ;
        typedef
      typename super_ns::field_iterator_acceptor_abs
    field_iterator_acceptor_abs
    ;
        typedef
      typename super_ns::maker_iterator_acceptor_abs
    maker_iterator_acceptor_abs
    ;

    template
      < typename SelectedField
      >
    struct field_iterator_acceptor_typed
     /**@brief
      *  A  field_iterator_acceptor_abs implemented by
      *  a offset_field_iterator<SelectedField>.
      *
      *  Member function, accept, applies a visitor to current SelectedField.
      *  Instantiations of this class determine the overloads for
      *  FieldsVisitor::visit_field method. IOW, for each instantiation
      *  of this template, field_iterator_acceptor_typed<value_type>, there must
      *  be a method, FieldsVisitor::visit_field(value_type&).
      */
      : public field_iterator_acceptor_abs
      , private detail::offset_field_iterator<SelectedField>
    {
     public:
            typedef
          SelectedField
        value_type
        ;
            typedef
          detail::offset_field_iterator<value_type>
        super_type
        ;
            typedef
          field_iterator_acceptor_typed<value_type>
        my_type
        ;
        field_iterator_acceptor_typed
          ( record_top_type* a_record
          , detail::field_offsets_type const& a_field_offsets
          )
          : super_type(a_record,a_field_offsets)
        {}
          void
        accept(FieldsVisitor& a_visitor)const
        { 
            value_type& l_val = this->super_type::current();
            a_visitor.visit_field(l_val);
        } 
          void
        increment(void)
        { 
            super_type::operator++();
        } 
          std::size_t
        size(void)const
        { 
            my_type mt(*this);
            std::size_t n=0;
            for(;!mt.empty(); mt.increment(),++n);
            return n;
        } 
          bool
        empty(void)const
        { 
            return !super_type::operator bool();
        } 
    };//end field_iterator_acceptor_typed template
    
    struct build_materials
    /**@class build_materials
      * @brief
      *  Simply a convenience class to hold all 
      *  build materials for building a 
      *  selected_fields_descriptor for some "record"
      *  i.e. some type.
      */
    {
     public:
          selected_fields_descriptor*
        m_descriptor
          /**@brief 
           * The descriptor being built.
           */
        ;
          detail::build_buffer*
        m_buffer
          /**@brief 
           * memory where "dummy" record, described by m_descriptor
           * is being created.
           */
        ;
          maker_iterator_acceptor_abs const*
        m_record_maker
          /**@brief 
           * The ONLY purpose is to prevent m_descriptor from indicating
           * the record it describes contains itself.
           */
        ;
     public:
        build_materials(void)
          : m_descriptor(0)
          , m_buffer(0)
          , m_record_maker(0)
        {}
        build_materials
          ( build_materials const& a_self
          )
          : m_descriptor(a_self.m_descriptor)
          , m_buffer(a_self.m_buffer)
          , m_record_maker(a_self.m_record_maker)
        {}
          void
        set_desc_buf
          ( selected_fields_descriptor* a_descriptor
          , detail::build_buffer* a_buffer
          , maker_iterator_acceptor_abs const* a_record_maker
          )
        {
            m_descriptor= a_descriptor;
            m_buffer= a_buffer;
            m_record_maker= a_record_maker;
        }
          void
        reset(void)
        {
            m_descriptor= 0;
            m_buffer= 0;
            m_record_maker= 0;
        }
    };//end build_materials struct
      
        static
      build_materials&
    the_materials(void)
    /**@brief
      * The single instance of build_materials
      * used to make a selected_fields_descriptor.
      */
    { 
        static build_materials l_materials;
        return l_materials;
    } 
  
    template
      < typename SelectedField
      >
      struct 
    maker_iterator_acceptor_typed
    : public maker_iterator_acceptor_abs
    {
            typedef
          field_iterator_acceptor_typed<SelectedField>
        field_iterator_type
        ;
          field_iterator_acceptor_abs*
        make_field_iter_from_record_offsets
          ( record_top_type* a_record
          , detail::field_offsets_type const& a_field_offsets
          , detail::mem_buffer& a_buffer
          )const
       /**@brief 
        * Create iterator over SelectedField's in a_record.
        */
        {
            assert(iter_size() <= a_buffer.size() );
            field_iterator_type*l_iter=new (a_buffer.start_mut())
              field_iterator_type(a_record,a_field_offsets);
            return l_iter;
        }
          
          std::size_t
        iter_size(void)const
        { 
            return sizeof(field_iterator_type);
        }
            
            static
          maker_iterator_acceptor_abs const&
        the_singleton(void)
        { 
            static maker_iterator_acceptor_typed l_maker;
            return l_maker;
        }
  
    };//end maker_iterator_acceptor_typed<SelectedField>
  
};//end names_builder<FieldsVisitor> template

/**@brief If building a descriptor, insert:
 *    maker_iterator_acceptor_abs
 *  for CTOR arg type into this descriptor.
 */
template
  < typename FieldsVisitor
  >
struct field_registrar
{
    template
      < typename SelectedField
      >
    field_registrar
      ( SelectedField* a_selected_field
      )
   /**
    * If building a descriptor, record the maker_iterator_acceptor_abs
    * for given SelectedField in the descriptor for the Record
    * currently being constructed in a
    * descriptor_builder<FieldsVisitor,Record> instantiation.
    */
    {
        typedef names_builder<FieldsVisitor> bldr_ns;
        typename bldr_ns::build_materials& l_materials= bldr_ns::the_materials();
        bool descriptor_is_building
          =  l_materials.m_buffer; //is a descriptor being built?
        if(descriptor_is_building)
        {
            typename bldr_ns::maker_iterator_acceptor_abs const* the_field_maker 
              = &(bldr_ns::template maker_iterator_acceptor_typed<SelectedField>
                ::the_singleton());
            bool record_not_field
              = l_materials.m_record_maker != the_field_maker; //is record != field
          #ifdef CONTAINER_SINGLE_OBJ_ID
            mout()
              <<"***field_registrar.descriptor_is_building\n"
              <<": record_not_field="<<record_not_field
              <<": record_maker="<<(void*)l_materials.m_record_maker
              <<": field_maker="<<(void*)the_field_maker
              <<": obj_id="<<a_selected_field->id_get()
              <<"\n";
          #endif
            if(record_not_field)
            {
                bool record_contains_field
                  = l_materials.m_buffer
                    ->contains(a_selected_field); //is a_selected_field within the Record?
                if(record_contains_field)
                {
                    detail::offset_type offset_field_record 
                      = detail::offset_from_to
                        ( a_selected_field
                        , l_materials.m_buffer->start_con() //record start
                        );
                    l_materials.m_descriptor->append_maker_at
                      ( the_field_maker 
                      , offset_field_record
                      );
                }//end record_contains_field
            }//end record_not_field
        }//end descriptor_is_building
    }//end field_registrar CTOR
};//end field_registrar struct

  template
  < typename FieldsVisitor
  , typename Record
  , bool IsRecordFieldSelector
  >
struct record_iter_maker_is_selector
{
    typedef names_builder<FieldsVisitor> bldr_ns;
    typedef typename bldr_ns::maker_iterator_acceptor_abs maker_type;
        static
      maker_type const*
    the_maker(void)
    {
        maker_type const*l_maker
          = &(bldr_ns::template maker_iterator_acceptor_typed<Record>
            ::the_singleton());
        return l_maker;
    }
};
  template
  < typename FieldsVisitor
  , typename Record
  >
struct record_iter_maker_is_selector
  < FieldsVisitor
  , Record
  , false
  >
{
    typedef names_builder<FieldsVisitor> bldr_ns;
    typedef typename bldr_ns::maker_iterator_acceptor_abs maker_type;
        static
      maker_type const*
    the_maker(void)
    {
        return 0;
    }
};

  template
  < typename FieldsVisitor
  , typename Record
  >
struct record_iter_maker
  : public record_iter_maker_is_selector
    < FieldsVisitor
    , Record
    , is_base_and_derived<field_registrar<FieldsVisitor>,Record>::value
    >
{
};

template
  < typename Record
  >
  struct
builder_new
 /**@brief
  *  Provide a speclializable class whose only purpose
  *  is to provide a method for building an instance
  *  of Record using placement new for use by the
  *  CTOR of descriptor_builder<FieldsVisitor,Record>
  */
{
        static
      Record*
    make(void* buffer)
    {
        return new(buffer) Record;
    }
};
        
template
  < typename FieldsVisitor
  , typename Record
  >
struct descriptor_builder
 /**@class descriptor_builder
  * @brief
  * "Build" the selected_fields_descriptor for Record class
  *  which MIGHT have fields visited by FieldsVisitor.
  */
  : public names_descriptor<FieldsVisitor>::selected_fields_descriptor
{
 public:
        typedef 
      names_descriptor<FieldsVisitor> 
    desc_ns
    ;
    
    descriptor_builder(void)
    {
        typedef names_builder<FieldsVisitor> bldr_ns;
        detail::build_buffer parent_mem(sizeof(Record));
        typename bldr_ns::maker_iterator_acceptor_abs const* the_maker
          = record_iter_maker<FieldsVisitor,Record>::the_maker();
        bldr_ns::the_materials().set_desc_buf(this,&parent_mem,the_maker);
        Record* a_record = builder_new<Record>::make(parent_mem.start_mut());
        a_record->~Record();
        bldr_ns::the_materials().reset(); 
    }
        
        static
      typename desc_ns::selected_fields_descriptor const*
    ptr(void)
    { 
        static descriptor_builder the_builder;
        return &the_builder;
    } 
};//end descriptor_builder general template
      
/** @brief
 *  Define a "namespace template", i.e. a group of
 *  nested classes with template parameter, FieldsVisitor,
 *  accessible from this "namespace template" class.
 */
template
  < typename FieldsVisitor
  >
  struct
names_iterator
  : public names_descriptor<FieldsVisitor>
{
        typedef
      names_descriptor<FieldsVisitor>
    super_ns
    ;
        typedef
      typename super_ns::selected_fields_descriptor
    selected_fields_descriptor
    ;
        typedef
      typename super_ns::field_iterator_acceptor_abs
    field_iterator_acceptor_abs
    ;
        typedef
      typename super_ns::maker_iterator_acceptor_abs
    maker_iterator_acceptor_abs
    ;
  
    struct iterator_iterator_described
   /**@brief 
    *  An iterator over the field_iterator_acceptor_abs's
    *  for a given record and selected_fields_descriptor for
    *  that record.
    */
    {
     public:
            typedef
          rangelib::crange<selected_fields_descriptor const>
        iter_type
        ;
            typedef
          iterator_iterator_described
        my_type
        ;
        iterator_iterator_described
          ( void
          )
          : m_record(0)
      	  , m_iter(selected_fields_descriptor::the_default())
        {}
        iterator_iterator_described
          ( record_top_type* a_record
          , selected_fields_descriptor const*const a_desc 
            //^the selected_fields_descriptor for a_record
          )
          : m_record(a_record)
          , m_iter
            ( (m_record && a_desc)
              ? iter_type(*a_desc)
              : iter_type(selected_fields_descriptor::the_default()) 
            )
        {}
          my_type const&
        operator=(my_type const& a_iter)
        { 
            m_record = a_iter.m_record;
            m_iter = a_iter.m_iter;
            return *this;
        }  
          field_iterator_acceptor_abs*
        current(detail::mem_buffer& a_buffer)const
         /**
          *@brief create field_iterator_acceptor_abs in a_buffer
          *  from the current value of this->m_iter.current().
          */
        { 
            typename selected_fields_descriptor::mker_offs_type const& l_mker_offs 
              = *m_iter;
            field_iterator_acceptor_abs* l_iter 
              = l_mker_offs.first->make_field_iter_from_record_offsets
                ( m_record
                , l_mker_offs.second
                , a_buffer
                );
            return l_iter;
        } 
          void
        increment(void)
        { 
            ++m_iter;
        } 
          std::size_t
        size(void)const
        { 
            iter_type mt(m_iter);
            std::size_t n=0;
            for(;!mt; ++mt,++n);
            return n;
        } 
          bool
        empty(void)const
        { 
            return !m_iter;
        } 
     private:
          record_top_type*
        m_record
        ;
          iter_type
        m_iter
        ;
    };//end iterator_iterator_described struct
    
    struct field_iterator_described
   /**@class field_iterator_described
    * @brief 
    *  Given a record and a selected_fields_descriptor for given record.  
    *  create a series of field_iterator_acceptor_abs which iterate over
    *  each selected field in the given record and apply the iterator
    *  accept method to each field.
    */
    {
        struct field_iterator_acceptor_fwd
        /**@class field_iterator_acceptor_fwd
         * @brief 
         *  Simple smart pointer for field_iterator_acceptor_abs
         *  which just forwards calls to the pointee.
         */
        {
         public:
                typedef
              field_iterator_acceptor_fwd
            my_type
            ;
            field_iterator_acceptor_fwd
              ( std::size_t a_size
              )
              : m_buffer(a_size)
              , m_piter(0)
            {
            }
              void
            accept(FieldsVisitor& a_visitor)const
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
             /**@class field_iterator_acceptor_fwd
              * @pre-conditions
              *  neither this->increment or some derived class increment
              *  is executing.
              */
            { 
                std::size_t r = 0;
                if(!empty())
                { 
                    r = m_piter->size();
                }
                return r;
            }
              bool
            empty(void)const
            { 
                bool r = (m_piter == 0);
                if( !r )
                { r =  m_piter->empty();
                }
                return r;
            } 
              void
            operator++(void)
            { 
                increment();
            } 
              void
            put_current( iterator_iterator_described& a_maker)
            { 
                m_piter = a_maker.current(m_buffer);
            } 
         private:
              my_type const&
            operator=( my_type const& a_iter)
            //no need to define because not used
            ;
              detail::mem_buffer
            m_buffer
            ;
              field_iterator_acceptor_abs*
            m_piter
            ;
        };//end field_iterator_acceptor_fwd
      
            typedef
          field_iterator_acceptor_fwd
        iter_type
        ;
            typedef
          field_iterator_described
        my_type
        ;
     public:
        field_iterator_described
          ( record_top_type* a_record=0 
            //the "given" record for which the iterator is made.
          , selected_fields_descriptor const*const a_desc=0 
            //the selected_fields_descriptor for a_record
          )
          : m_mker(a_record, a_desc)
          , m_iter((a_record && a_desc)?a_desc->iter_max_size():0)
        {
            first_non_empty();
        } 
          void
        accept(FieldsVisitor& a_visitor)const
        { 
            m_iter.accept(a_visitor);
        } 
          void
        increment(void)
        { 
            m_iter.increment();
            next_non_empty();
        }
          std::size_t
        size(void)const
        {
            return m_iter.size();
        }
          bool
        empty(void)const
        {
            return m_iter.empty();
        }
     private:
          my_type const&
        operator=(my_type const& a_iter)
          //No need to define because not used
          ;
          void
        first_non_empty(void)
         /**Find first non empty iter_type in m_mker
          */
        {
            if (!m_mker.empty())
            { 
                m_iter.put_current(m_mker);
                next_non_empty();
            }
        } 
          void
        next_non_empty(void)
         /**Find next non empty iter_type in m_mker
          */
        {
            while (m_iter.empty())
            { 
                m_mker.increment();
                if(m_mker.empty())
                { 
                    break;
                }
                m_iter.put_current(m_mker);
            }
        } 
          iterator_iterator_described
        m_mker
        ;
          iter_type
        m_iter
        //Contains the current field_iterator_acceptor_abs*
        ;
        
    };//end field_iterator_described
    
};//end names_iterator<FieldsVisitor> template

/**@brief
 * A "general" template implementing a field_iterator_described 
 * for the given Record.
 *
 * Specializations are useful for when Record is something like
 * an stl container and the visitor simply visits each
 * element in the container.
 */
template
  < typename FieldsVisitor
  , typename Record
  >
struct field_iterator_of
: public names_iterator<FieldsVisitor>::field_iterator_described
{
 public:
        typedef
      Record
    record_type
    ;
        typedef
      names_iterator<FieldsVisitor>
    super_ns
    ;
        typedef
      typename super_ns::selected_fields_descriptor
    selected_fields_descriptor
    ;
        typedef
      typename super_ns::field_iterator_described
    super_type
    ;
        typedef
      field_iterator_of<FieldsVisitor,record_type>
    my_type
    ;
    field_iterator_of
      ( void
      )
    {
    }
    field_iterator_of
      ( record_type* a_record
      )
      : super_type
        ( a_record
        , selected_fields_description_of
          < FieldsVisitor
          , record_type
          >::ptr()
        )
    {
    }
      my_type const&
    operator=(my_type const& a_iter)
    { 
        super_type::operator=(a_iter);
        return *this;
    }
      my_type const&
    operator=(record_type& a_record)
    {
        selected_fields_descriptor const* 
          fields_desc
          ( selected_fields_description_of
            < FieldsVisitor
            , record_type
            >::ptr()
          );
        super_type::operator=(super_type(&a_record, fields_desc));
        return *this;
    }
       
};//end field_iterator_of<FieldsVisitor,Record> template

}//exit fields_visitor namespace
}//exit boost namespace

/**@def SELECTED_FIELDS_DESCRIPTION_OF_RECORD_VISITOR(RECORD,VISITOR)
 * @brief
 *  Invoking this macro in global namespace causes calculation of the 
 *  selected_fields_descriptor of RECORD for field visitor, VISITOR, and 
 *  makes its address available as selected_fields_description_of<Record>::ptr().
 *
 *  How it works:
 *
 *    Expansion of this macro produces a definition of a specialization
 *    of the selected_fields_description_of<FieldsVisitor,RECORD> class template 
 *    so that it is derived from descriptor_builder<FieldsVisitor, RECORD>, 
 *    whose static ptr(void) returns the required selected_fields_descriptor* 
 *    of RECORD.
 */
#define SELECTED_FIELDS_DESCRIPTION_OF_RECORD(RECORD) \
namespace boost{namespace fields_visitor{ \
template<typename FieldsVisitor > \
  struct selected_fields_description_of<FieldsVisitor, RECORD > \
  : public descriptor_builder<FieldsVisitor, RECORD > \
  {}; \
}}//exit boost::fields_visitor namespace

//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-10-25: Larry Evans
//     WHAT:
//       Copied and renamed from ../managed_ptr/prox_children.hpp
//     WHY:
//       To make code more general.  IOW, it can now be applied to
//       any kind of member type instead of just proxies (or smart
//       pointers).
//////////////////////////////////////////////////////////////////////////////
#endif
