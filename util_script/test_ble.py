import Adafruit_BluefruitLE
from Adafruit_BluefruitLE.services import UART


# Get the BLE provider for the current platform.
ble = Adafruit_BluefruitLE.get_provider()

ble.list_devices()