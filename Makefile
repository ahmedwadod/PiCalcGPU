run: build
	@./bin/pi
	
build:
	@cp -f src/calcPi.cl bin/calcPi.cl
	@g++ -I /usr/include/CL -I ./include/ src/main.cpp -lOpenCL -o ./bin/pi
