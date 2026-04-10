/*
Copyright (c) 2023 tevador <tevador@gmail.com>
Copyright (c) 2026, Forest Crossman <cyrozap@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdexcept>
#include <cstring>

#include <unistd.h>

#include "cpu.hpp"
#include "program.hpp"
#include "reciprocal.h"
#include "superscalar.hpp"
#include "virtual_memory.h"

#include "jit_compiler_ppc64.hpp"

namespace {
#define HANDLER_ARGS randomx::CompilerState& state, randomx::Instruction isn, int i, randomx_flags flags
	using InstructionHandler = void(HANDLER_ARGS);
	extern InstructionHandler* opcodeMap1[256];
}

namespace PPC64 {

	static inline uint32_t A_form(uint32_t po, uint32_t frt, uint32_t fra, uint32_t frb, uint32_t frc, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(frt <= 0x1F)) throw std::runtime_error("frt <= 0x1F");
		if (!(fra <= 0x1F)) throw std::runtime_error("fra <= 0x1F");
		if (!(frb <= 0x1F)) throw std::runtime_error("frb <= 0x1F");
		if (!(frc <= 0x1F)) throw std::runtime_error("frc <= 0x1F");
		if (!(xo <= 0x1F)) throw std::runtime_error("xo <= 0x1F");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		return (po << 26) | (frt << 21) | (fra << 16) | (frb << 11) | (frc << 6) | (xo << 1) | rc;
	}

	static inline uint32_t B_form(uint32_t po, uint32_t bo, uint32_t bi, uint32_t bd, uint32_t aa, uint32_t lk) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(bo <= 0x1F)) throw std::runtime_error("bo <= 0x1F");
		if (!(bi <= 0x1F)) throw std::runtime_error("bi <= 0x1F");
		if (!(bd <= 0x3FFF)) throw std::runtime_error("bd <= 0x3FFF");
		if (!(aa <= 0x1)) throw std::runtime_error("aa <= 0x1");
		if (!(lk <= 0x1)) throw std::runtime_error("lk <= 0x1");
		return (po << 26) | (bo << 21) | (bi << 16) | (bd << 2) | (aa << 1) | lk;
	}

	static inline uint32_t D_form(uint32_t po, uint32_t rt, uint32_t ra, uint32_t d) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rt <= 0x1F)) throw std::runtime_error("rt <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(d <= 0xFFFF)) throw std::runtime_error("d <= 0xFFFF");
		return (po << 26) | (rt << 21) | (ra << 16) | d;
	}

	static inline uint32_t I_form(uint32_t po, uint32_t li, uint32_t aa, uint32_t lk) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(li <= 0xFFFFFF)) throw std::runtime_error("li <= 0xFFFFFF");
		if (!(aa <= 0x1)) throw std::runtime_error("aa <= 0x1");
		if (!(lk <= 0x1)) throw std::runtime_error("lk <= 0x1");
		return (po << 26) | (li << 2) | (aa << 1) | lk;
	}

	static inline uint32_t M_form(uint32_t po, uint32_t rs, uint32_t ra, uint32_t sh, uint32_t mb, uint32_t me, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rs <= 0x1F)) throw std::runtime_error("rs <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(sh <= 0x1F)) throw std::runtime_error("sh <= 0x1F");
		if (!(mb <= 0x1F)) throw std::runtime_error("mb <= 0x1F");
		if (!(me <= 0x1F)) throw std::runtime_error("me <= 0x1F");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		return (po << 26) | (rs << 21) | (ra << 16) | (sh << 11) | (mb << 6) | (me << 1) | rc;
	}

	static inline uint32_t MD_form(uint32_t po, uint32_t rs, uint32_t ra, uint32_t sh, uint32_t mb, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rs <= 0x1F)) throw std::runtime_error("rs <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(sh <= 0x3F)) throw std::runtime_error("sh <= 0x3F");
		if (!(mb <= 0x3F)) throw std::runtime_error("mb <= 0x3F");
		if (!(xo <= 0x7)) throw std::runtime_error("xo <= 0x7");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		uint32_t sh0_4 = sh & 0x1F;
		uint32_t sh5 = (sh >> 5) & 0x1;
		uint32_t mb0_4 = mb & 0x1F;
		uint32_t mb5 = (mb >> 5) & 0x1;
		return (po << 26) | (rs << 21) | (ra << 16) | (sh0_4 << 11) | (mb0_4 << 6) | (mb5 << 5) | (xo << 2) | (sh5 << 1) | rc;
	}

	static inline uint32_t MDS_form(uint32_t po, uint32_t rs, uint32_t ra, uint32_t rb, uint32_t mb, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rs <= 0x1F)) throw std::runtime_error("rs <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(rb <= 0x1F)) throw std::runtime_error("rb <= 0x1F");
		if (!(mb <= 0x3F)) throw std::runtime_error("mb <= 0x3F");
		if (!(xo <= 0xF)) throw std::runtime_error("xo <= 0xF");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		uint32_t mb0_4 = mb & 0x1F;
		uint32_t mb5 = (mb >> 5) & 0x1;
		return (po << 26) | (rs << 21) | (ra << 16) | (rb << 11) | (mb0_4 << 6) | (mb5 << 5) | (xo << 1) | rc;
	}

	static inline uint32_t VA_form(uint32_t po, uint32_t vrt, uint32_t vra, uint32_t vrb, uint32_t vrc, uint32_t xo) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(vrt <= 0x1F)) throw std::runtime_error("vrt <= 0x1F");
		if (!(vra <= 0x1F)) throw std::runtime_error("vra <= 0x1F");
		if (!(vrb <= 0x1F)) throw std::runtime_error("vrb <= 0x1F");
		if (!(vrc <= 0x1F)) throw std::runtime_error("vrc <= 0x1F");
		if (!(xo <= 0x3F)) throw std::runtime_error("xo <= 0x3F");
		return (po << 26) | (vrt << 21) | (vra << 16) | (vrb << 11) | (vrc << 6) | xo;
	}

	static inline uint32_t X_form(uint32_t po, uint32_t rt, uint32_t ra, uint32_t rb, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rt <= 0x1F)) throw std::runtime_error("rt <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(rb <= 0x1F)) throw std::runtime_error("rb <= 0x1F");
		if (!(xo <= 0x3FF)) throw std::runtime_error("xo <= 0x3FF");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		return (po << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (xo << 1) | rc;
	}

	static inline uint32_t XFL_form(uint32_t po, uint32_t l, uint32_t flm, uint32_t w, uint32_t frb, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(l <= 0x1)) throw std::runtime_error("l <= 0x1");
		if (!(flm <= 0xFF)) throw std::runtime_error("flm <= 0xFF");
		if (!(w <= 0x1)) throw std::runtime_error("w <= 0x1");
		if (!(frb <= 0x1F)) throw std::runtime_error("frb <= 0x1F");
		if (!(xo <= 0x3FF)) throw std::runtime_error("xo <= 0x3FF");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		return (po << 26) | (l << 25) | (flm << 17) | (w << 16) | (frb << 11) | (xo << 1) | rc;
	}

	static inline uint32_t XO_form(uint32_t po, uint32_t rt, uint32_t ra, uint32_t rb, uint32_t oe, uint32_t xo, uint32_t rc) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(rt <= 0x1F)) throw std::runtime_error("rt <= 0x1F");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (!(rb <= 0x1F)) throw std::runtime_error("rb <= 0x1F");
		if (!(oe <= 0x1)) throw std::runtime_error("oe <= 0x1");
		if (!(xo <= 0x1FF)) throw std::runtime_error("xo <= 0x1FF");
		if (!(rc <= 0x1)) throw std::runtime_error("rc <= 0x1");
		return (po << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (oe << 10) | (xo << 1) | rc;
	}

	static inline uint32_t XX2_form(uint32_t po, uint32_t t, uint32_t a, uint32_t b, uint32_t xo, uint32_t bx, uint32_t tx) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(t <= 0x1F)) throw std::runtime_error("t <= 0x1F");
		if (!(a <= 0x1F)) throw std::runtime_error("a <= 0x1F");
		if (!(b <= 0x1F)) throw std::runtime_error("b <= 0x1F");
		if (!(xo <= 0x1FF)) throw std::runtime_error("xo <= 0x1FF");
		if (!(bx <= 0x1)) throw std::runtime_error("bx <= 0x1");
		if (!(tx <= 0x1)) throw std::runtime_error("tx <= 0x1");
		return (po << 26) | (t << 21) | (a << 16) | (b << 11) | (xo << 2) | (bx << 1) | tx;
	}

	static inline uint32_t XX3_form(uint32_t po, uint32_t t, uint32_t a, uint32_t b, uint32_t xo, uint32_t ax, uint32_t bx, uint32_t tx) {
		if (!(po <= 0x3F)) throw std::runtime_error("po <= 0x3F");
		if (!(t <= 0x1F)) throw std::runtime_error("t <= 0x1F");
		if (!(a <= 0x1F)) throw std::runtime_error("a <= 0x1F");
		if (!(b <= 0x1F)) throw std::runtime_error("b <= 0x1F");
		if (!(xo <= 0xFF)) throw std::runtime_error("xo <= 0xFF");
		if (!(ax <= 0x1)) throw std::runtime_error("ax <= 0x1");
		if (!(bx <= 0x1)) throw std::runtime_error("bx <= 0x1");
		if (!(tx <= 0x1)) throw std::runtime_error("tx <= 0x1");
		return (po << 26) | (t << 21) | (a << 16) | (b << 11) | (xo << 3) | (ax << 2) | (bx << 1) | tx;
	}

	static inline uint32_t b(int32_t offset) {
		if (offset & 3) throw std::runtime_error("offset must be 4-byte aligned");
		if (offset < -(1 << 25) || offset >= (1 << 25)) throw std::runtime_error("offset out of range");
		return I_form(18, (offset >> 2) & 0xFFFFFF, 0, 0);
	}

	static inline uint32_t bl(int32_t offset) {
		if (offset & 3) throw std::runtime_error("offset must be 4-byte aligned");
		if (offset < -(1 << 25) || offset >= (1 << 25)) throw std::runtime_error("offset out of range");
		return I_form(18, (offset >> 2) & 0xFFFFFF, 0, 1);
	}

	static inline uint32_t bc(uint32_t bo, uint32_t bi, int32_t offset) {
		if (!(bo <= 0x1F)) throw std::runtime_error("bo <= 0x1F");
		if (!(bi <= 0x1F)) throw std::runtime_error("bi <= 0x1F");
		if (offset & 3) throw std::runtime_error("offset must be 4-byte aligned");
		if (offset < -(1 << 15) || offset >= (1 << 15)) throw std::runtime_error("offset out of range");
		return B_form(16, bo, bi, (offset >> 2) & 0x3FFF, 0, 0);
	}

	static inline uint32_t beq(int32_t offset) {
		return bc(12, 2, offset);
	}

	static inline uint32_t bne(int32_t offset) {
		return bc(4, 2, offset);
	}

	static inline uint32_t cmpi(uint32_t bf, uint32_t l, uint32_t ra, int32_t si) {
		if (!(bf <= 0x7)) throw std::runtime_error("bf <= 0x7");
		if (!(l <= 0x1)) throw std::runtime_error("l <= 0x1");
		if (!(ra <= 0x1F)) throw std::runtime_error("ra <= 0x1F");
		if (si < -(1 << 15) || si >= (1 << 15)) throw std::runtime_error("si out of range");
		return D_form(11, (bf << 2) | l, ra, si);
	}

	static inline uint32_t addi(uint32_t rt, uint32_t ra, uint32_t si) { return D_form(14, rt, ra, si); }
	static inline uint32_t addis(uint32_t rt, uint32_t ra, uint32_t si) { return D_form(15, rt, ra, si); }
	static inline uint32_t ori(uint32_t ra, uint32_t rs, uint32_t ui) { return D_form(24, rs, ra, ui); }
	static inline uint32_t oris(uint32_t ra, uint32_t rs, uint32_t ui) { return D_form(25, rs, ra, ui); }
	static inline uint32_t andi_dot(uint32_t ra, uint32_t rs, uint32_t ui) { return D_form(28, rs, ra, ui); }

	static inline uint32_t add(uint32_t rt, uint32_t ra, uint32_t rb) { return XO_form(31, rt, ra, rb, 0, 266, 0); }
	static inline uint32_t subf(uint32_t rt, uint32_t ra, uint32_t rb) { return XO_form(31, rt, ra, rb, 0, 40, 0); }
	static inline uint32_t neg(uint32_t rt, uint32_t ra) { return XO_form(31, rt, ra, 0, 0, 104, 0); }
	static inline uint32_t and_(uint32_t ra, uint32_t rs, uint32_t rb) { return X_form(31, rs, ra, rb, 28, 0); }
	static inline uint32_t and_dot(uint32_t ra, uint32_t rs, uint32_t rb) { return X_form(31, rs, ra, rb, 28, 1); }
	static inline uint32_t xor_(uint32_t ra, uint32_t rs, uint32_t rb) { return X_form(31, rs, ra, rb, 316, 0); }
	static inline uint32_t or_(uint32_t ra, uint32_t rs, uint32_t rb) { return X_form(31, rs, ra, rb, 444, 0); }

	static inline uint32_t mulld(uint32_t rt, uint32_t ra, uint32_t rb) { return XO_form(31, rt, ra, rb, 0, 233, 0); }
	static inline uint32_t mulhdu(uint32_t rt, uint32_t ra, uint32_t rb) { return XO_form(31, rt, ra, rb, 0, 9, 0); }
	static inline uint32_t mulhd(uint32_t rt, uint32_t ra, uint32_t rb) { return XO_form(31, rt, ra, rb, 0, 73, 0); }

	static inline uint32_t rlwinm(uint32_t ra, uint32_t rs, uint32_t sh, uint32_t mb, uint32_t me) { return M_form(21, rs, ra, sh, mb, me, 0); }
	static inline uint32_t rldicl(uint32_t ra, uint32_t rs, uint32_t sh, uint32_t mb) { return MD_form(30, rs, ra, sh, mb, 0, 0); }
	static inline uint32_t rldicl_dot(uint32_t ra, uint32_t rs, uint32_t sh, uint32_t mb) { return MD_form(30, rs, ra, sh, mb, 0, 1); }
	static inline uint32_t rldicr(uint32_t ra, uint32_t rs, uint32_t sh, uint32_t me) { return MD_form(30, rs, ra, sh, me, 1, 0); }
	static inline uint32_t rldic(uint32_t ra, uint32_t rs, uint32_t sh, uint32_t mb) { return MD_form(30, rs, ra, sh, mb, 2, 0); }
	static inline uint32_t rldcl(uint32_t ra, uint32_t rs, uint32_t rb, uint32_t mb) { return MDS_form(30, rs, ra, rb, mb, 8, 0); }

	static inline uint32_t cmpdi(uint32_t rx, int32_t si) { return cmpi(0, 1, rx, si); }

	static inline uint32_t li(uint32_t rx, int32_t si) { return addi(rx, 0, si); }
	static inline uint32_t lis(uint32_t rx, int32_t si) { return addis(rx, 0, si); }
	static inline uint32_t mr(uint32_t rx, uint32_t ry) { return or_(rx, ry, ry); }
	static inline uint32_t rotldi(uint32_t ra, uint32_t rs, uint32_t n) { return rldicl(ra, rs, n, 0); }
	static inline uint32_t rotrdi(uint32_t ra, uint32_t rs, uint32_t n) { return rldicl(ra, rs, 64-n, 0); }
	static inline uint32_t sldi(uint32_t rx, uint32_t ry, uint32_t n) { return rldicr(rx, ry, n, 63-n); }
	static inline uint32_t srdi(uint32_t rx, uint32_t ry, uint32_t n) { return rldicl(rx, ry, 64-n, n); }

	static inline uint32_t ldx(uint32_t rt, uint32_t ra, uint32_t rb) { return X_form(31, rt, ra, rb, 21, 0); }
	static inline uint32_t ldbrx(uint32_t rt, uint32_t ra, uint32_t rb) { return X_form(31, rt, ra, rb, 532, 0); }
	static inline uint32_t stdx(uint32_t rs, uint32_t ra, uint32_t rb) { return X_form(31, rs, ra, rb, 149, 0); }
	static inline uint32_t stdbrx(uint32_t rs, uint32_t ra, uint32_t rb) { return X_form(31, rs, ra, rb, 660, 0); }

	static inline uint32_t lfd(uint32_t frt, uint32_t ra, uint32_t d) { return D_form(50, frt, ra, d); }
	static inline uint32_t lfdx(uint32_t frt, uint32_t ra, uint32_t rb) { return X_form(31, frt, ra, rb, 599, 0); }
	static inline uint32_t mtfsf(uint32_t flm, uint32_t frb, uint32_t l, uint32_t w) { return XFL_form(63, l, flm, w, frb, 711, 0); }

	static inline uint32_t lxsdx(uint32_t xt, uint32_t ra, uint32_t rb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		return X_form(31, t, ra, rb, 588, tx);
	}

	static inline uint32_t lxvd2x(uint32_t xt, uint32_t ra, uint32_t rb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		return X_form(31, t, ra, rb, 844, tx);
	}

	static inline uint32_t vperm(uint32_t vrt, uint32_t vra, uint32_t vrb, uint32_t vrc) { return VA_form(4, vrt, vra, vrb, vrc, 43); }

	static inline uint32_t xxmrghw(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 18, ax, bx, tx);
	}

	static inline uint32_t xvadddp(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 96, ax, bx, tx);
	}

	static inline uint32_t xvsubdp(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 104, ax, bx, tx);
	}

	static inline uint32_t xvmuldp(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 112, ax, bx, tx);
	}

	static inline uint32_t xvdivdp(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 120, ax, bx, tx);
	}

	static inline uint32_t xvsqrtdp(uint32_t xt, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX2_form(60, t, 0, b, 203, bx, tx);
	}

	static inline uint32_t xvcvsxwdp(uint32_t xt, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX2_form(60, t, 0, b, 248, bx, tx);
	}

	static inline uint32_t xxpermdi(uint32_t xt, uint32_t xa, uint32_t xb, uint32_t dm) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, (dm << 5) | 10, ax, bx, tx);
	}

	static inline uint32_t xxswapd(uint32_t xt, uint32_t xa) { return xxpermdi(xt, xa, xa, 2); }

	static inline uint32_t xxland(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 130, ax, bx, tx);
	}

	static inline uint32_t xxlor(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 146, ax, bx, tx);
	}

	static inline uint32_t xxlxor(uint32_t xt, uint32_t xa, uint32_t xb) {
		if (!(xt <= 0x3F)) throw std::runtime_error("xt <= 0x3F");
		if (!(xa <= 0x3F)) throw std::runtime_error("xa <= 0x3F");
		if (!(xb <= 0x3F)) throw std::runtime_error("xb <= 0x3F");
		uint32_t t = xt & 0x1F;
		uint32_t tx = xt >> 5;
		uint32_t a = xa & 0x1F;
		uint32_t ax = xa >> 5;
		uint32_t b = xb & 0x1F;
		uint32_t bx = xb >> 5;
		return XX3_form(60, t, a, b, 154, ax, bx, tx);
	}

}

namespace randomx {

	static const uint8_t* codeConstants = (uint8_t*)&randomx_ppc64_constants;
	static const uint8_t* codeConstantLutFprcToFpscr = (uint8_t*)&randomx_ppc64_constant_lut_fprc_to_fpscr;
	static const uint8_t* codeConstantsEnd = (uint8_t*)&randomx_ppc64_constants_end;

	static const uint8_t* codeDatasetInit = (uint8_t*)&randomx_ppc64_dataset_init;
	static const uint8_t* codeDatasetInitFixCall = (uint8_t*)&randomx_ppc64_dataset_init_fix_call;
	static const uint8_t* codeDatasetInitEnd = (uint8_t*)&randomx_ppc64_dataset_init_end;

	static const uint8_t* codeSshashSingleItemPrologue = (uint8_t*)&randomx_ppc64_sshash_single_item_prologue;
	static const uint8_t* codeSshashSingleItemPrologueEnd = (uint8_t*)&randomx_ppc64_sshash_single_item_prologue_end;
	static const uint8_t* codeSshashSingleItemEpilogue = (uint8_t*)&randomx_ppc64_sshash_single_item_epilogue;
	static const uint8_t* codeSshashSingleItemEpilogueEnd = (uint8_t*)&randomx_ppc64_sshash_single_item_epilogue_end;
	static const uint8_t* codeSshashCachePrefetch = (uint8_t*)&randomx_ppc64_sshash_cache_prefetch;
	static const uint8_t* codeSshashCachePrefetchEnd = (uint8_t*)&randomx_ppc64_sshash_cache_prefetch_end;
	static const uint8_t* codeSshashXor = (uint8_t*)&randomx_ppc64_sshash_xor;
	static const uint8_t* codeSshashXorEnd = (uint8_t*)&randomx_ppc64_sshash_xor_end;

	static const uint8_t* codeVmPrologue = (uint8_t*)&randomx_ppc64_vm_prologue;
	static const uint8_t* codeVmPrologueEnd = (uint8_t*)&randomx_ppc64_vm_prologue_end;
	static const uint8_t* codeVmEpilogue = (uint8_t*)&randomx_ppc64_vm_epilogue;
	static const uint8_t* codeVmFixLoop = (uint8_t*)&randomx_ppc64_vm_fix_loop;
	static const uint8_t* codeVmEpilogueEnd = (uint8_t*)&randomx_ppc64_vm_epilogue_end;
	static const uint8_t* codeVmLoopPrologue = (uint8_t*)&randomx_ppc64_vm_loop_prologue;
	static const uint8_t* codeVmLoopPrologueEnd = (uint8_t*)&randomx_ppc64_vm_loop_prologue_end;
	static const uint8_t* codeVmDataRead = (uint8_t*)&randomx_ppc64_vm_data_read;
	static const uint8_t* codeVmDataReadEnd = (uint8_t*)&randomx_ppc64_vm_data_read_end;
	static const uint8_t* codeVmDataReadLight = (uint8_t*)&randomx_ppc64_vm_data_read_light;
	static const uint8_t* codeVmDataReadLightFixCall = (uint8_t*)&randomx_ppc64_vm_data_read_light_fix_call;
	static const uint8_t* codeVmDataReadLightEnd = (uint8_t*)&randomx_ppc64_vm_data_read_light_end;
	static const uint8_t* codeVmSpadStorePrologue = (uint8_t*)&randomx_ppc64_vm_spad_store_prologue;
	static const uint8_t* codeVmSpadStorePrologueEnd = (uint8_t*)&randomx_ppc64_vm_spad_store_prologue_end;
	static const uint8_t* codeVmSpadStoreMixV1 = (uint8_t*)&randomx_ppc64_vm_spad_store_mix_v1;
	static const uint8_t* codeVmSpadStoreMixV1End = (uint8_t*)&randomx_ppc64_vm_spad_store_mix_v1_end;
	static const uint8_t* codeVmSpadStoreEpilogue = (uint8_t*)&randomx_ppc64_vm_spad_store_epilogue;
	static const uint8_t* codeVmSpadStoreEpilogueEnd = (uint8_t*)&randomx_ppc64_vm_spad_store_epilogue_end;
	static const uint8_t* codeVmSpadStoreMixV2HardAes = (uint8_t*)&randomx_ppc64_vm_spad_store_mix_v2_hard_aes;
	static const uint8_t* codeVmSpadStoreMixV2HardAesEnd = (uint8_t*)&randomx_ppc64_vm_spad_store_mix_v2_hard_aes_end;

	static const int32_t sizeConstants = codeConstantsEnd - codeConstants;

	static const int32_t sizeDatasetInit = codeDatasetInitEnd - codeDatasetInit;

	static const int32_t sizeSshashSingleItemPrologue = codeSshashSingleItemPrologueEnd - codeSshashSingleItemPrologue;
	static const int32_t sizeSshashSingleItemEpilogue = codeSshashSingleItemEpilogueEnd - codeSshashSingleItemEpilogue;
	static const int32_t sizeSshashCachePrefetch = codeSshashCachePrefetchEnd - codeSshashCachePrefetch;
	static const int32_t sizeSshashXor = codeSshashXorEnd - codeSshashXor;

	static const int32_t sizeVmPrologue = codeVmPrologueEnd - codeVmPrologue;
	static const int32_t sizeVmEpilogue = codeVmEpilogueEnd - codeVmEpilogue;
	static const int32_t sizeVmLoopPrologue = codeVmLoopPrologueEnd - codeVmLoopPrologue;
	static const int32_t sizeVmDataRead = codeVmDataReadEnd - codeVmDataRead;
	static const int32_t sizeVmDataReadLight = codeVmDataReadLightEnd - codeVmDataReadLight;
	static const int32_t sizeVmSpadStorePrologue = codeVmSpadStorePrologueEnd - codeVmSpadStorePrologue;
	static const int32_t sizeVmSpadStoreMixV1 = codeVmSpadStoreMixV1End - codeVmSpadStoreMixV1;
	static const int32_t sizeVmSpadStoreEpilogue = codeVmSpadStoreEpilogueEnd - codeVmSpadStoreEpilogue;
	static const int32_t sizeVmSpadStoreMixV2HardAes = codeVmSpadStoreMixV2HardAesEnd - codeVmSpadStoreMixV2HardAes;

	static const int32_t offsetConstantLutFprcToFpscr = codeConstantLutFprcToFpscr - codeConstants;

	static const int32_t offsetDatasetInitFixCall = codeDatasetInitFixCall - codeDatasetInit;

	static const int32_t offsetVmFixLoop = codeVmFixLoop - codeVmEpilogue;
	static const int32_t offsetVmDataReadLightFixCall = codeVmDataReadLightFixCall - codeVmDataReadLight;

	constexpr size_t CodeAlign = 64*1024;  // 64 kB, to ensure alignment on systems with a page size <= 64 kB
	static const size_t ConstantPoolSize = alignSize(sizeConstants + 16, CodeAlign);  // Add 16 bytes for the Group E OR vector mask
	static const size_t ReserveCodeSize = alignSize(sizeVmPrologue + sizeVmEpilogue + sizeVmLoopPrologue + sizeVmDataRead + sizeVmDataReadLight + sizeVmSpadStorePrologue + sizeVmSpadStoreMixV2HardAes + sizeVmSpadStoreEpilogue, CodeAlign);
	constexpr size_t MaxRandomXInstrCodeSize = 4*10;  // FDIV_M requires at most 10 instructions
	constexpr size_t MaxSuperscalarInstrSize = 4*6;  // IMUL_RCP requires at most 6 instructions
	static const size_t SuperscalarProgramHeaders = sizeSshashSingleItemPrologue + sizeSshashSingleItemEpilogue;

	static const size_t RandomXCodeSize = alignSize(ConstantPoolSize + ReserveCodeSize + MaxRandomXInstrCodeSize * RANDOMX_PROGRAM_MAX_SIZE, CodeAlign);
	static const size_t SuperscalarSize = alignSize(sizeDatasetInit + SuperscalarProgramHeaders + (sizeSshashCachePrefetch + sizeSshashXor + MaxSuperscalarInstrSize * SuperscalarMaxSize) * RANDOMX_CACHE_ACCESSES, CodeAlign);

	static const uint32_t CodeSize = RandomXCodeSize + SuperscalarSize;

	constexpr uint32_t ConstantsBaseAddressRegisterGPR2 = 2;
	constexpr uint32_t ConstantVectorBePermutationMaskVR16 = 16;
	constexpr uint32_t ConstantVectorBePermutationMaskVSR48 = 32 + ConstantVectorBePermutationMaskVR16;
	constexpr uint32_t ConstantVectorGroupEAndMaskVR17 = 17;
	constexpr uint32_t ConstantVectorGroupEAndMaskVSR49 = 32 + ConstantVectorGroupEAndMaskVR17;
	constexpr uint32_t ConstantVectorFscalXorMaskVR18 = 18;
	constexpr uint32_t ConstantVectorFscalXorMaskVSR50 = 32 + ConstantVectorFscalXorMaskVR18;
	constexpr uint32_t ConstantVectorGroupEOrMaskVR19 = 19;
	constexpr uint32_t ConstantVectorGroupEOrMaskVSR51 = 32 + ConstantVectorGroupEOrMaskVR19;

	constexpr uint32_t MaGPR24 = 24;
	constexpr uint32_t MxGPR25 = 25;
	constexpr uint32_t SpAddr0GPR26 = 26;
	constexpr uint32_t SpAddr1GPR27 = 27;
	constexpr uint32_t ScratchpadPointerGPR30 = 30;

	template<size_t N>
	struct GprMap {
		uint32_t regs[N];
		uint32_t getPpcGprNum(uint8_t idx) const {
			return regs[idx % N];
		}
	};

	template<size_t N>
	struct VsrMap {
		uint32_t regs[N];
		uint32_t getPpcVrNum(uint8_t idx) const {
			return regs[idx % N];
		}
		uint32_t getPpcVsrNum(uint8_t idx) const {
			return regs[idx % N] + 32;
		}
	};

	static const GprMap<8> RegisterMapR = {{ 14, 15, 16, 17, 18, 19, 20, 21 }};
	static const VsrMap<4> RegisterMapF = {{ 0, 1, 2, 3 }};
	static const VsrMap<4> RegisterMapE = {{ 4, 5, 6, 7 }};
	static const VsrMap<4> RegisterMapA = {{ 8, 9, 10, 11 }};
	static const VsrMap<8> RegisterMapFE = {{ 0, 1, 2, 3, 4, 5, 6, 7 }};

	static const GprMap<8> RegisterMapSsh = {{ 4, 6, 7, 9, 10, 11, 12, 22 }};

	template<typename T> static constexpr size_t Log2(T value) { return (value > 1) ? (Log2(value / 2) + 1) : 0; }

	constexpr int32_t unsigned32ToSigned2sCompl(uint32_t x) {
		return (-1 == ~0) ? (int32_t)x : (x > INT32_MAX ? (-(int32_t)(UINT32_MAX - x) - 1) : (int32_t)x);
	}

	static void syncInstructionCache(void* start_ptr, void* end_ptr) {
		// Apparently GCC compiles __builtin___clear_cache to nothing, so we use LLVM's implementation instead.
		//
		// This code has been modified from compiler-rt/lib/builtins/clear_cache.c, found at
		// https://github.com/llvm/llvm-project revision 7459e10f34aa86952b1620d0cb48b40be112ebe9.
		//
		// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
		// See https://llvm.org/LICENSE.txt for license information.
		// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
		char* start = (char*)start_ptr;
		char* end = (char*)end_ptr;
		const size_t len = (uintptr_t)end - (uintptr_t)start;
		if (len == 0) return;

		// Query data and instruction cache line sizes
		long dcache_val = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
		long icache_val = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);

		const size_t d_line_size = (dcache_val > 0) ? dcache_val : 32;
		const size_t i_line_size = (icache_val > 0) ? icache_val : 32;

		// Flush Data Cache
		const uintptr_t d_mask = ~(d_line_size - 1);
		const uintptr_t d_start_line = ((uintptr_t)start) & d_mask;
		const uintptr_t d_end_line = ((uintptr_t)start + len + d_line_size - 1) & d_mask;

		for (uintptr_t line = d_start_line; line < d_end_line; line += d_line_size)
			__asm__ volatile("dcbst 0, %0" : : "r"(line));

		// Wait for memory writes to complete
		__asm__ volatile("sync");

		// Invalidate Instruction Cache
		const uintptr_t i_mask = ~(i_line_size - 1);
		const uintptr_t i_start_line = ((uintptr_t)start) & i_mask;
		const uintptr_t i_end_line = ((uintptr_t)start + len + i_line_size - 1) & i_mask;

		for (uintptr_t line = i_start_line; line < i_end_line; line += i_line_size)
			__asm__ volatile("icbi 0, %0" : : "r"(line));

		// Flush the local instruction pipeline
		__asm__ volatile("isync");
	}

	static void emitLoadGpr64(CompilerState& state, uint32_t rt, uint32_t ra, uint32_t rb) {
		if (PPC_BIG_ENDIAN) {
			state.emit(PPC64::ldbrx(rt, ra, rb));
		} else {
			state.emit(PPC64::ldx(rt, ra, rb));
		}
	}

	static void emitStoreGpr64(CompilerState& state, uint32_t rs, uint32_t ra, uint32_t rb) {
		if (PPC_BIG_ENDIAN) {
			state.emit(PPC64::stdbrx(rs, ra, rb));
		} else {
			state.emit(PPC64::stdx(rs, ra, rb));
		}
	}

	static void emitLoadVr64(CompilerState& state, uint32_t vrt, uint32_t ra, uint32_t rb) {
		// We need to load the two packed little-endian signed 32-bit integers into a VSR, then we need to
		// shuffle them so they're in the correct halves of the VSR register and in the correct byte order,
		// and then we need to convert the signed 32-bit ints to doubles.
		uint32_t xt = 32 + vrt;
		state.emit(PPC64::lxsdx(xt, ra, rb));
		if (PPC_BIG_ENDIAN) {
			// Register XT contains the value as [ 0123 4567 zzzz zzzz ]
			state.emit(PPC64::vperm(vrt, vrt, vrt, ConstantVectorBePermutationMaskVR16)); // Shuffles values in XT to be [ 7654 7654 3210 3210 ]
		} else {
			// Register XT contains the value as [ 7654 3210 zzzz zzzz ]
			state.emit(PPC64::xxmrghw(xt, xt, xt)); // Shuffles values in XT to be [ 7654 7654 3210 3210 ]
		}
		state.emit(PPC64::xvcvsxwdp(xt, xt)); // Needs values in XT as [ 7654 zzzz 3210 zzzz ]
	}

	static void emitMovImm32(CompilerState& state, int reg, uint32_t imm) {
		// Move signed 32-bit immediate into 64-bit register.
		// Note that `imm` is a `uint32_t` and not an `int32_t` for type compatibility--it has no effect on
		// functionality because `lis` will automatically sign-extend the 16-bit value.
		int32_t simm = (int32_t)imm;
		if (simm >= -32768 && simm <= 32767) {
			state.emit(PPC64::li(reg, simm & 0xFFFF));
		} else {
			uint16_t upper  = (imm >> 16) & 0xFFFF;
			uint16_t lower  = (imm >>  0) & 0xFFFF;

			state.emit(PPC64::lis(reg, upper));
			if (lower)
				state.emit(PPC64::ori(reg, reg, lower));
		}
	}

	static void emitAddImm32(CompilerState& state, int dstReg, int srcReg, uint32_t imm) {
		int32_t simm = (int32_t)imm;
		if (simm >= -32768 && simm <= 32767) {
			state.emit(PPC64::addi(dstReg, srcReg, simm & 0xFFFF));
		} else {
			emitMovImm32(state, 8, imm);
			state.emit(PPC64::add(dstReg, srcReg, 8));
		}
	}

	static void emitMovImm64(CompilerState& state, int reg, uint64_t imm) {
		if (imm == (uint64_t)(int64_t)(int32_t)imm) {
			// Values that can be represented by loading a 32-bit signed immediate
			emitMovImm32(state, reg, (uint32_t)imm);
		} else {
			uint64_t lowestBit = imm & -(int64_t)imm;
			uint64_t added = imm + lowestBit;
			if (imm != 0 && imm != ~0ULL && (added & (added - 1)) == 0) {
				// Values that are a contiguous sequence of 1s
				uint32_t mb = added == 0 ? 0 : __builtin_clzll(added);
				uint32_t me = 63 - __builtin_ctzll(lowestBit);
				state.emit(PPC64::li(reg, -1));
				if (mb == 0) {
					state.emit(PPC64::rldicr(reg, reg, 0, me));
				} else if (me == 63) {
					state.emit(PPC64::rldicl(reg, reg, 0, mb));
				} else {
					state.emit(PPC64::rldic(reg, reg, 63 - me, mb));
				}
				return;
			}

			// Values that can be generated by loading a <=32-bit immediate and rotating it
			for (int i = 1; i < 64; ++i) {
				uint64_t rot = (imm << i) | (imm >> (64 - i));
				if (rot == (uint64_t)(int64_t)(int32_t)rot) {
					emitMovImm32(state, reg, (uint32_t)rot);
					state.emit(PPC64::rotldi(reg, reg, 64 - i));
					return;
				}
			}

			// All other values
			uint32_t high = imm >> 32;
			uint32_t low = imm & 0xFFFFFFFF;

			if (high) {
				emitMovImm32(state, reg, high);
				state.emit(PPC64::sldi(reg, reg, 32));
			} else {
				state.emit(PPC64::li(reg, 0));
			}

			uint16_t lower = (low >> 16) & 0xFFFF;
			uint16_t lowest = low & 0xFFFF;

			if (lower)
				state.emit(PPC64::oris(reg, reg, lower));

			if (lowest)
				state.emit(PPC64::ori(reg, reg, lowest));
		}
	}

	template<uint32_t tmp_gpr>
	static void emitLoadGprFromScratchpad(CompilerState& state, uint32_t dst, uint32_t src, Instruction& instr) {
		uint32_t imm = instr.getImm32();

		if (src != dst) {
			uint32_t size = instr.getModMem() ? RANDOMX_SCRATCHPAD_L1 : RANDOMX_SCRATCHPAD_L2;
			imm &= size - 1;
			emitAddImm32(state, tmp_gpr, src, imm);

			uint32_t mb = 32 - Log2(size);
			state.emit(PPC64::rlwinm(tmp_gpr, tmp_gpr, 0, mb, 28));

			emitLoadGpr64(state, tmp_gpr, ScratchpadPointerGPR30, tmp_gpr);
		} else {
			imm = (imm & ScratchpadL3Mask) >> 3;
			emitMovImm32(state, tmp_gpr, imm);
			state.emit(PPC64::sldi(tmp_gpr, tmp_gpr, 3));

			emitLoadGpr64(state, tmp_gpr, ScratchpadPointerGPR30, tmp_gpr);
		}
	}

	template<uint32_t tmp_vr>
	static void emitLoadVsrFromScratchpad(CompilerState& state, Instruction& instr) {
		int src = RegisterMapR.getPpcGprNum(instr.src);

		uint32_t imm = instr.getImm32();
		uint32_t size = instr.getModMem() ? RANDOMX_SCRATCHPAD_L1 : RANDOMX_SCRATCHPAD_L2;
		imm &= size - 1;
		emitAddImm32(state, 8, src, imm);

		uint32_t mb = 32 - Log2(size);
		state.emit(PPC64::rlwinm(8, 8, 0, mb, 28));

		emitLoadVr64(state, tmp_vr, ScratchpadPointerGPR30, 8);
	}

	void JitCompilerPPC64::emitProgramPrefix(CompilerState& state, Program& prog, ProgramConfiguration& pcfg, randomx_flags flags) {
		// Set the Group E OR vector mask
		state.emitAt(sizeConstants, pcfg.eMask[0]);
		state.emitAt(sizeConstants + 8, pcfg.eMask[1]);

		state.codePos = RandomXCodePos;

		state.emit(codeVmPrologue, sizeVmPrologue);
		// Mask mx and ma with Scratchpad L3 mask
		uint32_t mask_begin = 32 - Log2(RANDOMX_SCRATCHPAD_L3);
		uint32_t mask_end = 31 - Log2(RANDOMX_DATASET_ITEM_SIZE);
		state.emit(PPC64::rlwinm(SpAddr0GPR26, MxGPR25, 0, mask_begin, mask_end));
		state.emit(PPC64::rlwinm(SpAddr1GPR27, MaGPR24, 0, mask_begin, mask_end));
		// Init spAddr0 to masked mx + scratchpad base
		state.emit(PPC64::add(SpAddr0GPR26, SpAddr0GPR26, ScratchpadPointerGPR30));
		// Init spAddr1 to masked ma + scratchpad base
		state.emit(PPC64::add(SpAddr1GPR27, SpAddr1GPR27, ScratchpadPointerGPR30));

		LoopBeginPos = state.codePos;
		state.emit(codeVmLoopPrologue, sizeVmLoopPrologue);

		// Step 4: The 256 instructions stored in the Program Buffer are executed.
		for (unsigned i = 0; i < RegistersCount; ++i) {
			state.registerUsage[i] = -1;
		}
		for (unsigned i = 0; i < prog.getSize(flags); ++i) {
			Instruction instr = prog(i);
			instr.src %= RegistersCount;
			instr.dst %= RegistersCount;
			state.instructionOffsets[i] = state.codePos;
			opcodeMap1[instr.opcode](state, instr, i, flags);
		}
	}

	void JitCompilerPPC64::emitProgramSuffix(CompilerState& state, ProgramConfiguration& pcfg, randomx_flags flags) {
		state.emit(codeVmSpadStorePrologue, sizeVmSpadStorePrologue);

		if (flags & RANDOMX_FLAG_V2) {
			if (true || (flags & RANDOMX_FLAG_HARD_AES)) {  // TODO: Remove the "true" once software AES is working
				if (!randomx::cpu.hasAes()) {
					throw std::runtime_error("This CPU is missing support for hardware AES!");
				}
				state.emit(codeVmSpadStoreMixV2HardAes, sizeVmSpadStoreMixV2HardAes);
			} else {
				throw std::runtime_error("Software AES is not yet implemented for PPC64!");
			}
		} else {
			state.emit(codeVmSpadStoreMixV1, sizeVmSpadStoreMixV1);
		}

		state.emit(codeVmSpadStoreEpilogue, sizeVmSpadStoreEpilogue);

		state.emit(PPC64::xor_(SpAddr0GPR26, RegisterMapR.getPpcGprNum(pcfg.readReg0), RegisterMapR.getPpcGprNum(pcfg.readReg1)));

		// spAddr1 (r27) = r26 >> 32
		state.emit(PPC64::srdi(SpAddr1GPR27, SpAddr0GPR26, 32));
		// spAddr0 (r26) = r26 & 0xFFFFFFFF
		state.emit(PPC64::rldicl(SpAddr0GPR26, SpAddr0GPR26, 0, 32));

		// Apply Scratchpad L3 mask
		uint32_t mb = 32 - Log2(RANDOMX_SCRATCHPAD_L3);
		uint32_t me = 31 - Log2(RANDOMX_DATASET_ITEM_SIZE);
		state.emit(PPC64::rlwinm(SpAddr0GPR26, SpAddr0GPR26, 0, mb, me));
		state.emit(PPC64::rlwinm(SpAddr1GPR27, SpAddr1GPR27, 0, mb, me));

		// Add scratchpad base pointer (r30)
		state.emit(PPC64::add(SpAddr0GPR26, SpAddr0GPR26, ScratchpadPointerGPR30));
		state.emit(PPC64::add(SpAddr1GPR27, SpAddr1GPR27, ScratchpadPointerGPR30));

		int32_t fixPos = state.codePos;
		state.emit(codeVmEpilogue, sizeVmEpilogue);

		int32_t fixContinuePos = fixPos + offsetVmFixLoop;
		state.emitAt(fixContinuePos, PPC64::b(LoopBeginPos - fixContinuePos));
	}

	JitCompilerPPC64::JitCompilerPPC64() {
		state.code = (uint8_t*) allocMemoryPages(CodeSize);
		if (state.code == nullptr)
			throw std::runtime_error("allocMemoryPages");

		state.codePos = 0;
		state.emit(codeConstants, sizeConstants);

		state.codePos = ConstantPoolSize;
		entryProgram = state.code + state.codePos;
		if (PPC_ABI_V2) {
			// Load r2 with the base address of the constant pool
			emitMovImm64(state, ConstantsBaseAddressRegisterGPR2, reinterpret_cast<uint64_t>(state.code));
		}
		RandomXCodePos = state.codePos;

		state.codePos = RandomXCodeSize;
		entryDataInit = state.code + state.codePos;
		if (PPC_ABI_V2) {
			// Load r2 with the base address of the constant pool
			emitMovImm64(state, ConstantsBaseAddressRegisterGPR2, reinterpret_cast<uint64_t>(state.code));
		}
		int32_t datasetInitFixCallPos = state.codePos + offsetDatasetInitFixCall;
		state.emit(codeDatasetInit, sizeDatasetInit);
		SshashSingleItemPos = alignSize(state.codePos, 128);
		// Patch in the call to the SuperScalar Hash single item function
		state.emitAt(datasetInitFixCallPos, PPC64::bl(SshashSingleItemPos - datasetInitFixCallPos));

#if !PPC_ABI_V2
		// Initialize the ABI V1 function descriptors
		descriptorProgram[0] = reinterpret_cast<uint64_t>(entryProgram);
		descriptorProgram[1] = reinterpret_cast<uint64_t>(state.code);
		descriptorProgram[2] = 0;

		descriptorDataInit[0] = reinterpret_cast<uint64_t>(entryDataInit);
		descriptorDataInit[1] = reinterpret_cast<uint64_t>(state.code);
		descriptorDataInit[2] = 0;
#endif
	}

	JitCompilerPPC64::~JitCompilerPPC64() {
		freePagedMemory(state.code, CodeSize);
	}

	void JitCompilerPPC64::enableWriting() {
		setPagesRW(state.code, CodeSize);
	}

	void JitCompilerPPC64::enableExecution() {
		setPagesRX(state.code, CodeSize);
	}

	void JitCompilerPPC64::enableAll() {
		setPagesRWX(state.code, CodeSize);
	}

	void JitCompilerPPC64::generateProgram(Program& prog, ProgramConfiguration& pcfg) {
		emitProgramPrefix(state, prog, pcfg, flags);

		// Step 5a: Save ma in mt (r9, temporary)
		int mtReg = 9;
		state.emit(PPC64::mr(mtReg, MaGPR24));

		// Step 5b: the mp register is XORed with the low 32 bits of registers readReg2 and readReg3
		int mpReg = (flags & RANDOMX_FLAG_V2) ? MaGPR24 : MxGPR25;  // r24 = ma, r25 = mx
		state.emit(PPC64::xor_(8, RegisterMapR.getPpcGprNum(pcfg.readReg2), RegisterMapR.getPpcGprNum(pcfg.readReg3)));
		// Zero-extend r8 to 32 bits (clear upper 32 bits)
		state.emit(PPC64::rldicl(8, 8, 0, 32));
		// mp ^= (readReg2 ^ readReg3)
		state.emit(PPC64::xor_(mpReg, mpReg, 8));

		int32_t dataReadPos = state.codePos;
		state.emit(codeVmDataRead, sizeVmDataRead);

		uint32_t mask_begin = 32 - Log2(RANDOMX_DATASET_BASE_SIZE);
		uint32_t mask_end = 31 - Log2(CacheLineSize);

		// Patch prefetch address calculation (offset 0)
		state.emitAt(dataReadPos, PPC64::rlwinm(8, mpReg, 0, mask_begin, mask_end));

		// Patch read address calculation (offset 12)
		state.emitAt(dataReadPos + 12, PPC64::rlwinm(8, mtReg, 0, mask_begin, mask_end));

		emitProgramSuffix(state, pcfg, flags);

		syncInstructionCache(entryProgram, state.code + state.codePos);
	}

	void JitCompilerPPC64::generateProgramLight(Program& prog, ProgramConfiguration& pcfg, uint32_t datasetOffset) {
		emitProgramPrefix(state, prog, pcfg, flags);

		// Step 5a: Save ma in mt (r9, temporary)
		int mtReg = 9;
		state.emit(PPC64::mr(mtReg, MaGPR24));

		// Step 5b: the mp register is XORed with the low 32 bits of registers readReg2 and readReg3
		int mpReg = (flags & RANDOMX_FLAG_V2) ? MaGPR24 : MxGPR25;  // r24 = ma, r25 = mx
		state.emit(PPC64::xor_(8, RegisterMapR.getPpcGprNum(pcfg.readReg2), RegisterMapR.getPpcGprNum(pcfg.readReg3)));
		// Zero-extend r8 to 32 bits (clear upper 32 bits)
		state.emit(PPC64::rldicl(8, 8, 0, 32));
		// mp ^= (readReg2 ^ readReg3)
		state.emit(PPC64::xor_(mpReg, mpReg, 8));

		// Calculate itemNumber = (mt & datasetMask) / CacheLineSize
		uint32_t datasetMask = (RANDOMX_DATASET_BASE_SIZE - 1) & ~63;
		emitMovImm32(state, 8, datasetMask);
		state.emit(PPC64::and_(5, mtReg, 8)); // r5 = mt & datasetMask
		state.emit(PPC64::srdi(5, 5, Log2(CacheLineSize))); // r5 = r5 >> 6

		emitAddImm32(state, 5, 5, datasetOffset / CacheLineSize);

		int32_t callPos = state.codePos + offsetVmDataReadLightFixCall;
		state.emit(codeVmDataReadLight, sizeVmDataReadLight);
		state.emitAt(callPos, PPC64::bl(SshashSingleItemPos - callPos));

		emitProgramSuffix(state, pcfg, flags);

		syncInstructionCache(entryProgram, state.code + state.codePos);
	}

	static void generateSuperscalarCode(CompilerState& state, Instruction instr, const std::vector<uint64_t>& reciprocalCache) {
		int dst = RegisterMapSsh.getPpcGprNum(instr.dst);
		int src = RegisterMapSsh.getPpcGprNum(instr.src);
		uint32_t rotation = instr.getImm32() & 63;

		switch ((SuperscalarInstructionType)instr.opcode) {
			case SuperscalarInstructionType::ISUB_R:
				// subf dst, src, dst
				state.emit(PPC64::subf(dst, src, dst));
				break;
			case SuperscalarInstructionType::IXOR_R:
				// xor dst, dst, src
				state.emit(PPC64::xor_(dst, dst, src));
				break;
			case SuperscalarInstructionType::IADD_RS:
				// sldi r8, src, shift
				state.emit(PPC64::sldi(8, src, instr.getModShift()));
				// add dst, dst, r8
				state.emit(PPC64::add(dst, dst, 8));
				break;
			case SuperscalarInstructionType::IMUL_R:
				// mulld dst, dst, src
				state.emit(PPC64::mulld(dst, dst, src));
				break;
			case SuperscalarInstructionType::IROR_C:
				if (rotation) {
					// rotrdi dst, dst, imm
					state.emit(PPC64::rotrdi(dst, dst, rotation));
				}
				break;
			case SuperscalarInstructionType::IADD_C7:
			case SuperscalarInstructionType::IADD_C8:
			case SuperscalarInstructionType::IADD_C9:
				emitMovImm32(state, 8, instr.getImm32());
				// add dst, dst, r8
				state.emit(PPC64::add(dst, dst, 8));
				break;
			case SuperscalarInstructionType::IXOR_C7:
			case SuperscalarInstructionType::IXOR_C8:
			case SuperscalarInstructionType::IXOR_C9:
				emitMovImm32(state, 8, instr.getImm32());
				// xor dst, dst, r8
				state.emit(PPC64::xor_(dst, dst, 8));
				break;
			case SuperscalarInstructionType::IMULH_R:
				// mulhdu dst, dst, src
				state.emit(PPC64::mulhdu(dst, dst, src));
				break;
			case SuperscalarInstructionType::ISMULH_R:
				// mulhd dst, dst, src
				state.emit(PPC64::mulhd(dst, dst, src));
				break;
			case SuperscalarInstructionType::IMUL_RCP:
				emitMovImm64(state, 8, reciprocalCache[instr.getImm32()]);
				// mulld dst, dst, r8
				state.emit(PPC64::mulld(dst, dst, 8));
				break;
			default:
				UNREACHABLE;
		}
	}

	void JitCompilerPPC64::generateSuperscalarHash(SuperscalarProgramList& programs, std::vector<uint64_t> &reciprocalCache) {
		state.codePos = SshashSingleItemPos;

		// Steps 1 and 2
		state.emit(codeSshashSingleItemPrologue, sizeSshashSingleItemPrologue);

		for (size_t i = 0; i < programs.size(); ++i) {
			SuperscalarProgram& prog = programs[i];

			// Step 4
			// rldic r8, r5, Log2(CacheLineSize), 64 - Log2(CacheSize / CacheLineSize) - Log2(CacheLineSize)
			state.emit(PPC64::rldic(8, 5, Log2(CacheLineSize), 64 - Log2(CacheSize / CacheLineSize) - Log2(CacheLineSize)));
			state.emit(codeSshashCachePrefetch + 4, sizeSshashCachePrefetch - 4);

			// Step 5
			for (uint32_t j = 0; j < prog.getSize(); ++j) {
				Instruction& instr = prog(j);
				generateSuperscalarCode(state, instr, reciprocalCache);
			}

			// Step 6
			state.emit(codeSshashXor, sizeSshashXor);

			uint32_t addrReg = RegisterMapSsh.getPpcGprNum(prog.getAddressRegister());
			state.emit(PPC64::mr(5, addrReg));

		}

		// Return
		state.emit(codeSshashSingleItemEpilogue, sizeSshashSingleItemEpilogue);

		syncInstructionCache(entryDataInit, state.code + state.codePos);
	}

	size_t JitCompilerPPC64::getCodeSize() {
		return CodeSize;
	}

	static void h_IADD_RS(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		int shift = isn.getModShift();

		if (shift) {
			state.emit(PPC64::sldi(8, src, shift));
			state.emit(PPC64::add(dst, dst, 8));
		} else {
			state.emit(PPC64::add(dst, dst, src));
		}

		if (isn.dst == RegisterNeedsDisplacement) {
			emitAddImm32(state, dst, dst, isn.getImm32());
		}
	}
	static void h_IADD_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::add(dst, dst, 8));
	}
	static void h_ISUB_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		if (isn.src != isn.dst) {
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::subf(dst, src, dst));
		} else {
			int32_t imm = unsigned32ToSigned2sCompl(-isn.getImm32());
			emitAddImm32(state, dst, dst, imm);
		}
	}
	static void h_ISUB_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::subf(dst, 8, dst));
	}
	static void h_IMUL_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		if (isn.src != isn.dst) {
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::mulld(dst, dst, src));
		} else {
			emitMovImm32(state, 8, isn.getImm32());
			state.emit(PPC64::mulld(dst, dst, 8));
		}
	}
	static void h_IMUL_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::mulld(dst, dst, 8));
	}
	static void h_IMULH_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		state.emit(PPC64::mulhdu(dst, dst, src));
	}
	static void h_IMULH_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::mulhdu(dst, dst, 8));
	}
	static void h_ISMULH_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		state.emit(PPC64::mulhd(dst, dst, src));
	}
	static void h_ISMULH_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::mulhd(dst, dst, 8));
	}
	static void h_IMUL_RCP(HANDLER_ARGS) {
		uint32_t divisor = isn.getImm32();
		if (!isZeroOrPowerOf2(divisor)) {
			state.registerUsage[isn.dst] = i;
			int dst = RegisterMapR.getPpcGprNum(isn.dst);
			uint64_t rcp = randomx_reciprocal_fast(divisor);
			emitMovImm64(state, 8, rcp);
			state.emit(PPC64::mulld(dst, dst, 8));
		}
	}
	static void h_INEG_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		state.emit(PPC64::neg(dst, dst));
	}
	static void h_IXOR_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		if (isn.src != isn.dst) {
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::xor_(dst, dst, src));
		} else {
			emitMovImm32(state, 8, isn.getImm32());
			state.emit(PPC64::xor_(dst, dst, 8));
		}
	}
	static void h_IXOR_M(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		emitLoadGprFromScratchpad<8>(state, dst, src, isn);
		state.emit(PPC64::xor_(dst, dst, 8));
	}
	static void h_IROR_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		if (isn.src != isn.dst) {
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::neg(8, src));
			state.emit(PPC64::rldcl(dst, dst, 8, 0));
		} else {
			uint32_t imm = isn.getImm32() & 63;
			if (imm)
				state.emit(PPC64::rotrdi(dst, dst, imm));
		}
	}
	static void h_IROL_R(HANDLER_ARGS) {
		state.registerUsage[isn.dst] = i;
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		if (isn.src != isn.dst) {
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::rldcl(dst, dst, src, 0));
		} else {
			uint32_t imm = isn.getImm32() & 63;
			if (imm)
				state.emit(PPC64::rotldi(dst, dst, imm));
		}
	}
	static void h_ISWAP_R(HANDLER_ARGS) {
		if (isn.src != isn.dst) {
			state.registerUsage[isn.dst] = i;
			state.registerUsage[isn.src] = i;
			int dst = RegisterMapR.getPpcGprNum(isn.dst);
			int src = RegisterMapR.getPpcGprNum(isn.src);
			state.emit(PPC64::mr(8, dst));
			state.emit(PPC64::mr(dst, src));
			state.emit(PPC64::mr(src, 8));
		}
	}
	static void h_FSWAP_R(HANDLER_ARGS) {
		int dst = RegisterMapFE.getPpcVsrNum(isn.dst);
		state.emit(PPC64::xxswapd(dst, dst));
	}
	static void h_FADD_R(HANDLER_ARGS) {
		int dst = RegisterMapF.getPpcVsrNum(isn.dst);
		int src = RegisterMapA.getPpcVsrNum(isn.src);
		state.emit(PPC64::xvadddp(dst, dst, src));
	}
	static void h_FADD_M(HANDLER_ARGS) {
		int dst = RegisterMapF.getPpcVsrNum(isn.dst);
		emitLoadVsrFromScratchpad<12>(state, isn);
		state.emit(PPC64::xvadddp(dst, dst, 32 + 12));
	}
	static void h_FSUB_R(HANDLER_ARGS) {
		int dst = RegisterMapF.getPpcVsrNum(isn.dst);
		int src = RegisterMapA.getPpcVsrNum(isn.src);
		state.emit(PPC64::xvsubdp(dst, dst, src));
	}
	static void h_FSUB_M(HANDLER_ARGS) {
		int dst = RegisterMapF.getPpcVsrNum(isn.dst);
		emitLoadVsrFromScratchpad<12>(state, isn);
		state.emit(PPC64::xvsubdp(dst, dst, 32 + 12));
	}
	static void h_FSCAL_R(HANDLER_ARGS) {
		int dst = RegisterMapF.getPpcVsrNum(isn.dst);
		state.emit(PPC64::xxlxor(dst, dst, ConstantVectorFscalXorMaskVSR50));
	}
	static void h_FMUL_R(HANDLER_ARGS) {
		int dst = RegisterMapE.getPpcVsrNum(isn.dst);
		int src = RegisterMapA.getPpcVsrNum(isn.src);
		state.emit(PPC64::xvmuldp(dst, dst, src));
	}
	static void h_FDIV_M(HANDLER_ARGS) {
		int dst = RegisterMapE.getPpcVsrNum(isn.dst);
		uint32_t temp_vsr = 32 + 12;
		emitLoadVsrFromScratchpad<12>(state, isn);
		state.emit(PPC64::xxland(temp_vsr, temp_vsr, ConstantVectorGroupEAndMaskVSR49));
		state.emit(PPC64::xxlor(temp_vsr, temp_vsr, ConstantVectorGroupEOrMaskVSR51));
		state.emit(PPC64::xvdivdp(dst, dst, temp_vsr));
	}
	static void h_FSQRT_R(HANDLER_ARGS) {
		int dst = RegisterMapE.getPpcVsrNum(isn.dst);
		state.emit(PPC64::xvsqrtdp(dst, dst));
	}
	static void h_CBRANCH(HANDLER_ARGS) {
		int reg = isn.dst;
		int target = state.registerUsage[reg] + 1;
		int shift = isn.getModCond() + ConditionOffset;
		int32_t imm = unsigned32ToSigned2sCompl(isn.getImm32());
		imm |= (1UL << shift);
		if (ConditionOffset > 0 || shift > 0)
			imm &= ~(1UL << (shift - 1));

		int dst = RegisterMapR.getPpcGprNum(reg);
		emitAddImm32(state, dst, dst, imm);

		// Calculate the Mask Begin (MB) parameter
		uint32_t mb = 64 - RANDOMX_JUMP_BITS;

		// rldicl. r8, dst, 64 - shift, mb
		state.emit(PPC64::rldicl_dot(8, dst, (64 - shift) & 63, mb));

		int32_t targetPos = state.instructionOffsets[target];
		int offset = targetPos - state.codePos;

		if (offset >= -(1 << 15) && offset < (1 << 15)) {
			state.emit(PPC64::beq(offset));
		} else {
			// Branch over the jump if not equal
			state.emit(PPC64::bne(8));
			state.emit(PPC64::b(offset - 4));
		}

		for (unsigned j = 0; j < RegistersCount; ++j) {
			state.registerUsage[j] = i;
		}
	}
	static void h_CFROUND(HANDLER_ARGS) {
		int src = RegisterMapR.getPpcGprNum(isn.src);
		int32_t rotateBits = isn.getImm32() & 63;

		// Operate directly on src by default
		int rot_src = src;

		// Rotate right by rotateBits
		if (rotateBits) {
			// rotrdi r8, src, rotateBits
			state.emit(PPC64::rotrdi(8, src, rotateBits));

			// We rotated src and put the new value in r8
			rot_src = 8;
		}

		int32_t patch_pos = 0;
		if (flags & RANDOMX_FLAG_V2) {
			// Skip the rest of the code if bits 5:2 are not zero. Use GPR0 as a discard register.
			// andi. r0, rot_src, 0x003C
			state.emit(PPC64::andi_dot(0, rot_src, 0x003C));

			// Get position to patch with conditional branch.
			patch_pos = state.codePos;

			// Emit invalid instruction now and patch later once we have the code length.
			state.emit(0); // bne skip_update
		}

		// Mask out bits 1:0 and multiply by 8 (shift left by 3) to get the table word offset (0, 8, 16, 24)
		// rldic r8, rot_src, 3, 59
		state.emit(PPC64::rldic(8, rot_src, 3, 59));

		// Load table address into scratch GPR0
		emitAddImm32(state, 0, ConstantsBaseAddressRegisterGPR2, offsetConstantLutFprcToFpscr);

		// Load value from fprc-to-FPSCR table into temporary FPR0
		// lfdx f0, r8, r0
		state.emit(PPC64::lfdx(0, 8, 0));

		// Move the RN value from scratch FPR0 to FPSCR (masked)
		// mtfsf 0x01, f0, 0, 0
		state.emit(PPC64::mtfsf(0x01, 0, 0, 0));

		if (flags & RANDOMX_FLAG_V2) {
			// Patch in the conditional branch instruction.
			int32_t branch_offset = state.codePos - patch_pos;
			state.emitAt(patch_pos, PPC64::bne(branch_offset));
		}
	}
	static void h_ISTORE(HANDLER_ARGS) {
		int dst = RegisterMapR.getPpcGprNum(isn.dst);
		int src = RegisterMapR.getPpcGprNum(isn.src);
		uint32_t imm = isn.getImm32();

		uint32_t size;
		if (isn.getModCond() < StoreL3Condition) {
			size = isn.getModMem() ? RANDOMX_SCRATCHPAD_L1 : RANDOMX_SCRATCHPAD_L2;
		} else {
			size = RANDOMX_SCRATCHPAD_L3;
		}
		imm &= size - 1;

		emitAddImm32(state, 8, dst, imm);

		uint32_t mb = 32 - Log2(size);
		state.emit(PPC64::rlwinm(8, 8, 0, mb, 28));

		emitStoreGpr64(state, src, ScratchpadPointerGPR30, 8);
	}
	static void h_NOP(HANDLER_ARGS) {
	}
}

#include "instruction_weights.hpp"

namespace {

#define INST_HANDLE(x) REPN(&randomx::h_##x, WT(x))

	InstructionHandler* opcodeMap1[256] = {
		INST_HANDLE(IADD_RS)
		INST_HANDLE(IADD_M)
		INST_HANDLE(ISUB_R)
		INST_HANDLE(ISUB_M)
		INST_HANDLE(IMUL_R)
		INST_HANDLE(IMUL_M)
		INST_HANDLE(IMULH_R)
		INST_HANDLE(IMULH_M)
		INST_HANDLE(ISMULH_R)
		INST_HANDLE(ISMULH_M)
		INST_HANDLE(IMUL_RCP)
		INST_HANDLE(INEG_R)
		INST_HANDLE(IXOR_R)
		INST_HANDLE(IXOR_M)
		INST_HANDLE(IROR_R)
		INST_HANDLE(IROL_R)
		INST_HANDLE(ISWAP_R)
		INST_HANDLE(FSWAP_R)
		INST_HANDLE(FADD_R)
		INST_HANDLE(FADD_M)
		INST_HANDLE(FSUB_R)
		INST_HANDLE(FSUB_M)
		INST_HANDLE(FSCAL_R)
		INST_HANDLE(FMUL_R)
		INST_HANDLE(FDIV_M)
		INST_HANDLE(FSQRT_R)
		INST_HANDLE(CBRANCH)
		INST_HANDLE(CFROUND)
		INST_HANDLE(ISTORE)
		INST_HANDLE(NOP)
	};

#undef INST_HANDLE
}

#define INST_HANDLE(x) REPN(static_cast<uint8_t>(randomx::InstructionType::x), WT(x))

alignas(128) uint8_t randomx::JitCompilerPPC64::instMap[256] = {
	INST_HANDLE(IADD_RS)
	INST_HANDLE(IADD_M)
	INST_HANDLE(ISUB_R)
	INST_HANDLE(ISUB_M)
	INST_HANDLE(IMUL_R)
	INST_HANDLE(IMUL_M)
	INST_HANDLE(IMULH_R)
	INST_HANDLE(IMULH_M)
	INST_HANDLE(ISMULH_R)
	INST_HANDLE(ISMULH_M)
	INST_HANDLE(IMUL_RCP)
	INST_HANDLE(INEG_R)
	INST_HANDLE(IXOR_R)
	INST_HANDLE(IXOR_M)
	INST_HANDLE(IROR_R)
	INST_HANDLE(IROL_R)
	INST_HANDLE(ISWAP_R)
	INST_HANDLE(FSWAP_R)
	INST_HANDLE(FADD_R)
	INST_HANDLE(FADD_M)
	INST_HANDLE(FSUB_R)
	INST_HANDLE(FSUB_M)
	INST_HANDLE(FSCAL_R)
	INST_HANDLE(FMUL_R)
	INST_HANDLE(FDIV_M)
	INST_HANDLE(FSQRT_R)
	INST_HANDLE(CBRANCH)
	INST_HANDLE(CFROUND)
	INST_HANDLE(ISTORE)
	INST_HANDLE(NOP)
};
