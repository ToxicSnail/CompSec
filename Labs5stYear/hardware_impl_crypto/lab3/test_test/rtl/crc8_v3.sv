module crc8_v3 (
  input  logic       rst,
  input  logic       clk,
  input  logic       data,
  output logic [7:0] crc
);
  // Polynomial: x^8 + x^4 + x^3 + x^2 + 1
  // Bit-serial, MSB-first, feedback from crc[7] ^ data, inject to bit 0.
  logic fb;
  assign fb = data ^ crc[7];

  always @(posedge clk) begin
    if (rst) begin
      crc <= 8'b0;
    end else begin
      for (int i = 0; i < 8; i++) begin
        if (i == 0) begin
          crc[i] <= fb;                 // x^0
        end else if (i == 2) begin
          crc[i] <= crc[i-1] ^ fb;      // tap x^2
        end else if (i == 3) begin
          crc[i] <= crc[i-1] ^ fb;      // tap x^3
        end else if (i == 4) begin
          crc[i] <= crc[i-1] ^ fb;      // tap x^4
        end else begin
          crc[i] <= crc[i-1];           // shift
        end
      end
    end
  end
endmodule

