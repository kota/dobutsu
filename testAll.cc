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
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+SI",0),Ptype::makePtype(BLACK,Ptype::SILVER));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-SI",0),Ptype::makePtype(WHITE,Ptype::SILVER));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+GO",0),Ptype::makePtype(BLACK,Ptype::GOLD));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-GO",0),Ptype::makePtype(WHITE,Ptype::GOLD));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("+KI",0),Ptype::makePtype(BLACK,Ptype::KING));
    CPPUNIT_ASSERT_EQUAL(Ptype::makePtype("-KI",0),Ptype::makePtype(WHITE,Ptype::KING));
  }
  void testPtypeChar(){
    CPPUNIT_ASSERT(Ptype::SILVER==makePtypeChar('S','I'));
    CPPUNIT_ASSERT(Ptype::GOLD==makePtypeChar('G','O'));
    CPPUNIT_ASSERT(Ptype::KING==makePtypeChar('K','I'));
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
    for(int y=0;y<5;y++)
      for(int x=0;x<3;x++){
        int pos=makePosition(x,y);
        CPPUNIT_ASSERT(0<=pos);
        CPPUNIT_ASSERT(pos<15);
        CPPUNIT_ASSERT_EQUAL(x,pos2X(pos));
        CPPUNIT_ASSERT_EQUAL(y,pos2Y(pos));
      }
    CPPUNIT_ASSERT_EQUAL(makePosition('A','1'),makePosition(2,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','2'),makePosition(2,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','3'),makePosition(2,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','4'),makePosition(2,3));
    CPPUNIT_ASSERT_EQUAL(makePosition('A','5'),makePosition(2,4));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','1'),makePosition(1,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','2'),makePosition(1,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','3'),makePosition(1,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','4'),makePosition(1,3));
    CPPUNIT_ASSERT_EQUAL(makePosition('B','5'),makePosition(1,4));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','1'),makePosition(0,0));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','2'),makePosition(0,1));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','3'),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','4'),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(makePosition('C','5'),makePosition(0,4));
  }
  void testRot180(){
    for(int y=0;y<5;y++)
      for(int x=0;x<3;x++){
        int pos=makePosition(x,y);
        int rpos=rot180(pos);
        CPPUNIT_ASSERT_EQUAL(2-x,pos2X(rpos));
        CPPUNIT_ASSERT_EQUAL(4-y,pos2Y(rpos));
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
    Move m1("+C3C4GO");
    Move m2("+C3C4SI");
    Move m3("-C3C4KI");
    Move m4("+B4C4GO");
    Move m5("+C3B3GO");
    CPPUNIT_ASSERT(!(m1==m2));
    CPPUNIT_ASSERT(!(m1==m3));
    CPPUNIT_ASSERT(!(m1==m4));
    CPPUNIT_ASSERT(!(m1==m5));
    CPPUNIT_ASSERT_EQUAL(m1.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m1.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m1.player(),BLACK);
    CPPUNIT_ASSERT(m1.ptype()==Ptype::GOLD);
    CPPUNIT_ASSERT_EQUAL(m2.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m2.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m2.player(),BLACK);
    CPPUNIT_ASSERT(m2.ptype()==Ptype::SILVER);
    CPPUNIT_ASSERT_EQUAL(m3.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m3.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m3.player(),WHITE);
    CPPUNIT_ASSERT(m3.ptype()==Ptype::KING);
    CPPUNIT_ASSERT_EQUAL(m4.from(),makePosition(1,3));
    CPPUNIT_ASSERT_EQUAL(m4.to(),makePosition(0,3));
    CPPUNIT_ASSERT_EQUAL(m4.player(),BLACK);
    CPPUNIT_ASSERT(m4.ptype()==Ptype::GOLD);
    CPPUNIT_ASSERT_EQUAL(m5.from(),makePosition(0,2));
    CPPUNIT_ASSERT_EQUAL(m5.to(),makePosition(1,2));
    CPPUNIT_ASSERT_EQUAL(m5.player(),BLACK);
    CPPUNIT_ASSERT(m5.ptype()==Ptype::GOLD);
  }
  void testReverse() {
    {
      Move m1("+C3C4GO");
      Move m2=m1.rotateChangeTurn();
      CPPUNIT_ASSERT_EQUAL(Move("-A3A2GO"),m2);
      CPPUNIT_ASSERT_EQUAL(m1,m2.rotateChangeTurn());
    }
    {
      Move m1("+00C4GO");
      Move m2=m1.rotateChangeTurn();
      CPPUNIT_ASSERT_EQUAL(Move("-00A2GO"),m2);
      CPPUNIT_ASSERT_EQUAL(m1,m2.rotateChangeTurn());
    }
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION(MoveTest);

/* TODO */
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
      "-GO-KI-SI"
      " .  .  . "
      " .  .  . "
      " .  .  . "
      "+SI+KI+GO"
      "000000"
      "+");
    State s1;
    CPPUNIT_ASSERT(s==s1);
  }
  void testEqual() {
    {
      // まずは簡単なパターン
      State s(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " .  .  . "
	      " . +KI . "
	      "000000"
	      "+");
      State s1;
      State s2(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " .  .  . "
	      " . +KI . "
	      "000000"
	      "+");
      CPPUNIT_ASSERT(s!=s1);
      CPPUNIT_ASSERT(s==s2);
    }
   /* {
      // 持ち駒の違いを認識
      State s1(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " . +KI . "
	      "100100"
	      "+");
      State s2(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " . +KI . "
	      "200000"
	      "+");
      State s3(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " . +KI . "
	      "000200"
	      "+");
      CPPUNIT_ASSERT(s1!=s2);
      CPPUNIT_ASSERT(s1!=s3);
      CPPUNIT_ASSERT(s2!=s3);
    }
    */
    { // 手番の違いを認識
      State s1(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " .  .  . "
	      " . +KI . "
	      "100100"
	      "+");
      State s2(
	      " . +GO-SI"
	      "+GO+SI-KI"
	      " .  .  . "
	      " .  .  . "
	      " . +KI . "
	      "100100"
	      "-");
      CPPUNIT_ASSERT(s1!=s2);
    }
  }
  void testIsWin() {
    {
      State s(
	      " . -GO-SI"
	      "+GO . -KI"
	      " . +SI . "
	      " . +KI . "
	      " .  .  . "
	      "000000"
	      "+");
      CPPUNIT_ASSERT(s.isWin());
    }
    {
      State s(
	      " .  .  . "
	      " . -KI . "
	      " . -SI . "
	      "+KI . -GO"
	      "+SI+GO . "
	      "000000"
	      "-");
      CPPUNIT_ASSERT(s.isWin());
    }
    {
      State s(
	      " .  . -SI"
	      "-GO+SI-KI"
	      " . +GO . "
	      " . +KI . "
	      " .  .  . "
	      "000000"
	      "+");
      CPPUNIT_ASSERT(!s.isWin());
    }
    {
      State s(
	      " .  .  . "
	      " . -KI . "
	      " . -GO . "
	      "+KI-SI+GO"
	      "+SI .  . "
	      "000000"
	      "-");
      CPPUNIT_ASSERT(!s.isWin());
    }
  }
  void testIsLose() {
    {
      State s(
	      " . +GO-SI"
	      "-GO+SI . "
	      " .  . +KI"
	      " .  .  . "
	      " . -KI . "
	      "000000"
	      "+");
      CPPUNIT_ASSERT(s.isLose());
    }
    {
      State s(
	      " . +KI . "
	      " .  .  . "
	      "-KI .  . "
	      " . -SI-GO"
	      "+SI-GO . "
	      "000000"
	      "-");
      CPPUNIT_ASSERT(s.isLose());
    }
  }
  void testRotateChangeTurn() {
    {
      State s(
        " . -SI-GO"
        " .  . -KI"
        " .  .  . "
        "+SI .  . "
        "+KI .  . "
        "000000"
        "+"
        );
      State s1(
        " .  . -KI"
        " .  . -SI"
        " .  .  . "
        "+KI .  . "
        "+GO+SI . "
        "000000"
        "-");
      State s2(
        " . -SI-GO"
        " .  . -KI"
        " .  .  . "
        "+SI .  . "
        "+KI .  . "
        "000000"
        "-"
        );
      CPPUNIT_ASSERT_EQUAL(s1,s.rotateChangeTurn());
      CPPUNIT_ASSERT(s2!=s.rotateChangeTurn());
    }
    {
      State s(
        " . -SI-GO"
        " .  .  . "
        " .  . -KI"
        "+SI .  . "
        "+KI .  . "
        "000000"
        "+"
        );
      State s1(
        " .  . -KI"
        " .  . -SI"
        "+KI .  . "
        " .  .  . "
        "+GO+SI . "
        "000000"
        "-");
      CPPUNIT_ASSERT_EQUAL(s1,s.rotateChangeTurn());
    }
  }
  void testFlip() {
    State s(
      " . -SI-GO"
      " .  . -KI"
      " .  .  . "
      "+SI .  . "
      "+KI .  . "
      "000000"
      "+"
      );
    State s1(
      "-GO-SI . "
      "-KI .  . "
      " .  .  . "
      " .  . +SI"
      " .  . +KI"
      "000000"
      "+"
      );
    CPPUNIT_ASSERT_EQUAL(s1,s.flip());
  }
  void testNormalize() {
    State s1(
      " . -SI-GO"
      " .  . -KI"
      " .  .  . "
      "+SI .  . "
      "+KI .  . "
      "000000"
      "+"
      );
    uint64 u1=s1.normalize();
    State s2(
      "-GO-SI . "
      "-KI .  . "
      " .  .  . "
      " .  . +SI"
      " .  . +KI"
      "000000"
      "+"
      );
    uint64 u2=s2.normalize();
    State s3(
      " .  . -KI"
      " .  . -SI"
      " .  .  . "
      "+KI .  . "
      "+GO+SI . "
      "000000"
      "-"
      );
    uint64 u3=s3.normalize();
    State s4(
      "-KI .  . "
      "-SI .  . "
      " .  .  . "
      " .  . +KI"
      " . +SI+GO"
      "000000"
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
    {
      State s;
      State s1;
      State s2 = s1.flip();
      uint64 u = s.normalize();
      CPPUNIT_ASSERT( State(u,BLACK)==s1 || State(u,BLACK)==s2 );
    }
  }
  void testMoveGen() {
    {
      State s(
	      " . -SI-GO"
	      " .  . -KI"
	      " .  .  . "
	      "+SI . +GO"
	      "+KI .  . "
	      "000000"
	      "+"
	    );
      CPPUNIT_ASSERT(!s.isWin());
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(6==moves.size());
      // King
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A5B5KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A5B4KI"))!=moves.end());
      // Gold
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+C4C3GO"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+C4B4GO"))!=moves.end());
      // Silver
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A4A3SI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A4B3SI"))!=moves.end());
    }
    {
      State s(
	      " .  . -KI"
	      "-GO . -SI"
	      " .  .  . "
	      "+KI .  . "
	      "+GO+SI . "
	      "000000"
	      "-");
      CPPUNIT_ASSERT(!s.isWin());
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(6==moves.size());
      // King
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C1B1KI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C1B2KI"))!=moves.end());
      // Gold
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-A2A3GO"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-A2B2GO"))!=moves.end());
      // Silver
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C2B3SI"))!=moves.end());
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("-C2C3SI"))!=moves.end());
    }
  }
  void testApplyMove() {
    {
      State s(
        "-GO .  . "
        "+GO-SI-KI"
        " .  .  . "
        "+SI .  . "
        "+KI .  . "
        "000000"
        "+"
        );
      vMove moves=s.nextMoves();
      CPPUNIT_ASSERT(find(moves.begin(),moves.end(),Move("+A2A1GO"))!=moves.end());
      s.applyMove(Move("+A2A1GO"));
      State s2(
        "+GO .  . "
        " . -SI-KI"
        " .  .  . "
        "+SI .  . "
        "+KI .  . "
        "000000"
        "-"
        );
      CPPUNIT_ASSERT_EQUAL(s,s2);
    }
    {
      State s(
        " .  . -GO"
        " . -SI-KI"
        " .  .  . "
        "+SI . +GO"
        "+KI .  . "
        "000000"
        "+"
        );
      {
        State s1(s);
        s1.applyMove(Move("+A4A3SI"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          "+SI .  . "
          " .  . +GO"
          "+KI .  . "
          "000000"
          "-"
        );
        CPPUNIT_ASSERT(s1==s2);
      }
      {
        State s1(s);
        s1.applyMove(Move("+A4B3SI"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          " . +SI . "
          " .  . +GO"
          "+KI .  . "
          "000000"
          "-"
        );
        CPPUNIT_ASSERT(s1==s2);
      }
      {
        State s1(s);
        s1.applyMove(Move("+C4B4GO"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          " .  .  . "
          "+SI+GO . "
          "+KI .  . "
          "000000"
          "-"
        );
        if(s1!=s2){
          std::cerr << s1 << std::endl;
        }
        CPPUNIT_ASSERT(s1==s2);
      }
      {
        State s1(s);
        s1.applyMove(Move("+C4C3GO"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          " .  . +GO"
          "+SI .  . "
          "+KI .  . "
          "000000"
          "-"
        );
        CPPUNIT_ASSERT(s1==s2);
      }
      {
        State s1(s);
        s1.applyMove(Move("+A5B4KI"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          " .  .  . "
          "+SI+KI+GO"
          " .  .  . "
          "000000"
          "-"
        );
        CPPUNIT_ASSERT(s1==s2);
      }
      {
        State s1(s);
        s1.applyMove(Move("+A5B5KI"));
        State s2(
          " .  . -GO"
          " . -SI-KI"
          " .  .  . "
          "+SI . +GO"
          " . +KI . "
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
        " . -SI-GO"
        " .  . -KI"
        " .  .  . "
        "+SI .  . "
        "+KI .  . "
        "000000"
        "+"
      );
      CPPUNIT_ASSERT(s.isConsistent());
      State s1(
        " . -SI-GO"
        " .  . -KI"
        " .  .  . "
        "+SI . +GO"
        "+KI .  . "
        "000000"
        "+"
      );
      CPPUNIT_ASSERT(s1.isConsistent());
      State s2(
        " . -SI-GO"
        "+GO . -KI"
        " .  .  . "
        "+SI . +GO"
        "+KI .  . "
        "000000"
        "+"
      );
      CPPUNIT_ASSERT(!s2.isConsistent());
      State s3(
        " . -SI-GO"
        " .  . -KI"
        " .  .  . "
        "+SI . -GO"
        "+KI .  . "
        "000000"
        "+"
      );
      CPPUNIT_ASSERT(!s3.isConsistent());
    }
  }
};
  CPPUNIT_TEST_SUITE_REGISTRATION(StateTest);
