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

## Next Diagnostic Order

1. Connect USB directly to the Freenove ESP32 WROVER board, not to the GPIO expansion board.
2. Use a known data-capable USB cable.
3. Avoid hubs/adapters for the first test if possible.
4. Run:

```bash
tools/dev/check_ports.sh
```

5. If still no port, try a second cable.
6. If still no port, try another Mac USB port or adapter.
7. If a USB device appears but no serial port appears, install/check the board's USB-UART driver.

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
