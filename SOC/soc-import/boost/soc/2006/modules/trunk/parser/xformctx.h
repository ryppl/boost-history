#ifndef INCLUDE_XFORMCTX_H
#define INCLUDE_XFORMCTX_H

#include "lexpolicies.h"
#include "../driver/output.h"
#include <vector>
#include <stack>
#include "operations.h"
/** @file xformctx.h
    \brief Transformation Context Management
    
    We go through three passes to get our transformations complete:
    -# An analysis phase, where we store some TransformStages around specific
       ranges of iterators.  As we may have some backtracking during parse,
       we need this to be a seperate phase from the next, so we enter that
       phase with a fully-known parse.
    -# A transform phase, where the TransformStages are arranged on a stack
       as their relevant portions of the source text come in and out of
       scope.  The TransformStages transform the text, generating transformed
       text for both source and header versions.
    -# A generation phase, where the transformed text is streamed to files.
       This phase is seperate as the prior phase may cause out-of-order
       text generation, such as adding file #includes.
*/

// Single instance per file transformed
class TransformContext {
    std::list<TransformStage_p>    m_annotations;
    std::list<TransformStage_p>    m_curstate;
    context_iter_t                 m_position;
    bool                           m_moving;
    std::list<TransformStage_p>::iterator m_ann_pos;
    std::vector<std::string>       m_modules;
    
    std::vector<Operation_p>       m_for_header, 
                                   m_for_source;

    /** @name StateVars
     * State Variables during transform
     */
    //@{
    
    /// should we emit tokens the header?
    bool   m_header_emit;
    //@}


    /**
      Move to the specified state in the iteration
      range.
    */
    void set_position (const context_iter_t& pos);


public:
    TransformContext (/*OutputDelegate *del*/);
    ~TransformContext ();

    /** @name StateVar_Methods
     * Methods to manipulate the state variables during output.
     *
     * Warning: these can be changed by inner and outer TransformStages.
     */
    //@{
    
    void start_hdr_emit () { m_header_emit = true; }
    void stop_hdr_emit () { m_header_emit = false; }
    
    //@}

    /** @name TransformationContext_State
     * The Context has two phases of operation: a construction
     * phase, and a playback phase.  Use add() to put in TransformStage
     * objects, and set_position() for playback.  Once set_position()
     * is called, don't call add().  set_position can't be used
     * for reverse operations.
     */
    //@{

        /**
           Add a TransformStage object to our internal stack.
           
           Ordering Note: We know that the TransformStage objects
           are going to be added in a semi-stack order, so we
           sort with higher precedence for later objects.
           (e.g. for two identical start positions, the object
           added latest will have its at_start() functor run 
           first )
           
           Copying note: we make one copy of the TransformStage object
           we're handed.
        */
        void add (TransformStage_p m);
        void add (TransformStage * p) { add (TransformStage_p(p)); }
    
        /// Go through the text, apply the transforms, and emit to the
        /// outputdelegates.
        void output (const context_iter_t& start,
                     const context_iter_t& end,
                     OutputDelegate * header,
                     OutputDelegate * source);
    
        /// called by the transforms to mention modules declared in
        /// the processed text.
        void register_module (const std::string& module) 
          { m_modules.push_back(module); }
        
        /// returns all the modules registered.
        std::vector<std::string> all_modules() { return m_modules; }

        /// called by a transformer, during at_start or at_end.
        void add_header (Operation_p p) { m_for_header.push_back(p); }
        void add_source (Operation_p p) { m_for_source.push_back(p); }

    //@}
};

#endif
