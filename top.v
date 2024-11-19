module top (
    input            clk,
    input            rst,
    output reg [3:0] led
);

localparam MAX_COUNT = 4'hf;

always @ (posedge clk or posedge rst) begin
    if (rst == 1 || led == MAX_COUNT) begin
        led <= 0;
    end else begin
        led <= led + 1;
    end
end

endmodule
