% Serial Communication Formats
% Joe <kamikazejoe@gmail.com> & Rob <robert@rtward.com>
%![](static/qrcode.png)<br/>Talk: [${TALK_URL}](${TALK_URL})<br/>Repo: [${REPO_URL}](${REPO_URL})

# Serial Communication Formats

::: notes

Some speaker notes here

:::

# RS-232 et al.

## Overview

 - _Standard_ Serial Port
 - Originally designed for modems
 - Only point-to-point

## Diagram
 ```
    -------------------
    | Data Terminal   |
    | Equipment (DTE) |
    -------------------
            |
----------------------------
| Data Circuit-Terminating |
| Equipment (DCE)          |
----------------------------
```

## Pinout - All

 - Data Terminal Ready
 - Data Carrier Connect
 - Data Set Ready
 - Ring
 - Request to Send
 
## Pinout - All

 - Ready to Receive
 - Clear to Send
 - Tx
 - Rx
 - Ground

## Pinout - Required

 - Tx
 - Rx
 - Terminal Ready
 - Set Ready
 - Ground

## Signalling

 - Pin driven high for 1
 - Pin driven low for 0
 - 3-15v diff

## RS-422

 - Often used as an extender
 - Runs on two twisted pairs + ground
 - Voltage diff on each pair for signal
 - Still point-to-point (kinda)
 
::: notes

 - Spec allows for multiple receivers on a single twisted pair, but only one driver.

:::

## RS-485

 - Similar to 422
 - Supports multiple devices on a single pair
 - Was the SCSI transmission protocol
 - Heavily used on model trains

::: notes

 - Can actually interoperate with 422 in some cases
 - Also used to extend 232 connections

:::

## 422 / 485 Pinout

```
  Device A                              Device B
  --------                              --------
            ~~~~~~~~~~~~~~~~~~~~~
   TX+ ----~  Twisted Pair 1    ~---- RX+
   TX- ----~                    ~---- RX-
            ~~~~~~~~~~~~~~~~~~~~~

            ~~~~~~~~~~~~~~~~~~~~~
   RX+ ----~  Twisted Pair 2    ~---- TX+
   RX- ----~                    ~---- TX-
            ~~~~~~~~~~~~~~~~~~~~~
```

## 422 / 485 Signaling

 - Wires are driven apart
 - +/- 200mv min
 - +/- 6v max

# UART

Universal Asynchronous Receiver-Transmitter

::: notes

- One of the oldest and simplest methods for devices to communicate
- No shared clock. Which is why it's "Asynchronous"
- Instead, there is an agreed upon baud rate.
- Is slow, anywhere between 300 to 115200 bps (though usually 9600 or above)

:::

## Uses

- Microcontrollers
- Rasberry Pis
- Bluetooth Modules
- GPS Modules

::: notes

- Not just Respberry Pis
- Pretty much any System-on-Chip
- How you get console access
- Goes over the aformentioned RS standards

:::

## Signaling

![](static/async-frame.png)

::: notes

- Devices must agree upon:
- Voltages
- Baud Rate
- Parity bits
- Data bit size
- Stop bit size
- Flow control

:::

## Wiring

```
TX ------------> RX
RX <------------ TX
```

::: notes

- Wiring is dead simple
- Which is why it's so widely used
- Could scale down even more with HDUART

:::

# SPI

# 1-Wire

 - Half Duplex Only
 - Each chip is uialt textque
 - Multi device
 - Two wires (data & ground)
 
## Uses

 - iButtons
 - Magsafe 
 - Power supplies
 
## Signaling

 - Wire is normally high (3-5v)
 - Signals work by pulling low
 - Short low = 1
 - Long low = 0

# I2C

Inter Integrated Circuit

::: notes

- I2C or I^2C
- Little bit UART, little bit SPI
- Slower than SPI, but easier to add components

:::

## Uses

- Sensors
- Small OLED Displays
- DACs and ADCs
- EEPROMs

::: notes

- Lots of sensors, including what's in your PCs
- Drives small OLEDs
- Extended Display Identification Data (EDID)
  - Identify monitor specs for VGA, DVI, and HDMI
  - Can hack a VGA port into an I2C port

:::

## Signaling

![](static/i2c-frame.png)

::: notes

- Typical voltages +3.3V to +5V
- Each devices has a 7-bit hardware address
- This allows for 127 devices connected by just two wires.
- Standar speed is around 100 kbps, but there are specs for up to 5 Mbps

:::

## Wiring

```                                                                                                     
---------------------------------------------------------- Vdd                                           
                        |   |                                                                                 
                        <   <                                                                           
                        >   >  Pull-up Resistors
                        <   <                                                                      
                        |   |                                                                            
---+---------------+----+---|----+-------------+---------  SDA                                           
---|--+------------|--+-----+----|--+----------|--+------  SCL                                           
   |  |            |  |          |  |          |  |                                                      
+------------+   +----------+  +----------+  +----------+                                                
|            |   |          |  |          |  |          |                                                
|     uC     |   |   ADC    |  |   DAC    |  |   uC     |                                                
| Controller |   | Target 1 |  | Target 2 |  | Target 3 |                                                
|            |   |          |  |          |  |          |                                                
+------------+   +----------+  +----------+  +----------+                                                                                                                                                    
```

::: notes

- Only 2 wires connecting all devices
  - Serial Data Line (SDA)
  - Serial Clock Line (SCL)
  - They also need pull-up resistors to the voltage line

:::

# CAN

 - Mostly used for cars
 - Similar to RS-422 / 485
 
# High-Speed Bus

![](static/canbus-high-speed.png)

::: notes

 - Each end has 120ohm resistor

:::

# Signaling

 - Signal 1 by driving high and low
 - Signal 0 by allowing them to equalize

# Low-Speed Bus

![](static/canbus-low-speed.png)

::: notes

 - Total resistence should be 100ohm
 - More fault tolerant

:::

# Signaling

 - Signal 1 by driving high and low
 - Signal 0 by inverting 1

# Protocol

 - Wait after message
 - Message start by driving high
 - Start with message ID
 
# Packet Format

![](static/canbus-frame.png)

# Priority

![](static/canbus-priority.png)

# Demos

# Bob

# Joe

---

% Joe <kamikazejoe@gmail.com> & Rob <robert@rtward.com>

![](static/qrcode.png)

Talk: [${TALK_URL}](${TALK_URL})

Repo: [${REPO_URL}](${REPO_URL})
