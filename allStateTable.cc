#include "allStateTable.h"
#include <sys/types.h>
#include <sys/stat.h>

AllStateTable::AllStateTable(string const& fileName,bool lazy)
{
  struct stat st;
  if(stat(fileName.c_str(),&st)<0){
    abort();
  }
  c_size=st.st_size/sizeof(uint64);
  ifs.open(fileName.c_str());
  if(lazy==false){
    contents.resize(c_size);
    ifs.read((char *)&contents[0],c_size*sizeof(uint64));
    ifs.close();
  }
}

AllStateTable::~AllStateTable()
{
  if(contents.size()==0){
    ifs.close();
  }
}

int AllStateTable::find(uint64 v) const
{
  int low=0, high=(int)c_size-1;
  while(low<high){
    int mid=(low+high)/2;
    uint64 v1=(*this)[mid];
    if(v1==v) return mid;
    else if(v1>v){ high=mid-1; }
    else {
      low=mid+1;
    }
  }
  if((*this)[low]!=v) return -1;
  return low;
}
