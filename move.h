#ifndef _MOVE_H
#define _MOVE_H
#include "dobutsu.h"
/*
 * 手番は入らない
 */
struct Move{
  // bit 0-7 to - if promote set bit 7
  // bit 8-15 from if stand 0ff
  // bit 16-19 ptype - 元の駒のptype
  // bit 31 - if player is black 0 else 1
  int v;
  Move(string const& str);
  Move(Player pl,int from, int to, char ptype,bool isPromote)
  {
    assert(pl==BLACK || pl==WHITE);
    assert(0<=ptype && ptype<16);
    assert((0<=from && from<12)||from==255);
    assert(0<=to && to<12);
    v=(pl==BLACK ? 0 : (1<<31)) |
      (ptype<<16) |
      (from<<8) |
      (isPromote ? (1<<7) : 0) |
      to;
  }
};

#endif
