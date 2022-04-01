run: build
	@cd bin/release && ./pi
	
build:
	@cp -f src/calcPi.cl bin/release/calcPi.cl
	@g++ -I /usr/include/CL -I ./include/ src/PiCalcGPU.cpp -lOpenCL -o ./bin/release/pi

build_debug:
	@cp -f src/calcPi.cl bin/debug/calcPi.cl
	@g++ -fdiagnostics-color=always -g -I /usr/include/CL -I ./include/ src/PiCalcGPU.cpp -lOpenCL -o ./bin/debug/pi

clean:
	@rm -r bin/*
	@ mkdir bin/debug
	@ mkdir bin/release
