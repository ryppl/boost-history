// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_LABEL_HPP
#define BOOST_OBJECT_MODEL_LABEL_HPP

#include <string>
#include <boost/object_model/detail/prefix.hpp>

BOOST_OM_BEGIN

struct label
{
	typedef std::string string_type;
	typedef string_type::value_type char_type;

private:
	string_type value;

public:
	label() { }
	label(const char_type *);
	label(const string_type &);

	const string_type &to_string() const { return value; }

	friend bool operator==(label const &a, label const &b) { return a.value == b.value; }
	friend bool operator<(label const &a, label const &b) { return a.value < b.value; }

private:
	void from_string(const string_type &);
	void from_string(const char_type *);
};

BOOST_OM_END

BOOST_BEGIN

template <>
struct hash<BOOST_OBJECT_MODEL_NAMESPACE(label)>
{
	size_t operator()(BOOST_OBJECT_MODEL_NAMESPACE(label) ident) const
	{
		return 42;// TODO: hash on ident.to_string().c_str()
	}
};

BOOST_END

#include <boost/object_model/detail/postfix.hpp>

#endif // BOOST_OBJECT_MODEL_LABEL_HPP

//EOF
