//defines "flattening" iterator for a heirarchy of containers.
#ifndef BOOST_FIELDS_VISITOR_FLATTEN_ITERATOR_HPP_LJE20050111
#define BOOST_FIELDS_VISITOR_FLATTEN_ITERATOR_HPP_LJE20050111

template 
  < class Parent
  , unsigned Depth
  >
class flatten_iterator
  : public flatten_iterator<Parent::value_type, Depth-1>
{
public:
  typedef typename Parent::iterator parent_iter_type;
  typedef flatten_iterator<Parent::value_type, Depth-1> child_iter_type;
  flatten_iterator(void)
  {
  }
  flatten_iterator
    ( Parent& a_parent
    )
    : my_current(a_parent.begin())
    , my_end(a_parent.end())
  {
      first_non_empty();
  }
    bool
  is_empty(void)const
  {
      return my_current == my_end;
  }
    void
  put_current(Parent& a_parent)
  {
      my_current=a_parent.begin();
      my_end=a_parent.end();
      first_non_empty();
  }      
    void
  increment(void) 
  {
     this->child_iter_type::increment();
     next_non_empty();
  }
private:
      void
    first_non_empty(void)
     /**Find first non empty in child_iter_type
      */
    {
        if (!this->is_empty())
        { 
            this->child_iter_type::put_current(*my_current);
            next_non_empty();
        }
    } 
      void
    next_non_empty(void)
     /**Find next non empty in child_iter_type
      */
    {
        while (this->child_iter_type::is_empty())
        { 
            ++my_current;
            if(this->is_empty())
            { 
                break;
            }
            this->child_iter_type::put_current(*my_current);
        }
    }
         
    parent_iter_type my_current;
    parent_iter_type my_end;
};
template 
  < class Parent
  >
class flatten_iterator
  < Parent
  , 0
  >
{
public:
  static unsigned const Depth=0;
  typedef Parent* parent_iter_type;
  flatten_iterator(void)
    : my_current(0)
    , my_empty(true)
  {
  }
  flatten_iterator
    ( Parent& a_parent
    )
    : my_current(&a_parent)
    , my_empty(false)
  {
  }
    bool
  is_empty(void)const
  {
      return my_empty;
  }
    void
  put_current(Parent& a_parent)
  {
      my_current=&a_parent;
      my_empty=false;
  }      
    void
  increment(void) 
  {
     my_empty=true;
  }
    Parent&
  deref(void)
  {
      return *my_current;
  }
private:
    parent_iter_type my_current;
    bool my_empty;
};
#endif
