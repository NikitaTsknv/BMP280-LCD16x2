all: bmp280_lcd16x2

lcd16x2:
	g++ main.cpp bmp280.cpp -o bmp280_lcd16x2 -lwiringPi -lwiringPiDev -lpthread

clean:
	rm -rf bmp280_lcd16x2
