#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"

typedef uint64_t u64;
typedef uint32_t u32;

struct Ticker {
private:
	Vtop &top;
	VerilatedVcdC &tfp;
	u64 cycle = 0;

public:
	Ticker(Vtop &top, VerilatedVcdC &tfp) : top(top), tfp(tfp) {
	}

	void tick() {
		top.eval();
		tfp.dump(cycle);
		cycle += 1;
	}

	void tick_with(const std::function<void()> &eval) {
		eval();
		tick();
	}

	void tick_posedge(CData &wire) {
		tick_with([&] { wire = 1; });
		tick_with([&] { wire = 0; });
	}

	void tick_wire(CData &wire, const CData value) {
		tick_with([&] { wire = value; });
	}
};

int main(const int argc, char **argv) {
	const auto ctx = std::make_unique<VerilatedContext>();
	ctx->commandArgs(argc, argv);
	const auto top = std::make_unique<Vtop>(ctx.get());

	Verilated::traceEverOn(true);
	const auto tracer = std::make_unique<VerilatedVcdC>();
	top->trace(tracer.get(), 99);
	tracer->open("wave.vcd");

	auto ticker = Ticker(*top, *tracer);

	ticker.tick_with([&] {
		top->clk = 0;
		top->rst = 1;
	});

	ticker.tick_wire(top->rst, 0);

	for (u32 count_cycle = 0; count_cycle < 3; ++count_cycle) {
		for (u32 local_cycle = 0; local_cycle < 15; ++local_cycle) {
			ticker.tick_posedge(top->clk);
		}

		ticker.tick_posedge(top->rst);
	}

	tracer->close();
	top->final();

	return 0;
}
