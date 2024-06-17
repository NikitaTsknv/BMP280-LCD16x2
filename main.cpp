#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <wiringPi.h>
#include <lcd.h>
#include "bmp280.h"

#define	LED	0

#define LCD_RS  2  // RS - Выбор регистра
#define LCD_E   3  // E  - Еnable (строб по спаду)
#define LCD_D4  1  // D4 - бит данных
#define LCD_D5  4  // D5 - бит данных
#define LCD_D6  5  // D6 - бит данных
#define LCD_D7  6  // D7 - бит данных

int main(int argc, char **argv) {
    //	char * device = "/dev/i2c-0";
    //	char * device = "/dev/i2c-1";
    std::string devicePath = "/dev/i2c-2";
    char * device = const_cast<char*>(devicePath.c_str());
    //	char * device = "/dev/i2c-3";
    
    int counterUpdate = 0;
    
	int fdLcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
	
	int code = wiringPiSetup();
	if (code != 0) {
		return -1;
	}
	else {
		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPuts(fdLcd, "LCD init: OK");
        delay(2000);
	}
    
    pinMode(LED, OUTPUT);
    
    int devId = BMP280_I2C_ADDRESS1;

    try {
        BMP280 * bmp280 = new BMP280(device, devId);
        int fdBMP280 = bmp280->init();
        if (fdBMP280 < 0) {
			lcdClear(fdLcd);
			lcdPosition(fdLcd, 0, 0);
			lcdPuts(fdLcd, "BMP280 init: ERROR");
			delay(2000);
            return -1;
        }
		else {
			lcdPosition(fdLcd, 0, 1);
			lcdPuts(fdLcd, "BMP280 init: OK");
			delay(2000);
		}

		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPuts(fdLcd, "BMP280 PARAMETRS");
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "fd BMP280: 0x%02x", fdBMP280);
        delay(2000);
		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "chip id: 0x%02x", bmp280->getChipId());
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "chip ver: 0x%02x", bmp280->getChipVersion());
        delay(2000);

        bmp280->reset();
        bmp280->setPowerMode(BMP280_NORMAL_MODE);
        bmp280->setTemperatureOversampling(BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE);
        bmp280->setPressureOversampling(BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE);
        bmp280->setIrrFilter(BMP280_FILTER_COEFF_16);
        bmp280->setStandbyTime(BMP280_STANDBY_TIME_250_MS);

		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "pw mode: 0x%02x", bmp280->getPowerMode());
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "osrs_p: 0x%02x", bmp280->getPressureOversampling());
        delay(2000);
		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "osrs_t: 0x%02x", bmp280->getTemperatureOversampling());
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "filter: 0x%02x", bmp280->getIrrFilter());
        delay(2000);
		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "t_sb: 0x%02x", bmp280->getStandbyTime());
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "spi3w sts: 0x%02x", bmp280->getSpi3w());
        delay(2000);
		lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "measuring: 0x%02x", bmp280->getMeasuringStatus());
		lcdPosition(fdLcd, 0, 1);
		lcdPrintf(fdLcd, "im_update: 0x%02x", bmp280->getImUpdateStatus());
        delay(2000);
		lcdClear(fdLcd);
		
        while (1) {
			
			if (counterUpdate > 0) {
				delay(10000);
			} else {
				delay(1000);
			}
			
			digitalWrite(LED, HIGH);	// On
			delay(500);
            
            BMP280Data * bmp280Data = bmp280->getBMP280Data();
		
			lcdClear(fdLcd);
			lcdPosition(fdLcd, 0, 0);
			lcdPrintf(fdLcd, "P: %.2f hPa", bmp280Data->getPressure());
			lcdPosition(fdLcd, 0, 1);
			lcdPrintf(fdLcd, "T: %.2f C", bmp280Data->getTemperature());
            //printf("A: %.2f m\n\n", bmp280Data->getAltitude());
			
			digitalWrite(LED, LOW);	// Off
			
			counterUpdate = 1;
			
            delete bmp280Data;
        }
        delete bmp280;
    } catch (std::exception & e) {
        lcdClear(fdLcd);
		lcdPosition(fdLcd, 0, 0);
		lcdPrintf(fdLcd, "%s\n", e.what());
		delay(5000);
    }
    return 0;
}
