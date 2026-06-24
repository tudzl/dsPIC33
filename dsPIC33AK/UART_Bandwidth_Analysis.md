# UART Bandwidth Analysis - dsPIC33AK I2S Mic Capture

## System Parameters

| Parameter | Value |
|-----------|-------|
| MCU | dsPIC33AK512MPS506 |
| I2S Sample Rate | 24000 Hz |
| Sample Width | 16-bit (2 bytes) |
| MIC_BUFFER_SIZE | 4000 samples |
| UART Baud | 500000 bps (8N1) |
| DMA TX | Enabled (payload zero-copy) |

## Frame Protocol

```
[Header 4B] AA 55 A5 5A
[Length 2B] payload bytes (uint16_t LE)
[Payload 8000B] int16_t[4000] little-endian PCM audio
[Counter 4B] frame count (uint32_t LE)
[Checksum 2B] sum of payload+counter bytes (uint16_t LE)
```

Total frame size: 4 + 2 + 8000 + 4 + 2 = **8012 bytes**

## Bandwidth Calculation

| Item | Formula | Result |
|------|---------|--------|
| UART max throughput | 500000 / 10 | **50,000 bytes/s** |
| Frame rate | 24000 / 4000 | **6 frames/s** |
| Required data rate | 6 x 8012 | **48,072 bytes/s** |
| Utilization | 48072 / 50000 | **96.1%** |

## Timing Analysis

| Item | Formula | Result |
|------|---------|--------|
| Buffer fill time | 4000 / 24000 | **166.67 ms** |
| Frame TX time | 8012 x 10 / 500000 | **160.24 ms** |
| Margin | 166.67 - 160.24 | **6.43 ms (3.9%)** |

## Baud Rate Comparison

| Baud Rate | TX Time (ms) | Fill Time (ms) | Margin | Status |
|-----------|-------------|----------------|--------|--------|
| 460800 | 173.9 | 166.67 | -7.2 ms | **OVERFLOW - drops frames** |
| **500000** | **160.2** | **166.67** | **+6.4 ms** | **OK** |
| 921600 | 87.0 | 166.67 | +79.7 ms | OK (HW not supported) |

## DMA Optimization (V1.08)

### Architecture
```
1. CPU: Calculate checksum (traverse 8000B read-only)
2. CPU: Send header + length blocking (6 bytes, ~0.12ms)
3. DMA: Send payload directly from mic_read_ptr (8000 bytes, CPU free)
4. CPU: Send counter + checksum blocking (6 bytes, ~0.12ms)
```

### Benefits vs Full-Copy DMA (V1.06-V1.07)
- Eliminated 16KB RAM (2 x 8012B tx buffers)
- Eliminated 8000-byte memcpy per frame
- CPU free during 8000B DMA transfer (~160ms)

### Benefits vs Blocking TX (V1.05)
- CPU idle during payload transmission
- ISR (SCCP6 I2S sampling) not delayed by UART TX busy-wait
- Reduced SPI FIFO overflow risk

## Key Design Constraint

At 500000 baud with 24KHz/4000-sample frames, the system operates at **96.1% UART utilization** with only 6.4ms margin. Any baud rate below ~482,000 will cause frame drops.

## Version History

| Version | Baud | Buffer | DMA | Notes |
|---------|------|--------|-----|-------|
| V1.05 | 460800 | 4000 | No | Working but bandwidth overflow |
| V1.06 | 460800 | 4000 | Full-copy | DMA debug, checksum errors |
| V1.07 | 460800 | 4000 | Full-copy | DMA working, CPU idle stats |
| V1.08 | 500000 | 4000 | Zero-copy | Optimized, no frame drops |
