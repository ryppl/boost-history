
//   "Spirit chess" - A simple chess parser for Spirit.
//   Requires -  Spirit version 1.6.0 or later
//
//   Written by:  Andy Elvey
//  
//   This parser is released under the Boost licence, a copy of 
//   which is reproduced below.
//
//  ========================================================================= 
//    Permission to copy, use, modify, sell and distribute this software is
//    granted provided this copyright notice appears in all copies. This
//    software is provided "as is" without express or implied warranty, and
//    with no claim as to its suitability for any purpose.
//  =========================================================================  

//  *******************************************
    Notes about the parser - 

    The parser is able to parse pretty much all types of moves in 
a chess game. This includes moves where two (or more) pieces of the
same type are able to move to the same square (and so the piece moved
needs to be specified).  In other words, moves like - 

    N4e6 (which specifies the rank)
    Rac3 (which specifies the file), and so on.

Pawn promotions are also parsed correctly. 

 There are several areas that are not included in the parser. These are as
follows - 
a) The notation for "check" (+) and "mate" (#).  These are the *result*
of a move, rather than being part of the move itself. 
b) Stalemate. Same as (a) - again, the result of a move. 
c) The notation for good and bad moves (!, ?, !!, ?!, !?, ??). These 
are opinions of a given move, rather than being part of the move.  

 Anyway, the parser was a lot of fun to do, and is an interesting 
little example that shows what Spirit can do. Enjoy .... :-) 

- Andy Elvey


 
