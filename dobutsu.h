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
 * ファイル入力等でフォーマット異常を発見した時にthrowする例外
 */
struct FormatException{
};

/**
 * 局面等で不整合が見つかった時にthrowする例外
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
 * プレイヤー
 * 囲碁にならって先手番をBLACK, 後手番をWHITEとする．
 */
enum Player{
  BLACK=1, WHITE=-1
};
/**
 * 手番の反転
 */
static inline Player alt(Player pl)
{
  return static_cast<Player>(-static_cast<int>(pl));
}
static int makePtypeChar(char c0,char c1);
/**
 * 駒の種類とplayerを表す整数を管理するためのクラス
 * 対応するオブジェクトはない
 */
struct Ptype{
  enum {
    EMPTY=0,
    SILVER=1,
    GOLD=2,
    KING=3,
  };
  /**
   * ptypeが負の時はWHITEの駒を表す
   */
  static int makePtype(Player p,int type){
    return p*type;
  }
  /**
   * 文字列 s のindex文字目からの3文字を見てptypeを返す
   */
  static int makePtype(string const& s,int index);
  /**
   * 2文字の駒シンボル
   * HI, ZO, KI, NI, LI
   * との対応
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
 * 2文字のcharから駒のtypeを作る
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
 * 文字列 s のindex文字目からの3文字を見てptypeを返す
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
 * 8近傍のベクトル
 */
extern const Point directions[8];
/**
 * 駒ごとにどの方向に動けるかを記録したもの
 */
extern const int canMoves[5];

/**
 * 盤面のx,yから1次元のPositionを作る
 * ここのx,y は将棋流で
 * (2,0) (1,0) (0,0)
 * (2,1) (1,1) (0,1)
 * (2,2) (1,2) (0,2)
 * (2,3) (1,3) (0,3)
 * となっているがなぜかは聞かないで
 */
static inline int makePosition(int x, int y)
{
  assert(0<=x && x<3);
  assert(0<=y && y<5);
  return x*5+y;
}
/**
 * 駒台を表すPosition
 */
extern const int STAND;
/**
 * Positionからxを取り出す
 */
static inline int pos2X(int pos)
{
  assert(0<=pos && pos<15);
  return pos/5;
}
/**
 * Positionからyを取り出す
 */
static inline int pos2Y(int pos)
{
  assert(0<=pos && pos<15);
  return pos%5;
}
/**
 * Positionを180度回転する
 * (2-x)*4+(3-y)=8+3-(x*4+y)=11-pos
 * 3*5将棋では (2-x)*5+(4-y)=10+4-(x*5+y)=14-pos
 */
static inline int rot180(int pos)
{
  assert(0<=pos && pos<15);
  return 14-pos;
}
/**
 * 文字2文字からpositionを作る
 * '0' '0' はSTANDを表すことにする
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
 * 指し手を表すクラス
 * 手番も入る．
 */
struct Move{
  // bit 0-7 to
  // bit 8-15 from if stand 0ff
  // bit 16-19 ptype - move後のptype
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
 * 局面を表すクラス
 */
struct State{
  /**
   * 盤面の15マスに入るptype
   */
  char board[3*5];
  /**
   * それぞれのプレイヤーの駒台の駒の個数
   * 2 * 3
   */
  int stands[6];
  /**
   * 次の手番のプレイヤー(BLACK or WHITE)
   */
  int turn;
  /**
   * 初期配置を作る
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
   * packした状態からplayerを指定して作る
   */
  State(uint64 p,Player pl=BLACK)
  {
    if(pl==BLACK) *this=makeBlackFromUint64(p);
    else *this=makeBlackFromUint64(p).rotateChangeTurn();
  }
  /**
   * CSA風の表記から作る
   */
  State(string const& s);
  /**
   * packした状態から黒番で作る
   * bit 59 - 48 : 48+2*jビットからの2ビットづつで stands[j]を表す
   * bit 47 - 0  : 4*posビットからの4ビットずつで board[pos] を表す
   *             : ptypeの下位4ビットなのでbit3がsetされている時は，
   *             : WHITE用に補正
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
   * 盤面を180度回転してturnを反転したStateを作る
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
   * turnを反転
   */
  void changeTurn()
  {
    turn = -turn;
  }
  /**
   * 左右を反転したStateを作る
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
   * 黒番の盤面を64ビット整数に変換
   * bit 59 - 48 : 48+2*jビットからの2ビットづつで stands[j]を表す
   * bit 47 - 0  : 4*posビットからの4ビットずつで board[pos] を表す
   *             : ptypeの下位4ビットなのでbit3がsetされている時は，
   *             : WHITE用に補正
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
   * 黒番に正規化した上で64ビット整数に変換し，flipしたものと小さい方を返す
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
   * 指定されたPlayerのlionを捜す
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
   * Point pが盤面の内部かどうかを判定する
   */
  static bool isInside(Point p){
    return 0<=p.real() && p.real()<=2 && 0<=p.imag() && p.imag()<=4;
  }
  /**
   * 黒番のptypeがdir方向に動けるかどうかを判定する
   */
  static bool canMove(char ptype,int dir){
    return ((1<<dir)&canMoves[ptype-Ptype::SILVER])!=0;
  }
  /**
   * 黒番の盤面が手番の勝ちかどうかを判定する
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
   * 盤面が手番の勝ちかどうかを判定する
   */
  bool isWin() const{
    if(turn==BLACK) return isWinByBlack();
    return rotateChangeTurn().isWinByBlack();
  }
  /**
   * 黒番の盤面が手番の負けかどうかを判定する
   * isWinでないことはチェック済みとする
   */
  bool isLoseByBlack() const{
    assert(turn==BLACK);
    assert(!isWin());
    // can capture the opponent's lion
    Point pLion=lion(WHITE);
    return pLion.imag() == 4;
  }
  /**
   * 盤面が手番の負けかどうかを判定する
   * isWinでないことはチェック済みとする
   */
  bool isLose() const{
    if(turn==BLACK) return isLoseByBlack();
    return rotateChangeTurn().isLoseByBlack();
  }
  /**
   * 駒の数があっていることを確認する
   * 3*5では持ち駒が無いのでKingが２個あるか&&それぞれのコマが２個以上無いかだけチェックする
   */
  bool isConsistent() const{
    vInt counts(7,0);
    for(int x=0;x<3;x++)
      for(int y=0;y<5;y++){
        char ptype=board[x*5+y];
        if(ptype<0){
          ptype = -ptype + Ptype::KING;//先後別々に集計
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
   * 黒番のstateで合法手をすべて作る
   * isWin, isLoseでは呼ばない
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
   * stateで合法手をすべて作る
   * isWin, isLoseでは呼ばない
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
   * moveがあるstateでvalidかどうかを判定する．
   * generateしてからそのメンバーであることで判定するという
   * 遅い方法
   */
  bool isValidWithGenerate(Move const& move) const
  {
    vMove ret=nextMoves();
    for(size_t i=0;i<ret.size();i++)
      if(ret[i]==move) return true;
    return false;
  }
  /**
   * moveがstateでvalidかどうかを判定する．
   * ruleによって可能なことを前提にする　
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
   * Stateにmoveをapplyして変化させる
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
