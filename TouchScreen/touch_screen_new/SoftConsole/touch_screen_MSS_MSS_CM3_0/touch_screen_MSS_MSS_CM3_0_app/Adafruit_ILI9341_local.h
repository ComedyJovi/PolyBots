#ifndef ADAFRUIT_ILI9341_H_
#define ADAFRUIT_ILI9341_H_
#include <inttypes.h>
#include "Adafruit_GFX_local.h"
#include "drivers/mss_spi/drivers/mss_spi/mss_spi.h"


#define ILI9341_TFTWIDTH   240
#define ILI9341_TFTHEIGHT  320

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09

#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13

#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29

#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E

#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_VSCRSADD   0x37
#define ILI9341_PIXFMT     0x3A

#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6

#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7

#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD

#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1
/*
#define ILI9341_PWCTR6     0xFC

 */

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F


uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void startWrite(mss_spi_slave_t slave);
void spiWrite(uint8_t msg);
void endWrite(mss_spi_slave_t slave);
uint8_t spiRead();
void rx_handler( uint32_t rx_frame );
void writeCommand(uint8_t cmd);
void begin(struct Print * print);
void delay(int milliseconds);
void scrollTo(uint16_t y);
void invertDisplay(boolean i);
void setRotation(struct Print * print, uint8_t m);
void SPI_WRITE16(uint16_t y);
void SPI_WRITE32(uint32_t xa);
void SPI_WRITE_PIXELS(uint8_t*colors, uint8_t len);
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void writePixel(uint16_t color);
void writePixels(uint16_t * colors, uint32_t len);
void writePixelCoordinates(struct Print * print, int16_t x, int16_t y, uint16_t color);
void writeColor(struct Print * print, uint16_t color, uint32_t len);
void writeFillRect(struct Print * print,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void writeFastVLine(struct Print * print,int16_t x, int16_t y, int16_t h, uint16_t color);
void writeFastHLine(struct Print * print,int16_t x, int16_t y, int16_t w, uint16_t color);
void drawPixel(struct Print * print, int16_t x, int16_t y, uint16_t color);
void drawFastVLine(struct Print * print,int16_t x, int16_t y,
        int16_t h, uint16_t color);
void drawFastHLine(struct Print * print,int16_t x, int16_t y,
        int16_t w, uint16_t color);
void fillRect(struct Print * print, int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color);
void drawBitmap(struct Print * print, int16_t x, int16_t y, int16_t w, int16_t h,
  const uint16_t *pcolors);
uint8_t readcommand8(uint8_t c, uint8_t index);


#endif /* ADAFRUIT_ILI9341_H_ */
