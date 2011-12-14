#include "winLoseTable.h"
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>

WinLoseTable::WinLoseTable(AllStateTable const& allS_,string const& wlName, string const& wlcName,bool lazy)
  :allS(allS_)
{
  struct stat st1,st2;
  if(stat(wlName.c_str(),&st1)<0 || stat(wlcName.c_str(),&st2)<0){
    assert(0);
  }
  if(allS.size() != st1.st_size || st1.st_size != st2.st_size){
    std::cerr << "File size miss match " << wlName << " <=> " << wlcName << std::endl;
    assert(0);
  }
  ifs1.open(wlName.c_str());
  ifs2.open(wlcName.c_str());
  if(!lazy){
    winLose.resize(st1.st_size);
    winLoseCount.resize(st2.st_size);
    ifs1.read((char *)&winLose[0],st1.st_size);
    ifs2.read((char *)&winLoseCount[0],st2.st_size);
    ifs1.close();
    ifs2.close();
  }
}

WinLoseTable::~WinLoseTable(){
  if(winLose.size()==0){
    ifs1.close();
  }
  if(winLoseCount.size()==0){
    ifs2.close();
  }
}

int WinLoseTable::getWinLose(State const& s, int& wlc) const
{
#if 0
  if(s.turn==WHITE){
    State rev_s=s.rotateReverse();
    rev_s.changeTurn();
    uint64 v=rev_s.normalize();
    int index=allS.find(v);
    if(!(0<=index && index<allS.size())){
      std::cerr << s << std::endl;
      std::cerr << rev_s << std::endl;
      std::cerr << "v=" << v << std::endl;
      std::cerr << "index=" << index << std::endl;
    }
    assert(0<=index && index<allS.size());
    wlc=winLoseCount[index];
    return -winLose[index];
  }
  else{
    uint64 v=s.normalize();
    int index=allS.find(v);
    assert(0<=index && index<allS.size());
    wlc=winLoseCount[index];
    return winLose[index];
  }
#else
  uint64 v=s.normalize();
  int index=allS.find(v);
  assert(0<=index && index<allS.size());
  wlc=getWinLoseCount(index);
#if 0
  if(s.turn==BLACK)
  else
    return -winLose[index];
#else
  return getWinLose(index);
#endif
#endif
}
int WinLoseTable::getWinLose(State const& s, Move const& move,int& wlc) const
{
  State news(s);
  news.applyMove(move);
  return getWinLose(news,wlc);
}

void WinLoseTable::showSequence(State const& s) const
{
  if(!s.isConsistent()) throw InconsistentException();
  uint64 v=s.normalize();
  int index=allS.find(v);
  std::cerr << "------------------" << std::endl;
  std::cerr << s << std::endl;
  std::cerr << (int)(s.turn==BLACK ? getWinLose(index) : -getWinLose(index)) <<
    "(" << (int)getWinLoseCount(index) << ")" << std::endl;
  if(getWinLoseCount(index)==0){
    if(getWinLose(index)==0){
      vMove moves=s.nextMoves();
      for(size_t i=0;i<moves.size();i++){
	int wl,wlc;
	wl=getWinLose(s,moves[i],wlc);
	std::cerr << i << " : " << moves[i] << " " << wl << "(" << wlc << ")" << std::endl;
      }
    }
    return;
  }
  if(getWinLose(index)==0){
    std::cerr << s << std::endl;
    std::cerr << "winLose=0" << std::endl;
    throw InconsistentException();
  }
  vMove moves=s.nextMoves();
  for(size_t i=0;i<moves.size();i++){
    int wl,wlc;
    wl=getWinLose(s,moves[i],wlc);
    std::cerr << i << " : " << moves[i] << " " << wl << "(" << wlc << ")" << std::endl;
  }
  for(size_t i=0;i<moves.size();i++){
    int wl,wlc;
    wl=getWinLose(s,moves[i],wlc);
    if(getWinLose(index)==-1){
      if(wl!=1){
//	throw InconsistentException();
      }
      if(getWinLoseCount(index)-1==wlc){
	std::cerr << "Move : " << moves[i] << " " << wl << "(" << wlc << ")" << std::endl;
	State news(s);
	news.applyMove(moves[i]);
	showSequence(news);
	break;
      }
      else if(getWinLoseCount(index)-1<wlc){
	throw InconsistentException();
      }
    }
    else{
      assert(getWinLose(index)==1);
      if(wl==-1){
	if(getWinLoseCount(index)-1==wlc){
	  std::cerr << "Move : " << moves[i] << " " << wl << "(" << wlc << ")" << std::endl;
	  State news(s);
	  news.applyMove(moves[i]);
	  showSequence(news);
	  break;
	}
	else if(getWinLoseCount(index)-1>wlc){
	  throw InconsistentException();
	}
      }
    }
  }
}
