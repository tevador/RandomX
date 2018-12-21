#CXX=g++-8
#CC=gcc-8
PLATFORM=$(shell uname -i)
CXXFLAGS=-std=c++17
CCFLAGS=
ifeq ($(PLATFORM),x86_64)
    CXXFLAGS += -maes
endif
BINDIR=bin
SRCDIR=src
OBJDIR=obj
LDFLAGS=-lpthread
TOBJS=$(addprefix $(OBJDIR)/,instructionsPortable.o TestAluFpu.o)
ROBJS=$(addprefix $(OBJDIR)/,argon2_core.o argon2_ref.o AssemblyGeneratorX86.o blake2b.o CompiledVirtualMachine.o dataset.o JitCompilerX86.o instructionsPortable.o Instruction.o InterpretedVirtualMachine.o main.o Program.o softAes.o VirtualMachine.o t1ha2.o Cache.o)

all: release test

release: CXXFLAGS += -march=native -O3 -flto
release: CCFLAGS += -march=native -O3 -flto
release: $(BINDIR)/randomx

debug: CXXFLAGS += -g
debug: CCFLAGS += -g
debug: LDFLAGS += -g
debug: $(BINDIR)/randomx

test: CXXFLAGS += -O0
test: $(BINDIR)/AluFpuTest

$(BINDIR)/randomx: $(ROBJS) | $(BINDIR)
	$(CXX) $(ROBJS) $(LDFLAGS) -o $@ 

$(BINDIR)/AluFpuTest: $(TOBJS) | $(BINDIR)
	$(CXX) $(TOBJS) $(LDFLAGS) -o $@
  
$(OBJDIR)/TestAluFpu.o: $(addprefix $(SRCDIR)/,TestAluFpu.cpp instructions.hpp Pcg32.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/TestAluFpu.cpp -o $@
  
$(OBJDIR)/argon2_core.o: $(addprefix $(SRCDIR)/,argon2_core.c argon2_core.h blake2/blake2.h blake2/blake2-impl.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/argon2_core.c -o $@
  
$(OBJDIR)/argon2_ref.o: $(addprefix $(SRCDIR)/,argon2_ref.c argon2.h argon2_core.h blake2/blake2.h blake2/blake2-impl.h blake2/blamka-round-ref.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/argon2_ref.c -o $@

$(OBJDIR)/AssemblyGeneratorX86.o: $(addprefix $(SRCDIR)/,AssemblyGeneratorX86.cpp AssemblyGeneratorX86.hpp Instruction.hpp Pcg32.hpp common.hpp instructions.hpp instructionWeights.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/AssemblyGeneratorX86.cpp -o $@

$(OBJDIR)/blake2b.o: $(addprefix $(SRCDIR)/blake2/,blake2b.c blake2.h blake2-impl.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/blake2/blake2b.c -o $@

$(OBJDIR)/CompiledVirtualMachine.o: $(addprefix $(SRCDIR)/,CompiledVirtualMachine.cpp CompiledVirtualMachine.hpp Pcg32.hpp common.hpp instructions.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/CompiledVirtualMachine.cpp -o $@
  
$(OBJDIR)/dataset.o: $(addprefix $(SRCDIR)/,dataset.cpp common.hpp Pcg32.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/dataset.cpp -o $@

$(OBJDIR)/JitCompilerX86.o: $(addprefix $(SRCDIR)/,JitCompilerX86.cpp JitCompilerX86.hpp Instruction.hpp instructionWeights.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/JitCompilerX86.cpp -o $@

$(OBJDIR)/instructionsPortable.o: $(addprefix $(SRCDIR)/,instructionsPortable.cpp instructions.hpp intrinPortable.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/instructionsPortable.cpp -o $@

$(OBJDIR)/Instruction.o: $(addprefix $(SRCDIR)/,Instruction.cpp Instruction.hpp instructionWeights.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Instruction.cpp -o $@
  
$(OBJDIR)/InterpretedVirtualMachine.o: $(addprefix $(SRCDIR)/,InterpretedVirtualMachine.cpp InterpretedVirtualMachine.hpp Pcg32.hpp instructions.hpp instructionWeights.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/InterpretedVirtualMachine.cpp -o $@
  
$(OBJDIR)/main.o: $(addprefix $(SRCDIR)/,main.cpp InterpretedVirtualMachine.hpp Stopwatch.hpp blake2/blake2.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.cpp -o $@
  
$(OBJDIR)/Program.o: $(addprefix $(SRCDIR)/,Program.cpp Program.hpp Pcg32.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Program.cpp -o $@

$(OBJDIR)/Cache.o: $(addprefix $(SRCDIR)/,Cache.cpp Cache.hpp Pcg32.hpp argon2_core.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Cache.cpp -o $@
  
$(OBJDIR)/softAes.o: $(addprefix $(SRCDIR)/,softAes.cpp softAes.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/softAes.cpp -o $@
  
$(OBJDIR)/VirtualMachine.o: $(addprefix $(SRCDIR)/,VirtualMachine.cpp VirtualMachine.hpp common.hpp dataset.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/VirtualMachine.cpp -o $@

$(OBJDIR)/t1ha2.o: $(addprefix $(SRCDIR)/t1ha/,t1ha2.c t1ha.h t1ha_bits.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/t1ha/t1ha2.c -o $@
  
$(OBJDIR):
	mkdir $(OBJDIR)
  
$(BINDIR):
	mkdir $(BINDIR)

clean:
	rm -f $(BINDIR)/randomx $(BINDIR)/AluFpuTest $(OBJDIR)/*.o