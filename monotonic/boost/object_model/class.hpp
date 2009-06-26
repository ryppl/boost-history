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
//#include <boost/object_model/system_traits.hpp>

BOOST_OM_BEGIN

template <class Traits>
struct klass_base : generic::klass
{
	typedef typename Traits::label_type Label;

	typedef std::map<Label, generic::property const *> properties_type;
	typedef std::map<Label, generic::method const *> methods_type;

private:
	const char *name;
	type::number type_number;

	properties_type properties;
	methods_type methods;

public:
	klass_base(const char *ident, type::number num)
		: generic::klass(num), name(ident)
	{
	}

	bool has_method(Label const &name) const
	{
		return methods.find(name) != methods.end();
	}

	bool has_field(Label const &name) const
	{
		return properties.find(name) != properties.end();
	}
};

	
template <class T, class Registry>
struct klass : klass_base<typename Registry::traits_type>
{
	typedef typename Registry::traits_type system_traits;
	typedef klass_base<system_traits> klass_base_type;
	typedef typename system_traits::label_type label_type;
	typedef typename Registry::rebind_storage<T>::type storage_type;
	typedef typename system_traits::rebind_allocator<storage_type>::type allocator_type;
	typedef type::traits<T> traits;

	Registry &reg;
	mutable allocator_type allocator;

	klass(Registry &factory)
		: klass_base_type(traits::name, traits::type_number), reg(factory), allocator(reg.get_allocator()) { }

	generic::object &create(handle h) const
	{
		storage_type *store = allocator.allocate(1);
		//allocator.construct(store);
		new (store) storage_type();
		store->construct(reg, *this, h);
		return *store;
	}

	void destroy(generic::object &obj) const
	{
		storage_type *store = &static_cast<storage_type &>(obj);
		allocator.destroy(store);
		allocator.deallocate(store, 1);
	}

};

BOOST_OM_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_CLASS_HPP

//EOF
