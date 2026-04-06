/*
 * Serial Protocol Demonstrator for Logic Analyzer
 * Target: ESP32
 *
 * Demonstrates UART, SPI, I2C, CAN (TWAI), and 1-Wire by generating
 * signals on dedicated pins. Capture all channels simultaneously in
 * PulseView or Saleae Logic with the appropriate protocol decoder.
 *
 * ── Wiring ──────────────────────────────────────────────────────────────────
 *
 *  Protocol  Pin(s)                  Notes
 *  --------  ----------------------  ----------------------------------------
 *  UART      TX  GPIO17              Connect GPIO17 → GPIO16 for loopback
 *            RX  GPIO16
 *  SPI       MOSI GPIO23             Connect GPIO23 → GPIO19 for loopback
 *            MISO GPIO19
 *            SCK  GPIO18
 *            CS   GPIO5
 *  I2C       SDA  GPIO21             No slave needed; NACK is normal here
 *            SCL  GPIO22
 *  CAN       TX   GPIO26             Connect GPIO26 → GPIO27 for loopback
 *            RX   GPIO27             TWAI monitors RX to verify its own bits;
 *                                    floating RX causes bus errors + ESP_ERR_TIMEOUT
 *  1-Wire    DATA GPIO4              4.7 kΩ pullup to 3.3 V on this pin
 *
 * ── Logic Analyzer Setup ────────────────────────────────────────────────────
 *
 *  UART:   Decoder: UART  Baud: 9600  Format: 8N1
 *  SPI:    Decoder: SPI   Clock rate: 1 MHz   CPOL=0 CPHA=0
 *  I2C:    Decoder: I2C   (auto-detects clock rate)
 *  CAN:    Decoder: CAN   Bit rate: 500 kbps
 *  1-Wire: Decoder: 1-Wire
 */

#include <SPI.h>
#include <Wire.h>
#include "driver/twai.h"

// ── Pin assignments ──────────────────────────────────────────────────────────

#define UART_TX_PIN   17
#define UART_RX_PIN   16   // Wire to TX for loopback

#define SPI_SCK_PIN   18
#define SPI_MISO_PIN  19   // Wire to MOSI for loopback
#define SPI_MOSI_PIN  23
#define SPI_CS_PIN     5

#define I2C_SDA_PIN   21
#define I2C_SCL_PIN   22

#define CAN_TX_PIN    26
#define CAN_RX_PIN    27   // Required by driver; leave unconnected

#define OW_PIN         4   // 4.7 kΩ pullup to 3.3 V required

// ── Timing ───────────────────────────────────────────────────────────────────

#define DEMO_PAUSE_MS   2000   // Pause between each protocol demo

// ── 1-Wire bit-bang ──────────────────────────────────────────────────────────
//
// Releases the line by switching to INPUT so the external pullup raises it.
// Driving low is done by switching to OUTPUT and writing LOW.

static void ow_release() {
  pinMode(OW_PIN, INPUT_PULLUP);   // Internal ~45 kΩ pullup — fine for bench demo
                                   // Replace with external 4.7 kΩ for real devices
}

static void ow_pull_low() {
  pinMode(OW_PIN, OUTPUT);
  digitalWrite(OW_PIN, LOW);
}

// Returns true if a slave sent a presence pulse.
static bool ow_reset() {
  ow_pull_low();
  delayMicroseconds(480);   // Reset pulse
  ow_release();
  delayMicroseconds(70);    // Wait for presence window
  bool present = (digitalRead(OW_PIN) == LOW);
  delayMicroseconds(410);   // Rest of reset slot
  return present;
}

static void ow_write_bit(bool bit) {
  ow_pull_low();
  if (bit) {
    delayMicroseconds(6);   // Short low = logic 1
    ow_release();
    delayMicroseconds(64);
  } else {
    delayMicroseconds(60);  // Long low = logic 0
    ow_release();
    delayMicroseconds(10);
  }
}

static void ow_write_byte(uint8_t data) {
  for (int i = 0; i < 8; i++) {
    ow_write_bit(data & 0x01);
    data >>= 1;   // LSB first per 1-Wire spec
  }
}

// ── Initialization ───────────────────────────────────────────────────────────

static void init_uart() {
  Serial2.begin(9600, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
}

static void init_spi() {
  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH);
}

static void init_i2c() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(100000);   // 100 kHz standard mode
}

static void init_can() {
  // TWAI_MODE_NO_ACK skips the ACK bit check, but the controller still
  // reads RX to verify its own transmitted bits. Wire TX → RX for loopback.
  twai_general_config_t g = TWAI_GENERAL_CONFIG_DEFAULT(
    (gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN, TWAI_MODE_NO_ACK);
  twai_timing_config_t  t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t  f = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g, &t, &f) != ESP_OK ||
      twai_start()                    != ESP_OK) {
    Serial.println("CAN: driver init failed");
  }
}

static void init_onewire() {
  ow_release();   // Start with line released (high via pullup)
}

// ── Protocol demos ───────────────────────────────────────────────────────────

static void demo_uart() {
  Serial.println(">>> UART  (GPIO17=TX, GPIO16=RX)  9600 8N1");
  Serial2.println("Hello from UART");
  Serial2.flush();
}

static void demo_spi() {
  Serial.println(">>> SPI   (GPIO23=MOSI, GPIO19=MISO, GPIO18=SCK, GPIO5=CS)  1 MHz SPI Mode 0");
  uint8_t tx[] = { 0xDE, 0xAD, 0xBE, 0xEF };
  uint8_t rx[sizeof(tx)] = {};

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_CS_PIN, LOW);
  for (size_t i = 0; i < sizeof(tx); i++) {
    rx[i] = SPI.transfer(tx[i]);
  }
  digitalWrite(SPI_CS_PIN, HIGH);
  SPI.endTransaction();

  Serial.printf("    TX: %02X %02X %02X %02X\n", tx[0], tx[1], tx[2], tx[3]);
  Serial.printf("    RX: %02X %02X %02X %02X  (loopback — should match TX)\n",
                rx[0], rx[1], rx[2], rx[3]);
}

static void demo_i2c() {
  Serial.println(">>> I2C   (GPIO21=SDA, GPIO22=SCL)  100 kHz, address 0x42");
  Wire.beginTransmission(0x42);
  Wire.write(0xAB);
  Wire.write(0xCD);
  uint8_t err = Wire.endTransmission();

  // No slave is present, so a NACK (err=2) is expected.
  // The logic analyzer still captures the full address + data framing.
  Serial.printf("    endTransmission result: %d  (%s)\n",
                err, err == 0 ? "ACK" : "NACK — no slave present, expected");
}

static void demo_can() {
  Serial.println(">>> CAN   (GPIO26=TX)  500 kbps, ID 0x123");
  twai_message_t msg = {};
  msg.identifier      = 0x123;
  msg.data_length_code = 4;
  msg.data[0] = 0xDE;
  msg.data[1] = 0xAD;
  msg.data[2] = 0xBE;
  msg.data[3] = 0xEF;

  esp_err_t result = twai_transmit(&msg, pdMS_TO_TICKS(100));
  Serial.printf("    twai_transmit: %s\n",
                result == ESP_OK ? "OK" : esp_err_to_name(result));
}

static void demo_onewire() {
  Serial.println(">>> 1-Wire (GPIO4)  Reset + Skip ROM (0xCC) + Convert T (0x44)");
  bool present = ow_reset();
  Serial.printf("    Presence pulse: %s\n",
                present ? "detected" : "none — no device attached, expected");
  ow_write_byte(0xCC);   // Skip ROM: address all devices
  ow_write_byte(0x44);   // Convert T: trigger temperature conversion (DS18B20)
}

// ── Arduino entry points ─────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n========================================");
  Serial.println(" Serial Protocol Demonstrator — ESP32");
  Serial.println("========================================");
  Serial.println("Initializing interfaces...");

  init_uart();
  init_spi();
  init_i2c();
  init_can();
  init_onewire();

  Serial.println("Ready. Starting demo loop.\n");
}

void loop() {
  demo_uart();
  delay(DEMO_PAUSE_MS);

  demo_spi();
  delay(DEMO_PAUSE_MS);

  demo_i2c();
  delay(DEMO_PAUSE_MS);

  demo_can();
  delay(DEMO_PAUSE_MS);

  demo_onewire();
  delay(DEMO_PAUSE_MS);

  Serial.println("--- cycle complete ---\n");
}
