module automata (
    input logic rst,
    input logic clk,
    input logic [1:0] i,
    output logic [1:0] y
);
    parameter [1:0] S0 = 2'b00, S1 = 2'b01, S2 = 2'b10, S3 = 2'b11;
    logic [1:0] state, next_state;

    always @(*) begin
        case(state)
            S0:
                if (i == 2'b00) 
                    next_state = S0;
                else if (i == 2'b01)
                    next_state = S1;
                else if (i == 2'b10)
                    next_state = S2;
                else 
                    next_state = S3;

            S1:
                if (i == 2'b00) 
                    next_state = S0;
                else if (i == 2'b01)
                    next_state = S1;
                else if (i == 2'b10)
                    next_state = S2;
                else 
                    next_state = S3;

            S2:
                if (i == 2'b00) 
                    next_state = S0;
                else if (i == 2'b01)
                    next_state = S1;
                else if (i == 2'b10)
                    next_state = S2;
                else 
                    next_state = S3;

            S3:
                if (i == 2'b00) 
                    next_state = S0;
                else if (i == 2'b01)
                    next_state = S1;
                else if (i == 2'b10)
                    next_state = S2;
                else 
                    next_state = S3;

            default: next_state = S0;
        endcase
    end

    always @(posedge clk) begin
        if (rst) state <= S0;
        else     state <= next_state;
    end
    // Автомат Мили: выход зависит от текущего состояния и входа
    // Нетривиальная функция выхода: XOR состояния и входа
    assign y = state ^ i;
endmodule
