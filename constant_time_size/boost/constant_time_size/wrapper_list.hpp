#ifndef BOOST_CONSTANT_TIME_SIZE_WRAPPER_LIST_H_
#define BOOST_CONSTANT_TIME_SIZE_WRAPPER_LIST_H_

namespace boost { 
namespace constant_time_size {

template <class List, template <typename> class Policy> 
class wrapper_list: public Policy<typename List::size_type> 
{
    typedef wrapper_list this_type;
    typedef Policy<typename List::size_type> super_type;
public:
    typedef  List               						  container;
    typedef typename  container::value_type               value_type;
    typedef typename  container::pointer                  pointer;
    typedef typename  container::const_pointer            const_pointer;
    typedef typename  container::reference                reference;
    typedef typename  container::const_reference          const_reference;
    typedef typename  container::iterator                 iterator;
    typedef typename  container::const_iterator           const_iterator;
    typedef typename  container::const_reverse_iterator   const_reverse_iterator;
    typedef typename  container::reverse_iterator         reverse_iterator;
    typedef typename  container::size_type                size_type;
    typedef typename  container::difference_type          difference_type;
    typedef typename  container::allocator_type           allocator_type;
    typedef non_const<wrapper_list<container, Policy> >      non_const;
    typedef super_type          counter_type;

//    BOOST_CONSTANT_TIME_SIZE_CONSTRUCTORS(wrapper_list, super_type, container)

    /**
     *  @brief  Assigns a given value to a %list.
     *  @param  n  Number of elements to be assigned.
     *  @param  val  Value to be assigned.
     *
     *  This function fills a %list with @a n copies of the given
     *  value.  Note that the assignment completely changes the %list
     *  and that the resulting %list's size is the same as the number
     *  of elements assigned.  Old data may be lost.
     */
    void
    assign(size_type __n, const value_type& __val)
    { 
        this->cont_.assign(__n, __val);
        this->size_ = __n;
    }

    /**
     *  @brief  Assigns a range to a %list.
     *  @param  first  An input iterator.
     *  @param  last   An input iterator.
     *
     *  This function fills a %list with copies of the elements in the
     *  range [@a first,@a last).
     *
     *  Note that the assignment completely changes the %list and
     *  that the resulting %list's size is the same as the number of
     *  elements assigned.  Old data may be lost.
     */
    template<typename _InputIterator>
      void
      assign(_InputIterator __first, _InputIterator __last)
      { 
          this->cont_.assign(__first, __last);
          this->reinit_size(__first, __last);
          // TODO iterate
      }


      // [23.2.2.4] list operations
      /**
       *  @brief  Insert contents of another %list.
       *  @param  position  Iterator referencing the element to insert before.
       *  @param  x  Source list.
       *
       *  The elements of @a x are inserted in constant time in front of
       *  the element referenced by @a position.  @a x becomes an empty
       *  list.
       */
      void
      splice(iterator __position, this_type& __x)
      { 
          size_type n=__x.size();
          this->cont_.splice(__position, __x);
          this->size_ += n;
          __x.size_ = 0;
      }

      void
      splice(iterator __position, container& __x)
      { 
          this->cont_.splice(__position, __x);
          this->reinit_size(__x, __x.begin(), __x.end());
      }
      void
      splice(iterator __position, container& __x, size_type __d)
      { 
          this->cont_.splice(__position, __x);
          this->size_ += __d;
          __x.size_ = 0;
      }

      /**
       *  @brief  Insert element from another %list.
       *  @param  position  Iterator referencing the element to insert before.
       *  @param  x  Source list.
       *  @param  i  Iterator referencing the element to move.
       *
       *  Removes the element in list @a x referenced by @a i and
       *  inserts it into the current list before @a position.
       */
      void
      splice(iterator __position, this_type& __x, iterator __i)
      { 
          this->cont_.splice(__position, __x->cont_, __i);
          this->size_ += 1;
          __x.size_ -= 1;
      }
      void
      splice(iterator __position, container& __x, iterator __i)
      { 
          this->cont_.splice(__position, __x, __i);
          this->size_ += 1;
      }

      /**
       *  @brief  Insert range from another %list.
       *  @param  position  Iterator referencing the element to insert before.
       *  @param  x  Source list.
       *  @param  first  Iterator referencing the start of range in x.
       *  @param  last  Iterator referencing the end of range in x.
       *
       *  Removes elements in the range [first,last) and inserts them
       *  before @a position in constant time.
       *
       *  Undefined if @a position is in [first,last).
       */
      void
      splice(iterator __position, this_type& __x, iterator __first, iterator __last)
      { 
          this->cont_.splice(__position, __x, __first, __last);
          this->reinit_size(__x, __first, __last);
      }
      /**
       *  @brief  Insert range from another %list.
       *  @param  position  Iterator referencing the element to insert before.
       *  @param  x  Source list.
       *  @param  first  Iterator referencing the start of range in x.
       *  @param  last  Iterator referencing the end of range in x.
       *  @param  d  distance betwee first and last.
       *
       *  Removes elements in the range [first,last) and inserts them
       *  before @a position in constant time.
       *
       *  Undefined if @a position is in [first,last).
       */
      void
      splice(iterator __position, this_type& __x, iterator __first, iterator __last, size_type __d)
      { 
          this->cont_.splice(__position, __x, __first, __last);
          this->size_ += __d;
          __x.size_ -= __d;
      }

      /**
       *  @brief  Remove all elements equal to value.
       *  @param  value  The value to remove.
       *
       *  Removes every element in the list equal to @a value.
       *  Remaining elements stay in list order.  Note that this
       *  function only erases the elements, and that if the elements
       *  themselves are pointers, the pointed-to memory is not
       *  touched in any way.  Managing the pointer is the user's
       *  responsibilty.
       */
      void
      remove(const value_type& __value) 
      {
          this->cont_.remove(__value);
          this->reinit_size(this->cont_);
          // TODO iterate

      }

      /**
       *  @brief  Remove all elements satisfying a predicate.
       *  @param  Predicate  Unary predicate function or object.
       *
       *  Removes every element in the list for which the predicate
       *  returns true.  Remaining elements stay in list order.  Note
       *  that this function only erases the elements, and that if the
       *  elements themselves are pointers, the pointed-to memory is
       *  not touched in any way.  Managing the pointer is the user's
       *  responsibilty.
       */
      template<typename _Predicate>
      void
      remove_if(_Predicate __pred)
      {
          this->cont_.remove_if(__pred);
          this->reinit_size(this->cont_);
          // TODO iterate
      }

      /**
       *  @brief  Remove consecutive duplicate elements.
       *
       *  For each consecutive set of elements with the same value,
       *  remove all but the first one.  Remaining elements stay in
       *  list order.  Note that this function only erases the
       *  elements, and that if the elements themselves are pointers,
       *  the pointed-to memory is not touched in any way.  Managing
       *  the pointer is the user's responsibilty.
       */
      void
      unique()
      {
          this->cont_.unique();
          this->reinit_size(this->cont_);
          // TODO iterate         
      }

      /**
       *  @brief  Remove consecutive elements satisfying a predicate.
       *  @param  BinaryPredicate  Binary predicate function or object.
       *
       *  For each consecutive set of elements [first,last) that
       *  satisfy predicate(first,i) where i is an iterator in
       *  [first,last), remove all but the first one.  Remaining
       *  elements stay in list order.  Note that this function only
       *  erases the elements, and that if the elements themselves are
       *  pointers, the pointed-to memory is not touched in any way.
       *  Managing the pointer is the user's responsibilty.
       */
      template<typename _BinaryPredicate>
        void
        unique(_BinaryPredicate __pred)
        {
          this->cont_.unique(__pred);
          this->reinit_size();
        }

      /**
       *  @brief  Merge sorted lists.
       *  @param  x  Sorted list to merge.
       *
       *  Assumes that both @a x and this list are sorted according to
       *  operator<().  Merges elements of @a x into this list in
       *  sorted order, leaving @a x empty when complete.  Elements in
       *  this list precede elements in @a x that are equal.
       */
        void
        merge(this_type& __x)
        { 
            size_type n = __x.size();
            this->cont_.merge(__x.cont_);
            this->size_ += n;
            __x.size_ = 0;
        }

        void
        merge(container& __x)
        { 
            size_type n = __x.size();
            this->cont_.merge(__x);
            this->size_ += n;
        }
        void
        merge(container& __x, size_type __d)
        { 
            this->cont_.merge(__x);
            this->size_ += __d;
        }

      /**
       *  @brief  Merge sorted lists according to comparison function.
       *  @param  x  Sorted list to merge.
       *  @param StrictWeakOrdering Comparison function definining
       *  sort order.
       *
       *  Assumes that both @a x and this list are sorted according to
       *  StrictWeakOrdering.  Merges elements of @a x into this list
       *  in sorted order, leaving @a x empty when complete.  Elements
       *  in this list precede elements in @a x that are equivalent
       *  according to StrictWeakOrdering().
       */
        template<typename _StrictWeakOrdering>
          void
          merge(this_type& __x, _StrictWeakOrdering __pred)
          { 
          	size_type n = __x.size();
              this->cont_.merge(__x, __pred);
              this->size_ += n;
              __x.size_ = 0;
          }
          template<typename _StrictWeakOrdering>
            void
            merge(container& __x, _StrictWeakOrdering __pred)
            { 
            	size_type n = __x.size();
                this->cont_.merge(__x, __pred);
                this->size_ += n;
            }
            template<typename _StrictWeakOrdering>
              void
              merge(container& __x, _StrictWeakOrdering __pred, size_type __d)
              { 
                  this->cont_.merge(__x, __pred);
                  this->size_ += __d;
              }

        void print(std::ostream& os) {
        for (iterator it=this->cont_.begin(), endIt=this->cont_.end(); it!=endIt; ++it) {
            os << *it << ", " ;  
        }
        os << std::endl;
        }  
 
        /**
         *  @brief  Reverse the elements in list.
         *
         *  Reverse the order of elements in the list in linear time.
         */
        void
        reverse()
        { this->cont_.reverse(); }

        /**
         *  @brief  Sort the elements.
         *
         *  Sorts the elements of this list in NlogN time.  Equivalent
         *  elements remain in list order.
         */
        void
        sort() { this->cont_.sort(); }

        /**
         *  @brief  Sort the elements according to comparison function.
         *
         *  Sorts the elements of this list in NlogN time.  Equivalent
         *  elements remain in list order.
         */
        template<typename _StrictWeakOrdering>
          void
          sort(_StrictWeakOrdering&  __pred) {
        	  this->cont_.sort(__pred); 
          }

      /**
       *  @brief  Add data to the end of the %list.
       *  @param  x  Data to be added.
       *
       *  This is a typical stack operation.  The function creates an
       *  element at the end of the %list and assigns the given data to
       *  it.  Due to the nature of a %list this operation can be done
       *  in constant time, and does not invalidate iterators and
       *  references.
       */
      void
      push_back(const value_type& __x)
      { 
  //        std::cout << __x <<" " << this->size_ <<" " << super_type::linear_size() << std::endl;
          this->cont_.push_back(__x);
          this->size_+=1;
      }

      /**
       *  @brief  Removes last element.
       *
       *  This is a typical stack operation.  It shrinks the %list by
       *  one.  Due to the nature of a %list this operation can be done
       *  in constant time, and only invalidates iterators/references to
       *  the element being removed.
       *
       *  Note that no data is returned, and if the last element's data
       *  is needed, it should be retrieved before pop_back() is called.
       */
      void
      pop_back()
      { 
          this->cont_.pop_back();
          this->size_-=1;
      }

      /**
       *  Returns a read/write reference to the data at the last element
       *  of the %list.
       */
      reference
      back()
      { return this->cont_.back(); }

      /**
       *  Returns a read-only (constant) reference to the data at the last
       *  element of the %list.
       */
      const_reference
      back() const
      { return this->cont_.back(); }

      /**
       *  @brief  Add data to the front of the %list.
       *  @param  x  Data to be added.
       *
       *  This is a typical stack operation.  The function creates an
       *  element at the front of the %list and assigns the given data
       *  to it.  Due to the nature of a %list this operation can be
       *  done in constant time, and does not invalidate iterators and
       *  references.
       */
      void
      push_front(const value_type& __x)
      { 
          this->cont_.push_front(__x);
          this->size_+=1;
      }

      /**
       *  @brief  Removes first element.
       *
       *  This is a typical stack operation.  It shrinks the %list by
       *  one.  Due to the nature of a %list this operation can be done
       *  in constant time, and only invalidates iterators/references to
       *  the element being removed.
       *
       *  Note that no data is returned, and if the first element's data
       *  is needed, it should be retrieved before pop_front() is
       *  called.
       */
      void
      pop_front()
      { 
          this->cont_.pop_front();
          this->size_-=1;
      }

      /**
       *  Returns a read/write reference to the data at the first
       *  element of the %list.
       */
      reference
      front()
      { return this->cont_.front(); }

      /**
       *  Returns a read-only (constant) reference to the data at the first
       *  element of the %list.
       */
      const_reference
      front() const
      { return this->cont_.front(); }
    
      /**
       *  @brief Resizes the %list to the specified number of elements.
       *  @param new_size Number of elements the %list should contain.
       *  @param x Data with which new elements should be populated.
       *
       *  This function will %resize the %list to the specified number
       *  of elements.  If the number is smaller than the %list's
       *  current size the %list is truncated, otherwise the %list is
       *  extended and new elements are populated with given data.
       */
      void
      resize(size_type __new_size, const value_type& __x)
      { 
          this->cont_.resize(__new_size, __x);
          this->size_ = __new_size;
      }

      /**
       *  @brief  Resizes the %list to the specified number of elements.
       *  @param  new_size  Number of elements the %list should contain.
       *
       *  This function will resize the %list to the specified number of
       *  elements.  If the number is smaller than the %list's current
       *  size the %list is truncated, otherwise the %list is extended
       *  and new elements are default-constructed.
       */
      void
      resize(size_type __new_size)
      {     resize(__new_size, value_type()); }

      /**
       *  @brief  Inserts given value into %list before specified iterator.
       *  @param  position  An iterator into the %list.
       *  @param  x  Data to be inserted.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert a copy of the given value before
       *  the specified location.  Due to the nature of a %list this
       *  operation can be done in constant time, and does not
       *  invalidate iterators and references.
       */
      iterator
      insert(iterator __position, const value_type& __x)
      { 
          this->cont_.insert(__position, __x);
          this->size_+=1;
      }

      /**
       *  @brief  Inserts a number of copies of given data into the %list.
       *  @param  position  An iterator into the %list.
       *  @param  n  Number of elements to be inserted.
       *  @param  x  Data to be inserted.
       *
       *  This function will insert a specified number of copies of the
       *  given data before the location specified by @a position.
       *
       *  Due to the nature of a %list this operation can be done in
       *  constant time, and does not invalidate iterators and
       *  references.
       */
      void
      insert(iterator __position, size_type __n, const value_type& __x)
      { 
          this->cont_.insert(__position, __n, __x);
          this->size_+=__n;
      }

      /**
       *  @brief  Inserts a range into the %list.
       *  @param  position  An iterator into the %list.
       *  @param  first  An input iterator.
       *  @param  last   An input iterator.
       *
       *  This function will insert copies of the data in the range [@a
       *  first,@a last) into the %list before the location specified by
       *  @a position.
       *
       *  Due to the nature of a %list this operation can be done in
       *  constant time, and does not invalidate iterators and
       *  references.
       */
      template<typename _InputIterator>
        void
        insert(iterator __position, _InputIterator __first,
             _InputIterator __last)
        { 
            this->cont_.insert(__position, __first, __last);
            this->inc_size(__first, __last);
            // TODO do iterating
        }

        template<typename _InputIterator>
          void
          insert(iterator __position, _InputIterator __first,
               _InputIterator __last, size_type __d)
          { 
              this->cont_.insert(__position, __first, __last);
              this->size_+=__d;
          }

      /**
       *  @brief  Remove element at given position.
       *  @param  position  Iterator pointing to element to be erased.
       *  @return  An iterator pointing to the next element (or end()).
       *
       *  This function will erase the element at the given position and thus
       *  shorten the %list by one.
       *
       *  Due to the nature of a %list this operation can be done in
       *  constant time, and only invalidates iterators/references to
       *  the element being removed.  The user is also cautioned that
       *  this function only erases the element, and that if the element
       *  is itself a pointer, the pointed-to memory is not touched in
       *  any way.  Managing the pointer is the user's responsibilty.
       */
      iterator
      erase(iterator __position)
      { 
          std::cout << "=== "<<__LINE__ << std::endl;

          iterator res = this->cont_.erase(__position);
          std::cout << "=== "<<__LINE__ << " " << this->size_ << std::endl;
            this->size_-=1;
          return res;
      }

      /**
       *  @brief  Remove a range of elements.
       *  @param  first  Iterator pointing to the first element to be erased.
       *  @param  last  Iterator pointing to one past the last element to be
       *                erased.
       *  @return  An iterator pointing to the element pointed to by @a last
       *           prior to erasing (or end()).
       *
       *  This function will erase the elements in the range @a
       *  [first,last) and shorten the %list accordingly.
       *
       *  Due to the nature of a %list this operation can be done in
       *  constant time, and only invalidates iterators/references to
       *  the element being removed.  The user is also cautioned that
       *  this function only erases the elements, and that if the
       *  elements themselves are pointers, the pointed-to memory is not
       *  touched in any way.  Managing the pointer is the user's
       *  responsibilty.
       */
      iterator
      erase(iterator __first, iterator __last)
      {
          while (__first != __last) {
              __first = erase(__first);
          }
          return __last;
      }


      /**
       *  Erases all the elements.  Note that this function only erases
       *  the elements, and that if the elements themselves are
       *  pointers, the pointed-to memory is not touched in any way.
       *  Managing the pointer is the user's responsibilty.
       */
      void
      clear()
      { 
          this->cont_.clear();
          this->size_ = 0;
      }

      /**
       *  Returns a read/write reverse iterator that points to the last
       *  element in the %list.  Iteration is done in reverse element
       *  order.
       */
      reverse_iterator
      rbegin()
      { return this->cont_.rbegin(); }

      /**
       *  Returns a read-only (constant) reverse iterator that points to
       *  the last element in the %list.  Iteration is done in reverse
       *  element order.
       */
      const_reverse_iterator
      rbegin() const
      { return this->cont_.rbegin(); }

      /**
       *  Returns a read/write reverse iterator that points to one
       *  before the first element in the %list.  Iteration is done in
       *  reverse element order.
       */
      reverse_iterator
      rend()
      { return this->cont_.rend(); }

      /**
       *  Returns a read-only (constant) reverse iterator that points to one
       *  before the first element in the %list.  Iteration is done in reverse
       *  element order.
       */
      const_reverse_iterator
      rend() const
      { return this->cont_.rend(); }
      
      /**  Returns the number of elements in the %list.  */
      size_type size() const
      { 
  #ifndef NDEBUG
          if (audit) {
            std::cout << this->size_ << std::endl;  
            std::cout << counter_type::size(cont_) << std::endl;  
            std::cout << cont_.size() << std::endl;  
              assert(counter_type::size(cont_)==cont_.size());
          }
  #endif
          return counter_type::size(cont_); 
      }
      
      
      /**
       *  @brief  Default constructor creates no elements.
       */
      explicit
      wrapper_list(const allocator_type& __a = allocator_type())
      : counter_type(0)
      , cont_(__a) 
      { }

      wrapper_list(size_type __n, const value_type& __value,   
         const allocator_type& __a = allocator_type())
      : counter_type(__n)
      , cont_(__n, __value, __a)
      { 
          std::cout << "=== "<<__LINE__ << " " << __n << " " <<  this->size() << std::endl;        
      }

      explicit
      wrapper_list(size_type __n)
      : counter_type(__n)
      , cont_(__n)
      { }


      template<typename _InputIterator>
      wrapper_list(_InputIterator __first, _InputIterator __last,
         const allocator_type& __a = allocator_type())
      : counter_type(0)
      , cont_(__first, __last, __a)
      { 
          
          counter_type::reinit_size(__first, __last);
          // TODO iterate
      }

      template<typename _InputIterator>
      wrapper_list(_InputIterator __first, _InputIterator __last, size_type __d,
         const allocator_type& __a = allocator_type())
      : counter_type(__d) 
      , cont_(__first, __last, __a)
      { }

      /**
       *  @brief  %List copy constructor.
       *  @param  x  A %list of identical element and allocator types.
       *
       *  The newly-created %list uses a copy of the allocation object used
       *  by @a x.
       */
  //    wrapper_list(const wrapper_list& __x)
  //    : counter_type(__x.size_)
  //    , cont_(__x)
  //    { }
      
      template < 
          template <class> class OtherPolicy
      > 
      wrapper_list(const wrapper_list<List, OtherPolicy>& __x)
      : counter_type(__x)
      , cont_(__x)
      { }

      
      explicit
      wrapper_list(const container& __x)
      : counter_type(0)
      , cont_(__x)
      { 
          counter_type::reinit_size(cont_);
          // TODO iterate
      }

      wrapper_list(const container& __x, size_type __d)
      : counter_type(__d)
      , cont_(__x)
      { }
      
      /**
       *  @brief  %List assignment operator.
       *  @param  x  A %list of identical element and allocator types.
       *
       *  All the elements of @a x are copied, but unlike the copy
       *  constructor, the allocator object is not copied.
       */
      wrapper_list&
      operator=(const wrapper_list& __x)
      {
          if (this != &__x)
          {
              cont_ = __x.cont_;
              this->size_ = __x.cont_.size();
          }
          return *this;
      }

      wrapper_list&
      operator=(const container& __x)
      {
             cont_ = __x;
             counter_type::reinit_size(cont_);
          // TODO iterate
          return *this;
      }

      operator const container&() const
      {
          return cont_;
      }
      operator const container*() const
      {
          return &cont_;
      }

      operator non_const()
      {
          return non_const(*this);
      }

      /// Get a copy of the memory allocation object.
      allocator_type
      get_allocator() const
      { return this->cont_->get_allocator(); }

  private:
      container&
      as_container()
      {
          return cont_;
      }

  public:
      const container& as_const_container() const
      {
          return cont_;
      }
      

      /**  Returns the number of elements in the %list.  */
      size_type
      linear_size() const
      { 
          return cont_.size(); 
      }

      /**
       *  @brief  Swaps data with another %list.
       *  @param  x  A %list of the same element and allocator types.
       *
       *  This exchanges the elements between two lists in constant
       *  time.  Note that the global std::swap() function is
       *  specialized such that std::swap(l1,l2) will feed to this
       *  function.
       */
      void
      swap(wrapper_list& __x)
      { 
          std::swap(cont_,__x.cont_);
              std::swap(this->size_,__x.size_);         
      }
      void
      swap(container& __x)
      { 
          std::swap(cont_,__x);
          counter_type::reinit_size(cont_);
          // TODO iterate
      }

      // iterators
      /**
       *  Returns a read/write iterator that points to the first element in the
       *  %list.  Iteration is done in ordinary element order.
       */
      iterator
      begin()
      { return cont_.begin(); }

      /**
       *  Returns a read-only (constant) iterator that points to the
       *  first element in the %list.  Iteration is done in ordinary
       *  element order.
       */
      const_iterator
      begin() const
      { return cont_.begin(); }

      /**
       *  Returns a read/write iterator that points one past the last
       *  element in the %list.  Iteration is done in ordinary element
       *  order.
       */
      iterator
      end() 
      { return cont_.end(); }

      /**
       *  Returns a read-only (constant) iterator that points one past
       *  the last element in the %list.  Iteration is done in ordinary
       *  element order.
       */
      const_iterator
      end() const
      { return cont_.end(); }
      
      // [23.2.2.2] capacity
      /**
       *  Returns true if the %list is empty.  (Thus begin() would equal
       *  end().)
       */
      bool
      empty() const
      { return cont_.empty(); }

      /**  Returns the size() of the largest possible %list.  */
      size_type
      max_size() const
      { return cont_.max_size(); }
      

  protected:
      template<typename> friend struct boost::constant_time_size::non_const;     
      container cont_;
      
      
};


/**
 *  @brief  List equality comparison.
 *  @param  x  A %list.
 *  @param  y  A %list of the same type as @a x.
 *  @return  True iff the size and elements of the lists are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of
 *  the lists.  Lists are considered equivalent if their sizes are
 *  equal, and if corresponding elements compare equal.
*/
template <class Container, template <typename> class Policy> 
  inline bool
  operator==(const wrapper_list<Container, Policy>& __x, 
             const wrapper_list<Container, Policy>& __y)
{    return (__x.size() == __y.size()) && (__x.as_const_container() == __y.as_const_container()); }


/**
 *  @brief  List ordering relation.
 *  @param  x  A %list.
 *  @param  y  A %list of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  lists.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
*/
template <class Container, template <typename> class Policy> 
  inline bool
  operator<(const wrapper_list<Container, Policy>& __x, const wrapper_list<Container, Policy>& __y)
    {    return Container(__x) < Container(__y); }

/// Based on operator==
template <class Container, template <typename> class Policy> 
  inline bool
  operator!=(const wrapper_list<Container, Policy>& __x, const wrapper_list<Container, Policy>& __y)
{    return Container(__x) != Container(__y); }

/// Based on operator<
template <class Container, template <typename> class Policy> 
  inline bool
  operator>(const wrapper_list<Container, Policy>& __x, const wrapper_list<Container, Policy>& __y)
    {    return Container(__x) > Container(__y); }

/// Based on operator<
  template <class Container, template <typename> class Policy> 
  inline bool
  operator<=(const wrapper_list<Container, Policy>& __x, const wrapper_list<Container, Policy>& __y)
{    return Container(__x) <= Container(__y); }

/// Based on operator<
  template <class Container, template <typename> class Policy> 
  inline bool
  operator>=(const wrapper_list<Container, Policy>& __x, const wrapper_list<Container, Policy>& __y)
{    return Container(__x) >= Container(__y); }


}
}
  
#endif // BOOST_CONSTANT_TIME_SIZE_WRAPPER_LIST_H_
