#include "dobutsu.h"
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <unistd.h>

int main()
{
  CppUnit::TextUi::TestRunner runner;

  CppUnit::TestFactoryRegistry &registry 
    = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cout));
  runner.run("", false);

  CppUnit::TestResultCollector &result = runner.result(); 
  return result.wasSuccessful() ? 0 : 1;
}

class PlayerTest : public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(PlayerTest);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testAlt);
  CPPUNIT_TEST_SUITE_END();
public:
  void testEqual() {
    Player black=BLACK, white=WHITE;
    CPPUNIT_ASSERT(black==BLACK);
    CPPUNIT_ASSERT(white==WHITE);
    CPPUNIT_ASSERT(black!=WHITE);
    CPPUNIT_ASSERT(white!=BLACK);
  }
  void testAlt() {
    CPPUNIT_ASSERT(alt(BLACK)==WHITE);
    CPPUNIT_ASSERT(alt(WHITE)==BLACK);
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION(PlayerTest);

class PtypeTest : public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(PtypeTest);
  CPPUNIT_TEST(testMake);
  CPPUNIT_TEST(testPtypeChar);
  CPPUNIT_TEST_SUITE_END();
public:
  void testMake(){
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype(" . ",0),Ptype::makePtype(BLACK,Ptype::EMPTY));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+HI",0),Ptype::makePtype(BLACK,Ptype::BABY));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-HI",0),Ptype::makePtype(WHITE,Ptype::BABY));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+ZO",0),Ptype::makePtype(BLACK,Ptype::ELEPHANT));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-ZO",0),Ptype::makePtype(WHITE,Ptype::ELEPHANT));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+KI",0),Ptype::makePtype(BLACK,Ptype::GIRAFFE));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-KI",0),Ptype::makePtype(WHITE,Ptype::GIRAFFE));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+NI",0),Ptype::makePtype(BLACK,Ptype::CHICKEN));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-NI",0),Ptype::makePtype(WHITE,Ptype::CHICKEN));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+LI",0),Ptype::makePtype(BLACK,Ptype::LION));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-LI",0),Ptype::makePtype(WHITE,Ptype::LION));
  }
  void testPtypeChar(){
    CPPUNIT_ASSERT(Ptype::BABY==makePtypeChar('H','I'));
    CPPUNIT_ASSERT(Ptype::ELEPHANT==makePtypeChar('Z','O'));
    CPPUNIT_ASSERT(Ptype::GIRAFFE==makePtypeChar('K','I'));
    CPPUNIT_ASSERT(Ptype::CHICKEN==makePtypeChar('N','I'));
    CPPUNIT_ASSERT(Ptype::LION==makePtypeChar('L','I'));
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION(PtypeTest);

class PositionTest : public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(PositionTest);
  CPPUNIT_TEST(testMake);
  CPPUNIT_TEST(testRot180);
  CPPUNIT_TEST_SUITE_END();
public:
  void testMake(){
    for(int y=0;y<4;y++)
      for(int x=0;x<3;x++){
	int pos=makePosition(x,y);
	CPPUNIT_ASSERT(0<=pos);
	CPPUNIT_ASSERT(pos<12);
	CPPUNIT_ASSERT_EQUAL(x,pos2X(pos));
	CPPUNIT_ASSERT_EQUAL(y,pos2Y(pos));
      }
    CPPUNIT_ASSERT_EQUAL(makePosition('A','1'),makePosition(2,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','2'),makePosition(2,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','3'),makePosition(2,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','4'),makePosition(2,3));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','1'),makePosition(1,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','2'),makePosition(1,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','3'),makePosition(1,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','4'),makePosition(1,3));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','1'),makePosition(0,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','2'),makePosition(0,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','3'),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','4'),makePosition(0,3));
  }
  void testRot180(){
    for(int y=0;y<4;y++)
      for(int x=0;x<3;x++){
	int pos=makePosition(x,y);
	int rpos=rot180(pos);
	CPPUNIT_ASSERT_EQUAL(2-x,pos2X(rpos));
	CPPUNIT_ASSERT_EQUAL(3-y,pos2Y(rpos));
      }
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION(PositionTest);

class MoveTest : public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(MoveTest);
  CPPUNIT_TEST(testMake);
  CPPUNIT_TEST(testReverse);
  CPPUNIT_TEST_SUITE_END();
public:
  void testMake() {
    Move m1("+C3C4KI");
    Move m2("+C3C4HI");
    Move m3("-C3C4KI");
    Move m4("+B4C4KI");
    Move m5("+C3B3KI");
    CPPUNIT_ASSERT(!(m1==m2));
    CPPUNIT_ASSERT(!(m1==m3));
    CPPUNIT_ASSERT(!(m1==m4));
    CPPUNIT_ASSERT(!(m1==m5));
    CPPUNIT_ASSERT_EQUAL(m1.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m1.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m1.player(),BLACK);
    CPPUNIT_ASSERT(m1.ptype()==Ptype::GIRAFFE);
    CPPUNIT_ASSERT_EQUAL(m2.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m2.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m2.player(),BLACK);
    CPPUNIT_ASSERT(m2.ptype()==Ptype::BABY);
    CPPUNIT_ASSERT_EQUAL(m3.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m3.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m3.player(),WHITE);
    CPPUNIT_ASSERT(m3.ptype()==Ptype::GIRAFFE);
    CPPUNIT_ASSERT_EQUAL(m4.from(),makePosition(1,3));
    CPPUNIT_ASSERT_EQUAL(m4.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m4.player(),BLACK);
    CPPUNIT_ASSERT(m4.ptype()==Ptype::GIRAFFE);
    CPPUNIT_ASSERT_EQUAL(m5.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m5.to(),makePosition(1,2));
    CPPUNIT_ASSERT_EQUAL(m5.player(),BLACK);
    CPPUNIT_ASSERT(m5.ptype()==Ptype::GIRAFFE);
  }
  void testReverse() {
    {
      Move m1("+C3C4KI");
      Move m2=m1.rotateChangeTurn();
      CPPUNIT_ASSERT_EQUAL(Move("-A2A1KI"),m2);
      CPPUNIT_ASSERT_EQUAL(m1,m2.rotateChangeTurn());
    }
    {
      Move m1("+00C4KI");
      Move m2=m1.rotateChangeTurn();
      CPPUNIT_ASSERT_EQUAL(Move("-00A1KI"),m2);
      CPPUNIT_ASSERT_EQUAL(m1,m2.rotateChangeTurn());
    }
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION(MoveTest);

class StateTest : public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(StateTest);
  CPPUNIT_TEST(testMake);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testIsWin);
  CPPUNIT_TEST(testIsLose);
  CPPUNIT_TEST(testRotateChangeTurn);
  CPPUNIT_TEST(testFlip);
  CPPUNIT_TEST(testNormalize);
  CPPUNIT_TEST(testMoveGen);
  CPPUNIT_TEST(testApplyMove);
  CPPUNIT_TEST(testConsistent);
  CPPUNIT_TEST_SUITE_END();
public:
  void testMake(){
    State s(
      "-KI-LI-ZO"
      " . -HI . "
      " . +HI . "
      "+ZO+LI+KI"
      "000000"
      "+");
    State s1;
    CPPUNIT_ASSERT(s==s1);
  }
  void testEqual() {
    {
      // まずは簡単なパターン
      State s(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" . +NI-NI"
	" . +LI . "
	"000000"
	"+");
      State s1;
      State s2(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" . +NI-NI"
	" . +LI . "
	"000000"
	"+");
      CPPUNIT_ASSERT(s!=s1);
      CPPUNIT_ASSERT(s==s2);
    }
    {
      // 持ち駒の違いを認識
      State s1(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" .  .  . "
	" . +LI . "
	"100100"
	"+");
      State s2(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" .  .  . "
	" . +LI . "
	"200000"
	"+");
      State s3(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" .  .  . "
	" . +LI . "
	"000200"
	"+");
      CPPUNIT_ASSERT(s1!=s2);
      CPPUNIT_ASSERT(s1!=s3);
      CPPUNIT_ASSERT(s2!=s3);
    }
    { // 手番の違いを認識
      State s1(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" .  .  . "
	" . +LI . "
	"100100"
	"+");
      State s2(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" .  .  . "
	" . +LI . "
	"100100"
	"-");
      CPPUNIT_ASSERT(s1!=s2);
    }
  }
  void testIsWin() {
    {
      State s(
	" . +KI-ZO"
	"+KI+ZO-LI"
	" . +NI-NI"
	" . +LI . "
	"000000"
	"+");
      CPPUNIT_ASSERT(s.isWin());
    }
    {
      State s(
	" . -LI . "
	"+NI-NI . "
	"+LI-ZO-KI"
	"+ZO-KI . "
	"000000"
	"-");
      CPPUNIT_ASSERT(s.isWin());
    }
    {
      State s(
	" . +NI-ZO"
	"+KI+ZO-LI"
	" . +KI-NI"
	" . +LI . "
	"000000"
	"+");
      CPPUNIT_ASSERT(!s.isWin());
    }
    {
      State s(
	" . -LI . "
	"+NI-KI . "
	"+LI-ZO-KI"
	"+ZO-NI . "
	"000000"
	"-");
      CPPUNIT_ASSERT(!s.isWin());
    }
  }
  void testIsLose() {
    {
      State s(
	" . +KI-ZO"
	"+KI+ZO+LI"
	" .  .  . "
	" . -LI . "
	"100100"
	"+");
      CPPUNIT_ASSERT(s.isLose());
    }
    {
      State s(
	" . +LI . "
	" .  .  . "
	"-LI-ZO-KI"
	"+ZO-KI . "
	"100100"
	"-");
      CPPUNIT_ASSERT(s.isLose());
    }
  }
  void testRotateChangeTurn() {
    State s(
      " . -ZO-KI"
      "+NI-HI-LI"
      "+ZO .  . "
      "+LI .  . "
      "001000"
      "+"
      );
    State s1(
      " .  . -LI"
      " .  . -ZO"
      "+LI+HI-NI"
      "+KI+ZO . "
      "000001"
      "-");
    State s2(
      " . -ZO-KI"
      "+NI-HI-LI"
      "+ZO .  . "
      "+LI .  . "
      "001000"
      "-"
      );
    CPPUNIT_ASSERT_EQUAL(s1,s.rotateChangeTurn());
    CPPUNIT_ASSERT(s2!=s.rotateChangeTurn());
  }
  void testFlip() {
    State s(
      " . -ZO-KI"
      "+NI-HI-LI"
      "+ZO .  . "
      "+LI .  . "
      "001000"
      "+"
      );
    State s1(
      "-KI-ZO . "
      "-LI-HI+NI"
      " .  . +ZO"
      " .  . +LI"
      "001000"
      "+"
      );
    CPPUNIT_ASSERT_EQUAL(s1,s.flip());
  }
  void testNormalize() {
    State s1(
      " . -ZO-KI"
      "+NI-HI-LI"
      "+ZO .  . "
      "+LI .  . "
      "001000"
      "+"
      );
    uint64 u1=s1.normalize();
    State s2(
      "-KI-ZO . "
      "-LI-HI+NI"
      " .  . +ZO"
      " .  . +LI"
      "001000"
      "+"
      );
    uint64 u2=s2.normalize();
    State s3(
      " .  . -LI"
      " .  . -ZO"
      "+LI+HI-NI"
      "+KI+ZO . "
      "000001"
      "-"
      );
    uint64 u3=s3.normalize();
    State s4(
      "-LI .  . "
      "-ZO .  . "
      "-NI+HI+LI"
      " . +ZO+KI"
      "000001"
      "-"
      );
    uint64 u4=s4.normalize();
    CPPUNIT_ASSERT(u1==u2);
    CPPUNIT_ASSERT(u1==u3);
    CPPUNIT_ASSERT(u1==u4);
    if(!(State(u1,BLACK)==s1 || State(u1,BLACK)==s2 )){
      std::cerr << s1 << std::endl;
      std::cerr << s2 << std::endl;
      std::cerr << State(u1,BLACK) << std::endl;
    }
    CPPUNIT_ASSERT( State(u1,BLACK)==s1 || State(u1,BLACK)==s2 );
    CPPUNIT_ASSERT( State(u1,WHITE)==s3 || State(u1,WHITE)==s4 );
  }
  void testMoveGen() {
    {
      State s(
	" . -ZO-KI"
	"+NI-HI-LI"
	"+ZO .  . "
	"+LI .  . "
	"001000"
	"+"
	);
      CPPUNIT_ASSERT(!s.isWin());
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(12==moves.size());
      // kirin drop moves
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+00A1KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+00B3KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+00B4KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+00C3KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+00C4KI"))!=moves.end());
      // niwatori
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2A1NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2B1NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2B2NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2B3NI"))==moves.end());
      // zo
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A3B2ZO"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A3B4ZO"))!=moves.end());
      // LION
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A4B3LI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A4B4LI"))!=moves.end());
    }
    {
      State s(
	" .  . -LI"
	" .  . -ZO"
	"+LI+HI-NI"
	"+KI+ZO . "
	"000001"
	"-");
      CPPUNIT_ASSERT(!s.isWin());
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(12==moves.size());
      // kirin drop moves
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-00C4KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-00B2KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-00B1KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-00A2KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-00A1KI"))!=moves.end());
      // niwatori
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C3C4NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C3B4NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C3B3NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C3B2NI"))==moves.end());
      // zo
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C2B3ZO"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C2B1ZO"))!=moves.end());
      // LION
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C1B2LI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C1B1LI"))!=moves.end());
    }
  }
  void testApplyMove() {
    {
      State s(
	"-NI . -KI"
	"+HI-ZO-LI"
	"+ZO .  . "
	"+LI .  . "
	"001000"
	"+"
	);
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2A1NI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2A1HI"))==moves.end());
      s.applyMove(Move("+A2A1NI"));
      State s2(
	"+NI . -KI"
	" . -ZO-LI"
	"+ZO .  . "
	"+LI .  . "
	"101000"
	"-"
	);
      CPPUNIT_ASSERT_EQUAL(s,s2);
    }
    {
      State s(
	" . -NI-KI"
	"+NI-ZO-LI"
	"+ZO .  . "
	"+LI .  . "
	"001000"
	"+"
	);
      {
	State s1(s);
	s1.applyMove(Move("+A2A1NI"));
	State s2(
	  "+NI-NI-KI"
	  " . -ZO-LI"
	  "+ZO .  . "
	  "+LI .  . "
	  "001000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A2B1NI"));
	State s2(
	  " . +NI-KI"
	  " . -ZO-LI"
	  "+ZO .  . "
	  "+LI .  . "
	  "101000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A2B2NI"));
	State s2(
	  " . -NI-KI"
	  " . +NI-LI"
	  "+ZO .  . "
	  "+LI .  . "
	  "011000"
	  "-"
	  );
	if(s1!=s2){
	  std::cerr << s1 << std::endl;
	}
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A3B2ZO"));
	State s2(
	  " . -NI-KI"
	  "+NI+ZO-LI"
	  " .  .  . "
	  "+LI .  . "
	  "011000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A3B4ZO"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  " .  .  . "
	  "+LI+ZO . "
	  "001000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A4B3LI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO+LI . "
	  " .  .  . "
	  "001000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+A4B4LI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO .  . "
	  " . +LI . "
	  "001000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+00A1KI"));
	State s2(
	  "+KI-NI-KI"
	  "+NI-ZO-LI"
	  "+ZO .  . "
	  "+LI .  . "
	  "000000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+00B3KI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO+KI . "
	  "+LI .  . "
	  "000000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+00B4KI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO .  . "
	  "+LI+KI . "
	  "000000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+00C3KI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO . +KI"
	  "+LI .  . "
	  "000000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
      {
	State s1(s);
	s1.applyMove(Move("+00C4KI"));
	State s2(
	  " . -NI-KI"
	  "+NI-ZO-LI"
	  "+ZO .  . "
	  "+LI . +KI"
	  "000000"
	  "-"
	  );
	CPPUNIT_ASSERT(s1==s2);
      }
    }
  }

  void testConsistent() {
    {
      State s(
	" . -ZO-KI"
	"+NI-HI-LI"
	"+ZO .  . "
	"+LI .  . "
	"001000"
	"+"
	);
      CPPUNIT_ASSERT(s.isConsistent());
      s.stands[0]=1;
      CPPUNIT_ASSERT(!s.isConsistent());
      s.stands[0]=0; s.stands[2]=0;
      CPPUNIT_ASSERT(!s.isConsistent());
      s.stands[2]=2; 
      CPPUNIT_ASSERT(!s.isConsistent());
    }
  }
};
  CPPUNIT_TEST_SUITE_REGISTRATION(StateTest);
