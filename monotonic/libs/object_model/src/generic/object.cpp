// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#include <boost/object_model/generic/object.hpp>
#include <boost/object_model/generic/class.hpp>
#include <boost/object_model/generic/storage.hpp>
#include <boost/object_model/registry.hpp>

BOOST_OM_BEGIN

namespace generic
{
	//---------------------------------------------------
	// object_base
	//---------------------------------------------------

	object_base::object_base() 
		: reg(0), type(0), number(0)
	{
	}

	object_base::object_base(const object_base &other)
		: reg(other.reg), type(other.type), number(other.number)
	{
	}

	object_base::object_base(const const_storage& store)
		: reg(store.reg), type(store.type), number(store.number)
	{
	}


	void object_base::construct(registry &r, klass const &k, handle h)
	{
		reg = &r;
		type = &k;
		number = h;
	}

	registry &object_base::get_registry() const
	{
		if (reg == 0)
			throw empty_object();
		return *reg;
	}
	klass const &object_base::get_class() const
	{
		if (type == 0)
			throw empty_object();
		return *type;
	}

	type::number object_base::get_type_number() const
	{
		return get_class().get_type_number();
	}

	handle object_base::get_handle() const
	{
		return number;
	}

	bool object_base::exists() const
	{
		return get_registry().exists(number);
	}

	//---------------------------------------------------
	// const_object
	//---------------------------------------------------

	//const_object::const_object()
	//{
	//}

	//const_object::const_object(const const_storage &store) : object_base(store)
	//{
	//}

	//const_object::const_object(const object &)
	//{
	//}


	//---------------------------------------------------
	// mutable_object
	//---------------------------------------------------

	mutable_object::mutable_object()
	{
	}

	mutable_object::mutable_object(storage &store) : const_object(store)
	{
	}

	//---------------------------------------------------
	// object
	//---------------------------------------------------

	object::object() : konst(false) { }

	object::object(const const_object& obj) : const_object(obj), konst(true) { }
	object::object(const const_storage & obj) : const_object(obj), konst(true) { }

	object::object(const mutable_object& obj) : const_object(obj), konst(false) { }
	object::object(const storage &store) : const_object(store), konst(false) { }

	object &object::operator=(const const_object& obj)
	{
		const_object::operator=(obj);
		konst = true;
		return *this;
	}

	object &object::operator=(const mutable_object& obj)
	{
		const_object::operator=(obj);
		konst = false;
		return *this;
	}
}

const generic::object null_object;

BOOST_OM_END

//EOF
