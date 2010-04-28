
#include <cassert>
#include <cstdio>

#include <boost/array.hpp>
#include <boost/cstdint.hpp>
#include <boost/hash/pack.hpp>

using boost::array;
using namespace boost::hash;
using namespace boost::hash::bitstream_endian;

void test_explodebb() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_big_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint8_t, 8> out;
    pack<big_byte_big_bit, 32, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}};
    assert(out == eout);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint16_t, 4> out;
    pack<big_byte_big_bit, 32, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 4> eout = {{0x0123, 0x4567, 0x89AB, 0xCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 4> out;
    pack<big_byte_big_bit, 16, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 4> eout = {{0x45, 0x67, 0x89, 0xAB}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 8> out;
    pack<big_byte_big_bit, 16, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{0xEC15}};
    array<bool, 16> out;
    pack<big_byte_big_bit, 16, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 16> eout = {{true, true, true, false,
                             true, true, false, false,
                             false, false, false, true,
                             false, true, false, true}};
    assert(out == eout);
    }

    {
    array<uint8_t, 2> in = {{0xC, 0x5}};
    array<bool, 8> out;
    pack<big_byte_big_bit, 4, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 8> eout = {{true, true, false, false,
                            false, true, false, true}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{(31 << 10) | (17 << 5) | (4 << 0)}};
    array<uint8_t, 3> out;
    pack<big_byte_big_bit, 15, 5>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 3> eout = {{31, 17, 4}};
    assert(out == eout);
    }

}

void test_explodelb() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<little_byte_big_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint8_t, 8> out;
    pack<little_byte_big_bit, 32, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89}};
    assert(out == eout);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint16_t, 4> out;
    pack<little_byte_big_bit, 32, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 4> eout = {{0x4567, 0x0123, 0xCDEF, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 4> out;
    pack<little_byte_big_bit, 16, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 4> eout = {{0x67, 0x45, 0xAB, 0x89}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 8> out;
    pack<little_byte_big_bit, 16, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x6, 0x7, 0x4, 0x5, 0xA, 0xB, 0x8, 0x9}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{0xEC15}};
    array<bool, 16> out;
    pack<little_byte_big_bit, 16, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 16> eout = {{false, false, false, true,
                             false, true, false, true,
                             true, true, true, false,
                             true, true, false, false}};
    assert(out == eout);
    }

    {
    array<uint8_t, 2> in = {{0xC, 0x5}};
    array<bool, 8> out;
    pack<little_byte_big_bit, 4, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 8> eout = {{true, true, false, false,
                            false, true, false, true}};
    assert(out == eout);
    }

}

void test_explodebl() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_little_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint8_t, 8> out;
    pack<big_byte_little_bit, 32, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}};
    assert(out == eout);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint16_t, 4> out;
    pack<big_byte_little_bit, 32, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 4> eout = {{0x0123, 0x4567, 0x89AB, 0xCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 4> out;
    pack<big_byte_little_bit, 16, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 4> eout = {{0x45, 0x67, 0x89, 0xAB}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 8> out;
    pack<big_byte_little_bit, 16, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x5, 0x4, 0x7, 0x6, 0x9, 0x8, 0xB, 0xA}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{0xEC15}};
    array<bool, 16> out;
    pack<big_byte_little_bit, 16, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 16> eout = {{false, false, true, true,
                             false, true, true, true,
                             true, false, true, false,
                             true, false, false, false}};
    assert(out == eout);
    }

    {
    array<uint8_t, 2> in = {{0xC, 0x5}};
    array<bool, 8> out;
    pack<big_byte_little_bit, 4, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 8> eout = {{false, false, true, true,
                            true, false, true, false}};
    assert(out == eout);
    }

}

void test_explodell() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<little_byte_little_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint8_t, 8> out;
    pack<little_byte_little_bit, 32, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89}};
    assert(out == eout);
    }

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint16_t, 4> out;
    pack<little_byte_little_bit, 32, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 4> eout = {{0x4567, 0x0123, 0xCDEF, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 4> out;
    pack<little_byte_little_bit, 16, 8>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 4> eout = {{0x67, 0x45, 0xAB, 0x89}};
    assert(out == eout);
    }

    {
    array<uint16_t, 2> in = {{0x4567, 0x89AB}};
    array<uint8_t, 8> out;
    pack<little_byte_little_bit, 16, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 8> eout = {{0x7, 0x6, 0x5, 0x4, 0xB, 0xA, 0x9, 0x8}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{0xEC15}};
    array<bool, 16> out;
    pack<little_byte_little_bit, 16, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 16> eout = {{true, false, true, false,
                             true, false, false, false,
                             false, false, true, true,
                             false, true, true, true}};
    assert(out == eout);
    }

    {
    array<uint8_t, 2> in = {{0xC, 0x5}};
    array<bool, 8> out;
    pack<little_byte_little_bit, 4, 1>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<bool, 8> eout = {{false, false, true, true,
                            true, false, true, false}};
    assert(out == eout);
    }

    {
    array<uint16_t, 1> in = {{(31 << 10) | (17 << 5) | (4 << 0)}};
    array<uint8_t, 3> out;
    pack<little_byte_little_bit, 15, 5>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.2x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 3> eout = {{4, 17, 31}};
    assert(out == eout);
    }

}

void test_implodebb() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_big_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint8_t, 8> in = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}};
    array<uint32_t, 2> out;
    pack<big_byte_big_bit, 8, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 4> in = {{0x0123, 0x4567, 0x89AB, 0xCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_big_bit, 16, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint8_t, 4> in = {{0x45, 0x67, 0x89, 0xAB}};
    array<uint16_t, 2> out;
    pack<big_byte_big_bit, 8, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint8_t, 8> in = {{0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB}};
    array<uint16_t, 2> out;
    pack<big_byte_big_bit, 4, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<bool, 16> in = {{true, true, true, false,
                           true, true, false, false,
                           false, false, false, true,
                           false, true, false, true}};
    array<uint16_t, 1> out;
    pack<big_byte_big_bit, 1, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{0xEC15}};
    assert(out == eout);
    }

    {
    array<bool, 8> in = {{true, true, false, false,
                          false, true, false, true}};
    array<uint8_t, 2> out;
    pack<big_byte_big_bit, 1, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 2> eout = {{0xC, 0x5}};
    assert(out == eout);
    }

    {
    array<uint8_t, 3> in = {{31, 17, 4}};
    array<uint16_t, 1> out;
    pack<big_byte_big_bit, 5, 15>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{(31 << 10) | (17 << 5) | (4 << 0)}};
    assert(out == eout);
    }

}

void test_implodelb() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<little_byte_big_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint8_t, 8> in = {{0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89}};
    array<uint32_t, 2> out;
    pack<little_byte_big_bit, 8, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 4> in = {{0x4567, 0x0123, 0xCDEF, 0x89AB}};
    array<uint32_t, 2> out;
    pack<little_byte_big_bit, 16, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint8_t, 4> in = {{0x67, 0x45, 0xAB, 0x89}};
    array<uint16_t, 2> out;
    pack<little_byte_big_bit, 8, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint8_t, 8> in = {{0x6, 0x7, 0x4, 0x5, 0xA, 0xB, 0x8, 0x9}};
    array<uint16_t, 2> out;
    pack<little_byte_big_bit, 4, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<bool, 16> in = {{false, false, false, true,
                           false, true, false, true,
                           true, true, true, false,
                           true, true, false, false}};
    array<uint16_t, 1> out;
    pack<little_byte_big_bit, 1, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{0xEC15}};
    assert(out == eout);
    }

    {
    array<bool, 8> in = {{true, true, false, false,
                          false, true, false, true}};
    array<uint8_t, 2> out;
    pack<little_byte_big_bit, 1, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 2> eout = {{0xC, 0x5}};
    assert(out == eout);
    }

}

void test_implodebl() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_little_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint8_t, 8> in = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}};
    array<uint32_t, 2> out;
    pack<big_byte_little_bit, 8, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 4> in = {{0x0123, 0x4567, 0x89AB, 0xCDEF}};
    array<uint32_t, 2> out;
    pack<big_byte_little_bit, 16, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint8_t, 4> in = {{0x45, 0x67, 0x89, 0xAB}};
    array<uint16_t, 2> out;
    pack<big_byte_little_bit, 8, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint8_t, 8> in = {{0x5, 0x4, 0x7, 0x6, 0x9, 0x8, 0xB, 0xA}};
    array<uint16_t, 2> out;
    pack<big_byte_little_bit, 4, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<bool, 16> in = {{false, false, true, true,
                           false, true, true, true,
                           true, false, true, false,
                           true, false, false, false}};
    array<uint16_t, 1> out;
    pack<big_byte_little_bit, 1, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{0xEC15}};
    assert(out == eout);
    }

    {
    array<bool, 8> in = {{false, false, true, true,
                          true, false, true, false}};
    array<uint8_t, 2> out;
    pack<big_byte_little_bit, 1, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 2> eout = {{0xC, 0x5}};
    assert(out == eout);
    }

}

void test_implodell() {

    {
    array<uint32_t, 2> in = {{0x01234567, 0x89ABCDEF}};
    array<uint32_t, 2> out;
    pack<little_byte_little_bit, 32, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    assert(in == out);
    }

    {
    array<uint8_t, 8> in = {{0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89}};
    array<uint32_t, 2> out;
    pack<little_byte_little_bit, 8, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint16_t, 4> in = {{0x4567, 0x0123, 0xCDEF, 0x89AB}};
    array<uint32_t, 2> out;
    pack<little_byte_little_bit, 16, 32>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.8x ", (int)out[i]);
    printf("\n");
    array<uint32_t, 2> eout = {{0x01234567, 0x89ABCDEF}};
    assert(out == eout);
    }

    {
    array<uint8_t, 4> in = {{0x67, 0x45, 0xAB, 0x89}};
    array<uint16_t, 2> out;
    pack<little_byte_little_bit, 8, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<uint8_t, 8> in = {{0x7, 0x6, 0x5, 0x4, 0xB, 0xA, 0x9, 0x8}};
    array<uint16_t, 2> out;
    pack<little_byte_little_bit, 4, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 2> eout = {{0x4567, 0x89AB}};
    assert(out == eout);
    }

    {
    array<bool, 16> in = {{true, false, true, false,
                           true, false, false, false,
                           false, false, true, true,
                           false, true, true, true}};
    array<uint16_t, 1> out;
    pack<little_byte_little_bit, 1, 16>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{0xEC15}};
    assert(out == eout);
    }

    {
    array<bool, 8> in = {{false, false, true, true,
                          true, false, true, false}};
    array<uint8_t, 2> out;
    pack<little_byte_little_bit, 1, 4>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.1x ", (int)out[i]);
    printf("\n");
    array<uint8_t, 2> eout = {{0xC, 0x5}};
    assert(out == eout);
    }

    {
    array<uint8_t, 3> in = {{4, 17, 31}};
    array<uint16_t, 1> out;
    pack<little_byte_little_bit, 5, 15>(in, out);
    for (unsigned i = 0; i < out.size(); ++i) printf("%.4x ", (int)out[i]);
    printf("\n");
    array<uint16_t, 1> eout = {{(31 << 10) | (17 << 5) | (4 << 0)}};
    assert(out == eout);
    }

}

int main() {

    test_explodebb();
    test_explodelb();
    test_explodebl();
    test_explodell();

    test_implodebb();
    test_implodelb();
    test_implodebl();
    test_implodell();

}

