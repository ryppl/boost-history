/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/lexical_cast.hpp>
#include "template.hpp"
#include "phrase_actions.hpp"
#include "grammar.hpp"
#include "state.hpp"
#include "utils.hpp"

#ifdef BOOST_MSVC
#pragma warning(disable : 4355)
#endif

namespace quickbook
{
    namespace
    {
        int callout_id = 0;
    }

    struct template_symbol
    {
        template_symbol(
                std::string const& identifier,
                std::vector<std::string> const& params,
                bool is_block,
                template_value const& body,
                quickbook::callouts const& callouts,
                template_scope const* parent)
           : identifier(identifier)
           , params(params)
           , is_block(is_block)
           , body(body)
           , callouts(callouts)
           , parent(parent) {}

        std::string identifier;
        std::vector<std::string> params;
        bool is_block;
        template_value body;
        quickbook::callouts callouts;
        template_scope const* parent;
    };

    typedef boost::spirit::qi::symbols<char, template_symbol> template_symbols;    

    // template scope
    //
    // 1.4-: parent_scope is the previous scope on the stack
    //       (the template's dynamic parent).
    // 1.5+: parent_scope is the template's lexical parent.
    //
    // This means that a search along the parent_scope chain will follow the
    // correct lookup chain for that version of quickboook.
    //
    // symbols contains the templates defined in this scope.
    
    struct template_scope
    {
        template_scope() : parent_scope() {}
        template_scope const* parent_scope;
        template_symbols symbols;
        boost::scoped_ptr<template_scope> next;
    };

    template_stack::template_stack()
        : scope(template_stack::parser(*this))
        , top_scope(new template_scope())
    {
    }
    
    template_stack::~template_stack() {}
    
    template_symbol const* template_stack::prefix_find(iterator& first, iterator const& last) const
    {
        // search all scopes for the longest matching symbol.
        iterator found = first;
        template_symbol const* result = 0;
        for (template_scope const* i = top_scope.get(); i; i = i->parent_scope)
        {
            iterator iter = first;
            template_symbol const* symbol = i->symbols.prefix_find(iter, last);
            if(symbol && iter.base() > found.base())
            {
                found = iter;
                result = symbol;
            }
        }
        first = found;
        return result;
    }

    template_symbol const* template_stack::find(std::string const& symbol) const
    {
        for (template_scope const* i = top_scope.get(); i; i = i->parent_scope)
        {
            if (template_symbol const* ts = i->symbols.find(symbol.c_str()))
                return ts;
        }
        return 0;
    }

    template_symbol const* template_stack::find_top_scope(std::string const& symbol) const
    {
        return top_scope->symbols.find(symbol.c_str());
    }

    bool template_stack::add(
            define_template const& definition,
            template_scope const* parent)
    {
        BOOST_ASSERT(top_scope);

        if (this->find_top_scope(definition.id)) {
            return false;
        }

        std::string::const_iterator
            iter = definition.body.content.begin(),
            end = definition.body.content.end();
        while (iter != end && ((*iter == ' ') || (*iter == '\t')))
            ++iter; // skip spaces and tabs
        bool is_block = (iter != end) && ((*iter == '\r') || (*iter == '\n'));

        template_symbol ts(
            definition.id,
            definition.params,
            is_block,
            definition.body,
            definition.callouts,
            parent ? parent : top_scope.get());

        top_scope->symbols.add(ts.identifier.c_str(), ts);
        
        return true;
    }

    void template_stack::push()
    {
        boost::scoped_ptr<template_scope> new_scope(
            new template_scope());
        new_scope->parent_scope = top_scope.get();

        new_scope->next.swap(top_scope);
        new_scope.swap(top_scope);
    }

    void template_stack::pop()
    {
        boost::scoped_ptr<template_scope> popped_scope;
        popped_scope.swap(top_scope);
        popped_scope->next.swap(top_scope);
    }

    namespace
    {
        std::string::size_type find_bracket_end(std::string const& str, std::string::size_type pos)
        {
            unsigned int depth = 1;

            while(depth > 0) {
                pos = str.find_first_of("[]\\", pos);
                if(pos == std::string::npos) return pos;

                if(str[pos] == '\\')
                {
                    pos += 2;
                }
                else
                {
                    depth += (str[pos] == '[') ? 1 : -1;
                    ++pos;
                }
            }

            return pos;
        }

        std::string::size_type find_first_seperator(std::string const& str)
        {
            if(qbk_version_n < 105) {
                return str.find_first_of(" \t\r\n");
            }
            else {
                std::string::size_type pos = 0;

                while(true)
                {
                    pos = str.find_first_of(" \t\r\n\\[", pos);
                    if(pos == std::string::npos) return pos;

                    switch(str[pos])
                    {
                    case '[':
                        pos = find_bracket_end(str, pos + 1);
                        break;
                    case '\\':
                        pos += 2;
                        break;
                    default:
                        return pos;
                    }
                }
            }
        }
    
        bool break_arguments(
            std::vector<template_value>& args
          , std::vector<std::string> const& params
          , file_position const& pos
        )
        {
            // Quickbook 1.4-: If there aren't enough parameters seperated by
            //                 '..' then seperate the last parameter using
            //                 whitespace.
            // Quickbook 1.5+: If '..' isn't used to seperate the parameters
            //                 then use whitespace to separate them
            //                 (2 = template name + argument).

            if (qbk_version_n < 105 || args.size() == 1)
            {
                while (args.size() < params.size() )
                {
                    // Try to break the last argument at the first space found
                    // and push it into the back of args. Do this
                    // recursively until we have all the expected number of
                    // arguments, or if there are no more spaces left.

                    template_value& str = args.back();
                    std::string::size_type l_pos = find_first_seperator(str.content);
                    if (l_pos == std::string::npos)
                        break;
                    template_value first(
                        str.position,
                        std::string(str.content.begin(), str.content.begin() + l_pos)
                        );
                    std::string::size_type r_pos = str.content.find_first_not_of(" \t\r\n", l_pos);
                    if (r_pos == std::string::npos)
                        break;
                    // TODO: Work out position?
                    template_value second(
                        str.position,
                        std::string(str.content.begin()+r_pos, str.content.end())
                    );
                    str = first;
                    args.push_back(second);
                }
            }

            if (args.size() != params.size())
            {
                detail::outerr(pos.file, pos.line)
                    << "Invalid number of arguments passed. Expecting: "
                    << params.size()
                    << " argument(s), got: "
                    << args.size()
                    << " argument(s) instead."
                    << std::endl;
                return false;
            }
            return true;
        }

        std::pair<bool, std::vector<std::string>::const_iterator>
        get_arguments(
            std::vector<template_value>& args
          , std::vector<std::string> const& params
          , template_scope const& scope
          , file_position const& pos
          , quickbook::state& state
        )
        {
            std::vector<template_value>::const_iterator arg = args.begin();
            std::vector<std::string>::const_iterator tpl = params.begin();
            std::vector<std::string> empty_params;

            // Store each of the argument passed in as local templates:
            while (arg != args.end())
            {
                if (!state.templates.add(
                        define_template(*tpl, empty_params, *arg),
                        &scope))
                {
                    detail::outerr(pos.file,pos.line)
                        << "Duplicate Symbol Found" << std::endl;
                    ++state.error_count;
                    return std::make_pair(false, tpl);
                }
                ++arg; ++tpl;
            }
            return std::make_pair(true, tpl);
        }

        bool parse_template(
            bool is_block
          , std::string body
          , std::string& result
          , file_position const& template_pos
          , bool template_escape
          , quickbook::state& state
        )
        {
            // How do we know if we are to parse the template as a block or
            // a phrase? We apply a simple heuristic: if the body starts with
            // a newline, then we regard it as a block, otherwise, we parse
            // it as a phrase.
            
            bool r = false;

            if (template_escape)
            {
                //  escape the body of the template
                //  we just copy out the literal body
                result = body;
                r = true;
            }
            else if (!is_block)
            {
                quickbook::actions actions(state);
                quickbook_grammar g(actions);

                //  do a phrase level parse
                iterator first(body.begin(), body.end(), state.filename.native().c_str());
                first.set_position(template_pos);
                iterator last(body.end(), body.end());
                r = boost::spirit::qi::parse(first, last, g.simple_phrase) && first == last;
                std::string phrase;
                state.phrase.swap(phrase);
                result = phrase;
            }
            else
            {
                quickbook::actions actions(state);
                quickbook_grammar g(actions);

                //  do a block level parse
                //  ensure that we have enough trailing newlines to eliminate
                //  the need to check for end of file in the grammar.
                body += "\n\n";

                iterator first(body.begin(), body.end(), state.filename.native().c_str());
                first.set_position(template_pos);
                iterator last(body.end(), body.end());

                while (first != last && ((*first == ' ') || (*first == '\t')))
                    ++first; // skip spaces and tabs
                while (first != last && ((*first == '\r') || (*first == '\n')))
                    ++first; // skip initial newlines

                r = boost::spirit::qi::parse(first, last, g.block) && first == last;
                state.paragraph_output();
                std::string block;
                state.block.swap(block);
                result = block;
            }
            
            return r;
        }
    }

    void process(quickbook::state& state, call_template const& x)
    {
        ++state.template_depth;
        if (state.template_depth > state.max_template_depth)
        {
            detail::outerr(x.position.file, x.position.line)
                << "Infinite loop detected" << std::endl;
            --state.template_depth;
            ++state.error_count;
            return;
        }

        // The template arguments should have the scope that the template was
        // called from, not the template's own scope.
        //
        // Note that for quickbook 1.4- this value is just ignored when the
        // arguments are expanded.
        template_scope const& call_scope = *state.templates.top_scope;

        std::string result;
        state.push(); // scope the state
        {
            // Store the current section level so that we can ensure that
            // [section] and [endsect] tags in the template are balanced.
            state.min_section_level = state.section_level;

            // Quickbook 1.4-: When expanding the tempalte continue to use the
            //                 current scope (the dynamic scope).
            // Quickbook 1.5+: Use the scope the template was defined in
            //                 (the static scope).
            if (qbk_version_n >= 105)
                state.templates.top_scope->parent_scope = x.symbol->parent;

            std::vector<template_value> args = x.args;
    
            ///////////////////////////////////
            // Initialise the arguments
            
            if (!x.symbol->callouts.size())
            {
                // Break the arguments for a template
            
                if (!break_arguments(args, x.symbol->params, x.position))
                {
                    state.pop(); // restore the state
                    --state.template_depth;
                    ++state.error_count;
                    return;
                }
            }
            else
            {
                if (!args.empty())
                {
                    detail::outerr(x.position.file, x.position.line)
                        << "Arguments for code snippet."
                        <<std::endl;
                    ++state.error_count;

                    args.clear();
                }

                BOOST_ASSERT(x.symbol->params.size() == x.symbol->callouts.size());
                unsigned int size = x.symbol->callouts.size();

                for(unsigned int i = 0; i < size; ++i)
                {
                    template_value value;
                    value.content = "[[callout]" + x.symbol->callouts[i].role + " " +
                        state.doc_id.value +
                        boost::lexical_cast<std::string>(callout_id + i) +
                        "]";
                    args.push_back(value);
                }
            }

            ///////////////////////////////////
            // Prepare the arguments as local templates
            bool get_arg_result;
            std::vector<std::string>::const_iterator tpl;
            boost::tie(get_arg_result, tpl) =
                get_arguments(args, x.symbol->params,
                    call_scope, x.position, state);

            if (!get_arg_result)
            {
                state.pop(); // restore the state
                --state.template_depth;
                return;
            }

            ///////////////////////////////////
            // parse the template body:

            if (!parse_template(x.symbol->is_block, x.symbol->body.content, result, x.symbol->body.position, x.escape, state))
            {
                detail::outerr(x.position.file,x.position.line)
                    << "Expanding template:" << x.symbol->identifier << std::endl
                    << std::endl
                    << "------------------begin------------------" << std::endl
                    << x.symbol->body.content
                    << "------------------end--------------------" << std::endl
                    << std::endl;
                state.pop(); // restore the state
                --state.template_depth;
                ++state.error_count;
                return;
            }

            if (state.section_level != state.min_section_level)
            {
                detail::outerr(x.position.file,x.position.line)
                    << "Mismatched sections in template " << x.symbol->identifier << std::endl;
                state.pop(); // restore the actions' states
                --state.template_depth;
                ++state.error_count;
                return; 
            }
        }

        state.pop(); // restore the state
        --state.template_depth;

        if(x.symbol->callouts.size()) {
            callout_list list;
            BOOST_FOREACH(callout_source const& c, x.symbol->callouts) {
                callout_item item;
                item.identifier = state.doc_id.value +
                        boost::lexical_cast<std::string>(callout_id++);
                
                state.push();
                // TODO: adjust the position?
                bool r = parse_template(true, c.body.content, item.content, c.body.position, false, state);
                state.pop();

                if(!r)
                {
                    detail::outerr(c.body.position.file, c.body.position.line)
                        << "Error expanding callout."
                        << std::endl;
                    ++state.error_count;
                    return;
                }

                list.push_back(item);
            }

            state.push();
            {
                quickbook::actions actions(state);
                actions.process(list);
            }
            result += state.block.str();
            state.pop();
        }

        if(x.symbol->is_block) {
            state.paragraph_output();
            state.block << result;
        }
        else {
            state.phrase << result;
        }
        
        return;
    }
}
