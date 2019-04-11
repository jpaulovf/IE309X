/*  ESP32 SPI Test
 *  Send some data to SPI and sniff it
 */
#include <SPI.h>

// SPI clock (Hz)
#define SPICLK_HZ 1000000 // 1 MHz clock

// SPI device pointer
SPIClass *my_spi = NULL;

// Message to send via SPI
char *msg = "Jill Sandwich!";

void setup() {

  // Creating new instance of SPI
  my_spi = new SPIClass(HSPI);

  // Initializing SPI with default pins
  // SCLK: 14, MISO: 12, MOSI: 13, SS: 15
  my_spi->begin();

  // Setting SS pin as output
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT); // Indicator LED

}

void loop() {

  digitalWrite(2, HIGH);
  
  my_spi->beginTransaction(SPISettings(SPICLK_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(15, LOW);
  my_spi->transferBytes((uint8_t *) msg, NULL, 14);
  digitalWrite(15, HIGH);
  my_spi->endTransaction();

  delay(100);
  digitalWrite(2, LOW);
  delay(100);

}
