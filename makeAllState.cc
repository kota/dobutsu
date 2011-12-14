#include "dobutsu.h"

#include <ext/hash_set>
typedef deque<uint64> dUint64;
struct hashUint64{
  int operator()(uint64 const& v) const{
    return (v>>32)^(v&0xffffffffull);
  }
};
typedef __gnu_cxx::hash_set<uint64,hashUint64> hUint64;

int main()
{
  vUint64 allIS;
  dUint64 q;
  hUint64 v;
  q.push_back(State().normalize());
  while(!q.empty()){
    uint64 is=q.front();
    q.pop_front();
    hUint64::const_iterator it=v.find(is);
    if(it==v.end()){
      State s(is,BLACK);
      //      std::cout << s << ",isWin==" << s.isWin()  << std::endl;
      allIS.push_back(is);
      v.insert(is);
      if(!s.isWin() && !s.isLose()){
	vUint64 ns=s.nextStates();
	for(size_t i=0;i<ns.size();i++)
	  q.push_back(ns[i]);
      }
    }
  }
  cout << v.size() << endl;
  sort(allIS.begin(),allIS.end());
  ofstream ofs("allstates.dat");
  ofs.write((char *)&allIS[0],allIS.size()*sizeof(uint64));
}
