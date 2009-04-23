// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <concepts>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>

namespace boost{namespace fusion{namespace gsoc{

	namespace detail
	{
		template<class... Elements> class vector_impl;
		template<> class vector_impl<>{};

		template<class Head,class...Other>class vector_impl<Head,Other...> :
			public vector_impl<Other...>
		{
			template<class...> friend class vector_impl;

		private:
			typedef vector_impl<Other...> base;

			Head _element;

		public:
			vector_impl()
			{
			}

			vector_impl(const vector_impl& other_impl):
				base(other_impl),
				_element(other_impl._element)
			{
			}

			vector_impl(vector_impl&& other_impl):
				base(std::move(other_impl)),
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
				base(other_impl),
				_element(other_impl._element)
			{
			}

			template<class OtherHead,class... OtherElements>vector_impl(vector_impl<OtherHead,OtherElements...>&& other_impl):
				base(std::move(other_impl)),
				_element(std::move(other_impl._element))
			{
			}

			void operator=(const vector_impl& other_impl)
			{
				_element=other_impl._element;
				static_cast<base&>(*this)=other_impl;
			}

			void operator=(vector_impl&& other_impl)
			{
				_element=std::move(other_impl._element);
				static_cast<base&>(*this)=std::move(other_impl);
			}

			template<class OtherHead,class... OtherElements>void operator=(const vector_impl<OtherHead,OtherElements...>& other_impl)
			{
				_element=other_impl._element;
				static_cast<base&>(*this)=other_impl;
			}

			template<class OtherHead,class... OtherElements>void operator=(vector_impl<OtherHead,OtherElements...>&& other_impl)
			{
				_element=std::move(other_impl._element);
				static_cast<base&>(*this)=std::move(other_impl);
			}
		};
	}

	template<class... Elements>class vector:
		public detail::vector_impl<Elements...>
	{
	private:
		typedef detail::vector_impl<Elements...> base;

	public:
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

	template<class Vector, std::size_t Index> class vector_iterator
    {
	public:
        typedef mpl::size_t<Index> index;
        typedef Vector vector;

	private:		
        Vector& _vector;

	public:
        vector_iterator(Vector& vector):
			_vector(vector)
		{
		}
    };

	namespace result_of
	{
		template<class Sequence>class size;
		template<class... Elements>class size<vector<Elements...>>
		{
		public:
			typedef mpl::size_t<sizeof...(Elements)> type;
		};

		template<class Sequence>class empty
		{
		public:
			typedef mpl::bool_<!size<Sequence>::type::value> type;
		};

		template<class Sequence>class begin;
		template<class... Elements>class begin<vector<Elements...>>
		{
		public:
			typedef vector_iterator<vector<Elements...>,0> type;
		};

		template<class Sequence>class end;
		template<class... Elements>class end<vector<Elements...>>
		{
		public:
			typedef vector_iterator<vector<Elements...>,size<vector<Elements...>>::type::value> type;
		};

		template<class Sequence>class next;
		template<class Vector,std::size_type Index>class next<vector_iterator<vector,Index>>
		{
		public:
			typedef vector_iterator<Vector,Index+1> type;
		};
	}

	template<class Sequence> typename result_of::size<Sequence>::type size(const Sequence&)
	{
		return typename result_of::size<Sequence>::type();
	}

	template<class Sequence> typename result_of::empty<Sequence>::type empty(const Sequence&)
	{
		return typename result_of::empty<Sequence>::type();
	}

    template<class Sequence> typename result_of::begin<Sequence>::type begin(Sequence& sequence)
    {
        return result_of::begin<Sequence>(sequence);
    }

    template<class Sequence> typename result_of::begin<Sequence const>::type begin(const Sequence& sequence)
    {
        return result_of::begin<const Sequence>(sequence);
    }

    template<class Sequence> typename result_of::end<Sequence>::type end(Sequence& sequence)
    {
        return result_of::end<Sequence>(sequence);
    }

    template<class Sequence> typename result_of::end<Sequence const>::type end(const Sequence& sequence)
    {
        return result_of::end<const Sequence>(sequence);
    }
}}}

class copyable
{
//private:
	//copyable(copyable&&);

public:
	copyable()
	{
	}
	copyable(const copyable&)
	{
	}
};

class moveable
{
private:
	moveable(const moveable&);

public:
	moveable()
	{
	}
	moveable(moveable&&)
	{
	}
};

int main()
{
	namespace gsoc=boost::fusion::gsoc;
	using namespace std;

	gsoc::vector<int,float,char> a;
	gsoc::vector<int,float,char> b(0,0,0);

	a=b;
	b=move(a);

	gsoc::vector<moveable,copyable> c;
	moveable ma;
	copyable ca;
	gsoc::vector<moveable,copyable> d(move(ma),move(ca));

	//c=c;
	//d=move(d);

	return 0;
}
