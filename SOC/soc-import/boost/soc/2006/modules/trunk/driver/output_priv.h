// -*- x-counterpart: output.cpp -*-

#ifndef INCLUDE_OUTPUT_PRIV_H
#define INCLUDE_OUTPUT_PRIV_H
#include <set>
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
// this is really the messy private section of OutputDelegate.  Don't expect
// very clear separations between this and its containing class.
struct OutputDelegate::Context {
	typedef std::pair<context_iter_t, context_iter_t>  range_t;

	/// The output stream
	std::ostream& header;
	std::ostream& source;
	
	/// The depth, in braces
	int header_depth, source_depth;
	
	struct Emitter {
	private:
		Emitter (const Emitter& other); // not defined
	public:
		Emitter () {}
		virtual ~Emitter () {}
		virtual void operator()(std::ostream& o) = 0;
	};
	
	typedef boost::shared_ptr<Emitter>  Emitter_sp;
	
	class RangeEmitter : public Emitter {
		range_t m_range;
	public:
		RangeEmitter (range_t r) : m_range(r) {}
		void operator()(std::ostream& s) {
			for (context_iter_t it = m_range.first; it != m_range.second; ++it) {
				s << it->get_value () << " ";
			}
		}
	};
	
	class StringEmitter : public Emitter {
		std::string m_text;
	public:
		StringEmitter (std::string s) : m_text(s) {}
		void operator()(std::ostream& s) {
			s << m_text << " ";
		}
	};
	
	// we keep a string emitter "}\n" around for common reuse.
	Emitter_sp m_close_bracket;
	
	// we share these between the header and source texts.
	std::set<std::string> m_includes;
	
	// we don't emit the actual text until after we have a final
	// set of #include directives to emit.
	std::vector< Emitter_sp >  m_header_text, 
	                           m_source_text;
	MapManager & m_map;
	bool m_emitted;
	bool m_in_priv_ns;
	
	// Public code goes in the header only.
	// Private code goes in the header and source.  In the header,
	// it's in a private namespace.
	// OutputDelegate manages the namespace decls as needed.
	enum Mode {
		// not in an export namespace decl.
		out_external,
		out_public,
		out_private,
		out_none
	};

	int m_priv_cnt;
	
	std::stack<Mode> m_mode;
	

	Context (std::ostream& h, std::ostream& s, MapManager& m)
	 : header(h), source(s), m_map(m), 
	   m_emitted(false), m_in_priv_ns(false),
	   m_priv_cnt(0) {
	 	m_close_bracket = Emitter_sp(new StringEmitter("\n}\n"));
	}

	void close_private () {
		if (m_in_priv_ns)
			put(m_close_bracket);
		m_in_priv_ns = false;
	}
	
	void put_header (std::string s) {
		put_header (Emitter_sp(new StringEmitter(s)));
	}

	void put_header (Emitter_sp p) {
		m_header_text.push_back (p);
	}
	
	void put_source (std::string s) {
		put_source(Emitter_sp(new StringEmitter(s)));
	}
	
	void put_source (Emitter_sp p) {
		m_source_text.push_back (p);
	}

	void put (std::string s) {
		put (new StringEmitter (s));
	}

	void put (Emitter_sp p) {
		if (m_mode.top () == out_public) {
			m_header_text.push_back (p);
		} else if (m_mode.top () == out_private) {
			m_source_text.push_back (p);
			m_header_text.push_back (p);
		} else if (m_mode.top () == out_external) {
			m_source_text.push_back (p);
		}
	}

	void put (Emitter *e) {
		put (Emitter_sp(e));
	}
};

#endif
