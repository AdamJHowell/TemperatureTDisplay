cmake_minimum_required( VERSION 3.23 )
project( T-Display-S3_SHT40 )

set( CMAKE_CXX_STANDARD 14 )

include_directories( .
    "C:/Users/Adam/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/cores/esp32"
    "C:/Users/Adam/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/libraries/ESPmDNS/src"
    "C:/Users/Adam/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/libraries/WiFi/src"
    "C:/Users/Adam/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/libraries/ArduinoOTA/src"
    "C:/Users/Adam/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/tools/sdk/esp32/include/esp_wifi/include"
    "C:/Code/Arduino/libraries"
    "C:/Users/Adam/Documents/Arduino/libraries/Adafruit_SHT4x_Library"
)

add_executable( T-Display-S3_SHT40
    VolosTemperatureTDisplay.ino
    image.h
    image2.h
)

set_target_properties( T-Display-S3_SHT40 PROPERTIES LINKER_LANGUAGE CXX )
