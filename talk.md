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
| Equipment (DCE)         |
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

# UART

# SPI

# CAN

# I2C

# 1-Wire

# Demos

# Bob

# Joe

---

% Joe <kamikazejoe@gmail.com> & Rob <robert@rtward.com>

![](static/qrcode.png)

Talk: [${TALK_URL}](${TALK_URL})

Repo: [${REPO_URL}](${REPO_URL})
