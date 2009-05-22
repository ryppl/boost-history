//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_LEX_STATIC_LEXER_FEB_10_2008_0753PM)
#define BOOST_SPIRIT_LEX_STATIC_LEXER_FEB_10_2008_0753PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/support/safe_bool.hpp>
#include <boost/spirit/home/lex/lexer/lexertl/token.hpp>
#include <boost/spirit/home/lex/lexer/lexertl/static_functor.hpp>
#include <boost/spirit/home/lex/lexer/lexertl/iterator.hpp>
#if defined(BOOST_SPIRIT_DEBUG)
#include <boost/spirit/home/support/detail/lexer/debug.hpp>
#endif

namespace boost { namespace spirit { namespace lex { namespace lexertl
{ 
    ///////////////////////////////////////////////////////////////////////////
    // forward declaration only
    namespace static_ 
    {
        //  Both items, the table of names and the function to return the next 
        //  token have to be generated using the function generate_static().
        //
        //  This is a forward declaration for the generated static table of 
        //  valid state names
        extern char const* const lexer_state_names[];
        extern std::size_t const lexer_state_count;

        //  This is the forward declaration of the generated function to be 
        //  called to get the next token. 
        template <typename Iterator>
        std::size_t next_token (std::size_t& state_, Iterator const& start_, 
            Iterator &current_, Iterator const& end_);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //  static_token_set
    ///////////////////////////////////////////////////////////////////////////
    template <typename Token, typename Iterator = typename Token::iterator_type>
    class static_token_set
    {
    protected:
        typedef typename boost::detail::iterator_traits<Iterator>::value_type 
            char_type;
        typedef std::basic_string<char_type> string_type;

    public:
        typedef Token token_type;
        typedef typename Token::id_type id_type;

        // interface for token definition management
        void add_token (char_type const* state, string_type const& tokendef
          , std::size_t token_id) {}

        // interface for pattern definition management
        void add_pattern (char_type const* state, string_type const& name
          , string_type const& patterndef) {}

        void clear() {}

        std::size_t add_state(char_type const* state)
        {
            return 0;
        }
        string_type initial_state() const 
        { 
            return lex::lexertl::static_::lexer_state_names[0];
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Every lexer type to be used as a lexer for Spirit has to conform to 
    //  the following public interface:
    //
    //    typedefs: 
    //        iterator_type   The type of the iterator exposed by this lexer.
    //        token_type      The type of the tokens returned from the exposed 
    //                        iterators.
    //        token_set       The type of the token set representing a lexer 
    //                        state.
    //
    //    functions:
    //        default constructor
    //                        Since lexers are instantiated as base classes 
    //                        only it might be a good idea to make this 
    //                        constructor protected.
    //        begin, end      Return a pair of iterators, when dereferenced
    //                        returning the sequence of tokens recognized in 
    //                        the input stream given as the parameters to the 
    //                        begin() function.
    //        add_token       Should add the definition of a token to be 
    //                        recognized by this lexer.
    //        clear           Should delete all current token definitions
    //                        associated with the given state of this lexer 
    //                        object.
    //
    //    template parameters:
    //        Token           The type of the tokens to be returned from the
    //                        exposed token iterator.
    //        Iterator        The type of the iterator used to access the
    //                        underlying character stream.
    //        Functor         The type of the InputPolicy to use to instantiate
    //                        the multi_pass iterator type to be used as the 
    //                        token iterator (returned from begin()/end()).
    //        TokenSet        The type of the token set to use in conjunction 
    //                        with this lexer type. This is used for the 
    //                        token_set typedef described above only.
    //
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The static_lexer class is a implementation of a Spirit.Lex 
    //  lexer on top of Ben Hanson's lexertl library (For more information 
    //  about lexertl go here: http://www.benhanson.net/lexertl.html). 
    //
    //  This class is designed to be used in conjunction with a generated, 
    //  static lexer. For more information see the documentation (The Static 
    //  Lexer Model).
    //
    //  This class is supposed to be used as the first and only template 
    //  parameter while instantiating instances of a lex::lexer class.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename Token = token<>
      , typename Iterator = typename Token::iterator_type
      , typename Functor = static_functor<Token, Iterator, mpl::false_>
      , typename TokenSet = lex::token_set<static_token_set<Token, Iterator> > >
    class static_lexer 
    {
    public:
        // operator_bool() is needed for the safe_bool base class
        operator typename safe_bool<static_lexer>::result_type() const 
        { 
            return safe_bool<static_lexer>()(true);    // object is always valid
        }

        typedef typename boost::detail::iterator_traits<Iterator>::value_type 
            char_type;
        typedef std::basic_string<char_type> string_type;

        //  Every lexer type to be used as a lexer for Spirit has to conform to 
        //  a public interface 
        typedef Token token_type;
        typedef typename Token::id_type id_type;
        typedef TokenSet token_set;
        typedef iterator<Functor> iterator_type;

    private:
        // this type is purely used for the iterator_type construction below
        struct iterator_data_type 
        {
            typename Functor::next_token_functor next_;
            typename Functor::semantic_actions_type const& actions_;
            std::size_t const state_count_;
            const char* const* state_names_;
        };

    public:
        //  Return the start iterator usable for iterating over the generated
        //  tokens, the Functor F is called to match the next token from the 
        //  input.
        template <typename F>
        iterator_type begin(Iterator& first, Iterator const& last, F next) const
        { 
            iterator_data_type iterator_data = { next, actions
              , static_::lexer_state_count, static_::lexer_state_names };
            return iterator_type(iterator_data, first, last);
        }

        //  Return the start iterator usable for iterating over the generated
        //  tokens, the generated function next_token(...) is called to match 
        //  the next token from the input.
        template <typename Iterator_>
        iterator_type begin(Iterator_& first, Iterator_ const& last) const
        { 
            iterator_data_type iterator_data = 
                { &lex::lexertl::static_::next_token<Iterator_>, actions,
                  static_::lexer_state_count, static_::lexer_state_names };
            return iterator_type(iterator_data, first, last);
        }

        //  Return the end iterator usable to stop iterating over the generated 
        //  tokens.
        iterator_type end() const
        { 
            return iterator_type(); 
        }

    protected:
        //  Lexer instances can be created by means of a derived class only.
        static_lexer(unsigned int flags) {}

    public:
        // interface for token definition management
        void add_token (char_type const* state, char_type tokendef
          , std::size_t token_id) {}
        void add_token (char_type const* state, string_type const& tokendef
          , std::size_t token_id) {}
        void add_token(char_type const* state, token_set& tokset) {}

        // interface for pattern definition management
        void add_pattern (char_type const* state, string_type const& name
          , string_type const& patterndef) {}

        void clear(char_type const* state) {}

        std::size_t add_state(char_type const* state)
        {
            return 0;
        }
        string_type initial_state() const 
        { 
            return lex::lexertl::static_::lexer_state_names[0];
        }

        // register a semantic action with the given id
        template <typename F>
        void add_action(id_type id, std::size_t state, F act) 
        {
            // If you get compilation errors below stating value_type not being
            // a member of boost::fusion::unused_type, then you are probably
            // using semantic actions in your token definition without 
            // the static_actor_lexer being specified as the base class of your 
            // lexer (instead of the static_lexer class).
            typedef typename Functor::semantic_actions_type::value_type
                value_type;

            actions.insert(value_type(std::make_pair(id, state), act));
        }

        bool init_dfa() const { return true; }

    private:
        typename Functor::semantic_actions_type actions;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The static_actor_lexer class is another implementation of a 
    //  Spirit.Lex lexer on top of Ben Hanson's lexertl library as outlined 
    //  above (For more information about lexertl go here: 
    //  http://www.benhanson.net/lexertl.html).
    //
    //  Just as the static_lexer class it is meant to be used with 
    //  a statically generated lexer as outlined above.
    //
    //  The only difference to the static_lexer class above is that 
    //  token_def definitions may have semantic (lexer) actions attached while 
    //  being defined:
    //
    //      int w;
    //      token_def<> word = "[^ \t\n]+";
    //      self = word[++ref(w)];        // see example: word_count_lexer
    //
    //  This class is supposed to be used as the first and only template 
    //  parameter while instantiating instances of a lex::lexer class.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename Token = token<>
      , typename Iterator = typename Token::iterator_type
      , typename Functor = static_functor<Token, Iterator, mpl::true_>
      , typename TokenSet = lex::token_set<static_token_set<Token, Iterator> > >
    class static_actor_lexer 
      : public static_lexer<Token, Iterator, Functor, TokenSet>
    {
    protected:
        // Lexer instances can be created by means of a derived class only.
        static_actor_lexer(unsigned int flags) 
          : static_lexer<Token, Iterator, Functor, TokenSet>(flags) {}
    };

}}}}

#endif
