#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include "external/kolmogorov/graph.h"
#include <sys/time.h>
#include <sys/resource.h>
float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

struct edge_vals{
  edge_vals():cap(0),rev_cap(0){}
  Graph::captype cap;
  Graph::captype rev_cap;
};
  
int read_dimacs_max_flow_kolmogorov(Graph& g, Graph::captype& flow_correction)
{
  const int ARC_FIELDS = 3;     /* no of fields in arc line  */
  const int NODE_FIELDS = 2;    /* no of fields in node line  */
  const int P_FIELDS = 3;       /* no of fields in problem line */
  const char* PROBLEM_TYPE = "max"; /* name of problem type*/

  long m, n,                    /*  number of edges and nodes */
  i, head, tail, src, sink;
  Graph::captype cap;

  std::vector<Graph::node_id> nodes;
  typedef std::map<std::pair<long, long>, edge_vals> tEdgeMap;
  tEdgeMap edges;
  
  long no_lines=0,              /* no of current input line */
  no_plines=0,                /* no of problem-lines */
  no_nslines=0,               /* no of node-source-lines */
  no_nklines=0,               /* no of node-source-lines */
  no_alines=0;                /* no of arc-lines */
  
  std::string in_line;          /* for reading input line */
  char pr_type[3];              /* for reading type of the problem */
  char nd;                      /* source (s) or sink (t) */
  
  int k,                        /* temporary */
  err_no;                     /* no of detected error */

  /* -------------- error numbers & error messages ---------------- */
  const int EN1   = 0;
  const int EN2   = 1;
  const int EN3   = 2;
  const int EN4   = 3;
  //  const int EN6   = 4;
  //  const int EN10  = 5;
  //  const int EN7   = 6;
  const int EN8   = 7;
  const int EN9   = 8;
  const int EN11  = 9;
  const int EN12 = 10;
  //  const int EN13 = 11;
  const int EN14 = 12;
  const int EN16 = 13;
  const int EN15 = 14;
  const int EN17 = 15;
  const int EN18 = 16;
  const int EN21 = 17;
  const int EN19 = 18;
  const int EN20 = 19;
  const int EN22 = 20;
  
  static char *err_message[] = 
  { 
    /* 0*/    "more than one problem line.",
    /* 1*/    "wrong number of parameters in the problem line.",
    /* 2*/    "it is not a Max Flow problem line.",
    /* 3*/    "bad value of a parameter in the problem line.",
    /* 4*/    "can't obtain enough memory to solve this problem.",
    /* 5*/    "more than one line with the problem name.",
    /* 6*/    "can't read problem name.",
    /* 7*/    "problem description must be before node description.",
    /* 8*/    "this parser doesn't support multiply sources and sinks.",
    /* 9*/    "wrong number of parameters in the node line.",
    /*10*/    "wrong value of parameters in the node line.",
    /*11*/    " ",
    /*12*/    "source and sink descriptions must be before arc descriptions.",
    /*13*/    "too many arcs in the input.",
    /*14*/    "wrong number of parameters in the arc line.",
    /*15*/    "wrong value of parameters in the arc line.",
    /*16*/    "unknown line type in the input.",
    /*17*/    "reading error.",
    /*18*/    "not enough arcs in the input.",
    /*19*/    "source or sink doesn't have incident arcs.",
    /*20*/    "can't read anything from the input file."
  };
  /* --------------------------------------------------------------- */

  /* The main loop:
  -  reads the line of the input,
  -  analyses its type,
  -  checks correctness of parameters,
  -  puts data to the arrays,
  -  does service functions
  */

  while (std::getline(std::cin, in_line)) {
    ++no_lines;

    switch (in_line[0]) {
      case 'c':                  /* skip lines with comments */
        case '\n':                 /* skip empty lines   */
          case '\0':                 /* skip empty lines at the end of file */
            break;
      
            case 'p':                  /* problem description      */
              if ( no_plines > 0 )
                /* more than one problem line */
              { err_no = EN1 ; goto error; }
      
              no_plines = 1;
      
              if (
                  /* reading problem line: type of problem, no of nodes, no of arcs */
                  sscanf ( in_line.c_str(), "%*c %3s %ld %ld", pr_type, &n, &m )
                  != P_FIELDS
                 )
                /*wrong number of parameters in the problem line*/
              { err_no = EN2; goto error; }
      
              if ( strcmp ( pr_type, PROBLEM_TYPE ) )
                /*wrong problem type*/
              { err_no = EN3; goto error; }
      
              if ( n <= 0  || m <= 0 )
                /*wrong value of no of arcs or nodes*/
              { err_no = EN4; goto error; }
              
//               if (n <=2){
//                 std::cerr << "Not enough nodes!" << std::endl;
//                 exit(1);
//               }
              //               
              {

              }
              break;
      
              case 'n':                    /* source(s) description */
                if ( no_plines == 0 )
                  /* there was not problem line above */
                { err_no = EN8; goto error; }
      
                /* reading source  or sink */
                k = sscanf ( in_line.c_str(),"%*c %ld %c", &i, &nd );
                --i; // index from 0
                if ( k < NODE_FIELDS )
                  /* node line is incorrect */
                { err_no = EN11; goto error; }
      
                if ( i < 0 || i > n )
                  /* wrong value of node */
                { err_no = EN12; goto error; }
      
                switch (nd) {
                  case 's':  /* source line */
        
                    if ( no_nslines != 0)
                      /* more than one source line */ 
                    { err_no = EN9; goto error; }
        
                    no_nslines = 1;
                    src = i;
                    break;
        
                    case 't':  /* sink line */
        
                      if ( no_nklines != 0)
                        /* more than one sink line */
                      { err_no = EN9; goto error; }
        
                      no_nklines = 1;
                      sink = i;
                      break;
        
                  default:
                    /* wrong type of node-line */
                    err_no = EN12; goto error; 
                }
                break;
      
                case 'a':                    /* arc description */
                  if ( no_nslines == 0 || no_nklines == 0 ) 
                    /* there was not source and sink description above */
                  { err_no = EN14; goto error; }
      
                  if ( no_alines >= m )
                    /*too many arcs on input*/
                  { err_no = EN16; goto error; }
      
                  if (
                      /* reading an arc description */
                      sscanf ( in_line.c_str(),"%*c %ld %ld %i",
                               &head, &tail, &cap )
                      != ARC_FIELDS
                     ) 
                    /* arc description is not correct */
                  { err_no = EN15; goto error; }

                  --tail; // index from 0, not 1
                  --head;
                  if ( tail < 0  ||  tail > n  ||
                       head < 0  ||  head > n  
                     )
                    /* wrong value of nodes */
                  { err_no = EN17; goto error; }

                  if(head<tail){
                    edges[std::make_pair(head,tail)].cap=cap;
                  } else{
                    edges[std::make_pair(tail,head)].rev_cap=cap;
                  }
                  ++no_alines;
                  break;
      
      default:
        /* unknown type of line */
        err_no = EN18; goto error;
      
    } /* end of switch */
  }     /* end of input loop */
  
  /* ----- all is red  or  error while reading ----- */ 
  
  if ( feof (stdin) == 0 ) /* reading error */
  { err_no=EN21; goto error; } 
  
  if ( no_lines == 0 ) /* empty input */
  { err_no = EN22; goto error; } 
  
  if ( no_alines < m ) /* not enough arcs */
  { err_no = EN19; goto error; } 
    
  /* Thanks God! all is done */
//   std:: cout  << " src/sink " << src << "/" << sink << std::endl;
  for (long vi = 0; vi < n; ++vi){
    if (vi == src || vi == sink)
      nodes.push_back(0);
    else
      nodes.push_back(g.add_node());
  }
  //add all the found edges    

  for(tEdgeMap::iterator vit = edges.begin(); vit!=edges.end();++vit){
    if(vit->first.first == src && vit->first.second == sink || vit->first.first == sink && vit->first.second == src){
      std::cerr << "kolmo-maxflow cant handle direct edges from src->sink (we're correcting that by hand)" <<std::endl;
      std::cerr << "edge was: " << vit->first.first << "->" << vit->first.second << "(" << vit->second.cap << "|"<<vit->second.rev_cap <<")"  <<std::endl;
      if(vit->first.first == src && vit->first.second == sink)
        flow_correction += vit->second.cap;
      else 
        flow_correction += vit->second.rev_cap;
      continue;
    }    
    if(vit->first.first == src){
//       std::cout << "adding tweights from " << vit->first.first << "->"<<vit->first.second<< " with ("<<vit->second.cap << ","<<vit->second.rev_cap << ")"<<std::endl;      
      g.add_tweights(nodes[vit->first.second],vit->second.cap,0);
    }
    else if(vit->first.second== src){
//       std::cout << "adding tweights from " << vit->first.first << "->"<<vit->first.second<< " with ("<<vit->second.cap << ","<<vit->second.rev_cap << ")"<<std::endl;      
      g.add_tweights(nodes[vit->first.first],vit->second.rev_cap,0);
    }
    else if(vit->first.second == sink){
//       std::cout << "adding tweights from " << vit->first.first << "->"<<vit->first.second<< " with ("<<vit->second.cap << ","<<vit->second.rev_cap << ")"<<std::endl;      
      g.add_tweights(nodes[vit->first.first],0,vit->second.cap);
    }
    else if(vit->first.first == sink){
//       std::cout << "adding tweights from " << vit->first.first << "->"<<vit->first.second<< " with ("<<vit->second.cap << ","<<vit->second.rev_cap << ")"<<std::endl;
      
      g.add_tweights(nodes[vit->first.second],0,vit->second.rev_cap);
    }
    else{
      //just a normal edge    
      /*     std::cout << "adding edge from " << vit->first.first << "->"<<vit->first.second<< " with ("<<vit->second.cap << ","<<vit->second.rev_cap << ")"<<std::endl;*/
      g.add_edge(nodes[vit->first.first], nodes[vit->first.second], vit->second.cap,vit->second.rev_cap);
    }
  }  

  return (0);
  
  /* ---------------------------------- */
  error:  /* error found reading input */
  
      printf ( "\nline %ld of input - %s\n", 
               no_lines, err_message[err_no] );
  
  exit (1);
  return (0); /* to avoid warning */
}


int main()
{
  Graph g;
  Graph::captype flow_correction = 0;
  read_dimacs_max_flow_kolmogorov(g, flow_correction);
  
  float time = timer();
  Graph::captype flow = g.maxflow();
  float elapsed = timer() - time;
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout << "flow: " << flow + flow_correction << std::endl;
  std::cout << "time: " << elapsed << std::endl;
  
  return EXIT_SUCCESS;
}
