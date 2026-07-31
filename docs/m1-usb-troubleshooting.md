# M1 USB Troubleshooting

## Symptom

The ESP32 board power LED is on, but no serial port appears.

Observed ports:

```text
/dev/cu.debug-console
/dev/cu.Bluetooth-Incoming-Port
```

Missing expected ports:

```text
/dev/cu.usbserial*
/dev/cu.SLAB_USB*
/dev/cu.wchusb*
/dev/cu.usbmodem*
```

## Interpretation

Power LED only proves 5 V power. It does not prove USB data.

If the board does not appear in the macOS USB device tree and no serial port is created, the likely causes are:

1. USB cable is charge-only.
2. USB is connected through a power-only hub or adapter.
3. Cable is connected to an expansion board power input instead of the WROVER programming USB port.
4. USB-UART chip or board USB path is damaged.
5. Driver issue, if the board appears in USB tree but no `/dev/cu.*` port is created.

Current hardware constraint:

```text
MacBook has no native USB-A port.
The WROVER must currently connect through a USB hub or adapter.
```

## Confirmed Attempts

2026-07-31:

```text
WROVER power LED turns on.
USB 2.0 connection did not create a serial port.
USB 3.0 connection did not create a serial port.
USB hub was confirmed working.
macOS USB tree did not show Freenove, ESP32, CH340, CH341, WCH, CP210x, UART, USB serial, usbserial, or usbmodem.
```

Current interpretation:

```text
The original issue was before firmware flashing.
The board was powered but the USB data path did not enumerate until the cable was changed.
After switching to a USB-C to USB-C cable, `/dev/cu.usbserial-10` appeared with USB VID:PID `1A86:7523`.
The firmware was flashed successfully to the Freenove ESP32 WROVER.
```

## Next Diagnostic Order

1. Keep using the working USB-C to USB-C data cable.
2. Keep the WROVER on `/dev/cu.usbserial-10` while the serial port remains stable.
3. Run:

```bash
tools/dev/check_ports.sh
```

4. If the port disappears, first re-seat or replace the cable before changing firmware settings.
5. If a USB device appears but no serial port appears, install/check the board's USB-UART driver.

## Driver Note

Many Freenove ESP32 boards use CH340/CH341 USB-UART. Freenove documentation for ESP32 kits states that ESP32 uses CH340 for downloading code and shows checking with:

```bash
ls /dev/cu.usb*
```

Expected macOS result after driver and data cable are working:

```text
/dev/cu.usbserialXXXX
```

or a similar USB serial path.
