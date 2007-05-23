//
// stream_container.hpp - container streaming.
//
// Slightly tested, much to clean up, run on exactly 1 compiler.
//
// Copyright (c) 2007 Jeff Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//

#ifndef print_container_h_
#define print_container_h_

#include "stream_state.hpp"

#include <ostream>
#include <map>    // concern: should we have an explore/map.hpp file?
#include <vector> // ditto ...
#include <set>
#include <list>
#include <boost/array.hpp>

namespace boost
{
	// A simple collection of additional stream state
	template<typename Elem, typename Tr>
	struct container_stream_state
	{
		typedef std::basic_string<Elem, Tr> str_typ;
		container_stream_state()
		{
			init<Elem>();
		}

		// is there an easier way to specialize between char and wchar_t?
		// Concern: this is only specialized for char and wchar_t streams.
		template<typename El>
		void init();

		template<>
		void init<char>()
		{
			separator = ", ";
			start = "[";
			end = "]";
		}

		template<>
		void init<wchar_t>()
		{
			separator = L", ";
			start = L"[";
			end = L"]";
		}

		str_typ separator;
		str_typ start;
		str_typ end;
	};

	// manipulator function wrapper for 1 char/wchar_t argument.  When streamed, will run manipulator
	// function with argument.
	template<typename Elem>
	struct manipfunc
	{
		manipfunc(void (*fun)(std::basic_ostream<Elem, std::char_traits<Elem> >&, const Elem*), const Elem* val)
			: pfun(fun), arg(val)
		{
		}

		void (*pfun)(std::basic_ostream<Elem, std::char_traits<Elem> >&, const Elem*);
		const Elem* arg;
	};

	// stream manipulator
	template<typename Elem, typename Tr, typename Arg>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<Arg>& manip)
	{
		(*manip.pfun)(ostr, manip.arg);
		return ostr;
	}

	// stream 'normal' value
	template<typename Elem, typename Tr, typename T>
	void stream_normal_value(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem, Tr>* state)
	{
		ostr << val;
	}

	// stream value from associative container
	template<typename Elem, typename Tr, typename T>
	void stream_map_value(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem, Tr>* state)
	{
		ostr << val.first << ":" << val.second;
	}

	template<typename Elem, typename Tr, typename C, typename F>
	void stream_container(std::basic_ostream<Elem, Tr>& ostr, const C& c, F f)
	{
		// grab the extra data embedded in the stream object.
		container_stream_state<Elem, Tr>* state = explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr);

		// starting delimiter
		ostr << state->start;

		C::const_iterator first = c.begin();
		C::const_iterator last = c.end();
		while( first != last )
		{
			// value
			f(ostr, *first, state);
			if( ++first != last )
			{
				// separation delimiter
				ostr << state->separator;
			}
		}

		// ending delimiter
		ostr << state->end;
	}

	template<typename Elem, typename Tr, typename C>
	void stream_container(std::basic_ostream<Elem, Tr>& ostr, const C& c)
	{
		// redirect with "normal" streaming.
		stream_container(ostr, c, &stream_normal_value<Elem, Tr, C::value_type>);
	}

	// concern: this will match everything that does not have a streaming operator.  I'm not sure this is
	// good enough.  Is it fair to say that there are no other collection of objects, other than container,
	// that may want add streaming functionality in this way?  I can't think of a case, but I still don't like
	// this approach.  Another option is to explicitly override for each container, leaving it up to the user
	// to define for user-defined containers.  Yet another option is to use define traits, leaving it up to the
	// user to define the traits for the user-defined container.  Any other ideas?
	//template<typename Elem, typename Tr, typename C>
	//std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const C& c)
	//{
	//	stream_container(ostr, c.begin(), c.end());
	//	return ostr;
	//}

	template<typename Elem, typename Tr, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::vector<T>& v)
	{
		stream_container(ostr, v);
		return ostr;
	}

	template<typename Elem, typename Tr, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::set<T>& s)
	{
		stream_container(ostr, s);
		return ostr;
	}

	template<typename Elem, typename Tr, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multiset<T>& s)
	{
		stream_container(ostr, s);
		return ostr;
	}

	template<typename Elem, typename Tr, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::list<T>& l)
	{
		stream_container(ostr, l);
		return ostr;
	}

	template<typename Elem, typename Tr, typename T, std::size_t N>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const boost::array<T, N>& a)
	{
		stream_container(ostr, a);
		return ostr;
	}

	template<typename Elem, typename Tr, typename T1, typename T2>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::pair<T1, T2>& p)
	{
		container_stream_state<Elem, Tr>* state = explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr);
		return ostr << state->start << p.first << state->separator << p.second << state->end;
	}

	// stream map<K, T>
	template<typename Elem, typename Tr, typename K, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::map<K, T>& m)
	{
		stream_container(ostr, m, &stream_map_value<Elem, Tr, std::map<K,T>::value_type>);
		return ostr;
	}

	// stream multimap<K, T>
	template<typename Elem, typename Tr, typename K, typename T>
	std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multimap<K, T>& m)
	{
		stream_container(ostr, m, &stream_map_value<Elem, Tr, std::multimap<K,T>::value_type>);
		return ostr;
	}

	// function ptr for separator manipulator
	template<typename Elem, typename Tr>
	void setSeparatorFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* sep)
	{
		explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->separator = sep;
	}

	// manipulator
	template<typename Elem>
	manipfunc<Elem> setSeparator(const Elem* sep)
	{
		return manipfunc<Elem>(&setSeparatorFn, sep);
	}

	// function ptr for start manipulator
	template<typename Elem, typename Tr>
	void setStartFn(std::basic_ostream<Elem, Tr>& ostr, const char* start)
	{
		explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->start = start;
	}

	// manipulator
	template<typename Elem>
	manipfunc<Elem> setStart(const Elem* Start)
	{
		return manipfunc<Elem>(&setStartFn, Start);
	}

	// function ptr for end manipulator
	template<typename Elem, typename Tr>
	void setEndFn(std::basic_ostream<Elem, Tr>& ostr, const char* end)
	{
		explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->end = end;
	}

	// manipulator
	template<typename Elem>
	manipfunc<Elem> setEnd(const Elem* end)
	{
		return manipfunc<Elem>(&setEndFn, end);
	}

	// manipulator
	template<typename Elem, typename Tr>
	std::basic_ostream<Elem, Tr>& format_normal(std::basic_ostream<Elem, Tr>& ostr)
	{
		return ostr << setStart("[") << setSeparator(", ") << setEnd("]");
	}

	// manipulator
	template<typename Elem, typename Tr>
	std::basic_ostream<Elem, Tr>& format_html_list(std::basic_ostream<Elem, Tr>& ostr)
	{
		return ostr
			<< setStart("<ul>\n   <li>")
			<< setSeparator("\n   <li>")
			<< setEnd ("\n</ul>");
	}
}

#endif
