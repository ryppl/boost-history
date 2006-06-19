#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include "lexpolicies.h"
#include <boost/spirit/actor.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

struct print_action {
	// ref is the output stream we're putting things through.
	// ValueT is the type we just saw come in.
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		std::cout << "print_action(" << value << ")" << std::endl;
		ref << value;
	}
	template<typename T, typename IterT>
	void act (T& ref, IterT const& first, IterT const& last) const {
		std::string vt(first,last);
		std::cout << "print_action(" << vt << ")" << std::endl;
		ref << vt;
	}
};

template<typename T>
inline boost::spirit::ref_value_actor<T, print_action>
print_a(T& ref) {
	return boost::spirit::ref_value_actor<T,print_action>(ref);
}

void
print (const context_t::iterator_type  & start, 
       const context_t::iterator_type  & end) {
	context_t::iterator_type s = start;
	std::cout << "(";
	while (s != end) {
		std::cout << *s;
		++s;
	}
	std::cout << ")" << std::endl;
}

#endif
