// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_CLASS_HPP
#define BOOST_OBJECT_MODEL_CLASS_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/detail/allocator.hpp>
#include <boost/object_model/generic/class.hpp>

BOOST_OM_BEGIN

template <class T, class Alloc>
struct klass : generic::klass
{
	typedef typename detail::rebind<Alloc,object_model::storage<T> >::type allocator_type;
	typedef type::traits<T> traits;

	registry<Alloc> &reg;
	mutable allocator_type allocator;

	klass(registry<Alloc> &factory)
		: generic::klass(traits::name, traits::type_number), reg(factory), allocator(reg.get_allocator()) { }

	generic::storage &create(handle h) const
	{
		storage<T> *store = allocator.allocate(1);
		//allocator.construct(store);
		new (store) storage<T>();
		store->construct(reg, *this, h);
		return *store;
	}

	void destroy(generic::storage &obj) const
	{
		allocator.destroy(&static_cast<storage<T> &>(obj));
	}

};

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_CLASS_HPP

//EOF
