
module c17 (N1,N2,N3,N6,N7,N22,N23);
input N1,N2,N3,N6,N7;
output N22,N23;
wire N10,N11,N16,N19;

nand2 gate1( .a(N1), .b(N3), .O(N10) );
nand2 gate2( .a(N3), .b(N6), .O(N11) );
nand2 gate3( .a(N2), .b(N11), .O(N16) );
nand2 gate4( .a(N11), .b(N7), .O(N19) );
nand2 gate5( .a(N10), .b(N16), .O(N22) );
nand2 gate6( .a(N16), .b(N19), .O(N23) );

endmodule
