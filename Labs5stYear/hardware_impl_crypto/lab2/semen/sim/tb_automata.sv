`timescale 1ns / 1ns

module tb_automata;
  parameter STATES = 4;
  parameter INPUTS = 4;
  
  logic rst = 0;
  logic clk = 1;
  logic [1:0] i;
  logic [1:0] y;

  initial begin
    $dumpfile("work/wave.vcd");
    $dumpvars(0, tb_automata);
  end
  
  // Тест: полный обход 16 рёбер (Mealy)
  // Выход: y = state ^ i, где state(t) = i(t-1)
  initial begin
    integer total;
    total = 0;

    $display("=== Starting Mealy Automata Test ===");
    $display("Testing all 16 edges: (state, input) -> output");

    // Сброс
    @(posedge clk);
    rst = 1'b1;
    @(posedge clk);
    rst = 1'b0;

    // Обход всех пар (a, b), где a — требуемое состояние, b — текущий вход
    for (int a = 0; a < STATES; a++) begin
      for (int b = 0; b < INPUTS; b++) begin
        logic [1:0] a_bits;
        logic [1:0] b_bits;
        logic [1:0] exp; // ожидаемый Mealy-выход

        a_bits = a[1:0];
        b_bits = b[1:0];
        exp = a_bits ^ b_bits;

        // Подготовка: выставить состояние a (предыдущим входом)
        i = a_bits;
        @(posedge clk); // теперь state == a_bits

        // Применить вход b и проверить выход немедленно (Mealy — комбин.)
        i = b_bits;
        #1; // дать времени комбинационной логике

        $display("Edge from state=%2b with i=%2b: y=%2b, expected=%2b",
                 a_bits, b_bits, y, exp);
        assert (y == exp)
        else $error($sformatf("Wrong output: state=%2b, i=%2b, expected=%2b, got=%2b",
                              a_bits, b_bits, exp, y));
        total++;

        // Тактовый шаг вперёд для стабильности
        @(posedge clk);
      end
    end

    $display("=== ALL %0d EDGES TESTED SUCCESSFULLY! ===", total);
    @(posedge clk);
    $finish;
  end
  
  always #5 clk = ~clk;
  
  automata dut (
    .rst(rst),
    .clk(clk),
    .i (i),
    .y (y)
  );
endmodule
