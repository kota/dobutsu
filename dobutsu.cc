#include "dobutsu.h"

const char *Ptype::strs[6]={0,"HI","ZO","KI","NI","LI"};
const Point directions[8]={Point(1,-1),Point(0,-1),Point(-1,-1),Point(1,0),Point(-1,0),Point(1,1),Point(0,1),Point(-1,1)};
const int canMoves[5]={
  0x2, // baby
  0xa5, // elephant
  0x5a, // giraffe
#if 0
  0xfa, // chiken
#else
  0x5f, // chiken
#endif
  0xff, // lion
};

const int STAND=0xff;

Move::Move(string const& str)
{
  if(str.length()!=7){
    assert(0);
  }
  Player pl;
  if(str[0]=='+') pl=BLACK;
  else if(str[0]=='-') pl=WHITE;
  else assert(0);
  int from=makePosition(str[1],str[2]);
  int to=makePosition(str[3],str[4]);
  int ptype=makePtypeChar(str[5],str[6]);
//  std::cerr << "pl=" << pl << ",from=" << from << ",to=" << to << ",ptype=" << static_cast<int>(ptype) << std::endl;
  v=makeMove(pl,from,to,ptype);
}

bool operator==(Move const &m1, Move const& m2){
  return m1.v==m2.v;
}

vMove readMoveFile(string const& fileName)
{
  vMove ret;
  ifstream ifs(fileName.c_str());
  for(;;){
    string s;
    if(!(ifs >> s)) break;
    if(s.length()!=7) break;
    ret.push_back(Move(s));
  }
  return ret;
}

ostream& outPosition(ostream& os,int pos)
{
  if(pos==0xff) return os<<"00";
  int x=pos/4,y=pos%4;
  assert(0<=x && x<=2);
  assert(0<=y && y<=3);
  return os << "CBA"[x] << "1234"[y];
}
ostream& outPtype(ostream& os,int ptype)
{
  assert(1<=ptype && ptype<=5);
  return os << Ptype::strs[ptype];
}

ostream& operator<<(ostream& os,Move const& m){
  if(m.player()==BLACK) os<<'+';
  else os<<'-';
  outPosition(os,m.from());
  outPosition(os,m.to());
  outPtype(os,m.ptype());
  return os;
}

State::State(string const& s)
{
  assert(s.length()==3*3*4+7);
  for(int x=0;x<3;x++)
    for(int y=0;y<4;y++)
      board[x*4+y]=(char)Ptype::makePtype(s,y*9+(2-x)*3);
  for(int i=0;i<6;i++) stands[i]=s[3*3*4+i]-'0';
  if(s[3*3*4+6]=='+') turn=BLACK;
  else{
    if(s[3*3*4+6]!='-') throw FormatException();
    turn=WHITE;
  }
}

ostream& operator<<(ostream& os,State const& s){
  for(int y=0;y<4;y++){
    for(int x=2;x>=0;x--)
      os << Ptype::str(s.board[x*4+y]);
    os << "\n";
  }
  for(int i=0;i<6;i++)
    os << s.stands[i];
  os << "\n";
  if(s.turn==BLACK)
    os << "+\n";
  else
    os << "-\n";
}

bool operator==(State const& s1, State const& s2)
{
  if(s1.turn!=s2.turn) return false;
  for(int i=0;i<12;i++) if(s1.board[i]!=s2.board[i]) return false;
  for(int i=0;i<6;i++) if(s1.stands[i]!=s2.stands[i]) return false;
  return true;
}
bool operator!=(State const& s1, State const& s2)
{
  return !(s1==s2);
}
