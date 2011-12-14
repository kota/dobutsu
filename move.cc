#include "dobutsu.h"
#include <cassert>

Move::Move(string const& str)
{
  if(!str.length()!=7){
    assert(0);
  }
  Player pl;
  if(str[0]=='+') pl=BLACK;
  else if(str[0]=='-') pl=WHITE;
  else assert(0);
  int from=makePosition(str[1],str[2]);
  int to=makePosition(str[3],str[4]);
  char ptype=makePtype(str[5],str[6]);
  v=makeMove(pl,from,to,ptype);
}
