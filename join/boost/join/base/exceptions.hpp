//
// boost/join/actor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_EXCEPTIONS_HPP
#define BOOST_JOIN_EXCEPTIONS_HPP

namespace boost {
  namespace join {

    class join_exception : public std::exception {
    public:
      virtual const char *what() const throw () {
        return "Join-related exception";
      }
    };
    class not_in_chord_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: async<>/synch<> methods not in chord (no method body) exception";
      }
    };
    class double_association_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: async<>/synch<> methods associated with more than one actor exception";
      }
    };
    class queue_overflow_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: queue overflow exception";
      }
    };
    class missing_result_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: synch<> method missing result exception";
      }
    };
    class no_executor_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: no executor exception";
      }
    };
    class hidden_chord_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: hidden chord exception";
      }
    };
    class too_many_ports_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: Too many ports defined in a actor";
      }
    };
    class chord_override_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: Chord override failure, chord not found";
      }
    };
    class chord_remove_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: Chord remove failure, chord not found";
      }
    };
    class executor_missing_rr_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: executor's dynamic task queues are used, however non round robin scheduling is defined";
      }
    };
    class synch_not_1st_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: synchronous method is used not as the first method of chord";
      }
    };
    class single_synch_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: more than one synchronous methods are used";
      }
    };
    class port_chord_reset_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: ports or actor are reset unexpectedly";
      }
    };
    class synch_time_out_exception : public join_exception {
    public:
      virtual const char *what() const throw () {
        return "Join: synchronous ports or methods time out unexpectedly";
      }
    };

  }
}

#endif
