`timescale 1ns / 1ns
module tb_crc8_v3;
  parameter ITERATIONS = 100;
  parameter MAX_LENGTH = 512;

  // x^8 + x^4 + x^3 + x^2 + 1, bit-serial reference
  function automatic logic [7:0] crc8_func(
    input logic [7:0] state,
    input logic       data
  );
    logic fb;
    logic [7:0] next;
    fb = state[7] ^ data;
    // Build next state with taps at 2,3,4
    next[0] = fb;                 // x^0
    next[1] = state[0];
    next[2] = state[1] ^ fb;      // x^2
    next[3] = state[2] ^ fb;      // x^3
    next[4] = state[3] ^ fb;      // x^4
    next[5] = state[4];
    next[6] = state[5];
    next[7] = state[6];
    return next;
  endfunction

  logic rst = 0;
  logic clk = 1;
  logic data;
  logic [7:0] crc;
  int length;
  logic current_bit;
  logic seq [0:MAX_LENGTH-1];
  logic [7:0] exp;

  initial begin
    $dumpfile("work/wave.ocd");
    $dumpvars(0, tb_crc8_v3);
  end

  initial begin
    for (int i = 0; i < ITERATIONS; i++) begin
      // Generate random message and compute expected CRC offline
      length = $urandom_range(1, MAX_LENGTH);
      exp = '0;
      for (int j = 0; j < length; j++) begin
        current_bit = $urandom; // LSB used as a random bit
        seq[j] = current_bit;
        exp = crc8_func(exp, current_bit);
      end
      // Append 7 zero bits to align with DUT latency
      for (int j = 0; j < 7; j++) begin
        exp = crc8_func(exp, 1'b0);
      end

      // Drive DUT (reset before first active cycle)
      rst = 1'b1;
      data = 1'b0;
      @(posedge clk);
      @(posedge clk);
      rst = 1'b0;
      for (int j = 0; j < length; j++) begin
        data = seq[j];
        @(posedge clk);
      end
      for (int j = 0; j < 8; j++) begin
        data = 1'b0;
        @(posedge clk);
      end

      $display("@%0t exp=%b rtl=%b len=%0d", $time, exp, crc, length);
      assert (crc == exp) else $error("CRC8-V3 calculation failed: rtl - %b, exp - %b", crc, exp);
    end
    $finish;
  end

  always #5 clk = ~clk;

  crc8_v3 dut (
    .rst (rst ),
    .clk (clk ),
    .data(data),
    .crc (crc )
  );
endmodule
