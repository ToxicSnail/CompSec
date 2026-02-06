`timescale 1ns/1ns

module tb_ps_transform;
  logic [3:0] din;
  logic [3:0] dout;

  logic [3:0] s_ref;
  logic [3:0] expected;

  // Optional wave dump (change path as needed)
  initial begin
    $dumpfile("work/wave.ocd");
    $dumpvars(0, tb_ps_transform);
  end

  // Sweep all inputs, compute golden model (S then P), compare
  initial begin
    integer i;
    for (i = 0; i < 16; i = i + 1) begin
      din = i[3:0];
      #1;

      // S-box (must match RTL):
      // 0→C, 1→5, 2→6, 3→B,
      // 4→9, 5→0, 6→A, 7→D,
      // 8→3, 9→E, A→F, B→8,
      // C→4, D→7, E→1, F→2
      unique case (din)
        4'h0: s_ref = 4'hC;
        4'h1: s_ref = 4'h5;
        4'h2: s_ref = 4'h6;
        4'h3: s_ref = 4'hB;
        4'h4: s_ref = 4'h9;
        4'h5: s_ref = 4'h0;
        4'h6: s_ref = 4'hA;
        4'h7: s_ref = 4'hD;
        4'h8: s_ref = 4'h3;
        4'h9: s_ref = 4'hE;
        4'hA: s_ref = 4'hF;
        4'hB: s_ref = 4'h8;
        4'hC: s_ref = 4'h4;
        4'hD: s_ref = 4'h7;
        4'hE: s_ref = 4'h1;
        4'hF: s_ref = 4'h2;
        default: s_ref = '0;
      endcase

      // P permutation: P = [1, 3, 0, 2]
      expected[3] = s_ref[1];
      expected[2] = s_ref[3];
      expected[1] = s_ref[0];
      expected[0] = s_ref[2];

      if (dout !== expected) begin
        $error($sformatf("Mismatch: din=%h exp=%h got=%h", din, expected, dout));
      end
    end

    $finish;
  end

  // DUT
  ps_transform dut (
    .din(din),
    .dout(dout)
  );
endmodule

