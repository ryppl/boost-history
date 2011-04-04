//ustility for associating a single component with its index or key.
#ifndef BOOST_COMPOSITE_STORAGE_INDEX_COMPONENT_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_INDEX_COMPONENT_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
namespace boost
{
namespace composite_storage
{
      template
      < typename IndexType
      , IndexType IndexValue
      , typename Component
      >
    struct index_component
    /**@brief
     *  Associate IndexValue with a value of type, Component.
     *  Could be used in some sort of dictionary for retrieving
     *  a value of type Component from the key, IndexValue.
     **@MAINTENANCE_NOTE:
     *  Did not use fusion::pair because:
     *  1) fusion::pair has operator<< already defined
     *    which just prints the 2nd component of the pair.
     *    With this class, which is intended for something
     *    like a dictionary, printing the first value in the
     *    pair is also important.
     *  2) fusion::pair had many more operations defined
     *    on it than were needed for the intended application
     *    of this class.
     *  3) fusion::pair has a default CTOR.  Since instances
     *    of this class are intended to created from a Component
     *    by a index<IndexType,IndexValue> instance, using fusion::pair
     *    would provide more ways to create the needed value
     *    than are absolutely necessary.
     */
    {
        Component my_comp;
        
        index_component(index_component const& a_comp)
          : my_comp(a_comp.my_comp)
          {
            //std::cout<<"(index_component const&):a_comp="<<a_comp.my_comp<<"\n";
          }
          
        index_component(Component const& a_comp)
          : my_comp(a_comp)
          {
            //std::cout<<"(Component a_comp):a_comp="<<a_comp<<"\n";
          }
        
     private:
        index_component()
          /**@brief
           *  Force Component value to be provided during construction.
           */
          ;
    };
          
}//exit composite_storage namespace
}//exit boost namespace

#endif
