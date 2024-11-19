#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"

typedef uint64_t u64;
typedef uint32_t u32;

struct Ticker {
	Vtop &top;
	VerilatedVcdC &tfp;
	u64 cycle = 0;

	Ticker(Vtop &top, VerilatedVcdC &tfp) : top(top), tfp(tfp) {
	}

	void tick() {
		top.eval();
		tfp.dump(cycle);
		cycle += 1;
	}
};

int main(const int argc, char **argv) {
	auto *const ctx = new VerilatedContext;
	ctx->commandArgs(argc, argv);
	auto *const top = new Vtop{ctx};

	Verilated::traceEverOn(true);
	auto *const tfp = new VerilatedVcdC;
	top->trace(tfp, 99);
	tfp->open("wave.vcd");

	auto ticker = Ticker(*top, *tfp);

	#define tick(with) {{with;} ticker.tick();}

	tick(
		top->clk = 0;
		top->rst = 1;
	);

	tick(top->rst = 0);

	for (u32 count_cycles = 0; count_cycles < 3; count_cycles++) {
		for (u32 local_cycle = 0; local_cycle < (16 - 1) * 2; ++local_cycle) {
			tick(top->clk = 1);
			tick(top->clk = 0);
		}

		tick(top->rst = 1);
		tick(top->rst = 0);
	}

	tfp->close();
	top->final();
	delete top;

	return 0;
}
