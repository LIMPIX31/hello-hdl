build:
	verilator -Wall --cc -CFLAGS "-g" --trace top.v --exe main.cpp
	make -j -C obj_dir -f Vtop.mk Vtop
run: build
	./obj_dir/Vtop -t
run-wave: run
	gtkwave wave.vcd
