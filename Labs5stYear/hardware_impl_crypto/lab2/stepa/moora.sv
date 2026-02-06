// top eror in my lab:

// 1) "S0" in first line in sequence.tv
// 2) zabul one perehod in S3 

module moora (
  input logic rst,
  input logic clk,
  input logic [2:0] a,
  output logic [2:0] y  
);

  typedef enum logic [2:0] {
    S0 = 3'b000,
    S1 = 3'b001, 
    S2 = 3'b010,
    S3 = 3'b011,
    S4 = 3'b100
  } state_t;

  state_t state;
  state_t next_state;

  always_ff @(posedge clk or posedge rst) begin
    if (rst)
      state <= S0;
    else
      state <= next_state;
  end

  always @(*) begin
    case(state)
      S0: begin
        if (a == 3'b000) next_state = S1; 
        else next_state = S0;             
      end
      S1: begin
        if (a == 3'b001) next_state = S2;
        else if (a == 3'b000) next_state = S3;
        else if (a == 3'b010) next_state = S4;
        else next_state = S1;
      end
      S2: begin
        if (a == 3'b010) next_state = S3;
        else if (a == 3'b000) next_state = S1;
        else next_state = S2;
      end
      S3: begin
        if (a == 3'b100) next_state = S4;
        else if (a == 3'b001) next_state = S1;
        else if (a == 3'b000) next_state = S2; // tut bula error
        else next_state = S3;
      end
      S4: begin
        if (a == 3'b101) next_state = S0;
        else next_state = S4;
      end
      default: next_state = S0;
    endcase
  end

  always @(*) begin
    case(state)
      S0: y = 3'b000;
      S1: y = 3'b001;
      S2: y = 3'b010;
      S3: y = 3'b011;
      S4: y = 3'b100;  
      default: y = 3'b000;
    endcase
  end

endmodule
