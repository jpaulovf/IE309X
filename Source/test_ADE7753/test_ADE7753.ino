#include <SPI.h>

#define PIN_RESET_ADE A17 
#define PIN_ADE7753_CS 15
#define PIN_LED_ESP32 2

#define VRMS    0x17
#define IRMS    0x16
#define MODE    0x09
#define WRITE   0x80

static const int spiClk = 1000000; // 1 MHz
uint32_t vrms_reg = 0;
uint32_t irms_reg = 0;
uint16_t read_mode = 0;

float vrms = 0;
float irms = 0;

SPIClass * hspi = NULL;

void setup() {
  Serial.begin(9600);
  
  hspi = new SPIClass(HSPI);
  hspi->begin(); 

  pinMode(PIN_RESET_ADE, OUTPUT);
  pinMode(PIN_ADE7753_CS, OUTPUT);
  pinMode(PIN_LED_ESP32, OUTPUT);
  
  digitalWrite(PIN_RESET_ADE, LOW);
    
}
void loop() {
  
  read_mode = ADE7753_read_mode ();
  vrms_reg = ADE7753_get_vrms ();
  irms_reg = ADE7753_get_irms ();

  //vrms = vrms_reg / 1561400.0 * 353.553;
  //irms = irms_reg / 1561400.0 * 353.553;

  Serial.print("MODE: ");
  Serial.println (read_mode, HEX);
  //Serial.print("\n");
  
  Serial.print("VRMS: ");
  Serial.println (vrms_reg);
  //Serial.print("\n");

  Serial.print("IRMS: ");
  Serial.println (irms_reg);
  
  Serial.print("\n");
  
  digitalWrite(PIN_LED_ESP32, !digitalRead(PIN_LED_ESP32));
  delay(1500);
}


void ADE7753_init (uint8_t mode_config){
  // escreve no registrador MODE (valor default)
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_ADE7753_CS, LOW);
  hspi->write(WRITE | MODE);
  hspi->write(mode_config);
  digitalWrite(PIN_ADE7753_CS, HIGH);
  hspi->endTransaction();
}

uint32_t ADE7753_get_vrms (){
  // le o registrador VRMS
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  uint8_t spi_read_b0;
  uint8_t spi_read_b1;
  uint8_t spi_read_b2;
  uint32_t spi_read;
  digitalWrite(PIN_ADE7753_CS, LOW);
  hspi->transfer(VRMS);
  spi_read_b2 = hspi->transfer(0x00);
  spi_read_b1 = hspi->transfer(0x00);
  spi_read_b0 = hspi->transfer(0x00);
  delayMicroseconds(10);
  digitalWrite(PIN_ADE7753_CS, HIGH);
  hspi->endTransaction();
  spi_read = spi_read_b2<<16 | spi_read_b1<<8 | spi_read_b0;
  return spi_read;
}


uint32_t ADE7753_get_irms (){
  // le o registrador IRMS
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  uint8_t spi_read_b0;
  uint8_t spi_read_b1;
  uint8_t spi_read_b2;
  uint32_t spi_read;
  digitalWrite(PIN_ADE7753_CS, LOW);
  hspi->transfer(IRMS);
  spi_read_b2 = hspi->transfer(0x00);
  spi_read_b1 = hspi->transfer(0x00);
  spi_read_b0 = hspi->transfer(0x00);
  delayMicroseconds(10);
  digitalWrite(PIN_ADE7753_CS, HIGH);
  hspi->endTransaction();
  spi_read = spi_read_b2<<16 | spi_read_b1<<8 | spi_read_b0;
  return spi_read;
}

uint16_t ADE7753_read_mode (){
  // le o registrador MODE
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  uint8_t spi_read_b0;
  uint8_t spi_read_b1;
  uint16_t spi_read;
  digitalWrite(PIN_ADE7753_CS, LOW);
  hspi->transfer(MODE);
  spi_read_b1 = hspi->transfer(0x00);
  spi_read_b0 = hspi->transfer(0x00);
  delayMicroseconds(10);
  digitalWrite(PIN_ADE7753_CS, HIGH);
  hspi->endTransaction();
  spi_read = spi_read_b1<<8 | spi_read_b0;
  return spi_read;
}

void ADE7753_write_VRMSOS (uint8_t offset){
  // escreve no registrador MODE (valor default)
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_ADE7753_CS, LOW);
  hspi->write(WRITE | MODE);
  hspi->write(offset);
  digitalWrite(PIN_ADE7753_CS, HIGH);
  hspi->endTransaction();
}
