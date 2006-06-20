#include "output.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

typedef pair<context_iter_t, context_iter_t>  range_t;

class OutputDelegate::RangeEmitter : public OutputDelegate::Emitter {
	range_t m_range;
public:
	RangeEmitter (range_t r) : m_range(r) {}
	void operator()(ostream& s) {
		for (; m_range.first != m_range.second; ++m_range.first) {
			s << m_range.first->get_value ();
		}
	}
};

class OutputDelegate::StringEmitter : public OutputDelegate::Emitter {
	string m_text;
public:
	StringEmitter (string s) : m_text(s) {}
	void operator()(ostream& s) {
		s << m_text << "\n";
	}
};

void
OutputDelegate::
out (context_iter_t start, context_iter_t end)  {
	typedef boost::shared_ptr<Emitter> p;
	// the text is all enqueued for later emission, after
	// we have a final list of #include directives to emit.
	if (m_dest.top () == out_header)
		m_header_text.push_back (p (new RangeEmitter(make_pair(start,end))));
	else if (m_dest.top () == out_source)
		m_source_text.push_back (p (new RangeEmitter(make_pair(start,end))));
	// other text is ignored.
}

void
OutputDelegate::
out (string text) {
	typedef boost::shared_ptr<Emitter> p;
	if (m_dest.top () == out_header)
		m_header_text.push_back (p (new StringEmitter (text)));
	else if (m_dest.top () == out_source)
		m_source_text.push_back (p (new StringEmitter (text)));
	// again, other text is ignored.
}

void
OutputDelegate::
emit () {
	// first, generate our include text.
	string include_text;
	for (set<string>::iterator it = m_includes.begin ();
	     it != m_includes.end ();
	     ++it) 
		include_text.append ((format("#include \"%s\"\n") % *it).str());
	
	include_text.append ("\n");

	typedef std::vector< boost::shared_ptr<Emitter> >::iterator iter_t;
	
	//
	// Now, the header.	
	m_header << include_text;
	for (iter_t it= m_header_text.begin (); it != m_header_text.end (); ++it)
		(**it)(m_header);
// 	for_each (m_header_text.begin (), m_header_text.end (),
// 	          _1(m_header));
	
	//
	// Finally, the source.
	m_source << include_text;
	for (iter_t it= m_source_text.begin (); it != m_source_text.end (); ++it)
		(**it)(m_source);
// 	for_each (m_source_text.begin (), m_source_text.end (),
// 	          bind<void>(Emitter::operator(), _1, m_source));

	m_emitted = true;
}
