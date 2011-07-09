struct ret { };
ret ret_val;

class C000 { C000(); public: C000(int) { } };
void operator+(C000, C000) { }

class C001 { C001(); public: C001(int) { } };
ret  operator+(C001, C001) { return ret_val; }

class C002 { C002(); public: C002(int) { } };
ret  const operator+(C002, C002) { return ret_val; }

class C003 { C003(); public: C003(int) { } };
ret  volatile operator+(C003, C003) { return ret_val; }

class C004 { C004(); public: C004(int) { } };
ret  const volatile operator+(C004, C004) { return ret_val; }

class C005 { C005(); public: C005(int) { } };
ret  & operator+(C005, C005) { return ret_val; }

class C006 { C006(); public: C006(int) { } };
ret  const & operator+(C006, C006) { return ret_val; }

class C007 { C007(); public: C007(int) { } };
ret  volatile & operator+(C007, C007) { return ret_val; }

class C008 { C008(); public: C008(int) { } };
ret  const volatile & operator+(C008, C008) { return ret_val; }

class C009 { C009(); public: C009(int) { } };
void operator+(C009, C009 const) { }

class C010 { C010(); public: C010(int) { } };
ret  operator+(C010, C010 const) { return ret_val; }

class C011 { C011(); public: C011(int) { } };
ret  const operator+(C011, C011 const) { return ret_val; }

class C012 { C012(); public: C012(int) { } };
ret  volatile operator+(C012, C012 const) { return ret_val; }

class C013 { C013(); public: C013(int) { } };
ret  const volatile operator+(C013, C013 const) { return ret_val; }

class C014 { C014(); public: C014(int) { } };
ret  & operator+(C014, C014 const) { return ret_val; }

class C015 { C015(); public: C015(int) { } };
ret  const & operator+(C015, C015 const) { return ret_val; }

class C016 { C016(); public: C016(int) { } };
ret  volatile & operator+(C016, C016 const) { return ret_val; }

class C017 { C017(); public: C017(int) { } };
ret  const volatile & operator+(C017, C017 const) { return ret_val; }

class C018 { C018(); public: C018(int) { } };
void operator+(C018, C018 volatile) { }

class C019 { C019(); public: C019(int) { } };
ret  operator+(C019, C019 volatile) { return ret_val; }

class C020 { C020(); public: C020(int) { } };
ret  const operator+(C020, C020 volatile) { return ret_val; }

class C021 { C021(); public: C021(int) { } };
ret  volatile operator+(C021, C021 volatile) { return ret_val; }

class C022 { C022(); public: C022(int) { } };
ret  const volatile operator+(C022, C022 volatile) { return ret_val; }

class C023 { C023(); public: C023(int) { } };
ret  & operator+(C023, C023 volatile) { return ret_val; }

class C024 { C024(); public: C024(int) { } };
ret  const & operator+(C024, C024 volatile) { return ret_val; }

class C025 { C025(); public: C025(int) { } };
ret  volatile & operator+(C025, C025 volatile) { return ret_val; }

class C026 { C026(); public: C026(int) { } };
ret  const volatile & operator+(C026, C026 volatile) { return ret_val; }

class C027 { C027(); public: C027(int) { } };
void operator+(C027, C027 const volatile) { }

class C028 { C028(); public: C028(int) { } };
ret  operator+(C028, C028 const volatile) { return ret_val; }

class C029 { C029(); public: C029(int) { } };
ret  const operator+(C029, C029 const volatile) { return ret_val; }

class C030 { C030(); public: C030(int) { } };
ret  volatile operator+(C030, C030 const volatile) { return ret_val; }

class C031 { C031(); public: C031(int) { } };
ret  const volatile operator+(C031, C031 const volatile) { return ret_val; }

class C032 { C032(); public: C032(int) { } };
ret  & operator+(C032, C032 const volatile) { return ret_val; }

class C033 { C033(); public: C033(int) { } };
ret  const & operator+(C033, C033 const volatile) { return ret_val; }

class C034 { C034(); public: C034(int) { } };
ret  volatile & operator+(C034, C034 const volatile) { return ret_val; }

class C035 { C035(); public: C035(int) { } };
ret  const volatile & operator+(C035, C035 const volatile) { return ret_val; }

class C036 { C036(); public: C036(int) { } };
void operator+(C036, C036 &) { }

class C037 { C037(); public: C037(int) { } };
ret  operator+(C037, C037 &) { return ret_val; }

class C038 { C038(); public: C038(int) { } };
ret  const operator+(C038, C038 &) { return ret_val; }

class C039 { C039(); public: C039(int) { } };
ret  volatile operator+(C039, C039 &) { return ret_val; }

class C040 { C040(); public: C040(int) { } };
ret  const volatile operator+(C040, C040 &) { return ret_val; }

class C041 { C041(); public: C041(int) { } };
ret  & operator+(C041, C041 &) { return ret_val; }

class C042 { C042(); public: C042(int) { } };
ret  const & operator+(C042, C042 &) { return ret_val; }

class C043 { C043(); public: C043(int) { } };
ret  volatile & operator+(C043, C043 &) { return ret_val; }

class C044 { C044(); public: C044(int) { } };
ret  const volatile & operator+(C044, C044 &) { return ret_val; }

class C045 { C045(); public: C045(int) { } };
void operator+(C045, C045 const &) { }

class C046 { C046(); public: C046(int) { } };
ret  operator+(C046, C046 const &) { return ret_val; }

class C047 { C047(); public: C047(int) { } };
ret  const operator+(C047, C047 const &) { return ret_val; }

class C048 { C048(); public: C048(int) { } };
ret  volatile operator+(C048, C048 const &) { return ret_val; }

class C049 { C049(); public: C049(int) { } };
ret  const volatile operator+(C049, C049 const &) { return ret_val; }

class C050 { C050(); public: C050(int) { } };
ret  & operator+(C050, C050 const &) { return ret_val; }

class C051 { C051(); public: C051(int) { } };
ret  const & operator+(C051, C051 const &) { return ret_val; }

class C052 { C052(); public: C052(int) { } };
ret  volatile & operator+(C052, C052 const &) { return ret_val; }

class C053 { C053(); public: C053(int) { } };
ret  const volatile & operator+(C053, C053 const &) { return ret_val; }

class C054 { C054(); public: C054(int) { } };
void operator+(C054, C054 volatile &) { }

class C055 { C055(); public: C055(int) { } };
ret  operator+(C055, C055 volatile &) { return ret_val; }

class C056 { C056(); public: C056(int) { } };
ret  const operator+(C056, C056 volatile &) { return ret_val; }

class C057 { C057(); public: C057(int) { } };
ret  volatile operator+(C057, C057 volatile &) { return ret_val; }

class C058 { C058(); public: C058(int) { } };
ret  const volatile operator+(C058, C058 volatile &) { return ret_val; }

class C059 { C059(); public: C059(int) { } };
ret  & operator+(C059, C059 volatile &) { return ret_val; }

class C060 { C060(); public: C060(int) { } };
ret  const & operator+(C060, C060 volatile &) { return ret_val; }

class C061 { C061(); public: C061(int) { } };
ret  volatile & operator+(C061, C061 volatile &) { return ret_val; }

class C062 { C062(); public: C062(int) { } };
ret  const volatile & operator+(C062, C062 volatile &) { return ret_val; }

class C063 { C063(); public: C063(int) { } };
void operator+(C063, C063 const volatile &) { }

class C064 { C064(); public: C064(int) { } };
ret  operator+(C064, C064 const volatile &) { return ret_val; }

class C065 { C065(); public: C065(int) { } };
ret  const operator+(C065, C065 const volatile &) { return ret_val; }

class C066 { C066(); public: C066(int) { } };
ret  volatile operator+(C066, C066 const volatile &) { return ret_val; }

class C067 { C067(); public: C067(int) { } };
ret  const volatile operator+(C067, C067 const volatile &) { return ret_val; }

class C068 { C068(); public: C068(int) { } };
ret  & operator+(C068, C068 const volatile &) { return ret_val; }

class C069 { C069(); public: C069(int) { } };
ret  const & operator+(C069, C069 const volatile &) { return ret_val; }

class C070 { C070(); public: C070(int) { } };
ret  volatile & operator+(C070, C070 const volatile &) { return ret_val; }

class C071 { C071(); public: C071(int) { } };
ret  const volatile & operator+(C071, C071 const volatile &) { return ret_val; }

class C072 { C072(); public: C072(int) { } };
void operator+(C072 const, C072) { }

class C073 { C073(); public: C073(int) { } };
ret  operator+(C073 const, C073) { return ret_val; }

class C074 { C074(); public: C074(int) { } };
ret  const operator+(C074 const, C074) { return ret_val; }

class C075 { C075(); public: C075(int) { } };
ret  volatile operator+(C075 const, C075) { return ret_val; }

class C076 { C076(); public: C076(int) { } };
ret  const volatile operator+(C076 const, C076) { return ret_val; }

class C077 { C077(); public: C077(int) { } };
ret  & operator+(C077 const, C077) { return ret_val; }

class C078 { C078(); public: C078(int) { } };
ret  const & operator+(C078 const, C078) { return ret_val; }

class C079 { C079(); public: C079(int) { } };
ret  volatile & operator+(C079 const, C079) { return ret_val; }

class C080 { C080(); public: C080(int) { } };
ret  const volatile & operator+(C080 const, C080) { return ret_val; }

class C081 { C081(); public: C081(int) { } };
void operator+(C081 const, C081 const) { }

class C082 { C082(); public: C082(int) { } };
ret  operator+(C082 const, C082 const) { return ret_val; }

class C083 { C083(); public: C083(int) { } };
ret  const operator+(C083 const, C083 const) { return ret_val; }

class C084 { C084(); public: C084(int) { } };
ret  volatile operator+(C084 const, C084 const) { return ret_val; }

class C085 { C085(); public: C085(int) { } };
ret  const volatile operator+(C085 const, C085 const) { return ret_val; }

class C086 { C086(); public: C086(int) { } };
ret  & operator+(C086 const, C086 const) { return ret_val; }

class C087 { C087(); public: C087(int) { } };
ret  const & operator+(C087 const, C087 const) { return ret_val; }

class C088 { C088(); public: C088(int) { } };
ret  volatile & operator+(C088 const, C088 const) { return ret_val; }

class C089 { C089(); public: C089(int) { } };
ret  const volatile & operator+(C089 const, C089 const) { return ret_val; }

class C090 { C090(); public: C090(int) { } };
void operator+(C090 const, C090 volatile) { }

class C091 { C091(); public: C091(int) { } };
ret  operator+(C091 const, C091 volatile) { return ret_val; }

class C092 { C092(); public: C092(int) { } };
ret  const operator+(C092 const, C092 volatile) { return ret_val; }

class C093 { C093(); public: C093(int) { } };
ret  volatile operator+(C093 const, C093 volatile) { return ret_val; }

class C094 { C094(); public: C094(int) { } };
ret  const volatile operator+(C094 const, C094 volatile) { return ret_val; }

class C095 { C095(); public: C095(int) { } };
ret  & operator+(C095 const, C095 volatile) { return ret_val; }

class C096 { C096(); public: C096(int) { } };
ret  const & operator+(C096 const, C096 volatile) { return ret_val; }

class C097 { C097(); public: C097(int) { } };
ret  volatile & operator+(C097 const, C097 volatile) { return ret_val; }

class C098 { C098(); public: C098(int) { } };
ret  const volatile & operator+(C098 const, C098 volatile) { return ret_val; }

class C099 { C099(); public: C099(int) { } };
void operator+(C099 const, C099 const volatile) { }

class C100 { C100(); public: C100(int) { } };
ret  operator+(C100 const, C100 const volatile) { return ret_val; }

class C101 { C101(); public: C101(int) { } };
ret  const operator+(C101 const, C101 const volatile) { return ret_val; }

class C102 { C102(); public: C102(int) { } };
ret  volatile operator+(C102 const, C102 const volatile) { return ret_val; }

class C103 { C103(); public: C103(int) { } };
ret  const volatile operator+(C103 const, C103 const volatile) { return ret_val; }

class C104 { C104(); public: C104(int) { } };
ret  & operator+(C104 const, C104 const volatile) { return ret_val; }

class C105 { C105(); public: C105(int) { } };
ret  const & operator+(C105 const, C105 const volatile) { return ret_val; }

class C106 { C106(); public: C106(int) { } };
ret  volatile & operator+(C106 const, C106 const volatile) { return ret_val; }

class C107 { C107(); public: C107(int) { } };
ret  const volatile & operator+(C107 const, C107 const volatile) { return ret_val; }

class C108 { C108(); public: C108(int) { } };
void operator+(C108 const, C108 &) { }

class C109 { C109(); public: C109(int) { } };
ret  operator+(C109 const, C109 &) { return ret_val; }

class C110 { C110(); public: C110(int) { } };
ret  const operator+(C110 const, C110 &) { return ret_val; }

class C111 { C111(); public: C111(int) { } };
ret  volatile operator+(C111 const, C111 &) { return ret_val; }

class C112 { C112(); public: C112(int) { } };
ret  const volatile operator+(C112 const, C112 &) { return ret_val; }

class C113 { C113(); public: C113(int) { } };
ret  & operator+(C113 const, C113 &) { return ret_val; }

class C114 { C114(); public: C114(int) { } };
ret  const & operator+(C114 const, C114 &) { return ret_val; }

class C115 { C115(); public: C115(int) { } };
ret  volatile & operator+(C115 const, C115 &) { return ret_val; }

class C116 { C116(); public: C116(int) { } };
ret  const volatile & operator+(C116 const, C116 &) { return ret_val; }

class C117 { C117(); public: C117(int) { } };
void operator+(C117 const, C117 const &) { }

class C118 { C118(); public: C118(int) { } };
ret  operator+(C118 const, C118 const &) { return ret_val; }

class C119 { C119(); public: C119(int) { } };
ret  const operator+(C119 const, C119 const &) { return ret_val; }

class C120 { C120(); public: C120(int) { } };
ret  volatile operator+(C120 const, C120 const &) { return ret_val; }

class C121 { C121(); public: C121(int) { } };
ret  const volatile operator+(C121 const, C121 const &) { return ret_val; }

class C122 { C122(); public: C122(int) { } };
ret  & operator+(C122 const, C122 const &) { return ret_val; }

class C123 { C123(); public: C123(int) { } };
ret  const & operator+(C123 const, C123 const &) { return ret_val; }

class C124 { C124(); public: C124(int) { } };
ret  volatile & operator+(C124 const, C124 const &) { return ret_val; }

class C125 { C125(); public: C125(int) { } };
ret  const volatile & operator+(C125 const, C125 const &) { return ret_val; }

class C126 { C126(); public: C126(int) { } };
void operator+(C126 const, C126 volatile &) { }

class C127 { C127(); public: C127(int) { } };
ret  operator+(C127 const, C127 volatile &) { return ret_val; }

class C128 { C128(); public: C128(int) { } };
ret  const operator+(C128 const, C128 volatile &) { return ret_val; }

class C129 { C129(); public: C129(int) { } };
ret  volatile operator+(C129 const, C129 volatile &) { return ret_val; }

class C130 { C130(); public: C130(int) { } };
ret  const volatile operator+(C130 const, C130 volatile &) { return ret_val; }

class C131 { C131(); public: C131(int) { } };
ret  & operator+(C131 const, C131 volatile &) { return ret_val; }

class C132 { C132(); public: C132(int) { } };
ret  const & operator+(C132 const, C132 volatile &) { return ret_val; }

class C133 { C133(); public: C133(int) { } };
ret  volatile & operator+(C133 const, C133 volatile &) { return ret_val; }

class C134 { C134(); public: C134(int) { } };
ret  const volatile & operator+(C134 const, C134 volatile &) { return ret_val; }

class C135 { C135(); public: C135(int) { } };
void operator+(C135 const, C135 const volatile &) { }

class C136 { C136(); public: C136(int) { } };
ret  operator+(C136 const, C136 const volatile &) { return ret_val; }

class C137 { C137(); public: C137(int) { } };
ret  const operator+(C137 const, C137 const volatile &) { return ret_val; }

class C138 { C138(); public: C138(int) { } };
ret  volatile operator+(C138 const, C138 const volatile &) { return ret_val; }

class C139 { C139(); public: C139(int) { } };
ret  const volatile operator+(C139 const, C139 const volatile &) { return ret_val; }

class C140 { C140(); public: C140(int) { } };
ret  & operator+(C140 const, C140 const volatile &) { return ret_val; }

class C141 { C141(); public: C141(int) { } };
ret  const & operator+(C141 const, C141 const volatile &) { return ret_val; }

class C142 { C142(); public: C142(int) { } };
ret  volatile & operator+(C142 const, C142 const volatile &) { return ret_val; }

class C143 { C143(); public: C143(int) { } };
ret  const volatile & operator+(C143 const, C143 const volatile &) { return ret_val; }

class C144 { C144(); public: C144(int) { } };
void operator+(C144 volatile, C144) { }

class C145 { C145(); public: C145(int) { } };
ret  operator+(C145 volatile, C145) { return ret_val; }

class C146 { C146(); public: C146(int) { } };
ret  const operator+(C146 volatile, C146) { return ret_val; }

class C147 { C147(); public: C147(int) { } };
ret  volatile operator+(C147 volatile, C147) { return ret_val; }

class C148 { C148(); public: C148(int) { } };
ret  const volatile operator+(C148 volatile, C148) { return ret_val; }

class C149 { C149(); public: C149(int) { } };
ret  & operator+(C149 volatile, C149) { return ret_val; }

class C150 { C150(); public: C150(int) { } };
ret  const & operator+(C150 volatile, C150) { return ret_val; }

class C151 { C151(); public: C151(int) { } };
ret  volatile & operator+(C151 volatile, C151) { return ret_val; }

class C152 { C152(); public: C152(int) { } };
ret  const volatile & operator+(C152 volatile, C152) { return ret_val; }

class C153 { C153(); public: C153(int) { } };
void operator+(C153 volatile, C153 const) { }

class C154 { C154(); public: C154(int) { } };
ret  operator+(C154 volatile, C154 const) { return ret_val; }

class C155 { C155(); public: C155(int) { } };
ret  const operator+(C155 volatile, C155 const) { return ret_val; }

class C156 { C156(); public: C156(int) { } };
ret  volatile operator+(C156 volatile, C156 const) { return ret_val; }

class C157 { C157(); public: C157(int) { } };
ret  const volatile operator+(C157 volatile, C157 const) { return ret_val; }

class C158 { C158(); public: C158(int) { } };
ret  & operator+(C158 volatile, C158 const) { return ret_val; }

class C159 { C159(); public: C159(int) { } };
ret  const & operator+(C159 volatile, C159 const) { return ret_val; }

class C160 { C160(); public: C160(int) { } };
ret  volatile & operator+(C160 volatile, C160 const) { return ret_val; }

class C161 { C161(); public: C161(int) { } };
ret  const volatile & operator+(C161 volatile, C161 const) { return ret_val; }

class C162 { C162(); public: C162(int) { } };
void operator+(C162 volatile, C162 volatile) { }

class C163 { C163(); public: C163(int) { } };
ret  operator+(C163 volatile, C163 volatile) { return ret_val; }

class C164 { C164(); public: C164(int) { } };
ret  const operator+(C164 volatile, C164 volatile) { return ret_val; }

class C165 { C165(); public: C165(int) { } };
ret  volatile operator+(C165 volatile, C165 volatile) { return ret_val; }

class C166 { C166(); public: C166(int) { } };
ret  const volatile operator+(C166 volatile, C166 volatile) { return ret_val; }

class C167 { C167(); public: C167(int) { } };
ret  & operator+(C167 volatile, C167 volatile) { return ret_val; }

class C168 { C168(); public: C168(int) { } };
ret  const & operator+(C168 volatile, C168 volatile) { return ret_val; }

class C169 { C169(); public: C169(int) { } };
ret  volatile & operator+(C169 volatile, C169 volatile) { return ret_val; }

class C170 { C170(); public: C170(int) { } };
ret  const volatile & operator+(C170 volatile, C170 volatile) { return ret_val; }

class C171 { C171(); public: C171(int) { } };
void operator+(C171 volatile, C171 const volatile) { }

class C172 { C172(); public: C172(int) { } };
ret  operator+(C172 volatile, C172 const volatile) { return ret_val; }

class C173 { C173(); public: C173(int) { } };
ret  const operator+(C173 volatile, C173 const volatile) { return ret_val; }

class C174 { C174(); public: C174(int) { } };
ret  volatile operator+(C174 volatile, C174 const volatile) { return ret_val; }

class C175 { C175(); public: C175(int) { } };
ret  const volatile operator+(C175 volatile, C175 const volatile) { return ret_val; }

class C176 { C176(); public: C176(int) { } };
ret  & operator+(C176 volatile, C176 const volatile) { return ret_val; }

class C177 { C177(); public: C177(int) { } };
ret  const & operator+(C177 volatile, C177 const volatile) { return ret_val; }

class C178 { C178(); public: C178(int) { } };
ret  volatile & operator+(C178 volatile, C178 const volatile) { return ret_val; }

class C179 { C179(); public: C179(int) { } };
ret  const volatile & operator+(C179 volatile, C179 const volatile) { return ret_val; }

class C180 { C180(); public: C180(int) { } };
void operator+(C180 volatile, C180 &) { }

class C181 { C181(); public: C181(int) { } };
ret  operator+(C181 volatile, C181 &) { return ret_val; }

class C182 { C182(); public: C182(int) { } };
ret  const operator+(C182 volatile, C182 &) { return ret_val; }

class C183 { C183(); public: C183(int) { } };
ret  volatile operator+(C183 volatile, C183 &) { return ret_val; }

class C184 { C184(); public: C184(int) { } };
ret  const volatile operator+(C184 volatile, C184 &) { return ret_val; }

class C185 { C185(); public: C185(int) { } };
ret  & operator+(C185 volatile, C185 &) { return ret_val; }

class C186 { C186(); public: C186(int) { } };
ret  const & operator+(C186 volatile, C186 &) { return ret_val; }

class C187 { C187(); public: C187(int) { } };
ret  volatile & operator+(C187 volatile, C187 &) { return ret_val; }

class C188 { C188(); public: C188(int) { } };
ret  const volatile & operator+(C188 volatile, C188 &) { return ret_val; }

class C189 { C189(); public: C189(int) { } };
void operator+(C189 volatile, C189 const &) { }

class C190 { C190(); public: C190(int) { } };
ret  operator+(C190 volatile, C190 const &) { return ret_val; }

class C191 { C191(); public: C191(int) { } };
ret  const operator+(C191 volatile, C191 const &) { return ret_val; }

class C192 { C192(); public: C192(int) { } };
ret  volatile operator+(C192 volatile, C192 const &) { return ret_val; }

class C193 { C193(); public: C193(int) { } };
ret  const volatile operator+(C193 volatile, C193 const &) { return ret_val; }

class C194 { C194(); public: C194(int) { } };
ret  & operator+(C194 volatile, C194 const &) { return ret_val; }

class C195 { C195(); public: C195(int) { } };
ret  const & operator+(C195 volatile, C195 const &) { return ret_val; }

class C196 { C196(); public: C196(int) { } };
ret  volatile & operator+(C196 volatile, C196 const &) { return ret_val; }

class C197 { C197(); public: C197(int) { } };
ret  const volatile & operator+(C197 volatile, C197 const &) { return ret_val; }

class C198 { C198(); public: C198(int) { } };
void operator+(C198 volatile, C198 volatile &) { }

class C199 { C199(); public: C199(int) { } };
ret  operator+(C199 volatile, C199 volatile &) { return ret_val; }

class C200 { C200(); public: C200(int) { } };
ret  const operator+(C200 volatile, C200 volatile &) { return ret_val; }

class C201 { C201(); public: C201(int) { } };
ret  volatile operator+(C201 volatile, C201 volatile &) { return ret_val; }

class C202 { C202(); public: C202(int) { } };
ret  const volatile operator+(C202 volatile, C202 volatile &) { return ret_val; }

class C203 { C203(); public: C203(int) { } };
ret  & operator+(C203 volatile, C203 volatile &) { return ret_val; }

class C204 { C204(); public: C204(int) { } };
ret  const & operator+(C204 volatile, C204 volatile &) { return ret_val; }

class C205 { C205(); public: C205(int) { } };
ret  volatile & operator+(C205 volatile, C205 volatile &) { return ret_val; }

class C206 { C206(); public: C206(int) { } };
ret  const volatile & operator+(C206 volatile, C206 volatile &) { return ret_val; }

class C207 { C207(); public: C207(int) { } };
void operator+(C207 volatile, C207 const volatile &) { }

class C208 { C208(); public: C208(int) { } };
ret  operator+(C208 volatile, C208 const volatile &) { return ret_val; }

class C209 { C209(); public: C209(int) { } };
ret  const operator+(C209 volatile, C209 const volatile &) { return ret_val; }

class C210 { C210(); public: C210(int) { } };
ret  volatile operator+(C210 volatile, C210 const volatile &) { return ret_val; }

class C211 { C211(); public: C211(int) { } };
ret  const volatile operator+(C211 volatile, C211 const volatile &) { return ret_val; }

class C212 { C212(); public: C212(int) { } };
ret  & operator+(C212 volatile, C212 const volatile &) { return ret_val; }

class C213 { C213(); public: C213(int) { } };
ret  const & operator+(C213 volatile, C213 const volatile &) { return ret_val; }

class C214 { C214(); public: C214(int) { } };
ret  volatile & operator+(C214 volatile, C214 const volatile &) { return ret_val; }

class C215 { C215(); public: C215(int) { } };
ret  const volatile & operator+(C215 volatile, C215 const volatile &) { return ret_val; }

class C216 { C216(); public: C216(int) { } };
void operator+(C216 const volatile, C216) { }

class C217 { C217(); public: C217(int) { } };
ret  operator+(C217 const volatile, C217) { return ret_val; }

class C218 { C218(); public: C218(int) { } };
ret  const operator+(C218 const volatile, C218) { return ret_val; }

class C219 { C219(); public: C219(int) { } };
ret  volatile operator+(C219 const volatile, C219) { return ret_val; }

class C220 { C220(); public: C220(int) { } };
ret  const volatile operator+(C220 const volatile, C220) { return ret_val; }

class C221 { C221(); public: C221(int) { } };
ret  & operator+(C221 const volatile, C221) { return ret_val; }

class C222 { C222(); public: C222(int) { } };
ret  const & operator+(C222 const volatile, C222) { return ret_val; }

class C223 { C223(); public: C223(int) { } };
ret  volatile & operator+(C223 const volatile, C223) { return ret_val; }

class C224 { C224(); public: C224(int) { } };
ret  const volatile & operator+(C224 const volatile, C224) { return ret_val; }

class C225 { C225(); public: C225(int) { } };
void operator+(C225 const volatile, C225 const) { }

class C226 { C226(); public: C226(int) { } };
ret  operator+(C226 const volatile, C226 const) { return ret_val; }

class C227 { C227(); public: C227(int) { } };
ret  const operator+(C227 const volatile, C227 const) { return ret_val; }

class C228 { C228(); public: C228(int) { } };
ret  volatile operator+(C228 const volatile, C228 const) { return ret_val; }

class C229 { C229(); public: C229(int) { } };
ret  const volatile operator+(C229 const volatile, C229 const) { return ret_val; }

class C230 { C230(); public: C230(int) { } };
ret  & operator+(C230 const volatile, C230 const) { return ret_val; }

class C231 { C231(); public: C231(int) { } };
ret  const & operator+(C231 const volatile, C231 const) { return ret_val; }

class C232 { C232(); public: C232(int) { } };
ret  volatile & operator+(C232 const volatile, C232 const) { return ret_val; }

class C233 { C233(); public: C233(int) { } };
ret  const volatile & operator+(C233 const volatile, C233 const) { return ret_val; }

class C234 { C234(); public: C234(int) { } };
void operator+(C234 const volatile, C234 volatile) { }

class C235 { C235(); public: C235(int) { } };
ret  operator+(C235 const volatile, C235 volatile) { return ret_val; }

class C236 { C236(); public: C236(int) { } };
ret  const operator+(C236 const volatile, C236 volatile) { return ret_val; }

class C237 { C237(); public: C237(int) { } };
ret  volatile operator+(C237 const volatile, C237 volatile) { return ret_val; }

class C238 { C238(); public: C238(int) { } };
ret  const volatile operator+(C238 const volatile, C238 volatile) { return ret_val; }

class C239 { C239(); public: C239(int) { } };
ret  & operator+(C239 const volatile, C239 volatile) { return ret_val; }

class C240 { C240(); public: C240(int) { } };
ret  const & operator+(C240 const volatile, C240 volatile) { return ret_val; }

class C241 { C241(); public: C241(int) { } };
ret  volatile & operator+(C241 const volatile, C241 volatile) { return ret_val; }

class C242 { C242(); public: C242(int) { } };
ret  const volatile & operator+(C242 const volatile, C242 volatile) { return ret_val; }

class C243 { C243(); public: C243(int) { } };
void operator+(C243 const volatile, C243 const volatile) { }

class C244 { C244(); public: C244(int) { } };
ret  operator+(C244 const volatile, C244 const volatile) { return ret_val; }

class C245 { C245(); public: C245(int) { } };
ret  const operator+(C245 const volatile, C245 const volatile) { return ret_val; }

class C246 { C246(); public: C246(int) { } };
ret  volatile operator+(C246 const volatile, C246 const volatile) { return ret_val; }

class C247 { C247(); public: C247(int) { } };
ret  const volatile operator+(C247 const volatile, C247 const volatile) { return ret_val; }

class C248 { C248(); public: C248(int) { } };
ret  & operator+(C248 const volatile, C248 const volatile) { return ret_val; }

class C249 { C249(); public: C249(int) { } };
ret  const & operator+(C249 const volatile, C249 const volatile) { return ret_val; }

class C250 { C250(); public: C250(int) { } };
ret  volatile & operator+(C250 const volatile, C250 const volatile) { return ret_val; }

class C251 { C251(); public: C251(int) { } };
ret  const volatile & operator+(C251 const volatile, C251 const volatile) { return ret_val; }

class C252 { C252(); public: C252(int) { } };
void operator+(C252 const volatile, C252 &) { }

class C253 { C253(); public: C253(int) { } };
ret  operator+(C253 const volatile, C253 &) { return ret_val; }

class C254 { C254(); public: C254(int) { } };
ret  const operator+(C254 const volatile, C254 &) { return ret_val; }

class C255 { C255(); public: C255(int) { } };
ret  volatile operator+(C255 const volatile, C255 &) { return ret_val; }

class C256 { C256(); public: C256(int) { } };
ret  const volatile operator+(C256 const volatile, C256 &) { return ret_val; }

class C257 { C257(); public: C257(int) { } };
ret  & operator+(C257 const volatile, C257 &) { return ret_val; }

class C258 { C258(); public: C258(int) { } };
ret  const & operator+(C258 const volatile, C258 &) { return ret_val; }

class C259 { C259(); public: C259(int) { } };
ret  volatile & operator+(C259 const volatile, C259 &) { return ret_val; }

class C260 { C260(); public: C260(int) { } };
ret  const volatile & operator+(C260 const volatile, C260 &) { return ret_val; }

class C261 { C261(); public: C261(int) { } };
void operator+(C261 const volatile, C261 const &) { }

class C262 { C262(); public: C262(int) { } };
ret  operator+(C262 const volatile, C262 const &) { return ret_val; }

class C263 { C263(); public: C263(int) { } };
ret  const operator+(C263 const volatile, C263 const &) { return ret_val; }

class C264 { C264(); public: C264(int) { } };
ret  volatile operator+(C264 const volatile, C264 const &) { return ret_val; }

class C265 { C265(); public: C265(int) { } };
ret  const volatile operator+(C265 const volatile, C265 const &) { return ret_val; }

class C266 { C266(); public: C266(int) { } };
ret  & operator+(C266 const volatile, C266 const &) { return ret_val; }

class C267 { C267(); public: C267(int) { } };
ret  const & operator+(C267 const volatile, C267 const &) { return ret_val; }

class C268 { C268(); public: C268(int) { } };
ret  volatile & operator+(C268 const volatile, C268 const &) { return ret_val; }

class C269 { C269(); public: C269(int) { } };
ret  const volatile & operator+(C269 const volatile, C269 const &) { return ret_val; }

class C270 { C270(); public: C270(int) { } };
void operator+(C270 const volatile, C270 volatile &) { }

class C271 { C271(); public: C271(int) { } };
ret  operator+(C271 const volatile, C271 volatile &) { return ret_val; }

class C272 { C272(); public: C272(int) { } };
ret  const operator+(C272 const volatile, C272 volatile &) { return ret_val; }

class C273 { C273(); public: C273(int) { } };
ret  volatile operator+(C273 const volatile, C273 volatile &) { return ret_val; }

class C274 { C274(); public: C274(int) { } };
ret  const volatile operator+(C274 const volatile, C274 volatile &) { return ret_val; }

class C275 { C275(); public: C275(int) { } };
ret  & operator+(C275 const volatile, C275 volatile &) { return ret_val; }

class C276 { C276(); public: C276(int) { } };
ret  const & operator+(C276 const volatile, C276 volatile &) { return ret_val; }

class C277 { C277(); public: C277(int) { } };
ret  volatile & operator+(C277 const volatile, C277 volatile &) { return ret_val; }

class C278 { C278(); public: C278(int) { } };
ret  const volatile & operator+(C278 const volatile, C278 volatile &) { return ret_val; }

class C279 { C279(); public: C279(int) { } };
void operator+(C279 const volatile, C279 const volatile &) { }

class C280 { C280(); public: C280(int) { } };
ret  operator+(C280 const volatile, C280 const volatile &) { return ret_val; }

class C281 { C281(); public: C281(int) { } };
ret  const operator+(C281 const volatile, C281 const volatile &) { return ret_val; }

class C282 { C282(); public: C282(int) { } };
ret  volatile operator+(C282 const volatile, C282 const volatile &) { return ret_val; }

class C283 { C283(); public: C283(int) { } };
ret  const volatile operator+(C283 const volatile, C283 const volatile &) { return ret_val; }

class C284 { C284(); public: C284(int) { } };
ret  & operator+(C284 const volatile, C284 const volatile &) { return ret_val; }

class C285 { C285(); public: C285(int) { } };
ret  const & operator+(C285 const volatile, C285 const volatile &) { return ret_val; }

class C286 { C286(); public: C286(int) { } };
ret  volatile & operator+(C286 const volatile, C286 const volatile &) { return ret_val; }

class C287 { C287(); public: C287(int) { } };
ret  const volatile & operator+(C287 const volatile, C287 const volatile &) { return ret_val; }

class C288 { C288(); public: C288(int) { } };
void operator+(C288 &, C288) { }

class C289 { C289(); public: C289(int) { } };
ret  operator+(C289 &, C289) { return ret_val; }

class C290 { C290(); public: C290(int) { } };
ret  const operator+(C290 &, C290) { return ret_val; }

class C291 { C291(); public: C291(int) { } };
ret  volatile operator+(C291 &, C291) { return ret_val; }

class C292 { C292(); public: C292(int) { } };
ret  const volatile operator+(C292 &, C292) { return ret_val; }

class C293 { C293(); public: C293(int) { } };
ret  & operator+(C293 &, C293) { return ret_val; }

class C294 { C294(); public: C294(int) { } };
ret  const & operator+(C294 &, C294) { return ret_val; }

class C295 { C295(); public: C295(int) { } };
ret  volatile & operator+(C295 &, C295) { return ret_val; }

class C296 { C296(); public: C296(int) { } };
ret  const volatile & operator+(C296 &, C296) { return ret_val; }

class C297 { C297(); public: C297(int) { } };
void operator+(C297 &, C297 const) { }

class C298 { C298(); public: C298(int) { } };
ret  operator+(C298 &, C298 const) { return ret_val; }

class C299 { C299(); public: C299(int) { } };
ret  const operator+(C299 &, C299 const) { return ret_val; }

class C300 { C300(); public: C300(int) { } };
ret  volatile operator+(C300 &, C300 const) { return ret_val; }

class C301 { C301(); public: C301(int) { } };
ret  const volatile operator+(C301 &, C301 const) { return ret_val; }

class C302 { C302(); public: C302(int) { } };
ret  & operator+(C302 &, C302 const) { return ret_val; }

class C303 { C303(); public: C303(int) { } };
ret  const & operator+(C303 &, C303 const) { return ret_val; }

class C304 { C304(); public: C304(int) { } };
ret  volatile & operator+(C304 &, C304 const) { return ret_val; }

class C305 { C305(); public: C305(int) { } };
ret  const volatile & operator+(C305 &, C305 const) { return ret_val; }

class C306 { C306(); public: C306(int) { } };
void operator+(C306 &, C306 volatile) { }

class C307 { C307(); public: C307(int) { } };
ret  operator+(C307 &, C307 volatile) { return ret_val; }

class C308 { C308(); public: C308(int) { } };
ret  const operator+(C308 &, C308 volatile) { return ret_val; }

class C309 { C309(); public: C309(int) { } };
ret  volatile operator+(C309 &, C309 volatile) { return ret_val; }

class C310 { C310(); public: C310(int) { } };
ret  const volatile operator+(C310 &, C310 volatile) { return ret_val; }

class C311 { C311(); public: C311(int) { } };
ret  & operator+(C311 &, C311 volatile) { return ret_val; }

class C312 { C312(); public: C312(int) { } };
ret  const & operator+(C312 &, C312 volatile) { return ret_val; }

class C313 { C313(); public: C313(int) { } };
ret  volatile & operator+(C313 &, C313 volatile) { return ret_val; }

class C314 { C314(); public: C314(int) { } };
ret  const volatile & operator+(C314 &, C314 volatile) { return ret_val; }

class C315 { C315(); public: C315(int) { } };
void operator+(C315 &, C315 const volatile) { }

class C316 { C316(); public: C316(int) { } };
ret  operator+(C316 &, C316 const volatile) { return ret_val; }

class C317 { C317(); public: C317(int) { } };
ret  const operator+(C317 &, C317 const volatile) { return ret_val; }

class C318 { C318(); public: C318(int) { } };
ret  volatile operator+(C318 &, C318 const volatile) { return ret_val; }

class C319 { C319(); public: C319(int) { } };
ret  const volatile operator+(C319 &, C319 const volatile) { return ret_val; }

class C320 { C320(); public: C320(int) { } };
ret  & operator+(C320 &, C320 const volatile) { return ret_val; }

class C321 { C321(); public: C321(int) { } };
ret  const & operator+(C321 &, C321 const volatile) { return ret_val; }

class C322 { C322(); public: C322(int) { } };
ret  volatile & operator+(C322 &, C322 const volatile) { return ret_val; }

class C323 { C323(); public: C323(int) { } };
ret  const volatile & operator+(C323 &, C323 const volatile) { return ret_val; }

class C324 { C324(); public: C324(int) { } };
void operator+(C324 &, C324 &) { }

class C325 { C325(); public: C325(int) { } };
ret  operator+(C325 &, C325 &) { return ret_val; }

class C326 { C326(); public: C326(int) { } };
ret  const operator+(C326 &, C326 &) { return ret_val; }

class C327 { C327(); public: C327(int) { } };
ret  volatile operator+(C327 &, C327 &) { return ret_val; }

class C328 { C328(); public: C328(int) { } };
ret  const volatile operator+(C328 &, C328 &) { return ret_val; }

class C329 { C329(); public: C329(int) { } };
ret  & operator+(C329 &, C329 &) { return ret_val; }

class C330 { C330(); public: C330(int) { } };
ret  const & operator+(C330 &, C330 &) { return ret_val; }

class C331 { C331(); public: C331(int) { } };
ret  volatile & operator+(C331 &, C331 &) { return ret_val; }

class C332 { C332(); public: C332(int) { } };
ret  const volatile & operator+(C332 &, C332 &) { return ret_val; }

class C333 { C333(); public: C333(int) { } };
void operator+(C333 &, C333 const &) { }

class C334 { C334(); public: C334(int) { } };
ret  operator+(C334 &, C334 const &) { return ret_val; }

class C335 { C335(); public: C335(int) { } };
ret  const operator+(C335 &, C335 const &) { return ret_val; }

class C336 { C336(); public: C336(int) { } };
ret  volatile operator+(C336 &, C336 const &) { return ret_val; }

class C337 { C337(); public: C337(int) { } };
ret  const volatile operator+(C337 &, C337 const &) { return ret_val; }

class C338 { C338(); public: C338(int) { } };
ret  & operator+(C338 &, C338 const &) { return ret_val; }

class C339 { C339(); public: C339(int) { } };
ret  const & operator+(C339 &, C339 const &) { return ret_val; }

class C340 { C340(); public: C340(int) { } };
ret  volatile & operator+(C340 &, C340 const &) { return ret_val; }

class C341 { C341(); public: C341(int) { } };
ret  const volatile & operator+(C341 &, C341 const &) { return ret_val; }

class C342 { C342(); public: C342(int) { } };
void operator+(C342 &, C342 volatile &) { }

class C343 { C343(); public: C343(int) { } };
ret  operator+(C343 &, C343 volatile &) { return ret_val; }

class C344 { C344(); public: C344(int) { } };
ret  const operator+(C344 &, C344 volatile &) { return ret_val; }

class C345 { C345(); public: C345(int) { } };
ret  volatile operator+(C345 &, C345 volatile &) { return ret_val; }

class C346 { C346(); public: C346(int) { } };
ret  const volatile operator+(C346 &, C346 volatile &) { return ret_val; }

class C347 { C347(); public: C347(int) { } };
ret  & operator+(C347 &, C347 volatile &) { return ret_val; }

class C348 { C348(); public: C348(int) { } };
ret  const & operator+(C348 &, C348 volatile &) { return ret_val; }

class C349 { C349(); public: C349(int) { } };
ret  volatile & operator+(C349 &, C349 volatile &) { return ret_val; }

class C350 { C350(); public: C350(int) { } };
ret  const volatile & operator+(C350 &, C350 volatile &) { return ret_val; }

class C351 { C351(); public: C351(int) { } };
void operator+(C351 &, C351 const volatile &) { }

class C352 { C352(); public: C352(int) { } };
ret  operator+(C352 &, C352 const volatile &) { return ret_val; }

class C353 { C353(); public: C353(int) { } };
ret  const operator+(C353 &, C353 const volatile &) { return ret_val; }

class C354 { C354(); public: C354(int) { } };
ret  volatile operator+(C354 &, C354 const volatile &) { return ret_val; }

class C355 { C355(); public: C355(int) { } };
ret  const volatile operator+(C355 &, C355 const volatile &) { return ret_val; }

class C356 { C356(); public: C356(int) { } };
ret  & operator+(C356 &, C356 const volatile &) { return ret_val; }

class C357 { C357(); public: C357(int) { } };
ret  const & operator+(C357 &, C357 const volatile &) { return ret_val; }

class C358 { C358(); public: C358(int) { } };
ret  volatile & operator+(C358 &, C358 const volatile &) { return ret_val; }

class C359 { C359(); public: C359(int) { } };
ret  const volatile & operator+(C359 &, C359 const volatile &) { return ret_val; }

class C360 { C360(); public: C360(int) { } };
void operator+(C360 const &, C360) { }

class C361 { C361(); public: C361(int) { } };
ret  operator+(C361 const &, C361) { return ret_val; }

class C362 { C362(); public: C362(int) { } };
ret  const operator+(C362 const &, C362) { return ret_val; }

class C363 { C363(); public: C363(int) { } };
ret  volatile operator+(C363 const &, C363) { return ret_val; }

class C364 { C364(); public: C364(int) { } };
ret  const volatile operator+(C364 const &, C364) { return ret_val; }

class C365 { C365(); public: C365(int) { } };
ret  & operator+(C365 const &, C365) { return ret_val; }

class C366 { C366(); public: C366(int) { } };
ret  const & operator+(C366 const &, C366) { return ret_val; }

class C367 { C367(); public: C367(int) { } };
ret  volatile & operator+(C367 const &, C367) { return ret_val; }

class C368 { C368(); public: C368(int) { } };
ret  const volatile & operator+(C368 const &, C368) { return ret_val; }

class C369 { C369(); public: C369(int) { } };
void operator+(C369 const &, C369 const) { }

class C370 { C370(); public: C370(int) { } };
ret  operator+(C370 const &, C370 const) { return ret_val; }

class C371 { C371(); public: C371(int) { } };
ret  const operator+(C371 const &, C371 const) { return ret_val; }

class C372 { C372(); public: C372(int) { } };
ret  volatile operator+(C372 const &, C372 const) { return ret_val; }

class C373 { C373(); public: C373(int) { } };
ret  const volatile operator+(C373 const &, C373 const) { return ret_val; }

class C374 { C374(); public: C374(int) { } };
ret  & operator+(C374 const &, C374 const) { return ret_val; }

class C375 { C375(); public: C375(int) { } };
ret  const & operator+(C375 const &, C375 const) { return ret_val; }

class C376 { C376(); public: C376(int) { } };
ret  volatile & operator+(C376 const &, C376 const) { return ret_val; }

class C377 { C377(); public: C377(int) { } };
ret  const volatile & operator+(C377 const &, C377 const) { return ret_val; }

class C378 { C378(); public: C378(int) { } };
void operator+(C378 const &, C378 volatile) { }

class C379 { C379(); public: C379(int) { } };
ret  operator+(C379 const &, C379 volatile) { return ret_val; }

class C380 { C380(); public: C380(int) { } };
ret  const operator+(C380 const &, C380 volatile) { return ret_val; }

class C381 { C381(); public: C381(int) { } };
ret  volatile operator+(C381 const &, C381 volatile) { return ret_val; }

class C382 { C382(); public: C382(int) { } };
ret  const volatile operator+(C382 const &, C382 volatile) { return ret_val; }

class C383 { C383(); public: C383(int) { } };
ret  & operator+(C383 const &, C383 volatile) { return ret_val; }

class C384 { C384(); public: C384(int) { } };
ret  const & operator+(C384 const &, C384 volatile) { return ret_val; }

class C385 { C385(); public: C385(int) { } };
ret  volatile & operator+(C385 const &, C385 volatile) { return ret_val; }

class C386 { C386(); public: C386(int) { } };
ret  const volatile & operator+(C386 const &, C386 volatile) { return ret_val; }

class C387 { C387(); public: C387(int) { } };
void operator+(C387 const &, C387 const volatile) { }

class C388 { C388(); public: C388(int) { } };
ret  operator+(C388 const &, C388 const volatile) { return ret_val; }

class C389 { C389(); public: C389(int) { } };
ret  const operator+(C389 const &, C389 const volatile) { return ret_val; }

class C390 { C390(); public: C390(int) { } };
ret  volatile operator+(C390 const &, C390 const volatile) { return ret_val; }

class C391 { C391(); public: C391(int) { } };
ret  const volatile operator+(C391 const &, C391 const volatile) { return ret_val; }

class C392 { C392(); public: C392(int) { } };
ret  & operator+(C392 const &, C392 const volatile) { return ret_val; }

class C393 { C393(); public: C393(int) { } };
ret  const & operator+(C393 const &, C393 const volatile) { return ret_val; }

class C394 { C394(); public: C394(int) { } };
ret  volatile & operator+(C394 const &, C394 const volatile) { return ret_val; }

class C395 { C395(); public: C395(int) { } };
ret  const volatile & operator+(C395 const &, C395 const volatile) { return ret_val; }

class C396 { C396(); public: C396(int) { } };
void operator+(C396 const &, C396 &) { }

class C397 { C397(); public: C397(int) { } };
ret  operator+(C397 const &, C397 &) { return ret_val; }

class C398 { C398(); public: C398(int) { } };
ret  const operator+(C398 const &, C398 &) { return ret_val; }

class C399 { C399(); public: C399(int) { } };
ret  volatile operator+(C399 const &, C399 &) { return ret_val; }

class C400 { C400(); public: C400(int) { } };
ret  const volatile operator+(C400 const &, C400 &) { return ret_val; }

class C401 { C401(); public: C401(int) { } };
ret  & operator+(C401 const &, C401 &) { return ret_val; }

class C402 { C402(); public: C402(int) { } };
ret  const & operator+(C402 const &, C402 &) { return ret_val; }

class C403 { C403(); public: C403(int) { } };
ret  volatile & operator+(C403 const &, C403 &) { return ret_val; }

class C404 { C404(); public: C404(int) { } };
ret  const volatile & operator+(C404 const &, C404 &) { return ret_val; }

class C405 { C405(); public: C405(int) { } };
void operator+(C405 const &, C405 const &) { }

class C406 { C406(); public: C406(int) { } };
ret  operator+(C406 const &, C406 const &) { return ret_val; }

class C407 { C407(); public: C407(int) { } };
ret  const operator+(C407 const &, C407 const &) { return ret_val; }

class C408 { C408(); public: C408(int) { } };
ret  volatile operator+(C408 const &, C408 const &) { return ret_val; }

class C409 { C409(); public: C409(int) { } };
ret  const volatile operator+(C409 const &, C409 const &) { return ret_val; }

class C410 { C410(); public: C410(int) { } };
ret  & operator+(C410 const &, C410 const &) { return ret_val; }

class C411 { C411(); public: C411(int) { } };
ret  const & operator+(C411 const &, C411 const &) { return ret_val; }

class C412 { C412(); public: C412(int) { } };
ret  volatile & operator+(C412 const &, C412 const &) { return ret_val; }

class C413 { C413(); public: C413(int) { } };
ret  const volatile & operator+(C413 const &, C413 const &) { return ret_val; }

class C414 { C414(); public: C414(int) { } };
void operator+(C414 const &, C414 volatile &) { }

class C415 { C415(); public: C415(int) { } };
ret  operator+(C415 const &, C415 volatile &) { return ret_val; }

class C416 { C416(); public: C416(int) { } };
ret  const operator+(C416 const &, C416 volatile &) { return ret_val; }

class C417 { C417(); public: C417(int) { } };
ret  volatile operator+(C417 const &, C417 volatile &) { return ret_val; }

class C418 { C418(); public: C418(int) { } };
ret  const volatile operator+(C418 const &, C418 volatile &) { return ret_val; }

class C419 { C419(); public: C419(int) { } };
ret  & operator+(C419 const &, C419 volatile &) { return ret_val; }

class C420 { C420(); public: C420(int) { } };
ret  const & operator+(C420 const &, C420 volatile &) { return ret_val; }

class C421 { C421(); public: C421(int) { } };
ret  volatile & operator+(C421 const &, C421 volatile &) { return ret_val; }

class C422 { C422(); public: C422(int) { } };
ret  const volatile & operator+(C422 const &, C422 volatile &) { return ret_val; }

class C423 { C423(); public: C423(int) { } };
void operator+(C423 const &, C423 const volatile &) { }

class C424 { C424(); public: C424(int) { } };
ret  operator+(C424 const &, C424 const volatile &) { return ret_val; }

class C425 { C425(); public: C425(int) { } };
ret  const operator+(C425 const &, C425 const volatile &) { return ret_val; }

class C426 { C426(); public: C426(int) { } };
ret  volatile operator+(C426 const &, C426 const volatile &) { return ret_val; }

class C427 { C427(); public: C427(int) { } };
ret  const volatile operator+(C427 const &, C427 const volatile &) { return ret_val; }

class C428 { C428(); public: C428(int) { } };
ret  & operator+(C428 const &, C428 const volatile &) { return ret_val; }

class C429 { C429(); public: C429(int) { } };
ret  const & operator+(C429 const &, C429 const volatile &) { return ret_val; }

class C430 { C430(); public: C430(int) { } };
ret  volatile & operator+(C430 const &, C430 const volatile &) { return ret_val; }

class C431 { C431(); public: C431(int) { } };
ret  const volatile & operator+(C431 const &, C431 const volatile &) { return ret_val; }

class C432 { C432(); public: C432(int) { } };
void operator+(C432 volatile &, C432) { }

class C433 { C433(); public: C433(int) { } };
ret  operator+(C433 volatile &, C433) { return ret_val; }

class C434 { C434(); public: C434(int) { } };
ret  const operator+(C434 volatile &, C434) { return ret_val; }

class C435 { C435(); public: C435(int) { } };
ret  volatile operator+(C435 volatile &, C435) { return ret_val; }

class C436 { C436(); public: C436(int) { } };
ret  const volatile operator+(C436 volatile &, C436) { return ret_val; }

class C437 { C437(); public: C437(int) { } };
ret  & operator+(C437 volatile &, C437) { return ret_val; }

class C438 { C438(); public: C438(int) { } };
ret  const & operator+(C438 volatile &, C438) { return ret_val; }

class C439 { C439(); public: C439(int) { } };
ret  volatile & operator+(C439 volatile &, C439) { return ret_val; }

class C440 { C440(); public: C440(int) { } };
ret  const volatile & operator+(C440 volatile &, C440) { return ret_val; }

class C441 { C441(); public: C441(int) { } };
void operator+(C441 volatile &, C441 const) { }

class C442 { C442(); public: C442(int) { } };
ret  operator+(C442 volatile &, C442 const) { return ret_val; }

class C443 { C443(); public: C443(int) { } };
ret  const operator+(C443 volatile &, C443 const) { return ret_val; }

class C444 { C444(); public: C444(int) { } };
ret  volatile operator+(C444 volatile &, C444 const) { return ret_val; }

class C445 { C445(); public: C445(int) { } };
ret  const volatile operator+(C445 volatile &, C445 const) { return ret_val; }

class C446 { C446(); public: C446(int) { } };
ret  & operator+(C446 volatile &, C446 const) { return ret_val; }

class C447 { C447(); public: C447(int) { } };
ret  const & operator+(C447 volatile &, C447 const) { return ret_val; }

class C448 { C448(); public: C448(int) { } };
ret  volatile & operator+(C448 volatile &, C448 const) { return ret_val; }

class C449 { C449(); public: C449(int) { } };
ret  const volatile & operator+(C449 volatile &, C449 const) { return ret_val; }

class C450 { C450(); public: C450(int) { } };
void operator+(C450 volatile &, C450 volatile) { }

class C451 { C451(); public: C451(int) { } };
ret  operator+(C451 volatile &, C451 volatile) { return ret_val; }

class C452 { C452(); public: C452(int) { } };
ret  const operator+(C452 volatile &, C452 volatile) { return ret_val; }

class C453 { C453(); public: C453(int) { } };
ret  volatile operator+(C453 volatile &, C453 volatile) { return ret_val; }

class C454 { C454(); public: C454(int) { } };
ret  const volatile operator+(C454 volatile &, C454 volatile) { return ret_val; }

class C455 { C455(); public: C455(int) { } };
ret  & operator+(C455 volatile &, C455 volatile) { return ret_val; }

class C456 { C456(); public: C456(int) { } };
ret  const & operator+(C456 volatile &, C456 volatile) { return ret_val; }

class C457 { C457(); public: C457(int) { } };
ret  volatile & operator+(C457 volatile &, C457 volatile) { return ret_val; }

class C458 { C458(); public: C458(int) { } };
ret  const volatile & operator+(C458 volatile &, C458 volatile) { return ret_val; }

class C459 { C459(); public: C459(int) { } };
void operator+(C459 volatile &, C459 const volatile) { }

class C460 { C460(); public: C460(int) { } };
ret  operator+(C460 volatile &, C460 const volatile) { return ret_val; }

class C461 { C461(); public: C461(int) { } };
ret  const operator+(C461 volatile &, C461 const volatile) { return ret_val; }

class C462 { C462(); public: C462(int) { } };
ret  volatile operator+(C462 volatile &, C462 const volatile) { return ret_val; }

class C463 { C463(); public: C463(int) { } };
ret  const volatile operator+(C463 volatile &, C463 const volatile) { return ret_val; }

class C464 { C464(); public: C464(int) { } };
ret  & operator+(C464 volatile &, C464 const volatile) { return ret_val; }

class C465 { C465(); public: C465(int) { } };
ret  const & operator+(C465 volatile &, C465 const volatile) { return ret_val; }

class C466 { C466(); public: C466(int) { } };
ret  volatile & operator+(C466 volatile &, C466 const volatile) { return ret_val; }

class C467 { C467(); public: C467(int) { } };
ret  const volatile & operator+(C467 volatile &, C467 const volatile) { return ret_val; }

class C468 { C468(); public: C468(int) { } };
void operator+(C468 volatile &, C468 &) { }

class C469 { C469(); public: C469(int) { } };
ret  operator+(C469 volatile &, C469 &) { return ret_val; }

class C470 { C470(); public: C470(int) { } };
ret  const operator+(C470 volatile &, C470 &) { return ret_val; }

class C471 { C471(); public: C471(int) { } };
ret  volatile operator+(C471 volatile &, C471 &) { return ret_val; }

class C472 { C472(); public: C472(int) { } };
ret  const volatile operator+(C472 volatile &, C472 &) { return ret_val; }

class C473 { C473(); public: C473(int) { } };
ret  & operator+(C473 volatile &, C473 &) { return ret_val; }

class C474 { C474(); public: C474(int) { } };
ret  const & operator+(C474 volatile &, C474 &) { return ret_val; }

class C475 { C475(); public: C475(int) { } };
ret  volatile & operator+(C475 volatile &, C475 &) { return ret_val; }

class C476 { C476(); public: C476(int) { } };
ret  const volatile & operator+(C476 volatile &, C476 &) { return ret_val; }

class C477 { C477(); public: C477(int) { } };
void operator+(C477 volatile &, C477 const &) { }

class C478 { C478(); public: C478(int) { } };
ret  operator+(C478 volatile &, C478 const &) { return ret_val; }

class C479 { C479(); public: C479(int) { } };
ret  const operator+(C479 volatile &, C479 const &) { return ret_val; }

class C480 { C480(); public: C480(int) { } };
ret  volatile operator+(C480 volatile &, C480 const &) { return ret_val; }

class C481 { C481(); public: C481(int) { } };
ret  const volatile operator+(C481 volatile &, C481 const &) { return ret_val; }

class C482 { C482(); public: C482(int) { } };
ret  & operator+(C482 volatile &, C482 const &) { return ret_val; }

class C483 { C483(); public: C483(int) { } };
ret  const & operator+(C483 volatile &, C483 const &) { return ret_val; }

class C484 { C484(); public: C484(int) { } };
ret  volatile & operator+(C484 volatile &, C484 const &) { return ret_val; }

class C485 { C485(); public: C485(int) { } };
ret  const volatile & operator+(C485 volatile &, C485 const &) { return ret_val; }

class C486 { C486(); public: C486(int) { } };
void operator+(C486 volatile &, C486 volatile &) { }

class C487 { C487(); public: C487(int) { } };
ret  operator+(C487 volatile &, C487 volatile &) { return ret_val; }

class C488 { C488(); public: C488(int) { } };
ret  const operator+(C488 volatile &, C488 volatile &) { return ret_val; }

class C489 { C489(); public: C489(int) { } };
ret  volatile operator+(C489 volatile &, C489 volatile &) { return ret_val; }

class C490 { C490(); public: C490(int) { } };
ret  const volatile operator+(C490 volatile &, C490 volatile &) { return ret_val; }

class C491 { C491(); public: C491(int) { } };
ret  & operator+(C491 volatile &, C491 volatile &) { return ret_val; }

class C492 { C492(); public: C492(int) { } };
ret  const & operator+(C492 volatile &, C492 volatile &) { return ret_val; }

class C493 { C493(); public: C493(int) { } };
ret  volatile & operator+(C493 volatile &, C493 volatile &) { return ret_val; }

class C494 { C494(); public: C494(int) { } };
ret  const volatile & operator+(C494 volatile &, C494 volatile &) { return ret_val; }

class C495 { C495(); public: C495(int) { } };
void operator+(C495 volatile &, C495 const volatile &) { }

class C496 { C496(); public: C496(int) { } };
ret  operator+(C496 volatile &, C496 const volatile &) { return ret_val; }

class C497 { C497(); public: C497(int) { } };
ret  const operator+(C497 volatile &, C497 const volatile &) { return ret_val; }

class C498 { C498(); public: C498(int) { } };
ret  volatile operator+(C498 volatile &, C498 const volatile &) { return ret_val; }

class C499 { C499(); public: C499(int) { } };
ret  const volatile operator+(C499 volatile &, C499 const volatile &) { return ret_val; }

class C500 { C500(); public: C500(int) { } };
ret  & operator+(C500 volatile &, C500 const volatile &) { return ret_val; }

class C501 { C501(); public: C501(int) { } };
ret  const & operator+(C501 volatile &, C501 const volatile &) { return ret_val; }

class C502 { C502(); public: C502(int) { } };
ret  volatile & operator+(C502 volatile &, C502 const volatile &) { return ret_val; }

class C503 { C503(); public: C503(int) { } };
ret  const volatile & operator+(C503 volatile &, C503 const volatile &) { return ret_val; }

class C504 { C504(); public: C504(int) { } };
void operator+(C504 const volatile &, C504) { }

class C505 { C505(); public: C505(int) { } };
ret  operator+(C505 const volatile &, C505) { return ret_val; }

class C506 { C506(); public: C506(int) { } };
ret  const operator+(C506 const volatile &, C506) { return ret_val; }

class C507 { C507(); public: C507(int) { } };
ret  volatile operator+(C507 const volatile &, C507) { return ret_val; }

class C508 { C508(); public: C508(int) { } };
ret  const volatile operator+(C508 const volatile &, C508) { return ret_val; }

class C509 { C509(); public: C509(int) { } };
ret  & operator+(C509 const volatile &, C509) { return ret_val; }

class C510 { C510(); public: C510(int) { } };
ret  const & operator+(C510 const volatile &, C510) { return ret_val; }

class C511 { C511(); public: C511(int) { } };
ret  volatile & operator+(C511 const volatile &, C511) { return ret_val; }

class C512 { C512(); public: C512(int) { } };
ret  const volatile & operator+(C512 const volatile &, C512) { return ret_val; }

class C513 { C513(); public: C513(int) { } };
void operator+(C513 const volatile &, C513 const) { }

class C514 { C514(); public: C514(int) { } };
ret  operator+(C514 const volatile &, C514 const) { return ret_val; }

class C515 { C515(); public: C515(int) { } };
ret  const operator+(C515 const volatile &, C515 const) { return ret_val; }

class C516 { C516(); public: C516(int) { } };
ret  volatile operator+(C516 const volatile &, C516 const) { return ret_val; }

class C517 { C517(); public: C517(int) { } };
ret  const volatile operator+(C517 const volatile &, C517 const) { return ret_val; }

class C518 { C518(); public: C518(int) { } };
ret  & operator+(C518 const volatile &, C518 const) { return ret_val; }

class C519 { C519(); public: C519(int) { } };
ret  const & operator+(C519 const volatile &, C519 const) { return ret_val; }

class C520 { C520(); public: C520(int) { } };
ret  volatile & operator+(C520 const volatile &, C520 const) { return ret_val; }

class C521 { C521(); public: C521(int) { } };
ret  const volatile & operator+(C521 const volatile &, C521 const) { return ret_val; }

class C522 { C522(); public: C522(int) { } };
void operator+(C522 const volatile &, C522 volatile) { }

class C523 { C523(); public: C523(int) { } };
ret  operator+(C523 const volatile &, C523 volatile) { return ret_val; }

class C524 { C524(); public: C524(int) { } };
ret  const operator+(C524 const volatile &, C524 volatile) { return ret_val; }

class C525 { C525(); public: C525(int) { } };
ret  volatile operator+(C525 const volatile &, C525 volatile) { return ret_val; }

class C526 { C526(); public: C526(int) { } };
ret  const volatile operator+(C526 const volatile &, C526 volatile) { return ret_val; }

class C527 { C527(); public: C527(int) { } };
ret  & operator+(C527 const volatile &, C527 volatile) { return ret_val; }

class C528 { C528(); public: C528(int) { } };
ret  const & operator+(C528 const volatile &, C528 volatile) { return ret_val; }

class C529 { C529(); public: C529(int) { } };
ret  volatile & operator+(C529 const volatile &, C529 volatile) { return ret_val; }

class C530 { C530(); public: C530(int) { } };
ret  const volatile & operator+(C530 const volatile &, C530 volatile) { return ret_val; }

class C531 { C531(); public: C531(int) { } };
void operator+(C531 const volatile &, C531 const volatile) { }

class C532 { C532(); public: C532(int) { } };
ret  operator+(C532 const volatile &, C532 const volatile) { return ret_val; }

class C533 { C533(); public: C533(int) { } };
ret  const operator+(C533 const volatile &, C533 const volatile) { return ret_val; }

class C534 { C534(); public: C534(int) { } };
ret  volatile operator+(C534 const volatile &, C534 const volatile) { return ret_val; }

class C535 { C535(); public: C535(int) { } };
ret  const volatile operator+(C535 const volatile &, C535 const volatile) { return ret_val; }

class C536 { C536(); public: C536(int) { } };
ret  & operator+(C536 const volatile &, C536 const volatile) { return ret_val; }

class C537 { C537(); public: C537(int) { } };
ret  const & operator+(C537 const volatile &, C537 const volatile) { return ret_val; }

class C538 { C538(); public: C538(int) { } };
ret  volatile & operator+(C538 const volatile &, C538 const volatile) { return ret_val; }

class C539 { C539(); public: C539(int) { } };
ret  const volatile & operator+(C539 const volatile &, C539 const volatile) { return ret_val; }

class C540 { C540(); public: C540(int) { } };
void operator+(C540 const volatile &, C540 &) { }

class C541 { C541(); public: C541(int) { } };
ret  operator+(C541 const volatile &, C541 &) { return ret_val; }

class C542 { C542(); public: C542(int) { } };
ret  const operator+(C542 const volatile &, C542 &) { return ret_val; }

class C543 { C543(); public: C543(int) { } };
ret  volatile operator+(C543 const volatile &, C543 &) { return ret_val; }

class C544 { C544(); public: C544(int) { } };
ret  const volatile operator+(C544 const volatile &, C544 &) { return ret_val; }

class C545 { C545(); public: C545(int) { } };
ret  & operator+(C545 const volatile &, C545 &) { return ret_val; }

class C546 { C546(); public: C546(int) { } };
ret  const & operator+(C546 const volatile &, C546 &) { return ret_val; }

class C547 { C547(); public: C547(int) { } };
ret  volatile & operator+(C547 const volatile &, C547 &) { return ret_val; }

class C548 { C548(); public: C548(int) { } };
ret  const volatile & operator+(C548 const volatile &, C548 &) { return ret_val; }

class C549 { C549(); public: C549(int) { } };
void operator+(C549 const volatile &, C549 const &) { }

class C550 { C550(); public: C550(int) { } };
ret  operator+(C550 const volatile &, C550 const &) { return ret_val; }

class C551 { C551(); public: C551(int) { } };
ret  const operator+(C551 const volatile &, C551 const &) { return ret_val; }

class C552 { C552(); public: C552(int) { } };
ret  volatile operator+(C552 const volatile &, C552 const &) { return ret_val; }

class C553 { C553(); public: C553(int) { } };
ret  const volatile operator+(C553 const volatile &, C553 const &) { return ret_val; }

class C554 { C554(); public: C554(int) { } };
ret  & operator+(C554 const volatile &, C554 const &) { return ret_val; }

class C555 { C555(); public: C555(int) { } };
ret  const & operator+(C555 const volatile &, C555 const &) { return ret_val; }

class C556 { C556(); public: C556(int) { } };
ret  volatile & operator+(C556 const volatile &, C556 const &) { return ret_val; }

class C557 { C557(); public: C557(int) { } };
ret  const volatile & operator+(C557 const volatile &, C557 const &) { return ret_val; }

class C558 { C558(); public: C558(int) { } };
void operator+(C558 const volatile &, C558 volatile &) { }

class C559 { C559(); public: C559(int) { } };
ret  operator+(C559 const volatile &, C559 volatile &) { return ret_val; }

class C560 { C560(); public: C560(int) { } };
ret  const operator+(C560 const volatile &, C560 volatile &) { return ret_val; }

class C561 { C561(); public: C561(int) { } };
ret  volatile operator+(C561 const volatile &, C561 volatile &) { return ret_val; }

class C562 { C562(); public: C562(int) { } };
ret  const volatile operator+(C562 const volatile &, C562 volatile &) { return ret_val; }

class C563 { C563(); public: C563(int) { } };
ret  & operator+(C563 const volatile &, C563 volatile &) { return ret_val; }

class C564 { C564(); public: C564(int) { } };
ret  const & operator+(C564 const volatile &, C564 volatile &) { return ret_val; }

class C565 { C565(); public: C565(int) { } };
ret  volatile & operator+(C565 const volatile &, C565 volatile &) { return ret_val; }

class C566 { C566(); public: C566(int) { } };
ret  const volatile & operator+(C566 const volatile &, C566 volatile &) { return ret_val; }

class C567 { C567(); public: C567(int) { } };
void operator+(C567 const volatile &, C567 const volatile &) { }

class C568 { C568(); public: C568(int) { } };
ret  operator+(C568 const volatile &, C568 const volatile &) { return ret_val; }

class C569 { C569(); public: C569(int) { } };
ret  const operator+(C569 const volatile &, C569 const volatile &) { return ret_val; }

class C570 { C570(); public: C570(int) { } };
ret  volatile operator+(C570 const volatile &, C570 const volatile &) { return ret_val; }

class C571 { C571(); public: C571(int) { } };
ret  const volatile operator+(C571 const volatile &, C571 const volatile &) { return ret_val; }

class C572 { C572(); public: C572(int) { } };
ret  & operator+(C572 const volatile &, C572 const volatile &) { return ret_val; }

class C573 { C573(); public: C573(int) { } };
ret  const & operator+(C573 const volatile &, C573 const volatile &) { return ret_val; }

class C574 { C574(); public: C574(int) { } };
ret  volatile & operator+(C574 const volatile &, C574 const volatile &) { return ret_val; }

class C575 { C575(); public: C575(int) { } };
ret  const volatile & operator+(C575 const volatile &, C575 const volatile &) { return ret_val; }

