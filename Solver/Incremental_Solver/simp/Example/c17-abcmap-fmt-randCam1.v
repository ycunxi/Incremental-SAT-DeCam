// model of gate1 camouflaged by MUX2,
// control bits are s_0 and s_1,
// output is N6,

module	c17 (N1,N2,N3,N4,N5,s_0,s_1,N10,N11);
input N1,N2,N3,N4,N5; //RE__PI;
input s_0, s_1; //RE__ALLOW(00,01,10);
 
output N10,N11;
wire N6,N7,N8,N9, gate1inter0, gate1inter1, gate1inter2, gate1inter3, gate1inter4, gate1inter5, gate1inter6, gate1inter7, gate1inter8, gate1inter9, gate1inter10, gate1inter11, gate1inter12;

  xor2  gate7(.a(N3), .b(N1), .O(gate1inter0));
  nand2 gate8(.a(gate1inter0), .b(s_0), .O(gate1inter1));
  and2  gate9(.a(N3), .b(N1), .O(gate1inter2));
  inv1  gate10(.a(s_0), .O(gate1inter3));
  inv1  gate11(.a(s_1), .O(gate1inter4));
  nand2 gate12(.a(gate1inter4), .b(gate1inter3), .O(gate1inter5));
  nor2  gate13(.a(gate1inter5), .b(gate1inter2), .O(gate1inter6));
  inv1  gate14(.a(N1), .O(gate1inter7));
  inv1  gate15(.a(N3), .O(gate1inter8));
  nand2 gate16(.a(gate1inter8), .b(gate1inter7), .O(gate1inter9));
  nand2 gate17(.a(s_1), .b(gate1inter3), .O(gate1inter10));
  nor2  gate18(.a(gate1inter10), .b(gate1inter9), .O(gate1inter11));
  nor2  gate19(.a(gate1inter11), .b(gate1inter6), .O(gate1inter12));
  nand2 gate20(.a(gate1inter12), .b(gate1inter1), .O(N6));
nand2 gate2( .a(N3), .b(N4), .O(N8) );
nand2 gate3( .a(N2), .b(N8), .O(N7) );
nand2 gate4( .a(N8), .b(N5), .O(N9) );
nand2 gate5( .a(N6), .b(N7), .O(N10) );
nand2 gate6( .a(N7), .b(N9), .O(N11) );

endmodule