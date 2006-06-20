#ifndef INCLUDE_OUTPUT_H
#define INCLUDE_OUTPUT_H

#include "../parser/lexpolicies.h"

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stack>
#include <boost/shared_ptr.hpp>

//
// Two functions:
// 1. Keeps the output streams open for the generated text
// 2. Keeps track of where each source segment goes.
class OutputDelegate {
	enum Context {
		out_header,
		out_source,
		out_none
	};
	
	std::ostream&  m_header;
	std::ostream&  m_source;

	struct Emitter {
		virtual ~Emitter () {}
		virtual void operator()(std::ostream& o) = 0;
	};
	
	struct RangeEmitter;
	struct StringEmitter;
	
	// we share these between the header and source texts.
	std::set<std::string> m_includes;
	
	// we don't emit the actual text until after we have a final
	// set of #include directives to emit.
	std::vector< boost::shared_ptr<Emitter> >  m_header_text, 
	                                           m_source_text;
	
	std::stack<Context> m_dest;
	
	bool m_emitted;
	
public:
	OutputDelegate (std::ostream& h, std::ostream& s)
	 : m_header(h), m_source(s), m_emitted(false) {
	 	m_dest.push (out_none);
	 }

	void push_source () {
		assert (!m_emitted);
		m_dest.push (out_source);
	}
	
	void push_header () {
		assert (!m_emitted);
		m_dest.push (out_header);
	}
	
	void pop () {
		assert (!m_emitted);
		m_dest.pop ();
	}

	void include (std::string include) {
		assert (!m_emitted);
		m_includes.insert(include);
	}

	void out (context_iter_t start, context_iter_t end);
	void out (std::string text);
	
	void emit ();
};


#endif
