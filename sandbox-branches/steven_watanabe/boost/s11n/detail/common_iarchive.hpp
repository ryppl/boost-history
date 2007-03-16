#ifndef BOOST_S11N_DETAIL_COMMON_IARCHIVE_HPP_INCLUDED
#define BOOST_S11N_DETAIL_COMMON_IARCHIVE_HPP_INCLUDED

#include <boost/s11n/serialize.hpp>
#include <boost/s11n/types.hpp>
#include <boost/s11n/string.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>
#include <map>
#include <typeinfo>
#include <stdexcept>

namespace boost { namespace s11n { namespace detail
{

    template<class Archive> class common_iarchive
    {

    public:
        
        static const bool is_loading = true;
        static const bool is_saving = false;
        
        template<class T> Archive &operator &(const T &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load(const_cast<T &>(t));
            return ar;
        }

        template<class Base>
        struct registry_entry
        {
            Base *(*create_func)();
            void (*load_func)(Archive &, Base *);
        };

        template<class Base>
        static std::map<std::string, registry_entry<Base> > &registry()
        {
            static std::map<std::string, registry_entry<Base> > r;
            return r;
        }

        template<class Base, class Derived>
        static void register_class(const std::string &name)
        {
            BOOST_STATIC_ASSERT(boost::is_polymorphic<Base>::value);
            registry_entry<Base> e;
            e.create_func = &create_derived<Base, Derived>;
            e.load_func = &load_through_base_class<Base, Derived>;
            if (registry<Base>().count(name) != 0)
                throw std::runtime_error("duplicate registration of class '" + name + "'");
            registry<Base>()[name] = e; 
        }

    protected:        
        
        template<class T> void load(T &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            serialize(ar, t);
        }

        template<class T> 
        typename boost::enable_if_c<boost::is_polymorphic<T>::value && 
                                    !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value, 
                                    void>::type
        load(T *&t)
        {
            BOOST_STATIC_ASSERT((boost::is_polymorphic<T>::value && !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value));
            Archive &ar = static_cast<Archive &>(*this);
            typedef std::map<std::string, registry_entry<T> > registry_t;
            class_name_t name;
            ar & name;
            registry_t &r = registry<T>();
            typename registry_t::iterator it = r.find(name);
            if (it != r.end())
            {
                t = it->second.create_func();
                it->second.load_func(ar, t); 
            }
            else
                throw std::runtime_error(std::string("unregistered class \"") + static_cast<std::string &>(name) + "\"");
        }

        template<class T> 
        typename boost::disable_if_c<boost::is_polymorphic<T>::value && 
                                     !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value, 
                                     void>::type 
        load(T *&t, int = 0)
        {
            BOOST_STATIC_ASSERT((!(boost::is_polymorphic<T>::value && !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value)));
            Archive &ar = static_cast<Archive &>(*this);
            t = new T;
            ar & make_nvp(NULL, *t);
        }

        void load(class_name_t &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar & static_cast<std::string &>(t);
        }

        void load(container_size_t &t)
        {
            load(static_cast<std::size_t &>(t));
        }

        void load(raw_bytes &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(char &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(signed char &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(unsigned char &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(short &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(unsigned short &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(int &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(unsigned int &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(long &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(unsigned long &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(boost::int64_t &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(boost::uint64_t &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(float &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(double &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(long double &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

        void load(bool &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.load_primitive(t);
        }

    private:

        template<class Base, class Derived>
        static Base *create_derived()
        {
            return new Derived;
        }

        template<class Base, class Derived>
        static void load_through_base_class(Archive &ar, Base *b)
        {
            Derived &d = dynamic_cast<Derived &>(*b);
            ar & make_nvp(NULL, d);
        }

    };

} } }

#endif
