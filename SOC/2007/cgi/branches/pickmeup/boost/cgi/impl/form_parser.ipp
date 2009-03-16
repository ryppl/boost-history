//                  -- form_parser.ihpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_FORM_PARSER_IPP_INCLUDED__
#define CGI_DETAIL_FORM_PARSER_IPP_INCLUDED__

#include "boost/cgi/error.hpp"
#include "boost/cgi/basic_client.hpp"
#include "boost/cgi/detail/url_decode.hpp"
#include "boost/cgi/common/form_parser.hpp"
#include "boost/cgi/common/source_enums.hpp"

#include <iostream> // **FIXME**

namespace cgi {
 namespace detail {

/*
    BOOST_CGI_INLINE
    form_parser::form_parser (
    )
      : content_type_(env_vars(impl.vars_)["CONTENT_TYPE"])
      , buffer_(impl.buffer_)
      , bytes_left_(impl.client_.bytes_left_)
      //, stdin_data_read_(impl.stdin_data_read_)
      , offset_(0)
      , data_map_(post_vars(impl.vars_))
      , callback_()
    {
    }

    BOOST_CGI_INLINE
    form_parser::form_parser (
      , callback_type const& callback
    )
      : content_type_(env_vars(impl.vars_)["CONTENT_TYPE"])
      , buffer_(impl.buffer_)
      , bytes_left_(impl.client_.bytes_left_)
      //, stdin_data_read_(impl.stdin_data_read_)
      , offset_(0)
      , data_map_(post_vars(impl.vars_))
      , callback_(callback)
    {
    }
*/

    BOOST_CGI_INLINE
    form_parser::form_parser (
        buffer_type& buf
      , common::post_map& data_map
      , form_parser::string_type const& ct
      , callback_type const& callback
      , std::size_t& bytes_left
      , bool& stdin_parsed
    )
      : content_type_(ct)//env_vars(impl.vars_)["CONTENT_TYPE"])
      , buffer_(buf)
      , bytes_left_(bytes_left)
      , data_map_(data_map)
      , callback_(callback)
      , stdin_parsed_(stdin_parsed)
      , offset_(0)
    {
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse(boost::system::error_code& ec)
    {
      BOOST_ASSERT(!content_type_.empty());

      if (boost::algorithm::ifind_first(content_type_,
            "application/x-www-form-urlencoded"))
      {
        parse_url_encoded_form(ec);
      }
      else
      if (boost::algorithm::ifind_first(content_type_,
            "multipart/form-data"))
      {
        parse_multipart_form(ec);
      }
      else
        return ec = error::invalid_form_type;

      return ec;
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_url_encoded_form(boost::system::error_code& ec)
    {
      std::string name;
      std::string str;

      char ch;
      char ch1;
      while( bytes_left_ )
      {
        ch = getchar();
        --bytes_left_;

        switch(ch)
        {
        case '%': // unencode a hex character sequence
          if (bytes_left_ >= 2)
          {
            ch = getchar();
            ch1 = getchar();
            if (std::isxdigit(ch) && std::isxdigit(ch1))
            {
              str.append(1, detail::hex_to_char(ch, ch1));
            }
            else // we don't have a hex sequence
            { // **FIXME** isn't this broken? What about '%%53' ? - use url_decode instead!
              str.append(1, '%').append(1, ch).append(1, ch1);
            }
            bytes_left_ -= 2;
          }
          else // There aren't enough characters to make a hex sequence
          {
            str.append(1, '%');
            --bytes_left_;
          }
          break;
        case '+':
            str.append(1, ' ');
            break;
        case ' ': // skip spaces
            break;
        case '=': // the name is complete, now get the corresponding value
            name.swap(str);
            break;
        case '&': // we now have the name/value pair, so save it
            // **FIXME** have to have .c_str() ?
            data_map_[name.c_str()] = str;
            str.clear();
            name.clear();
           break;
        default:
            str.append(1, ch);
        }
      }
      // save the last param (it won't have a trailing &)
      if( !name.empty() )
          // **FIXME** have to have .c_str() ?
          data_map_[name.c_str()] = str;

      return ec;
    }

    /// Parse a multipart form.
    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_multipart_form(boost::system::error_code& ec)
    {
      parse_boundary_marker(ec);
      std::cerr<< "Parsed boundary marker" << std::endl;
      move_to_start_of_first_part(ec);
      std::cerr<< (ec ? "Error finding first multipart section" : "Moved to start of actual data") << std::endl;

      if (ec && ec != boost::asio::error::eof)
        return ec;

      do {
        parse_form_part(ec);
      }while( //!impl_.stdin_parsed_
            bytes_left_ // != 0
           );//&& ec != boost::asio::error::eof );

      return ec;
    }


    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_form_part(boost::system::error_code& ec)
    {
      if (!parse_form_part_meta_data(ec)
      &&  !parse_form_part_data(ec))
        return ec;

      return ec;
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_form_part_data(boost::system::error_code& ec)
    {
      std::cerr<< "In form_parser::parse_form_part_data" << std::endl;
      std::string regex("^(.*?)" // the data
                        "\\x0D\\x0A" // CR LF
                        "--" "(");
      if (boundary_markers.size() > 1)
      {
        std::list<std::string>::iterator i(boundary_markers.begin());
        regex = regex + "(?:" + *i + ")";
        ++i;
        for(; i != boundary_markers.end(); ++i)
        {
          regex = regex + "|(?:" + *i + ")";
        }
      }
      else
      {
        regex += *boundary_markers.begin();
      }

      regex += ")(--)?[ ]*(?:\\x0D\\x0A)?";
      boost::regex re(regex);

      typedef buffer_type::iterator buffer_iter;

      boost::match_results<buffer_iter> matches;

      std::size_t offset = offset_;

      //int runs = 0;
      buffer_iter begin(buffer_.begin() + offset);
      buffer_iter end(buffer_.end());
      std::cerr<< "Current buffer (a) :{ " << std::string(begin,end) << " }: " << std::endl;

      for(;;)
      {
        if (!boost::regex_search(begin, end, matches, re
                                , boost::match_default
                                | boost::match_partial))
        {
          return boost::system::error_code(345, boost::system::system_category);
        }
        else
        {
          if (matches[1].matched)
          {
            form_parts_.back().buffer_
             // = boost::range_iterator<;
             = std::make_pair(matches[1].first, matches[1].second);
            // **FIXME**
            data_map_[form_parts_.back().name.c_str()] = matches[1];
            //std::ofstream of("c:/cc/log/post_vars.log");
            //of<< "var == " << matches[1] << std::endl;
            offset_ = offset + matches[0].length();
            pos_ = matches[0].second;

            if (matches[3].matched)
            {
              bytes_left_ = 0; // stop reading completely.
              //impl_.stdin_parsed_ = true;
            }
            return ec;
          }
          else
          {
            std::size_t bytes_read
                = callback_(ec);
                            //impl_.client_.read_some(prepare(64), ec);

            if (bytes_read == 0 && bytes_left_ == 0) // **FIXME**
            {
              //stdin_data_read_ = true;
              return ec;
            }

            begin = buffer_.begin() + offset;
            end = buffer_.end();

            if (ec)
              return ec;
          }
        }
      }

      return ec;
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_form_part_meta_data(boost::system::error_code& ec)
    {
      // Oh dear this is ugly. The move to Boost.Spirit will have to be sooner than planned.
      // (it's a nested, recursive pattern, which regexes don't suit, apparently)
      boost::regex re(  "(?:"         // [IGNORE] the line may be empty, as meta-data is optional
                          "^"
                          "([-\\w]+)" // name
                          ":[ ^]*"       // separator
                          "([-/\\w]+)" // optional(?) value
                          ""
                          "(?:"
                            ";"
                            "[ ]*"    // additional name/value pairs (don't capture)
                            "([-\\w]+)" // name
                            "[ \\x0D\\x0A]*=[ \\x0D\\x0A]*"       // separator
                            "(?:\"?([-.\\w]*)\"?)" // value may be empty
                          ")?"
                          "(?:"
                            ";"
                            "[ ]*"    // additional name/value pairs (don't capture)
                            "([-\\w]+)" // name
                            "[ \\x0D\\x0A]*=[ \\x0D\\x0A]*"       // separator
                            "(?:\"?([-.\\w]*)\"?)" // value may be empty
                          ")?"        // mark the extra n/v pairs optional
                          "(?:\\x0D\\x0A)"
                        ")"
                        "(?:"
                          "([-\\w]+)" // name
                          ":[ ^]*"       // separator
                          "([-/\\w]+)" // optional(?) value
                          ""
                          "(?:"
                            ";"
                            "[ ]*"    // additional name/value pairs (don't capture)
                            "([-\\w]+)" // name
                            "[ \\x0D\\x0A]*=[ \\x0D\\x0A]*"       // separator
                            "(?:\"?([-.\\w]*)\"?)" // value may be empty
                          ")?"
                          "(?:"
                            ";"
                            "[ ]*"    // additional name/value pairs (don't capture)
                            "([-\\w]+)" // name
                            "[ \\x0D\\x0A]*=[ \\x0D\\x0A]*"       // separator
                            "(?:\"?([-.\\w]*)\"?)" // value may be empty
                          ")?"        // mark the extra n/v pairs optional
                          "(?:\\x0D\\x0A)"    // followed by the end of the line
                        ")?"
                      "(\\x0D\\x0A)");     // followed by the 'header termination' line
                      
      boost::regex re2("([^\\n\\r]+?)\\r\\n\\r\\n");

      typedef buffer_type::iterator buffer_iter;

      boost::match_results<buffer_iter> matches;

      std::size_t offset = offset_;
      pos_ = buffer_.begin();
      int runs = 0;

      std::size_t bytes_read = 0;
      for(;;)
      {
        buffer_iter begin(buffer_.begin() + offset);
        buffer_iter end(buffer_.end());
        std::cerr<< "Current buffer (b) :{ " << std::string(begin,end) << " }: " << std::endl;

        if (!boost::regex_search(begin, end, matches, re2
                                , boost::match_default | boost::match_partial))
        {
          std::cerr<< "Read all stdin." << std::endl;
          stdin_parsed_ = true;
          return ec;
        }
        if (matches[0].matched)
        {
          common::form_part part;
          for ( unsigned int i = 1
              ; i < matches.size()
               && matches[i].matched
               && !matches[i].str().empty()
              ; i+=1)
          {
            if (matches[i].str() == "name")
            {
              part.name = matches[i+1];
            }
            else
            {
              part.meta_data_[matches[i]]
                = std::make_pair(matches[i+1].first, matches[i+1].second);
            }
            std::cerr<< "Adding form part with name :{ " << part.name << " }: " << std::endl;
            form_parts_.push_back(part);
         }
         
         std::cerr<< " num matches = " << matches.size() << std::endl;
         std::cerr<< "matches[13] :{ " << matches[13].str() << " }: " << std::endl;

         if (matches[13].str() == "\r\n")
         {
           offset_ = offset + matches[0].length();
           offset += matches[0].length();
           pos_ = matches[0].second;
           std::cerr<< "stuff :{ " << matches[0].str() << " }: " << std::endl;
           return ec;
         }
         else
         {
           std::cerr<< "oh no" << std::endl;
           throw std::runtime_error("Invalid POST data (header wasn't terminated as expected)");
         }

        }else{
          bytes_read = callback_(ec);
          if (ec) return ec;
          if (++runs > 40)
          {
            std::cerr<< "Done 40 runs; bailing out" << std::endl;
            break;
          }
       }
      }
      std::cerr<< " -- Done form part meta data. -- " << std::endl;
      return ec;
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::move_to_start_of_first_part(boost::system::error_code& ec)
    {
      //std::cerr<< "In func: " << boundary_markers.front() << std::endl;
      boost::regex re("(?:"//(?:.*?)?"   // optional leading characters
                      //"(?:\\x0D\\x0A)|^" // start of line
                      //"[\\x0D\\x0A^\\<]*?"
                      "("
                        "--" + boundary_markers.front() + // two dashes and our marker
                      ")"
                      "(--)?" // optional two dashes
                      "[ \\r\\n]*?\\r\\n)");
                                        // on the first marker.

      typedef buffer_type::iterator buffer_iter;
      std::cerr<< "Regex :{ " << re << " }: " << std::endl;

      boost::match_results<buffer_iter> matches;

      // get data into our buffer until we reach the first boundary marker.
      int runs = 0;
      std::size_t offset = 0;
      std::size_t bytes_read = 0;
      for(;;)
      {
        // The function object stored in `callback_` is used to read data.
        bytes_read = callback_(ec);
        
        std::cerr<< "Read " << bytes_read << " bytes from the socket (offset = " << offset << ")." << std::endl
                 << "Current buffer :{ " << std::string(buffer_.begin(), buffer_.end()) << " }: " << std::endl;

        //if (bytes_read == 0) ec = common::error::multipart_form_boundary_not_found;
        if (ec || bytes_read == 0) return ec;
        
        buffer_iter begin(buffer_.begin());// + offset);
        buffer_iter end(buffer_.end());
        if (!boost::regex_search(begin, end //impl.buffer_.begin(), impl.buffer_.end()
                                , matches, re, boost::match_default | boost::match_partial))
        {
          offset = buffer_.size();
          continue;
        }
        else
        {
          std::cerr<< "Matches :{ " << matches[0].str() << " }: " << std::endl;

          if (matches[1].matched)
          {
            buffer_.erase(buffer_.begin(), matches[0].second);
            offset_ = 0;
            pos_ = buffer_.begin();
            return ec;
          }
          else
          {
            if (++runs > 10)
              return ec = common::error::multipart_form_boundary_not_found;
            continue;
          }
        }
      }
      // skip that line and then erase the buffer
      return ec;
    }

    BOOST_CGI_INLINE
    boost::system::error_code
      form_parser::parse_boundary_marker(boost::system::error_code& ec)
    {
      //BOOST_ASSERT(!content_type.empty());

      // **FIXME** (don't use Boost.Regex)
      std::cerr<< " :{ " << content_type_ << " }: " << std::endl;
      /*
      std::string::size_type sz = content_type_.find("=");
      if (sz == std::string::npos)
        return boost::system::error_code(666, boost::system::system_category);
        
      std::string::size_type sz2 = content_type_.find("\r", sz);
      boundary_marker = content_type_.substr(sz+1);
      // New boundary markers are added to the front of the list.
      boundary_markers.push_front(boundary_marker);
      /*
      std::cerr<< "Need string:: " << content_type_.substr(sz+1) << " (" << sz << "," << (sz2) << ")";
      */
      boost::regex re("; ?boundary=\"?([^\"\\r]+)\"?(\\r\\n)?");
      boost::smatch match_results;
      if (!boost::regex_search(content_type_, match_results, re))
        return boost::system::error_code(666, boost::system::system_category);

      boundary_marker = match_results[1].str();
      // New boundary markers are added to the front of the list.
      boundary_markers.push_front(match_results[1].str());
      std::cerr<< "boundary marker :{ " << boundary_marker << " }: " << std::endl;

      return ec;
    }


 } // namespace detail
} // namespace cgi

#endif // CGI_DETAIL_FORM_PARSER_IPP_INCLUDED__

