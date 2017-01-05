module	c17 (N1,N2,N3,N4,N5,N6,N7,N8,N9,N10,N11);



input N1,N2,N3,N4,N5;
 
output N10,N11;

wire N6,N7,N8,N9;
     
nand2 gate1( .a(N1), .b(N3), .O(N6) );
nand2 gate2( .a(N3), .b(N4), .O(N8) );
nand2 gate3( .a(N2), .b(N8), .O(N7) );
nand2 gate4( .a(N8), .b(N5), .O(N9) );
nand2 gate5( .a(N6), .b(N7), .O(N10) );
nand2 gate6( .a(N7), .b(N9), .O(N11) );

endmodule
