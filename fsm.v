module fsm (
    input  wire      clk,
    input  wire      rst,
    input  wire      go,

    output reg [3:0] counter,
    output reg       done_sig
);

localparam STATE_IDLE = 2'd0;
localparam STATE_COUNTING = 2'd1;
localparam STATE_DONE = 2'd2;

localparam MAX_COUNT = 4'hf;

reg [1:0] state;

always @ (posedge clk or posedge rst) begin
    if (rst == 1) begin
        state <= STATE_IDLE;
    end else begin
        case (state)
            STATE_IDLE: begin
                if (go == 1) begin
                    state <= STATE_COUNTING;
                end
            end

			STATE_COUNTING: begin
				if (counter == MAX_COUNT) begin
					state <= STATE_DONE;
				end
			end

			STATE_DONE: state <= STATE_IDLE;

            default: state <= STATE_IDLE;
        endcase
    end
end

always @ (posedge clk or posedge rst) begin
	if (state == STATE_COUNTING && rst == 0) begin
		counter <= counter + 1;
	end else begin
		counter <= 0;
	end
end

always @ (state) begin
	if (state == STATE_DONE) begin
		done_sig = 1;
	end else begin
		done_sig = 0;
	end
end

endmodule
