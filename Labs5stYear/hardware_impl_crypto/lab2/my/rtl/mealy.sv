module mealy2 (
    input  logic       rst,
    input  logic       clk,
    input  logic [1:0] i,
    output logic [1:0] y
);
    parameter [1:0] S0 = 2'b00;
    parameter [1:0] S1 = 2'b01;
    parameter [1:0] S2 = 2'b10;
    parameter [1:0] S3 = 2'b11;

    logic [1:0] state, next_state;

    always @(*) begin
        case (state)
            S0: case (i)
                    2'b00: begin next_state = S0; y = 2'b01; end
                    2'b01: begin next_state = S1; y = 2'b00; end
                    2'b10: begin next_state = S2; y = 2'b11; end
                    default: begin next_state = S3; y = 2'b10; end
                endcase
            S1: case (i)
                    2'b00: begin next_state = S1; y = 2'b11; end
                    2'b01: begin next_state = S2; y = 2'b10; end
                    2'b10: begin next_state = S3; y = 2'b01; end
                    default: begin next_state = S0; y = 2'b00; end
                endcase
            S2: case (i)
                    2'b00: begin next_state = S2; y = 2'b10; end
                    2'b01: begin next_state = S3; y = 2'b01; end
                    2'b10: begin next_state = S0; y = 2'b00; end
                    default: begin next_state = S1; y = 2'b11; end
                endcase
            S3: case (i)
                    2'b00: begin next_state = S3; y = 2'b00; end
                    2'b01: begin next_state = S0; y = 2'b11; end
                    2'b10: begin next_state = S1; y = 2'b10; end
                    default: begin next_state = S2; y = 2'b01; end
                endcase
            default: begin next_state = S0; y = 2'b00; end
        endcase
    end

    always @(posedge clk) begin
        if (rst) state <= S0;
        else     state <= next_state;
    end
endmodule
