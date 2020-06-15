## Arduino Sketch for SpectrePlus spectrometer and other prototypes

SpectrePlus is an amateur device that uses an AS7262 to sense fluorescence from 
a PCR tube and transmit the data over bluetooth to a laptop.

- ./lib: dependencies
- ./spectre_plus: firmware for as7262 reading + bluetooth transmission for bluefruit 32u4 feather
- ./spectre_zero: firmware for the Arudino-Uno-based device.
- ./spectre_zero_double: firmware for the Arudino-Uno-based device. This device meausres two PCR tubes
separately.
- ./util_script: scripts for testing ble and receving data.
  - ./util_script/readAda.py

### QuickStart: Upload `spectre_plus/`

- copy the content of `lib/` folder to 
- Use arduino IDE to open the sketch under `spectre_plus/`.
- Connect your arduino to your laptop
- Select the right "Port" in the dropdown menu.
- Select the right board type in dropdown menu.
- Select compile and upload in the dropdown menu/button.

### ToDo

- Simplify this workflow with gnu-make
- drop dependency on ArudinoIDE and use plain C++.