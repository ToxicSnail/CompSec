// S->P transform: updated S-box and P-block

module ps_transform (
  input  logic [3:0] din,
  output logic [3:0] dout
);

  logic [3:0] s_out; // S-substitution result

  // New 4-bit S-box (permutation of 0..F):
  // 0→C, 1→5, 2→6, 3→B,
  // 4→9, 5→0, 6→A, 7→D,
  // 8→3, 9→E, A→F, B→8,
  // C→4, D→7, E→1, F→2
  always @(*) begin
    unique case (din)
      4'h0: s_out = 4'hC;
      4'h1: s_out = 4'h5;
      4'h2: s_out = 4'h6;
      4'h3: s_out = 4'hB;
      4'h4: s_out = 4'h9;
      4'h5: s_out = 4'h0;
      4'h6: s_out = 4'hA;
      4'h7: s_out = 4'hD;
      4'h8: s_out = 4'h3;
      4'h9: s_out = 4'hE;
      4'hA: s_out = 4'hF;
      4'hB: s_out = 4'h8;
      4'hC: s_out = 4'h4;
      4'hD: s_out = 4'h7;
      4'hE: s_out = 4'h1;
      4'hF: s_out = 4'h2;
    endcase
  end

  // New P permutation of bits: P = [1, 3, 0, 2]
  // dout[3:0] = { s_out[1], s_out[3], s_out[0], s_out[2] }
  assign dout[3] = s_out[1];
  assign dout[2] = s_out[3];
  assign dout[1] = s_out[0];
  assign dout[0] = s_out[2];

endmodule

