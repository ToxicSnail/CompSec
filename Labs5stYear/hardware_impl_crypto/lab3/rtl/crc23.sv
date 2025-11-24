module crc23 (
  input  logic        rst,
  input  logic        clk,
  input  logic        data,
  output logic [22:0] crc
);
  logic main_xor;
  assign main_xor = data ^ crc[22];

  always @(posedge clk) begin
    if (rst) begin
      crc <= 23'b0;
    end else begin
      for (int i = 0; i < 23; i++) begin
        if (i == 0) begin
          crc[i] <= main_xor;
        end else if (i == 5) begin
          crc[i] <= main_xor ^ crc[i-1];
        end else begin
          crc[i] <= crc[i-1];
        end
      end
    end
  end
endmodule