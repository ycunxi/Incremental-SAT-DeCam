module c432 (N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
             N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
             N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
             N99,N102,N105,N108,N112,N115,N223,N329,N370,N421,
             N430,N431,N432);

input N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
      N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
      N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
      N99,N102,N105,N108,N112,N115;//RE__PI;

input s_0,s_1;//RE__ALLOW(00,01,10,11);
input s_2,s_3;//RE__ALLOW(00,01,10,11);
input s_4,s_5;//RE__ALLOW(00,01,10,11);
input s_6,s_7;//RE__ALLOW(00,01,10,11);
input s_8,s_9;//RE__ALLOW(00,01,10,11);
input s_10,s_11;//RE__ALLOW(00,01,10,11);
output N223,N329,N370,N421,N430,N431,N432;

wire N118,N119,N122,N123,N126,N127,N130,N131,N134,N135,
     N138,N139,N142,N143,N146,N147,N150,N151,N154,N157,
     N158,N159,N162,N165,N168,N171,N174,N177,N180,N183,
     N184,N185,N186,N187,N188,N189,N190,N191,N192,N193,
     N194,N195,N196,N197,N198,N199,N203,N213,N224,N227,
     N230,N233,N236,N239,N242,N243,N246,N247,N250,N251,
     N254,N255,N256,N257,N258,N259,N260,N263,N264,N267,
     N270,N273,N276,N279,N282,N285,N288,N289,N290,N291,
     N292,N293,N294,N295,N296,N300,N301,N302,N303,N304,
     N305,N306,N307,N308,N309,N319,N330,N331,N332,N333,
     N334,N335,N336,N337,N338,N339,N340,N341,N342,N343,
     N344,N345,N346,N347,N348,N349,N350,N351,N352,N353,
     N354,N355,N356,N357,N360,N371,N372,N373,N374,N375,
     N376,N377,N378,N379,N380,N381,N386,N393,N399,N404,
     N407,N411,N414,N415,N416,N417,N418,N419,N420,N422,
     N425,N428,N429, gate85inter0, gate85inter1, gate85inter2, gate85inter3, gate85inter4, gate85inter5, gate85inter6, gate85inter7, gate85inter8, gate85inter9, gate85inter10, gate85inter11, gate85inter12, gate84inter0, gate84inter1, gate84inter2, gate84inter3, gate84inter4, gate84inter5, gate84inter6, gate84inter7, gate84inter8, gate84inter9, gate84inter10, gate84inter11, gate84inter12, gate104inter0, gate104inter1, gate104inter2, gate104inter3, gate104inter4, gate104inter5, gate104inter6, gate104inter7, gate104inter8, gate104inter9, gate104inter10, gate104inter11, gate104inter12, gate124inter0, gate124inter1, gate124inter2, gate124inter3, gate124inter4, gate124inter5, gate124inter6, gate124inter7, gate124inter8, gate124inter9, gate124inter10, gate124inter11, gate124inter12, gate40inter0, gate40inter1, gate40inter2, gate40inter3, gate40inter4, gate40inter5, gate40inter6, gate40inter7, gate40inter8, gate40inter9, gate40inter10, gate40inter11, gate40inter12, gate82inter0, gate82inter1, gate82inter2, gate82inter3, gate82inter4, gate82inter5, gate82inter6, gate82inter7, gate82inter8, gate82inter9, gate82inter10, gate82inter11, gate82inter12;



inv1 gate1( .a(N1), .O(N118) );
inv1 gate2( .a(N4), .O(N119) );
inv1 gate3( .a(N11), .O(N122) );
inv1 gate4( .a(N17), .O(N123) );
inv1 gate5( .a(N24), .O(N126) );
inv1 gate6( .a(N30), .O(N127) );
inv1 gate7( .a(N37), .O(N130) );
inv1 gate8( .a(N43), .O(N131) );
inv1 gate9( .a(N50), .O(N134) );
inv1 gate10( .a(N56), .O(N135) );
inv1 gate11( .a(N63), .O(N138) );
inv1 gate12( .a(N69), .O(N139) );
inv1 gate13( .a(N76), .O(N142) );
inv1 gate14( .a(N82), .O(N143) );
inv1 gate15( .a(N89), .O(N146) );
inv1 gate16( .a(N95), .O(N147) );
inv1 gate17( .a(N102), .O(N150) );
inv1 gate18( .a(N108), .O(N151) );
nand2 gate19( .a(N118), .b(N4), .O(N154) );
nor2 gate20( .a(N8), .b(N119), .O(N157) );
nor2 gate21( .a(N14), .b(N119), .O(N158) );
nand2 gate22( .a(N122), .b(N17), .O(N159) );
nand2 gate23( .a(N126), .b(N30), .O(N162) );
nand2 gate24( .a(N130), .b(N43), .O(N165) );
nand2 gate25( .a(N134), .b(N56), .O(N168) );
nand2 gate26( .a(N138), .b(N69), .O(N171) );
nand2 gate27( .a(N142), .b(N82), .O(N174) );
nand2 gate28( .a(N146), .b(N95), .O(N177) );
nand2 gate29( .a(N150), .b(N108), .O(N180) );
nor2 gate30( .a(N21), .b(N123), .O(N183) );
nor2 gate31( .a(N27), .b(N123), .O(N184) );
nor2 gate32( .a(N34), .b(N127), .O(N185) );
nor2 gate33( .a(N40), .b(N127), .O(N186) );
nor2 gate34( .a(N47), .b(N131), .O(N187) );
nor2 gate35( .a(N53), .b(N131), .O(N188) );
nor2 gate36( .a(N60), .b(N135), .O(N189) );
nor2 gate37( .a(N66), .b(N135), .O(N190) );
nor2 gate38( .a(N73), .b(N139), .O(N191) );
nor2 gate39( .a(N79), .b(N139), .O(N192) );

  xor2  gate217(.a(N143), .b(N86), .O(gate40inter0));
  nand2 gate218(.a(gate40inter0), .b(s_8), .O(gate40inter1));
  and2  gate219(.a(N143), .b(N86), .O(gate40inter2));
  inv1  gate220(.a(s_8), .O(gate40inter3));
  inv1  gate221(.a(s_9), .O(gate40inter4));
  nand2 gate222(.a(gate40inter4), .b(gate40inter3), .O(gate40inter5));
  nor2  gate223(.a(gate40inter5), .b(gate40inter2), .O(gate40inter6));
  inv1  gate224(.a(N86), .O(gate40inter7));
  inv1  gate225(.a(N143), .O(gate40inter8));
  nand2 gate226(.a(gate40inter8), .b(gate40inter7), .O(gate40inter9));
  nand2 gate227(.a(s_9), .b(gate40inter3), .O(gate40inter10));
  nor2  gate228(.a(gate40inter10), .b(gate40inter9), .O(gate40inter11));
  nor2  gate229(.a(gate40inter11), .b(gate40inter6), .O(gate40inter12));
  nand2 gate230(.a(gate40inter12), .b(gate40inter1), .O(N193));
nor2 gate41( .a(N92), .b(N143), .O(N194) );
nor2 gate42( .a(N99), .b(N147), .O(N195) );
nor2 gate43( .a(N105), .b(N147), .O(N196) );
nor2 gate44( .a(N112), .b(N151), .O(N197) );
nor2 gate45( .a(N115), .b(N151), .O(N198) );
and9 gate46( .a(N154), .b(N159), .c(N162), .d(N165), .e(N168), .f(N171), .g(N174), .h(N177), .i(N180), .O(N199) );
inv1 gate47( .a(N199), .O(N203) );
inv1 gate48( .a(N199), .O(N213) );
inv1 gate49( .a(N199), .O(N223) );
xor2 gate50( .a(N203), .b(N154), .O(N224) );
xor2 gate51( .a(N203), .b(N159), .O(N227) );
xor2 gate52( .a(N203), .b(N162), .O(N230) );
xor2 gate53( .a(N203), .b(N165), .O(N233) );
xor2 gate54( .a(N203), .b(N168), .O(N236) );
xor2 gate55( .a(N203), .b(N171), .O(N239) );
nand2 gate56( .a(N1), .b(N213), .O(N242) );
xor2 gate57( .a(N203), .b(N174), .O(N243) );
nand2 gate58( .a(N213), .b(N11), .O(N246) );
xor2 gate59( .a(N203), .b(N177), .O(N247) );
nand2 gate60( .a(N213), .b(N24), .O(N250) );
xor2 gate61( .a(N203), .b(N180), .O(N251) );
nand2 gate62( .a(N213), .b(N37), .O(N254) );
nand2 gate63( .a(N213), .b(N50), .O(N255) );
nand2 gate64( .a(N213), .b(N63), .O(N256) );
nand2 gate65( .a(N213), .b(N76), .O(N257) );
nand2 gate66( .a(N213), .b(N89), .O(N258) );
nand2 gate67( .a(N213), .b(N102), .O(N259) );
nand2 gate68( .a(N224), .b(N157), .O(N260) );
nand2 gate69( .a(N224), .b(N158), .O(N263) );
nand2 gate70( .a(N227), .b(N183), .O(N264) );
nand2 gate71( .a(N230), .b(N185), .O(N267) );
nand2 gate72( .a(N233), .b(N187), .O(N270) );
nand2 gate73( .a(N236), .b(N189), .O(N273) );
nand2 gate74( .a(N239), .b(N191), .O(N276) );
nand2 gate75( .a(N243), .b(N193), .O(N279) );
nand2 gate76( .a(N247), .b(N195), .O(N282) );
nand2 gate77( .a(N251), .b(N197), .O(N285) );
nand2 gate78( .a(N227), .b(N184), .O(N288) );
nand2 gate79( .a(N230), .b(N186), .O(N289) );
nand2 gate80( .a(N233), .b(N188), .O(N290) );
nand2 gate81( .a(N236), .b(N190), .O(N291) );

  xor2  gate231(.a(N192), .b(N239), .O(gate82inter0));
  nand2 gate232(.a(gate82inter0), .b(s_10), .O(gate82inter1));
  and2  gate233(.a(N192), .b(N239), .O(gate82inter2));
  inv1  gate234(.a(s_10), .O(gate82inter3));
  inv1  gate235(.a(s_11), .O(gate82inter4));
  nand2 gate236(.a(gate82inter4), .b(gate82inter3), .O(gate82inter5));
  nor2  gate237(.a(gate82inter5), .b(gate82inter2), .O(gate82inter6));
  inv1  gate238(.a(N239), .O(gate82inter7));
  inv1  gate239(.a(N192), .O(gate82inter8));
  nand2 gate240(.a(gate82inter8), .b(gate82inter7), .O(gate82inter9));
  nand2 gate241(.a(s_11), .b(gate82inter3), .O(gate82inter10));
  nor2  gate242(.a(gate82inter10), .b(gate82inter9), .O(gate82inter11));
  nor2  gate243(.a(gate82inter11), .b(gate82inter6), .O(gate82inter12));
  nand2 gate244(.a(gate82inter12), .b(gate82inter1), .O(N292));
nand2 gate83( .a(N243), .b(N194), .O(N293) );

  xor2  gate175(.a(N196), .b(N247), .O(gate84inter0));
  nand2 gate176(.a(gate84inter0), .b(s_2), .O(gate84inter1));
  and2  gate177(.a(N196), .b(N247), .O(gate84inter2));
  inv1  gate178(.a(s_2), .O(gate84inter3));
  inv1  gate179(.a(s_3), .O(gate84inter4));
  nand2 gate180(.a(gate84inter4), .b(gate84inter3), .O(gate84inter5));
  nor2  gate181(.a(gate84inter5), .b(gate84inter2), .O(gate84inter6));
  inv1  gate182(.a(N247), .O(gate84inter7));
  inv1  gate183(.a(N196), .O(gate84inter8));
  nand2 gate184(.a(gate84inter8), .b(gate84inter7), .O(gate84inter9));
  nand2 gate185(.a(s_3), .b(gate84inter3), .O(gate84inter10));
  nor2  gate186(.a(gate84inter10), .b(gate84inter9), .O(gate84inter11));
  nor2  gate187(.a(gate84inter11), .b(gate84inter6), .O(gate84inter12));
  nand2 gate188(.a(gate84inter12), .b(gate84inter1), .O(N294));

  xor2  gate161(.a(N198), .b(N251), .O(gate85inter0));
  nand2 gate162(.a(gate85inter0), .b(s_0), .O(gate85inter1));
  and2  gate163(.a(N198), .b(N251), .O(gate85inter2));
  inv1  gate164(.a(s_0), .O(gate85inter3));
  inv1  gate165(.a(s_1), .O(gate85inter4));
  nand2 gate166(.a(gate85inter4), .b(gate85inter3), .O(gate85inter5));
  nor2  gate167(.a(gate85inter5), .b(gate85inter2), .O(gate85inter6));
  inv1  gate168(.a(N251), .O(gate85inter7));
  inv1  gate169(.a(N198), .O(gate85inter8));
  nand2 gate170(.a(gate85inter8), .b(gate85inter7), .O(gate85inter9));
  nand2 gate171(.a(s_1), .b(gate85inter3), .O(gate85inter10));
  nor2  gate172(.a(gate85inter10), .b(gate85inter9), .O(gate85inter11));
  nor2  gate173(.a(gate85inter11), .b(gate85inter6), .O(gate85inter12));
  nand2 gate174(.a(gate85inter12), .b(gate85inter1), .O(N295));
and9 gate86( .a(N260), .b(N264), .c(N267), .d(N270), .e(N273), .f(N276), .g(N279), .h(N282), .i(N285), .O(N296) );
inv1 gate87( .a(N263), .O(N300) );
inv1 gate88( .a(N288), .O(N301) );
inv1 gate89( .a(N289), .O(N302) );
inv1 gate90( .a(N290), .O(N303) );
inv1 gate91( .a(N291), .O(N304) );
inv1 gate92( .a(N292), .O(N305) );
inv1 gate93( .a(N293), .O(N306) );
inv1 gate94( .a(N294), .O(N307) );
inv1 gate95( .a(N295), .O(N308) );
inv1 gate96( .a(N296), .O(N309) );
inv1 gate97( .a(N296), .O(N319) );
inv1 gate98( .a(N296), .O(N329) );
xor2 gate99( .a(N309), .b(N260), .O(N330) );
xor2 gate100( .a(N309), .b(N264), .O(N331) );
xor2 gate101( .a(N309), .b(N267), .O(N332) );
xor2 gate102( .a(N309), .b(N270), .O(N333) );
nand2 gate103( .a(N8), .b(N319), .O(N334) );

  xor2  gate189(.a(N273), .b(N309), .O(gate104inter0));
  nand2 gate190(.a(gate104inter0), .b(s_4), .O(gate104inter1));
  and2  gate191(.a(N273), .b(N309), .O(gate104inter2));
  inv1  gate192(.a(s_4), .O(gate104inter3));
  inv1  gate193(.a(s_5), .O(gate104inter4));
  nand2 gate194(.a(gate104inter4), .b(gate104inter3), .O(gate104inter5));
  nor2  gate195(.a(gate104inter5), .b(gate104inter2), .O(gate104inter6));
  inv1  gate196(.a(N309), .O(gate104inter7));
  inv1  gate197(.a(N273), .O(gate104inter8));
  nand2 gate198(.a(gate104inter8), .b(gate104inter7), .O(gate104inter9));
  nand2 gate199(.a(s_5), .b(gate104inter3), .O(gate104inter10));
  nor2  gate200(.a(gate104inter10), .b(gate104inter9), .O(gate104inter11));
  nor2  gate201(.a(gate104inter11), .b(gate104inter6), .O(gate104inter12));
  nand2 gate202(.a(gate104inter12), .b(gate104inter1), .O(N335));
nand2 gate105( .a(N319), .b(N21), .O(N336) );
xor2 gate106( .a(N309), .b(N276), .O(N337) );
nand2 gate107( .a(N319), .b(N34), .O(N338) );
xor2 gate108( .a(N309), .b(N279), .O(N339) );
nand2 gate109( .a(N319), .b(N47), .O(N340) );
xor2 gate110( .a(N309), .b(N282), .O(N341) );
nand2 gate111( .a(N319), .b(N60), .O(N342) );
xor2 gate112( .a(N309), .b(N285), .O(N343) );
nand2 gate113( .a(N319), .b(N73), .O(N344) );
nand2 gate114( .a(N319), .b(N86), .O(N345) );
nand2 gate115( .a(N319), .b(N99), .O(N346) );
nand2 gate116( .a(N319), .b(N112), .O(N347) );
nand2 gate117( .a(N330), .b(N300), .O(N348) );
nand2 gate118( .a(N331), .b(N301), .O(N349) );
nand2 gate119( .a(N332), .b(N302), .O(N350) );
nand2 gate120( .a(N333), .b(N303), .O(N351) );
nand2 gate121( .a(N335), .b(N304), .O(N352) );
nand2 gate122( .a(N337), .b(N305), .O(N353) );
nand2 gate123( .a(N339), .b(N306), .O(N354) );

  xor2  gate203(.a(N307), .b(N341), .O(gate124inter0));
  nand2 gate204(.a(gate124inter0), .b(s_6), .O(gate124inter1));
  and2  gate205(.a(N307), .b(N341), .O(gate124inter2));
  inv1  gate206(.a(s_6), .O(gate124inter3));
  inv1  gate207(.a(s_7), .O(gate124inter4));
  nand2 gate208(.a(gate124inter4), .b(gate124inter3), .O(gate124inter5));
  nor2  gate209(.a(gate124inter5), .b(gate124inter2), .O(gate124inter6));
  inv1  gate210(.a(N341), .O(gate124inter7));
  inv1  gate211(.a(N307), .O(gate124inter8));
  nand2 gate212(.a(gate124inter8), .b(gate124inter7), .O(gate124inter9));
  nand2 gate213(.a(s_7), .b(gate124inter3), .O(gate124inter10));
  nor2  gate214(.a(gate124inter10), .b(gate124inter9), .O(gate124inter11));
  nor2  gate215(.a(gate124inter11), .b(gate124inter6), .O(gate124inter12));
  nand2 gate216(.a(gate124inter12), .b(gate124inter1), .O(N355));
nand2 gate125( .a(N343), .b(N308), .O(N356) );
and9 gate126( .a(N348), .b(N349), .c(N350), .d(N351), .e(N352), .f(N353), .g(N354), .h(N355), .i(N356), .O(N357) );
inv1 gate127( .a(N357), .O(N360) );
inv1 gate128( .a(N357), .O(N370) );
nand2 gate129( .a(N14), .b(N360), .O(N371) );
nand2 gate130( .a(N360), .b(N27), .O(N372) );
nand2 gate131( .a(N360), .b(N40), .O(N373) );
nand2 gate132( .a(N360), .b(N53), .O(N374) );
nand2 gate133( .a(N360), .b(N66), .O(N375) );
nand2 gate134( .a(N360), .b(N79), .O(N376) );
nand2 gate135( .a(N360), .b(N92), .O(N377) );
nand2 gate136( .a(N360), .b(N105), .O(N378) );
nand2 gate137( .a(N360), .b(N115), .O(N379) );
nand4 gate138( .a(N4), .b(N242), .c(N334), .d(N371), .O(N380) );
nand4 gate139( .a(N246), .b(N336), .c(N372), .d(N17), .O(N381) );
nand4 gate140( .a(N250), .b(N338), .c(N373), .d(N30), .O(N386) );
nand4 gate141( .a(N254), .b(N340), .c(N374), .d(N43), .O(N393) );
nand4 gate142( .a(N255), .b(N342), .c(N375), .d(N56), .O(N399) );
nand4 gate143( .a(N256), .b(N344), .c(N376), .d(N69), .O(N404) );
nand4 gate144( .a(N257), .b(N345), .c(N377), .d(N82), .O(N407) );
nand4 gate145( .a(N258), .b(N346), .c(N378), .d(N95), .O(N411) );
nand4 gate146( .a(N259), .b(N347), .c(N379), .d(N108), .O(N414) );
inv1 gate147( .a(N380), .O(N415) );
and8 gate148( .a(N381), .b(N386), .c(N393), .d(N399), .e(N404), .f(N407), .g(N411), .h(N414), .O(N416) );
inv1 gate149( .a(N393), .O(N417) );
inv1 gate150( .a(N404), .O(N418) );
inv1 gate151( .a(N407), .O(N419) );
inv1 gate152( .a(N411), .O(N420) );
nor2 gate153( .a(N415), .b(N416), .O(N421) );
nand2 gate154( .a(N386), .b(N417), .O(N422) );
nand4 gate155( .a(N386), .b(N393), .c(N418), .d(N399), .O(N425) );
nand3 gate156( .a(N399), .b(N393), .c(N419), .O(N428) );
nand4 gate157( .a(N386), .b(N393), .c(N407), .d(N420), .O(N429) );
nand4 gate158( .a(N381), .b(N386), .c(N422), .d(N399), .O(N430) );
nand4 gate159( .a(N381), .b(N386), .c(N425), .d(N428), .O(N431) );
nand4 gate160( .a(N381), .b(N422), .c(N425), .d(N429), .O(N432) );

endmodule