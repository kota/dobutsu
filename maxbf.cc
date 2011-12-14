/**
 * 分岐数最大の局面
 */
#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"
#include <vector>
using namespace std;
typedef vector<int> vInt;
typedef vector<vInt> vvInt;

int main(int ac,char **ag)
{
  int maxbf=0;

  AllStateTable allS("allstates.dat",false);     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat",false);
  vvInt counts(3,vInt(256,0));
  int internalCount=0;
  int allbf=0;
  for(size_t i=0;i<allS.size();i++){
    State s(allS[i],BLACK);
    if(s.isWin() || s.isLose()) continue;
    internalCount++;
    vMove ret=s.nextMoves();
    int bf=ret.size();
    allbf+=bf;
    if(bf>=256) throw InconsistentException();
    counts[winLose.getWinLose(i)+1][bf]++;
    if(bf==0){
      std::cerr << s << std::endl;
      std::cerr << "bf=" << bf << std::endl;
    }
    if(bf>maxbf){
      std::cerr << s << std::endl;
      std::cerr << "bf=" << bf << std::endl;
      for(size_t j=0;j<ret.size();j++){
	int wlc,wl;
	wl=winLose.getWinLose(s,ret[j],wlc);
	std::cerr << j << " : " << ret[j] << ",wl=" << wl << "(" << wlc << ")"<< std::endl;
      }
      maxbf=bf;
    }
  }
  std::cerr << "internalCount=" << internalCount << ",allbf=" << allbf << ",average=" << (double)allbf/(double)internalCount << std::endl;
  for(int i=0;i<3;i++){
    std::cerr << "winLose=" << (i-1) << std::endl;
    for(int j=0;j<256;j++){
      if(counts[i][j]>0){
	std::cerr << j << " : " << counts[i][j] << std::endl;
      }
    }
  }
}
