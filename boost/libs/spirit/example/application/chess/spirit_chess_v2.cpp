//
//   A simple chess parser for Spirit.
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
//  
//  #define BOOST_SPIRIT_DEBUG  ///$$$ DEFINE THIS WHEN DEBUGGING $$$///

#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols/symbols.hpp>
#include <boost/spirit/utility/lists.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////
using namespace boost::spirit;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//  Start grammar definition
//----------------------------------------------------------------------------

struct chess_grammar :
    public grammar<chess_grammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(chess_grammar const& self)
        {
        
            //-----------------------------------------------------------------
            // KEYWORDS
            //-----------------------------------------------------------------
//  Only two keywords : resign or draw ... :-) 
            
            keywords =
                "res" ,  "draw"  ;             
        
            //-----------------------------------------------------------------
            // OPERATORS
            //-----------------------------------------------------------------

//  No operators are used in this grammar. 


            //---------------------------------------
            // TOKENS
            //---------------------------------------
            typedef inhibit_case<strlit<> > token_t;

            token_t RESIGN     = nocase_d["res"];
            token_t DRAW    = nocase_d["draw?"];

                  
          //-----------------------------------------
          //	Start grammar definition
          //-----------------------------------------

          //-----------------------------------------
          // RULES
          //-----------------------------------------

          //  Now - the actual BNF grammar for the parser

  game =
  +(moves) ; 
         
  moves 
  = longest_d[ ( pawn_move | ordinary_pawn_move | pawn_capture 
     | ordinary_pawn_capture | en_passant_capture 
     | pawn_capture_and_promotion
     | piece_move | ordinary_piece_move | piece_capture 
     | specifying_move | resign | promotion | offer_draw 
     | q_castle | k_castle ) ];


//  Pawn moves and captures
  pawn_move = ( ordinary_pawn_move 
   | pawn_capture
   | promotion
   | pawn_capture_and_promotion ) ; 

  ordinary_pawn_move = lexeme_d[ range_p('a', 'h') 
          >> range_p('1', '8') ] ;             
  
  pawn_capture 
  = ( ordinary_pawn_capture 
     | en_passant_capture ) ;
  
  ordinary_pawn_capture =  lexeme_d[ range_p('a', 'h')  
     >>  ch_p('x') 
     >>  range_p('a', 'h') >> range_p('1', '8') ];     

//  An "en passant" capture is when a pawn captures
//  another pawn (which moved two squares in its first
//  move), **as if** it had only moved ONE square.  
//  This is the only move where the captured piece is
//  not on the "destination square".
   
  en_passant_capture =  lexeme_d[ range_p('a', 'h')  
     >>  ch_p('x') 
     >>  range_p('a', 'h') 
     >>  ( ch_p('6') | ch_p('3') ) >> str_p("ep") ];   
     
  promotion 
     = lexeme_d[ range_p('a', 'h') 
     >>   ch_p('8') 
     >> ( ch_p('B')  
        | ch_p('N') 
        | ch_p('Q')
        | ch_p('R') ) ];    
         
// It is possible for a pawn to do a capture AND be promoted at 
// the same time. We cover that possibility here. 
   pawn_capture_and_promotion
   =  lexeme_d[ range_p('a', 'h')  
     >>  ch_p('x') 
     >>  range_p('a', 'h') >> ch_p('8') 
     >> ( ch_p('B')  
        | ch_p('N') 
        | ch_p('Q')
        | ch_p('R') ) ];  

         
// Piece moves and captures  
  piece_move = ( ordinary_piece_move | piece_capture );     
  
  ordinary_piece_move 
  =     lexeme_d[ ( ch_p('B') 
                  | ch_p('K')
                  | ch_p('N') 
                  | ch_p('Q')
                  | ch_p('R') )
   >>  range_p('a', 'h') >> range_p('1', '8') ];         
       
  piece_capture 
  =     lexeme_d[ ( ch_p('B') 
                  | ch_p('K')
                  | ch_p('N') 
                  | ch_p('Q')
                  | ch_p('R') )
     >>  ch_p('x') 
     >>  range_p('a', 'h') >> range_p('1', '8') ];        
       
       
// Specifying move. Sometimes, you may have two pieces of the same 
// type that can move to the same square. This means that you must 
// specify which one you are moving. This is done by giving either
// the rank or file of the piece being moved.
// For example,  N4e6 (which specifies the rank), 
// Rac3 (which specifies the file), and so on. 
// NOTE - I have included the bishop here. It is only possible to 
// have two bishops that attack the same square if one of them is 
// a promoted pawn.  

     specifying_move 
     = lexeme_d[ ( ch_p('B')  
        | ch_p('N') 
        | ch_p('Q')
        | ch_p('R') )
  >>  (  range_p('a','h') | range_p('1','8') )
  >>     range_p('a','h') >> range_p('1','8')  ] ;      
                                                               
   resign = RESIGN ; 
   offer_draw = DRAW ;  
          

//  Castle on the Queen's side            
    q_castle = str_p("O-O-O") ; 
            
//  Castle on the King's side
    k_castle = str_p("O-O") ;             
                                                                                                                                                                                                                                                                                                                                                                
    }


      rule<ScannerT> const&
        start() const { return game; }
         
         
//  Declare the keywords here         

  symbols<> keywords;
        
  rule<ScannerT> 
  game, moves, pawn_move, ordinary_pawn_move, pawn_capture, 
  ordinary_pawn_capture, en_passant_capture, promotion, 
  pawn_capture_and_promotion, 
   piece_move,  ordinary_piece_move, piece_capture,  
  specifying_move, resign,  offer_draw,  
  q_castle, k_castle ;      
                  
    };
};


int
main(int /*argc*/, char* /*argv[]*/)
{
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\t A chess parser using Spirit ...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "Type a chess move \n" ; 
    cout << " (e.g.  d4 , a8Q, g3 , Nxe7, O-O-O, Bb5  ) \n" ;
    cout << " Type [z] or [Z] to quit\n\n" ;

    chess_grammar g;

    string str;
    while (getline(cin, str))
    {
        if (str[0] == 'z' || str[0] == 'Z' )
            break;

        if (parse(str.c_str(), g, space_p).full)
        {
            cout << "parsing succeeded\n";
        }
        else
        {
            cout << "parsing failed\n";
        }
    }

    cout << "Bye... :-) \n\n";
    return 0;
}



