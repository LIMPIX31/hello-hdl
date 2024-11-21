module top (
    input  wire      clk,
    input  wire      rst,
    input  wire      go,

    output reg [3:0] counter,
    output reg       done_sig
);

fsm f1 (
	.clk     (clk     ),
	.rst     (rst     ),
	.go      (go      ),
	.counter (counter ),
	.done_sig(done_sig)
);

endmodule
