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

BOOST_OM_BEGIN

/// an object type factory, and registery of instances
template <class Allocator>
struct registry : generic::registry
{
	typedef Allocator allocator_type;
	//typedef boost::unordered_map<handle, generic::storage *, hash<handle>, std::less<handle>, Allocator> instances_type;
	//typedef boost::unordered_map<type::number, generic::klass const *, hash<type::number>, std::less<type::number>, Allocator> classes_type;
	typedef std::map<handle, generic::storage *, std::less<handle>, allocator_type> instances_type;
	typedef std::map<type::number, generic::klass const *, std::less<type::number>, allocator_type> classes_type;

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
		clear();
	}
	void clear()
	{
		BOOST_FOREACH(typename instances_type::value_type &val, instances)
		{
			destroy(*val.second);
		}
		instances.clear();
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

	generic::storage &get_storage(handle h) const
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
	klass<T> *register_class()
	{
		BOOST_ASSERT(!has_class<T>());
		klass<T> *new_class = allocator_create<klass<T> >(*this);
		classes[new_class->get_type_number()] = new_class;
		BOOST_ASSERT(classes.find(new_class->get_type_number()) != classes.end());
		return new_class;
	}

	template <class T>
	storage<T> &create()
	{
		klass<T> const *k = get_class<T>();
		if (k == 0)
			throw unknown_type();
		handle h = ++next_handle;
		storage<T> &obj = static_cast<storage<T> &>(k->create(h));
		instances[h] = &obj;
		return obj;
	}

	template <class T>
	bool has_class() const
	{
		return get_class<T>() != 0;
	}
	
	template <class T>
	klass<T> const *get_class() const
	{
		classes_type::const_iterator iter = classes.find(type::traits<T>::type_number);
		if (iter == classes.end())
			return 0;
		return static_cast<klass<T> const *>(iter->second);
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
	template <class T, class U>
	T *allocator_create(U &init)
	{
		typename Allocator::template rebind<T>::other alloc(allocator);
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
