#ifndef BOOST_CONSTANT_TIME_SIZE_PERFORMANCE_POLICY_H_
#define BOOST_CONSTANT_TIME_SIZE_PERFORMANCE_POLICY_H_

namespace boost { 
namespace constant_time_size {

template <typename size_type>
class coherent;
template <typename size_type>
class lazy;
template <typename size_type>
class if_not_incoherent;

struct size_constant_time {};

struct size_quasy_constant_time {};

struct size_linear_time {};

template <template <typename> class Policy>
struct size_performance_policy {
};

template <typename SizeType>
class coherent {
    typedef SizeType size_type;
    template <typename> friend class lazy; 
    template <typename> friend class if_not_incoherent; 
public:
    coherent() : size_(0) {
            std::cout << "coherent " << std::endl;  
}
    coherent(size_type __s) : size_(__s) {
            std::cout << "coherent " << std::endl;  
}
    
    template <typename other_size_type>
    coherent(lazy<other_size_type> __p) : size_(__p.size_) {}

    template <typename other_size_type>
    coherent(if_not_incoherent<other_size_type> __p) : size_(__p.size_) {}
    
    /**  Returns the number of elements in the %list.  */
    template<typename Container>
    size_type size(Container&) const
    { 
        return size_; 
    }

protected:
    size_type size_;

    template<typename Container>
    void reinit_size(Container& cont) 
    { 
        size_ = cont.size(); 
    }

    template<typename _InputIterator>
    void inc_size(_InputIterator __first, _InputIterator __last) {
              this->size_ += std::distance(__first, __last);
      }

    template<typename _InputIterator>
    void reinit_size(_InputIterator __first, _InputIterator __last) {
              this->size_ = std::distance(__first, __last);
      }

    template<typename WrapperContainer, typename _InputIterator>
    void reinit_size(WrapperContainer& __x, _InputIterator __first, _InputIterator __last) {
      size_type n = std::distance(__first, __last);
      this->size_ += n;
      __x.size_ -= n;
      }
};

template <typename SizeType>
class lazy {
    typedef SizeType size_type;
    template <typename> friend class coherent; 
    template <typename> friend class if_not_incoherent; 
public:
    lazy() 
    : size_(0) 
    , coherent_(true) 
    {
            std::cout << "lazy " << std::endl;  
}

    lazy(size_type __s) 
    : size_(__s) 
    , coherent_(true)
    {
            std::cout << "lazy " << std::endl;  
}
    
    template <typename other_size_type>
    lazy(coherent<other_size_type> __p) 
    : size_(__p.size_) 
    , coherent_(true) 
    {}

    template <typename other_size_type>
    lazy(if_not_incoherent<other_size_type> __p) 
    : size_(__p.size_) 
    , coherent_(__p.coherent_) 
    {}

    /**  Returns the number of elements in the %list.  */
    template<typename Container>
    size_type size(Container& cont) const
    { 
        if (!is_coherent()) {
            size_ = cont.size();
            coherent_=true;
        } 
            return size_; 
    }

protected:
    mutable size_type size_;
    mutable bool coherent_;

    void set_incoherent() {
        coherent_ = false; 
    }
    bool is_coherent() const {
        return coherent_; 
    }
    template<typename Container>
    void reinit_size(Container& ) 
    { 
          set_incoherent();
    }
    template<typename _InputIterator>
    void inc_size(_InputIterator __first, _InputIterator __last) {
          set_incoherent();
      }

    template<typename _InputIterator>
    void reinit_size(_InputIterator __first, _InputIterator __last) {
          set_incoherent();
      }

    template<typename WrapperContainer, typename _InputIterator>
    void reinit_size(WrapperContainer& __x, _InputIterator __first, _InputIterator __last) {
          set_incoherent();
           __x.set_incoherent();
      }
};

template <typename SizeType>
class if_not_incoherent {
    typedef SizeType size_type;
    template <typename> friend class coherent; 
    template <typename> friend class lazy; 
public:
    if_not_incoherent() 
    : size_(0) 
    , coherent_(true) 
    {
            std::cout << "if_not_incoherent " << std::endl;  
}

    if_not_incoherent(size_type __s) 
    : size_(__s) 
    , coherent_(true) 
    {
            std::cout << "if_not_incoherent " << std::endl;  
}

    template <typename other_size_type>
    if_not_incoherent(coherent<other_size_type> __p) 
    : size_(__p.size_) 
    , coherent_(true) 
    {}

    template <typename other_size_type>
    if_not_incoherent(if_not_incoherent<other_size_type> __p) 
    : size_(__p.size_) 
    , coherent_(__p.coherent_) 
    {}

    /**  Returns the number of elements in the %list.  */
    template<typename Container>
    size_type size(Container& cont) const
    { 
        if (!is_coherent()) {
            std::cout << "is_incoherent " << std::endl;  
            std::cout << "is_incoherent " << size_ << std::endl;  
            std::cout << "is_incoherent " << cont.size() << std::endl;  
            size_ = cont.size();
            coherent_=true;
        } 
            return size_; 
    }
protected:
    mutable size_type size_;
    mutable bool coherent_;

    void set_incoherent() {
        coherent_ = false; 
    }
    bool is_coherent() const {
        return coherent_; 
    }

    template<typename Container>
    void reinit_size(Container& ) 
    { 
            std::cout << "reinit_size " << std::endl;  
          set_incoherent();
    }

    template<typename _InputIterator>
    void inc_size(_InputIterator __first, _InputIterator __last) {
          if (this->is_coherent()) {
              this->size_ += std::distance(__first, __last);
          }
      }

    template<typename _InputIterator>
    void reinit_size(_InputIterator __first, _InputIterator __last) {
          if (this->is_coherent()) {
              this->size_ = std::distance(__first, __last);
          }
      }

    template<typename WrapperContainer, typename _InputIterator>
    void reinit_size(WrapperContainer& __x, _InputIterator __first, _InputIterator __last) {
          if (this->is_coherent() || __x->is_coherent()) {
              size_type n = std::distance(__first, __last);
              this->size_ += n;
              __x.size_ -= n;
          }
      }

};


}
}

#endif // BOOST_CONSTANT_TIME_SIZE_PERFORMANCE_POLICY_H_
