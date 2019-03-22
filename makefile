#CXX=g++-8
#CC=gcc-8
PLATFORM=$(shell uname -m)
CXXFLAGS=-std=c++11
CCFLAGS=
BINDIR=bin
SRCDIR=src
OBJDIR=obj
LDFLAGS=-lpthread
CPPSRC=src/argon2_core.c src/Cache.cpp src/divideByConstantCodegen.c src/Instruction.cpp src/JitCompilerX86.cpp src/Program.cpp src/VirtualMachine.cpp src/argon2_ref.c src/CompiledVirtualMachine.cpp src/executeProgram-linux.cpp src/instructionsPortable.cpp src/LightClientAsyncWorker.cpp src/softAes.cpp src/virtualMemory.cpp src/AssemblyGeneratorX86.cpp  src/dataset.cpp src/hashAes1Rx4.cpp src/InterpretedVirtualMachine.cpp src/main.cpp src/TestAluFpu.cpp src/blake2/blake2b.c
TOBJS=$(addprefix $(OBJDIR)/,instructionsPortable.o TestAluFpu.o)
ROBJS=$(addprefix $(OBJDIR)/,argon2_core.o argon2_ref.o AssemblyGeneratorX86.o blake2b.o CompiledVirtualMachine.o CompiledLightVirtualMachine.o dataset.o JitCompilerX86.o instructionsPortable.o Instruction.o InterpretedVirtualMachine.o main.o Program.o softAes.o VirtualMachine.o Cache.o virtualMemory.o reciprocal.o LightClientAsyncWorker.o hashAes1Rx4.o)
ifeq ($(PLATFORM),amd64)
    ROBJS += $(OBJDIR)/JitCompilerX86-static.o $(OBJDIR)/squareHash.o
    CXXFLAGS += -maes
endif
ifeq ($(PLATFORM),x86_64)
    ROBJS += $(OBJDIR)/JitCompilerX86-static.o $(OBJDIR)/squareHash.o
    CXXFLAGS += -maes
endif

all: release

release: CXXFLAGS += -march=native -O3 -flto
release: CCFLAGS += -march=native -O3 -flto
release: LDFLAGS += -flto
release: $(BINDIR)/randomx

nolto: CXXFLAGS += -march=native -O3
nolto: CCFLAGS += -march=native -O3
nolto: $(BINDIR)/randomx

debug: CXXFLAGS += -g
debug: CCFLAGS += -g
debug: LDFLAGS += -g
debug: $(BINDIR)/randomx

profile: CXXFLAGS += -pg
profile: CCFLAGS += -pg
profile: LDFLAGS += -pg
profile: $(BINDIR)/randomx

test: CXXFLAGS += -O0
test: $(BINDIR)/AluFpuTest

$(BINDIR)/randomx: $(ROBJS) | $(BINDIR)
	$(CXX) $(ROBJS) $(LDFLAGS) -o $@ 

$(BINDIR)/AluFpuTest: $(TOBJS) | $(BINDIR)
	$(CXX) $(TOBJS) $(LDFLAGS) -o $@
  
$(OBJDIR)/TestAluFpu.o: $(addprefix $(SRCDIR)/,TestAluFpu.cpp instructions.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/TestAluFpu.cpp -o $@
  
$(OBJDIR)/argon2_core.o: $(addprefix $(SRCDIR)/,argon2_core.c argon2_core.h blake2/blake2.h blake2/blake2-impl.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/argon2_core.c -o $@
  
$(OBJDIR)/argon2_ref.o: $(addprefix $(SRCDIR)/,argon2_ref.c argon2.h argon2_core.h blake2/blake2.h blake2/blake2-impl.h blake2/blamka-round-ref.h blake2/endian.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/argon2_ref.c -o $@

$(OBJDIR)/AssemblyGeneratorX86.o: $(addprefix $(SRCDIR)/,AssemblyGeneratorX86.cpp AssemblyGeneratorX86.hpp Instruction.hpp common.hpp instructionWeights.hpp blake2/endian.h reciprocal.h Program.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/AssemblyGeneratorX86.cpp -o $@

$(OBJDIR)/blake2b.o: $(addprefix $(SRCDIR)/blake2/,blake2b.c blake2.h blake2-impl.h endian.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/blake2/blake2b.c -o $@

$(OBJDIR)/CompiledVirtualMachine.o: $(addprefix $(SRCDIR)/,CompiledVirtualMachine.cpp CompiledVirtualMachine.hpp common.hpp configuration.h JitCompilerX86.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/CompiledVirtualMachine.cpp -o $@

$(OBJDIR)/CompiledLightVirtualMachine.o: $(addprefix $(SRCDIR)/,CompiledLightVirtualMachine.cpp CompiledLightVirtualMachine.hpp common.hpp configuration.h JitCompilerX86.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/CompiledLightVirtualMachine.cpp -o $@
  
$(OBJDIR)/dataset.o: $(addprefix $(SRCDIR)/,dataset.cpp common.hpp blake2/endian.h dataset.hpp intrinPortable.h Cache.hpp virtualMemory.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/dataset.cpp -o $@

$(OBJDIR)/reciprocal.o: $(addprefix $(SRCDIR)/,reciprocal.c reciprocal.h) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c $(SRCDIR)/reciprocal.c -o $@

$(OBJDIR)/hashAes1Rx4.o: $(addprefix $(SRCDIR)/,hashAes1Rx4.cpp softAes.h intrinPortable.h blake2/endian.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/hashAes1Rx4.cpp -o $@

$(OBJDIR)/JitCompilerX86.o: $(addprefix $(SRCDIR)/,JitCompilerX86.cpp JitCompilerX86.hpp Instruction.hpp instructionWeights.hpp common.hpp blake2/endian.h Program.hpp reciprocal.h virtualMemory.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/JitCompilerX86.cpp -o $@

$(OBJDIR)/JitCompilerX86-static.o: $(addprefix $(SRCDIR)/,JitCompilerX86-static.S $(addprefix asm/program_, prologue_linux.inc prologue_load.inc epilogue_linux.inc epilogue_store.inc read_dataset.inc loop_load.inc loop_store.inc xmm_constants.inc read_dataset_light.inc read_dataset_light_sub.inc)) | $(OBJDIR)
	$(CXX) -x assembler-with-cpp -c $(SRCDIR)/JitCompilerX86-static.S -o $@

$(OBJDIR)/squareHash.o: $(addprefix $(SRCDIR)/,squareHash.S $(addprefix asm/, squareHash.inc initBlock.inc) configuration.h)  | $(OBJDIR)
	$(CXX) -x assembler-with-cpp -c $(SRCDIR)/squareHash.S -o $@

$(OBJDIR)/instructionsPortable.o: $(addprefix $(SRCDIR)/,instructionsPortable.cpp intrinPortable.h blake2/endian.h common.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/instructionsPortable.cpp -o $@

$(OBJDIR)/Instruction.o: $(addprefix $(SRCDIR)/,Instruction.cpp Instruction.hpp instructionWeights.hpp blake2/endian.h common.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Instruction.cpp -o $@
  
$(OBJDIR)/InterpretedVirtualMachine.o: $(addprefix $(SRCDIR)/,InterpretedVirtualMachine.cpp InterpretedVirtualMachine.hpp instructionWeights.hpp VirtualMachine.hpp common.hpp blake2/endian.h Program.hpp Instruction.hpp intrinPortable.h dataset.hpp Cache.hpp virtualMemory.hpp LightClientAsyncWorker.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/InterpretedVirtualMachine.cpp -o $@

$(OBJDIR)/LightClientAsyncWorker.o: $(addprefix $(SRCDIR)/,LightClientAsyncWorker.cpp LightClientAsyncWorker.hpp common.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/LightClientAsyncWorker.cpp -o $@
  
$(OBJDIR)/main.o: $(addprefix $(SRCDIR)/,main.cpp InterpretedVirtualMachine.hpp Stopwatch.hpp blake2/blake2.h VirtualMachine.hpp common.hpp blake2/endian.h Program.hpp Instruction.hpp intrinPortable.h CompiledVirtualMachine.hpp JitCompilerX86.hpp AssemblyGeneratorX86.hpp dataset.hpp Cache.hpp virtualMemory.hpp hashAes1Rx4.hpp softAes.h configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.cpp -o $@
  
$(OBJDIR)/Program.o: $(addprefix $(SRCDIR)/,Program.cpp Program.hpp configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Program.cpp -o $@

$(OBJDIR)/Cache.o: $(addprefix $(SRCDIR)/,Cache.cpp Cache.hpp argon2_core.h configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Cache.cpp -o $@
  
$(OBJDIR)/softAes.o: $(addprefix $(SRCDIR)/,softAes.cpp softAes.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/softAes.cpp -o $@
  
$(OBJDIR)/VirtualMachine.o: $(addprefix $(SRCDIR)/,VirtualMachine.cpp VirtualMachine.hpp common.hpp dataset.hpp blake2/endian.h Program.hpp Instruction.hpp hashAes1Rx4.hpp softAes.h intrinPortable.h blake2/blake2.h configuration.h) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/VirtualMachine.cpp -o $@

$(OBJDIR)/virtualMemory.o: $(addprefix $(SRCDIR)/,virtualMemory.cpp virtualMemory.hpp) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/virtualMemory.cpp -o $@
  
$(OBJDIR):
	mkdir $(OBJDIR)
  
$(BINDIR):
	mkdir $(BINDIR)

clean:
	rm -f $(BINDIR)/randomx $(BINDIR)/AluFpuTest $(OBJDIR)/*.o
