// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef POLYMORPHIC_DWA2004921_HPP
# define POLYMORPHIC_DWA2004921_HPP

# include <boost/langbinding/classes/override.hpp>

namespace boost { namespace langbinding { namespace classes {

// Derive a wrapper class from T and polymorphic<T> in order to
// override T virtual functions in backend subclasses.
template <class T>
struct polymorphic
{
 public:
    override find_override(char const* name)
    {
        backend::plugin const& back_end = this->instance.class_type->back_end;
        
        return classes::override(back_end, back_end.find_override(name, this->instance));
    }
    
 private:
    template<class> friend class instance_holder;
    backend::class_instance instance;
};


}}} // namespace boost::langbinding::classes

#endif // POLYMORPHIC_DWA2004921_HPP
