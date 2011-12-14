/**
 * �Ҥ褳��1���ܤ�drop����꤬����Ǥ���褦�ʶ��̤��ܤ���count���롥
 */

#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"

int main()
{
  AllStateTable allS("allstates.dat",false);     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat",false);
  int dropcount=0;
  for(size_t i=0;i<allS.size();i++){
    if((i%100000)==0) std::cerr << i << std::endl;
    if(winLose.getWinLose(i)== -1) continue; // �餱���̤ˤ϶�̣���ʤ�
    State s(allS[i],BLACK);
    if(s.isWin() || s.isLose()) continue;
    vMove moves=s.nextMoves();
    int minv=1;
    for(size_t j=0;j<moves.size();j++){
      // �Ҥ褳��1���ܤ�drop
      if(moves[j].ptype()==Ptype::BABY && moves[j].from()==STAND &&
	 pos2Y(moves[j].to())==0) continue;
      int wl,wlc;
      wl=winLose.getWinLose(s,moves[j],wlc);
      minv=std::min(wl,minv);
    }
    if(-minv != winLose.getWinLose(i)){
      dropcount++;
      std::cerr << "-------------\n";
      std::cerr << s << std::endl;
      for(size_t j=0;j<moves.size();j++){
	int wl,wlc;
	wl=winLose.getWinLose(s,moves[j],wlc);
	std::cerr << j << " : " << moves[j] << " " << wl << "(" << wlc << ")\n";
      }
    }
  }
  std::cerr << "dropcount=" << dropcount << std::endl;
}
