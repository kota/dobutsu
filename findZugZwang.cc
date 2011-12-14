/**
 * ZugZwangの局面を捜す．
 */

#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"

int main()
{
  AllStateTable allS("allstates.dat",false);     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat",false);
  int symcount=0,count=0,norevcount=0;
  for(size_t i=0;i<allS.size();i++){
    if(winLose.getWinLose(i)!= -1) continue;
    State s(allS[i],BLACK);
    if(s.isWin()) throw InconsistentException();
    if(s.isLose()) continue;
    s.changeTurn();
    int index=winLose.getAllS().find(s.normalize());
    if(index<0){
#if 0
      std::cerr << "--------------" << std::endl;
      std::cerr << "no rev" << std::endl;
      std::cerr << s << std::endl;
#endif
      norevcount++; continue;
    }
    int wl,wlc;
    wl=winLose.getWinLose(index);
    if(wl== -1){
      count++;
#if 1
      if(index==i)
#endif
      {
	symcount++;
      std::cerr << "--------------" << std::endl;
      std::cerr << s << std::endl;
      std::cerr << wl << "(" << winLose.getWinLose(index) << ")";
      }
    }
  }
  std::cerr << "count=" << count << std::endl;
  std::cerr << "norevcount=" << norevcount << std::endl;
  std::cerr << "symcount=" << symcount << std::endl;
}  
