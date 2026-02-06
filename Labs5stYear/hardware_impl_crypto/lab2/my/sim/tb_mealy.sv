`timescale 1ns / 1ns

module tb_mealy2;
  parameter TRANSITIONS = 22;

  logic rst = 0;
  logic clk = 1;
  logic [1:0] i;
  logic [1:0] y;

  logic [1:0] test_inputs [TRANSITIONS];
  logic [1:0] expected_outputs [TRANSITIONS];

  initial begin
    $dumpfile("work/mealy2.vcd");
    $dumpvars(0, tb_mealy2);
  end

  initial begin
    // Подаём 00,01,10,11 циклом
    test_inputs[0]  = 2'b00; expected_outputs[0]  = 2'b01; // S0 - S0
    test_inputs[1]  = 2'b01; expected_outputs[1]  = 2'b00; // S0 - S1
    test_inputs[2]  = 2'b10; expected_outputs[2]  = 2'b01; // S1 - S3
    test_inputs[3]  = 2'b11; expected_outputs[3]  = 2'b01; // S3 - S2
    test_inputs[4]  = 2'b00; expected_outputs[4]  = 2'b10; // S2 - S2
    test_inputs[5]  = 2'b01; expected_outputs[5]  = 2'b01; // S2 - S3
    test_inputs[6]  = 2'b10; expected_outputs[6]  = 2'b10; // S3 - S1
    test_inputs[7]  = 2'b11; expected_outputs[7]  = 2'b00; // S1 - S0
    test_inputs[8]  = 2'b01; expected_outputs[8]  = 2'b00; // S0 - S1
    test_inputs[9]  = 2'b00; expected_outputs[9]  = 2'b11; // S1 - S1
    test_inputs[10] = 2'b11; expected_outputs[10] = 2'b00; // S1 - S0
    test_inputs[11] = 2'b01; expected_outputs[11] = 2'b00; // S0 - S1
    test_inputs[12] = 2'b11; expected_outputs[12] = 2'b00; // S1 - S0
    test_inputs[13] = 2'b11; expected_outputs[13] = 2'b10; // S0 - S3
    test_inputs[14] = 2'b10; expected_outputs[14] = 2'b10; // S3 - S1
    test_inputs[15] = 2'b01; expected_outputs[15] = 2'b10; // S1 - S2
    test_inputs[16] = 2'b11; expected_outputs[16] = 2'b11; // S2 - S1
    test_inputs[17] = 2'b10; expected_outputs[17] = 2'b01; // S1 - S3
    test_inputs[18] = 2'b00; expected_outputs[18] = 2'b00; // S3 - S3
    test_inputs[19] = 2'b01; expected_outputs[19] = 2'b11; // S3 - S0
    test_inputs[20] = 2'b10; expected_outputs[20] = 2'b11; // S0 - S2
    test_inputs[21] = 2'b10; expected_outputs[21] = 2'b00; // S2 - S0
    
    @(posedge clk);
    rst = 1'b1;
    @(posedge clk);
    rst = 1'b0;

    for (int idx = 0; idx < TRANSITIONS; idx++) begin
      i = test_inputs[idx];
      @(posedge clk);
      $display("Test %2d: i=%2b, y=%2b, expected=%2b",
               idx, test_inputs[idx], y, expected_outputs[idx]);
      assert (y == expected_outputs[idx])
      else $error($sformatf("Wrong output on test vector %0d: i=%2b, expected=%2b, got=%2b",
                            idx, test_inputs[idx], expected_outputs[idx], y));
    end

    $finish;
  end

  always #5 clk = ~clk;

  mealy2 dut (
    .rst(rst),
    .clk(clk),
    .i(i),
    .y(y)
  );
endmodule
