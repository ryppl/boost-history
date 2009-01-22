#ifndef STATEMACHINE_H
#define STATEMACHINE_H

// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/mpl/fold.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/logical.hpp>

#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>
#include <queue>
#include <functional>
#include <numeric>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/function.hpp>
#include <boost/msm/metafunctions.hpp>
#include <boost/msm/history_policies.hpp>
#include <boost/msm/bind_helpers.hpp>
#include <boost/msm/copy_policies.hpp>

namespace mpl = boost::mpl;

// traits used for entry/exit states
BOOST_MPL_HAS_XXX_TRAIT_DEF(no_automatic_create)
BOOST_MPL_HAS_XXX_TRAIT_DEF(direct_entry)
BOOST_MPL_HAS_XXX_TRAIT_DEF(explicit_entry_state)

namespace boost { namespace msm
{

// the interface for all states. Defines entry and exit functions. Overwrite to implement for any state needing it.
struct state_base
{
    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~state_base() {}
#else
    ~state_base() {}
#endif
    // empty implementation for the states not wishing to define an entry condition
    // will not be called polymorphic way
	template <class Event>
    void on_entry(Event const& incomingEvent){}
	template <class Event>
    void on_exit(Event const& incomingEvent){}
};
typedef boost::shared_ptr<state_base>   pstate_base;

struct NoSMPtr 
{
    // tags
    typedef mpl::bool_<false>   needs_sm;
};
struct SMPtr 
{
    // tags
    typedef mpl::bool_<true>    needs_sm;
};

// used for disable_if
template <int> struct dummy { dummy(int) {} };

// return value for transition handling
typedef enum
{
    HANDLED_FALSE=0,
    HANDLED_TRUE =1,
    HANDLED_GUARD_REJECT=2
} HandledEnum;
typedef boost::tuple<HandledEnum,const std::vector<int>*> execute_return;

// provides the typedefs and interface. Concrete states derive from it.
// template argument: pointer-to-fsm policy
template<class SMPtrPolicy = NoSMPtr>
struct state :  public state_base, SMPtrPolicy
{
    // tags
    typedef mpl::bool_<false>   is_composite_tag;
    // default: no flag
    typedef mpl::vector<>       flag_list;
    //default: no deferred events
    typedef mpl::vector<>       deferred_events;
    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~state() {}
#else
    ~state() {}
#endif
};

// flags used internally to handle terminate / interrupt states
struct TerminateFlag {};
struct InterruptedFlag {};
template <class EndEvent>
struct EndInterruptFlag {};

// terminate state simply defines the TerminateFlag flag
// template argument: pointer-to-fsm policy
template<class SMPtrPolicy = NoSMPtr>
struct terminate_state : public state_base, SMPtrPolicy
{
    // tags
    typedef mpl::bool_<false>                           is_composite_tag;
    typedef mpl::vector<boost::msm::TerminateFlag>      flag_list;
    //default: no deferred events
    typedef mpl::vector<>                               deferred_events;
    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~terminate_state() {}
#else
    ~terminate_state() {}
#endif
};

// terminate state simply defines the InterruptedFlag and EndInterruptFlag<EndInterruptEvent> flags
// template argument: event which ends the interrupt
// template argument: pointer-to-fsm policy
template <class EndInterruptEvent,class SMPtrPolicy = NoSMPtr>
struct interrupt_state : public state_base, SMPtrPolicy
{
    // tags
    typedef mpl::bool_<false>                       is_composite_tag;
    typedef mpl::vector<boost::msm::InterruptedFlag,
                        boost::msm::EndInterruptFlag<EndInterruptEvent> >       
                                                    flag_list;
    //default: no deferred events
    typedef mpl::vector<>                           deferred_events;
    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~interrupt_state() {}
#else
    ~interrupt_state() {}
#endif
};

// not a state but a bunch of extra typedefs to handle direct entry into a composite state. To be derived from
// template argument: containing composite
// template argument: zone index of this state
template <class Composite,int ZoneIndex=-1>
struct explicit_entry 
{
    typedef int no_automatic_create;
    typedef int explicit_entry_state;
    typedef Composite owner;
    enum {zone_index=ZoneIndex};
    template <class ToTest>
    // metafunction helping determine who needs to create this state
    struct is_owning_composite 
    {
        typedef typename boost::is_same<Composite,ToTest>::type type;
    };
};

// to be derived from. Makes a type an entry (pseudo) state. Actually an almost full-fledged state
// template argument: containing composite
// template argument: zone index of this state
// template argument: pointer-to-fsm policy
template<class Composite,int ZoneIndex=-1,class SMPtrPolicy = NoSMPtr>
struct entry_pseudo_state
    :  public state_base, explicit_entry<Composite,ZoneIndex> ,SMPtrPolicy
{
    // tags
    typedef mpl::bool_<false>   is_composite_tag;
    typedef int                 pseudo_entry;
    // default: no flag
    typedef mpl::vector<>       flag_list;
    //default: no deferred events
    typedef mpl::vector<>       deferred_events;
    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~entry_pseudo_state() {}
#else
    ~entry_pseudo_state() {}
#endif
};

// to be derived from. Makes a state an exit (pseudo) state. Actually an almost full-fledged state
// template argument: containing composite
// template argument: event to forward
// template argument: pointer-to-fsm policy
template<class Composite,class Event,class SMPtrPolicy = NoSMPtr>
struct exit_pseudo_state : public state_base , SMPtrPolicy
{
    // tags
    typedef mpl::bool_<false>   is_composite_tag;
    typedef int                 pseudo_exit;
    typedef int                 no_automatic_create;
    typedef Composite           owner;
    typedef Event               event;

    // metafunction helping determine who needs to create this state
    template <class ToTest>
    struct is_owning_composite 
    {
        typedef typename boost::is_same<Composite,ToTest>::type type;
    };
    // default: no flag
    typedef mpl::vector< > flag_list;
    //default: no deferred events
    typedef mpl::vector<>  deferred_events;

    // by default are states not polymorphic. Define this symbol to make them polymorphic
    // for example if you need to use a pointer to a state_base in typeid
#ifdef POLYMORPHIC_STATES
    virtual ~exit_pseudo_state() {}
#else
    ~exit_pseudo_state() {}
#endif
    // forward event to the higher-level FSM
    template <class ForwardEvent>
    void forward_event(ForwardEvent const& incomingEvent)
    {
        // use helper to forward or not
        helper(incomingEvent);
    }
    void set_forward_fct(boost::function<execute_return (Event const&)> fct)
    {
        m_forward = fct;
    }    
    exit_pseudo_state():m_forward(){}
    // by assignments, we keep our forwarding functor unchanged as our containing SM did not change
    exit_pseudo_state(exit_pseudo_state<Composite,Event>& rhs){}
    exit_pseudo_state<Composite,Event>& operator= (const exit_pseudo_state<Composite,Event>& rhs) 
    { 
        return *this; 
    } 
private:
    boost::function<execute_return (Event const&)>           m_forward;

    // helper used to forward an event if it is the one we are supposed to
    template <class ForwardEvent>
    typename boost::enable_if<typename boost::is_convertible<ForwardEvent,Event>::type,void >::type
        helper(ForwardEvent const& incomingEvent,boost::msm::dummy<0> = 0)
    {
        // call if handler set, if not, this state is simply a terminate state
        if (m_forward)
            m_forward(incomingEvent);
    }
    template <class ForwardEvent>
    typename boost::disable_if<typename boost::is_convertible<ForwardEvent,Event>::type,void >::type
        helper(ForwardEvent const& incomingEvent,boost::msm::dummy<1> = 0)
    {
        // Not our event, ignore
    }
};

// wrapper for mpl::for_each as showed in the C++ Template Metaprogramming ch. 9
template <class T>
struct wrap{};
// event used internally for wrapping a direct entry
template <class StateType,class Event>
struct direct_entry_event 
{
    typedef int direct_entry;
    typedef StateType active_state;

    direct_entry_event(Event const& evt):m_event(evt){}
    Event const& m_event;
};

// forward declaration
template<class Derived,class HistoryPolicy,class CopyPolicy>
class state_machine;

// Generates a singleton runtime lookup table that maps current state
// to a function that makes the SM take its transition on the given
// Event type.
template <class Fsm, class HistoryPolicy, class CopyPolicy,class Stt, class Event>
struct dispatch_table
{
 private:
    // This is a table of these function pointers.
    typedef std::pair<int,HandledEnum> (*cell)(Fsm&, int, pstate_base*,Event const&);
    typedef bool (*guard)(Fsm&, Event const&);

    // class used to build a chain (or sequence) of transitions for a given event and start state
    // (like an UML diamond). Allows transition conflicts.
    template< typename Seq,typename AnEvent,typename State >
    struct chain_row
    {
        typedef State	current_state_type;
        typedef AnEvent event;

        // helper for building a disable/enable_if-controlled execute function
        struct execute_helper
        {
            template <class Sequence>
            static std::pair<int,HandledEnum> execute(Fsm& fsm, int state, pstate_base* all_states, Event const& evt,
                            typename boost::enable_if<typename mpl::empty<Sequence>::type,void >::type* dmy=0)
            {
                // if at least one guard rejected, this will be ignored, otherwise will generate an error
                return std::make_pair(state,HANDLED_FALSE);
            }

            template <class Sequence>     
            static std::pair<int,HandledEnum> execute(Fsm& fsm, int state, pstate_base* all_states, Event const& evt,
                            typename boost::disable_if<typename mpl::empty<Sequence>::type,void >::type* dmy=0)
            {
                // try the first guard
                typedef typename mpl::front<Sequence>::type first_row;
                 std::pair<int,HandledEnum> res = 
                     first_row::execute(fsm,state,all_states,evt);
                 if (HANDLED_TRUE!=res.second)
                 {
                     // if the first rejected, move on to the next one
                     std::pair<int,HandledEnum> sub_res = 
                         execute<typename mpl::pop_front<Sequence>::type>(fsm,state,all_states,evt);
                     // if at least one guards rejects, the event will not generate a call to no_transition
                     HandledEnum handled = ((HANDLED_GUARD_REJECT==sub_res.second) || (HANDLED_GUARD_REJECT==res.second))?
                            HANDLED_GUARD_REJECT:sub_res.second;
                     return std::make_pair(sub_res.first,handled);
                 }
                 return res;
            }
        };
        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Fsm& fsm, int state, pstate_base* all_states, Event const& evt)
        {
            // forward to helper
            return execute_helper::template execute<Seq>(fsm,state,all_states,evt);
        }
    };
    // nullary metafunction whose only job is to prevent early evaluation of _1
    template< typename Entry > 
    struct make_chain_row_from_map_entry
    { 
        typedef chain_row<typename Entry::second,Event,
            typename Entry::first > type;
    }; 
    // Compute the maximum state value in the sm so we know how big
    // to make the table
    typedef typename generate_state_set<Stt>::type state_list;
    BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));

    // A function object for use with mpl::for_each that stuffs
    // transitions into cells.
    struct init_cell
    {
        init_cell(dispatch_table* self_)
          : self(self_)
        {}
        
        // Cell initializer function object, used with mpl::for_each
        template <class Transition>
        void operator()(Transition const&) const
        {
            typedef typename create_stt<Fsm>::type stt; 
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,typename Transition::current_state_type>::value));
            self->entries[state_id] = &Transition::execute;
        }
    
        dispatch_table* self;
    };
    struct default_init_cell
    {
	    default_init_cell(dispatch_table* self_,cell* tofill_entries_)
		    : self(self_),tofill_entries(tofill_entries_)
	    {}
	    // Cell default-initializer function object, used with mpl::for_each
	    // initializes with call_no_transition or defer_transition
	    template <class State>
	    typename boost::disable_if<typename has_state_delayed_event<State,Event>::type,void >::type
            operator()(boost::msm::wrap<State> const&,boost::msm::dummy<0> = 0)
	    {
            typedef typename create_stt<Fsm>::type stt; 
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,State>::value));
            cell call_no_transition = &state_machine<Fsm,HistoryPolicy,CopyPolicy>::call_no_transition;
            tofill_entries[state_id] = call_no_transition;
	    }
	    template <class State>
	    typename boost::enable_if<typename has_state_delayed_event<State,Event>::type,void >::type
            operator()(boost::msm::wrap<State> const&,boost::msm::dummy<1> = 0)
	    {
            typedef typename create_stt<Fsm>::type stt; 
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,State>::value));
            cell call_no_transition = &state_machine<Fsm,HistoryPolicy,CopyPolicy>::defer_transition;
            tofill_entries[state_id] = call_no_transition;
	    }
	    dispatch_table* self;
	    cell* tofill_entries;
    };
 public:
    // initialize the dispatch table for a given Event and Fsm
    dispatch_table()
    {
        // Initialize cells for no transition
	    mpl::for_each<typename generate_state_set<Stt>::type, boost::msm::wrap<mpl::placeholders::_1> >
                        (default_init_cell(this,entries));

        // build chaining rows for rows coming from the same state and the current event
        // first we build a map of sequence for every source
        // in reverse order so that the frow's are handled first (UML priority)
        typedef typename mpl::reverse_fold<
                        // filter on event
                        mpl::filter_view
                            <Stt, boost::is_same<transition_event<mpl::placeholders::_>, Event> >,
                        // build a map
                        mpl::map<>,
                        mpl::if_<
                            // if we already have a row on this source state
                            mpl::has_key<mpl::placeholders::_1,transition_source_type<mpl::placeholders::_2> >,
                            // insert a new element in the value type
                            mpl::insert<mpl::placeholders::_1,
                                        mpl::pair<transition_source_type<mpl::placeholders::_2>,
                                                  mpl::push_back<mpl::at<mpl::placeholders::_1,
                                                                         transition_source_type<mpl::placeholders::_2> >,
                                                                 mpl::placeholders::_2 > > >,
                            // first row on this source state, make a vector with 1 element
                            mpl::insert<mpl::placeholders::_1,
                                        mpl::pair<transition_source_type<mpl::placeholders::_2>,
                                                  make_vector<mpl::placeholders::_2> > >
                                >
                       >::type map_of_row_seq;
        // and then build chaining rows for all source states having more than 1 row
        typedef typename mpl::fold<
            map_of_row_seq,mpl::vector<>,
            mpl::if_<mpl::greater<mpl::size<mpl::second<mpl::placeholders::_2> >,mpl::int_<1> >,
                     // we need row chaining
                     mpl::push_back<mpl::placeholders::_1, 
                                    make_chain_row_from_map_entry<mpl::placeholders::_2> >,
                     // just one row, no chaining, we rebuild the row like it was before
                     mpl::push_back<mpl::placeholders::_1, get_first_element_pair_second<mpl::placeholders::_2> > 
             > >::type chained_rows; 
        // Go back and fill in cells for matching transitions.
        mpl::for_each<
            chained_rows
        >(init_cell(this));
    }

    // The singleton instance.
    static const dispatch_table instance;

 public: // data members
    cell entries[max_state];
};


// This declares the statically-initialized dispatch_table instance.
template <class Fsm, class HistoryPolicy, class CopyPolicy,class Stt, class Event>
const dispatch_table<Fsm, HistoryPolicy,CopyPolicy,Stt, Event>
dispatch_table<Fsm, HistoryPolicy,CopyPolicy,Stt, Event>::instance;

// CRTP base class for state machines.  Pass the actual FSM class as
// the Derived parameter.
template<class Derived,class HistoryPolicy=NoHistory,class CopyPolicy=NoCopy>
class state_machine : public state_base, CopyPolicy
{
private: 
    typedef boost::function<
        execute_return ()>                   transition_fct;
    typedef boost::function<
        execute_return () >                  deferred_fct;
    typedef std::queue<deferred_fct >                                           deferred_events_queue_t;
    typedef std::queue<transition_fct >	                                        events_queue_t;
	typedef bool (*flag_handler)(state_machine<Derived,HistoryPolicy,CopyPolicy>&);

    // all state machines are friend with each other to allow embedding any of them in another fsm
    template <class ,class ,class > friend class state_machine;

 public: 
    // tags
    typedef mpl::bool_<true>   is_composite_tag;
    // default: no flag
    typedef mpl::vector0<>     flag_list;
    //default: no deferred events
    typedef mpl::vector<>      deferred_events;
    // in case someone needs to know
    typedef HistoryPolicy      history_policy;
    typedef CopyPolicy         copy_policy;

    struct InitEvent { };
    // flag handling
    struct Flag_AND
    {
        typedef std::logical_and<bool> type;
    };
    struct Flag_OR
    {
     typedef std::logical_or<bool> type;
    };

    // Member functions

    // start the state machine (calls entry of the initial state)
    void start()
    {
		typedef typename get_initial_states<typename Derived::initial_state>::type init_states;
		mpl::for_each<init_states, boost::msm::wrap<mpl::placeholders::_1> >(call_init<InitEvent>(&m_state_list[0],InitEvent()));
    }

    // Main function used by clients of the derived FSM to make
    // transitions. Can also be called for internally (for example in an action method) generated events.
    template<class Event>
    execute_return process_event(Event const& evt)
    {
        // extend the table with tables from composite states
        typedef typename extend_table<Derived>::type complete_table;
        // use this table as if it came directly from the user
        typedef dispatch_table<Derived,HistoryPolicy,CopyPolicy,complete_table,Event> table;

        HandledEnum ret_handled=HANDLED_FALSE;
        // if the state machine is terminated, do not handle any event
        if (is_flag_active<boost::msm::TerminateFlag>())
            return boost::make_tuple(HANDLED_TRUE,&this->m_states);
        // if the state machine is interrupted, do not handle any event
        // unless the event is the end interrupt event
        if (is_flag_active<boost::msm::InterruptedFlag>() && !is_flag_active<boost::msm::EndInterruptFlag<Event> >())
            return boost::make_tuple(HANDLED_TRUE,&this->m_states);

        // if we are already processing an event
        if (m_event_processing)
        {
            // event has to be put into the queue
            execute_return (state_machine<Derived,HistoryPolicy,CopyPolicy>::*pf) (Event const& evt) = 
                &state_machine<Derived,HistoryPolicy,CopyPolicy>::process_event; 
            transition_fct f = boost::bind(pf,this,evt);
            m_events_queue.push(f);
            return boost::make_tuple(HANDLED_TRUE,&this->m_states);
        }
        else
        {
            // event can be handled, processing
            m_event_processing = true;
            // prepare the next deferred event for handling
            deferred_fct next_deferred_event;
            if (!m_deferred_events_queue.empty())
            {
                next_deferred_event = m_deferred_events_queue.front();
                m_deferred_events_queue.pop();
            }
            typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
            bool handled = false;
            try
            {
                // dispatch the event to every region
                for (int i=0; i<nr_regions::value;++i)
                {	
                    std::pair<int,HandledEnum> res =
                        table::instance.entries[this->m_states[i]](
                        *static_cast<Derived*>(this), this->m_states[i], &m_state_list[0],evt);
                    this->m_states[i] = res.first;
                    handled = (handled || res.second);
                }
                // if the event has not been handled and we have orthogonal zones, then
                // generate an error on every active state 
                // for state machine states contained in other state machines, do not handle
                // but let the containing sm handle the error
                if (!handled && !is_contained())
                {
                    for (int i=0; i<nr_regions::value;++i)
                    {	
                        (static_cast<Derived*>(this))->no_transition(this->m_states[i],evt);
                    }
                }
            }
            catch (std::exception& e)
            {
                // give a chance to the concrete state machine to handle
                (static_cast<Derived*>(this))->exception_caught(e);
            }
            if (handled)
            {
                ret_handled = HANDLED_TRUE;
            }
            // after handling, take care of the deferred events
            if (next_deferred_event)
            {
                next_deferred_event();
            }
            m_event_processing = false;
            // now check if some events were generated in a transition and was not handled
            // because of another processing, and if yes, start handling them
            process_message_queue();
            return boost::make_tuple(ret_handled,&this->m_states);
        }       
    }

    // Getter that returns the current state of the FSM
    const std::vector<int>& current_state() const
    {
        return this->m_states;
    }

    // return the state whose id is passed or 0 if not found
    // caution if you need this, you probably need to define POLYMORPHIC_STATES
    const state_base* get_state_by_id(int id)
    {
        typedef typename create_stt<Derived>::type stt;
        typedef typename generate_state_set<stt>::type state_list;
        BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));
        BOOST_ASSERT(id < (max_state));
        if (id < max_state)
            return (this->m_state_list[id]).get();
        // error
        return 0;
    }
    // true if the sm is used in another sm
    bool is_contained()
    {
        return m_is_included;
    }
    // get a state
    // as a pointer
    template <class State>
    State get_state(typename boost::enable_if<typename boost::is_pointer<State>::type,void >::type* =0)
    {
        typedef typename create_stt<Derived>::type stt;
        return &(static_cast<typename boost::add_reference<typename boost::remove_pointer<State>::type>::type > 
            (*(m_state_list[get_state_id<stt,typename boost::remove_pointer<State>::type>::type::value])));	
    }
    // as a reference
    template <class State>
    State get_state(typename boost::enable_if<typename boost::is_reference<State>::type,void >::type* =0)
    {
        typedef typename create_stt<Derived>::type stt;
        return static_cast<State > 
            (*(m_state_list[get_state_id<stt,typename boost::remove_reference<State>::type>::type::value]));	
    }	

    // checks if a flag is active using the BinaryOp as folding function
    template <class Flag,class BinaryOp>
    bool is_flag_active()
    {
        flag_handler* flags_entries = get_entries_for_flag<Flag>();

        typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
        return std::accumulate(m_states.begin(),
            m_states.end(),false,
            boost::bind(typename BinaryOp::type(),
                boost::bind(boost::apply<bool>(),
                    boost::bind(deref<flag_handler>(),
                        boost::bind(plus2<flag_handler*,int>(),
                        flags_entries, ::_2)),
                        boost::ref(*this)), ::_1));		
    }
    // checks if a flag is active using no binary op if 1 region, or OR if > 1 regions
    template <class Flag>
    bool is_flag_active()
    {
        typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
        return FlagHelper<Flag,(nr_regions::value>1)>::helper(*this,get_entries_for_flag<Flag>());
    }

 protected:    // interface for the derived class
     typedef std::vector<pstate_base>	pstate_base_list;
	 
     // helper used to fill the initial states
     struct init_states
     {
         init_states(std::vector<int>& init):m_initial_states(init){}

         // History initializer function object, used with mpl::for_each
         template <class State>
         void operator()(boost::msm::wrap<State> const&)
         {
             typedef typename create_stt<Derived>::type stt; 
             m_initial_states.push_back(get_state_id<stt,State>::type::value);
         }
         std::vector<int>& m_initial_states;
     };
     // Construct with the default initial states
     state_machine()
	     : m_states(),
	     m_events_queue() ,
	     m_deferred_events_queue(),
	     m_history(),
	     m_state_list(),
         m_event_processing(false),
         m_is_included(false)
     {
         typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
         m_states.reserve(nr_regions::value);
         // build a sequence of regions if needed
         typedef typename get_regions_as_sequence<typename Derived::initial_state>::type initial_states;
         // initialize our list of states with the ones defined in Derived::initial_state
         mpl::for_each<
             initial_states, boost::msm::wrap<mpl::placeholders::_1>  
         >(init_states(m_states));
         m_history.set_initial_states(m_states);
         fill_states(this);
     }
     // template constructor. Needed only for sub-fsms having exit pseudo states.
     template <class ContainingSM>
     state_machine(ContainingSM* containing_sm)
         : m_states(),
         m_events_queue() ,
         m_deferred_events_queue(),
         m_history(),
         m_state_list(),
         m_event_processing(false),
         m_is_included(true)
     {
         typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
         m_states.reserve(nr_regions::value);
         // build a sequence of regions if needed
         typedef typename get_regions_as_sequence<typename Derived::initial_state>::type initial_states;
         // initialize our list of states with the ones defined in Derived::initial_state
         mpl::for_each<
             initial_states, boost::msm::wrap<mpl::placeholders::_1>  
         >(init_states(m_states));
         m_history.set_initial_states(m_states);
         fill_states(containing_sm);
     }
     // assignment operator using the copy policy to decide if non_copyable, shallow or deep copying is necessary
     state_machine<Derived,HistoryPolicy,CopyPolicy>& operator= (state_machine<Derived,HistoryPolicy,CopyPolicy> const& rhs)
     {
         if (this != &rhs) 
         {
            CopyPolicy::operator=(rhs);
            do_copy<mpl::bool_<CopyPolicy::shallow_copy::value> >(rhs);
         }
        return *this;
     }
     state_machine<Derived,HistoryPolicy,CopyPolicy> (state_machine<Derived,HistoryPolicy,CopyPolicy> const& rhs):CopyPolicy(rhs)
     {
        if (this != &rhs) 
        {
            typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
            m_states.reserve(nr_regions::value);
            // build a sequence of regions if needed
            typedef typename get_regions_as_sequence<typename Derived::initial_state>::type initial_states;
            // initialize our list of states with the ones defined in Derived::initial_state
            fill_states(this);
            do_copy<mpl::bool_<CopyPolicy::shallow_copy::value> >(rhs);
        }
     }
    // default row arguments for the compilers which accept this
    template <class Event>
    bool no_guard(Event const&){return true;}
    template <class Event>
    void no_action(Event const&){}

    // Template used to form rows in the transition table
    template<
        typename T1
        , class Event
        , typename T2
        , void (Derived::*action)(Event const&) 
#if  !defined(__GNUC__)
        = static_cast<void (Derived::*)(Event const&)>(&no_action<Event>)
#endif
        , bool (Derived::*guard)(Event const&) 
#if  !defined(__GNUC__)
        = static_cast<bool (Derived::*)(Event const&)>(&no_guard<Event>)
#endif
    >
    struct row
    {
        // if the source has no automatic creation (i.e. is an exit pseudo state), then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename mpl::eval_if<
            typename has_no_automatic_create<T1>::type,
            get_owner<T1,Derived>,
            mpl::identity<T1> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename mpl::eval_if<
            typename mpl::is_sequence<T2>::type,
            get_fork_owner<T2,Derived>,
            mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,Derived>,
                    mpl::identity<T2> >
        >::type next_state_type;

        typedef Event event;
        typedef mpl::bool_<false>	internal_event;

        // if a guard condition is here, call it to check that the event is accepted
        static bool check_guard(Derived& fsm,Event const& evt)
        {
            if (!guard || (guard && (fsm.*guard)(evt)))
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Derived& fsm, int state, pstate_base* all_states, Event const& evt)
        {
            typedef typename create_stt<Derived>::type stt;

            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (!boost::is_same<T1,current_state_type>::value && !is_exit_state_active<T1,get_owner<T1,Derived> >(fsm))
            {
                return std::make_pair(current_state,HANDLED_FALSE);
            }
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return std::make_pair(current_state,HANDLED_GUARD_REJECT);
            }

            // the guard condition has already been checked
            execute_exit<current_state_type>(all_states[current_state].get(),evt);
            if (action)
            {
                // then call the action method
                (fsm.*action)(evt);
            }
            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>(all_states[next_state].get(),evt);
            return std::make_pair(next_state,HANDLED_TRUE);
        }
    };

    // row having only a guard condition
    template<
        typename T1
        , class Event
        , typename T2
        , bool (Derived::*guard)(Event const&)
    >
    struct g_row
    {
        // if the source has no automatic creation (i.e. is an exit pseudo state), then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename mpl::eval_if<
            typename has_no_automatic_create<T1>::type,
            get_owner<T1,Derived>,
            mpl::identity<T1> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename mpl::eval_if<
            typename mpl::is_sequence<T2>::type,
            get_fork_owner<T2,Derived>,
            mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,Derived>,
                    mpl::identity<T2> >
        >::type next_state_type;

        typedef Event event;
        typedef mpl::bool_<false>	internal_event;

        // if a guard condition is defined, call it to check that the event is accepted
        static bool check_guard(Derived& fsm,Event const& evt)
        {
            if (!guard || (guard && (fsm.*guard)(evt)))
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Derived& fsm, int state, pstate_base* all_states, Event const& evt)
        {
            typedef typename create_stt<Derived>::type stt;
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (!boost::is_same<T1,current_state_type>::value && !is_exit_state_active<T1,get_owner<T1,Derived> >(fsm))
            {
                return std::make_pair(current_state,HANDLED_FALSE);
            }
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return std::make_pair(current_state,HANDLED_GUARD_REJECT);
            }
            // the guard condition has already been checked
            execute_exit<current_state_type>(all_states[current_state].get(),evt);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>(all_states[next_state].get(),evt);
            return std::make_pair(next_state,HANDLED_TRUE);
        }
    };

    // row having only an action method
    template<
        typename T1
        , class Event
        , typename T2
        , void (Derived::*action)(Event const&)
    >
    struct a_row
    {
        // if the source has no automatic creation (i.e. is an exit pseudo state), then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename mpl::eval_if<
            typename has_no_automatic_create<T1>::type,
            get_owner<T1,Derived>,
            mpl::identity<T1> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename mpl::eval_if<
            typename mpl::is_sequence<T2>::type,
            get_fork_owner<T2,Derived>,
            mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,Derived>,
                    mpl::identity<T2> >
        >::type next_state_type;

        typedef Event event;
        typedef mpl::bool_<false>	internal_event;

        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Derived& fsm, int state, pstate_base* all_states, Event const& evt)
        {
            typedef typename create_stt<Derived>::type stt;
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));

            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (!boost::is_same<T1,current_state_type>::value && !is_exit_state_active<T1,get_owner<T1,Derived> >(fsm))
            {
                return std::make_pair(current_state,HANDLED_FALSE);
            }
            // no need to check the guard condition
            // first call the exit method of the current state
            execute_exit<current_state_type>(all_states[current_state].get(),evt);
            if (action)
            {
                // then call the action method
                (fsm.*action)(evt);
            }
            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>(all_states[next_state].get(),evt);
            return std::make_pair(next_state,HANDLED_TRUE);
        }
    };

    // row having no guard condition or action, simply transitions
    template<
        typename T1
        , class Event
        , typename T2
    >
    struct _row
    {
        // if the source has no automatic creation (i.e. is an exit pseudo state), then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename mpl::eval_if<
                typename has_no_automatic_create<T1>::type,
                get_owner<T1,Derived>,
                mpl::identity<T1> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename mpl::eval_if<
            typename mpl::is_sequence<T2>::type,
            get_fork_owner<T2,Derived>,
            mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,Derived>,
                    mpl::identity<T2> >
        >::type next_state_type;

        typedef Event event;
        typedef mpl::bool_<false>	internal_event;

        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Derived& fsm, int state, pstate_base* all_states, Event const& evt)
        {
            typedef typename create_stt<Derived>::type stt;
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));

            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (!boost::is_same<T1,current_state_type>::value && !is_exit_state_active<T1,get_owner<T1,Derived> >(fsm))
            {
                return std::make_pair(current_state,HANDLED_FALSE);
            }
            // first call the exit method of the current state
            execute_exit<current_state_type>(all_states[current_state].get(),evt);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>(all_states[next_state].get(),evt);
            return std::make_pair(next_state,HANDLED_TRUE);
        }
    };
private:

	// helper used to call the init states at the start of the state machine
    template <class Event>
	struct call_init
	{
		call_init(pstate_base* states,Event const& an_event):all_states(states),evt(an_event){}
		template <class State>
		void operator()(boost::msm::wrap<State> const&)
		{
            typedef typename create_stt<Derived>::type stt;
			execute_entry<State>(all_states[get_state_id<stt,State>::type::value].get(),evt);
		}
	private:
		pstate_base* all_states;
        Event const& evt;
	};
    // helper for flag handling. Uses OR by default on orthogonal zones.
    template <class Flag,bool orthogonalStates>
    struct FlagHelper 
    {
        static bool helper(state_machine<Derived,HistoryPolicy,CopyPolicy>& sm,flag_handler* )
        {
            // by default we use OR to accumulate the flags
            return sm.is_flag_active<Flag,Flag_OR>();
        }
    };
    template <class Flag>
    struct FlagHelper<Flag,false>
    {
        static bool helper(state_machine<Derived,HistoryPolicy,CopyPolicy>& sm,flag_handler* flags_entries)
        {
            // just one active state, so we can call operator[] with 0
            return flags_entries[sm.current_state()[0]](sm);
        }
    };
    // handling of flag
    // defines a true and false functions plus a forwarding one for composite states
    template <class StateType,class Flag>
    struct FlagHandler
    {
        static bool flag_true(state_machine<Derived,HistoryPolicy,CopyPolicy>& )
        {
            return true;
        }
        static bool flag_false(state_machine<Derived,HistoryPolicy,CopyPolicy>& )
        {
            return false;
        }
        static bool forward(state_machine<Derived,HistoryPolicy,CopyPolicy>& fsm)
        {
            typedef typename create_stt<Derived>::type stt;
            return (static_cast<StateType& > 
                (*(fsm.m_state_list[get_state_id<stt,StateType>::type::value]))).is_flag_active<Flag>();
        }
    };
    template <class Flag>
    struct init_flags
    {
    private:
        // helper function, helps hiding the forward function for non-state machines states.
        template <class T>
        typename boost::enable_if<typename is_composite_state<T>::type,void >::type
            helper (flag_handler* an_entry,int offset,boost::msm::dummy<0> = 0 )
        {
            // composite => forward
            an_entry[offset] = &FlagHandler<T,Flag>::forward;
        }
        template <class T>
        typename boost::disable_if<typename is_composite_state<T>::type,void >::type
            helper (flag_handler* an_entry,int offset,boost::msm::dummy<1> = 0 )
        {
            // default no flag
            an_entry[offset] = &FlagHandler<T,Flag>::flag_false;
        }
        // attributes
        flag_handler* entries;

    public:
        init_flags(flag_handler* entries_)
            : entries(entries_)
        {}

        // Flags initializer function object, used with mpl::for_each
        template <class StateType>
        void operator()(boost::msm::wrap<StateType> const& )
        {
            typedef typename StateType::flag_list flags;
            typedef typename mpl::contains<flags,Flag >::type found;
            typedef typename is_composite_state<StateType>::type composite;
            typedef typename create_stt<Derived>::type stt;

            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,StateType>::type::value));
            if (found::type::value)
            {
                // the type defined the flag => true
                entries[state_id] = &FlagHandler<StateType,Flag>::flag_true;
            }
            else
            {
                // false or forward
                helper<StateType>(entries,state_id);
            }
        }
    };
    // maintains for every flag a static array containing the flag value for every state
    template <class Flag>
    flag_handler* get_entries_for_flag()
    {
        typedef typename create_stt<Derived>::type Stt;
        typedef typename generate_state_set<Stt>::type state_list;
        BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));

        static flag_handler flags_entries[max_state];
        // build a state list
        mpl::for_each<state_list, boost::msm::wrap<mpl::placeholders::_1> >(init_flags<Flag>(flags_entries));
        return flags_entries;	
    }

    // helper used to create a state using the correct constructor
    template <class State, class Enable=void>
    struct create_state_helper
    {
        static void set_sm(state_machine<Derived,HistoryPolicy,CopyPolicy>* ,pstate_base )
        {
	        // state doesn't need its sm
        }
    };
    // create a state requiring a pointer to the state machine
    template <class State>
    struct create_state_helper<State,typename boost::enable_if<typename State::needs_sm >::type>
    {
        static void set_sm(state_machine<Derived,HistoryPolicy,CopyPolicy>* sm,pstate_base new_state)
        {
	        // create and set the fsm
	        static_cast<State*>(new_state.get())->set_sm_ptr(static_cast<Derived*>(sm));
        }
    };
	// A function object for use with mpl::for_each that stuffs
    // states into the state list.
    template<class ContainingSM>
    struct add_state
    {
        add_state(state_machine<Derived,HistoryPolicy,CopyPolicy>* self_,ContainingSM* sm)
	        : self(self_),containing_sm(sm){}

        // State is a sub fsm with exit pseudo states and gets a pointer to this fsm, so it can build a callback
        template <class StateType>
        typename boost::enable_if<
            typename has_exit_pseudo_states<StateType>::type,state_base* >::type
        new_state_helper(boost::msm::dummy<0> = 0) const
        {
            return new StateType(containing_sm);
        }
        // State is a sub fsm without exit pseudo states and does not get a callback to this fsm
		// or state is a normal state and needs nothing except creation
        template <class StateType>
        typename boost::enable_if<
					typename mpl::and_<typename mpl::not_<typename has_exit_pseudo_states<StateType>::type>::type,
									   typename mpl::not_<typename is_pseudo_exit<StateType>::type>::type
			       >::type,state_base*>::type
        new_state_helper(boost::msm::dummy<1> = 0) const
        {
            return new StateType;
        }
        // state is exit pseudo state and gets callback to target fsm
        template <class StateType>
        typename boost::enable_if<typename is_pseudo_exit<StateType>::type,state_base* >::type
        new_state_helper(boost::msm::dummy<2> = 0) const
        {
            state_base* to_return = new StateType();
            execute_return (ContainingSM::*pf) (typename StateType::event const& evt)= 
                &ContainingSM::process_event;
            boost::function<execute_return (typename StateType::event const&)> fct = 
                boost::bind(pf,containing_sm,::_1);
            static_cast<StateType*>(to_return)->set_forward_fct(fct);
            return to_return;
        }
        // for every defined state in the sm
        template <class State>
        void operator()(boost::msm::wrap<State> const&) const
        {
            //create a new state with the defined id and type
            typedef typename create_stt<Derived>::type stt;
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,State>::value));

            pstate_base new_state (this->new_state_helper<State>());
            self->m_state_list[state_id]= new_state;
            create_state_helper<State>::set_sm(self,new_state);
        }
    private:
        state_machine<Derived,HistoryPolicy,CopyPolicy>* self;
        ContainingSM*                                    containing_sm;
    };

     // helper used to copy every state if needed
     struct copy_helper
     {
         copy_helper(pstate_base_list& to_fill,const pstate_base_list& rhs,state_machine<Derived,HistoryPolicy,CopyPolicy>* sm):
            m_tofill_states(to_fill),m_rhs(rhs),m_sm(sm){}
         template <class StateType>
         void operator()(boost::msm::wrap<StateType> const& )
         {
            typedef typename create_stt<Derived>::type stt;
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,StateType>::type::value));
            if (m_rhs.empty())
	            return;
            pstate_base copiedState 
	            (new StateType (*static_cast<StateType*> (m_rhs[state_id].get())));
            m_tofill_states[state_id] = copiedState;
            // and for states that keep a pointer to the fsm, reset the pointer
            create_state_helper<StateType>::set_sm(m_sm,copiedState);
         }
         pstate_base_list&                         m_tofill_states;
         const pstate_base_list&                   m_rhs;
         state_machine<Derived,
            HistoryPolicy,CopyPolicy>*	           m_sm;
     };

     // copy functions for shallow or deep copy (no need of a 3rd version for NoCopy as noncopyable handles it)
     template <class IsShallowCopy>
     typename boost::disable_if<typename IsShallowCopy::type,void >::type
     do_copy (state_machine<Derived,HistoryPolicy,CopyPolicy> const& rhs,boost::msm::dummy<0> = 0)
     {
         // deep copy simply assigns the data
         m_states = rhs.m_states;
         m_events_queue = rhs.m_events_queue;
         m_history = rhs.m_history;
         // except for the states themselves, which get duplicated
         typedef typename create_stt<Derived>::type Stt;

         typedef typename generate_state_set<Stt>::type state_list;
         mpl::for_each<state_list, boost::msm::wrap<mpl::placeholders::_1> >(copy_helper(m_state_list,rhs.m_state_list,this));
     }
     template <class IsShallowCopy>
     typename boost::enable_if<typename IsShallowCopy::type,void >::type
     do_copy (state_machine<Derived,HistoryPolicy,CopyPolicy> const& rhs,boost::msm::dummy<1> = 0)
     {
         // shallow copy simply assigns the data
         m_states = rhs.m_states;
         m_events_queue = rhs.m_events_queue;
         m_history = rhs.m_history;
         m_state_list = rhs.m_state_list;
         m_is_included = rhs.m_is_included;
     }

     // helper used to call the correct entry/exit method
     // unfortunately in O(number of states in the sub-sm) but should be better than a virtual call
     template<class Event,bool is_entry> 
     struct entry_exit_helper
     {
         entry_exit_helper(int id,Event const& e,pstate_base* states):state_id(id),evt(e),all_states(states){}
         // helper for entry actions
         template <class IsEntry,class State>
         typename boost::enable_if<typename IsEntry::type,void >::type
         helper(boost::msm::dummy<0> = 0)
         {
             typedef typename create_stt<Derived>::type stt;
             BOOST_STATIC_CONSTANT(int, id = (get_state_id<stt,State>::value));
             if (id == state_id)
             {
                execute_entry<State>(all_states[id].get(),evt);
             }
         }
         // helper for exit actions
         template <class IsEntry,class State>
         typename boost::disable_if<typename IsEntry::type,void >::type
         helper(boost::msm::dummy<1> = 0)
         {
             typedef typename create_stt<Derived>::type stt;
             BOOST_STATIC_CONSTANT(int, id = (get_state_id<stt,State>::value));
             if (id == state_id)
             {
                 execute_exit<State>(all_states[id].get(),evt);
             }
         }
         // iterates through all states to find the one to be activated
         template <class State>
         void operator()(boost::msm::wrap<State> const&)
         {
             entry_exit_helper<Event,is_entry>::template helper<mpl::bool_<is_entry>,State >();
         }
     private:
         int            state_id;
         Event const&   evt;
         pstate_base*   all_states;
     };
     // start for states machines which are themselves embedded in other state machines (composites)
	 template <class Event>
     void start(Event const& incomingEvent)
     {
         typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
         typedef typename create_stt<Derived>::type stt;
         typedef typename generate_state_set<stt>::type state_list;
         for (int i=0; i<nr_regions::value;++i)
         {
             //forward the event for handling by sub state machines
             mpl::for_each<state_list, boost::msm::wrap<mpl::placeholders::_1> >
                 (entry_exit_helper<Event,true>(m_states[i],incomingEvent,&m_state_list[0]));
         }
     }

     // helper used to set the correct state as active state upon entry into a fsm
     struct direct_event_start_helper 
     {
         direct_event_start_helper(state_machine<Derived,HistoryPolicy,CopyPolicy>* self_):self(self_){}
         // this variant is for the standard case, entry due to activation of the containing FSM
         template <class EventType>
         typename boost::disable_if<typename has_direct_entry<EventType>::type,void>::type
             operator()(EventType const& evt,boost::msm::dummy<0> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt);
             self->start(evt);
         }

         // this variant is for the direct entry case (just one entry, not a sequence of entries)
         template <class EventType>
         typename boost::enable_if<
             typename mpl::and_<typename mpl::not_< typename is_pseudo_entry<typename EventType::active_state>::type >::type,
                                typename mpl::and_<typename has_direct_entry<EventType>::type,
                                                    typename mpl::not_<typename mpl::is_sequence
                                                                            <typename EventType::active_state>::type >::type 
                                                    >::type>::type,void
                                  >::type
         operator()(EventType const& evt,boost::msm::dummy<1> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt);
             typedef typename create_stt<Derived>::type stt;
             int state_id = get_state_id<stt,typename EventType::active_state>::value;
             BOOST_STATIC_ASSERT(EventType::active_state::zone_index >= 0);
             BOOST_STATIC_ASSERT(EventType::active_state::zone_index <= 
                 get_number_of_regions<typename Derived::initial_state>::type::value);
             // just set the correct zone, the others will be default/history initialized
             self->m_states[EventType::active_state::zone_index] = state_id;
             self->start(evt.m_event);
         }

         // this variant is for the fork entry case (a sequence on entries)
         template <class EventType>
         typename boost::enable_if<
             typename mpl::and_<typename mpl::not_< typename is_pseudo_entry<typename EventType::active_state>::type >::type,
                                typename mpl::and_<typename has_direct_entry<EventType>::type,
                                                   typename mpl::is_sequence<typename EventType::active_state>::type 
                                                  >::type>::type,void 
                                >::type
         operator()(EventType const& evt,boost::msm::dummy<2> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt);
             mpl::for_each<typename EventType::active_state,boost::msm::wrap<mpl::placeholders::_1> >
                                                        (fork_helper<EventType>(self,evt));
             // set the correct zones, the others (if any) will be default/history initialized
             self->start(evt.m_event);
         }

         // this variant is for the pseudo state entry case
         template <class EventType>
         typename boost::enable_if<
             typename is_pseudo_entry<typename EventType::active_state >::type,void
                                >::type
         operator()(EventType const& evt,boost::msm::dummy<3> = 0)
         {
             // entry on the FSM
             (static_cast<Derived*>(self))->on_entry(evt);
             typedef typename create_stt<Derived>::type stt;
             int state_id = get_state_id<stt,typename EventType::active_state>::value;
             // given region starts with the entry pseudo state as active state
             self->m_states[EventType::active_state::zone_index] = state_id;
             self->start(evt.m_event);
             // and we process the transition in the zone of the newly active state
             // (entry pseudo states are, according to UML, a state connecting 1 transition outside to 1 inside
             self->process_event(evt.m_event);
         }
     private:
         // helper for the fork case, does almost like the direct entry
         state_machine<Derived,HistoryPolicy,CopyPolicy>* self;
         template <class EventType>
         struct fork_helper
         {
             fork_helper(state_machine<Derived,HistoryPolicy,CopyPolicy>* self_,EventType const& evt_):
                helper_self(self_),helper_evt(evt_){}
             template <class StateType>
             void operator()(boost::msm::wrap<StateType> const& )
             {
                 typedef typename create_stt<Derived>::type stt;
                 int state_id = get_state_id<stt,StateType>::value;
                 BOOST_STATIC_ASSERT(StateType::zone_index >= 0);
                 BOOST_STATIC_ASSERT
                     (StateType::zone_index <= get_number_of_regions<typename Derived::initial_state>::type::value);
                 helper_self->m_states[StateType::zone_index] = state_id;
             }
         private:
             state_machine<Derived,HistoryPolicy,CopyPolicy>* helper_self;
             EventType const&                                 helper_evt;
         };
     };

     // entry/exit for states machines which are themselves embedded in other state machines (composites)
	 template <class Event>
     void entry(Event const& incomingEvent)
     {
        // by default we activate the history/init states, can be overwritten by direct_event_start_helper
        typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
        for (int i=0; i<nr_regions::value;++i)
        {
            this->m_states[i] = m_history.history_entry(incomingEvent)[i];
        }
        // block immediate handling of events
        m_event_processing = true;
        // if the event is generating a direct entry/fork, set the current state(s) to the direct state(s)
        direct_event_start_helper(this)(incomingEvent);
        // handle messages which were generated and blocked in the init calls
        m_event_processing = false;
        process_message_queue();
     }
	 template <class Event>
     void exit(Event const& incomingEvent)
     {
        typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
        typedef typename create_stt<Derived>::type stt;
        typedef typename generate_state_set<stt>::type state_list;
        for (int i=0; i<nr_regions::value;++i)
        {
            // first recursively exit the sub machines
            // forward the event for handling by sub state machines
            mpl::for_each<state_list, boost::msm::wrap<mpl::placeholders::_1> >
                (entry_exit_helper<Event,false>(m_states[i],incomingEvent,&m_state_list[0]));
        }
        // then call our own exit
        (static_cast<Derived*>(this))->on_exit(incomingEvent);
        // give the history a chance to handle this (or not).
        m_history.history_exit(this->m_states);
     }

    // no transition for event.
    template <class Event>
    static std::pair<int,HandledEnum> call_no_transition(Derived& , int state, pstate_base* ,Event const& )
    {
        return std::make_pair(state,HANDLED_FALSE);
    }
    // called for deferred events. Address set in the dispatch_table at init
    template <class Event>
    static std::pair<int,HandledEnum> defer_transition(Derived& fsm, int state, pstate_base*,Event const& e)
    {
        execute_return (Derived::*pf) (Event const& evt)= &Derived::process_event;
        Event temp (e);
        boost::function<execute_return () > f= boost::bind(pf,&fsm,temp);
        fsm.post_deferred_event(f);
        return std::make_pair(state,HANDLED_TRUE);
    }

    // Default no-transition handler. Can be replaced in the Derived SM class.
    template <class Event>
    int no_transition(int state, Event const& e)
    {
        BOOST_ASSERT(false);
        return state;
    }
    // default exception handler. Can be replaced in the Derived SM class.
    void exception_caught (std::exception& )
    {
        BOOST_ASSERT(false);
    }
    // puts a deferred event in the queue
    void post_deferred_event(deferred_fct& deferred)
    {
        m_deferred_events_queue.push(deferred);
    }
    // removes one event from the message queue and processes it
    void process_message_queue()
    {
        if (!m_events_queue.empty())
        {
            transition_fct to_call = m_events_queue.front();
            m_events_queue.pop();
            to_call();
        }
    }

    // calls the entry/exit or on_entry/on_exit depending on the state type
    // (avoids calling virtually)
    // variant for FSMs
    template <class StateType,class EventType>
    static
        typename boost::enable_if<typename is_composite_state<StateType>::type,void >::type
        execute_entry(state_base* astate,EventType const& evt,boost::msm::dummy<0> = 0)
    {
        // calls on_entry on the fsm then handles direct entries, fork, entry pseudo state
        (static_cast<StateType*>(astate))->entry(evt);
    }
    // variant for states
    template <class StateType,class EventType>
    static
        typename boost::disable_if<
            typename mpl::or_<typename is_composite_state<StateType>::type,
                              typename is_pseudo_exit<StateType>::type >::type,void >::type
        execute_entry(state_base* astate,EventType const& evt,boost::msm::dummy<1> = 0)
    {
        // simple call to on_entry
        (static_cast<StateType*>(astate))->on_entry(evt);
    }
    // variant for exit pseudo states
    template <class StateType,class EventType>
    static
        typename boost::enable_if<typename is_pseudo_exit<StateType>::type,void >::type
        execute_entry(state_base* astate,EventType const& evt,boost::msm::dummy<2> = 0)
    {
        // calls on_entry on the state then forward the event to the transition which should be defined inside the 
        // contained fsm
        (static_cast<StateType*>(astate))->on_entry(evt);
        (static_cast<StateType*>(astate))->forward_event(evt);
    }
    template <class StateType,class EventType>
    static
        typename boost::enable_if<typename is_composite_state<StateType>::type,void >::type
        execute_exit(state_base* astate,EventType const& evt,boost::msm::dummy<0> = 0)
    {
        (static_cast<StateType*>(astate))->exit(evt);
    }
    template <class StateType,class EventType>
    static
        typename boost::disable_if<typename is_composite_state<StateType>::type,void >::type
        execute_exit(state_base* astate,EventType const& evt,boost::msm::dummy<1> = 0)
    {
        // simple call to on_exit
        (static_cast<StateType*>(astate))->on_exit(evt);
    }

    // helper allowing special handling of direct entries / fork
    template <class StateType,class TargetType,class EventType>
    static
        typename boost::disable_if<
            typename mpl::or_<typename has_explicit_entry_state<TargetType>::type,mpl::is_sequence<TargetType> >::type,void>::type
        convert_event_and_execute_entry(state_base* astate,EventType const& evt,boost::msm::dummy<1> = 0)
    {
        // if the target is a normal state, do the standard entry handling
        execute_entry<StateType>(astate,evt);
    }
    template <class StateType,class TargetType,class EventType>
    static
        typename boost::enable_if<
            typename mpl::or_<typename has_explicit_entry_state<TargetType>::type,mpl::is_sequence<TargetType> >::type,void >::type
        convert_event_and_execute_entry(state_base* astate,EventType const& evt,boost::msm::dummy<0> = 0)
    {
        // for the direct entry, pack the event in a wrapper so that we handle it differently during fsm entry
        execute_entry<StateType>(astate,direct_entry_event<TargetType,EventType>(evt));
    }

    // creates all the states
    template <class ContainingSM>
    void fill_states(ContainingSM* containing_sm=0)
    {
        typedef typename create_stt<Derived>::type stt;
        typedef typename generate_state_set<stt>::type state_list;
        BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));
        // allocate the place without reallocation
        m_state_list.resize(max_state);
        mpl::for_each<
	        state_list,boost::msm::wrap<mpl::placeholders::_1>  
        >(add_state<ContainingSM>(this,containing_sm));
    }

    // Template used to form forwarding rows in the transition table for every row of a composite SM
    template<
        typename T1
        , class Event
    >
    struct frow
    {
        typedef T1                  current_state_type;
        typedef T1                  next_state_type;
        typedef Event               event;
        typedef mpl::bool_<true>    internal_event;

        // Take the transition action and return the next state.
        static std::pair<int,HandledEnum> execute(Derived& fsm, int state, pstate_base* ,Event const& evt)
        {
	        execute_return res = 
                (static_cast<T1& > (*(fsm.m_state_list[state]))).process_event(evt);
            typedef typename create_stt<Derived>::type stt;
	        return std::make_pair(get_state_id<stt,T1>::type::value,res.get<0>());
        }
    };
public:
    // gets the transition table from a composite and make from it a forwarding row
    template <class Composite>
    struct get_transition_table_as_frow
    {
        // first get the table of a composite
        typedef typename recursive_get_transition_table<Composite>::type original_table;
        // and add for every event a forwarding row
        typedef typename generate_event_set<original_table>::type all_events;
        typedef typename mpl::fold<
            all_events, mpl::vector<>,
            mpl::push_back<mpl::placeholders::_1,
                           frow<Composite,mpl::placeholders::_2> > >::type type;
    };

    // extends the transition table with rows from composite states
    template <class Composite>
    struct extend_table
    {
        // add the init states
        typedef typename create_stt<Composite>::type stt;
		// get the state set
		typedef typename generate_state_set<stt>::type states;
        // for every state, add its transition table (if any)
        // transformed as frow
		typedef typename mpl::fold<states,stt,
				mpl::insert_range<mpl::placeholders::_1,mpl::end<mpl::placeholders::_1>,
								  get_transition_table_as_frow<mpl::placeholders::_2> > 
		>::type type;
    };
private:
    template <class Fsm, class History, class Copy,class Stt, class Event>
    friend struct dispatch_table;
    template <typename T1,class Event> friend struct frow;

    // data members
    std::vector<int>            m_states;
    events_queue_t              m_events_queue;
    deferred_events_queue_t     m_deferred_events_queue;
    HistoryPolicy               m_history;
    pstate_base_list            m_state_list;
    bool                        m_event_processing;
    bool                        m_is_included;
};

} } // boost::msm
#endif //STATEMACHINE_H
