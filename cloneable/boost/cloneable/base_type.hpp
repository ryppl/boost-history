// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_BASE_TYPE_HPP
#define BOOST_CLONEABLE_BASE_TYPE_HPP

#include <boost/aligned_storage.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/mixin.hpp>
#include <boost/cloneable/detail/create_new.hpp>

namespace boost
{
    namespace cloneable
    {
        template <class T>
        struct abstract_object
        {
            typedef abstract_object<T> abstract_object_type;

            virtual std::string to_string() const { return "abstract_object<T>"; }
            virtual size_t hash_value() const { return 0; }
            virtual bool less(const abstract_object_type& other) const { return false; }
            virtual bool equiv(const abstract_object_type& other) const
            {
                return !less(other) && !other.less(static_cast<const T&>(*this));
            }
        };

        template <class T>
        inline bool operator<(const abstract_object<T>& left, const abstract_object<T>& right)
        {
            return left.less(right);
        }

        template <class T>
        inline bool operator==(const abstract_object<T>& left, const abstract_object<T>& right)
        {
            return left.equiv(right);
        }

        /// default base type used for object hierarchies. the user can supply their own when using 
        /// cloneable<Derived, Base>.
        /// this will be used as a base by default.
        struct base_type : abstract_object<base_type>
        {
            virtual ~base_type() { }

            std::string to_string() const { return "base_type"; }
            size_t hash_value() const { return 0; }
            bool less(const base_type &other) const { return false; }    // to_string() < other.to_string()
            bool equiv(const base_type &other) const { return true; }    // to_string() == other.to_string()
        };

    } // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_BASE_TYPE_HPP

//EOF

