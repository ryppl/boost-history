//  
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//  
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.  
//  It is provided "as is" without express or implied warranty.
//  
//  The authors gratefully acknowledge the support of 
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_VECTOR_SP_H
#define NUMERICS_VECTOR_SP_H

#include "config.h"
#include "storage_sp.h"
#include "vector.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based sparse vector class 
    template<class T, class A>
    class sparse_vector: 
        public vector_expression<sparse_vector<T, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef A array_type;
        typedef const A const_array_type;
        typedef const sparse_vector<T, A> const_self_type;
        typedef sparse_vector<T, A> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
        typedef vector_range<self_type> vector_range_type;
#endif
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef sparse_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        sparse_vector (): 
            size_ (0), non_zeros_ (0), data_ () {}
        NUMERICS_INLINE
        sparse_vector (size_type size, size_type non_zeros = 0): 
            size_ (size), non_zeros_ (non_zeros), data_ () {}
        NUMERICS_INLINE
        sparse_vector (const sparse_vector &v): 
            size_ (v.size_), non_zeros_ (v.non_zeros_), data_ (v.data_) {}
        template<class AE>
        NUMERICS_INLINE
        sparse_vector (const vector_expression<AE> &ae, size_type non_zeros = 0): 
            size_ (ae ().size ()), non_zeros_ (non_zeros), data_ () { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }
        NUMERICS_INLINE
        size_type non_zeros () const { 
            return data_.size (); 
        }
        NUMERICS_INLINE
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size, size_type non_zeros = 0) {
            size_ = size;
            non_zeros_ = non_zeros;
            data ().clear ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            const_iterator_type it (data ().find (i));
            if (it == data ().end () || (*it).first != i)
                return value_type ();
            return (*it).second;
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            return data () [i]; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        sparse_vector &operator = (const sparse_vector &v) { 
            check (size_ == v.size_, bad_size ());
            check (non_zeros_ == v.non_zeros_, bad_size ());
            size_ = v.size_;
            non_zeros_ = v.non_zeros_;
            data () = v.data ();
            return *this;
        }
        NUMERICS_INLINE
        sparse_vector &assign_temporary (sparse_vector &v) { 
            swap (v);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator = (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (ae, non_zeros_));
#else
            return assign (self_type (ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &reset (const vector_expression<AE> &ae) {
            resize (ae ().size (), non_zeros_);
#ifndef USE_GCC
            return assign_temporary (self_type (ae, non_zeros_));
#else
            return assign (self_type (ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator += (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae, non_zeros_));
#else
            return assign (self_type (*this + ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator -= (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae, non_zeros_));
#else
            return assign (self_type (*this - ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        sparse_vector &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        sparse_vector &operator /= (const AT &at) {
            vector_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (sparse_vector &v) {
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            check (non_zeros_ == v.non_zeros_, bad_size ());
            std::swap (size_, v.size_);
            std::swap (non_zeros_, v.non_zeros_);
            data ().swap (v.data ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend void swap (sparse_vector &v1, sparse_vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
#ifndef NUMERICS_USE_ET
            if (t == value_type ()) 
                return;
#endif
            check (data ().find (i) == data ().end (), bad_index ());
            data ().insert (data ().end (), std::pair<size_type, value_type> (i, t));
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            iterator_type it = data ().find (i);
            if (it == data ().end ())
                return;
            data ().erase (it);
        }
        NUMERICS_INLINE
        void clear () {
            data ().clear ();
        }

        class const_iterator;
        class iterator;

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, data ().lower_bound (i));
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, data ().lower_bound (i));
        }
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_first (size_type i) {
            return find (i);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_last (size_type i) {
            return find (i);
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<sparse_vector>,
            public bidirectional_iterator_base<const_iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename sparse_vector::difference_type difference_type;
            typedef typename sparse_vector::value_type value_type;
            typedef typename sparse_vector::value_type reference;
            typedef typename sparse_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<sparse_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const sparse_vector &v, const const_iterator_type &it):
                container_const_reference<sparse_vector> (v), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<sparse_vector> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename sparse_vector::iterator &it):
                container_const_reference<sparse_vector> (it ()), it_ (it.it_) {}
#endif
            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<sparse_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

        class iterator:
            public container_reference<sparse_vector>,
            public bidirectional_iterator_base<iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename sparse_vector::difference_type difference_type;
            typedef typename sparse_vector::value_type value_type;
            typedef typename sparse_vector::reference reference;
            typedef typename sparse_vector::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<sparse_vector> (), it_ () {}
            NUMERICS_INLINE
            iterator (sparse_vector &v, const iterator_type &it):
                container_reference<sparse_vector> (v), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
            }

            // Assignment
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<sparse_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find_first (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        size_type size_;
        size_type non_zeros_;
        array_type data_;
    };

    // Array based sparse vector class 
    template<class T, class IA, class TA>
    class compressed_vector: 
        public vector_expression<compressed_vector<T, IA, TA> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef IA index_array_type;
        typedef TA value_array_type;
        typedef const compressed_vector<T, IA, TA> const_self_type;
        typedef compressed_vector<T, IA, TA> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
        typedef vector_range<self_type> vector_range_type;
#endif
        typedef typename IA::const_iterator const_iterator_type;
        typedef typename IA::iterator iterator_type;
        typedef sparse_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        compressed_vector (): 
            size_ (0), non_zeros_ (0), filled_ (0),
            index_data_ (), value_data_ () {}
        NUMERICS_INLINE
        compressed_vector (size_type size, size_type non_zeros = 0): 
            size_ (size), non_zeros_ (non_zeros), filled_ (0),
            index_data_ (non_zeros), value_data_ (non_zeros) {}
        NUMERICS_INLINE
        compressed_vector (const compressed_vector &v): 
            size_ (v.size_), non_zeros_ (v.non_zeros_), filled_ (v.filled_),
            index_data_ (v.index_data_), value_data_ (v.value_data_) {}
        template<class AE>
        NUMERICS_INLINE
        compressed_vector (const vector_expression<AE> &ae, size_type non_zeros = 0): 
            size_ (ae ().size ()), non_zeros_ (non_zeros), filled_ (0),
            index_data_ (non_zeros), value_data_ (non_zeros) { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }
        NUMERICS_INLINE
        size_type non_zeros () const { 
            return non_zeros; 
        }
        NUMERICS_INLINE
        const index_array_type &index_data () const {
            return index_data_;
        }
        NUMERICS_INLINE
        index_array_type &index_data () {
            return index_data_;
        }
        NUMERICS_INLINE
        const value_array_type &value_data () const {
            return value_data_;
        }
        NUMERICS_INLINE
        value_array_type &value_data () {
            return value_data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size, size_type non_zeros = 0) {
            size_ = size;
            non_zeros_ = non_zeros;
            filled_ = 0;
            index_data ().resize (non_zeros);
            value_data ().resize (non_zeros);
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            const_iterator_type it (std::lower_bound (index_data ().begin (), index_data ().begin () + filled_, i + 1, std::less<size_type> ()));
            if (it == index_data ().begin () + filled_ || *it != i + 1)
                return value_type ();
            return value_data () [it - index_data ().begin ()];
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            iterator_type it (std::lower_bound (index_data ().begin (), index_data ().begin () + filled_, i + 1, std::less<size_type> ()));
            if (it == index_data ().begin () + filled_ || *it != i + 1) 
                insert (i, value_type ());
            return value_data () [it - index_data ().begin ()];
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        compressed_vector &operator = (const compressed_vector &v) { 
            check (size_ == v.size_, bad_size ());
            check (non_zeros_ == v.non_zeros_, bad_size ());
            size_ = v.size_;
            non_zeros_ = v.non_zeros_;
            filled_ = v.filled_;
            index_data () = v.index_data ();
            value_data () = v.value_data ();
            return *this;
        }
        NUMERICS_INLINE
        compressed_vector &assign_temporary (compressed_vector &v) { 
            swap (v);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &operator = (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (ae, non_zeros_));
#else
            return assign (self_type (ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &reset (const vector_expression<AE> &ae) {
            resize (ae ().size (), non_zeros_);
#ifndef USE_GCC
            return assign_temporary (self_type (ae, non_zeros_));
#else
            return assign (self_type (ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &operator += (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae, non_zeros_));
#else
            return assign (self_type (*this + ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &operator -= (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae, non_zeros_));
#else
            return assign (self_type (*this - ae, non_zeros_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        compressed_vector &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        compressed_vector &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        compressed_vector &operator /= (const AT &at) {
            vector_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (compressed_vector &v) {
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            check (non_zeros_ == v.non_zeros_, bad_size ());
            std::swap (size_, v.size_);
            std::swap (non_zeros_, v.non_zeros_);
            std::swap (filled_, v.filled_);
            index_data ().swap (v.index_data ());
            value_data ().swap (v.value_data ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend void swap (compressed_vector &v1, compressed_vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
#ifndef NUMERICS_USE_ET
            if (t == value_type ()) 
                return;
#endif
            check (filled_ == 0 || index_data () [filled_ - 1] < i + 1, external_logic ()); 
            index_data () [filled_] = i + 1;
            value_data () [filled_] = t;
            ++ filled_;
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            check (filled_ > 0 && index_data () [filled_ - 1] == i + 1, external_logic ());
            -- filled_;
        }
        NUMERICS_INLINE
        void clear () {
            filled_ = 0;
        }

        class const_iterator;
        class iterator;

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_iterator find (size_type i) const {            
            return const_iterator (*this, std::lower_bound (index_data ().begin (), index_data ().begin () + filled_, i + 1, std::less<size_type> ()));
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, std::lower_bound (index_data ().begin (), index_data ().begin () + filled_, i + 1, std::less<size_type> ()));
        }
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_first (size_type i) {
            return find (i);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_last (size_type i) {
            return find (i);
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<compressed_vector>,
            public bidirectional_iterator_base<const_iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename compressed_vector::difference_type difference_type;
            typedef typename compressed_vector::value_type value_type;
            typedef typename compressed_vector::value_type reference;
            typedef typename compressed_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<compressed_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const compressed_vector &v, const const_iterator_type &it):
                container_const_reference<compressed_vector> (v), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<compressed_vector> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename compressed_vector::iterator &it):
                container_const_reference<compressed_vector> (it ()), it_ (it.it_) {}
#endif
            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*this) ().value_data () [it_ - (*this) ().index_data ().begin ()];
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_) - 1;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<compressed_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

        class iterator:
            public container_reference<compressed_vector>,
            public bidirectional_iterator_base<iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename compressed_vector::difference_type difference_type;
            typedef typename compressed_vector::value_type value_type;
            typedef typename compressed_vector::reference reference;
            typedef typename compressed_vector::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<compressed_vector> (), it_ () {}
            NUMERICS_INLINE
            iterator (compressed_vector &v, const iterator_type &it):
                container_reference<compressed_vector> (v), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*this) ().value_data () [it_ - (*this) ().index_data ().begin ()];
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_) - 1;
            }

            // Assignment
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<compressed_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find_first (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        size_type size_;
        size_type non_zeros_;
        size_type filled_;
        index_array_type index_data_;
        value_array_type value_data_;
    };

}

#endif 


