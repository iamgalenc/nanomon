import psutil
import serial
import serial.tools.list_ports
import time
import sys

def find_serial_port():
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        raise Exception("No serial ports found")
    for port in ports:
        print(f"Found port: {port.device} - {port.description}")
    return ports[0].device

try:
    port = find_serial_port()
    ser = serial.Serial(port=port, baudrate=9600)
except Exception as e:
    sys.exit(f"Error opening serial port: {e}")

ser.write(b"WELCOME")

prev_data = psutil.net_io_counters()
prev_time = time.time()

def format_value(value):
    return f"{value:>5.1f}"

try:
    while True:
        current_time = time.time()
        time_elapsed = current_time - prev_time

        if time_elapsed > 0.5:  
        
            current_data = psutil.net_io_counters()
            download_speed = ((current_data.bytes_recv - prev_data.bytes_recv ) / (time_elapsed * 1_048_576))
            upload_speed = ((current_data.bytes_sent - prev_data.bytes_sent)  / (time_elapsed * 1_048_576))
            prev_data = current_data
            prev_time = current_time

            cpu = psutil.cpu_percent(interval=1.2) 
            mem = psutil.virtual_memory().percent

            serial_data = (
                f"{format_value(cpu)}"
                f"{format_value(mem)}"
                f"{format_value(download_speed)}"
                f"{format_value(upload_speed)}"
            )
            serial_data_bytes = serial_data.encode("UTF-8")

            print(serial_data_bytes)
            ser.write(serial_data_bytes)  
            
except KeyboardInterrupt:
    print("Exiting...")

finally:
    ser.close()