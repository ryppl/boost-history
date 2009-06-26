// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_REGISTRY_HPP
#define BOOST_OBJECT_MODEL_REGISTRY_HPP

#include <map>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/generic/registry.hpp>
#include <boost/object_model/type/number.hpp>
#include <boost/object_model/class.hpp>
#include <boost/object_model/exceptions.hpp>
#include <boost/object_model/system_traits.hpp>
#include <boost/object_model/object.hpp>

BOOST_OM_BEGIN

/// an object type factory, and registery of instances
template <class Tr = system_traits<char, default_allocator> >
struct registry;

/// an object type factory, and registery of instances
template <class Tr>
struct registry : generic::registry
{
	typedef Tr traits, traits_type;
	typedef typename traits::allocator_type allocator_type;
	typedef typename traits::char_traits char_traits;
	typedef typename traits::char_type char_type;
	typedef typename traits::string_type string_type;
	typedef typename traits::label_type label_type;

	typedef registry<Tr> This, this_type;

	template <class T>
	struct rebind_klass
	{
		typedef klass<T, this_type> type;
	};
	template <class T>
	struct rebind_storage
	{
		typedef storage<T, traits_type> type;
	};

	typedef std::map<
		handle
		, generic::storage<traits> *
		, std::less<handle>
		, allocator_type
	> instances_type;

	typedef std::map<
		type::number
		, klass_base<traits_type> const *
		, std::less<type::number>
		, allocator_type
	> classes_type;

	template <class T>
	static typename type::traits<T>::reference_type deref(generic::object &object)
	{
		if (!object.is_type<T>())
			throw type_mismatch();
		if (object.is_const())
			throw const_error();
		return static_cast<typename rebind_storage<T>::type &>(object.get_storage()).get_reference();
	}

	template <class T>
	static typename type::traits<T>::const_reference_type const_deref(generic::object const &object)
	{
		if (!object.is_type<T>())
			throw type_mismatch();
		throw;
		//return static_cast<typename rebind_storage<T>::type const &>(object.get_storage()).get_const_reference();
	}

	void set(generic::object &parent, typename traits::label_type const &label, generic::object &child)
	{
		generic::storage<traits> &p = get_storage(parent.get_handle());
		generic::storage<traits> &q = get_storage(child.get_handle());
		p.set(label, q);
	}
	generic::object get(generic::object &parent, typename traits::label_type const &label)
	{
		generic::storage<traits> &p = get_storage(parent.get_handle());
		return p.get(label);
	}
	bool has(generic::object &parent, typename traits::label_type const &label)
	{
		generic::storage<traits> &p = get_storage(parent.get_handle());
		return p.has(label);
	}

protected:
	allocator_type allocator;
	instances_type instances;
	classes_type classes;
	handle::value_type next_handle;

public:
	registry()
	{
	}
	~registry()
	{
		clear_classes();
	}
	void clear()
	{
		BOOST_FOREACH(typename instances_type::value_type &val, instances)
		{
			generic::object &obj = *val.second;
			obj.get_class().destroy(obj);
		}
		instances.clear();
	}
	void clear_classes()
	{
		clear();
		BOOST_FOREACH(typename classes_type::value_type &val, classes)
		{
			allocator_destroy(const_cast<klass_base<traits_type> *>(val.second));
		}
		classes.clear();
	}

	void destroy(generic::mutable_object obj)
	{
		instances_type::iterator val = instances.find(obj.get_handle());
		if (val == instances.end())
		{
			return;
		}
		obj.get_class().destroy(*val->second);
		instances.erase(val);
	}

	generic::storage<traits> &get_storage(handle h) const
	{
		instances_type::const_iterator iter = instances.find(h);
		if (iter == instances.end())
			throw unknown_handle();
		return *iter->second;
	}

	size_t num_classes() const
	{
		return classes.size();
	}
	size_t num_instances() const
	{
		return instances.size();
	}

	bool exists(const generic::const_object &obj) const
	{
		return exists(obj.get_handle());
	}
	bool exists(handle h) const
	{
		return instances.find(h) != instances.end();
	}

	allocator_type &get_allocator()
	{
		return allocator;
	}

	template <class T>
	typename rebind_klass<T>::type *register_class()
	{
		BOOST_ASSERT(!has_class<T>());
		typedef typename rebind_klass<T>::type class_type;
		class_type *new_class = allocator_create<class_type >(*this);
		classes[new_class->get_type_number()] = new_class;
		BOOST_ASSERT(classes.find(new_class->get_type_number()) != classes.end());
		return new_class;
	}

	generic::klass const *get_class(type::number number) const
	{
		classes_type::const_iterator iter = classes.find(number);
		return iter == classes.end() ? 0 : iter->second;
	}

	template <class T>
	object<T> create()
	{
		typename rebind_klass<T>::type const *k = get_class<T>();
		if (k == 0)
			throw unknown_type();
		handle h = ++next_handle;
		typedef typename rebind_storage<T>::type storage_type;
		storage_type &obj = static_cast<storage_type &>(k->create(h));
		instances[h] = &obj;
		return object<T>(obj, &this_type::deref<T>, &this_type::const_deref<T>);
	}

	template <class T>
	bool has_class() const
	{
		return get_class<T>() != 0;
	}
	
	template <class T>
	typename rebind_klass<T>::type const *get_class() const
	{
		classes_type::const_iterator iter = classes.find(type::traits<T>::type_number);
		if (iter == classes.end())
			return 0;
		return static_cast<typename rebind_klass<T>::type const *>(iter->second);
	}

private:

	template <class T>
	T *allocator_create()
	{
		typename Allocator::template rebind<T>::other alloc(allocator);
		T *ptr = alloc.allocate(1);
		//alloc.construct(ptr);
		new (ptr) T();
		return ptr;
	}
	template <class T>
	void allocator_destroy(T *ptr)
	{
		typename allocator_type::template rebind<T>::other alloc(allocator);
		alloc.destroy(ptr);
		alloc.deallocate(ptr,1);
	}
	template <class T, class U>
	T *allocator_create(U &init)
	{
		typename allocator_type::template rebind<T>::other alloc(allocator);
		T *ptr = alloc.allocate(1);
		//alloc.construct(ptr, init);
		new (ptr) T(init);
		return ptr;
	}
};

BOOST_OM_END

#include <boost/object_model/detail/prefix.hpp>

#endif // BOOST_OBJECT_MODEL_REGISTRY_HPP

//EOF
