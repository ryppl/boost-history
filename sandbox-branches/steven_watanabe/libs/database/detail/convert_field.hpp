#ifndef BOOST_DATABASE_DETAIL_CONVERT_FIELD_HPP_
#define BOOST_DATABASE_DETAIL_CONVERT_FIELD_HPP_

#include "../field.hpp"

#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/variant/apply_visitor.hpp>

#include <boost/lexical_cast.hpp>

namespace boost {

	namespace db {

		namespace detail {
		
			template <typename T> struct enum_type {

				typedef int type;
			};

			template <typename T> struct convert_field_visitor : static_visitor<> {

				typedef T result_type;

				T operator()(const T& t) const {
				
					return t;
				}

				struct normal_converter {

					template <typename U> static T convert(const U& u) {

						return lexical_cast<typename remove_const<T>::type>(u);
					}
				};

				struct to_enum_converter {

					template <typename U> static T convert(const U& u) {

						return static_cast<T>(lexical_cast<typename enum_type<T>::type>(u));
					}
				};

				struct from_enum_converter {

					template <typename U> static T convert(const U& u) {

						return lexical_cast<T>(static_cast<typename enum_type<T>::type>(u));
					}
				};

				template <typename U> T operator()(const U& u) const {

					typedef typename mpl::if_<
							is_enum<U>,
							from_enum_converter,
							typename mpl::if_<
								is_enum<T>,
								to_enum_converter,
								normal_converter
							>::type
					>::type converter_type;

					return converter_type::convert(u);
				}
			};

			template <typename Type> inline
			Type
			convert_field(const field& f) {

				return ::boost::apply_visitor(convert_field_visitor<Type>(), f);
			}
		}
	}
}

#endif /* BOOST_DATABASE_DETAIL_CONVERT_FIELD_HPP_ */
