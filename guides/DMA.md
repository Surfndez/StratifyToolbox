

# IO
| Pin | Function | Primary Pin | Timing     |
|-----|----------|-------------|------------|
| 1   | RX4      | PB8         | PC8/T3C3   |
| 2   | TX4      | PB9         | PA1/T2C2   |
| 3   | CS4      | PE11/T1C2   | PB10/T2C3* |
| 4   | SCK4     | PE12        | PB11/T2C4* |
| 5   | MISO4    | PE13/T1C3   | PC6/T3C1*  |
| 6   | MOSI4    | PE14/T1C4   | PC7/T3C2*  |
| 7   | GND      | GND         | GND        |
| 8   | TX2      | PA2         | PA2/T2C3   |
| 9   | DIN4     | PE3         | PA3/T2C4   |
| 10  | WS4      | PE4         | PA6/T3C1   |
| 11  | BCLK4    | PE5         | PA7/T3C2   |
| 12  | DOUT4    | PE6         | PA15/T2C1  |
| 13  | TRIGGER  | PA10        | X          |

*only if needed to do timing analysis on SPI bus.


# Timer DMA Requests for Logic

DMAMUX allows any stream to be used for any request.

| DMA.Stream | Function | IO     |
|------------|----------|--------|
| 1.0        | T1C2     | 3      |
| 1.1        | T1C3     | 5/13   |
| 1.2        | T1C4     | 6/     |
| 1.3        | T2C1     | 12     |
| 1.4        | T2C2     | 2      |
| 1.5        | T2C3     | 8/3    |
| 1.6        | T2C4     | 9/4    |
| 1.7        | T3C1     | 10/5   |
| 2.0        | T3C2     | 11/6,8 |
| 2.1        | T3C4     | 1      |
| 2.2        | SPITX    | 1      |
| 2.3        | SPIRX    | 1      |
| 2.4        | UARTTX   | 1      |
| 2.5        | UARTRX   | 1      |
| 2.6        | I2STX    | 1      |
| 2.7        | I2SRX    | 1      |

SDIO has dedicated DMA.