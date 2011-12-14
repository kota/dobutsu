#ifndef _WIN_LOSE_TABLE_H
#define _WIN_LOSE_TABLE_H
#include "dobutsu.h"
#include "allStateTable.h"

class WinLoseTable {
  AllStateTable const& allS;
  vChar winLose;
  vuChar winLoseCount;
  mutable ifstream ifs1,ifs2;
public:
  WinLoseTable(AllStateTable const& allS_,string const& wlName, string const& wlcName,bool lazy=true);
  ~WinLoseTable();
  AllStateTable const& getAllS() const { return allS; }
  int getWinLose(size_t index) const {
    if(winLose.size()>0){
      return winLose[index]; 
    }
    else{
      ifs1.seekg(index,_S_beg);
      char ret;
      ifs1.read((char *)&ret,1);
      return ret;
    }
  }
  int getWinLoseCount(size_t index) const {
    if(winLoseCount.size()>0){
      return winLoseCount[index]; 
    }
    else{
      ifs2.seekg(index,_S_beg);
      unsigned char ret;
      ifs2.read((char *)&ret,1);
      return ret;
    }
  }
  int getWinLose(State const& s, int& wlc) const;
  int getWinLose(State const& s, Move const& m,int& wlc) const;
  void showSequence(State const& s) const;
};
#endif
