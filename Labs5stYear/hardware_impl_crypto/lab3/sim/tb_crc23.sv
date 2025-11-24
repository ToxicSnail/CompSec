`timescale 1ns / 1ns
module tb_crc23;
  parameter ITERATIONS = 100;
  parameter MAX_LENGTH = 1024;
  
  // x^23 + x^5 + 1 polynomial
  function logic [22:0] crc23_func(
    input logic [22:0] state,
    input logic        data
  );
    logic [22:0] result;
    result = {state[21:0], data};
    result = result ^ state[22] ^ (state[22] << 5);
    return result;
  endfunction

  logic rst = 0;
  logic clk = 1;
  logic data;
  logic [22:0] crc, ref_crc;
  int length;
  logic current_bit;

  initial begin
    $dumpfile("work/wave.ocd");
    $dumpvars(0, tb_crc23);
  end

  initial begin
    for (int i = 0; i < ITERATIONS; i++) begin
      data = 1'b0;
      @(posedge clk);
      rst = 1'b1;
      @(posedge clk);
      rst = 1'b0;
      ref_crc = 23'd0;
      length = $urandom_range(1, MAX_LENGTH);
      for (int j = 0; j < length; j++) begin
        current_bit = $urandom;
        ref_crc = crc23_func(ref_crc, current_bit);
        data = current_bit;
        @(posedge clk);
      end
      @(posedge clk);
      for (int j = 0; j < 23; j++)
        ref_crc = crc23_func(ref_crc, 1'b0);
      $display("@%0t data=%0b ref=%b rtl=%b", $time, data, ref_crc, crc);
      assert (crc == ref_crc) else $error("CRC23 calculation failed: rtl - %b, model - %b", crc, ref_crc);
    end
    $finish;
  end

  always #5 clk = ~clk;

  crc23 dut (
    .rst (rst ),
    .clk (clk ),
    .data(data),
    .crc (crc )
  );
endmodule
