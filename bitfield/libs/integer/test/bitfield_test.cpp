#include <iostream>
#include <cassert>
#include "boost/integer/bitfield.hpp"
#include "boost/integer/bitfield_dcl.hpp"

#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>

#define ASSERT_EQUALS(a,b) assert((a) == (b))
#define ASSERT_UNEQUALS(a,b) assert((a) != (b))
#define ASSERT(a) assert((a))

//typedef unsigned short uint16_t;
//typedef unsigned char uint8_t;

struct Register
{
   typedef uint16_t T;
   T word;
   struct STR {
   uint8_t f:3;
   uint16_t m:9;
   uint32_t l:4;
   };
   union {
	uint16_t padd;
	STR value;
   } s, sl;
   
   typedef boost::integer::bitfield_traits<T, 0, 2, uint8_t>    f_type;
   typedef boost::integer::bitfield_traits<T, 3, 11>    m_type;
   typedef boost::integer::bitfield_traits<T, 12, 15, uint32_t>    l_type;
   typedef boost::integer::bitfield_traits<T, 0, 7>   high_type;
   typedef boost::integer::bitfield_traits<T, 4, 11>   mid_type; // Intentionally overlap
   typedef boost::integer::bitfield_traits<T, 8, 15>    low_type;
   typedef boost::integer::bitfield_traits<T, 0, 15>   all_type;
   typedef boost::integer::bitfield_traits<T, 4, 4>    single_type;
   //typedef bitfield_traits<T, 2, 4, signed char>    ss_type;
   typedef boost::integer::bitfield_traits<T, 2, 4, signed char>    ss_type;

   f_type::reference f() { return f_type::reference(s.padd); }
   l_type::reference l() { return l_type::reference(s.padd); }
   m_type::reference m() { return m_type::reference(s.padd); }

   high_type::reference high() { return high_type::reference(word); }
   mid_type::reference mid() { return mid_type::reference(word); }
   low_type::reference low() { return low_type::reference(word); }
   all_type::reference all() { return all_type::reference(word); }
   single_type::reference single() { return single_type::reference(word); }
   ss_type::reference ss() { return ss_type::reference(word); }


   f_type::value::value_type f() const { return f_type::value(word).get(); }
   l_type::value::value_type l() const { return l_type::value(word).get(); }
   m_type::value::value_type m() const { return m_type::value(word).get(); }
   high_type::value::value_type high() const { return high_type::value(word).get(); }
   mid_type::value::value_type mid() const { return mid_type::value(word).get(); }
   low_type::value::value_type low() const { return low_type::value(word).get(); }
   all_type::value::value_type all() const { return all_type::value(word).get(); }
   single_type::value::value_type single() const { return single_type::value(word).get(); }
   ss_type::value::value_type ss() const { return ss_type::value(word).get(); }


};

struct Register2
{
   typedef uint32_t T;
   T word;
   struct STR {
   unsigned char f:3;
   uint16_t m:9;
   int32_t l:4;
   };
   union {
	uint32_t padd;
	STR value;
   } s, sl;
   
   BOOST_BITFIELD_DCL(uint32_t, word, unsigned char, f, 0, 2);
   BOOST_BITFIELD_DCL(uint32_t, word, uint32_t, m, 3, 11);
   BOOST_BITFIELD_DCL(uint32_t, word, uint32_t, l, 12, 15);
   
   typedef boost::integer::bitfield_traits<T, 0, 7>   high_type;
   typedef boost::integer::bitfield_traits<T, 4, 11>   mid_type; // Intentionally overlap
   typedef boost::integer::bitfield_traits<T, 8, 15>    low_type;
   typedef boost::integer::bitfield_traits<T, 0, 15>   all_type;
   typedef boost::integer::bitfield_traits<T, 4, 4>    single_type;
   typedef boost::integer::bitfield_traits<T, 2, 4, signed char>    ss_type;

   high_type::reference high() { return high_type::reference(word); }
   mid_type::reference mid() { return mid_type::reference(word); }
   low_type::reference low() { return low_type::reference(word); }
   all_type::reference all() { return all_type::reference(word); }
   single_type::reference single() { return single_type::reference(word); }
   ss_type::reference ss() { return ss_type::reference(word); }

   high_type::value::value_type high() const { return high_type::value(word).get(); }
   mid_type::value::value_type mid() const { return mid_type::value(word).get(); }
   low_type::value::value_type low() const { return low_type::value(word).get(); }
   all_type::value::value_type all() const { return all_type::value(word).get(); }
   single_type::value::value_type single() const { return single_type::value(word).get(); }
   ss_type::value::value_type ss() const { return ss_type::value(word).get(); }


};


namespace
{
   Register r;
}

void test_tt()
{
    Register r;
    // 0001|0001|0001|0010

    r.s.padd=0x1112;
    std::cout << "f=" << std::hex << int(r.s.value.f) <<  std::endl;
    std::cout << "m=" << std::hex << int(r.s.value.m) <<  std::endl;
    std::cout << "l=" << std::hex << int(r.s.value.l) <<  std::endl;

    r.s.padd=0x1112;
    // 0001|001000110|100
    std::cout << "f()=" << std::hex << int(r.f()) << std::endl;
    std::cout << "m()=" << std::hex << int(r.m()) << std::endl;
    std::cout << "l()=" << std::hex << int(r.l()) << std::endl;


    //ASSERT_EQUALS(0, 1);

}

//-----------------------------------------------------------------------------
void test_assign()
{
   r.word = 0;
   r.high() = 0x12;
   //printf("word=%x\n",int(r.word));
   //printf("all=%x\n",int(r.all()));
   ASSERT_EQUALS(r.all(), 0x1200);
   r.low() = 0x34;
   //printf("word=%x\n",int(r.word));
   //printf("all=%x\n",int(r.all()));
   ASSERT_EQUALS(r.all(), 0x1234);
   r.mid() = 0xab;
   //printf("word=%x\n",int(r.word));
   //printf("all=%x\n",int(r.all()));

   ASSERT_EQUALS(r.all(), 0x1ab4);
   r.all() = 0x4321;
   //printf("word=%x\n",int(r.word));
   //printf("all=%x\n",int(r.all()));
   ASSERT_EQUALS(r.all(), 0x4321);
   r.single() = 1;
   //printf("word=%x\n",int(r.word));
   //printf("all=%x\n",int(r.all()));
   ASSERT_EQUALS(r.all(), 0x4b21);
}

//-----------------------------------------------------------------------------
void test_get()
{
   r.word = 0x1234;
   ASSERT_EQUALS(r.high(), 0x12);
   ASSERT_EQUALS(r.low(), 0x34);
   ASSERT_EQUALS(r.mid(), 0x23);
   ASSERT_EQUALS(r.all(), 0x1234);
   ASSERT_EQUALS(r.single(), 0);
   r.word = 0x1A34;
   ASSERT_EQUALS(r.single(), 1);
}

//-----------------------------------------------------------------------------
void test_flags()
{

   
   typedef uint16_t T;
   ASSERT_EQUALS((boost::integer::bitfield<T, 8, 15>::static_value_to_storage<0x12>::value), 0x12);
   ASSERT_EQUALS((boost::integer::bitfield<T, 8, 15>::get_flags(0x12)), 0x12);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 11>::static_value_to_storage<0x34>::value), 0x0340);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 11>::get_flags(0x34)), 0x0340);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 7>::static_value_to_storage<0x56>::value), 0x5600);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 7>::get_flags(0x56)), 0x5600);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 15>::get_flags(0xabcd)), 0xabcd);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 4>::get_flags(0)), 0);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 4>::get_flags(1)), 0x0800);
}

//-----------------------------------------------------------------------------
void test_invert()
{
   r.word = 0xff00;
   ASSERT_EQUALS(~r.high(), 0x00);
   ASSERT_EQUALS(~r.mid(), 0x0f);
   ASSERT_EQUALS(~r.low(), 0xff);
   ASSERT_EQUALS(~r.all(), 0x00ff);
   ASSERT_EQUALS(~r.single(), 0);
}

//-----------------------------------------------------------------------------
void test_traits()
{
   typedef uint8_t T;
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 0>::FIRST), 0);
   ASSERT_EQUALS((boost::integer::bitfield<T, 3, 5>::FIRST), 3);
   ASSERT_EQUALS((boost::integer::bitfield<T, 7, 7>::FIRST), 7);

   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 0>::LAST), 0);
   ASSERT_EQUALS((boost::integer::bitfield<T, 3, 5>::LAST), 5);
   ASSERT_EQUALS((boost::integer::bitfield<T, 7, 7>::LAST), 7);

   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 0>::WIDTH), 1);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 7>::WIDTH), 4);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 7>::WIDTH), 8);

   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 0>::VAL_MASK), 0x01);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 4>::VAL_MASK), 0x01);
   ASSERT_EQUALS((boost::integer::bitfield<T, 1, 3>::VAL_MASK), 0x07);
   ASSERT_EQUALS((boost::integer::bitfield<T, 7, 7>::VAL_MASK), 0x01);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 7>::VAL_MASK), 0xff);

   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 0>::FIELD_MASK), 0x80);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 3>::FIELD_MASK), 0xf0);
   ASSERT_EQUALS((boost::integer::bitfield<T, 4, 7>::FIELD_MASK), 0x0f);
   ASSERT_EQUALS((boost::integer::bitfield<T, 7, 7>::FIELD_MASK), 0x01);
   ASSERT_EQUALS((boost::integer::bitfield<T, 0, 7>::FIELD_MASK), 0xff);
}

//-----------------------------------------------------------------------------
template <class B, class CB>
void do_bit_access_test(B bf, CB& cbf)
{
   // Manually set and clear each bit (using r.word) in the bitfield and verify
   // that the changes are reflected while reading bf[i].
   // Also test bit inversion and constant access at the same time.
   for (int i=0; i<B::WIDTH; ++i)
   {
      r.word = htons(1u << (B::FIRST + i));
      ASSERT_EQUALS(cbf[i], true);
      ASSERT_EQUALS(~cbf[i], false);

      r.word = htons(~ (1u << (B::FIRST + i)));
      ASSERT_EQUALS(bf[i], false);
      ASSERT_EQUALS(~cbf[i], true);
   }

   // Set and clear bf[i] and verify that the changes are reflected while
   // reading r.word.
   for (int i=0; i<B::WIDTH; ++i)
   {
      r.word = htons(0);
      bf[i] = 1u;
      ASSERT_EQUALS(htons(r.word), 1u << (B::FIRST + i));

      r.word = htons(0xffff);
      bf[i] = 0;
      ASSERT_EQUALS( htons(r.word), (uint16_t)(~(1u << (B::FIRST + i))) );
   }
}

//-----------------------------------------------------------------------------
void test_bit_access()
{
    r.high()=0;
    bool b = r.high()[0];
    ASSERT_EQUALS(b, false);
    r.high() = 0x01;
    ASSERT_EQUALS(r.high(), 0x01);
    ASSERT_EQUALS(r.high()[0], true);
    ASSERT_EQUALS(r.high()[1], false);
    ASSERT_EQUALS(r.high()[2], false);
    ASSERT_EQUALS(r.high()[3], false);

#if 0
   Register& cr = r;
   do_bit_access_test(r.high(), cr.high());
   do_bit_access_test(r.mid(), cr.mid());
   do_bit_access_test(r.low(), cr.low());
   do_bit_access_test(r.all(), cr.all());
   do_bit_access_test(r.single(), cr.single());
#endif
}

//-----------------------------------------------------------------------------

void test_signed_unsigned() {
   r.word = 0;

   r.ss() = -2;
   r.ss()++;
   r.ss()--;
   int i = r.ss();
   signed char c= r.ss();
   //printf("----- l=%d %x\n", int(r.ss()), int(r.ss()));
   //std::cout << std::hex << -2 << " " << r.word << " " << int(r.ss) << std::endl;
   ASSERT_EQUALS(r.ss(), -2);
   ASSERT_EQUALS(i, -2);
   ASSERT_EQUALS(c, -2);

}

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    test_tt();
    test_assign();

    test_get();
    test_flags();
    test_invert();
    test_traits();
    test_bit_access();
    test_signed_unsigned();

    std::cout << "All tests successful!\n";
    return 0;
}

