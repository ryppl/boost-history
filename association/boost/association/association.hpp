//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/association for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_BIASSOCIATION__HPP
#define BOOST_BIASSOCIATION__HPP

#include <stddef.h>
#include <boost/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
    template <typename Type,typename Tag> 
struct tagged {
    typedef Type type;    
    typedef Tag tag;
};

namespace member_at {
    struct left{};
    struct right{};
}
        
template <typename T> 
struct tagged_traits {
    typedef T type;    
    typedef member_at::left tag;
};

template <typename T, typename Tag> 
struct tagged_traits<tagged<T,Tag> > {
    typedef T type;    
    typedef Tag tag;
};


// specializations of get_field must define a static function apply 
// with the following prototype
// typename TaggedType::type& apply(T&);

template <typename TaggedType, typename T>
struct get_field;
    
// specializations of get_embeding must define a static function apply 
// with the following prototype
// T& apply(FieldType&);

template <typename T, typename Tag, typename FieldType>
struct get_embeding;
    
    

namespace association {
  

template <typename T, typename U>
class end_ptr {
    
public:
    typedef typename tagged_traits<T>::type own_type;
    typedef typename tagged_traits<T>::tag own_tag;
    typedef typename tagged_traits<U>::type other_type;
    typedef typename tagged_traits<U>::tag other_tag;
private:
    other_type* other_ptr_;

public:

    typedef other_type element_type;
    typedef other_type value_type;
    typedef other_type * pointer;

    end_ptr() : other_ptr_(0) {}

    ~end_ptr()
    {
        disconnect();
    }

    element_type* get()
    {
        return other_ptr_;
    }

    const other_type* operator-> () const
    {
        BOOST_ASSERT(get() != 0);
        return get();
    }
    other_type* operator-> ()
    {
        BOOST_ASSERT(get() != 0);
        return get();
    }

    other_type const& operator* () const
    {
        BOOST_ASSERT(get() != 0);
        return *get();
    }

    other_type& operator* ()
    {
        BOOST_ASSERT(get() != 0);
        return *get();
    }

    // C c; c.f=&u;
    end_ptr& operator=(other_type* ptr) {
        connect(ptr);
        return *this;
    }
    end_ptr& operator=(end_ptr& rhs) {
        if (this!= &rhs) {
            disconnect();
            connect(rhs.other_ptr_);
        }
        return *this;
    }
    
    void swap(end_ptr& other)
    {
        U* own_u = other_ptr_;
        U* other_u = other.other_ptr_;
        
        connect(other_u);
        other.connect(own_u);
    }
    
    void connect(other_type* ptr)
    {
        if(ptr == other_ptr_) return;
        disconnect();
        if(ptr == 0) return;
        other_ptr_ = ptr;
        get_field<tagged<end_ptr<U,T>,own_tag>, other_type>::apply(*ptr).connect_one(
            &get_embeding<own_type, own_tag, end_ptr<T,U> >::apply(*this));
    }

    void disconnect()
    {
        if(other_ptr_==0) return;
        other_type* tmp = other_ptr_;
        other_ptr_= 0;
        get_field<tagged<end_ptr<U,T>,own_tag>, other_type>::apply(*tmp).disconnect_one();
    }    

    friend class end_ptr<U, T>;

protected:

    void connect_one(other_type* ptr)
    {
        if(ptr == other_ptr_) return;
        disconnect();
        other_ptr_ = ptr;
    }

    void disconnect_one()
    {
        if(other_ptr_==0) return;
        other_ptr_ = 0;
    }
};

template <typename T, typename U>
class bidir {
public:
    typedef typename tagged_traits<T>::type left_type;
    typedef typename tagged_traits<T>::tag left_tag;
    typedef typename tagged_traits<U>::type right_type;
    typedef typename tagged_traits<U>::tag right_tag;
    typedef end_ptr<T,U> left_end_point;
    typedef end_ptr<U,T> right_end_point;

public:
    template <typename WTag>
    struct end_point {
        typedef typename mpl::if_<is_same<WTag,right_tag>, left_end_point,
                 typename mpl::if_<is_same<WTag,left_tag>, right_end_point, 
                    void>::type
                >::type
            type;
    };

#if 0    
    template <typename WTag>
    static typename enable_if<is_same<WTag,right_tag>, right_type*> ::type
    //~ static void
    get(left_type& ptr)
    {
        return get_field<tagged<left_end_point, right_tag>, left_type>::apply(ptr).get();
    }

    template <typename WTag>
    static typename enable_if<is_same<WTag,left_tag>, left_type*> ::type
    //~ static void
    get(right_type& ptr)
    {
        return get_field<tagged<right_end_point, left_tag>, right_type>::apply(ptr).get();
    }
    
#endif
    static left_end_point& get_left_end_point(left_type& lptr) {
        return get_field<tagged<left_end_point, right_tag>, left_type>::apply(lptr);
    }
    static right_end_point& get_right_end_point(right_type& lptr) {
        return get_field<tagged<right_end_point, left_tag>, right_type>::apply(lptr);
    }

    template <typename WTag>
    static typename enable_if<is_same<WTag,right_tag>, right_type*>::type 
    //~ static void
    get(left_type& ptr)
    {
        return get_field<tagged<left_end_point, right_tag>, left_type>::apply(ptr).get();
    }

    template <typename WTag>
    static typename enable_if<is_same<WTag,left_tag>, left_type*> ::type
    //~ static void
    get(right_type& ptr)
    {
        return get_field<tagged<right_end_point, left_tag>, right_type>::apply(ptr).get();
    }
    
    static void connect(left_type* lptr, right_type* rptr)
    {
        if (lptr!=0) {
            get_left_end_point(*lptr).connect(rptr);
        } else if(rptr!=0) {
            get_right_end_point(*rptr).connect(lptr);
        }
    }

    template <typename WTag>
    static typename enable_if<is_same<WTag,left_tag>, void>::type 
    //~ static void
    disconnect(left_type& ptr)
    {
        get_left_end_point(ptr).disconnect();
    }

    template <typename WTag>
    static typename enable_if<is_same<WTag,right_tag>, void> ::type
    //~ static void
    disconnect(right_type& ptr)
    {
        get_right_end_point(ptr).disconnect();
    }

};
    
} // namespace association
} // namespace boost

#define BOOST_GET_OBJECT(T, FIELD, VAR) \
    *(reinterpret_cast<T*>(reinterpret_cast<char*>(&VAR) - offsetof(T, FIELD)))

#define BOOST_ASSOCIATION_FIELD_DCL(A,ATAG, B, BTAG, FIELD)\
namespace boost {\
template <>\
struct get_field<tagged<association::end_ptr<tagged<A,ATAG>,tagged<B,BTAG> >,BTAG>,A> {\
    static association::end_ptr<tagged<A,ATAG>,tagged<B,BTAG> >& apply(A&v) {\
        return v.FIELD;\
    }\
};\
   \
template <>\
struct get_embeding<A,ATAG,association::end_ptr<tagged<A,ATAG>,tagged<B,BTAG> > > {\
    static A& apply(association::end_ptr<tagged<A,ATAG>,tagged<B,BTAG> >&v) {\
        return BOOST_GET_OBJECT(A, FIELD, v);\
    }\
};\
}

#define BOOST_ASSOCIATION_DCL(ASSOC, LEFT, RIGHT) \
BOOST_ASSOCIATION_FIELD_DCL(ASSOC::left_type, ASSOC::left_tag, ASSOC::right_type, ASSOC::right_tag, RIGHT); \
BOOST_ASSOCIATION_FIELD_DCL(ASSOC::right_type, ASSOC::right_tag, ASSOC::left_type, ASSOC::left_tag, LEFT)


#endif
