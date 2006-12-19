#ifndef BOOST_S11N_NVP_HPP_INCLUDED
#define BOOST_S11N_NVP_HPP_INCLUDED

namespace boost { namespace s11n
{

    template<class T>
    class nvp
    {
    
    public:
    
        nvp(const char *name, T &value): name(name), value(value) { }
        const char *name;
        T &value;

    private:

        friend class access;
        template<class Archive> void serialize(Archive &ar) const
        {
            ar & value;
        }

    };

    template<class T>
    inline nvp<T> make_nvp(const char *name, T &value)
    {
        return nvp<T>(name, value);
    }

    template<class T>
    class default_nvp: public nvp<T>
    {
    
    public:
    
        default_nvp(const char *name, T &value): nvp<T>(name, value), default_value(0) { }
        default_nvp(const char *name, T &value, const T &default_value): nvp<T>(name, value), default_value(&default_value) { }

        const T *default_value;

    };

    template<class T>
    inline default_nvp<T> make_default_nvp(const char *name, T &value)
    {
        return default_nvp<T>(name, value);
    }

    template<class T>
    inline default_nvp<T> make_default_nvp(const char *name, T &value, const T &default_value)
    {
        return default_nvp<T>(name, value, default_value);
    }

    template<class T>
    class concise_nvp: public default_nvp<T>
    {
    
    public:
    
        concise_nvp(const char *name, T &value): default_nvp<T>(name, value) { }
        concise_nvp(const char *name, T &value, const T &default_value): default_nvp<T>(name, value, default_value) { }

    };

    template<class T>
    inline concise_nvp<T> make_concise_nvp(const char *name, T &value)
    {
        return concise_nvp<T>(name, value);
    }

    template<class T>
    inline concise_nvp<T> make_concise_nvp(const char *name, T &value, const T &default_value)
    {
        return concise_nvp<T>(name, value, default_value);
    }

} }

#endif
