/**
 * 「詰めどうぶつしょうぎ」の局面を作る
 * 
 */
#include "dobutsu.h"
#include "allStateTable.h"
#include "winLoseTable.h"

bool winByTry(State const& s){
  assert(!s.isWin() && !s.isLose());
  vMove moves=s.nextMoves();
  for(size_t j=0;j<moves.size();j++){
    if(moves[j].ptype()==Ptype::LION && pos2Y(moves[j].to())==0){
      State news(s);
      news.applyMove(moves[j]);
      if(!news.isWin() && news.isLose()) return true;
    }
  }
  return false;
}

int main()
{
  AllStateTable allS("allstates.dat",false);     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat",false);
  int dSize=allS.size();
  vChar canCheck(dSize,0);
  vChar winLossCheck(dSize,0);
  vChar winLossCheckCount(dSize,0);
  /**
   * check状態で負けのものを求める
   */
  for(size_t i=0;i<dSize;i++){
    if(winLose.getWinLose(i)<0 && winLose.getWinLoseCount(i)!=0){
      State s(allS[i],BLACK);
      assert(!s.isWin());
      assert(!s.isLose());
      s.changeTurn();
      if(s.isWin() || winByTry(s)){
	canCheck[i]= -1;
	if(winLose.getWinLoseCount(i)<=2){
	  winLossCheck[i]= -1;
#if 0
	  if((i%100000)==0){
	    std::cerr << "checked: \n";
	    std::cerr << s << std::endl;
	  }
#endif
	}
      }
    }
  }
  /**
   * check可能なものを求める
   */
  for(size_t i=0;i<dSize;i++){
    if(winLose.getWinLose(i)>0 && winLose.getWinLoseCount(i)!=0){
      State s(allS[i],BLACK);
      assert(!s.isWin());
      assert(!s.isLose());
      vMove moves=s.nextMoves();
      for(size_t j=0;j<moves.size();j++){
	int wlc,wl;
	State news(s);
	news.applyMove(moves[j]);
	uint64 v=news.normalize();
	int index=allS.find(v);
	if(index<0) throw InconsistentException();
	if(canCheck[index]==-1 && winLose.getWinLose(index)== -1){
	  canCheck[i]=1;
#if 1
	  if((i%1000000)==0){
	    std::cerr << "can check: \n";
	    std::cerr << s << std::endl;
	  }
#endif
	  break;
	}
      }
    }
  }
  for(int c=1;;c++){
    vChar winLossCheckNew(winLossCheck);
    std::cout << "iteration " << c << std::endl;
    bool changed=false;
    for(size_t i=0;i<dSize;i++){
      if(winLossCheck[i]==0){
	if((c&1)==1){
	  if(canCheck[i]==1){
	    State s(allS[i],BLACK);
	    assert(!s.isWin());
	    assert(!s.isLose());
	    vMove moves=s.nextMoves();
	    for(size_t j=0;j<moves.size();j++){
	      int wlc,wl;
	      State news(s);
	      news.applyMove(moves[j]);
	      uint64 v=news.normalize();
	      int index=allS.find(v);
	      if(index<0) throw InconsistentException();
	      if(winLossCheck[index]==-1){
		winLossCheckNew[i]=1;
		winLossCheckCount[i]=c;
		if(!changed){
		  std::cerr << "changed\n";
		  std::cerr << s << std::endl;
		  changed=true;
		}
		break;
	      }
	    }
	  }
	}
	else{
	  if(canCheck[i]== -1){
	    State s(allS[i],BLACK);
	    assert(!s.isWin());
	    assert(!s.isLose());
	    vMove moves=s.nextMoves();
	    bool allflag=true;
	    for(size_t j=0;j<moves.size();j++){
	      int wlc,wl;
	      State news(s);
	      news.applyMove(moves[j]);
	      if(news.isWin()) continue;
	      uint64 v=news.normalize();
	      int index=allS.find(v);
	      if(index<0) throw InconsistentException();
	      if(winLossCheck[index]!= 1){
		allflag=false;
		break;
	      }
	    }
	    if(allflag){
	      winLossCheckNew[i]= -1;
	      winLossCheckCount[i]=c;
	      if(!changed){
		std::cerr << "changed\n";
		std::cerr << s << std::endl;
		changed=true;
	      }
	    }
	  }
	}
      }
    }
    winLossCheck.swap(winLossCheckNew);
    if(changed==false) break;
  }
  {
    ofstream ofs("winLossCheck.dat");
    ofs.write((char *)&winLossCheck[0],winLossCheck.size());
  }
  {
    ofstream ofs("winLossCheckCount.dat");
    ofs.write((char *)&winLossCheckCount[0],winLossCheckCount.size());
  }
}
