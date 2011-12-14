/**
 * csa形式の盤面ファイルを読み込んでその後の終局までの読み筋を表示する
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
#if 1
  if(ac<2){
    usage();
  }
  std::ifstream ifs(ag[1]);
  std::string all;
  std::string line;
  while( getline(ifs,line) ){
    all += line;
  }
  State s(all);
#else
  State s;
#endif
  AllStateTable allS("allstates.dat");     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat");
//   WinLoseTable winLose(allS,"winLossWoDrop.dat","winLossWoDropCount.dat");
  winLose.showSequence(s);
}
