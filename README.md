# Make a smart doorbell using the ESP32

Take a look at the [Heise article](https://www.heise.de/select/ct/2018/17/1534215254552977) and the [Reichelt article](https://www.reichelt.de/magazin/how-to/smarte-tuerklingel/) to get started. 
These articles serve as foundation for this project. But of course there are some differences:
- This project uses an ESP32 instead of an ESP8266.
- This project does not reset the ESP32 every time someone rings the bell. Instead a GPIO pin is used for waking up the ESP32 from deep sleep.
- The code is optimized for ease of use (it requires only minimal config) and performance (it buffers the WiFi settings so that connecting to the WiFi is fast).

## Tips & Tricks
Here are some information that might be handy:
- Create the [esp-doorbell-config.h](esp-doorbell/esp-doorbell-config.h) file based on the [esp-doorbell-config.h.example](esp-doorbell/esp-doorbell-config.h.example) file and add your credentials in there.
- Depending on the lenght of the wire to your door bell button you might want to use a different resistance/LED to esnure that the optocoupler works properly (especially if you only a low supply voltage).
- Make sure to add the [ESP32 board manager](https://dl.espressif.com/dl/package_esp32_index.json) and install the [TR-064-SOAP-Library](https://github.com/Aypac/Arduino-TR-064-SOAP-Library) in the Arduino IDE.
- For configuring your FritzBox check out the [Heise article](https://www.heise.de/select/ct/2018/17/1534215254552977).

## Changelog

Please see [CHANGELOG](CHANGELOG.md) for more information what has changed recently.

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for details.

## Security

If you discover any security related issues, please email [lukas@aerdes.com](mailto:lukas@aerdes.com) instead of using the issue tracker.

## Postcardware

You are free to use this sketch, but if it makes it to your production environment we highly appreciate you sending us a postcard from your hometown. The address is: Lukas Müller, Dirklangendwarsstraat 5, 2611HZ Delft, The Netherlands.

## License

The MIT License (MIT). Please see [LICENSE](LICENSE.md) for more information.