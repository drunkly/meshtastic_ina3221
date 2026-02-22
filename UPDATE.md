#UPDATE
git fetch upstream
git checkout beta
git merge upstream/beta
git checkout ina3221_enhanced
git merge beta



#COMPILE
pio --version
pip3 install --user -U platformio
~/.local/bin/pio --version
echo 'export PATH=$PATH:~/.local/bin' >> ~/.bashrc
source ~/.bashrc
pio run -e heltec-v4

#esptool -b 115200 write-flash 0x650000 .pio/build/heltec-v4/firmware.bin


#UPDATE OTA (https://meshtastic.es/docs/guias-diy/guia-wifiota-esp32/)
#install and compile ota firmware, then
esptool --port COM5 -b 115200 write_flash 0x650000 .pio/build/esp32s3/firmware.elf
meshtastic --host 192.168.3.156 --reboot-ota
#wait 10 secs
espota.py -r -i 192.168.3.156 -f .pio/build/heltec-v4/firmware.bin


rmdir /s /q .pio\libdeps\heltec-v4\INA3221