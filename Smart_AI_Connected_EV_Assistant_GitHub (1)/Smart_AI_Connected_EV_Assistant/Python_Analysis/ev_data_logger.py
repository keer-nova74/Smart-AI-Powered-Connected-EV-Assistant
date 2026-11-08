"""
Read CSV telemetry from ESP32 #2 over USB serial, save it to a CSV file,
and print basic summary statistics.

Install: pip install pyserial
Run: python ev_data_logger.py --port COM5 --baud 115200
Close Arduino Serial Monitor before running this script.
"""
import argparse
import csv
import time
from pathlib import Path

import serial

FIELDS = [
    "elapsed_ms", "speed_kmh", "battery_voltage_v", "motor_temp_c",
    "soc_percent", "latitude", "longitude"
]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", required=True, help="Serial port, e.g. COM5")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--output", default="ev_vehicle_log.csv")
    args = parser.parse_args()

    rows = []
    output_path = Path(args.output)
    print(f"Opening {args.port} at {args.baud} baud. Press Ctrl+C to stop.")

    try:
        with serial.Serial(args.port, args.baud, timeout=2) as ser, \
                output_path.open("w", newline="", encoding="utf-8") as csv_file:
            time.sleep(2)  # ESP32 may reset when the serial port opens.
            writer = csv.DictWriter(csv_file, fieldnames=FIELDS)
            writer.writeheader()

            while True:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line or line.startswith("elapsed_ms"):
                    continue

                parts = line.split(",")
                if len(parts) != len(FIELDS):
                    continue

                try:
                    row = dict(zip(FIELDS, parts))
                    # Validate numeric fields before saving.
                    for key in FIELDS:
                        float(row[key])
                    writer.writerow(row)
                    csv_file.flush()
                    rows.append({k: float(v) for k, v in row.items()})
                    print(line)
                except ValueError:
                    print(f"Skipped malformed row: {line}")

    except KeyboardInterrupt:
        print("\nLogging stopped.")
    except serial.SerialException as exc:
        print(f"Serial error: {exc}")
        print("Check the port and close Arduino Serial Monitor.")
        return

    if rows:
        print(f"\nSaved {len(rows)} records to {output_path.resolve()}")
        for key in ("speed_kmh", "battery_voltage_v", "motor_temp_c", "soc_percent"):
            values = [r[key] for r in rows]
            print(f"{key}: min={min(values):.2f}, max={max(values):.2f}, "
                  f"average={sum(values)/len(values):.2f}")
    else:
        print("No valid telemetry rows were received.")


if __name__ == "__main__":
    main()
