#include "dobutsu.h"
#include "allStateTable.h"

int newWinLoss(AllStateTable const& allIS,vChar const& winLoss,uint64 v){
  State s(v);
  vUint64 ns=s.nextStates();
  bool alllose=true;
  for(size_t j=0;j<ns.size();j++){
    int i1=allIS.find(ns[j]);
    assert(0<=i1 && i1<allIS.size());
    if(winLoss[i1]== -1) return 1;
    if(winLoss[i1]==0) alllose=false;
  }
  if(alllose) return -1;
  else return 0;
}

int main()
{
  AllStateTable allIS("allstates.dat",false);     
  int dSize=allIS.size();
  vChar winLoss(dSize,0);
  vChar winLossCount(dSize,0);
  vInt count(3);
  for(size_t i=0;i<dSize;i++){
    State s(allIS[i]);
    if(s.isWin()){
      winLoss[i]=1;
      count[2]++;
    }
    else if(s.isLose()){
      winLoss[i]=-1;
      count[0]++;
    }
    else count[1]++;
  }
  for(int c=1;;c++){
    vChar winLossNew(winLoss);
    std::cout << "iteration " << c << std::endl;
    for(int j=0;j<3;j++){
      std::cout << (j-1) << " : " << count[j] << std::endl;
    }
    bool changed=false;
    for(size_t i=0;i<dSize;i++){
      if(winLoss[i]==0){
        int nv=newWinLoss(allIS,winLoss,allIS[i]);
        if(nv!=0){
          winLossNew[i]=nv;
          winLossCount[i]=c;
          count[nv+1]++; count[1]--;
          changed=true;
        }
      }
    }
    winLoss.swap(winLossNew);
    if(changed==false) break;
  }
  {
    ofstream ofs("winLoss.dat");
    ofs.write((char *)&winLoss[0],winLoss.size());
  }
  {
    ofstream ofs("winLossCount.dat");
    ofs.write((char *)&winLossCount[0],winLossCount.size());
  }
}
