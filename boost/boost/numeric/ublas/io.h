//  
//  Copyright (c) 2000-2001
//  Joerg Walter, Mathias Koch
//  
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.  
//  It is provided "as is" without express or implied warranty.
//  
//  The authors gratefully acknowledge the support of 
//	GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_IO_H
#define NUMERICS_IO_H

#include <iostream>

namespace numerics {

#ifdef USE_MSVC

    template<class E, class T, class VE> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os, 
                                           const vector_expression<VE> &v) {
        std::size_t size = v ().size ();
        std::basic_ostringstream<E, T, std::allocator<E> > s;
	    s.flags (os.flags ());
	    s.imbue (os.getloc ());
	    s.precision (os.precision ());
        s << '[' << size << "](";
        if (size > 0)
            s << v () (0);
        for (std::size_t i = 1; i < size; ++ i) 
            s << ',' << v () (i);
        s << ')';
	    return os << s.str ().c_str (); 
    }

    template<class E, class T, class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os, 
                                           const vector<VT, VA, VF> &v) {
        std::size_t size = v.size ();
        std::basic_ostringstream<E, T, std::allocator<E> > s;
	    s.flags (os.flags ());
	    s.imbue (os.getloc ());
	    s.precision (os.precision ());
        s << '[' << size << "](";
        if (size > 0)
            s << v (0);
        for (std::size_t i = 1; i < size; ++ i) 
            s << ',' << v (i);
        s << ')';
	    return os << s.str ().c_str (); 
    }

    template<class E, class T, class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_istream<E, T> &operator >> (std::basic_istream<E, T> &is, 
										   vector<VT, VA, VF> &v) {
	    E ch;
        std::size_t size;
        if (is >> ch && ch != '[') {
            is.putback (ch);
            is.setstate (std::ios_base::failbit);
        } else if (is >> size >> ch && ch != ']') {
            is.putback (ch);
            is.setstate (std::ios_base::failbit);
        } else {
            vector<VT, VA, VF> s (size);
            if (is >> ch && ch != '(') {
                is.putback (ch);
                is.setstate (std::ios_base::failbit);
            } else {
                for (std::size_t i = 0; i < size; i ++) {
                    if (is >> s (i) >> ch && ch != ',') {
                        is.putback (ch);
                        if (i < size - 1)
                            is.setstate (std::ios_base::failbit);
                        break;
                    }
                }
                if (is >> ch && ch != ')') {
                    is.putback (ch);
                    is.setstate (std::ios_base::failbit);
                }
            }
            if (! is.fail ()) {
                v.resize (size);
                v = s;
            }
        }
	    return is; 
    }

    template<class E, class T, class ME> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os, 
                                           const matrix_expression<ME> &m) {
        std::size_t size1 = m ().size1 ();
        std::size_t size2 = m ().size2 ();
        std::basic_ostringstream<E, T, std::allocator<E> > s;
	    s.flags (os.flags ());
	    s.imbue (os.getloc ());
	    s.precision (os.precision ());
        s << '[' << size1 << ',' << size2 << "](";
        if (size1 > 0) {
            s << '(' ;
            if (size2 > 0) 
                s << m () (0, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                s << ',' << m () (0, j);
            s << ')';
        }
        for (std::size_t i = 1; i < size1; ++ i) {
            s << ",(" ;
            if (size2 > 0) 
                s << m () (i, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                s << ',' << m () (i, j);
            s << ')';
        }
        s << ')';
	    return os << s.str ().c_str (); 
    }

    template<class E, class T, class MT, class MA, class MF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_ostream<E, T> &operator << (std::basic_ostream<E, T> &os, 
                                           const matrix<MT, MA, MF> &m) {
        std::size_t size1 = m.size1 ();
        std::size_t size2 = m.size2 ();
        std::basic_ostringstream<E, T, std::allocator<E> > s;
	    s.flags (os.flags ());
	    s.imbue (os.getloc ());
	    s.precision (os.precision ());
        s << '[' << size1 << ',' << size2 << "](";
        if (size1 > 0) {
            s << '(' ;
            if (size2 > 0) 
                s << m (0, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                s << ',' << m (0, j);
            s << ')';
        }
        for (std::size_t i = 1; i < size1; ++ i) {
            s << ",(" ;
            if (size2 > 0) 
                s << m (i, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                s << ',' << m (i, j);
            s << ')';
        }
        s << ')';
	    return os << s.str ().c_str (); 
    }

    template<class E, class T, class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::basic_istream<E, T> &operator >> (std::basic_istream<E, T> &is, 
                                           matrix<VT, VA, VF> &m) {
	    E ch;
        std::size_t size1, size2;
        if (is >> ch && ch != '[') {
            is.putback (ch);
            is.setstate (std::ios_base::failbit);
        } else if (is >> size1 >> ch && ch != ',') {
            is.putback (ch);
            is.setstate (std::ios_base::failbit);
        } else if (is >> size2 >> ch && ch != ']') {
            is.putback (ch);
            is.setstate (std::ios_base::failbit);
        } else {
            matrix<VT, VA, VF> s (size1, size2);
            if (is >> ch && ch != '(') {
                is.putback (ch);
                is.setstate (std::ios_base::failbit);
            } else {
                for (std::size_t i = 0; i < size1; i ++) {
                    if (is >> ch && ch != '(') {
                        is.putback (ch);
                        is.setstate (std::ios_base::failbit);
                        break;
                    }
                    for (std::size_t j = 0; j < size2; j ++) {
                        if (is >> s (i, j) >> ch && ch != ',') {
                            is.putback (ch);
                            if (j < size2 - 1) {
                                is.setstate (std::ios_base::failbit);
                                break;
                            }
                        }
                    }
                    if (is >> ch && ch != ')') {
                        is.putback (ch);
                        is.setstate (std::ios_base::failbit);
                        break;
                    } 
                    if (is >> ch && ch != ',') {
                       is.putback (ch);
                       if (i < size1 - 1) {
                            is.setstate (std::ios_base::failbit);
                            break;
                       }
                    }
                }
                if (is >> ch && ch != ')') {
                    is.putback (ch);
                    is.setstate (std::ios_base::failbit);
                } 
            }
            if (! is.fail ()) {
                m.resize (size1, size2);
                m = s;
            }
        }
	    return is; 
    }

#endif 

#if defined (USE_GCC) || defined (USE_BCC) || defined (USE_ICC)

    template<class VE> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::ostream &operator << (std::ostream &os, 
                               const vector_expression<VE> &v) {
        std::size_t size = v ().size ();
        os << '[' << size << "](";
        if (size > 0)
            os << v () (0);
        for (std::size_t i = 1; i < size; ++ i) 
            os << ',' << v () (i);
        os << ')';
	    return os; 
    }

    template<class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::ostream &operator << (std::ostream &os, 
                               const vector<VT, VA, VF> &v) {
        std::size_t size = v.size ();
        os << '[' << size << "](";
        if (size > 0)
            os << v (0);
        for (std::size_t i = 1; i < size; ++ i) 
            os << ',' << v (i);
        os << ')';
	    return os; 
    }

    template<class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::istream &operator >> (std::istream &is, 
							   vector<VT, VA, VF> &v) {
	    char ch;
        std::size_t size;
        if (is >> ch && ch != '[') {
            is.putback (ch);
            is.setstate (std::ios::failbit);
        } else if (is >> size >> ch && ch != ']') {
            is.putback (ch);
            is.setstate (std::ios::failbit);
        } else {
            vector<VT, VA, VF> s (size);
            if (is >> ch && ch != '(') {
                is.putback (ch);
                is.setstate (std::ios::failbit);
            } else {
                for (std::size_t i = 0; i < size; i ++) {
                    if (is >> s (i) >> ch && ch != ',') {
                        is.putback (ch);
                        if (i < size - 1)
                            is.setstate (std::ios::failbit);
                        break;
                    }
                }
                if (is >> ch && ch != ')') {
                    is.putback (ch);
                    is.setstate (std::ios::failbit);
                }
            }
            if (! is.fail ()) {
                v.resize (size);
                v = s;
            }
        }
	    return is; 
    }

    template<class ME> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::ostream &operator << (std::ostream &os, 
                               const matrix_expression<ME> &m) {
        std::size_t size1 = m ().size1 ();
        std::size_t size2 = m ().size2 ();
        os << '[' << size1 << ',' << size2 << "](";
        if (size1 > 0) {
            os << '(' ;
            if (size2 > 0) 
                os << m () (0, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                os << ',' << m () (0, j);
            os << ')';
        }
        for (std::size_t i = 1; i < size1; ++ i) {
            os << ",(" ;
            if (size2 > 0) 
                os << m () (i, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                os << ',' << m () (i, j);
            os << ')';
        }
        os << ')';
	    return os; 
    }

    template<class MT, class MA, class MF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::ostream &operator << (std::ostream &os, 
                               const matrix<MT, MA, MF> &m) {
        std::size_t size1 = m.size1 ();
        std::size_t size2 = m.size2 ();
        os << '[' << size1 << ',' << size2 << "](";
        if (size1 > 0) {
            os << '(' ;
            if (size2 > 0) 
                os << m (0, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                os << ',' << m (0, j);
            os << ')';
        }
        for (std::size_t i = 1; i < size1; ++ i) {
            os << ",(" ;
            if (size2 > 0) 
                os << m (i, 0);
            for (std::size_t j = 1; j < size2; ++ j) 
                os << ',' << m (i, j);
            os << ')';
        }
        os << ')';
	    return os; 
    }

    template<class VT, class VA, class VF> 
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    std::istream &operator >> (std::istream &is, 
                               matrix<VT, VA, VF> &m) {
	    char ch;
        std::size_t size1, size2;
        if (is >> ch && ch != '[') {
            is.putback (ch);
            is.setstate (std::ios::failbit);
        } else if (is >> size1 >> ch && ch != ',') {
            is.putback (ch);
            is.setstate (std::ios::failbit);
        } else if (is >> size2 >> ch && ch != ']') {
            is.putback (ch);
            is.setstate (std::ios::failbit);
        } else {
            matrix<VT, VA, VF> s (size1, size2);
            if (is >> ch && ch != '(') {
                is.putback (ch);
                is.setstate (std::ios::failbit);
            } else {
                for (std::size_t i = 0; i < size1; i ++) {
                    if (is >> ch && ch != '(') {
                        is.putback (ch);
                        is.setstate (std::ios::failbit);
                        break;
                    }
                    for (std::size_t j = 0; j < size2; j ++) {
                        if (is >> s (i, j) >> ch && ch != ',') {
                            is.putback (ch);
                            if (j < size2 - 1) {
                                is.setstate (std::ios::failbit);
                                break;
                            }
                        }
                    }
                    if (is >> ch && ch != ')') {
                        is.putback (ch);
                        is.setstate (std::ios::failbit);
                        break;
                    } 
                    if (is >> ch && ch != ',') {
                       is.putback (ch);
                       if (i < size1 - 1) {
                            is.setstate (std::ios::failbit);
                            break;
                       }
                    }
                }
                if (is >> ch && ch != ')') {
                    is.putback (ch);
                    is.setstate (std::ios::failbit);
                } 
            }
            if (! is.fail ()) {
                m.resize (size1, size2);
                m = s;
            }
        }
	    return is; 
    }

#endif 

}

#endif 



