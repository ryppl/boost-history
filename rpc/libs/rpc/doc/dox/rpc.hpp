namespace boost {

/** \mainpage


\section sec_contents Table of Contents

- \ref sec_server_side
 - \ref sec_registry
 - \ref sec_server
- \ref sec_client_side
 - \ref sec_client
 - \ref sec_call
 - \ref sec_making_call
- \ref sec_example

\section sec_server_side Server-side Components

\subsection sec_registry Registry

rpc::registry contains a collection of functions mapped by key.

\dontinclude rpc_example.cpp
\skip void nothing
\until add2);

\subsection sec_server Server

rpc::server will wait for incoming connections using a specified
connection protocol.  Upon making a successful connection, it will
assign it a dedicated rpc::registry_server object.

\dontinclude rpc_example.cpp
\skip create a server
\until ;

rpc::simple_acceptor just grants a connection to anyone that asks for
it.  The Acceptor is a template parameter, so different
connection/authentication protocols could be used (although they
should probably be in a different library, since they are not
necessarily specific to RPC).

\section sec_client_side Client-side components

\subsection sec_client Client

rpc::client negotiates a connection to the server and can service calls to the
registry on the server side.

\dontinclude rpc_example.cpp
\skip create a client
\until ;

\subsection sec_call Call

rpc::call prepares a call by storing the function id and serializing the parameters.

\dontinclude rpc_example.cpp
\skip // prepare some function calls
\until 3, i);

\subsection sec_making_call Making a Call and Getting the Results

A call is given to the client by passing the function id and parameters
through the rpc::call class.  The call will return a call handler which
can be used to get the return result or verify that the function has completed.

The behavior is currently specified as follows:
 - if there are no "out" arguments (currently, any non-const references), and the returned
   call handler is not stored, nothing will be marshaled back.
 - if there are no "out" arguments, and the return handler is stored into an rpc::acknowledgement,
   only an acknowledgement that the call has completed will be marshaled back.
 - if there are no "out" arguments, and the returned call handler is stored into either an
   rpc::handler or cast into an appropriate future, only the return value will be marshaled back
 - if there are "out" arguments, everything will be marshaled back.

In the future, the marshaling of return values/completion acknowledgement and "out" arguments will
be separated.

\dontinclude rpc_example.cpp
\skip // make some function calls
\until }

\section sec_example Example Code

\dontinclude rpc_example.cpp
\skip <boost
\until end void network_marshal_test
*/

}