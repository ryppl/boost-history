#ifndef BOOST_S11N_DETAIL_COMMON_OARCHIVE_HPP_INCLUDED
#define BOOST_S11N_DETAIL_COMMON_OARCHIVE_HPP_INCLUDED

#include <boost/s11n/serialize.hpp>
#include <boost/s11n/types.hpp>
#include <boost/s11n/nvp.hpp>
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

    template<class Archive> class common_oarchive
    {

    public:
        
        static const bool is_loading = false;
        static const bool is_saving = true;

        template<class T> Archive &operator &(const T &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save(t);
            return ar;
        }

        template<class Base>
        struct registry_entry
        {
            class_name_t name;
            void (*save_func)(Archive &, Base *);
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
            std::string native_name = typeid(Derived).name();
            registry_entry<Base> e;
            e.name = name;
            e.save_func = &save_through_base_class<Base, Derived>;
            if (registry<Base>().count(native_name) != 0)
                throw std::runtime_error("duplicate registration of class '" + name + "'");
            registry<Base>()[native_name] = e; 
        }

    protected:

        template<class T> void save(const T &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            serialize(ar, const_cast<T &>(t));
        }

        template<class T> 
        typename boost::enable_if_c<boost::is_polymorphic<T>::value && 
                                    !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value, 
                                    void>::type
        save(T *t)
        {
            BOOST_STATIC_ASSERT((boost::is_polymorphic<T>::value && !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value));
            typedef std::map<std::string, registry_entry<T> > registry_t;
            std::string native_name = typeid(*t).name();
            registry_t &r = registry<T>();
            typename registry_t::iterator it = r.find(native_name);
            if (it != r.end())
            {
                Archive &ar = static_cast<Archive &>(*this);
                ar & it->second.name;
                it->second.save_func(ar, t); 
            }
            else
                throw std::runtime_error(std::string("unregistered class \"") + native_name + "\"");
        }

        template<class T> 
        typename boost::disable_if_c<boost::is_polymorphic<T>::value && 
                                     !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value, 
                                     void>::type 
        save(T *t, int = 0)
        {
            BOOST_STATIC_ASSERT((!(boost::is_polymorphic<T>::value && !boost::is_base_and_derived<force_nonpolymorphic_serialization, T>::value)));
            Archive &ar = static_cast<Archive &>(*this);
            ar & make_nvp(NULL, *t);
        }

        void save(const class_name_t &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar & static_cast<const std::string &>(t);
        }

        void save(container_size_t t)
        {
            save(static_cast<std::size_t>(t));
        }

        void save(const raw_bytes &t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(char t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(signed char t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(unsigned char t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(short t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(unsigned short t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(int t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(unsigned int t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(long t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(unsigned long t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }
        
        void save(boost::int64_t t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(boost::uint64_t t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(float t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(double t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(long double t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

        void save(bool t)
        {
            Archive &ar = static_cast<Archive &>(*this);
            ar.save_primitive(t);
        }

    private:

        template<class Base, class Derived>
        static void save_through_base_class(Archive &ar, Base *b)
        {
            Derived &d = dynamic_cast<Derived &>(*b);
            ar & make_nvp(NULL, d);
        }

    };

} } }

#endif
