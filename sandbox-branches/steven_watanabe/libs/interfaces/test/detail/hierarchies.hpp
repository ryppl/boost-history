// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED
#define BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces.hpp>
#include <boost/interfaces/detail/invoker.hpp>
#include "./counter.hpp"
#include "./method_tags.hpp"

namespace boost { namespace interfaces { namespace test {

//-------

//struct ifirst_interface_impl_ {
//	static const int start_line_idl_ = 114; template<typename Base_> struct inner : Base_ {
//		typedef typename ::boost::interfaces::advice_category< Base_ >::type category; public: friend class ::boost::interfaces::access; template<typename NNN_, typename Dummy_ = int> struct tracker_idl_ {
//			typedef void type;
//		}
//		; public: struct supers_idl_ : ::boost::mpl::identity< ::boost::mpl::vector<
//		::boost::mpl::na > > {
//		}
//		; public: template<typename Dummy_> struct tracker_idl_< ::boost::mpl::int_<115 - start_line_idl_>, Dummy_> {
//			typedef ::boost::interfaces::detail::fn_ptr fn_ptr; template<typename XXX_> struct function_holder {
//				static char function(void* xxx_ , int xxx_0 , float xxx_1) {
//					return static_cast<XXX_*>(xxx_)->one( xxx_0 , xxx_1);
//				}
//			}
//			; struct type {
//				template< typename DDD_, typename OOO_, typename FFF_, typename BBB_ > struct apply {
//					struct type : BBB_ {
//						struct interface_metadata {
//							typedef DDD_ derived_type; typedef OOO_ offset_type; typedef FFF_ flags_type; typedef BBB_ base_type; static const int size = 1; typedef ::boost::mpl::vector1< char( int xxx_0 , float xxx_1) > signatures; template<typename XXX_> struct implements {
//								template< char (XXX_::*MemFun)( int , float) > struct holder {
//								}
//								; typedef char (&yes) [1]; typedef char (&no) [2]; template<typename YYY_> static no tester(...); template<typename YYY_> static yes tester( holder<&YYY_::one >* = 0 ); static const bool value = sizeof(tester<XXX_>(0)) == sizeof(yes); typedef ::boost::mpl::bool_<value> type;
//							}
//							; struct proxy {
//							}
//							; template<typename XXX_> static void initialize(fn_ptr* table) {
//								table[OOO_::value] = reinterpret_cast<fn_ptr>(&function_holder<XXX_>::function);
//							}
//							::std::pair<const char*, fn_ptr> lookup(int, fn_ptr* table) {
//								return ::std::pair<const char*, fn_ptr> ("one", table[OOO_::value]);
//							}
//						};
//						using BBB_::one; 
//						; char one( int xxx_0 , float xxx_1) {
//                            typedef ::boost::interfaces::detail::invoker< type, OOO_, char( int xxx_0 , float xxx_1) > invoker; 
//                            return invoker::invoke( this , xxx_0 , xxx_1 );
//						}
//					}
//					;
//				}
//				;
//			}
//			;
//		}
//		; friend struct tracker_idl_< ::boost::mpl::int_<115 - start_line_idl_>, int>; struct dummy_idl_115; void one(dummy_idl_115*) {
//		}
//		struct dummy_idl_115 : ::boost::interfaces::detail::dummy< ::boost::interfaces::detail::dummy<
//		::boost::mpl::na > > {
//		}
//		; public: template<typename Dummy_> struct tracker_idl_< ::boost::mpl::int_<116 - start_line_idl_>, Dummy_> {
//			typedef ::boost::interfaces::detail::fn_ptr fn_ptr; template<typename XXX_> struct function_holder {
//				static void function(void* xxx_ , const char* xxx_0) {
//					return static_cast<XXX_*>(xxx_)->one( xxx_0);
//				}
//			}
//			; struct type {
//				template< typename DDD_, typename OOO_, typename FFF_, typename BBB_ > struct apply {
//					struct type : BBB_ {
//						struct interface_metadata {
//							typedef DDD_ derived_type; 
//                            typedef OOO_ offset_type; 
//                            typedef FFF_ flags_type; 
//                            typedef BBB_ base_type; 
//                            static const int size = 1; 
//                            typedef ::boost::mpl::vector1< void (*const) (const char* xxx_0) > signatures; 
//                            template<typename XXX_> 
//                            struct implements {
//								template< void (XXX_::*MemFun)( const char*) const > 
//                                struct holder { }; 
//                                typedef char (&yes) [1]; 
//                                typedef char (&no) [2]; 
//                                template<typename YYY_> 
//                                static no tester(...); 
//                                template<typename YYY_> 
//                                static yes tester( holder<&YYY_::one >* = 0 ); 
//                                static const bool value = sizeof(tester<XXX_>(0)) == sizeof(yes); 
//                                typedef ::boost::mpl::bool_<value> type;
//							}
//							; struct proxy {
//							}
//							; template<typename XXX_> static void initialize(fn_ptr* table) {
//								table[OOO_::value] = reinterpret_cast<fn_ptr>(&function_holder<XXX_>::function);
//							}
//							::std::pair<const char*, fn_ptr> lookup(int, fn_ptr* table) {
//								return ::std::pair<const char*, fn_ptr> ("one", table[OOO_::value]);
//							}
//						};
//						; using BBB_::one; 
//                        void one( const char* xxx_0) const {
//                            typedef ::boost::interfaces::detail::invoker< type, OOO_, void( const char* xxx_0) > invoker; 
//                            return invoker::invoke( this , xxx_0 );
//						}
//					}
//					;
//				}
//				;
//			}
//			;
//		}
//		; friend struct tracker_idl_< ::boost::mpl::int_<116 - start_line_idl_>, int>; struct dummy_idl_116; void one(dummy_idl_116*) {
//		}
//		struct dummy_idl_116 : ::boost::interfaces::detail::dummy< ::boost::interfaces::detail::dummy<
//		::boost::mpl::na > > {
//		}
//		; public: template<typename Dummy_> struct tracker_idl_< ::boost::mpl::int_<117 - start_line_idl_>, Dummy_> {
//			typedef ::boost::interfaces::detail::fn_ptr fn_ptr; template<typename XXX_> struct function_holder {
//				static void function(void* xxx_ ) {
//					return static_cast<XXX_*>(xxx_)->two();
//				}
//			}
//			; struct type {
//				template< typename DDD_, typename OOO_, typename FFF_, typename BBB_ > struct apply {
//					struct type : BBB_ {
//						struct interface_metadata {
//							typedef DDD_ derived_type; 
//                            typedef OOO_ offset_type; 
//                            typedef FFF_ flags_type; 
//                            typedef BBB_ base_type; 
//                            static const int size = 1; 
//                            typedef ::boost::mpl::vector1< void(* const) > signatures; 
//                            template<typename XXX_> struct implements {
//								template< void (XXX_::*MemFun)() const > struct holder {
//								}
//								; typedef char (&yes) [1]; typedef char (&no) [2]; template<typename YYY_> static no tester(...); template<typename YYY_> static yes tester( holder<&YYY_::two >* = 0 ); static const bool value = sizeof(tester<XXX_>(0)) == sizeof(yes); typedef ::boost::mpl::bool_<value> type;
//							}
//							; struct proxy {
//							}
//							; template<typename XXX_> static void initialize(fn_ptr* table) {
//								table[OOO_::value] = reinterpret_cast<fn_ptr>(&function_holder<XXX_>::function);
//							}
//							::std::pair<const char*, fn_ptr> lookup(int, fn_ptr* table) {
//								return ::std::pair<const char*, fn_ptr> ("two", table[OOO_::value]);
//							}
//						}
//						; using BBB_::two; 
//					    void two() const {
//                            typedef ::boost::interfaces::detail::invoker< type, OOO_, void() > invoker; 
//                            return invoker::invoke( this  );
//						}
//					}
//					;
//				}
//				;
//			}
//			;
//		}
//		; friend struct tracker_idl_< ::boost::mpl::int_<117 - start_line_idl_>, int>; struct dummy_idl_117; void two(dummy_idl_117*) {
//		}
//		struct dummy_idl_117 : ::boost::interfaces::detail::dummy< ::boost::interfaces::detail::dummy<
//		::boost::mpl::na > > {
//		}
//		; public: struct index_idl_ {
//			template<typename N> struct apply : tracker_idl_< ::boost::mpl::int_<N::value>, int > {
//			}
//			;
//		}
//		;
//	}
//	;
//}
//; class ifirst : public ::boost::interfaces::detail::interface_base< ifirst  , ifirst_interface_impl_  , 118 - ifirst_interface_impl_  ::start_line_idl_ >::type {
//	private: typedef ifirst_interface_impl_ implementation_type; typedef ::boost::interfaces::detail::interface_base< ifirst  , implementation_type, 118 - implementation_type::start_line_idl_ >::type base_idl_; friend class ::boost::interfaces::access; public: ifirst() : pv_idl_(0), table_idl_(0) {
//	}
//	template<typename XXX_> ifirst( XXX_& object_, typename ::boost::disable_if< ::boost::interfaces::is_interface<XXX_> >::type* = 0 ) : pv_idl_(&object_), table_idl_( ::boost::interfaces::detail::initialize_table<ifirst>(object_) ) {
//		typedef ::boost::static_assert_test< sizeof(::boost::STATIC_ASSERTION_FAILURE< (bool)( !::boost::is_const<XXX_>::value ) >)> boost_static_assert_typedef_4;
//	}
//	template<typename XXX_> ifirst( const XXX_& interface_, typename ::boost::enable_if< ::boost::interfaces::is_base_and_derived< XXX_, ::boost::interfaces::remove_qualifiers<ifirst>::type > >::type* = 0 ) {
//		::boost::interfaces::detail::fat_pointer ptr(interface_); pv_idl_ = ptr.pv; table_idl_ = ptr.table + ::boost::interfaces::detail::offset_of< XXX_, ::boost::interfaces::remove_qualifiers<ifirst>::type >::value;
//	}
//	ifirst& operator=(::boost::interfaces::detail::null_pointer_constant) {
//		this->reset(); return *this;
//	}
//	private: template<typename XXX_> ifirst& operator=(const XXX_* ptr); public: template<typename XXX_> typename ::boost::enable_if< ::boost::interfaces::is_base_and_derived< XXX_, ::boost::interfaces::remove_qualifiers<ifirst>::type >, ifirst& >::type operator=(const XXX_& interface_) {
//		::boost::interfaces::detail::fat_pointer ptr(interface_); pv_idl_ = ptr.pv; table_idl_ = ptr.table + ::boost::interfaces::detail::offset_of< XXX_, ::boost::interfaces::remove_qualifiers<ifirst>::type >::value; return *this;
//	}
//	void reset() {
//		pv_idl_ = 0; table_idl_ = 0;
//	}
//	operator const void*() const {
//		return table_idl_;
//	}
//	public: const void* pv_idl_; const ::boost::interfaces::detail::fn_ptr* table_idl_;
//}
//; template< typename XXX_  > ::boost::type_traits::yes_type is_interface_helper( ifirst  *, XXX_*, typename ::boost::enable_if< ::boost::is_same< XXX_, ifirst > >::type* = 0 );

//------

BOOST_IDL_BEGIN(ifirst)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(ifirst)

BOOST_IDL_BEGIN(ifirst_copy)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(ifirst_copy)

BOOST_IDL_BEGIN(isecond)
    BOOST_IDL_FN2(three, char, int, float)
    BOOST_IDL_CONST_FN1(three, void, const char*)
    BOOST_IDL_CONST_FN0(four, void)
BOOST_IDL_END(isecond)

BOOST_IDL_BEGIN(ithird)
    BOOST_IDL_FN2(five, char, int, float)
    BOOST_IDL_CONST_FN1(five, void, const char*)
    BOOST_IDL_CONST_FN0(six, void)
BOOST_IDL_END(ithird)

BOOST_IDL_BEGIN(ifourth)
    BOOST_IDL_FN2(seven, char, int, float)
    BOOST_IDL_CONST_FN1(seven, void, const char*)
    BOOST_IDL_CONST_FN0(eight, void)
BOOST_IDL_END(ifourth)
//
BOOST_IDL_BEGIN(iderived1)
    BOOST_IDL_EXTENDS(ifirst)
    BOOST_IDL_EXTENDS(isecond)
BOOST_IDL_END(iderived1)

BOOST_IDL_BEGIN(iderived2)
    BOOST_IDL_EXTENDS(ifirst)
    BOOST_IDL_EXTENDS(ifirst_copy)
BOOST_IDL_END(iderived2)

BOOST_IDL_BEGIN(iderived3)
    BOOST_IDL_EXTENDS(ithird)
    BOOST_IDL_EXTENDS(ifourth)
BOOST_IDL_END(iderived3)

BOOST_IDL_BEGIN(iderived4)
    BOOST_IDL_EXTENDS(iderived1)
    BOOST_IDL_EXTENDS(iderived3)
BOOST_IDL_END(iderived4)

template<int N>
struct cfirst : counted< cfirst<N> > {
    cfirst() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cfirst<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cfirst<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cfirst<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct csecond : counted< csecond<N> > {
    csecond() { } // Needed for const interface test.
    char three(int i, float f)
    { 
        counter< method<csecond<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<csecond<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<csecond<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cthird : counted< cthird<N> > {
    cthird() { } // Needed for const interface test.
    char five(int i, float f)
    { 
        counter< method<cthird<N>, func_five_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void five(const char* h) const 
    { 
        counter< method<cthird<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six() const 
    { 
        counter< method<cthird<N>, func_six_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cfourth : counted< cfourth<N> > {
    cfourth() { } // Needed for const interface test.
    char seven(int i, float f)
    { 
        counter< method<cfourth<N>, func_seven_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void seven(const char* h) const 
    { 
        counter< method<cfourth<N>, func_seven_tag, sig_b_tag> >::inc();
    }
    void eight() const 
    { 
        counter< method<cfourth<N>, func_eight_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived1 : counted< cderived1<N> > { 
    cderived1() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived1<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived1<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived1<N>, func_two_tag, sig_c_tag> >::inc();
    }
    char three(int i, float f)
    { 
        counter< method<cderived1<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<cderived1<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<cderived1<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived2 : counted< cderived2<N> > { 
    cderived2() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived2<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived2<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived2<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived4 : counted< cderived4<N> > {
    cderived4() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived4<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived4<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived4<N>, func_two_tag, sig_c_tag> >::inc();
    }
    char three(int i, float f)
    { 
        counter< method<cderived4<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<cderived4<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<cderived4<N>, func_four_tag, sig_c_tag> >::inc();
    }
    char five(int i, float f)
    { 
        counter< method<cderived4<N>, func_five_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void five(const char* h) const 
    { 
        counter< method<cderived4<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six() const 
    { 
        counter< method<cderived4<N>, func_six_tag, sig_c_tag> >::inc();
    }
    char seven(int i, float f)
    { 
        counter< method<cderived4<N>, func_seven_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void seven(const char* h) const 
    { 
        counter< method<cderived4<N>, func_seven_tag, sig_b_tag> >::inc();
    }
    void eight() const 
    { 
        counter< method<cderived4<N>, func_eight_tag, sig_c_tag> >::inc();
    }
};

} } } // End namespaces test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED
