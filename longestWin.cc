/**
 * 最長の勝ちを見つける．
 */
#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"
#include <fstream>

void usage()
{
  std::cerr << "Usage: checkcsa csafile" << std::endl;
}

int main(int ac,char **ag)
{
  AllStateTable allS("allstates.dat");     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat");
  int maxwin=0;
  for(size_t i=0;i<allS.size();i++){
    if(winLose.getWinLose(i)>0){
      maxwin=max(maxwin,winLose.getWinLoseCount(i));
    }
  }
  std::cerr << "maxwin=" << maxwin << std::endl;
  for(size_t i=0;i<allS.size();i++){
    if(winLose.getWinLose(i)>0 && maxwin==winLose.getWinLoseCount(i)){
      State s(allS[i],BLACK);
      std::cerr << "----------" << std::endl;
      std::cerr << s << std::endl;
    }
  }
//   WinLoseTable winLose(allS,"winLossWoDrop.dat","winLossWoDropCount.dat");
//  winLose.showSequence(s);
}
