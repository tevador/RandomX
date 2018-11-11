import random
import sys
import os

PROGRAM_SIZE = 512
INSTRUCTION_COUNT = 1024 * 1024
        
def genBytes(count):
    return ', '.join(str(random.getrandbits(8)) for i in range(count))
        
class OperandType:
    INT32 = 0
    UINT32 = 1
    INT64 = 2
    UINT64 = 3
    FLOAT = 4
    SHIFT = 5

def declareType(type):
    converters = {
        0: "int32_t",
        1: "uint32_t",
        2: "int64_t",
        3: "uint64_t",
        4: "double",
        5: "int32_t"
    }
    return converters.get(type)
    
def toSigned32(x):
    return x - ((x & 0x80000000) << 1)
    
def toSigned64(x):
    return x - ((x & 0x8000000000000000) << 1)

def immediateTo(symbol, type):
    converters = {
        0: toSigned32(symbol.imm1),
        1: symbol.imm1,
        2: toSigned32(symbol.imm1),
        3: symbol.imm1,
        4: float(toSigned32(symbol.imm1) << 32),
        5: symbol.imm0 & 63
    }
    return repr(converters.get(type))
    
def registerTo(expr, type):
    converters = {
        0: "(int64_t){0}",
        1: "{0}",
        2: "(int64_t){0}",
        3: "{0}",
        4: "{0}",
        5: "({0} & 63)"
    }
    return converters.get(type).format(expr)
    
def registerFrom(num, type):
    converters = {
        0: "r{0}",
        1: "r{0}",
        2: "r{0}",
        3: "r{0}",
        4: "((convertible_t)f{0}).u64",
        5: "r{0}"
    }
    return converters.get(type).format(num)
    
def convertibleTo(expr, type):
    converters = {
        0: "{0}.i32",
        1: "{0}.u32",
        2: "{0}.i64",
        3: "{0}.u64",
        4: "(double){0}.i64",
        5: "({0}.u64 & 63)"
    }
    return converters.get(type).format(expr)
    
def convertibleFrom(expr, type):
    converters = {
        0: "{0}.i32",
        1: "{0}.u32",
        2: "{0}.i64",
        3: "{0}.u64",
        4: "{0}.f64",
        5: "({0}.u64 & 63)"
    }
    return converters.get(type).format(expr)

def getRegister(num, type):
    registers = {
        0: "r{0}",
        1: "r{0}",
        2: "r{0}",
        3: "r{0}",
        4: "f{0}",
        5: "r{0}"
    }
    return registers.get(type).format(num)

def writeInitialValues(file):
    file.write("#ifdef RAM\n")
    file.write("\tmmu.buffer = (char*)_mm_malloc(DRAM_SIZE, 16);\n")
    file.write("\tif(!mmu.buffer) {\n")
    file.write('\t\tprintf("DRAM buffer allocation failed\\n");\n')
    file.write("\t\treturn 1;\n")
    file.write("\t}\n")
    file.write('\tprintf("Initializing DRAM buffer...\\n");\n')
    file.write("\taesInitialize((__m128i*)aesKey, (__m128i*)aesSeed, (__m128i*)mmu.buffer, DRAM_SIZE);\n")
    file.write("#endif\n")
    file.write("\tclock_t clockStart = clock(), clockEnd;\n")
    for i in range(8):
        file.write("\tr{0} = *(uint64_t*)(aesSeed + {1});\n".format(i, i * 8))
    for i in range(8):
        file.write("\tf{0} = *(int64_t*)(aesSeed + {1});\n".format(i, 64 + i * 8))
    file.write("\tmmu.m0 = (aesKey[9] << 8) | (aesKey[10] << 16) | (aesKey[11] << 24);\n")
    file.write("\taesInitialize((__m128i*)aesKey, (__m128i*)aesSeed, (__m128i*)scratchpad, SCRATCHPAD_SIZE);\n")
    file.write("\tmmu.mx = 0;\n")
    file.write("\tsp = 0;\n")
    file.write("\tic = {0};\n".format(INSTRUCTION_COUNT))
    file.write("\tmxcsr = (_mm_getcsr() | _MM_FLUSH_ZERO_ON) & ~_MM_ROUND_MASK; //flush denormals to zero, round to nearest\n")
    file.write("\t_mm_setcsr(mxcsr);\n")
    
def writeEpilog(file):
    file.write("\tend:\n")
    file.write("\t\tclockEnd = clock();\n")
    for i in range(8):
        file.write('\t\tprintf("r{0} = %-36lu f{0} = %g\\n", r{0}, f{0});\n'.format(i))
    file.write(("\t\tuint64_t spadsum = 0;\n"
        "\t\tfor(int i = 0; i < SCRATCHPAD_LENGTH; ++i) {\n"
        "\t\t	spadsum += scratchpad[i].u64;\n"
        "\t\t}\n"
        '\t\tprintf("scratchpad sum = %lu\\n", spadsum);\n'
        '\t\tprintf("runtime: %f\\n", (clockEnd - clockStart) / (double)CLOCKS_PER_SEC);\n'
        "#ifdef RAM\n"
        "\t\t_mm_free((void*)mmu.buffer);\n"
        "#endif\n"))
    file.write("\t\treturn 0;")
    file.write("}")

def writeCommon(file, i, symbol, type, name):
    file.write("\ti_{0}: {{ //{1}\n".format(i, name))
    file.write("\t\tif(0 == ic--) goto end;\n")
    file.write("\t\tr{0} ^= {1};\n".format(symbol.rega, symbol.addr0))
    file.write("\t\taddr_t addr = r{0};\n".format(symbol.rega))

def readA(symbol, type):
    location = {
        0: "readDram(&mmu, addr)",
        1: "readDram(&mmu, addr)",
        2: "readDram(&mmu, addr)",
        3: "readDram(&mmu, addr)",
        4: "SCRATCHPAD_256K(addr)",
        5: "SCRATCHPAD_16K(addr)",
        6: "SCRATCHPAD_16K(addr)",
        7: "SCRATCHPAD_16K(addr)",
    }
    return convertibleTo(location.get(symbol.loca), type)

def writeC(symbol, type):
    location = {
        0: "SCRATCHPAD_256K(r{0} ^ {1})",
        1: "SCRATCHPAD_16K(r{0} ^ {1})",
        2: "SCRATCHPAD_16K(r{0} ^ {1})",
        3: "SCRATCHPAD_16K(r{0} ^ {1})",
        4: "",
        5: "",
        6: "",
        7: ""
    }
    c = location.get(symbol.locc)
    if c == "":
        c = getRegister(symbol.regc, type)
    else:
        c = convertibleFrom(c.format(symbol.regc, symbol.addr1), type)
    return c

def readB(symbol, type):
    if symbol.locb < 6:
        return registerTo(getRegister(symbol.regb, type), type)
    else:
        return immediateTo(symbol, type)

class CodeSymbol:
    def __init__(self, qi):
        self.opcode = qi & 255
        self.loca = (qi >> 8) & 7
        self.rega = (qi >> 16) & 7
        self.locb = (qi >> 24) & 7
        self.regb = (qi >> 32) & 7
        self.locc = (qi >> 40) & 7
        self.regc = (qi >> 48) & 7
        self.imm0 = (qi >> 56) & 255
        self.addr0 = (qi >> 64) & 0xFFFFFFFF
        self.addr1 = self.imm1 = qi >> 96

def writeOperation(file, i, symbol, type, name, op):
    writeCommon(file, i, symbol, type, name)
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = A {1} B; }}\n".format(writeC(symbol, type), op))

def write_ADD_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'ADD_64', '+');

def write_ADD_32(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT32, 'ADD_32', '+');

def write_SUB_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'SUB_64', '-');

def write_SUB_32(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT32, 'SUB_32', '-');

def write_MUL_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'MUL_64', '*');

def write_MULH_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'MULH_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = ((uint128_t)A * B) >> 64; }}\n".format(writeC(symbol, type)))

def write_MUL_32(file, i, symbol):
    type = OperandType.UINT32
    writeCommon(file, i, symbol, type, 'MUL_32')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = (uint64_t)A * B; }}\n".format(writeC(symbol, OperandType.UINT64)))

def write_IMUL_32(file, i, symbol):
    type = OperandType.INT32
    writeCommon(file, i, symbol, type, 'IMUL_32')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = (int64_t)A * B; }}\n".format(writeC(symbol, OperandType.INT64)))

def write_IMULH_64(file, i, symbol):
    type = OperandType.INT64
    writeCommon(file, i, symbol, type, 'IMULH_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = ((int128_t)A * B) >> 64; }}\n".format(writeC(symbol, type)))

def write_DIV_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'DIV_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.UINT32), readB(symbol, OperandType.UINT32)))
    file.write("\t\tif(B == 0) B = 1;\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = A / B; }}\n".format(writeC(symbol, type)))

def write_IDIV_64(file, i, symbol):
    type = OperandType.INT64
    writeCommon(file, i, symbol, type, 'IDIV_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.INT32), readB(symbol, OperandType.INT32)))
    file.write("\t\tif(B == 0) B = 1;\n".format(declareType(type), readB(symbol, type)))
    file.write("\t\t{0} = A / B; }}\n".format(writeC(symbol, type)))

def write_AND_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'AND_64', '&');

def write_AND_32(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT32, 'AND_32', '&');

def write_OR_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'OR_64', '|');

def write_OR_32(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT32, 'OR_32', '|');

def write_XOR_64(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT64, 'XOR_64', '^');

def write_XOR_32(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.UINT32, 'XOR_32', '^');

def write_SHL_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'SHL_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.SHIFT), readB(symbol, OperandType.SHIFT)))
    file.write("\t\t{0} = A << B; }}\n".format(writeC(symbol, type)))

def write_SHR_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'SHR_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.SHIFT), readB(symbol, OperandType.SHIFT)))
    file.write("\t\t{0} = A >> B; }}\n".format(writeC(symbol, type)))

def write_SAR_64(file, i, symbol):
    type = OperandType.INT64
    writeCommon(file, i, symbol, type, 'SAR_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.SHIFT), readB(symbol, OperandType.SHIFT)))
    file.write("\t\t{0} = A >> B; }}\n".format(writeC(symbol, type)))

def write_ROL_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'ROL_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.SHIFT), readB(symbol, OperandType.SHIFT)))
    file.write("\t\t{0} = __rolq(A, B); }}\n".format(writeC(symbol, type)))

def write_ROR_64(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'ROR_64')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} B = {1};\n".format(declareType(OperandType.SHIFT), readB(symbol, OperandType.SHIFT)))
    file.write("\t\t{0} = __rorq(A, B); }}\n".format(writeC(symbol, type)))

def write_FADD(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.FLOAT, 'FADD', '+');

def write_FSUB(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.FLOAT, 'FSUB', '-');

def write_FMUL(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.FLOAT, 'FMUL', '*');

def write_FDIV(file, i, symbol):
    writeOperation(file, i, symbol, OperandType.FLOAT, 'FDIV', '/');

def write_FSQRT(file, i, symbol):
    type = OperandType.FLOAT
    writeCommon(file, i, symbol, type, 'FSQRT')
    file.write("\t\t{0} A = fabs({1});\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\t{0} = _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_load_pd(&A))); }}\n".format(writeC(symbol, type)))

def write_FROUND(file, i, symbol):
    type = OperandType.FLOAT
    writeCommon(file, i, symbol, type, 'FROUND')
    file.write("\t\t{0} A = {1};\n".format(declareType(OperandType.INT64), readA(symbol, OperandType.INT64)))
    file.write("\t\t{0} = A;\n".format(writeC(symbol, type)))
    file.write("\t\t_mm_setcsr(mxcsr | ((uint32_t)(A << 13) & _MM_ROUND_MASK)); }\n")

def write_CALL(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'CALL')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    if symbol.locb < 6:
        file.write("\t\tif((uint32_t)r{0} <= {1}) {{\n".format(symbol.regb, symbol.imm1))
    file.write("\t\t\tPUSH_VALUE(A);\n");
    file.write("\t\t\tPUSH_ADDRESS(&&i_{0});\n".format((i + 1) & (PROGRAM_SIZE - 1)));
    file.write("\t\t\tgoto i_{0};\n".format((i + 1 + (symbol.imm0 & ((PROGRAM_SIZE >> 2) - 1))) & (PROGRAM_SIZE - 1)));
    if symbol.locb < 6:
        file.write("\t\t}}\n\t\t{0} = A;".format(writeC(symbol, type)))
    file.write("\t\t}\n")

def write_RET(file, i, symbol):
    type = OperandType.UINT64
    writeCommon(file, i, symbol, type, 'RET')
    file.write("\t\t{0} A = {1};\n".format(declareType(type), readA(symbol, type)))
    file.write("\t\tif(!STACK_IS_EMPTY()")
    if symbol.locb < 6:
        file.write(" && (uint32_t)r{0} <= {1}".format(symbol.regb, symbol.imm1))
    file.write(") {\n")
    file.write("\t\t\tvoid* target = POP_ADDRESS();\n")
    file.write("\t\t\tuint64_t C = POP_VALUE();\n")
    file.write("\t\t\t{0} = A ^ C;\n".format(writeC(symbol, type)))
    file.write("\t\t\tgoto *target;\n")
    file.write("\t\t}}\n\t\t{0} = A; }}\n".format(writeC(symbol, type)))

opcodeMap = {
    0: write_ADD_64,
    1: write_ADD_64,
    2: write_ADD_64,
    3: write_ADD_64,
    4: write_ADD_64,
    5: write_ADD_64,
    6: write_ADD_64,
    7: write_ADD_64,
    8: write_ADD_64,
    9: write_ADD_64,
    10: write_ADD_64,
    11: write_ADD_64,
    12: write_ADD_64,
    13: write_ADD_64,
    14: write_ADD_32,
    15: write_ADD_32,
    16: write_ADD_32,
    17: write_ADD_32,
    18: write_ADD_32,
    19: write_ADD_32,
    20: write_ADD_32,
    21: write_SUB_64,
    22: write_SUB_64,
    23: write_SUB_64,
    24: write_SUB_64,
    25: write_SUB_64,
    26: write_SUB_64,
    27: write_SUB_64,
    28: write_SUB_64,
    29: write_SUB_64,
    30: write_SUB_64,
    31: write_SUB_64,
    32: write_SUB_64,
    33: write_SUB_64,
    34: write_SUB_64,
    35: write_SUB_32,
    36: write_SUB_32,
    37: write_SUB_32,
    38: write_SUB_32,
    39: write_SUB_32,
    40: write_SUB_32,
    41: write_SUB_32,
    42: write_MUL_64,
    43: write_MUL_64,
    44: write_MUL_64,
    45: write_MUL_64,
    46: write_MULH_64,
    47: write_MULH_64,
    48: write_MULH_64,
    49: write_MULH_64,
    50: write_MUL_32,
    51: write_MUL_32,
    52: write_MUL_32,
    53: write_MUL_32,
    54: write_IMUL_32,
    55: write_IMUL_32,
    56: write_IMUL_32,
    57: write_IMUL_32,
    58: write_IMULH_64,
    59: write_IMULH_64,
    60: write_IMULH_64,
    61: write_IMULH_64,
    62: write_DIV_64,
    63: write_IDIV_64,
    64: write_AND_64,
    65: write_AND_64,
    66: write_AND_64,
    67: write_AND_64,
    68: write_AND_64,
    69: write_AND_64,
    70: write_AND_64,
    71: write_AND_64,
    72: write_AND_64,
    73: write_AND_64,
    74: write_AND_64,
    75: write_AND_64,
    76: write_AND_64,
    77: write_AND_32,
    78: write_AND_32,
    79: write_AND_32,
    80: write_AND_32,
    81: write_AND_32,
    82: write_AND_32,
    83: write_OR_64,
    84: write_OR_64,
    85: write_OR_64,
    86: write_OR_64,
    87: write_OR_64,
    88: write_OR_64,
    89: write_OR_64,
    90: write_OR_64,
    91: write_OR_64,
    92: write_OR_64,
    93: write_OR_64,
    94: write_OR_64,
    95: write_OR_64,
    96: write_OR_32,
    97: write_OR_32,
    98: write_OR_32,
    99: write_OR_32,
    100: write_OR_32,
    101: write_OR_32,
    102: write_XOR_64,
    103: write_XOR_64,
    104: write_XOR_64,
    105: write_XOR_64,
    106: write_XOR_64,
    107: write_XOR_64,
    108: write_XOR_64,
    109: write_XOR_64,
    110: write_XOR_64,
    111: write_XOR_64,
    112: write_XOR_64,
    113: write_XOR_64,
    114: write_XOR_64,
    115: write_XOR_64,
    116: write_XOR_32,
    117: write_XOR_32,
    118: write_XOR_32,
    119: write_XOR_32,
    120: write_XOR_32,
    121: write_XOR_32,
    122: write_SHL_64,
    123: write_SHL_64,
    124: write_SHL_64,
    125: write_SHL_64,
    126: write_SHL_64,
    127: write_SHL_64,
    128: write_SHL_64,
    129: write_SHR_64,
    130: write_SHR_64,
    131: write_SHR_64,
    132: write_SHR_64,
    133: write_SAR_64,
    134: write_SAR_64,
    135: write_SAR_64,
    136: write_ROL_64,
    137: write_ROL_64,
    138: write_ROL_64,
    139: write_ROL_64,
    140: write_ROL_64,
    141: write_ROL_64,
    142: write_ROL_64,
    143: write_ROL_64,
    144: write_ROL_64,
    145: write_ROL_64,
    146: write_ROL_64,
    147: write_ROR_64,
    148: write_ROR_64,
    149: write_ROR_64,
    150: write_ROR_64,
    151: write_ROR_64,
    152: write_ROR_64,
    153: write_ROR_64,
    154: write_ROR_64,
    155: write_ROR_64,
    156: write_ROR_64,
    157: write_ROR_64,
    158: write_FADD,
    159: write_FADD,
    160: write_FADD,
    161: write_FADD,
    162: write_FADD,
    163: write_FADD,
    164: write_FADD,
    165: write_FADD,
    166: write_FADD,
    167: write_FADD,
    168: write_FADD,
    169: write_FADD,
    170: write_FADD,
    171: write_FADD,
    172: write_FADD,
    173: write_FADD,
    174: write_FADD,
    175: write_FADD,
    176: write_FSUB,
    177: write_FSUB,
    178: write_FSUB,
    179: write_FSUB,
    180: write_FSUB,
    181: write_FSUB,
    182: write_FSUB,
    183: write_FSUB,
    184: write_FSUB,
    185: write_FSUB,
    186: write_FSUB,
    187: write_FSUB,
    188: write_FSUB,
    189: write_FSUB,
    190: write_FSUB,
    191: write_FSUB,
    192: write_FSUB,
    193: write_FSUB,
    194: write_FMUL,
    195: write_FMUL,
    196: write_FMUL,
    197: write_FMUL,
    198: write_FMUL,
    199: write_FMUL,
    200: write_FMUL,
    201: write_FMUL,
    202: write_FMUL,
    203: write_FMUL,
    204: write_FMUL,
    205: write_FMUL,
    206: write_FMUL,
    207: write_FMUL,
    208: write_FMUL,
    209: write_FMUL,
    210: write_FMUL,
    211: write_FMUL,
    212: write_FDIV,
    213: write_FDIV,
    214: write_FDIV,
    215: write_FSQRT,
    216: write_FSQRT,
    217: write_FSQRT,
    218: write_FSQRT,
    219: write_FSQRT,
    220: write_FSQRT,
    221: write_FSQRT,
    222: write_FROUND,
    223: write_FROUND,
    224: write_CALL,
    225: write_CALL,
    226: write_CALL,
    227: write_CALL,
    228: write_CALL,
    229: write_CALL,
    230: write_CALL,
    231: write_CALL,
    232: write_CALL,
    233: write_CALL,
    234: write_CALL,
    235: write_CALL,
    236: write_CALL,
    237: write_CALL,
    238: write_CALL,
    239: write_CALL,
    240: write_CALL,
    241: write_RET,
    242: write_RET,
    243: write_RET,
    244: write_RET,
    245: write_RET,
    246: write_RET,
    247: write_RET,
    248: write_RET,
    249: write_RET,
    250: write_RET,
    251: write_RET,
    252: write_RET,
    253: write_RET,
    254: write_RET,
    255: write_RET,
}

def writeCode(file, i, symbol):
    opcodeMap.get(symbol.opcode)(file, i, symbol)

def writeMain(file):
    file.write(('__attribute__((optimize("Os"))) int main() {\n'
                "	register uint64_t r0, r1, r2, r3, r4, r5, r6, r7;\n"
                "	register double f0, f1, f2, f3, f4, f5, f6, f7;\n"
                "	register uint64_t ic, sp;\n"
                "	convertible_t scratchpad[SCRATCHPAD_LENGTH] __attribute__ ((aligned (16)));\n"
                "	stack_t stack[STACK_LENGTH];\n"
                "	mmu_t mmu;\n"
                "	uint32_t mxcsr;\n"
                ))

def writeProlog(file):
    file.write(("#include <stdint.h>\n"
                "#include <time.h>\n"
                "#include <stdio.h>\n"
                "#include <x86intrin.h>\n"
                "#include <emmintrin.h>\n"
                "#include <wmmintrin.h>\n"
                "#include <math.h>\n"
                "typedef uint32_t addr_t;\n"
                "typedef unsigned __int128 uint128_t;\n"
                "typedef __int128 int128_t;\n"
                "typedef unsigned char byte;\n"
                "typedef union {\n"
                "	double f64;\n"
                "	int64_t i64;\n"
                "	uint64_t u64;\n"
                "	int32_t i32;\n"
                "	uint32_t u32;\n"
                "} convertible_t;\n"
                "typedef union {\n"
                "	uint64_t value;\n"
                "	void* address;\n"
                "} stack_t;\n"
                "typedef struct {\n"
                "	addr_t m0;\n"
                "	addr_t m1;\n"
                "	addr_t mx;\n"
                "#ifdef RAM\n"
                "	const char* buffer;\n"
                "#endif\n"
                "} mmu_t;\n"
                "#define DRAM_SIZE (1UL << 32)\n"
                "#define SCRATCHPAD_SIZE (256 * 1024)\n"
                "#define SCRATCHPAD_LENGTH (SCRATCHPAD_SIZE / sizeof(convertible_t))\n"
                "#define SCRATCHPAD_MASK14 (16 * 1024 / sizeof(convertible_t) - 1)\n"
                "#define SCRATCHPAD_MASK18 (SCRATCHPAD_LENGTH - 1)\n"
                "#define SCRATCHPAD_16K(x) scratchpad[(x) & SCRATCHPAD_MASK14]\n"
                "#define SCRATCHPAD_256K(x) scratchpad[(x) & SCRATCHPAD_MASK18]\n"
                "#define STACK_LENGTH (32 * 1024)\n"
                "#ifdef RAM\n"
                "#define DRAM_READ(mmu) (convertible_t)*(uint64_t*)((mmu)->buffer + (mmu)->m0)\n"
                "#define PREFETCH(mmu) _mm_prefetch(((mmu)->buffer + (mmu)->m1), _MM_HINT_T0)\n"
                "#else\n"
                "#define DRAM_READ(mmu) (convertible_t)(uint64_t)__rolq(6364136223846793005ULL*((mmu)->m0)+1442695040888963407ULL,32)\n"
                "#define PREFETCH(x)\n"
                "#endif\n"
                "#define PUSH_VALUE(x) stack[sp++].value = x\n"
                "#define PUSH_ADDRESS(x) stack[sp++].address = x\n"
                "#define STACK_IS_EMPTY() (sp == 0)\n"
                "#define POP_VALUE() stack[--sp].value\n"
                "#define POP_ADDRESS() stack[--sp].address\n"
                "static convertible_t readDram(mmu_t* mmu, addr_t addr) {\n"
                "	convertible_t data;\n"
                "	data = DRAM_READ(mmu);\n"
                "	mmu->m0 += 8;\n"
                "	mmu->mx ^= addr;\n"
                "	if((mmu->m0 & 255) == 128) {\n"
                "		mmu->m1 = mmu->mx & 0xFFFFFF00;\n"
                "		PREFETCH(mmu);\n"
                "	}\n"
                "	if((mmu->m0 & 255) == 0)\n"
                "		mmu->m0 = mmu->m1;\n"
                "	return data;\n"
                "}\n"
                "static inline __m128i sl_xor(__m128i tmp1) {\n"
                "	__m128i tmp4;\n"
                "	tmp4 = _mm_slli_si128(tmp1, 0x04);\n"
                "	tmp1 = _mm_xor_si128(tmp1, tmp4);\n"
                "	tmp4 = _mm_slli_si128(tmp4, 0x04);\n"
                "	tmp1 = _mm_xor_si128(tmp1, tmp4);\n"
                "	tmp4 = _mm_slli_si128(tmp4, 0x04);\n"
                "	tmp1 = _mm_xor_si128(tmp1, tmp4);\n"
                "	return tmp1;\n"
                "}\n"
                "#define AES_GENKEY_SUB(rcon) do { \\\n"
                "	__m128i xout1 = _mm_aeskeygenassist_si128(xout2, rcon);	\\\n"
                "	xout1 = _mm_shuffle_epi32(xout1, 0xFF);	\\\n"
                "	xout0 = sl_xor(xout0);  \\\n"
                "	xout0 = _mm_xor_si128(xout0, xout1); \\\n"
                "	xout1 = _mm_aeskeygenassist_si128(xout0, 0x00); \\\n"
                "	xout1 = _mm_shuffle_epi32(xout1, 0xAA); \\\n"
                "	xout2 = sl_xor(xout2); \\\n"
                "	xout2 = _mm_xor_si128(xout2, xout1); } while(0)\n"
                "static inline void aes_genkey(const __m128i* memory, __m128i* k0, __m128i* k1, __m128i* k2, __m128i* k3, __m128i* k4, __m128i* k5, __m128i* k6, __m128i* k7, __m128i* k8, __m128i* k9) {\n"
                "	__m128i xout0, xout2;\n"
                "	xout0 = _mm_load_si128(memory);\n"
                "	xout2 = _mm_load_si128(memory+1);\n"
                "	*k0 = xout0;\n"
                "	*k1 = xout2;\n"
                "	AES_GENKEY_SUB(0x01);\n"
                "	*k2 = xout0;\n"
                "	*k3 = xout2;\n"
                "	AES_GENKEY_SUB(0x02);\n"
                "	*k4 = xout0;\n"
                "	*k5 = xout2;\n"
                "	AES_GENKEY_SUB(0x04);\n"
                "	*k6 = xout0;\n"
                "	*k7 = xout2;\n"
                "	AES_GENKEY_SUB(0x08);\n"
                "	*k8 = xout0;\n"
                "	*k9 = xout2;\n"
                "}\n"
                "static inline void aes_round(__m128i key, __m128i* x0, __m128i* x1, __m128i* x2, __m128i* x3, __m128i* x4, __m128i* x5, __m128i* x6, __m128i* x7) {\n"
                "	*x0 = _mm_aesenc_si128(*x0, key);\n"
                "	*x1 = _mm_aesenc_si128(*x1, key);\n"
                "	*x2 = _mm_aesenc_si128(*x2, key);\n"
                "	*x3 = _mm_aesenc_si128(*x3, key);\n"
                "	*x4 = _mm_aesenc_si128(*x4, key);\n"
                "	*x5 = _mm_aesenc_si128(*x5, key);\n"
                "	*x6 = _mm_aesenc_si128(*x6, key);\n"
                "	*x7 = _mm_aesenc_si128(*x7, key);\n"
                "}\n"
                "static void aesInitialize(__m128i* key, __m128i* seed, __m128i* output, size_t count) {\n"
                "	\n"
                "	__m128i xin0, xin1, xin2, xin3, xin4, xin5, xin6, xin7;\n"
                "	__m128i k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;\n"
                "	\n"
                "	aes_genkey(key, &k0, &k1, &k2, &k3, &k4, &k5, &k6, &k7, &k8, &k9);\n"
                "	\n"
                "	xin0 = _mm_load_si128(seed + 0);\n"
                "	xin1 = _mm_load_si128(seed + 1);\n"
                "	xin2 = _mm_load_si128(seed + 2);\n"
                "	xin3 = _mm_load_si128(seed + 3);\n"
                "	xin4 = _mm_load_si128(seed + 4);\n"
                "	xin5 = _mm_load_si128(seed + 5);\n"
                "	xin6 = _mm_load_si128(seed + 6);\n"
                "	xin7 = _mm_load_si128(seed + 7);\n"
                "	\n"
                "	for (size_t i = 0; i < count / sizeof(__m128i); i += 8)\n"
                "	{\n"
                "		aes_round(k0, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k1, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k2, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k3, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k4, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k5, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k6, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k7, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k8, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		aes_round(k9, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);\n"
                "		\n"
                "		_mm_store_si128(output + i + 0, xin0);\n"
                "		_mm_store_si128(output + i + 1, xin1);\n"
                "		_mm_store_si128(output + i + 2, xin2);\n"
                "		_mm_store_si128(output + i + 3, xin3);\n"
                "		_mm_store_si128(output + i + 4, xin4);\n"
                "		_mm_store_si128(output + i + 5, xin5);\n"
                "		_mm_store_si128(output + i + 6, xin6);\n"
                "		_mm_store_si128(output + i + 7, xin7);\n"
                "	}\n"
                "}\n"))

with sys.stdout as file:
    writeProlog(file)
    file.write("const byte aesKey[32] = {{ {0} }};\n".format(genBytes(32)))
    file.write("const byte aesSeed[128] = {{ {0} }};\n".format(genBytes(128)))
    writeMain(file)
    writeInitialValues(file)
    for i in range(PROGRAM_SIZE):
        writeCode(file, i, CodeSymbol(random.getrandbits(128)))
    if PROGRAM_SIZE > 0:
        file.write("\t\tgoto i_0;\n")
    writeEpilog(file)