`timescale 1ns / 1ns

module tb_moora;
	parameter TRANSITIONS = 41;
	
	logic rst = 0;
	logic clk = 1;
	logic [5:0] test_vectors [TRANSITIONS-1:0]; // явно указываю где будет деление по три бита?
	logic [2:0] a;
	logic [2:0] y;
	logic [2:0] y_ref;
	
initial begin
	$dumpfile("work/wave.ocd");
	$dumpvars(0, tb_moora);
end

initial begin
	$readmemb("sequence.tv", test_vectors);
	
	@(posedge clk);
	rst = 1'b1;
	@(posedge clk);
	rst = 1'b0;
	
	for (int i = 0; i < TRANSITIONS; i++) begin
		{a, y_ref} = test_vectors[i];
		
		@(posedge clk);
		
		if (y !== y_ref) begin
                $error("Ошибка на тесте %0d: a=%3b, ожидался y=%3b, получен y=%3b", i + 1, a, y_ref, y);
		end
	end
	
	$finish;
end

always #5 clk = ~clk;

moora dut(
	.rst (rst),
	.clk (clk),
	.a (a),
	.y (y)
);

endmodule
	
