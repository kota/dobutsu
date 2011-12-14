/**
 * csa形式の棋譜ファイルを読み込んで，それぞれの手の勝敗をチェックする
 */
#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"

void usage()
{
  std::cerr << "Usage: checkcsa csafile" << std::endl;
}

int main(int ac,char **ag)
{
  if(ac<2){
    usage();
  }
  vMove moves=readMoveFile(ag[1]);
  AllStateTable allS("allstates.dat");     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat");
  State s;
  std::cerr << s << std::endl;
  for(size_t i=0;i<moves.size();i++){
    vMove ret=s.nextMoves();
    int lastwlc;
    int lastwl=winLose.getWinLose(s,lastwlc);
#if 1
    for(size_t j=0;j<ret.size();j++){
      int wlc,wl;
      wl=winLose.getWinLose(s,ret[j],wlc);
      std::cerr << j << " : " << ret[j] << ",wl=" << wl << "(" << wlc << ")"<< std::endl;
    }
#endif
    std::cerr << "Move : " << moves[i] << std::endl;
    int wlc,wl;
    wl=winLose.getWinLose(s,moves[i],wlc);
    if(lastwl!= -wl){
      std::cerr << s << std::endl;
      std::cerr << i << " : " << moves[i] << " " << -lastwl << " -> " << wl << std::endl;
    }
    s.applyMove(moves[i]);
//    std::cerr << s << std::endl;
  }
}
