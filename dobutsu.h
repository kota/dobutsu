#ifndef _DOBUTSU_H
#define _DOBUTSU_H
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <complex>
#include <deque>
#include <string>
using namespace std;
typedef complex<int> Point;

typedef unsigned long long uint64;
typedef vector<uint64> vUint64;
typedef vector<char> vChar;
typedef unsigned char uchar;
typedef vector<uchar> vuChar;
typedef vector<short> vShort;
typedef vector<int> vInt;
// typedef map<uint64,int> sVals;

/**
 * �ե������������ǥե����ޥåȰ۾��ȯ����������throw�����㳰
 */
struct FormatException{
};

/**
 * �������������礬���Ĥ��ä�����throw�����㳰
 */
struct InconsistentException{
};

struct NoLionException{
};

struct InvalidMoveException{
};

struct InconsistentPTypeException{
};

/**
 * �ץ쥤�䡼
 * �ϸ�ˤʤ�ä�����֤�BLACK, ����֤�WHITE�Ȥ��롥
 */
enum Player{
  BLACK=1, WHITE=-1
};
/**
 * ���֤�ȿž
 */
static inline Player alt(Player pl)
{
  return static_cast<Player>(-static_cast<int>(pl));
}
static int makePtypeChar(char c0,char c1);
/**
 * ��μ����player��ɽ��������������뤿��Υ��饹
 * �б����륪�֥������ȤϤʤ�
 */
struct Ptype{
  enum {
    EMPTY=0,
    SILVER=1,
    GOLD=2,
    KING=3,
  };
  /**
   * ptype����λ���WHITE�ζ��ɽ��
   */
  static int makePtype(Player p,int type){
    return p*type;
  }
  /**
   * ʸ���� s ��indexʸ���ܤ����3ʸ���򸫤�ptype���֤�
   */
  static int makePtype(string const& s,int index);
  /**
   * 2ʸ���ζ𥷥�ܥ�
   * HI, ZO, KI, NI, LI
   * �Ȥ��б�
   */
  static const char *strs[];
  static string str(int type){
    if(type==EMPTY) return " . ";
    string pstr="+";
    if(type<0){
      pstr="-";
      type= -type;
    }
    if(type>=6)
      //throw InconsistentException();
      throw InconsistentPTypeException();
    return pstr+strs[type];
  }
};
/**
 * 2ʸ����char������type����
 */
static inline int makePtypeChar(char c0,char c1)
{
  for(int i=1;i<=5;i++)
    if(c0==Ptype::strs[i][0] && c1==Ptype::strs[i][1]) return i;
  std::cerr << "c0=" << c0 << ",c1=" << c1 << std::endl;
  throw FormatException();
  return -1;
}

/**
 * ʸ���� s ��indexʸ���ܤ����3ʸ���򸫤�ptype���֤�
 */
inline int Ptype::makePtype(string const& s,int index){
  if(index+3>s.length()) throw FormatException();
  if(s[index+1]=='.'){
    if(s[index]==' ' && s[index+2]==' ')
      return Ptype::EMPTY;
    throw FormatException();
  }
  Player pl;
  if(s[index]=='+') pl=BLACK;
  else{
    if(s[index]!='-') throw FormatException();
    pl=WHITE;
  }
  int ptype=makePtypeChar(s[index+1],s[index+2]);
  return makePtype(pl,ptype);
}

/**
 * 8��˵�Υ٥��ȥ�
 */
extern const Point directions[8];
/**
 * �𤴤Ȥˤɤ�������ư���뤫��Ͽ�������
 */
extern const int canMoves[5];

/**
 * ���̤�x,y����1������Position����
 * ������x,y �Ͼ���ή��
 * (2,0) (1,0) (0,0)
 * (2,1) (1,1) (0,1)
 * (2,2) (1,2) (0,2)
 * (2,3) (1,3) (0,3)
 * �ȤʤäƤ��뤬�ʤ�����ʹ���ʤ���
 */
static inline int makePosition(int x, int y)
{
  assert(0<=x && x<3);
  assert(0<=y && y<5);
  return x*5+y;
}
/**
 * �����ɽ��Position
 */
extern const int STAND;
/**
 * Position����x����Ф�
 */
static inline int pos2X(int pos)
{
  assert(0<=pos && pos<15);
  return pos/5;
}
/**
 * Position����y����Ф�
 */
static inline int pos2Y(int pos)
{
  assert(0<=pos && pos<15);
  return pos%5;
}
/**
 * Position��180�ٲ�ž����
 * (2-x)*4+(3-y)=8+3-(x*4+y)=11-pos
 * 3*5�����Ǥ� (2-x)*5+(4-y)=10+4-(x*5+y)=14-pos
 */
static inline int rot180(int pos)
{
  assert(0<=pos && pos<15);
  return 14-pos;
}
/**
 * ʸ��2ʸ������position����
 * '0' '0' ��STAND��ɽ�����Ȥˤ���
 */
static inline int makePosition(char c1,char c2)
{
  if(c1=='0' && c2=='0') return STAND;
  if('A'<= c1 && c1<='C'){
    int x=2-(c1-'A');
    if('1'<= c2 && c2<='5'){
      int y=c2-'1';
      return x*5+y;
    }
  }
  else throw FormatException();
}
/**
 * �ؤ����ɽ�����饹
 * ���֤����롥
 */
struct Move{
  // bit 0-7 to
  // bit 8-15 from if stand 0ff
  // bit 16-19 ptype - move���ptype
  // bit 31 - if player is black 0 else 1
  int v;
  Move(string const& str);
  static int makeMove(Player pl,int from, int to, int ptype)
  {
    assert(pl==BLACK || pl==WHITE);
    assert(0<=ptype && ptype<16);
    assert((0<=from && from<15)||from==255);
    assert(0<=to && to<15);
    return (pl==BLACK ? 0 : (1<<31)) | (ptype<<16) | (from<<8) | to;
  }
  Move(Player pl,int from, int to, int ptype)
  {
    v=makeMove(pl,from,to,ptype);
  }
  int from() const{ return (v>>8)&255; }
  int to() const{ return v&255; }
  Player player() const{ return ((v&(1<<31))!=0 ? WHITE : BLACK); }
  int ptype() const { return (v>>16)&255; }
  Move rotateChangeTurn() const
  {
    if(from()==STAND)
      return Move(alt(player()),STAND,rot180(to()),ptype());
    else
      return Move(alt(player()),rot180(from()),rot180(to()),ptype());
  }
};
bool operator==(Move const &m1, Move const& m2);
typedef vector<Move> vMove;
vMove readMoveFile(string const& fileName);
ostream& operator<<(ostream& os,Move const& m);
/**
 * ���̤�ɽ�����饹
 */
struct State{
  /**
   * ���̤�15�ޥ�������ptype
   */
  char board[3*5];
  /**
   * ���줾��Υץ쥤�䡼�ζ���ζ�θĿ�
   * 2 * 3
   */
  int stands[6];
  /**
   * ���μ��֤Υץ쥤�䡼(BLACK or WHITE)
   */
  int turn;
  /**
   * ������֤���
   */
  State(){
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++)
        board[x*5+y]=Ptype::EMPTY;
        board[0*5+0]=(char)Ptype::makePtype(WHITE,Ptype::SILVER);
        board[1*5+0]=(char)Ptype::makePtype(WHITE,Ptype::KING);
        board[2*5+0]=(char)Ptype::makePtype(WHITE,Ptype::GOLD);
        board[0*5+4]=(char)Ptype::makePtype(BLACK,Ptype::GOLD);
        board[1*5+4]=(char)Ptype::makePtype(BLACK,Ptype::KING);
        board[2*5+4]=(char)Ptype::makePtype(BLACK,Ptype::SILVER);
    for(int i=0;i<6;i++) stands[i]=0;
    turn=BLACK;
  }
  /**
   * pack�������֤���player����ꤷ�ƺ��
   */
  State(uint64 p,Player pl=BLACK)
  {
    if(pl==BLACK) *this=makeBlackFromUint64(p);
    else *this=makeBlackFromUint64(p).rotateChangeTurn();
  }
  /**
   * CSA����ɽ��������
   */
  State(string const& s);
  /**
   * pack�������֤�����֤Ǻ��
   * bit 59 - 48 : 48+2*j�ӥåȤ����2�ӥåȤŤĤ� stands[j]��ɽ��
   * bit 47 - 0  : 4*pos�ӥåȤ����4�ӥåȤ��Ĥ� board[pos] ��ɽ��
   *             : ptype�β���4�ӥåȤʤΤ�bit3��set����Ƥ�����ϡ�
   *             : WHITE�Ѥ�����
   */
  static State makeBlackFromUint64(uint64 p){
    State s;
    int i=0;
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++){
        char c=(p>>(i*4))&15;
        if((c&8)!=0) c-=16;
        s.board[x*5+y]=c;
        i++;
      }
    //for(int j=0;j<6;j++){
    //  s.stands[j]=(p>>(48+j*2))&3;
    //}
    s.turn=BLACK;
    return s;
  }
  /**
   * ���̤�180�ٲ�ž����turn��ȿž����State����
   */
  State rotateChangeTurn() const
  {
    State ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++)
        ret.board[x*5+y]= -board[(2-x)*5+4-y];
    for(int i=0;i<3;i++){
      ret.stands[i+3]=stands[i]; 
      ret.stands[i]=stands[i+3]; 
    }
    ret.turn= -turn;
    return ret;
  }
  /**
   * turn��ȿž
   */
  void changeTurn()
  {
    turn = -turn;
  }
  /**
   * ������ȿž����State����
   */
  State flip() const
  {
    State ret(*this);
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++)
        ret.board[x*5+y]= board[(2-x)*5+y];
    return ret;
  }
  /**
   * ���֤����̤�64�ӥå��������Ѵ�
   * bit 59 - 48 : 48+2*j�ӥåȤ����2�ӥåȤŤĤ� stands[j]��ɽ��
   * bit 47 - 0  : 4*pos�ӥåȤ����4�ӥåȤ��Ĥ� board[pos] ��ɽ��
   *             : ptype�β���4�ӥåȤʤΤ�bit3��set����Ƥ�����ϡ�
   *             : WHITE�Ѥ�����
   */
  uint64 pack() const
  {
    assert(turn==BLACK);
    uint64 ret=0ull;
    int i=0;
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++){
        ret|=static_cast<uint64>(board[x*5+y]&15)<<(i*4);
        i++;
      }
    //for(int j=0;j<6;j++)
    //  ret|=static_cast<uint64>(stands[j])<<(48+j*2);
    return ret;
  }
  /**
   * ���֤��������������64�ӥå��������Ѵ�����flip������ΤȾ����������֤�
   */
  uint64 normalize() const{
    if(turn==BLACK){
      uint64 u1=pack();
      uint64 u2=flip().pack();
      return std::min(u1,u2);
    }
    else{
      State news=rotateChangeTurn();
      uint64 u1=news.pack();
      uint64 u2=news.flip().pack();
      return std::min(u1,u2);
    }
  }
  /**
   * ���ꤵ�줿Player��lion���ܤ�
   */
  Point lion(Player p) const{
    char pLion=Ptype::makePtype(p,Ptype::KING);
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++)
        if(pLion==board[x*5+y]) return Point(x,y);
    std::cerr << *this << std::endl;
    std::cerr << "no lion" << std::endl;
    throw NoLionException();
    return Point(0,0);
  }
  /**
   * Point p�����̤��������ɤ�����Ƚ�ꤹ��
   */
  static bool isInside(Point p){
    return 0<=p.real() && p.real()<=2 && 0<=p.imag() && p.imag()<=4;
  }
  /**
   * ���֤�ptype��dir������ư���뤫�ɤ�����Ƚ�ꤹ��
   */
  static bool canMove(char ptype,int dir){
    return ((1<<dir)&canMoves[ptype-Ptype::SILVER])!=0;
  }
  /**
   * ���֤����̤����֤ξ������ɤ�����Ƚ�ꤹ��
   */
  bool isWinByBlack() const{
    assert(turn==BLACK);
    // can capture the opponent's lion
    Point pLion=lion(WHITE);
    //    std::cout << "pLion=" << pLion << std::endl;
    for(int dir=0;dir<8;dir++){
      Point pos=pLion-directions[dir];
      if(!isInside(pos)) continue;
      char ptype=board[pos.real()*5+pos.imag()];
      //      std::cout << "dir=" << dir << ",pos=" << pos << ",ptype=" << ptype << std::endl;
      if(ptype>0 && canMove(ptype,dir))
        return true;
    }
    return false;
  }
  /**
   * ���̤����֤ξ������ɤ�����Ƚ�ꤹ��
   */
  bool isWin() const{
    if(turn==BLACK) return isWinByBlack();
    return rotateChangeTurn().isWinByBlack();
  }
  /**
   * ���֤����̤����֤��餱���ɤ�����Ƚ�ꤹ��
   * isWin�Ǥʤ����Ȥϥ����å��ѤߤȤ���
   */
  bool isLoseByBlack() const{
    assert(turn==BLACK);
    assert(!isWin());
    // can capture the opponent's lion
    Point pLion=lion(WHITE);
    return pLion.imag() == 4;
  }
  /**
   * ���̤����֤��餱���ɤ�����Ƚ�ꤹ��
   * isWin�Ǥʤ����Ȥϥ����å��ѤߤȤ���
   */
  bool isLose() const{
    if(turn==BLACK) return isLoseByBlack();
    return rotateChangeTurn().isLoseByBlack();
  }
  /**
   * ��ο������äƤ��뤳�Ȥ��ǧ����
   * 3*5�Ǥϻ�����̵���Τ�King�����Ĥ��뤫&&���줾��Υ��ޤ����İʾ�̵�������������å�����
   */
  bool isConsistent() const{
    vInt counts(7,0);
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++){
        char ptype=board[x*5+y];
        if(ptype<0){
          ptype = -ptype + Ptype::KING;//����̡��˽���
        }
        if(ptype>0) counts[ptype]++;
      }
    if(counts[Ptype::KING]!=1) return false;
    if(counts[Ptype::KING+Ptype::KING]!=1) return false;
    if(counts[Ptype::SILVER]>1) return false;
    if(counts[Ptype::SILVER+Ptype::KING]>1) return false;
    if(counts[Ptype::GOLD]>1) return false;
    if(counts[Ptype::GOLD+Ptype::KING]>1) return false;
    return true;
  }
#if 0
  /**
   *
   */
  vUint64 nextStates() const{
    vUint64 ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<4;y++){
	Point pos(x,y);
	char ptype=board[x*4+y];
	if(ptype==0){
	  for(int i=0;i<3;i++)
	    if(stands[i]>0){
	      State s(*this);
	      s.board[x*4+y]=i+Ptype::BABY;
	      s.stands[i]--;
	      s.changeTurn();
	      ret.push_back(s.normalize());
	    }
	}
	if(ptype<=0){
	  for(int dir=0;dir<8;dir++){
	    Point pos1=pos-directions[dir];
	    if(!isInside(pos1)) continue;
	    char ptype1=board[pos1.real()*4+pos1.imag()];
	    if(ptype1>0 && canMove(ptype1,dir)){
	      if(ptype1==Ptype::BABY && y==0) ptype1=Ptype::CHICKEN;
	      State s(*this);
	      s.board[x*4+y]=ptype1;
	      s.board[pos1.real()*4+pos1.imag()]=Ptype::EMPTY;
	      if( ptype < 0 ){
		if(-ptype==Ptype::CHICKEN)
		  s.stands[0]++;
		else
		  s.stands[-ptype-Ptype::BABY]++;
	      }
	      s.changeTurn();
	      ret.push_back(s.normalize());
	    }
	  }
	}
      }
    return ret;
  }
#else
  vUint64 nextStates() const{
    vMove moves=nextMoves();
    vUint64 ret;
    for(size_t i=0;i<moves.size();i++){
      State news(*this);
      news.applyMove(moves[i]);
      State s(news.normalize(),BLACK);
      ret.push_back(news.normalize());
    }
    return ret;
  }
#endif
  /**
   * ���֤�state�ǹ�ˡ��򤹤٤ƺ��
   * isWin, isLose�ǤϸƤФʤ�
   */
  vMove nextMovesForBlack() const{
    assert(turn==BLACK);
    assert(!isWin());
    assert(!isLose());
    vMove ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++){
        Point pos(x,y);
        char ptype=board[x*5+y];
        //if(ptype==0){
        //  for(int i=0;i<3;i++)
        //    if(stands[i]>0){
        //      ret.push_back(Move(BLACK,STAND,makePosition(x,y),i+Ptype::SILVER));
        //    }
        //}
        if(ptype<=0){
	        for(int dir=0;dir<8;dir++){
	          Point pos1=pos-directions[dir];
	          if(!isInside(pos1)) continue;
	          char ptype1=board[pos1.real()*5+pos1.imag()];
	          if(ptype1>0 && canMove(ptype1,dir)){
	            ret.push_back(Move(BLACK,makePosition(pos1.real(),pos1.imag()),makePosition(x,y),ptype1));
	          }
	        }
	      }
      }
    return ret;
  }
  /**
   * state�ǹ�ˡ��򤹤٤ƺ��
   * isWin, isLose�ǤϸƤФʤ�
   */
  vMove nextMoves() const{
    if(turn==BLACK) return nextMovesForBlack();
    else{
      State rev_s=rotateChangeTurn();
      vMove rev=rev_s.nextMovesForBlack();
      vMove ret;
      for(size_t i=0;i<rev.size();i++)
        ret.push_back(rev[i].rotateChangeTurn());
      return ret;
    }
  }
  /**
   * move������state��valid���ɤ�����Ƚ�ꤹ�롥
   * generate���Ƥ��餽�Υ��С��Ǥ��뤳�Ȥ�Ƚ�ꤹ��Ȥ���
   * �٤���ˡ
   */
  bool isValidWithGenerate(Move const& move) const
  {
    vMove ret=nextMoves();
    for(size_t i=0;i<ret.size();i++)
      if(ret[i]==move) return true;
    return false;
  }
  /**
   * move��state��valid���ɤ�����Ƚ�ꤹ�롥
   * rule�ˤ�äƲ�ǽ�ʤ��Ȥ�����ˤ��롡
   */
  bool isValid(Move const& move) const
  {
    if(move.player() != turn) return false;
    int ptype=move.ptype();
    int capture_ptype=board[move.to()];
    int from_ptype=board[move.from()];
    if(turn==WHITE) from_ptype= -from_ptype;
    if(from_ptype<=0) return false;
    return (turn==BLACK ? capture_ptype<=0 : capture_ptype>=0);
  }
  /**
   * State��move��apply�����Ѳ�������
   */
  void applyMove(Move const& move)
  {
    assert(isValid(move));
    assert(isValidWithGenerate(move));
    //std::cerr << move << std::endl;
    //std::cerr << "bofore" << std::endl;
    //std::cerr << *this << std::endl;
    if(!isValid(move) || !isValidWithGenerate(move)){
      std::cerr << *this << std::endl;
      std::cerr << "invalidMove " << move << std::endl;
      throw InvalidMoveException();
    }
    assert(move.ptype()==abs(board[move.from()]));
    board[move.from()]=Ptype::EMPTY;
    int capture_ptype=board[move.to()];
    int ptype=move.ptype();
    if(turn==WHITE) 
      ptype= -ptype;
    else
      capture_ptype= -capture_ptype;
    assert(capture_ptype>=0);
    board[move.to()]=ptype;
    changeTurn();

    //std::cerr << "after" << std::endl;
    //std::cerr << *this << std::endl;
    if(!isConsistent()){
      std::cerr << *this << std::endl;
      std::cerr << "Inconsistent" << std::endl;
      throw InconsistentException();
    }
  }
  friend ostream& operator<<(ostream& os,State const& s);
  friend bool operator==(State const& s1, State const& s2);
};
bool operator==(State const& s1, State const& s2);
bool operator!=(State const& s1, State const& s2);
#endif
