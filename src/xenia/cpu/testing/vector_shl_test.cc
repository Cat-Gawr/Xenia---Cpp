#include "xenia/cpu/testing/util.h"

using namespace xe;
using namespace xe::cpu;
using namespace xe::cpu::hir;
using namespace xe::cpu::testing;
using xe::cpu::ppc::PPCContext;

TEST_CASE("VECTOR_SHL_I8", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(b, 3, b.VectorShl(LoadVR(b, 4), LoadVR(b, 5), INT8_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128b(0x7E, 0x7E, 0x7E, 0x7F, 0x80, 0xFF, 0x01, 0x12,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        ctx->v[5] =
            vec128b(0, 1, 2, 8, 4, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result == vec128b(0x7E, 0xFC, 0xF8, 0x7F, 0x00, 0xF0, 0x40,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00));
      });
}

TEST_CASE("VECTOR_SHL_I8_CONSTANT", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(
        b, 3,
        b.VectorShl(LoadVR(b, 4),
                    b.LoadConstantVec128(vec128b(0, 1, 2, 8, 4, 4, 6, 7, 8, 9,
                                                 10, 11, 12, 13, 14, 15)),
                    INT8_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128b(0x7E, 0x7E, 0x7E, 0x7F, 0x80, 0xFF, 0x01, 0x12,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result == vec128b(0x7E, 0xFC, 0xF8, 0x7F, 0x00, 0xF0, 0x40,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00));
      });
}

// This targets the "all_same" optimization of the Int8 specialization of
// VECTOR_SHL_V128
TEST_CASE("VECTOR_SHL_I8_SAME_CONSTANT", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(
        b, 3,
        b.VectorShl(LoadVR(b, 4), b.LoadConstantVec128(vec128b(5)), INT8_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128b(0x7E, 0x7E, 0x7E, 0x7F, 0x80, 0xFF, 0x01, 0x12,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result == vec128b(0xC0, 0xC0, 0xC0, 0xE0, 0x00, 0xE0, 0x20,
                                  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00));
      });
}

TEST_CASE("VECTOR_SHL_I16", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(b, 3, b.VectorShl(LoadVR(b, 4), LoadVR(b, 5), INT16_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128s(0x7FFE, 0x7FFE, 0x7FFE, 0x7FFF, 0x8000, 0xFFFF,
                            0x0001, 0x1234);
        ctx->v[5] = vec128s(0, 1, 8, 15, 15, 8, 1, 16);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result == vec128s(0x7FFE, 0xFFFC, 0xFE00, 0x8000, 0x0000,
                                  0xFF00, 0x0002, 0x1234));
      });
}

TEST_CASE("VECTOR_SHL_I16_CONSTANT", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(
        b, 3,
        b.VectorShl(LoadVR(b, 4),
                    b.LoadConstantVec128(vec128s(0, 1, 8, 15, 15, 8, 1, 16)),
                    INT16_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128s(0x7FFE, 0x7FFE, 0x7FFE, 0x7FFF, 0x8000, 0xFFFF,
                            0x0001, 0x1234);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result == vec128s(0x7FFE, 0xFFFC, 0xFE00, 0x8000, 0x0000,
                                  0xFF00, 0x0002, 0x1234));
      });
}

TEST_CASE("VECTOR_SHL_I32", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(b, 3, b.VectorShl(LoadVR(b, 4), LoadVR(b, 5), INT32_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128i(0x7FFFFFFE, 0x7FFFFFFE, 0x7FFFFFFE, 0x7FFFFFFF);
        ctx->v[5] = vec128i(0, 1, 16, 31);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result ==
                vec128i(0x7FFFFFFE, 0xFFFFFFFC, 0xFFFE0000, 0x80000000));
      });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128i(0x80000000, 0xFFFFFFFF, 0x00000001, 0x12345678);
        ctx->v[5] = vec128i(31, 16, 1, 32);
      },
      [](PPCContext* ctx) {
        auto result = ctx->v[3];
        REQUIRE(result ==
                vec128i(0x00000000, 0xFFFF0000, 0x00000002, 0x12345678));
      });
}

TEST_CASE("VECTOR_SHL_I32_CONSTANT", "[instr]") {
  TestFunction test([](HIRBuilder& b) {
    StoreVR(
        b, 3,
        b.VectorShl(LoadVR(b, 4), b.LoadConstantVec128(vec128i(0, 1, 16, 31)),
                    INT32_TYPE));
    StoreVR(
        b, 4,
        b.VectorShl(LoadVR(b, 5), b.LoadConstantVec128(vec128i(31, 16, 1, 32)),
                    INT32_TYPE));
    b.Return();
  });
  test.Run(
      [](PPCContext* ctx) {
        ctx->v[4] = vec128i(0x7FFFFFFE, 0x7FFFFFFE, 0x7FFFFFFE, 0x7FFFFFFF);
        ctx->v[5] = vec128i(0x80000000, 0xFFFFFFFF, 0x00000001, 0x12345678);
      },
      [](PPCContext* ctx) {
        auto result1 = ctx->v[3];
        REQUIRE(result1 ==
                vec128i(0x7FFFFFFE, 0xFFFFFFFC, 0xFFFE0000, 0x80000000));
        auto result2 = ctx->v[4];
        REQUIRE(result2 ==
                vec128i(0x00000000, 0xFFFF0000, 0x00000002, 0x12345678));
      });
}
