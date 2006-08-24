// xformctx.cpp
#include "xformctx.h"
#include <assert.h>
using namespace std;

bool 
operator< (const Markup& left, const Markup& right) {
	return left.start < right.start;
}

bool 
operator == (const context_iter_t& a, const context_iter_t& b) {
	return (!(a<b)) && (!(b<a));
}

bool 
operator >= (const context_iter_t& a, const context_iter_t& b) {
	return (a == b) || (b < a);
}

bool 
operator > (const context_iter_t& a, const context_iter_t& b) {
	return (b < a);
}


TransformContext::
TransformContext (OutputDelegate *del) : m_moving (false), m_outdel(del) {}

TransformContext::
~TransformContext () {}

void
TransformContext::
add (const Markup& m) {
	assert (!m_moving);
	// do our own sorted insert.
	if (m_annotations.empty ()) {
		m_annotations.push_back (m);
	} else {
		list<Markup>::iterator it = m_annotations.begin ();
		while (   (it != m_annotations.end ()) 
		       && (*it < m)) {
		     ++it;
		}		
		// TODO: lookup semantics of insert (end(),m), I
		// can probably take this if() out altogether.
// 		if (it == m_annotations.end ())
// 			m_annotations.push_back (m);
// 		else
			m_annotations.insert (it, m);
	}
}

void
TransformContext::
set_position (const context_iter_t& pos) {
	// start our traversal.
	// our annotations list is locked now.
	if (!m_moving) {
		m_ann_pos = m_annotations.begin ();
		m_moving = true;
	}
	
//	assert (pos >= m_ann_pos->start);
	
	// first, scan the current state for any Markups we
	// have to leave.  Remove them in stack order, and
	// call their at_end()s.
	while (m_curstate.size () && m_curstate.top()->end <= pos) {
		(*m_curstate.top()->at_end)(m_outdel);
		m_curstate.pop ();
	}
	
	// next, add any new items to the stack, and invoke
	// their at_begin()s.
	while (   (m_ann_pos != m_annotations.end ())
	       && (m_ann_pos->start <= pos)) {
		m_curstate.push (&*m_ann_pos);
		(*m_ann_pos->at_start)(m_outdel);
		m_ann_pos++;
	}
}