#ifndef INCLUDE_XFORMCTX_H
#define INCLUDE_XFORMCTX_H

#include "lexpolicies.h"
#include "../driver/output.h"
#include <vector>
#include <stack>

// xformctx.h
// Transformation Context Data
//
//  We keep track of what iterator ranges are under what transformation
// context states (e.g. in-module, public, etc), and use them to determine
// our output policy for various tokens.

struct MarkupFunctor {
	virtual void operator ()(OutputDelegate *) {}
	virtual ~MarkupFunctor () {}
};

struct Markup {
	context_iter_t start, end;
	MarkupFunctor *at_start, *at_end;
};

bool operator< (const Markup& left, const Markup& right);

// Single instance per file transformed
class TransformContext {
	std::list<Markup>    m_annotations;
	std::stack<Markup*>  m_curstate;
	context_iter_t       m_position;
	std::list<Markup>::iterator m_ann_pos;
	bool                 m_moving;
	OutputDelegate      *m_outdel;
public:
	TransformContext (OutputDelegate *del);
	~TransformContext ();


	/** @name TransformationContext_State
	 * The Context has two phases of operation: a construction
	 * phase, and a playback phase.  Use add() to put in Markup
	 * objects, and set_position() for playback.  Once set_position()
	 * is called, don't call add().  set_position can't be used
	 * for reverse operations.
	 */
	//@{

		/**
		   Add a Markup object to our internal stack.
		   
		   Ordering Note: We know that the Markup objects
		   are going to be added in a semi-stack order, so we
		   sort with higher precedence for later objects.
		   (e.g. for two identical start positions, the object
		   added latest will have its at_start() functor run 
		   first )
		   
		   Copying note: we make one copy of the Markup object
		   we're handed.
		*/
		void add (const Markup& m);
		
		/**
		  Move to the specified state in the iteration
		  range.
		*/
		void set_position (const context_iter_t& pos);

	//@}
};

#endif
