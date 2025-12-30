import serial
import time

PORT = "COM4"      # ZMIEŃ na swój port!
BAUD = 115200

def send_cmd(ser, cmd):
    ser.write((cmd + "\n").encode())
    time.sleep(1)
    return ser.read_all().decode(errors="ignore")

def main():
    ser = serial.Serial(PORT, BAUD, timeout=2)
    time.sleep(2)  # reset ESP32

    print("TEST 1: GET TEMP")
    resp = send_cmd(ser, "GET TEMP")
    print(resp)
    assert "OK TEMP" in resp

    print("TEST 2: GET AVG")
    resp = send_cmd(ser, "GET AVG")
    print(resp)
    assert "OK AVG" in resp

    print("TEST 3: STAT")
    resp = send_cmd(ser, "STAT")
    print(resp)
    assert "STATE" in resp

    print("ALL CLI TESTS PASSED")
    ser.close()

if __name__ == "__main__":
    main()
