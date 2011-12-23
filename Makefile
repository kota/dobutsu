#all : makeAllState makeWinLose checkState checkcsa findDropBaby findZugZwang longestWin maxbf makeCheckmate checkCheckState testAll
all : makeAllState makeWinLose checkState testAll
#CXX = g++-4.3
CXX = /usr/bin/g++
# CXX = g++-4 -m64
# CXXFLAGS = -Wno-deprecated -O0 -g
CXXFLAGS = -Wall -Wno-deprecated -DNDEBUG -O3
countState : countState.o
	$(CXX) -o countState countState.o

allStateTable.o : allStateTable.h dobutsu.h
winLoseTable.o : winLoseTable.h dobutsu.h
dobutsu.o : dobutsu.h
checkcsa.o : dobutsu.h
checkcsa : checkcsa.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o checkcsa checkcsa.o dobutsu.o  allStateTable.o winLoseTable.o
checkState.o : dobutsu.h
checkState : checkState.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o checkState checkState.o dobutsu.o  allStateTable.o winLoseTable.o

#
longestWin.o : dobutsu.h
longestWin : longestWin.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o longestWin longestWin.o dobutsu.o  allStateTable.o winLoseTable.o
# 
checkCheckState.o : dobutsu.h
checkCheckState : checkCheckState.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o checkCheckState checkCheckState.o dobutsu.o  allStateTable.o winLoseTable.o
# 
checkStatePass.o : dobutsu.h
checkStatePass : checkStatePass.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o checkStatePass checkStatePass.o dobutsu.o  allStateTable.o winLoseTable.o
# 
countWinLoseCheck.o : dobutsu.h
countWinLoseCheck : countWinLoseCheck.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o countWinLoseCheck countWinLoseCheck.o dobutsu.o  allStateTable.o winLoseTable.o



findDropBaby.o : dobutsu.h
findDropBaby : findDropBaby.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o findDropBaby findDropBaby.o dobutsu.o  allStateTable.o winLoseTable.o

makeCheckmate.o : dobutsu.h winLoseTable.h allStateTable.h
makeCheckmate : makeCheckmate.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o makeCheckmate makeCheckmate.o dobutsu.o  allStateTable.o winLoseTable.o

countReachable.o : dobutsu.h winLoseTable.h allStateTable.h
countReachable : countReachable.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o countReachable countReachable.o dobutsu.o  allStateTable.o winLoseTable.o
maxbf.o : dobutsu.h winLoseTable.h allStateTable.h
maxbf : maxbf.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o maxbf maxbf.o dobutsu.o  allStateTable.o winLoseTable.o
test.o : dobutsu.h
test : test.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o test test.o dobutsu.o allStateTable.o winLoseTable.o
checkWinLose.o : dobutsu.h
checkWinLose : checkWinLose.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o checkWinLose checkWinLose.o dobutsu.o allStateTable.o winLoseTable.o
findZugZwang.o : dobutsu.h
findZugZwang : findZugZwang.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o findZugZwang findZugZwang.o dobutsu.o allStateTable.o winLoseTable.o
makeAllState.o : dobutsu.h
makeAllState : makeAllState.o dobutsu.o
	$(CXX) -o makeAllState makeAllState.o dobutsu.o
makeWinLose.o : dobutsu.h
makeWinLose : makeWinLose.o dobutsu.o allStateTable.o
	$(CXX) -o makeWinLose makeWinLose.o dobutsu.o allStateTable.o

makeWinLoseWoDrop.o : dobutsu.h
makeWinLoseWoDrop : makeWinLoseWoDrop.o dobutsu.o allStateTable.o
	$(CXX) -o makeWinLoseWoDrop makeWinLoseWoDrop.o dobutsu.o allStateTable.o

makeWinLosePass.o : dobutsu.h
makeWinLosePass : makeWinLosePass.o dobutsu.o allStateTable.o
	$(CXX) -o makeWinLosePass makeWinLosePass.o dobutsu.o allStateTable.o


stateDiff.o : dobutsu.h
stateDiff : stateDiff.o dobutsu.o allStateTable.o
	$(CXX) -o stateDiff stateDiff.o dobutsu.o allStateTable.o winLoseTable.o

traceDraw.o : dobutsu.h
traceDraw : traceDraw.o dobutsu.o allStateTable.o
	$(CXX) -o traceDraw traceDraw.o dobutsu.o allStateTable.o winLoseTable.o

minTree.o : dobutsu.h
minTree : minTree.o dobutsu.o allStateTable.o winLoseTable.o
	$(CXX) -o minTree minTree.o dobutsu.o allStateTable.o winLoseTable.o


testAll.o : dobutsu.h
testAll : testAll.o dobutsu.o
	$(CXX) -o testAll testAll.o dobutsu.o -L/usr/local/Cellar/cppunit/1.12.1/lib/ -lcppunit
