// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DLL_DLOPEN_HPP_VP_2004_08_24
#define BOOST_DLL_DLOPEN_HPP_VP_2004_08_24

#include <string>
#include <stdexcept>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/plugin/config.hpp>

#include <dlfcn.h> 

#include <iostream>

#if !defined(BOOST_HAS_DLOPEN)
#error "This file shouldn't be included directly, use the file boost/plugin/dll.hpp only."
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace plugin {

    struct free_dll 
    {
        free_dll(void* h) : h(h) {}

        template<class T>
        void operator()(T)
        {
            if (NULL != h)
                dlclose(h);
        }

        void* h;
    };

    class dll 
    {
    public:
        dll() {} // TODO: should remove this or make non-public
        dll(const std::string& name) : m_name(name) {}

        std::string get_name() const { return m_name; }
        
        template<typename SymbolType>
        boost::shared_ptr<typename remove_pointer<SymbolType>::type> 
        get(const std::string& symbol_name) const
        {
            BOOST_STATIC_ASSERT(boost::is_pointer<SymbolType>::value);
            typedef typename remove_pointer<SymbolType>::type PointedType;
            
            // Open the library. Yes, we do it on every access to 
            // a symbol, see the design discussion in the documentation.
            void* handle = dlopen(m_name.c_str(), RTLD_LAZY|RTLD_GLOBAL);
            if (!handle) {
                throw std::logic_error("Could not open DLL");
            }
            // Clear the error state.
            dlerror();
            void* address = dlsym(handle, symbol_name.c_str());
            char* error = dlerror();
            if (error) {
                throw std::logic_error(error);
            }
            // Cast the to right type.
            SymbolType s = (SymbolType)(address);

            boost::shared_ptr<PointedType> result(s, 
                                                  //killer(handle));
                                                  bind(dlclose, handle));
            
            return result;            
        }        

    private:
        std::string m_name;
    };

///////////////////////////////////////////////////////////////////////////////
}}

#endif
