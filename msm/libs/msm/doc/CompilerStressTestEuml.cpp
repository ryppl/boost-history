#include <vector>
#include <list>
#include <set>
#include <map>
#include <iostream>

#define FUSION_MAX_VECTOR_SIZE 15

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;

// how long the timer will ring when countdown elapsed.
#define RINGING_TIME 5

#define m_src_container 0
#define m_tgt_container 1
#define m_test_container 2
#define m_song 0
#define m_letters 0
#define m_src_it 0

namespace  // Concrete FSM implementation
{
    // flag
    struct SomeFlag : euml_flag<SomeFlag>{};

    // events
    typedef BOOST_TYPEOF(build_attributes(attributes_ << string() << int()/*m_song*/ )) OneSongDef;
    struct OneSong : euml_event<OneSong>,OneSongDef
    {
        OneSong(){}
        OneSong(const string& asong)
        {
            get_attribute<m_song>()=asong;
            get_attribute<1>()=1;
        }
        OneSong(const OneSong& asong)
        {
            get_attribute<m_song>()=asong.get_attribute<m_song>();
            get_attribute<1>()=1;
        }
        const string& get_data() const {return get_attribute<m_song>();}
    };
    struct SongComparator : euml_action<SongComparator>
    {
        bool operator()(const OneSong& lhs,const OneSong& rhs)const
        {
            return lhs.get_data() == rhs.get_data();
        }
    };
    struct SongLessComparator : euml_action<SongLessComparator>
    {
        bool operator()(const OneSong& lhs,const OneSong& rhs)const
        {
            return lhs.get_data() < rhs.get_data();
        }
    };
    struct Comparator
    {
        template <class T>
        bool operator()(const T& lhs,const T& rhs)const
        {
            return lhs < rhs;
        }
    };
    struct RemoveDummy 
    {
        bool operator()(const OneSong& lhs)const
        {
            return (lhs.get_attribute<m_song>().compare(std::string("She-Dummy. Remove this one"))==0 );
        }
    };
    template <int val>
    struct LookFor
    {
        template <class T>
        bool operator()(const T& lhs)const
        {
            return lhs == val;
        }
    };
    template <int val>
    struct LessThan 
    {
        template <class T>
        bool operator()(const T& lhs)const
        {
            return lhs < val;
        }
    };
    struct SongDeleter : euml_action<SongDeleter>
    {
        bool operator()(const OneSong& lhs)const
        {
            return lhs.get_data() == "Twist and Shout";
        }
    };
    struct Generator
    {
        int operator()()const
        {
            return 1;
        }
    };
    struct Print
    {
        template <class T>
        void operator()(const T& lhs)const
        {
            std::cout << "Song:" << lhs.get_data() << endl;
        }
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
        int foo()const {std::cout << "foo()" << std::endl; return 0;}
        int foo(int i)const {std::cout << "foo(int):" << i << std::endl; return 1;}
        int foo(int i,int j)const {std::cout << "foo(int,int):" << i <<"," << j << std::endl; return 2;}

    };
    // some functions to call
    MSM_EUML_METHOD(FoundFoo_ , foo , foo_ , int , int )

    template <class T>
    int do_print(T& t ) {std::cout << "print(T):" << typeid(T).name() << std::endl;return 1;}
    MSM_EUML_FUNCTION(PrintState_ , do_print , print_ , int , int )

    struct Done : euml_event<Done> {};


    // Concrete FSM implementation 
    struct some_base 
    {
        int foobar()const {std::cout << "foobar()" << std::endl; return 0;}
        int foobar(int i)const {std::cout << "foobar(int):" << i << std::endl; return 1;}
        int foobar(int i,int j)const {std::cout << "foobar(int,int):" << i <<"," << j << std::endl; return 2;}
    };
    // some functions to call
    MSM_EUML_METHOD(FooBar_ , foobar , foobar_ , int , int )

    // The list of FSM states
    typedef BOOST_TYPEOF(build_state( ( insert_(Fsm_<m_tgt_container>(),end_(Fsm_<m_tgt_container>()),
                                                append_(Event_<m_song>(),Fsm_<7>()) ),//foo_(event_(),Int_<0>()) ,
                                                //foo_(event_(),Int_<0>(),Int_<1>()),print_(state_()),
                                                process_(Done()/*,fsm_()*/),if_then_(True_(),True_()) ),
                                       NoAction()
                                        )) Insert;


    typedef BOOST_TYPEOF(build_state( if_then_else_( (string_find_(Event_<m_song>(),State_<m_letters>(),Size_t_<0>()) != Npos_<string>()&&
                                                      string_find_(Event_<m_song>(),Char_<'S'>(),Size_t_<0>()) != Npos_<string>()&&
                                                      string_find_first_of_(Event_<m_song>(),Char_<'S'>()) == Size_t_<0>() &&
                                                      string_compare_(Event_<m_song>(),Int_<0>(),size_(Event_<m_song>()),Event_<m_song>()) == Int_<0>()
                                                      //&& is_flag_(SomeFlag(),fsm_())
                                                      //&& ( attribute_(event_(),Int_<1>()) == Int_<1>())
                                                      //&& string_find_(Event_<m_song>(),String_<mpl::string<'Sh','e'> >())
                                                      //      != Npos_<string>()

                                                      ),
                                                     process2_(Found(),
                                                        //string_insert_(Event_<m_song>(),Size_t_<0>(),Fsm_<6>()) ),
                                                        string_replace_(
                                                            string_assign_(
                                                                string_erase_(
                                                                    string_insert_(
                                                                        substr_(Event_<m_song>(),Size_t_<1>()),
                                                                        Size_t_<0>(),
                                                                        Size_t_<1>(),
                                                                        Char_<'S'>()),
                                                                    Size_t_<0>(),
                                                                    Size_t_<1>() ), 
                                                                 Event_<m_song>() ), 
                                                            Size_t_<0>(),
                                                            Size_t_<1>(),
                                                            c_str_(Fsm_<6>())
                                                            /*Size_t_<1>(),
                                                            Char_<'s'>()*/ ) ),
                                                     process2_(NotFound(),Event_<m_song>(),fsm_()) ) ,
                                     NoAction(), 
                                     attributes_ << string()/*m_letters*/,
                                     configure_<< SomeFlag() )) 
                         StringFind;

    typedef BOOST_TYPEOF(build_state( if_then_( (State_<m_src_it>() != end_(Fsm_<m_src_container>()) &&
                                                //associative_find_(Fsm_<4>(),Int_<9>()) != end_(Fsm_<4>())&&
                                                //associative_count_(Fsm_<4>(),Int_<9>()) == Size_t_<1>() &&
                                                //*associative_upper_bound_(Fsm_<4>(),Int_<8>()) == Int_<9>()&&
                                                //*associative_lower_bound_(Fsm_<4>(),Int_<9>()) == Int_<9>() &&
                                                //second_(associative_equal_range_(Fsm_<4>(),Int_<8>())) == associative_upper_bound_(Fsm_<4>(),Int_<8>()) &&
                                                //first_(associative_equal_range_(Fsm_<4>(),Int_<8>())) == associative_lower_bound_(Fsm_<4>(),Int_<8>())&&
                                                //second_(*associative_lower_bound_(Fsm_<5>(),Int_<0>())) == Int_<0>() && //map => pair as return
                                                //find_if_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Predicate_<LookFor<8> >()) != end_(Fsm_<4>())&&
                                                //*lower_bound_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>(),Predicate_<std::less<int> >()) == Int_<9>()&&
                                                //*upper_bound_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<8>(),Predicate_<std::less<int> >()) == Int_<9>() &&
                                                //second_(equal_range_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<8>())) 
                                                 //   == upper_bound_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<8>()) &&
                                                //first_(equal_range_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>(),Predicate_<std::less<int> >())) 
                                                //    == lower_bound_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>(),Predicate_<std::less<int> >())&&
                                                //binary_search_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>(),Predicate_<std::less<int> >())&&
                                                //binary_search_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>())&&
                                                //count_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Int_<9>()) == Int_<1>()&&
                                                //count_if_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Predicate_<LookFor<9> >()) == Int_<1>()&&
                                                //distance_(begin_(Fsm_<4>()),end_(Fsm_<4>())) == Int_<2>()&&
                                                //*min_element_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Predicate_<std::less<int> >()) == Int_<8>()&&
                                                //*max_element_(begin_(Fsm_<4>()),end_(Fsm_<4>()),Predicate_<std::less<int> >()) == Int_<9>()&&
                                                //adjacent_find_(begin_(Fsm_<4>()),end_(Fsm_<4>())) == end_(Fsm_<4>())&&
                                                //*find_end_(begin_(Fsm_<8>()),end_(Fsm_<8>()),begin_(Fsm_<9>()),end_(Fsm_<9>())) 
                                                //    == Int_<1>()&&
                                                //*find_first_of_(begin_(Fsm_<8>()),end_(Fsm_<8>()),begin_(Fsm_<9>()),end_(Fsm_<9>())) 
                                                //    == Int_<1>()&&
                                                //equal_(begin_(Fsm_<9>()),end_(Fsm_<9>()),begin_(Fsm_<8>()))&&
                                                //*search_(begin_(Fsm_<8>()),end_(Fsm_<8>()),begin_(Fsm_<9>()),end_(Fsm_<9>())) 
                                                //    == Int_<1>()&&
                                                //includes_(begin_(Fsm_<8>()),end_(Fsm_<8>()),begin_(Fsm_<9>()),end_(Fsm_<9>()))&&
                                                //!lexicographical_compare_(begin_(Fsm_<8>()),end_(Fsm_<8>()),
                                                //                          begin_(Fsm_<9>()),end_(Fsm_<9>()))&&
                                                //first_(mismatch_(begin_(Fsm_<9>()),end_(Fsm_<9>()),begin_(Fsm_<8>())))
                                                //    == end_(Fsm_<9>()) &&
                                                accumulate_(begin_(Fsm_<9>()),end_(Fsm_<9>()),Int_<1>(),
                                                            Predicate_<std::plus<int> >()) == Int_<1>()
                                                ),
                                                (process2_(OneSong(),*(State_<m_src_it>()++))/*,foobar_(fsm_(),Int_<0>())*/ ) ),
                                      NoAction(), 
                                      attributes_ << vector<OneSong>::iterator()/*m_src_it*/
                                      , configure_<< SomeFlag() )) Foreach;


    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt
        ((Foreach()     + OneSong()     == StringFind() [if_then_else_(True_(),True_(),True_())],
          StringFind()  + Found()       == Insert()  / (if_then_(True_(),NoAction())),
          StringFind()  + NotFound()    == Foreach() ,
          Insert()      + Done()        == Foreach()
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
                                    //insert_(State_<4>(),begin_(State_<2>()),end_(State_<2>())),
                                    (insert_(State_<4>(),Int_<5>()),insert_(State_<4>(),Int_<6>()),insert_(State_<4>(),Int_<7>()),
                                     insert_(State_<4>(),Int_<8>()),insert_(State_<4>(),Int_<9>()),
                                    associative_erase_(State_<4>(),Int_<6>()),associative_erase_(State_<4>(),begin_(State_<4>())),
                                    associative_erase_(State_<4>(),begin_(State_<4>()),++begin_(State_<4>())),
                                    insert_(State_<2>(),begin_(State_<2>()),begin_(State_<3>()),end_(State_<3>())),
                                    State_<5>()[Int_<0>()]=Int_<0>(),State_<5>()[Int_<1>()]=Int_<1>()
                                    ,attribute_(substate_(Foreach(),fsm_()),Int_<m_src_it>())
                                        = begin_(attribute_(fsm_(),Int_<m_src_container>()))
                                    //,fill_(begin_(State_<9>()),end_(State_<9>()),Int_<0>())
                                    //,fill_n_(begin_(State_<9>()),Size_t_<2>(),Int_<0>())
                                    //,transform_(begin_(State_<4>()),end_(State_<4>()),begin_(State_<2>()),begin_(State_<4>()),
                                    //            Predicate_<std::plus<int> >())
                                    //,process_(Done(),fsm_(),fsm_())
                                    //,process_(Done(),fsm_())
                                    //,fsm_()
                                    //,foobar_(state_(),Int_<0>(),Int_<1>())
                                    //,nth_element_(begin_(State_<9>()),++begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,partial_sort_(begin_(State_<9>()),end_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,partial_sort_copy_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,list_sort_(State_<2>())
                                    //,sort_(begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,inner_product_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()),Int_<1>())
                                    //,replace_copy_(begin_(State_<4>()),end_(State_<4>()),begin_(State_<4>()),Int_<8>(),Int_<7>())
                                    //,replace_copy_if_(begin_(State_<4>()),end_(State_<4>()),begin_(State_<4>()),Predicate_<LookFor<9> >(),Int_<8>())
                                    //,replace_(begin_(State_<4>()),end_(State_<4>()),Int_<8>(),Int_<7>())
                                    //,replace_if_(begin_(State_<4>()),end_(State_<4>()),Predicate_<LookFor<9> >(),Int_<8>())
                                    //,adjacent_difference_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()))
                                    //,partial_sum_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()))
                                    //,inner_product_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()),Int_<1>())
                                    //,next_permutation_(begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,prev_permutation_(begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,set_union_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()))
                                    //,inplace_merge_(begin_(State_<9>()),end_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,merge_(begin_(State_<9>()),end_(State_<9>()),begin_(State_<9>()),end_(State_<9>())
                                    //        ,begin_(State_<9>()),Predicate_<std::less<int> >())
                                    //,stable_sort_(begin_(State_<9>()),end_(State_<9>()),Predicate_<std::less<int> >())
                                    //,partition_(begin_(State_<2>()),end_(State_<2>()),Predicate_<LessThan<3> >())
                                    //,stable_partition_(begin_(State_<2>()),end_(State_<2>()),Predicate_<LessThan<3> >())
                                    //,rotate_copy_(begin_(State_<2>()),++begin_(State_<2>()),end_(State_<2>()),begin_(State_<2>()))
                                    //,rotate_(begin_(State_<2>()),++begin_(State_<2>()),end_(State_<2>()))
                                    //,unique_(begin_(State_<2>()),end_(State_<2>()))
                                    //,unique_copy_(begin_(State_<2>()),end_(State_<2>()),begin_(State_<2>()))
                                    //,random_shuffle_(begin_(State_<9>()),end_(State_<9>()))
                                    //,generate_(begin_(State_<9>()),end_(State_<9>()),Predicate_<Generator>())
                                    //,generate_n_(begin_(State_<9>()),Int_<2>(),Predicate_<Generator>())
                                    //,reverse_copy_(begin_(State_<2>()),end_(State_<2>()),begin_(State_<2>()))
                                    //erase_(State_<m_src_container>(),
                                    //       remove_if_(begin_(State_<m_src_container>()),end_(State_<m_src_container>()),
                                    //                  Predicate_<RemoveDummy>()),
                                    //       end_(State_<m_src_container>())),
                                    //list_remove_(State_<2>(),Int_<3>()),
                                    //remove_copy_if_(begin_(State_<9>()),end_(State_<9>()),back_inserter_(State_<2>()), 
                                    //                Predicate_<LookFor<2> >() )
                                    //for_each_(begin_(State_<m_src_container>()),end_(State_<m_src_container>()),
                                    //          Predicate_<Print>() ),
                                    //copy_(begin_(State_<9>()),end_(State_<9>()),inserter_(State_<2>(),end_(State_<2>()))),
                                    //reverse_(begin_(State_<2>()),end_(State_<2>()))
                                    ),
                                    //NoAction(), // Entry
                                    //splice_(State_<2>(),begin_(State_<2>()),State_<3>(),begin_(State_<3>()),end_(State_<3>())),
                                    //(list_remove_(State_<2>(),Int_<3>()),list_merge_(State_<2>(),State_<3>(),Comparator())),//NoAction(), // Entry
                                    NoAction(), // Exit
                                    attributes_ << vector<OneSong>() /*m_src_container*/ // song list
                                                << list<OneSong>() /*m_tgt_container*/ // result
                                                << list<int>()
                                                << list<int>()
                                                << std::set<int>()
                                                << std::map<int,int>()
                                                << std::string()
                                                << std::string()
                                                << std::vector<int>()
                                                << std::vector<int>()
                                                << int(),
                                    configure_<< no_configure_,
                                    Log_No_Transition()
                                    )) iPodSearch_helper;
    struct iPodSearch_ : public iPodSearch_helper, public some_base 
    {
    };


    // choice of back-end
    typedef msm::back::state_machine<iPodSearch_> iPodSearch;

    void test()
    {        
        iPodSearch search;
        // fill our song list
        //search.get_attribute<m_src_container>().push_back(OneSong("She-Dummy. Remove this one"));
        search.get_attribute<m_src_container>().push_back(OneSong("Let it be"));
        search.get_attribute<m_src_container>().push_back(OneSong("Yellow submarine"));
        search.get_attribute<m_src_container>().push_back(OneSong("Twist and Shout"));
        search.get_attribute<m_src_container>().push_back(OneSong("She Loves You"));

        search.get_attribute<2>().push_back(1);
        search.get_attribute<2>().push_back(3);
        search.get_attribute<2>().push_back(4);

        search.get_attribute<3>().push_back(2);
        search.get_attribute<3>().push_back(4);

        search.get_attribute<6>() = "S";
        search.get_attribute<7>() = "- Some text";

        search.get_attribute<8>().push_back(1);
        search.get_attribute<8>().push_back(2);
        search.get_attribute<8>().push_back(3);
        search.get_attribute<8>().push_back(4);

        search.get_attribute<9>().push_back(1);
        search.get_attribute<9>().push_back(2);


        // look for "She Loves You" using the first letters
        search.get_state<StringFind&>().get_attribute<m_letters>()="Sh";
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        search.start(); 
        // display all the songs
        for (list<OneSong>::const_iterator it = search.get_attribute<m_tgt_container>().begin(); 
             it != search.get_attribute<m_tgt_container>().end();++it)
        {
            cout << "candidate song:" << (*it).get_data() << endl;
        }
        for (list<int>::const_iterator iti = search.get_attribute<2>().begin(); 
            iti != search.get_attribute<2>().end();++iti)
        {
            cout << "int in attribute<2>:" << (*iti) << endl;
        }
        for (set<int>::const_iterator its = search.get_attribute<4>().begin(); 
            its != search.get_attribute<4>().end();++its)
        {
            cout << "int in attribute<4>:" << (*its) << endl;
        }
        cout << "search using more letters" << endl;
        // look for "She Loves You" using more letters
        search.get_state<StringFind&>().get_attribute<m_letters>()="She";
        search.get_attribute<m_tgt_container>().clear();
        search.start(); 
        // display all the songs
        for (list<OneSong>::const_iterator it = search.get_attribute<m_tgt_container>().begin(); 
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
