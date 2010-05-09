//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_OBJECT_ACCESS_HPP
#define BOOST_TRANSACT_OBJECT_ACCESS_HPP

#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/transact/char_archive.hpp>
#include <boost/transact/detail/embedded_vector.hpp>
#include <boost/transact/type_selection.hpp>
#include <cstring>
#include <algorithm>
#include <iterator>


#ifndef NO_BOOST_SERIALIZATION

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>
#include <boost/archive/basic_binary_oarchive.hpp>
#include <string>

#endif

namespace boost{
namespace transact{



struct deep_tag{};

namespace detail{

template<class T>
struct construct_tag;
template<class Archive,class T>
void construct(Archive &,T *&,deep_tag);

}


class object_access{
public:
    template<class Archive,class T>
    static void save(Archive &ar,T const &t){
        object_access::save(ar,t,boost::is_empty<T>(),serialization::is_bitwise_serializable<T>());
    }
    template<class Archive,class T>
    static void construct(Archive &ar,T *&t){
        object_access::construct(ar,t,boost::is_empty<T>(),serialization::is_bitwise_serializable<T>());
    }

    template<class Archive,class T>
    static void load(Archive &ar,T &t){
        object_access::load(ar,t,boost::is_empty<T>(),serialization::is_bitwise_serializable<T>());
    }

    template<class T>
    static T *clone(T const &t){
        return object_access::clone(t,boost::is_empty<T>(),serialization::is_bitwise_serializable<T>());
    }

    template<class T>
    static bool equal(T const &t1,T const &t2){
        return object_access::equal(t1,t2,boost::is_empty<T>(),serialization::is_bitwise_serializable<T>());
    }

    template<class UnaryFunction,class T>
    static void apply(UnaryFunction const &f,T const &t){
        object_access::apply(f,t,mpl::bool_<boost::is_empty<T>::value || serialization::is_bitwise_serializable<T>::value>());
    }
private:
    template<class T>
    friend class detail::construct_tag;
    template<class Archive,class T>
    friend void detail::construct(Archive &,T *&,deep_tag);

    template<class T>
    static void construct(T &t){
        new (&t) T();
    }

    template<class T>
    static void destruct(T &t){
        t.~T();
    }

    template<class UnaryFunction,class T>
    static void apply(UnaryFunction const &f,T const &t,mpl::true_ bitwise_or_empty){
        f(t);
    }
    template<class UnaryFunction,class T>
    static void apply(UnaryFunction const &f,T const &t,mpl::false_ bitwise_or_empty);
    template<class UnaryFunction,class T>
    static void apply(UnaryFunction const &f,T &t,mpl::true_ bitwise_or_empty){
        f(t);
    }
    template<class UnaryFunction,class T>
    static void apply(UnaryFunction const &f,T &t,mpl::false_ bitwise_or_empty);

    template<class Archive,class T,bool Bitwise>
    static void save(Archive &ar,T const &t,mpl::true_ empty,mpl::bool_<Bitwise>){}
    template<class Archive,class T>
    static void save(Archive &ar,T const &t,mpl::false_ empty,mpl::true_ bitwise){
        ar.save_binary(&t,mpl::size_t<sizeof(T)>());
    }
    template<class Archive,class T>
    static void save(Archive &ar,T const &t,mpl::false_ empty,mpl::false_ bitwise);

    template<class Archive,class T,bool Bitwise>
    static void load(Archive &ar,T &t,mpl::true_ empty,mpl::bool_<Bitwise>){}
    template<class Archive,class T>
    static void load(Archive &ar,T &t,mpl::false_ empty,mpl::true_ bitwise){
        ar.load_binary(&t,mpl::size_t<sizeof(T)>());
    }
    template<class Archive,class T>
    static void load(Archive &ar,T &t,mpl::false_ empty,mpl::false_ bitwise);

    template<class Archive,class T,bool Bitwise>
    static void construct(Archive &ar,T *&t,mpl::true_ empty,mpl::bool_<Bitwise>){
        t=static_cast<T *>( ::operator new(sizeof(T)));
    }
    template<class Archive,class T>
    static void construct(Archive &ar,T *&t,mpl::false_ empty,mpl::true_ bitwise){
        t=static_cast<T *>( ::operator new(sizeof(T)));
        try{
            ar.load_binary(t,mpl::size_t<sizeof(T)>());
        }catch(...){
            ::operator delete(t);
            throw;
        }
    }
    template<class Archive,class T>
    static void construct(Archive &ar,T *&t,mpl::false_ empty,mpl::false_ bitwise);

    template<class T,bool Bitwise>
    static T *clone(T const &t,mpl::true_ empty,mpl::bool_<Bitwise>){
        return static_cast<T *>( ::operator new(sizeof(T)));
    }
    template<class T>
    static T *clone(T const &t,mpl::false_ empty,mpl::true_ bitwise){
        T *tmp=static_cast<T *>( ::operator new(sizeof(T)));
        std::memcpy(tmp,&t,sizeof(T));
        return tmp;
    }
    template<class T>
    static T *clone(T const &t,mpl::false_ empty,mpl::false_ bitwise);

    template<class T,bool Bitwise>
    static bool equal(T const &,T const &,mpl::true_ empty,mpl::bool_<Bitwise>){
        return true;
    }
    template<class T>
    static bool equal(T const &t1,T const &t2,mpl::false_ empty,mpl::true_ bitwise){
        return std::memcmp(&t1,&t2,sizeof(T)) == 0;
    }
    template<class T>
    static bool equal(T const &t1,T const &t2,mpl::false_ empty,mpl::false_ bitwise);
};

namespace detail{

#ifndef NO_BOOST_SERIALIZATION

template<class UnaryFunction>
class apply_archive
    : public archive::detail::common_oarchive<apply_archive<UnaryFunction> >{
private:
    typedef archive::detail::common_oarchive<apply_archive> base_type;
public:
    explicit apply_archive(UnaryFunction const &f)
        : base_type(archive::no_header | archive::no_codecvt)
        , f(f){}
    void save_binary(void const *,std::size_t){}
    void save_override(archive::class_id_optional_type const &,int){}
    void save_override(archive::version_type const &,int){}
    void save_override(archive::class_id_type const &,int){}
    void save_override(archive::class_id_reference_type const &,int){}
    void save_override(archive::object_id_type const &,int){}
    void save_override(archive::object_reference_type const &,int){}
    void save_override(archive::tracking_type const &,int){}
    void save_override(archive::class_name_type const &,int){}
    void save_override(serialization::collection_size_type const &,int){}
    template<class Sequence>
    void save_override(type_selection<Sequence> const &,int){}

    template<class T>
    void save_override(T const &t,int i){
        this->f(t);
        this->save_override(t,i,mpl::bool_<serialization::is_bitwise_serializable<T>::value || boost::is_empty<T>::value>());
    }
private:
    template<class T>
    void save_override(T &,int,mpl::true_ bitwise_or_empty){}
    template<class T>
    void save_override(T &t,int i,mpl::false_ bitwise_or_empty){
        base_type::save_override(t,i);
    }

    friend class boost::archive::save_access;
    template<class T>
    void save(T const &){
        BOOST_STATIC_ASSERT(serialization::implementation_level<T>::value == serialization::primitive_type);
    }
    UnaryFunction f;
};

template<class BaseArchive>
class serialization_oarchive : public archive::basic_binary_oarchive<serialization_oarchive<BaseArchive> >{
    typedef archive::basic_binary_oarchive<serialization_oarchive<BaseArchive> > base_type;
public:
    explicit serialization_oarchive(BaseArchive &ar)
        : base_type(archive::no_header | archive::no_codecvt)
        , base(ar){}
    void save_binary(void const *data,std::size_t size){
        this->base.save_binary(data,size);
    }
    template<class T>
    void save_override(T const &t,int i){
        this->save_override(t,i,mpl::bool_<serialization::is_bitwise_serializable<T>::value || is_empty<T>::value>());
    }
    template<class Sequence>
    void save_override(type_selection<Sequence> const &type,int){
        this->base << type;
    }
private:
    template<class T>
    void save_override(T const &t,int,mpl::true_ bitwise){
        this->base << t;
    }
    template<class T>
    void save_override(T const &t,int i,mpl::false_ bitwise){
        base_type::save_override(t,i);
    }
    friend class boost::archive::save_access;
    template<class T>
    void save(T const &){}

    BaseArchive &base;
};

template<class BaseArchive>
class serialization_iarchive : public archive::basic_binary_iarchive<serialization_iarchive<BaseArchive> >{
    typedef archive::basic_binary_iarchive<serialization_iarchive<BaseArchive> > base_type;
public:
    explicit serialization_iarchive(BaseArchive &ar)
        : base_type(archive::no_header | archive::no_codecvt)
        , base(ar){}
    void load_binary(void *data,std::size_t size){
        this->base.load_binary(data,size);
    }
    template<class T>
    void load_override(T &t,int i){
        this->load_override(t,i,mpl::bool_<serialization::is_bitwise_serializable<T>::value || is_empty<T>::value>());
    }
    template<class Sequence>
    void load_override(type_selection<Sequence> &type,int){
        this->base >> type;
    }
private:
    template<class T>
    void load_override(T &t,int,mpl::true_ bitwise){
        this->base >> t;
    }
    template<class T>
    void load_override(T &t,int i,mpl::false_ bitwise){
        base_type::load_override(t,i);
    }
    friend class boost::archive::load_access;
    template<class T>
    void load(T const &){}

    BaseArchive &base;
};

#endif


template<class BaseArchive>
class save_archive{
public:
    typedef mpl::true_ is_saving;
    typedef mpl::false_ is_loading;
    explicit save_archive(BaseArchive &ar) : base(ar){}
    template<class Size>
    void save_binary(void const *data,Size size){
        this->base.save_binary(data,size);
    }
    template<class T>
    save_archive &operator<<(T const &t){
        this->save(t,is_empty<T>(),serialization::is_bitwise_serializable<T>());
        return *this;
    }
    template<class T>
    save_archive &operator&(T const &t){
        return this->operator<<(t);
    }
private:
    template<class T>
    void save(T const &t,mpl::false_,mpl::true_ bitwise){
        this->base << t;
    }
    template<class T>
    void save(T const &t,mpl::false_,mpl::false_ bitwise){
        transact::object_access::save(this->base,t);
    }
    template<class T,bool Bitwise>
    void save(T const &,mpl::true_ empty,mpl::bool_<Bitwise>){}

    template<class Sequence>
    void save(type_selection<Sequence> const &type,mpl::false_,mpl::false_){
        this->base << type;
    }
    void save(std::string const &s,mpl::false_,mpl::false_ bitwise){
        std::size_t const size=s.size();
        *this << size;
        this->save_binary(s.data(),size);
    }
    void save(std::wstring const &s,mpl::false_,mpl::false_ bitwise){
        std::size_t const size=s.size();
        *this << size;
        this->save_binary(s.data(),size * sizeof(wchar_t));
    }
    void save(char const *s,mpl::false_,mpl::false_ bitwise){
        std::size_t const size=std::strlen(s);
        *this << size;
        this->save_binary(s,size);
    }
    void save(wchar_t const *s,mpl::false_,mpl::false_ bitwise){
        std::size_t const size=std::wcslen(s);
        *this << size;
        this->save_binary(s,size*sizeof(wchar_t));
    }

    BaseArchive &base;
};

template<class BaseArchive>
class load_archive{
public:
    typedef mpl::false_ is_saving;
    typedef mpl::true_ is_loading;
    explicit load_archive(BaseArchive &ar) : base(ar){}
    template<class Size>
    void load_binary(void *data,Size size){
        this->base.load_binary(data,size);
    }
    template<class T>
    load_archive &operator>>(T &t){
        this->load(t,is_empty<T>(),serialization::is_bitwise_serializable<T>());
        return *this;
    }
    template<class T>
    load_archive &operator&(T &t){
        return this->operator>>(t);
    }
private:
    template<class T>
    void load(T &t,mpl::false_,mpl::true_ bitwise){
        this->base >> t;
    }
    template<class T>
    void load(T &t,mpl::false_,mpl::false_ bitwise){
        transact::object_access::load(this->base,t);
    }
    template<class T,bool Bitwise>
    void load(T &,mpl::true_ empty,mpl::bool_<Bitwise>){}

    template<class Sequence>
    void load(type_selection<Sequence> &type,mpl::false_,mpl::false_){
        this->base >> type;
    }
    void load(std::string &s,mpl::false_,mpl::false_ bitwise){
        std::size_t size;
        *this >> size;
        s.resize(size);
        this->load_binary(&*s.begin(),size);  //boost.serialization does the same thing
    }
    void load(std::wstring &s,mpl::false_,mpl::false_ bitwise){
        std::size_t size;
        *this >> size;
        s.resize(size);
        this->load_binary(const_cast<wchar_t *>(s.data()),size*sizeof(wchar_t)); //boost.serialization does the same thing
    }
    void load(char *s,mpl::false_,mpl::false_ bitwise){
        std::size_t size;
        *this >> size;
        this->load_binary(s,size);
        s[size]=0;
    }
    void load(wchar_t *s,mpl::false_,mpl::false_ bitwise){
        std::size_t size;
        *this >> size;
        this->load_binary(s,size*sizeof(wchar_t));
        s[size]=0;
    }

    BaseArchive &base;
};

template<class OutputIterator>
class memory_oarchive : public char_oarchive<OutputIterator>{
    typedef char_oarchive<OutputIterator> base_type;
public:
    explicit memory_oarchive(OutputIterator const &out) : base_type(out){}
    template<class Sequence>
    memory_oarchive &operator<<(type_selection<Sequence> const &type){
        unsigned char typenr=type.which();
        return *this << typenr;
    }
    template<class T>
    memory_oarchive &operator<<(T const &t){
        base_type::operator<<(t);
        return *this;
    }
    template<class T>
    memory_oarchive &operator&(T const &t){
        return this->operator<<(t);
    }
};

template<class InputIterator>
class memory_iarchive : public char_iarchive<InputIterator>{
    typedef char_iarchive<InputIterator> base_type;
public:
    explicit memory_iarchive(InputIterator const &begin,InputIterator const &end) : base_type(begin,end){}
    template<class Sequence>
    memory_iarchive &operator>>(type_selection<Sequence> &type){
        unsigned char typenr;
        *this >> typenr;
        type.reset(typenr);
        return *this;
    }
    template<class T>
    memory_iarchive &operator>>(T &t){
        base_type::operator>>(t);
        return *this;
    }
    template<class T>
    memory_iarchive &operator&(T const &t){
        return this->operator>>(t);
    }
};

template<class InputIterator>
class compare_archive{
public:
    typedef mpl::true_ is_saving;
    typedef mpl::false_ is_loading;
    explicit compare_archive(InputIterator const &begin,InputIterator const &end)
        : in(begin),end(end)
        , equal_(true){}
    template<class Size>
    void save_binary(void const *vdata,Size size){
        char const *data=static_cast<char const *>(vdata);
        this->save_binary(
            data,
            size,
            typename std::iterator_traits<InputIterator>::iterator_category(),
            typename continuous_values<InputIterator>::type()
        );
    }
    template<class T>
    compare_archive &operator<<(T const &t){
        BOOST_STATIC_ASSERT(serialization::is_bitwise_serializable<T>::value);
        this->save_binary(&t,mpl::size_t<sizeof(T)>());
        return *this;
    }
    template<class Sequence>
    compare_archive &operator<<(type_selection<Sequence> const &type){
        unsigned char typenr;
        if(this->in == this->end) this->unequal();
        else{
            typenr=*this->in++;
            if(typenr != type.which()) this->unequal();
        }
        return *this;
    }
    template<class T>
    compare_archive &operator&(T const &t){
        return this->operator<<(t);
    }
    bool equal() const{ return this->equal_; }
private:
    BOOST_STATIC_ASSERT((is_same<typename std::iterator_traits<InputIterator>::value_type,char>::value));
    template<class Size>
    void save_binary(char const *data,Size size,std::random_access_iterator_tag,mpl::true_ contvals){
        if((this->in + size > this->end) || (std::memcmp(data,&*this->in,size) != 0)) this->unequal();
        else this->in+=size;
    }
    template<class Size>
    void save_binary(char const *data,Size size,std::random_access_iterator_tag,mpl::false_ contvals){
        if((this->in + size > this->end) || (!std::equal(data,data+size,this->in))) this->unequal();
        else this->in+=size;
    }
    template<class Size,class Category>
    void save_binary(char const *data,Size size,Category,mpl::false_ contvals){
        for(std::size_t c=0;c<size;++c){
            if((this->in == this->end) || (*data++ != *this->in++)){
                this->unequal();
                break;
            }
        }
    }

    void unequal(){
        this->equal_=false;
        this->in=this->end;
    }

    InputIterator in;
    InputIterator end;
    bool equal_;
};




template<class T>
struct serialization_required;

template<class UnaryFunction,class T>
void apply(UnaryFunction const &f,T const &t,deep_tag tag){
#ifdef NO_BOOST_SERIALIZATION
    sizeof(serialization_required<T>);
#else
    detail::apply_archive<UnaryFunction> ar(f);
    serialization::serialize_adl(ar,const_cast<T &>(t),serialization::version<T>::value);
#endif
}

template<class UnaryFunction,class T>
void apply_(UnaryFunction const &f,T const &t,deep_tag tag){
    apply(f,t,tag);
}
template<class UnaryFunction,class T>
void apply_adl(UnaryFunction const &f,T const &t){
    apply_(f,t,deep_tag());
}


struct constructed_tag{
    constructed_tag(deep_tag){}
};

struct unconstructed_tag{
public:
    explicit unconstructed_tag(bool &constr) : constructed_(constr){
        BOOST_ASSERT(!this->constructed());
    }
    bool &constructed() const{ return this->constructed_; }
private:
    bool &constructed_;
};

template<class T>
struct construct_tag{
    construct_tag(T &t,bool &constructed) : t(t),constructed(constructed){}
    operator deep_tag() const{
        //a user-supplied load-function is called. construct:
        transact::object_access::construct(this->t);
        this->constructed=true;
        return deep_tag();
    }
    operator unconstructed_tag() const{ return unconstructed_tag(this->constructed); }
private:
    T &t;
    bool &constructed;
};


template<class Archive,class T>
void serialize(Archive &ar,T &t,constructed_tag){
#ifdef NO_BOOST_SERIALIZATION
    sizeof(serialization_required<T>);
#else
    BOOST_STATIC_ASSERT(Archive::is_saving::value);
    detail::serialization_oarchive<Archive> serar(ar);
    serialization::serialize_adl(serar,t,serialization::version<T>::value);
#endif
}

template<class Archive,class T>
void serialize_(Archive &ar,T &t,constructed_tag){
    serialize(ar,t,deep_tag());
}

template<class Archive,class T>
void serialize(Archive volatile &var,T &t,unconstructed_tag tag){
    Archive &ar=(Archive &)var;
#ifdef NO_BOOST_SERIALIZATION
    sizeof(serialization_required<T>);
#else
    BOOST_STATIC_ASSERT(Archive::is_loading::value);
    detail::serialization_iarchive<Archive> serar(ar);
    serialization::load_construct_data_adl(serar,&t,serialization::version<T>::value);
    tag.constructed()=true;
    serialization::serialize_adl(serar,t,serialization::version<T>::value);
#endif
}

template<class Archive,class T>
void serialize_(Archive volatile &ar,T &t,unconstructed_tag tag){
    serialize((Archive &)ar,t,construct_tag<T>(t,tag.constructed()));
}


template<class Archive,class T>
void serialize_adl(Archive &ar,T &t){
    serialize_(ar,t,deep_tag());
}


template<class Archive,class T>
void load(Archive &ar,T &t,constructed_tag){
    BOOST_STATIC_ASSERT(Archive::is_loading::value);
    serialize_adl(ar,t);
}

template<class Archive,class T>
void load_(Archive &ar,T &t,constructed_tag){
    load(ar,t,deep_tag());
}

template<class Archive,class T>
void load(Archive volatile &ar,T &t,unconstructed_tag tag){
    serialize_((Archive &)ar,t,construct_tag<T>(t,tag.constructed()));
}

template<class Archive,class T>
void load_(Archive volatile &ar,T &t,unconstructed_tag tag){
    load((Archive &)ar,t,construct_tag<T>(t,tag.constructed()));
}

template<class Archive,class T>
void load_adl(Archive &ar,T &t){
    load_(ar,t,deep_tag());
}


template<class Archive,class T>
void construct(Archive &ar,T *&t,deep_tag tag){
    void *data=::operator new(sizeof(T));
    T &tmp=*static_cast<T *>(data);
    bool constructed=false;
    try{
        load_(ar,tmp,construct_tag<T>(tmp,constructed));
    }catch(...){
        if(constructed) transact::object_access::destruct(tmp);
        ::operator delete(data);
        throw;
    }
    t=&tmp;
}

template<class Archive,class T>
void construct_(Archive &ar,T *&t,deep_tag tag){
    construct(ar,t,tag);
}

template<class Archive,class T>
void construct_adl(Archive &ar,T *&t){
    construct_(ar,t,deep_tag());
}


template<class Archive,class T>
void save(Archive &ar,T const &t,deep_tag){
    BOOST_STATIC_ASSERT(Archive::is_saving::value);
    detail::serialize_adl(ar,const_cast<T &>(t));
}

template<class Archive,class T>
void save_(Archive &ar,T const &t,deep_tag){
    save(ar,t,deep_tag());
}

template<class Archive,class T>
void save_adl(Archive &ar,T const &t){
    save_(ar,t,deep_tag());
}

template<class T>
T *clone(T const &t,deep_tag){
    typedef detail::embedded_vector<char,256,true> buffer_type;
    buffer_type buffer;
    {
        typedef vector_back_insert_iterator<buffer_type> iterator;
        iterator it(buffer);
        detail::memory_oarchive<iterator> ar(it);
        object_access::save(ar,t);
    }
    {
        detail::memory_iarchive<buffer_type::iterator> ar(buffer.begin(),buffer.end());
        T *tmp=0;
        object_access::construct(ar,tmp);
        BOOST_ASSERT(tmp);
        return tmp;
    }
}

template<class T>
T *clone_(T const &t,deep_tag){
    return clone(t,deep_tag());
}

template<class T>
T *clone_adl(T const &t){
    return clone_(t,deep_tag());
}

template<class T>
bool equal(T const &t1,T const &t2,deep_tag){
    typedef detail::embedded_vector<char,256,true> buffer_type;
    buffer_type buffer;
    {
        typedef vector_back_insert_iterator<buffer_type> iterator;
        iterator it(buffer);
        detail::memory_oarchive<iterator> ar(it);
        object_access::save(ar,t1);
    }
    {
        detail::compare_archive<buffer_type::iterator> ar(buffer.begin(),buffer.end());
        object_access::save(ar,t2);
        return ar.equal();
    }
}

template<class T>
bool equal_(T const &t1,T const &t2,deep_tag){
    return equal(t1,t2,deep_tag());
}

template<class T>
bool equal_adl(T const &t1,T const &t2){
    return equal_(t1,t2,deep_tag());
}

}

template<class UnaryFunction,class T>
void object_access::apply(UnaryFunction const &f,T const &t,mpl::false_ empty_or_bitwise){
    detail::apply_adl(f,t);
}

template<class Archive,class T>
void object_access::construct(Archive &ar,T *&t,mpl::false_ empty,mpl::false_ bitwise){
    detail::load_archive<Archive> loadar(ar);
    detail::construct_adl(loadar,t);
}


template<class Archive,class T>
void object_access::load(Archive &ar,T &t,mpl::false_ empty,mpl::false_ bitwise){
    detail::load_archive<Archive> loadar(ar);
    detail::load_adl(loadar,t);
}

template<class Archive,class T>
void object_access::save(Archive &ar,T const &t,mpl::false_ empty,mpl::false_ bitwise){
    detail::save_archive<Archive> savear(ar);
    detail::save_adl(savear,t);
}

template<class T>
T *object_access::clone(T const &t,mpl::false_ empty,mpl::false_ bitwise){
    return detail::clone_adl(t);
}

template<class T>
bool object_access::equal(T const &t1,T const &t2,mpl::false_ empty,mpl::false_ bitwise){
    return detail::equal_adl(t1,t2);
}

}
}


#endif
