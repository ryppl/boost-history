#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 2>, 33> digamma_small_data = { {
      { SC_(0.1690093176520690576580818742513656616211e-8), SC_(-591683355.0172646248558707395909205014789) }, 
      { SC_(0.2114990849122477811761200428009033203125e-8), SC_(-472815285.0570071002693788265718597340393) }, 
      { SC_(0.7099628440698779741069301962852478027344e-8), SC_(-140852442.1314070676912450420112690472767) }, 
      { SC_(0.136718796284185373224318027496337890625e-7), SC_(-73142832.95860873836229194739144163160831) }, 
      { SC_(0.1679341288252089725574478507041931152344e-7), SC_(-59547157.96538602413887273326920747232618) }, 
      { SC_(0.586768322818898013792932033538818359375e-7), SC_(-17042502.04007390142270552503694442817772) }, 
      { SC_(0.1140460881288163363933563232421875e-6), SC_(-8768385.503057815514505383587592316839882) }, 
      { SC_(0.1455586016163579188287258148193359375e-6), SC_(-6870085.813645861903628134604185829717845) }, 
      { SC_(0.38918477685001562349498271942138671875e-6), SC_(-2569474.152450422201931296992216788786447) }, 
      { SC_(0.623782625552848912775516510009765625e-6), SC_(-1603123.137920326054732670974252040029471) }, 
      { SC_(0.104669607026153244078159332275390625e-5), SC_(-955387.7506368215484908190151041836664367) }, 
      { SC_(0.2951089072666945867240428924560546875e-5), SC_(-338858.5294366592361422006155767583415332) }, 
      { SC_(0.4877083483734168112277984619140625e-5), SC_(-205041.1518328576044816818799248254570363) }, 
      { SC_(0.9066634447663091123104095458984375e-5), SC_(-110295.0867866920189113632829333848561728) }, 
      { SC_(0.2360353755648247897624969482421875e-4), SC_(-42367.10793974013680262660975431753380722) }, 
      { SC_(0.60817910707555711269378662109375e-4), SC_(-16443.10183188331524808941398664226054956) }, 
      { SC_(0.000119476739200763404369354248046875), SC_(-8370.407052088278326658500142471452559876) }, 
      { SC_(0.0002437086659483611583709716796875), SC_(-4103.836730163294582058528454940279573003) }, 
      { SC_(0.00047970912419259548187255859375), SC_(-2085.173007504666107151589993385624082421) }, 
      { SC_(0.000960788573138415813446044921875), SC_(-1041.387348674624002438653075235083900513) }, 
      { SC_(0.00113048148341476917266845703125), SC_(-885.1541983038739901476427277144444120955) }, 
      { SC_(0.0033707791008055210113525390625), SC_(-297.2390039349243537614805191705277299023) }, 
      { SC_(0.007697627879679203033447265625), SC_(-130.474775285766139271184373586979099581) }, 
      { SC_(0.0154774188995361328125), SC_(-65.1622965036480167454852163183367159499) }, 
      { SC_(0.0305807329714298248291015625), SC_(-33.22833448837103737169827250947288832931) }, 
      { SC_(0.0346831791102886199951171875), SC_(-29.35398677269140057359817914882516787732) }, 
      { SC_(0.09283597767353057861328125), SC_(-11.20575716299454988174075282116985360081) }, 
      { SC_(0.22476322948932647705078125), SC_(-4.707226986627979845318377740205580092891) }, 
      { SC_(0.4500701129436492919921875), SC_(-2.233123888225926352977853470362332872207) }, 
      { SC_(0.64851474761962890625), SC_(-1.375098345846253151137062991270963899568) }, 
      { SC_(1.14188635349273681640625), SC_(-0.3652988540135093192143191973421561168173) }, 
      { SC_(2.0095670223236083984375), SC_(0.4289360116443062870701048575669258219731) }, 
      { SC_(5.68704509735107421875), SC_(1.647702940606041030337068005600358865775) }
   } };
#undef SC_


