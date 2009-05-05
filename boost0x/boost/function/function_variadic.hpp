// Boost.Function library

//  Copyright Douglas Gregor 2001-2006
//  Copyright Emil Dotchevski 2007
//  Use, modification and distribution is subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_FUNCTION_FUNCTION_VARIADIC_HPP
#define BOOST_FUNCTION_FUNCTION_VARIADIC_HPP

#include <boost/function/detail/prologue.hpp>
#include <boost/function/function_variadic_invoker.hpp>
#include <boost/function/function_variadic_vtable.hpp>

namespace boost
{
    namespace detail
    {
        namespace function
        {
            struct not_a_std_function {};

            template <typename R, typename ... Args>
            struct which_std_function {
                typedef not_a_std_function type;
            };
            template <typename R, typename Arg1>
            struct which_std_function<R, Arg1> {
                typedef std::unary_function<Arg1, R> type;
            };
            template <typename R, typename Arg1, typename Arg2>
            struct which_std_function<R, Arg1, Arg2> {
                typedef std::binary_function<Arg1, Arg2, R> type;
            };
        }
    }

    template <typename R, typename ... Args>
    class function<R (Args...)> : public function_base,
        public detail::function::which_std_function<R, Args...>::type
    {
        typedef function<R (Args...)> self_type;
        struct clear_type {};

        typedef boost::detail::function::basic_vtable<R, Args...>
            vtable_type;

        vtable_type* get_vtable() const {
            return reinterpret_cast<vtable_type*>(
                reinterpret_cast<std::size_t>(vtable) & ~(std::size_t)0x01);
        }

    public:
        typedef R result_type;

        BOOST_STATIC_CONSTANT(int, args = sizeof...(Args));
        BOOST_STATIC_CONSTANT(int, arity = sizeof...(Args));

        // add signature for boost::lambda
        template<typename Argsx>
        struct sig
        {
            typedef result_type type;
        };
        // Note: argN_type typedefs are gone

        function() : function_base() {}

        template<typename Functor>
        function(Functor f
               ,typename enable_if_c<
                                (boost::type_traits::ice_not<
                              (is_integral<Functor>::value)>::value),
                           int>::type = 0
               ) :
            function_base()
        {
            this->assign_to(f);
        }
        template<typename Functor, typename Allocator>
        function(Functor f, Allocator a
               ,typename enable_if_c<
                                (boost::type_traits::ice_not<
                              (is_integral<Functor>::value)>::value),
                           int>::type = 0
               ) :
            function_base()
        {
            this->assign_to_a(f, a);
        }

        function(clear_type*) : function_base() {}

        function(const self_type& f)
            : function_base()
        {
            this->assign_to_own(f);
        }

        ~function() { clear(); }

        self_type& operator=(const self_type& f)
        {
            if (&f == this)
                return *this;

            this->clear();
            try {
                this->assign_to_own(f);
            } catch (...) {
                vtable = 0;
                throw;
            }
            return *this;
        }

        template<typename Functor>
        typename enable_if_c<
                                (boost::type_traits::ice_not<
                             (is_integral<Functor>::value)>::value),
                          self_type&>::type
        operator=(Functor f)
        {
            this->clear();
            try {
                this->assign_to(f);
            } catch (...) {
                vtable = 0;
                throw;
            }
            return *this;
        }

        template<typename Functor, typename Allocator>
        void assign(Functor f, Allocator a)
        {
            this->clear();
            try {
                this->assign_to_a(f, a);
            } catch (...) {
                vtable = 0;
                throw;
            }
        }

        self_type& operator=(clear_type*)
        {
            this->clear();
            return *this;
        }


        void swap(self_type& other)
        {
            if (&other == this)
                return;

            self_type tmp;
            tmp.move_assign(*this);
            this->move_assign(other);
            other.move_assign(tmp);
        }

        // Clear out a target, if there is one
        void clear()
        {
            if (vtable) {
                if (!this->has_trivial_copy_and_destroy())
                    get_vtable()->clear(this->functor);
                vtable = 0;
            }
        }

    private:
        struct dummy {
            void nonnull() {};
        };

    typedef void (dummy::*safe_bool)();

    public:
        operator safe_bool () const
        { return (this->empty())? 0 : &dummy::nonnull; }

        bool operator!() const
        { return this->empty(); }

        result_type operator()(Args... args) const;

    private:
        void assign_to_own(const self_type& f)
        {
            if (!f.empty()) {
                this->vtable = f.vtable;
                if (this->has_trivial_copy_and_destroy())
                    this->functor = f.functor;
                else
                    get_vtable()->base.manager(f.functor, this->functor,
                        boost::detail::function::clone_functor_tag);
            }
        }

        template<typename Functor>
        void assign_to(Functor f)
        {
            using detail::function::vtable_base;

            typedef typename
                detail::function::get_function_tag<Functor>::type tag;
            typedef detail::function::get_invoker<tag> get_invoker;
            typedef typename
                get_invoker::template apply<Functor, R, Args...>
                handler_type;

            typedef typename handler_type::invoker_type invoker_type;
            typedef typename handler_type::manager_type manager_type;

            // Note: it is extremely important that this initialization use
            // static initialization. Otherwise, we will have a race
            // condition here in multi-threaded code. See
            // http://thread.gmane.org/gmane.comp.lib.boost.devel/164902/.
            static vtable_type stored_vtable = 
                { { &manager_type::manage }, &invoker_type::invoke };

            if (stored_vtable.assign_to(f, functor)) {
                std::size_t value = reinterpret_cast<std::size_t>(
                    &stored_vtable.base);
                if (boost::has_trivial_copy_constructor<Functor>::value &&
                    boost::has_trivial_destructor<Functor>::value &&
                    detail::function::
                        function_allows_small_object_optimization<Functor>::
                        value)
                    value |= (std::size_t)0x01;
                vtable = reinterpret_cast<detail::function::vtable_base *>(
                    value);
            } else
                vtable = 0;
        }

        template<typename Functor, typename Allocator>
        void assign_to_a(Functor f, Allocator a)
        {
            using detail::function::vtable_base;

            typedef typename
                detail::function::get_function_tag<Functor>::type tag;
            typedef detail::function::get_invoker<tag> get_invoker;
            typedef typename get_invoker::
                template apply_a<Functor, Allocator, R, Args...>
                handler_type;

            typedef typename handler_type::invoker_type invoker_type;
            typedef typename handler_type::manager_type manager_type;

            // Note: it is extremely important that this initialization use
            // static initialization. Otherwise, we will have a race
            // condition here in multi-threaded code. See
            // http://thread.gmane.org/gmane.comp.lib.boost.devel/164902/.
            static vtable_type stored_vtable = 
                { { &manager_type::manage }, &invoker_type::invoke };

            if (stored_vtable.assign_to_a(f, functor, a)) {
                std::size_t value = reinterpret_cast<std::size_t>(
                    &stored_vtable.base);
                if (boost::has_trivial_copy_constructor<Functor>::value &&
                    boost::has_trivial_destructor<Functor>::value &&
                    detail::function::
                        function_allows_small_object_optimization<Functor>::
                        value)
                    value |= (std::size_t)0x01;
                vtable = reinterpret_cast<detail::function::vtable_base *>(
                    value);
            } else
                vtable = 0;
        }

        // Moves the value from the specified argument to *this. If the
        // argument has its function object allocated on the heap,
        // move_assign will pass its buffer to *this, and set the
        // argument's buffer pointer to NULL.
        void move_assign(self_type& f)
        {
            if (&f == this)
                return;

#if !defined(BOOST_NO_EXCEPTIONS)      
            try {
#endif
                if (!f.empty()) {
                    this->vtable = f.vtable;
                    if (this->has_trivial_copy_and_destroy())
                        this->functor = f.functor;
                    else
                        get_vtable()->base.manager(f.functor,
                            this->functor,
                            boost::detail::function::move_functor_tag);
                    f.vtable = 0;
                } else {
                    clear();
                }
#if !defined(BOOST_NO_EXCEPTIONS)      
            } catch (...) {
                vtable = 0;
                throw;
            }
#endif
        }
    };

    template<typename R, typename... Args>
    inline R
    function<R (Args...)>::operator()(Args ...args) const
    {
        if (this->empty())
            boost::throw_exception(bad_function_call());

        return R(get_vtable()->invoker(this->functor, args...));
    }

    // Poison comparisons between boost::function objects of the same type.
    template<typename Signature>
    void operator==(const function<Signature>&, const function<Signature>&);
    template<typename Signature>
    void operator!=(const function<Signature>&, const function<Signature>&);
}

#endif
