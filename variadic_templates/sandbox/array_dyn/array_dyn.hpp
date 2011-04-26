#ifndef ARRAY_DYN_HPP_INCLUDED
#define ARRAY_DYN_HPP_INCLUDED
#include <iostream>
#include <iomanip>

#include "box_domain.hpp"

  template
  < typename T
  >
  struct  
array_dyn
/**@brief
 *  Multidimensional array whose
 *  'shape' is defined at runtime.
 *  'Shape' means upper bounds on indices
 *  for each dimension (or axis in apl terminology).
 */
: box_domain<>
{
        typedef
      box_domain<>
    super_t
    ;
        typedef typename
      super_t::index_t
    index_t
    ;
      std::vector<T>
    my_data
    /**@brief
     *  data in the array.
     */
    ;
      template
      < typename... Size
      >
    array_dyn( dirs a_dir, Size... a_size)
    : super_t( a_dir, a_size...)
    , my_data( super_t::size())
    {
    }
    
      template
      < typename... Size
      >
      void
    reshape( Size... a_size)
    {
        my_data.resize(super_t::reshape(a_size...));
    }
    
      template
      < typename... Index
      >
      T&
    operator()( Index... a_index)
    {
        return my_data[offset_at_indices(a_index...)];
    }
    
      template
      < typename... Index
      >
      T const&
    operator()( Index... a_index)const
    {
        return my_data[offset_at_indices(a_index...)];
    }
    
};

  template
  < typename Array
  >
  struct
array_host
{
      Array&
    my_array
    ;
        typedef typename
      Array::index_t
    index_t
    ;
      int const
    my_axis_dir
    /**@brief
     *  +1 or -1: the direction in which axis changes.
     */
    ;
      index_t const
    my_axis_beg
    ;
      index_t const
    my_axis_end
    ;
    array_host
      ( Array& a_array
      , dirs a_dir=dir_fwd
      )
    : my_array(a_array)
    , my_axis_dir
      ( (a_dir==dir_fwd)
      ? +1
      : -1
      )
    , my_axis_beg
      ( (a_dir==dir_fwd)
      ? 0
      : my_array.rank()-1
      )
    , my_axis_end
      ( (a_dir==dir_fwd)
      ? my_array.rank()-1
      : 0
      )
    {
    }
    
      template
      < typename Visitor
      >
      void
    accept
      ( Visitor& a_viz
      )
    {
        accept_off_ax
        ( a_viz
        , 0
        , my_axis_beg
        );
    }
 private:
      template
      < typename Visitor
      >
      void
    accept_off_ax
      ( Visitor& a_viz
      , index_t offset
      , index_t axis_now
      )
    {
        index_t const axis_size=my_array.size(axis_now);
        bool const is_leaf=axis_now==my_axis_end;
        index_t const axis_stride=my_array.stride(axis_now);
        for(index_t i=0; i<axis_size; ++i)
        {
            a_viz.visit_pre(is_leaf, axis_now, i);
            if(is_leaf)
            {
                a_viz.visit_node(my_array.my_data[offset]);
            }
            else
            {
                this->accept_off_ax
                  ( a_viz
                  , offset
                  , axis_now+my_axis_dir
                  );
            }
            offset+=axis_stride;
        }
        a_viz.visit_post(is_leaf, axis_now);
    }
};  

  template
  < typename T
  >
  struct
print_array
{
      std::ostream&
    my_sout
    ;
        typedef typename
      array_dyn<T>::index_t
    index_t
    ;
    print_array(std::ostream& a_sout)
    : my_sout(a_sout)
    {}
    
    void visit_pre( bool is_leaf, index_t axis, index_t index)
    {
        if(index==0)
        {
            my_sout<<"{ ";
        }
        else
        {
            if(!is_leaf)
            {
                my_sout<<std::setw(2*axis)<<"";
            }
            my_sout<<", ";
        }
    }
    
    void visit_node(T const& a_t)
    {
        my_sout<<a_t;
    }
        
    void visit_post( bool is_leaf, index_t axis)
    {
        if(!is_leaf)
        {
            my_sout<<std::setw(2*axis)<<"";
        }
        my_sout<<"}\n";
    }
    
};

  template
  < typename T
  >
  std::ostream&
operator<<
( std::ostream& sout
, array_dyn<T>const& a_arr
)
{
    print_array<T> a_viz(sout);
    array_host<array_dyn<T> const> host_arr(a_arr);
    host_arr.accept(a_viz);
    return sout;
}    

#endif
