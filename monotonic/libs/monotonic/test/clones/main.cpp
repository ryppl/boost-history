//
// test cases for ptr_container and clone_allocator issues
//

#include <iostream>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/abstract_allocator.hpp>
#include <boost/monotonic/allocator.hpp>

using namespace std;
using namespace boost;

namespace boost 
{ 
	//namespace ptr_container {

	namespace cloneable 
	{
		struct allocator;

		struct dyn_alloc_base
		{

		};

		template <class Derived>
		struct dyn_alloc : dyn_alloc_base
		{
			template <class Base, class Alloc>
			static void *alloc(Base *ptr, Alloc &a)
			{
				return new Derived();
			}
		};

		// common base for all base types for hierachies
		struct base_base
		{
			virtual ~base_base() { }

			// this can't work because it doesnt have an allocator :/
			virtual base_base *clone() const { return 0; }

			virtual base_base *allocate(boost::abstract_allocator &alloc) const = 0;
			virtual base_base *create(boost::abstract_allocator &alloc) const = 0;
			virtual base_base *copy_construct(const base_base &original, boost::abstract_allocator &alloc) const = 0;

		};

		template <class Derived>
		struct base : base_base
		{
			typedef Derived derived_type;
			typedef base<derived_type> this_type;

		private:
			static size_t alignment;
			mutable derived_type *self_ptr;

			derived_type *&self(derived_type *ptr) const
			{
				return ptr->this_type::self_ptr;
			}

		public:
			base() : self_ptr(0) { }

			virtual base<Derived> *allocate(boost::abstract_allocator &alloc) const 
			{
				boost::abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				self(ptr) = ptr;
				return ptr;
			}

			virtual base<Derived> *create(boost::abstract_allocator &alloc) const 
			{
				base<Derived> *ptr = allocate(alloc);
				//new (ptr) this_type();
				new (ptr->self_ptr) Derived();
				return ptr;
			}

			virtual base<Derived> *copy_construct(const base_base &original, boost::abstract_allocator &alloc) const 
			{ 
				base<Derived> *ptr = allocate(alloc);
				//new (ptr) this_type();
				new (ptr->self_ptr) Derived(static_cast<const Derived &>(original));
				return ptr;
			}
		};
		template <class D>
		size_t base<D>::alignment = boost::aligned_storage<sizeof(D)>::alignment;

		struct allocator
		{
			template< class U >
			static U* allocate_clone( const U& r )
			{
				// this is not allocated using the parent containers custom allocator
				return r.clone();
			}

			template< class U >
			static void deallocate_clone( const U* clone )
			{
				// this is not de-allocated using the parent containers custom allocator
				//! ??clone->deallocate();??
				//! delete clone;			// this is not correct either; also calls dtor

				// this is not correct either, but seems the best we can do...?
				std::allocator<char> alloc;
				U *ptr = const_cast<U *>(clone);
				alloc.deallocate(reinterpret_cast<char *>(ptr), 1);
			}

			// idea: pass allocator to the clone_allocator.
			// allocator rebind could be done in the ptr_container.
			// calling this must be disabled at compile-time for types that are not boost::is_convertible<cloneable::base<U> *, U*>
			template< class U, class Alloc >
			static U* allocate_clone( const U& r, Alloc &alloc )
			{
				U *ptr = r.copy_construct(r, alloc);
				return ptr;
			}

			// idea: this is not even needed? 
			// allocator rebind could be done in the ptr_container.
			template< class U, class Alloc >
			static U* deallocate_clone( const U* r, Alloc &alloc )
			{
				typename Alloc::template rebind<U>::other my_alloc(alloc);
				my_alloc.deallocate(const_cast<U *>(r));
			}
		};

	} // namespace cloneable

} // namespace boost

// client code...

struct derived : cloneable::base<derived>
{
	int num;

	derived() : num(0) { }
	explicit derived(int n) : num(n) { }

	// this can't work
	cloneable::base_base *clone() const
	{
		// this is really no good as it doesnt use the correct allocator
		return new derived(num);
	}
};

struct derived2 : cloneable::base<derived2>
{
	string str;

	derived2() { }
	explicit derived2(string const &n) : str(n) { }

	// this can't work
	cloneable::base_base *clone() const
	{
		// this is really no good as it doesnt use the correct allocator
		return new derived2(str);
	}
};

/*

namespace boost { namespace ptr_container {

// idea: switch allocator type ordering, default to cloneable::allcoator
template <class T, class Al = std::allocator<T>, class CloneAl = cloneable::allocator>
class ptr_vector;

}}

*/

int main()
{
	typedef ptr_vector<cloneable::base_base, cloneable::allocator, monotonic::allocator<int> > vec;

	{
		vec bases;
		BOOST_ASSERT(bases.get_allocator().get_storage() == &monotonic::static_storage<>::get_storage());

		//! bases.push_back(new derived(42));					// this doesn't use the custom allocator!
		//! derived *obj = bases.get_allocator().allocate(1);	// this has to be recast

		// do a dance to get the object into the container...
		typedef vec::allocator_type::template rebind<derived>::other derived_alloc_type;
		derived_alloc_type derived_alloc(bases.get_allocator());
		derived *obj = derived_alloc.allocate(1);
		//! derived_alloc.construct(obj, 42);		// can't pass ctor args to a v1 allocator
		new (obj) derived(42);						// bypassing allocator::construct :/

		// finally get the correctly allocated object into the container
		bases.push_back(obj);

		// idea: use variadic template arguments for push_back etc: 
		// default to use BOOST_PP for C++03

		//! bases.push_back<derived>(ctor_args...);
		//! bases.push_back<derived2>(ctor_args...);
		//! ...
		//! bases.push_back<derivedN>(ctor_args...);

		// this now works properly; after small changes to:
		//		ptr_container/detail/scoped_ptr.hpp
		//		ptr_container/detail/reversible_ptr_container.hpp
		// and by introducing boost::abstract_allocator

		vec copy = bases;

		// idea: could be fixed by using base<derived>::clone_with(orig, rebind<derived>(alloc)) in the ptr_container...

		// doesn't work because the clones were put on the heap. could work with cloneable:: ?
		monotonic::static_storage<>::release();
	}

	return 0;
}

//EOF
