#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 2>, 100> ellint_k_data = {
      SC_(-0.99042308330535888671875e0), SC_(0.3377711175347896212115917173531827081735908096628e1), 
      SC_(-0.936324596405029296875e0), SC_(0.24799928378892127263868582279024816073017669022263e1), 
      SC_(-0.931098163127899169921875e0), SC_(0.24445366497109921574142184681825951967450363552834e1), 
      SC_(-0.928566992282867431640625e0), SC_(0.24283992440860978354078883091691713688375646750641e1), 
      SC_(-0.927107870578765869140625e0), SC_(0.24193766718674199444433297470157473721017375354321e1), 
      SC_(-0.907647669315338134765625e0), SC_(0.23150567546618192429964125282245930582572797978818e1), 
      SC_(-0.89755761623382568359375e0), SC_(0.22701438887345149877132302334749426657376446371554e1), 
      SC_(-0.805727422237396240234375e0), SC_(0.20065967275502056522161145711205880615166307631054e1), 
      SC_(-0.804910182952880859375e0), SC_(0.20049607901713509576649915223436597681197065440791e1), 
      SC_(-0.78026759624481201171875e0), SC_(0.19592100556166276984041617991618803394021101627948e1), 
      SC_(-0.775062084197998046875e0), SC_(0.19503477481367618080344709104861737619417735663137e1), 
      SC_(-0.749625742435455322265625e0), SC_(0.19104400630324162677555937926672404286027386161554e1), 
      SC_(-0.748197972774505615234375e0), SC_(0.19083524493958065222471464174677965403570194813599e1), 
      SC_(-0.746017634868621826171875e0), SC_(0.19051932837924025803305082607208317930893376343866e1), 
      SC_(-0.729037344455718994140625e0), SC_(0.18817193270349712906579552152250851402884979195389e1), 
      SC_(-0.7162187099456787109375e0), SC_(0.18652211443514110895753017730892076794376353163691e1), 
      SC_(-0.70176351070404052734375e0), SC_(0.18477488779057501703815851124765277365377561449605e1), 
      SC_(-0.684765398502349853515625e0), SC_(0.18285893795597042965531449099985329446409496660036e1), 
      SC_(-0.657618343830108642578125e0), SC_(0.18007288418337930091030357471813257042005823390856e1), 
      SC_(-0.65226137638092041015625e0), SC_(0.17955901766989373531455842967467859457919280319805e1), 
      SC_(-0.626246631145477294921875e0), SC_(0.17721381024097650892501675317068276777213166539113e1), 
      SC_(-0.62322795391082763671875e0), SC_(0.17695682119254878963375370515480337143290717650699e1), 
      SC_(-0.579573929309844970703125e0), SC_(0.17355084423179730268283500692922635797002148145716e1), 
      SC_(-0.576143443584442138671875e0), SC_(0.17330585071273208935299581419876214110701077039073e1), 
      SC_(-0.557924091815948486328125e0), SC_(0.17205458226692737403200667710314658175352937313565e1), 
      SC_(-0.4461468160152435302734375e0), SC_(0.1659143428704545241870880533420140506147522333952e1), 
      SC_(-0.442996323108673095703125e0), SC_(0.16577357283229169972381488150848899831930773096426e1), 
      SC_(-0.4059340953826904296875e0), SC_(0.16422906110493949856538834271333672987146221978421e1), 
      SC_(-0.3961667716503143310546875e0), SC_(0.16385456546730544519392193048583469346592283426711e1), 
      SC_(-0.38365900516510009765625e0), SC_(0.16339371135272803257688801436358682987064021339502e1), 
      SC_(-0.366892278194427490234375e0), SC_(0.16280775355945302002930914045811739595274403877109e1), 
      SC_(-0.3657942116260528564453125e0), SC_(0.16277061660792581984612626919271028055659130917587e1), 
      SC_(-0.2774055898189544677734375e0), SC_(0.16023984430282800438657607498499405531471989727973e1), 
      SC_(-0.236876904964447021484375e0), SC_(0.15935547153447952975660160168795914210559129958763e1), 
      SC_(-0.215539872646331787109375e0), SC_(0.1589532821488995266987321628138860611930750500013e1), 
      SC_(-0.20251691341400146484375e0), SC_(0.15872846186561139876915281752585049624892405264006e1), 
      SC_(-0.18253175914287567138671875e0), SC_(0.15841312442860499386355917744088632471227360101517e1), 
      SC_(-0.15647165477275848388671875e0), SC_(0.15805456328241604119645385355233009390341499402715e1), 
      SC_(-0.155818879604339599609375e0), SC_(0.15804633259546865645050516399359359956031398166357e1), 
      SC_(-0.12250564992427825927734375e0), SC_(0.15767400871102419514183615836102806382524229776023e1), 
      SC_(-0.108822040259838104248046875e0), SC_(0.15754779970087063716770992170704593788694293192204e1), 
      SC_(-0.84016263484954833984375e-1), SC_(0.15735793449827667079522175566532496627258499829808e1), 
      SC_(-0.5047737061977386474609375e-1), SC_(0.15717983468978644730669790777144243904901266418816e1), 
      SC_(-0.2924356050789356231689453125e-1), SC_(0.15711323191301626906143245522892930046111479139043e1), 
      SC_(-0.2485703863203525543212890625e-1), SC_(0.15710390490728134946675992373238514306032071520432e1), 
      SC_(-0.2046610601246356964111328125e-1), SC_(0.15709608520853443438268600756375428033257896081621e1), 
      SC_(-0.1881679333746433258056640625e-1), SC_(0.15709353981303437885151982749950681277334084182671e1), 
      SC_(0.73303808458149433135986328125e-2), SC_(0.15708174289149913873348609319163921760640886342517e1), 
      SC_(0.93767531216144561767578125e-1), SC_(0.15742662557451933000168330868057640508121413022011e1), 
      SC_(0.94445712864398956298828125e-1), SC_(0.15743168849992435161106319232000048329301973530501e1), 
      SC_(0.26472222805023193359375e0), SC_(0.15994564178224078022860215956833860720177728374035e1), 
      SC_(0.2795303165912628173828125e0), SC_(0.16029072336719100270539575574782330640458373122935e1), 
      SC_(0.2926295697689056396484375e0), SC_(0.16061468472007804719214570477294979509859730905091e1), 
      SC_(0.31095921993255615234375e0), SC_(0.1610983807838740712761368067192691405393131953881e1), 
      SC_(0.311484813690185546875e0), SC_(0.16111278469143224539903626000312660918785264354958e1), 
      SC_(0.3272143900394439697265625e0), SC_(0.16155797975233104967915214233565515716306532094698e1), 
      SC_(0.35747349262237548828125e0), SC_(0.16249404264362997324992641484372408858215631632002e1), 
      SC_(0.3627222478389739990234375e0), SC_(0.16266751570835125835068247288118651474745966539618e1), 
      SC_(0.3896603286266326904296875e0), SC_(0.16361224926513534404184731359873689992038364400691e1), 
      SC_(0.4120951592922210693359375e0), SC_(0.16447205720518345410219553921960387345221364427466e1), 
      SC_(0.418732583522796630859375e0), SC_(0.16473983100889105980743072684285213201961112169823e1), 
      SC_(0.451680660247802734375e0), SC_(0.16616537578199064695178688197080983284328919711812e1), 
      SC_(0.4812971055507659912109375e0), SC_(0.16759409027443021223185102539476096588592566487743e1), 
      SC_(0.486267507076263427734375e0), SC_(0.16784859353297838419400330721985671192135840854661e1), 
      SC_(0.509375751018524169921875e0), SC_(0.16909133957115563796676413045863923443879243171298e1), 
      SC_(0.515482723712921142578125e0), SC_(0.16943684153133994180565117266182132481147687142707e1), 
      SC_(0.52750241756439208984375e0), SC_(0.17013882771778724726932964262059466393678608760062e1), 
      SC_(0.531035959720611572265625e0), SC_(0.17035089955832848907265521472430416089318818612814e1), 
      SC_(0.584416687488555908203125e0), SC_(0.17390197327988114565246996689229900966075086201035e1), 
      SC_(0.587952077388763427734375e0), SC_(0.17416228307854377082064181076439075579393688159215e1), 
      SC_(0.5904018878936767578125e0), SC_(0.17434466233796448758342732941787231810206075234501e1), 
      SC_(0.5945618152618408203125e0), SC_(0.17465816562648604997075231101739061981996142156151e1), 
      SC_(0.5958592891693115234375e0), SC_(0.17475694098213745790315441260074496951659216831348e1), 
      SC_(0.59621369838714599609375e0), SC_(0.17478400476807070940242210641459121619833657136515e1), 
      SC_(0.60056293010711669921875e0), SC_(0.17511905345983286033588753251945176786720874355107e1), 
      SC_(0.6150639057159423828125e0), SC_(0.17627654245195403785778656645694141888969985612575e1), 
      SC_(0.629449188709259033203125e0), SC_(0.17748975492324966654441702323788071861970996989102e1), 
      SC_(0.6438083648681640625e0), SC_(0.17877036110717852208569994416480820233927146151031e1), 
      SC_(0.64691746234893798828125e0), SC_(0.17905734108284599293495949910396246976486037925061e1), 
      SC_(0.67001879215240478515625e0), SC_(0.18130645541261783392588745817521015225200222756122e1), 
      SC_(0.698260128498077392578125e0), SC_(0.18436823516091780453599478590327018132893271380223e1), 
      SC_(0.744858920574188232421875e0), SC_(0.1903528375308813580432291088034833377552119409638e1), 
      SC_(0.75686252117156982421875e0), SC_(0.1921257878508255118587957424341444494608703851379e1), 
      SC_(0.81158483028411865234375e0), SC_(0.20185697260071845965032147456371482208660801062775e1), 
      SC_(0.826752603054046630859375e0), SC_(0.20517631918180497335416092658794507587336758082983e1), 
      SC_(0.831471920013427734375e0), SC_(0.20628007317688811564201523487885011440347375983853e1), 
      SC_(0.841750323772430419921875e0), SC_(0.20881524894194487687984856759006121206951763482194e1), 
      SC_(0.867987096309661865234375e0), SC_(0.21626553443645542835585774105841427525784980815445e1), 
      SC_(0.90044462680816650390625e0), SC_(0.22824738454300952253057616561926426370482782713694e1), 
      SC_(0.914334356784820556640625e0), SC_(0.23479275233217975943026959240752324894340069192044e1), 
      SC_(0.915014088153839111328125e0), SC_(0.23514264944956428937368978831156074286111428698483e1), 
      SC_(0.918985307216644287109375e0), SC_(0.23725035893866734721028942202359170270846633736916e1), 
      SC_(0.9297773838043212890625e0), SC_(0.2436037207911617888011813888250469085423814242563e1), 
      SC_(0.935390174388885498046875e0), SC_(0.24734276363166698887860324191541438396367793619322e1), 
      SC_(0.937735855579376220703125e0), SC_(0.24901081139855480473523004657581668979713789812992e1), 
      SC_(0.941185891628265380859375e0), SC_(0.25159148953544058237986278218012688105174747190388e1), 
      SC_(0.962219536304473876953125e0), SC_(0.27197444631906795382660830171523843323266857162631e1), 
      SC_(0.985762655735015869140625e0), SC_(0.31847978937935282927541645440535551111318175394252e1), 
      SC_(0.988137066364288330078125e0), SC_(0.32733547952035871434783118403258690613147460127497e1), 
      SC_(0.992922723293304443359375e0), SC_(0.35258676303484017878276559590157568137293088406988e1), 
   };
#undef SC_

