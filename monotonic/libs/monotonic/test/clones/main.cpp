
#include <iostream>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/monotonic/allocator.hpp>

using namespace std;
using namespace boost;

struct type_number
{
	int value;
	enum 
	{
		None = 0,
		Base = 1,
		Derived = 2,
	};
	type_number(int n = None) : value(n) { }
};

struct base
{
	type_number type;

	explicit base(type_number t) : type(t) { }
	virtual ~base() { }

	virtual base *clone() const = 0;
};

struct derived : base
{
	int num;
	explicit derived(int n) : base(type_number::Derived), num(n) { }

	base *clone() const
	{
		return new derived(num);
	}
};

struct clone_allocator
{
	template< class U >
	static U* allocate_clone( const U& r )
	{
		return r.clone();
	}

	template< class U >
	static void deallocate_clone( const U* clone )
	{
		if (clone)
			clone->U::~U();
	}
};

int main()
{
	typedef ptr_vector<base, clone_allocator, monotonic::allocator<int> > vec;
	// why ptr_vector<..> p(n) resize the vector to have n default-constructed elements? 
	// it only reserves n elements at the moment...
	//vec bases(1);
	//bases[0] = derived(42);  // throws out of range error

	//vec bases;
	//bases.resize(1);			// doesnt work because it cannot instantiate an abstract class
	//bases[0] = derived(42);

	vec bases;
	bases.push_back(new derived(42));

	vec copy = bases;
	BOOST_ASSERT(copy.size() == 1);
	BOOST_ASSERT(copy.at(0).type.value == type_number::Derived);
	return 0;
}

//EOF
