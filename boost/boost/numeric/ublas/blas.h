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

#ifndef NUMERICS_BLAS_H
#define NUMERICS_BLAS_H

namespace numerics {

    namespace blas_1 {

        template<class V>
		type_traits<typename V::value_type>::norm_type 
		asum (const V &v) {
			return norm_1 (v);
        }
        template<class V>
		type_traits<typename V::value_type>::norm_type 
		nrm2 (const V &v) {
            return norm_2 (v);
        }
        template<class V>
		type_traits<typename V::value_type>::norm_type 
		amax (const V &v) {
            return norm_inf (v);
        }

        template<class V1, class V2>
		promote_traits<typename V1::value_type, typename V2::value_type>::promote_type 
		dot (const V1 &v1, const V2 &v2) {
            return inner_prod (v1, v2);
        }

        template<class V1, class V2>
		V1 & 
		safe_copy (V1 &v1, const V2 &v2) {
            return v1 = v2;
        }
        template<class V1, class V2>
		V1 & 
		fast_copy (V1 &v1, const V2 &v2) {
            return v1 ^= v2;
        }

        template<class V1, class V2>
		void safe_swap (V1 &v1, V2 &v2) {
            numerics::safe_swap (v1, v2);
        }
        template<class V1, class V2>
		void 
		fast_swap (V1 &v1, V2 &v2) {
            numerics::fast_swap (v1, v2);
        }

        template<class V, class T>
		V &
		scal (V &v, const T &t) {
            return v *= t;
        }

        template<class V1, class T, class V2>
		V1 &
		axpy (V1 &v1, const T &t, const V2 &v2) {
            return v1 ^= v1 + t * v2;
        }

        template<class T1, class V1, class T2, class V2>
		void 
		rot (const T1 &t1, V1 &v1, const T2 &t2, V2 &v2) {
#ifdef USE_MSVC
			typedef promote_traits<V1::value_type, V2::value_type>::promote_type promote_type;
#else // USE_MSVC
			typedef promote_traits<typename V1::value_type, typename V2::value_type>::promote_type promote_type;
#endif // USE_MSVC
			vector<promote_type> vt (t1 * v1 + t2 * v2);
			v2 ^= - t2 * v1 + t1 * v2;
			v1 ^= vt;
        }

    }

    namespace blas_2 {

		template<class V, class M>
		V &
		tmv (V &v, const M &m) {
			return v = prod (m, v);
		}

		template<class V, class M>
		V &
		tsv (V &v, const M &m, int hint) {
			return v = solve (m, v, v, hint);
		}

		template<class V1, class T1, class T2, class M, class V2>
		V1 &
		gmv (V1 &v1, const T1 &t1, const T2 &t2, const M &m, const V2 &v2) {
			return v1 = t1 * v1 + t2 * prod (m, v2);
		}

		template<class M, class T, class V1, class V2>
		M &
		gr (M &m, const T &t, const V1 &v1, const V2 &v2) {
#ifdef NUMERICS_USE_ET
			return m += t * numerics::outer_prod (v1, v2);
#else // NUMERICS_USE_ET
			return m = m + t * numerics::outer_prod (v1, v2);
#endif // NUMERICS_USE_ET
		}

		template<class M, class T, class V>
		M &
		sr (M &m, const T &t, const V &v) {
#ifdef NUMERICS_USE_ET
			return m += t * numerics::outer_prod (v, v);
#else // NUMERICS_USE_ET
			return m = m + t * numerics::outer_prod (v, v);
#endif // NUMERICS_USE_ET
		}
		template<class M, class T, class V>
		M &
		hr (M &m, const T &t, const V &v) {
#ifdef NUMERICS_USE_ET
			return m += t * numerics::outer_prod (v, numerics::conj (v));
#else // NUMERICS_USE_ET
			return m = m + t * numerics::outer_prod (v, numerics::conj (v));
#endif // NUMERICS_USE_ET
		}

		template<class M, class T, class V1, class V2>
		M &
		sr2 (M &m, const T &t, const V1 &v1, const V2 &v2) {
#ifdef NUMERICS_USE_ET
			return m += t * (numerics::outer_prod (v1, v2) + numerics::outer_prod (v2, v1));
#else // NUMERICS_USE_ET
			return m = m + t * (numerics::outer_prod (v1, v2) + numerics::outer_prod (v2, v1));
#endif // NUMERICS_USE_ET
		}
		template<class M, class T, class V1, class V2>
		M &
		hr2 (M &m, const T &t, const V1 &v1, const V2 &v2) {
#ifdef NUMERICS_USE_ET
			return m += t * numerics::outer_prod (v1, numerics::conj (v2)) + 
						numerics::detail::conj (t) * numerics::outer_prod (v2, numerics::conj (v1));
#else // NUMERICS_USE_ET
			return m = m + t * numerics::outer_prod (v1, numerics::conj (v2)) + 
						   numerics::detail::conj (t) * numerics::outer_prod (v2, numerics::conj (v1));
#endif // NUMERICS_USE_ET
		}

	}

    namespace blas_3 {

		template<class M1, class T, class M2, class M3>
		M1 &
		tmm (M1 &m1, const T &t, const M2 &m2, const M3 &m3) {
			return m1 = t * prod (m2, m3);
		}

		template<class M1, class T, class M2>
		M1 &
		tsm (M1 &m1, const T &t, const M2 &m2, int hint) {
			return m1 = solve (m2, m1, t * m1, hint);
		}

		template<class M1, class T1, class T2, class M2, class M3>
		M1 &
		gmm (M1 &m1, const T1 &t1, const T2 &t2, const M2 &m2, const M3 &m3) {
			return m1 = t1 * m1 + t2 * prod (m2, m3);
		}

		template<class M1, class T1, class T2, class M2>
		M1 &
		srk (M1 &m1, const T1 &t1, const T2 &t2, const M2 &m2) {
			return m1 = t1 * m1 + t2 * prod (m2, trans (m2));
		}
		template<class M1, class T1, class T2, class M2>
		M1 &
		hrk (M1 &m1, const T1 &t1, const T2 &t2, const M2 &m2) {
			return m1 = t1 * m1 + t2 * prod (m2, herm (m2));
		}

		template<class M1, class T1, class T2, class M2, class M3>
		M1 &
		sr2k (M1 &m1, const T1 &t1, const T2 &t2, const M2 &m2, const M3 &m3) {
			return m1 = t1 * m1 + t2 * (prod (m2, trans (m3)) + prod (m3, trans (m2)));
		}
		template<class M1, class T1, class T2, class M2, class M3>
		M1 &
		hr2k (M1 &m1, const T1 &t1, const T2 &t2, const M2 &m2, const M3 &m3) {
			return m1 = t1 * m1 + t2 * prod (m2, herm (m3)) + 
								  numerics::detail::conj (t2) * prod (m3, herm (m2));
		}

	}

}

#endif // NUMERICS_BLAS_H