CXXFLAGS=-Wall -std=c++17 -O0

TestAluFpu: TestAluFpu.o InstructionsPortable.o
	$(CXX) TestAluFpu.o InstructionsPortable.o -o $@
  
TestAluFpu.o: TestAluFpu.cpp
InstructionsPortable.o: InstructionsPortable.cpp

clean:
	rm -f TestAluFpu TestAluFpu.o InstructionsPortable.o