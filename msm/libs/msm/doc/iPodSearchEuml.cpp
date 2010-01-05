// same as iPodSearch.cpp but using eUML
// requires boost >= v1.40 because using mpl::string

#include <vector>
#include <iostream>

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;
namespace mpl = boost::mpl;

// how long the timer will ring when countdown elapsed.
#define RINGING_TIME 5

#define m_src_container 0
#define m_tgt_container 1
#define m_song 0
#define m_letters 0
#define m_src_it 0

namespace  // Concrete FSM implementation
{
    // events
    typedef BOOST_TYPEOF(build_attributes(attributes_ << string()/*m_song*/ )) OneSongDef;
    struct OneSong : euml_event<OneSong>,OneSongDef
    {
        OneSong(){}
        OneSong(const string& asong)
        {
            get_attribute<m_song>()=asong;
        }
        OneSong(const char* asong)
        {
            get_attribute<m_song>()=asong;
        }
        OneSong(const OneSong& asong)
        {
            get_attribute<m_song>()=asong.get_attribute<m_song>();
        }
        const string& get_data() const {return get_attribute<m_song>();}
    };

    typedef BOOST_TYPEOF(build_attributes(attributes_ << string()/*m_song*/ )) NotFoundDef;
    struct NotFound : euml_event<NotFound>,NotFoundDef
    {
        NotFound(){}
        NotFound (const string& data)
        {
            get_attribute<m_song>()=data;
        }
    };

    typedef BOOST_TYPEOF(build_attributes(attributes_ << string()/*m_song*/ )) FoundDef;
    struct Found : euml_event<Found>,FoundDef
    {
        Found(){}
        Found (const string& data)
        {
            get_attribute<m_song>()=data;
        }
    };
    struct Done : euml_event<Done> {};


    // Concrete FSM implementation 

    // The list of FSM states
    typedef BOOST_TYPEOF(build_state( (push_back_(Fsm_<m_tgt_container>(),Event_<m_song>()) ,process_(Done())),
                                       NoAction() )) Insert;


    typedef BOOST_TYPEOF(build_state( if_then_else_( string_find_(Event_<m_song>(),State_<m_letters>()) != Npos_<string>() ,
                                                     process2_(Found(),Event_<m_song>()),
                                                     process2_(NotFound(),Event_<m_song>()) ) ,
                                     NoAction(), 
                                     attributes_ << string()/*m_letters*/ )) StringFind;

    typedef BOOST_TYPEOF(build_state( if_then_( State_<m_src_it>() != end_(Fsm_<m_src_container>()),
                                                process2_(OneSong(),*(State_<m_src_it>()++)) ),
                                      NoAction(), 
                                      attributes_ << vector<OneSong>::iterator()/*m_src_it*/ )) Foreach;


    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt((
          StringFind()  == Foreach()     + OneSong() ,
          Insert()      == StringFind()  + Found()  ,
          Foreach()     == StringFind()  + NotFound() ,
          Foreach()     ==  Insert()     + Done()
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;
    struct Log_No_Transition : euml_action<Log_No_Transition>
    {
        template <class FSM,class Event>
        void operator()(Event const& e,FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    // create a state machine "on the fly"
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << Foreach(), // Init
                                    (
                                    clear_(attribute_(fsm_(),Int_<m_src_container>())), //clear source
                                    clear_(attribute_(fsm_(),Int_<m_tgt_container>())), //clear results
                                    push_back_(attribute_(fsm_(),Int_<m_src_container>()), 
                                               String_<mpl::string<'Let ','it ','be'> >()),//add a song
                                    push_back_(attribute_(fsm_(),Int_<m_src_container>()),
                                               String_<mpl::string<'Yell','ow s','ubma','rine'> >()),//add a song
                                    push_back_(attribute_(fsm_(),Int_<m_src_container>()),
                                               String_<mpl::string<'Twis','t an','d Sh','out'> >()),//add a song
                                    push_back_(attribute_(fsm_(),Int_<m_src_container>()),
                                               String_<mpl::string<'She ','love','s yo','u'> >()),//add a song
                                    attribute_(substate_(Foreach()),Int_<m_src_it>())
                                        = begin_(attribute_(fsm_(),Int_<m_src_container>())) //set the search begin
                                    ), // Entry
                                    NoAction(), // Exit
                                    attributes_ << vector<OneSong>() /*m_src_container*/ // song list
                                                << vector<OneSong>() /*m_tgt_container*/, // result
                                    configure_<< no_configure_,
                                    Log_No_Transition()
                                    )) iPodSearch_;


    // choice of back-end
    typedef msm::back::state_machine<iPodSearch_> iPodSearch;

    void test()
    {        
        iPodSearch search;

        // look for "She Loves You" using the first letters
        search.get_state<StringFind&>().get_attribute<m_letters>()="Sh";// will find 2 songs

        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        search.start(); 
        // display all the songs
        for (vector<OneSong>::const_iterator it = search.get_attribute<m_tgt_container>().begin(); 
             it != search.get_attribute<m_tgt_container>().end();++it)
        {
            cout << "candidate song:" << (*it).get_data() << endl;
        }

        cout << "search using more letters" << endl;
        // look for "She Loves You" using more letters
        search.get_state<StringFind&>().get_attribute<m_letters>()="She";// will find 1 song
        search.start(); 
        // display all the songs
        for (vector<OneSong>::const_iterator it = search.get_attribute<m_tgt_container>().begin(); 
             it != search.get_attribute<m_tgt_container>().end();++it)
        {
            cout << "candidate song:" << (*it).get_data() << endl;
        }
    }
}

int main()
{
    test();
    return 0;
}
