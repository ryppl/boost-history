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
using namespace boost;
using namespace boost::integer;

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

struct Register {
    struct f_tag{};
    struct m_tag{};
    struct l_tag{};
    typedef bitfield_group<mpl::vector<
        member<uint8_t, f_tag, 3>,
        member<uint16_t, m_tag, 9>,
        member<uint32_t, l_tag, 4>
    > > type;
    type word;

    struct STR {
        uint8_t f:3;
        uint16_t m:9;
        uint32_t l:4;
    };
    union {
	    uint16_t padd;
	    STR value;
    } s, sl;
   
   f_type::reference f() { return f_type::reference(s.padd); }
   l_type::reference l() { return l_type::reference(s.padd); }
   m_type::reference m() { return m_type::reference(s.padd); }
};


void test_tt()
{
    Register r;
    // 0001|0001|0001|0010

    r.s.padd=0x1112;
    std::cout << "f=" << std::hex << int(r.s.value.f) <<  std::endl;
    std::cout << "m=" << std::hex << int(r.s.value.m) <<  std::endl;
    std::cout << "l=" << std::hex << int(r.s.value.l) <<  std::endl;

    r.word=0x1112;
    // 0001|001000110|100
    std::cout << "f()=" << std::hex << int(r.f()) << std::endl;
    std::cout << "m()=" << std::hex << int(r.m()) << std::endl;
    std::cout << "l()=" << std::hex << int(r.l()) << std::endl;


    //ASSERT_EQUALS(0, 1);

}

struct Register2 {
    struct high_tag{};
    struct low_tag{};
    struct mid_tag{};
    typedef bitfield_group<mpl::vector<
        member<uint16_t, high_tag, 4>,
        member<uint16_t, mid_tag, 8>,
        member<uint16_t, low_tag, 4>
    > > type;
    type word;

   result_of::at_key<type, high_tag>::type high() { return at_key<high_tag>(word); }
   result_of::at_key<type, mid_tag>::type mid() { return at_key<mid_tag>(word); }
   result_of::at_key<type, low_tag>::type low() { return at_key<low_tag>(word); }
   
   result_of::at_key<type, high_tag>::type::value_type get_high() const { return at_key<high_tag>(word).get(); }
   result_of::at_key<type, mid_tag>::type::value_type get_mid() const { return at_key<mid_tag>(word).get(); }
   result_of::at_key<type, low_tag>::type::value_type get_low() const { return at_key<low_tag>(word).get(); }
   
   uint16_t& all() { return word.value; }
   uint16_t all() const { return word.value; }
   
};

//-----------------------------------------------------------------------------
void test_assign()
{
    Register2 r;
   r.all() = 0;
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
    Register2 r;
   r.all() = 0x1234;
   ASSERT_EQUALS(r.high(), 0x12);
   ASSERT_EQUALS(r.low(), 0x34);
   ASSERT_EQUALS(r.mid(), 0x23);
   ASSERT_EQUALS(r.all(), 0x1234);
   ASSERT_EQUALS(r.single(), 0);
   r.all() = 0x1A34;
}

//-----------------------------------------------------------------------------
void test_invert()
{
    Register2 r;
   r.all() = 0xff00;
   ASSERT_EQUALS(~r.high(), 0x00);
   ASSERT_EQUALS(~r.mid(), 0x0f);
   ASSERT_EQUALS(~r.low(), 0xff);
   ASSERT_EQUALS(~r.all(), 0x00ff);
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

