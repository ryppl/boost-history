#ifndef BOX_DOMAIN_HPP_INCLUDED
#define BOX_DOMAIN_HPP_INCLUDED
#include <vector>
#include <numeric>
#include <algorithm>

enum dirs //directions(used to flag order of processing an ordered sequence).
{ dir_fwd //forward direction.
, dir_rev //reverse direction.
};

  template
  < typename Index_Type = unsigned
  >
  struct
box_domain
  /**@brief
   *  Domain for indices contained within an (hyper)box.
   *  and used to index into (or as the domain of) 
   *  a multidimensional array.
   **@Acknowledgements
   *  Basic idea (and the name) from boost/index_list/domain.hpp
   *  at:
   *    http://svn.boost.org/svn/boost/sandbox/index_list/boost/index_list/domain.hpp
   *  on: 2011-04-23.
   *  
   *  I'm *guessing* that what's missing from this box_domain 
   *  but present in the index_list box_domain are(for I in
   *  0..num_dimensions_-1):
   *    ordering_[I] //Not sure what this is.
   *    ascending_[I] //Not sure what this is.
   *    index_bases_[I] //lower bound for index for I-th axis?
   *    index_tops_[I]  //upper bound for index for I-th axis?
   *  I'm also *guessing* that the my_dirs member variable here
   *  is somehow related to one of:
   *    ordering_
   *    ascending_
   */
{
        typedef
      Index_Type
    index_t
    ;
        typedef
      std::vector<index_t>
    strides_t
    ;
      strides_t
    my_strides
    /**@brief
     *  The strides for each axis in a multidimensional array.
     */
    ;
      int const
    my_dir
    /**@brief
     *  Either 0 or 1.
     *    If 1, then my_strides[i]/my_strides[i+1] > 0
     *    Otherwise,  my_strides[i+1]/my_strides[i] > 0
     */
    ;
      template
      < typename InpIter
      , typename OutIter
      >
      OutIter
    init_iter_strides
      ( InpIter sizes_beg
      , InpIter sizes_end
      , OutIter strides
      )
      /**@brief
       *  Helper function for init_strides( Sizes...)
       */
    {
        *strides=1;
        return
        std::partial_sum
        ( sizes_beg
        , sizes_end
        , ++strides
        , std::multiplies<typename InpIter::value_type>()
        );
    }
      template
      < typename... Sizes
      >
      index_t
    init_strides
      ( Sizes... a_size
      )
      /**@brief
       *  Calculates strides of the array with shape, a_size...
       *  If(my_dir==dir_fwd) then strides increase with index;
       *  otherwise, they decrease with index.
       *  Returns the size of the whole array, IOW, the
       *  product of a_size...
       */
    {
        strides_t const sizes({a_size...});
        index_t result;
        if(my_dir==dir_fwd)
        {
              auto
            it_v=init_iter_strides
              ( sizes.begin()
              , sizes.end()
              , my_strides.begin()
              );
            result=*(--it_v);
        }
        else
        {
              auto
            it_v=init_iter_strides
              ( sizes.rbegin()
              , sizes.rend()
              , my_strides.rbegin()
              );
            result=*(--it_v);
        }
        return result;
    }
    
      template
      < typename... Size
      >
    box_domain( dirs a_dir, Size... a_size)
    : my_strides( sizeof...(Size)+1)
    , my_dir(a_dir)
    {
        init_strides( a_size...);
    }
    
      index_t
    rank()const
    {
        return my_strides.size()-1;
    }
    
      index_t
    stride(index_t Index)const
      /**@brief
       *  size of Index-th axis.
       */
    {
        return my_strides[Index+my_dir];
    }
    
      index_t
    size(index_t Index)const
    {
        index_t const greater=(my_dir+1)%2;
        return my_strides[Index+greater]/my_strides[Index+my_dir];
    }
    
      index_t
    size()const
      /**@brief
       *  size of complete array.
       */
    {
        return 
          (my_dir==dir_fwd)
          ? my_strides[my_strides.size()-1]
          : my_strides[0]
          ;
    }
 
      template
      < typename... Index
      >
      index_t
    offset_at_indices
      ( Index... a_index
      )const
      /**@brief
       *  The offset of element in an array
       *  corresponding to indices, a_index...
       */
    {
        strides_t const indices({a_index...});
        index_t const offset
          = std::inner_product
            ( indices.begin()
            , indices.end()
            , my_strides.begin()+my_dir
            , index_t(0)
            );
        return offset;
    }
    
      template
      < typename InpIter
      , typename OutIter
      >
      void
    indices_at_offset
      ( index_t a_offset
      , InpIter strides_beg
      , InpIter strides_end
      , OutIter indices
      )const
      /**@brief
       *  Helper function for unary indices_at_offset(see below).
       */
    {
        std::transform
          ( strides_beg
          , strides_end
          , indices
          , [&a_offset](index_t stride)
            { 
                index_t index=a_offset/stride;
                a_offset-=index*stride;
                return index; 
            }
          );
    }
    
      std::vector<index_t>
    indices_at_offset
      ( index_t offset
      )const
      /**@brief
       *  Inverse of offset_at_indices
       */
    {
        std::vector<index_t> indices(rank());
        if(my_dir==dir_fwd)
        {
            indices_at_offset
            ( offset
            , my_strides.rbegin()+1
            , my_strides.rend()
            , indices.rbegin()
            );
        }
        else
        {
            indices_at_offset
            ( offset
            , my_strides.begin()+1
            , my_strides.end()
            , indices.begin()
            );
        }
        return indices;
    }
    
      template
      < typename... Size
      >
      index_t
    reshape( Size... a_size)
    {
        index_t const rankp1=sizeof...(a_size)+1;
        my_strides.resize(rankp1);
        return init_strides( a_size...);
    }
    
      template
      < typename... Index
      >
      bool
    ok_indices( Index... a_index)
      /**@brief
       *  Is a_index... a valid argument to
       *  offset_at_indices?
       */
    {
        std::vector<index_t> const indices({a_index...});
        unsigned n=indices.size();
        bool result= n<=rank();
        for( unsigned i=0; i<n && result; ++i)
        {
            result = 0<=indices[i] && indices[i]<size(i);
        }
        return result;
    }
};

#endif
