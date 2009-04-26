// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include "categories.hpp"
#include "convenience.hpp"

#include <utility>
#include <type_traits>
#include <cstddef>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost{namespace fusion{namespace gsoc{
	class vector_tag;

	namespace detail
	{
		class vector_iterator_tag;
		template<class Vector,std::size_t Index> class vector_iterator
		{
			template<class,class>friend class is_compareable;
			friend class result_of::impl::end<vector_tag>;
			friend class result_of::impl::begin<vector_tag>;
			friend class result_of::impl::advance_c<vector_iterator_tag>;
			friend class result_of::impl::distance<vector_iterator_tag>;
			friend class result_of::impl::value_of<vector_iterator_tag>;
			friend class result_of::impl::deref<vector_iterator_tag>;

		public:
			typedef random_access_iterator_category category;
			typedef vector_iterator_tag tag;
			typedef mpl::bool_<std::is_const<Vector>::value> is_const;

		private:
			typedef Vector sequence;
			typedef mpl::size_t<Index> index;

			Vector& _vector;

			vector_iterator(Vector& vector):
				_vector(vector)
			{
			}

		public:
			vector_iterator(const vector_iterator& other_iterator):
				_vector(other_iterator._vector)
			{
			}

			vector_iterator& operator=(const vector_iterator& other_iterator)
			{
				_vector=other_iterator._vector;
				return *this;
			}
		};

		template<class... Elements> class vector_impl;
		template<> class vector_impl<>{};

		template<class Head,class...Other>class vector_impl<Head,Other...> :
			public vector_impl<Other...>
		{
		public:
			friend class result_of::impl::deref<detail::vector_iterator_tag>;

			typedef vector_impl<Other...> base;
			typedef Head head_type;

		private:
			Head _element;

		public:
			vector_impl()
			{
			}

			vector_impl(const vector_impl& other_impl):
				base(static_cast<const base&>(other_impl)),
				_element(other_impl._element)
			{
			}

			vector_impl(vector_impl&& other_impl):
				base(std::move(static_cast<base&>(other_impl))),
				_element(std::move(other_impl._element))
			{
			}

			template<class OtherHead,class... OtherElements>vector_impl(const OtherHead& other_head,const OtherElements&... other_elements):
				base(other_elements...),
				_element(other_head)
			{
			}

			template<class OtherHead,class... OtherElements>vector_impl(OtherHead&& other_head,OtherElements&&... other_elements):
				base(std::forward<OtherElements>(other_elements)...),
				_element(std::forward<OtherHead>(other_head))
			{
			}

			template<class OtherHead,class... OtherElements>vector_impl(const vector_impl<OtherHead,OtherElements...>& other_impl):
				base(static_cast<const typename vector_impl<OtherHead,OtherElements...>::base&>(other_impl)),
				_element(other_impl._element)
			{
			}

			template<class OtherHead,class... OtherElements>vector_impl(vector_impl<OtherHead,OtherElements...>&& other_impl):
				base(std::move(static_cast<typename vector_impl<OtherHead,OtherElements...>::base&>(other_impl))),
				_element(std::move(other_impl._element))
			{
			}

			void operator=(const vector_impl& other_impl)
			{
				_element=other_impl._element;
				static_cast<base&>(*this)=static_cast<const base&>(other_impl);
			}

			void operator=(vector_impl&& other_impl)
			{
				_element=std::move(other_impl._element);
				static_cast<base&>(*this)=std::move(static_cast<base&>(other_impl));
			}

			template<class OtherHead,class... OtherElements>void operator=(const vector_impl<OtherHead,OtherElements...>& other_impl)
			{
				_element=other_impl._element;
				static_cast<base&>(*this)=static_cast<const typename vector_impl<OtherHead,OtherElements...>::base&>(other_impl);
			}

			template<class OtherHead,class... OtherElements>void operator=(vector_impl<OtherHead,OtherElements...>&& other_impl)
			{
				_element=std::move(other_impl._element);
				static_cast<base&>(*this)=std::move(static_cast<typename vector_impl<OtherHead,OtherElements...>::base&>(other_impl));
			}
		};
	}

	template<class... Elements>class vector:
		private detail::vector_impl<Elements...>
	{
		friend class result_of::impl::end<vector_tag>;
		friend class result_of::impl::begin<vector_tag>;
		friend class result_of::impl::value_of<detail::vector_iterator_tag>;
		friend class result_of::impl::deref<detail::vector_iterator_tag>;

	private:
		typedef detail::vector_impl<Elements...> base;
		typedef mpl::size_t<sizeof...(Elements)> num_elements;

	public:
		typedef random_access_sequence_category category;
		typedef vector_tag tag;

		vector()
		{
		}

		vector(const vector& other_vector):
			base(other_vector)
		{
		}

		vector(vector&& other_vector):
			base(std::move(other_vector))
		{
		}

		template<class... OtherElements> vector(const vector<OtherElements...>& other_vector):
			base(other_vector)
		{
		}

		template<class... OtherElements> vector(vector<OtherElements...>&& other_vector):
			base(std::move(other_vector))
		{
		}

		template<class... OtherElements> explicit vector(const OtherElements&... other_elements):
			base(std::forward<OtherElements>(other_elements)...)
		{
		}

		template<class... OtherElements> explicit vector(OtherElements&&... other_elements):
			base(std::forward<OtherElements>(other_elements)...)
		{
		}

		/*template<class OtherSequence> vector(OtherSequence&& other_sequence)
		{
		}

		template<class OtherSequence> vector(const OtherSequence& other_sequence)
		{
		}*/

		vector& operator=(const vector& other_vector)
		{
			static_cast<base&>(*this)=other_vector;
			return *this;
		}

		vector& operator=(vector&& other_vector)
		{
			static_cast<base&>(*this)=std::move(other_vector);
			return *this;
		}
	};

	//begin
	namespace result_of
	{
		namespace impl
		{
			template<>class begin<vector_tag>
			{
			public:
				template<class Vector>class apply
				{
				public:
					typedef gsoc::detail::vector_iterator<Vector,0> type;

					static type call(Vector&& vector)
					{
						return type(vector);
					}

					static type call(Vector& vector)
					{
						return type(vector);
					}
				};
			};
		}
	}

	//end
	namespace result_of
	{
		namespace impl
		{
			template<>class end<vector_tag>
			{
			public:
				template<class Vector> class apply
				{
				public:
					typedef gsoc::detail::vector_iterator<Vector,Vector::num_elements::value> type;

					static type call(Vector&& vector)
					{
						return type(vector);
					}

					static type call(Vector& vector)
					{
						return type(vector);
					}
				};
			};
		}
	}

	//advance_c
	namespace result_of
	{
		namespace impl
		{
			template<>class advance_c<gsoc::detail::vector_iterator_tag>
			{
			public:
				template<class Iterator,int Distance>class apply
				{
				public:
					typedef gsoc::detail::vector_iterator<typename Iterator::sequence,Iterator::index::value+Distance> type;

					static type call(const Iterator& iterator)
					{
						return type(iterator._vector);
					}
				};
			};
		}
	}

	//distance
	namespace result_of
	{
		namespace impl
		{
			template<>class distance<gsoc::detail::vector_iterator_tag>
			{
			public:
				template<class IteratorA,class IteratorB>class apply
				{
				public:
					typedef mpl::size_t<IteratorB::index::value-IteratorA::index::value> type;
				};
			};
		}
	}

	//value_of
	namespace result_of
	{
		namespace impl
		{
			template<>class value_of<gsoc::detail::vector_iterator_tag>
			{
			public:
				template<class Iterator>class apply
				{
				private:
					template<class VectorImpl,std::size_t Counter>class impl
					{
					public:
						typedef typename mpl::eval_if_c<!Counter,
							mpl::identity<typename VectorImpl::head_type>,
							impl<typename VectorImpl::base,Counter-1> >::type type;
					};

				public:
					typedef typename impl<typename Iterator::sequence::base,Iterator::index::value>::type type;
				};
			};
		}
	}

	//deref
	namespace result_of
	{
		namespace impl
		{
			template<>class deref<gsoc::detail::vector_iterator_tag>
			{
			public:
				template<class Iterator>class apply
				{
				private:
					typedef typename result_of::value_of<Iterator>::type value_type;
				public:
					typedef typename std::add_reference<
						typename std::conditional<Iterator::is_const::value,typename std::add_const<value_type>::type,value_type>::type
						>::type type;

				private:
					template<std::size_t Index,class VectorImpl>static typename std::enable_if<!Index,type>::type impl(VectorImpl& impl)
					{
						return impl._element;
					}

					template<std::size_t Index,class VectorImpl>static typename std::enable_if<Index,type>::type impl(VectorImpl& impl)
					{
						return impl<Index-1>(static_cast<typename VectorImpl::base&>(impl));
					}

					template<std::size_t Index,class VectorImpl>static typename std::enable_if<!Index,type>::type impl(const VectorImpl& impl)
					{
						return impl._element;
					}

					template<std::size_t Index,class VectorImpl>static typename std::enable_if<Index,type>::type impl(const VectorImpl& impl)
					{
						return impl<Index-1>(static_cast<const typename VectorImpl::base&>(impl));
					}

				public:
					static type call(const Iterator& iterator)
					{
						return impl<Iterator::index::value>(iterator._vector);
					}
				};
			};
		}
	}
}}}
