clean:
	rm -rf ./build

build:
	mkdir -p build && cd build && cmake ..
	cmake --build build -j

run: build
	build/hello_hdl +trace

wave:
	gtkwave result.vcd

run-wave: run && wave
