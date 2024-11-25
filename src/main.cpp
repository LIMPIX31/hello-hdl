#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef int32_t i32;

struct Ticker {
private:
	Vtop &top;
	VerilatedVcdC &tfp;
	u64 ticks = 0;

public:
	Ticker(Vtop &top, VerilatedVcdC &tfp) : top(top), tfp(tfp) {
	}

	void tick(const u64 times) {
		for (u64 i = 0; i < times; i++) {
			top.eval();
			tfp.dump(ticks);
			ticks += 1;
		}
	}

	void tick_once() {
		tick(1);
	}

	void tick_with(const std::function<void()> &eval) {
		eval();
		tick_once();
	}

	void tick_wire(CData &wire, const CData value) {
		tick_with([&] { wire = value; });
	}

	void tick_posedge(CData &wire) {
		tick_wire(wire, 1);
		tick_wire(wire, 0);
	}
};

void run(Vtop &top, Ticker &ticker) {
	auto clock = [&] (const u64 times) {
		for (u64 i = 0; i < times; i++) {
			ticker.tick_posedge(top.clk);
		}
	};

	ticker.tick_posedge(top.rst);
	// Idle ticks
	ticker.tick(8);

	// Trigger state machine
	ticker.tick_with([&] {
		top.go = 1;
		top.clk = 1;
	});
	ticker.tick_wire(top.clk, 0);
	ticker.tick_with([&] {
		top.clk = 1;
		top.go = 0;
	});
	ticker.tick_wire(top.clk, 0);

	// Clock and count to the end
	clock(32);
}

int main(const i32 argc, char **argv) {
	const auto ctx = std::make_unique<VerilatedContext>();
	ctx->commandArgs(argc, argv);
	const auto top = std::make_unique<Vtop>(ctx.get());

	Verilated::traceEverOn(true);
	const auto tracer = std::make_unique<VerilatedVcdC>();
	top->trace(tracer.get(), 99);
	tracer->open("result.vcd");

	auto ticker = Ticker(*top, *tracer);

	run(*top, ticker);

	tracer->close();
	top->final();

	return 0;
}
