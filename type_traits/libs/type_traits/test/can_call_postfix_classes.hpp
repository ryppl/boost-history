struct ret { };
ret ret_val;

class C000 { C000(); public: C000(int) { } };
void operator++(C000, int) { }

class C001 { C001(); public: C001(int) { } };
ret  operator++(C001, int) { return ret_val; }

class C002 { C002(); public: C002(int) { } };
ret  const operator++(C002, int) { return ret_val; }

class C003 { C003(); public: C003(int) { } };
ret  volatile operator++(C003, int) { return ret_val; }

class C004 { C004(); public: C004(int) { } };
ret  const volatile operator++(C004, int) { return ret_val; }

class C005 { C005(); public: C005(int) { } };
ret  & operator++(C005, int) { return ret_val; }

class C006 { C006(); public: C006(int) { } };
ret  const & operator++(C006, int) { return ret_val; }

class C007 { C007(); public: C007(int) { } };
ret  volatile & operator++(C007, int) { return ret_val; }

class C008 { C008(); public: C008(int) { } };
ret  const volatile & operator++(C008, int) { return ret_val; }

class C009 { C009(); public: C009(int) { } };
void operator++(C009 const, int) { }

class C010 { C010(); public: C010(int) { } };
ret  operator++(C010 const, int) { return ret_val; }

class C011 { C011(); public: C011(int) { } };
ret  const operator++(C011 const, int) { return ret_val; }

class C012 { C012(); public: C012(int) { } };
ret  volatile operator++(C012 const, int) { return ret_val; }

class C013 { C013(); public: C013(int) { } };
ret  const volatile operator++(C013 const, int) { return ret_val; }

class C014 { C014(); public: C014(int) { } };
ret  & operator++(C014 const, int) { return ret_val; }

class C015 { C015(); public: C015(int) { } };
ret  const & operator++(C015 const, int) { return ret_val; }

class C016 { C016(); public: C016(int) { } };
ret  volatile & operator++(C016 const, int) { return ret_val; }

class C017 { C017(); public: C017(int) { } };
ret  const volatile & operator++(C017 const, int) { return ret_val; }

class C018 { C018(); public: C018(int) { } };
void operator++(C018 volatile, int) { }

class C019 { C019(); public: C019(int) { } };
ret  operator++(C019 volatile, int) { return ret_val; }

class C020 { C020(); public: C020(int) { } };
ret  const operator++(C020 volatile, int) { return ret_val; }

class C021 { C021(); public: C021(int) { } };
ret  volatile operator++(C021 volatile, int) { return ret_val; }

class C022 { C022(); public: C022(int) { } };
ret  const volatile operator++(C022 volatile, int) { return ret_val; }

class C023 { C023(); public: C023(int) { } };
ret  & operator++(C023 volatile, int) { return ret_val; }

class C024 { C024(); public: C024(int) { } };
ret  const & operator++(C024 volatile, int) { return ret_val; }

class C025 { C025(); public: C025(int) { } };
ret  volatile & operator++(C025 volatile, int) { return ret_val; }

class C026 { C026(); public: C026(int) { } };
ret  const volatile & operator++(C026 volatile, int) { return ret_val; }

class C027 { C027(); public: C027(int) { } };
void operator++(C027 const volatile, int) { }

class C028 { C028(); public: C028(int) { } };
ret  operator++(C028 const volatile, int) { return ret_val; }

class C029 { C029(); public: C029(int) { } };
ret  const operator++(C029 const volatile, int) { return ret_val; }

class C030 { C030(); public: C030(int) { } };
ret  volatile operator++(C030 const volatile, int) { return ret_val; }

class C031 { C031(); public: C031(int) { } };
ret  const volatile operator++(C031 const volatile, int) { return ret_val; }

class C032 { C032(); public: C032(int) { } };
ret  & operator++(C032 const volatile, int) { return ret_val; }

class C033 { C033(); public: C033(int) { } };
ret  const & operator++(C033 const volatile, int) { return ret_val; }

class C034 { C034(); public: C034(int) { } };
ret  volatile & operator++(C034 const volatile, int) { return ret_val; }

class C035 { C035(); public: C035(int) { } };
ret  const volatile & operator++(C035 const volatile, int) { return ret_val; }

class C036 { C036(); public: C036(int) { } };
void operator++(C036 &, int) { }

class C037 { C037(); public: C037(int) { } };
ret  operator++(C037 &, int) { return ret_val; }

class C038 { C038(); public: C038(int) { } };
ret  const operator++(C038 &, int) { return ret_val; }

class C039 { C039(); public: C039(int) { } };
ret  volatile operator++(C039 &, int) { return ret_val; }

class C040 { C040(); public: C040(int) { } };
ret  const volatile operator++(C040 &, int) { return ret_val; }

class C041 { C041(); public: C041(int) { } };
ret  & operator++(C041 &, int) { return ret_val; }

class C042 { C042(); public: C042(int) { } };
ret  const & operator++(C042 &, int) { return ret_val; }

class C043 { C043(); public: C043(int) { } };
ret  volatile & operator++(C043 &, int) { return ret_val; }

class C044 { C044(); public: C044(int) { } };
ret  const volatile & operator++(C044 &, int) { return ret_val; }

class C045 { C045(); public: C045(int) { } };
void operator++(C045 const &, int) { }

class C046 { C046(); public: C046(int) { } };
ret  operator++(C046 const &, int) { return ret_val; }

class C047 { C047(); public: C047(int) { } };
ret  const operator++(C047 const &, int) { return ret_val; }

class C048 { C048(); public: C048(int) { } };
ret  volatile operator++(C048 const &, int) { return ret_val; }

class C049 { C049(); public: C049(int) { } };
ret  const volatile operator++(C049 const &, int) { return ret_val; }

class C050 { C050(); public: C050(int) { } };
ret  & operator++(C050 const &, int) { return ret_val; }

class C051 { C051(); public: C051(int) { } };
ret  const & operator++(C051 const &, int) { return ret_val; }

class C052 { C052(); public: C052(int) { } };
ret  volatile & operator++(C052 const &, int) { return ret_val; }

class C053 { C053(); public: C053(int) { } };
ret  const volatile & operator++(C053 const &, int) { return ret_val; }

class C054 { C054(); public: C054(int) { } };
void operator++(C054 volatile &, int) { }

class C055 { C055(); public: C055(int) { } };
ret  operator++(C055 volatile &, int) { return ret_val; }

class C056 { C056(); public: C056(int) { } };
ret  const operator++(C056 volatile &, int) { return ret_val; }

class C057 { C057(); public: C057(int) { } };
ret  volatile operator++(C057 volatile &, int) { return ret_val; }

class C058 { C058(); public: C058(int) { } };
ret  const volatile operator++(C058 volatile &, int) { return ret_val; }

class C059 { C059(); public: C059(int) { } };
ret  & operator++(C059 volatile &, int) { return ret_val; }

class C060 { C060(); public: C060(int) { } };
ret  const & operator++(C060 volatile &, int) { return ret_val; }

class C061 { C061(); public: C061(int) { } };
ret  volatile & operator++(C061 volatile &, int) { return ret_val; }

class C062 { C062(); public: C062(int) { } };
ret  const volatile & operator++(C062 volatile &, int) { return ret_val; }

class C063 { C063(); public: C063(int) { } };
void operator++(C063 const volatile &, int) { }

class C064 { C064(); public: C064(int) { } };
ret  operator++(C064 const volatile &, int) { return ret_val; }

class C065 { C065(); public: C065(int) { } };
ret  const operator++(C065 const volatile &, int) { return ret_val; }

class C066 { C066(); public: C066(int) { } };
ret  volatile operator++(C066 const volatile &, int) { return ret_val; }

class C067 { C067(); public: C067(int) { } };
ret  const volatile operator++(C067 const volatile &, int) { return ret_val; }

class C068 { C068(); public: C068(int) { } };
ret  & operator++(C068 const volatile &, int) { return ret_val; }

class C069 { C069(); public: C069(int) { } };
ret  const & operator++(C069 const volatile &, int) { return ret_val; }

class C070 { C070(); public: C070(int) { } };
ret  volatile & operator++(C070 const volatile &, int) { return ret_val; }

class C071 { C071(); public: C071(int) { } };
ret  const volatile & operator++(C071 const volatile &, int) { return ret_val; }

