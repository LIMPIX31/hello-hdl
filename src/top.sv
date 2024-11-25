module top (
    input logic clk,
    input logic rst,
    input logic go,

    output logic [3:0] counter,
    output logic       done_sig
);

    localparam logic [1:0] StateIdle = 0;
    localparam logic [1:0] StateCounting = 1;
    localparam logic [1:0] StateDone = 2;

    localparam logic [3:0] MaxCount = 'hf;

    logic [1:0] state;

    always @(posedge clk or posedge rst) begin
        if (rst == 1) begin
            state <= StateIdle;
        end else begin
            case (state)
                StateIdle: begin
                    if (go == 1) begin
                        state <= StateCounting;
                    end
                end

                StateCounting: begin
                    if (counter == MaxCount) begin
                        state <= StateDone;
                    end
                end

                StateDone: state <= StateIdle;

                default: state <= StateIdle;
            endcase
        end
    end

    always @(posedge clk or posedge rst) begin
        if (state == StateCounting && rst == 0) begin
            counter <= counter + 1;
        end else begin
            counter <= 0;
        end
    end

    always @(state) begin
        if (state == StateDone) begin
            done_sig = 1;
        end else begin
            done_sig = 0;
        end
    end

endmodule
