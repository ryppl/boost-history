// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef DEFAULT_ERROR_POLICY_HPP
#define DEFAULT_ERROR_POLICY_HPP 1

#include "boost/socket/socket_errors.hpp"
#include "boost/socket/socket_exception.hpp"

#include <iostream>

namespace boost
{
  namespace socket
  {

    struct default_error_policy
    {
      int handle_error(function::name fn, int error)
      {
        std::cerr << "Error in function " << fn << std::endl;

        switch (error)
        {
          case WouldBlock :
            return WouldBlock;
          case error::address_already_in_use :
            throw socket_exception("Address already in use",error);
          case error::address_family_not_supported_by_protocol_family :
            throw socket_exception("Address family not supported by protocol family",error);
          case error::address_not_available :
            throw socket_exception("Address not available",error);
          case error::bad_address :
            throw socket_exception("Bad address",error);
          case error::bad_protocol_option :
            throw socket_exception("Bad protocol option",error);
          case error::cannot_assign_requested_address :
            throw socket_exception("Cannot assign requested address",error);
          case error::cannot_send_after_socket_shutdown:
            throw socket_exception("Can't send after socket shutdown",error);
          case error::connection_aborted :
            throw socket_exception("Connection aborted",error);
          case error::connection_already_in_progress :
            throw socket_exception("Connection already in progress",error);
          case error::connection_refused :
            throw socket_exception("Connection refused",error);
          case error::connection_reset_by_peer :
            throw socket_exception("Connection reset by peer",error);
          case error::connection_timed_out :
            throw socket_exception("Connection timed out",error);
          case error::destination_address_required :
            throw socket_exception("Destination address required",error);
          case error::graceful_shutdown_in_progress :
            throw socket_exception("Graceful shutdown in progress",error);
          case error::host_is_down :
            throw socket_exception("Host is down",error);
          case error::host_is_unreachable :
            throw socket_exception("Host is unreachable",error);
          case error::host_not_found :
            throw socket_exception("Host not found",error);
          case error::insufficient_memory_available :
            throw socket_exception("Insufficient memory available",error);
          case error::interrupted_function_call :
            throw socket_exception("Interrupted function call",error);
          case error::invalid_argument :
            throw socket_exception("Invalid argument",error);
          case error::message_too_long :
            throw socket_exception("Message too long",error);
          case error::net_reset :
            throw socket_exception("net reset",error);
          case error::network_dropped_connection_on_reset :
            throw socket_exception("Network dropped connection on reset",error);
          case error::network_interface_is_not_configured :
            throw socket_exception("Network interface is not configured",error);
          case error::network_is_down :
            throw socket_exception("Network is down",error);
          case error::network_is_unreachable :
            throw socket_exception("Network is unreachable",error);
          case error::network_subsystem_is_unavailable :
            throw socket_exception("Network subsystem is unavailable",error);
          case error::no_buffer_space_available :
            throw socket_exception("No buffer space available",error);
          case error::no_route_to_host :
            throw socket_exception("No route to host",error);
          case error::nonauthoritative_host_not_found :
            throw socket_exception("Nonauthoritative host not found",error);
          case error::not_a_valid_descriptor :
            throw socket_exception("not a valid descriptor",error);
          case error::one_or_more_parameters_are_invalid :
            throw socket_exception("One or more parameters are invalid",error);
          case error::operation_already_in_progress :
            throw socket_exception("Operation already in progress",error);
          case error::operation_not_supported :
            throw socket_exception("Operation not supported",error);
          case error::operation_not_supported_on_transport_endpoint :
            throw socket_exception("Operation not supported on transport endpoint",error);
          case error::operation_now_in_progress :
            throw socket_exception("Operation now in progress",error);
          case error::overlapped_operation_aborted :
            throw socket_exception("Overlapped operation aborted",error);
          case error::permission_denied:
            throw socket_exception("Permission denied",error);
          case error::protocol_family_not_supported :
            throw socket_exception("Protocol family not supported",error);
          case error::protocol_not_available :
            throw socket_exception("Protocol not available",error);
          case error::protocol_wrong_type_for_socket :
            throw socket_exception("Protocol wrong type for socket",error);
          case error::socket_is_already_connected :
            throw socket_exception("Socket is already connected",error);
          case error::socket_is_not_connected :
            throw socket_exception("Socket is not connected",error);
          case error::socket_operation_on_nonsocket :
            throw socket_exception("Socket operation on nonsocket",error);
          case error::socket_type_not_supported :
            throw socket_exception("Socket type not supported",error);
          case error::software_caused_connection_abort :
            throw socket_exception("Software caused connection abort",error);
          case error::specified_event_object_handle_is_invalid :
            throw socket_exception("Specified event object handle is invalid",error);
          case error::system_call_failure :
            throw socket_exception("System call failure",error);
          case error::this_is_a_nonrecoverable_error :
            throw socket_exception("This is a nonrecoverable error",error);
          case error::too_many_open_files :
            throw socket_exception("Too many open files",error);
          case error::too_many_processes :
            throw socket_exception("Too many processes",error);
          case error::unknown_protocol :
            throw socket_exception("Unknown protocol",error);
          default:
            throw socket_exception("Unknown error",error);
        }
      }
    };

  }// namespace
}// namespace

#endif
