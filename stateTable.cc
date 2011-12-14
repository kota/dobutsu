
// const int dSize=242259083;
const int dSize=252208011;

vChar winLoss(dSize,0);
vuChar winLossCount(dSize,0);

vUint64 allIS;

int binSearch(uint64 v,vUint64 const& allIS)
{
  int low=0, high=(int)allIS.size()-1;
  while(low<high){
    int mid=(low+high)/2;
    uint64 v1=allIS[mid];
    if(v1==v) return mid;
    else if(v1>v){ high=mid-1; }
    else {
      low=mid+1;
    }
  }
  assert(allIS[low]==v);
  return low;
}

int newWinLoss(uint64 v){
  State s(v);
  vUint64 ns=s.nextStates();
  bool alllose=true;
  for(size_t j=0;j<ns.size();j++){
    int i1=binSearch(ns[j],allIS);
    if(winLoss[i1]== -1) return 1;
    if(winLoss[i1]==0) alllose=false;
  }
  if(alllose) return -1;
  else return 0;
}
void showWinLoss(uint64 v){
  State s(v);
  std::cout << "---------------------\n";
  std::cout << s << std::endl;
  std::cout << (int)winLoss[binSearch(v,allIS)] << std::endl;
  std::cout << "---------------------\n";
}
