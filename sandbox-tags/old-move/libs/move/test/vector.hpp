
// Copyright 2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// A vector-like class to test moving without the aid of adobe::vector. (Not a
// compliant STL sequence, and not at all fit for purpose).

#if !defined(BOOST_MOVE_TEST_VECTOR_HEADER)
#define BOOST_MOVE_TEST_VECTOR_HEADER

#include <boost/operators.hpp>

template <class T>
class test_vector : public boost::equality_comparable<test_vector<T> >
{
    struct array {
        T* begin_;
        T* end_;
        T* storage_end_;

        explicit array(std::size_t size = 0)
            : begin_(size == 0 ? (T*) 0 : (T*) new char[size * sizeof(T)]),
            end_(begin_), storage_end_(begin_ + size)
        {
        }
        
        ~array()
        {
            if(begin_) {
                for(T* it = begin_; it != end_; ++it) {
                    it->~T();
                }
                delete[]((char*) begin_);
            }
        }
        
        void swap(array& x) {
            std::swap(begin_, x.begin_);
            std::swap(end_, x.end_);
            std::swap(storage_end_, x.storage_end_);
        }
    private:
        array(array const&);
        array& operator=(array const&);
    };
    
    array data_;
public:
    typedef T value_type;
    typedef T* iterator;
    typedef T const* const_iterator;

    test_vector() : data_() {}
    test_vector(std::size_t n, T const& x)
        : data_(n) {
        std::uninitialized_fill_n(data_.begin_, n, x);
        data_.end_ = data_.begin_ + n;
    }
    ~test_vector() {}
    test_vector(test_vector const& x)
        : data_(x.size())
    {
        data_.end_ = std::uninitialized_copy(x.begin(), x.end(), data_.begin_);
    }

    friend bool operator==(test_vector const& x, test_vector const& y) {
        return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
    }

    std::size_t size() const{
        return data_.end_ - data_.begin_;
    }

    friend void swap(test_vector& x, test_vector& y) {
        x.data_.swap(y.data_);
    }

    // model concept Movable

    test_vector(boost::move_from<test_vector> x)
        : data_() {
        swap(*this, x.source);
    }
    
    test_vector& operator=(test_vector x)
    { swap(*this, x); return *this; }

    T* begin() { return data_.begin_; }
    T* end() { return data_.end_; }
    T const* begin() const { return data_.begin_; }
    T const* end() const { return data_.end_; }
    T const* cbegin() const { return data_.begin_; }
    T const* cend() const { return data_.end_; }

    T& operator[](std::ptrdiff_t n) { return *(data_.begin_ + n); }
    T const& operator[](std::ptrdiff_t n) const{ return *(data_.begin_ + n); }
    
    T& back() { return *(data_.end_ - 1); }
    T const& back() const{ return *(data_.end_ - 1); }

    template <class U>
    void push_back(U const& x, typename boost::copy_sink<U, T>::type = 0) {
        if(data_.end_ == data_.storage_end_) resize(data_.begin_ ? size() * 2 : 4);
        std::uninitialized_fill_n(data_.end_, 1, x);
        ++data_.end_;
    }

#if !defined(BOOST_NO_SFINAE)

    template <class U>
    void push_back(U x, typename boost::move_sink<U, T>::type = 0) {
        if(data_.end_ == data_.storage_end_) resize(data_.begin_ ? size() * 2 : 4);
        data_.end_ = boost::uninitialized_move(&x, &x + 1, data_.end_);
    }

#endif

private:
    void resize(std::size_t new_size) {
        array new_data(new_size);
        new_data.end_ = boost::uninitialized_move(
            data_.begin_, data_.end_, new_data.begin_);
        data_.swap(new_data);
    }
};

#endif
