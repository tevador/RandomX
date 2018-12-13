#include "common.hpp"
#include <stdexcept>

namespace RandomX {
  extern "C" {
      void executeProgram(RegisterFile& registerFile, MemoryRegisters& memory, DatasetReadFunc readFunc, convertible_t* scratchpad) {
        throw std::runtime_error("not implemented");
      }
  }
}