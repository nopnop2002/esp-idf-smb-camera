# esp-idf-smb-camera
Take a picture and Publish it via SMB.   
You can access a picture using Windows shared folder.   
This project use [this](https://components.espressif.com/components/espressif/esp32-camera) Camera Driver.   
This project use [this](https://github.com/sahlberg/libsmb2) SMB2/3 userspace client.   

![slide-0001](https://user-images.githubusercontent.com/6020549/119617916-454eb280-be3d-11eb-8ba4-d898780ed01d.jpg)
![slide-0002](https://user-images.githubusercontent.com/6020549/119617924-45e74900-be3d-11eb-83ae-03dd86cfd407.jpg)

# Hardware requirements
ESP32 development board with OV2640 camera.   
If you use other camera, edit sdkconfig.default.   
From the left:   
- Aithinker ESP32-CAM   
- Freenove ESP32-WROVER CAM   
- UICPAL ESPS3 CAM   
- Freenove ESP32S3-WROVER CAM (Clone)   

![es32-camera](https://github.com/nopnop2002/esp-idf-websocket-camera/assets/6020549/38dbef9a-ed85-4df2-8d22-499b2b497278)


# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

# Installation
For AiThinker ESP32-CAM, you need to use a USB-TTL converter and connect GPIO0 to GND.   

|ESP-32|USB-TTL|
|:-:|:-:|
|U0TXD|RXD|
|U0RXD|TXD|
|GPIO0|GND|
|5V|5V|
|GND|GND|


# Installation   
```
git clone https://github.com/nopnop2002/esp-idf-smb-camera
cd esp-idf-smb-camera
git clone https://github.com/sahlberg/libsmb2 -b libsmb2-6.2 components/libsmb2
cp esp-idf/CMakeLists.txt components/libsmb2/
idf.py set-target {esp32/esp32s3}
idf.py menuconfig
idf.py flash monitor
```

# Start firmware
For AiThinker ESP32-CAM, Change GPIO0 to open and press the RESET button.

# Configuration
Set the following items using menuconfig.

![config-main](https://user-images.githubusercontent.com/6020549/66692052-c17e9b80-ecd5-11e9-8316-075350ceb2e9.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/201548817-960220da-a6cf-4dab-8cc0-dc85bbc054cd.jpg)

## Wifi Setting

![config-wifi-1](https://user-images.githubusercontent.com/6020549/119243503-529c4080-bba2-11eb-92c5-b59f66f9fea6.jpg)

You can connect using the mDNS hostname instead of the IP address.   
![config-wifi-2](https://user-images.githubusercontent.com/6020549/119243504-5334d700-bba2-11eb-8c77-f958251d8611.jpg)

You can use static IP.   
![config-wifi-3](https://user-images.githubusercontent.com/6020549/119243505-5334d700-bba2-11eb-9677-47cb6d1f9536.jpg)


## SMB Server Setting

- CONFIG_ESP_SMB_USER   
Username with shared folder permissions.
- CONFIG_ESP_NEED_PASSWORD   
Shared access requires password.
- CONFIG_ESP_SMB_PASSWORD   
Password with shared folder permissions.
- CONFIG_ESP_SMB_HOST   
IP address or mDNS host name of shared host.   
- CONFIG_ESP_SMB_PATH   
Shared path name.

![config-smb-1](https://user-images.githubusercontent.com/6020549/183030948-2e077e33-7cc6-414f-8f3c-69c29f2dab80.jpg)

When a password is required to access the shared folder.
![config-smb-2](https://user-images.githubusercontent.com/6020549/183030955-4dccc112-beaf-43a0-ae43-ccc797a911fa.jpg)

## File Name Setting

You can select the file name to write to the shared folder from the following.   
- Always the same file name   
	![config-filename-1](https://user-images.githubusercontent.com/6020549/119243498-5203aa00-bba2-11eb-87d5-053636dbb85a.jpg)

- File name based on date and time   
	When you choose date and time file name, you will need an NTP server.   
	The file name will be YYYYMMDD-hhmmss.jpg.   
	![config-filename-2](https://user-images.githubusercontent.com/6020549/119243499-5203aa00-bba2-11eb-8c0f-6bb42d125d64.jpg)

- Add FrameSize to Remote file Name   
	When this is enabled, FrameSize is added to remote file name like this.   
	`20210520-165740_800x600.jpg`   
	![config-filename-3](https://user-images.githubusercontent.com/6020549/119243501-529c4080-bba2-11eb-8ba4-85cdd764b0fc.jpg)


## Select Board
![config-board](https://github.com/nopnop2002/esp-idf-smb-camera/assets/6020549/5a19cc82-d53a-45ed-9ebf-530f884fe8b2)


## Select Frame Size
Large frame sizes take longer to take a picture.   

![config-framesize-1](https://user-images.githubusercontent.com/6020549/122480400-c0078980-d007-11eb-9e91-75b30dad065c.jpg)
![config-framesize-2](https://user-images.githubusercontent.com/6020549/122480403-c0a02000-d007-11eb-9cd7-ee1a4250d120.jpg)

## Select Shutter

You can choose one of the following shutter methods

- Shutter is the Enter key on the keyboard   
	For operation check.   
	When using the USB port provided by the USB Serial/JTAG Controller Console, you need to enable the following line in sdkconfig.
	```
	CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
	```
	![config-shutter-1](https://user-images.githubusercontent.com/6020549/193154988-0de5f305-7bdb-4d05-8a74-8760b39a0161.jpg)

- Shutter is a GPIO toggle   

	- Initial Sate is PULLDOWN   
		The shutter is prepared when it is turned from OFF to ON, and a picture is taken when it is turned from ON to OFF.   

	- Initial Sate is PULLUP   
		The shutter is prepared when it is turned from ON to OFF, and a picture is taken when it is turned from OFF to ON.   

	I confirmed that the following GPIO can be used.   

	|GPIO|PullDown|PullUp|
	|:-:|:-:|:-:|
	|GPIO12|OK|NG|
	|GPIO13|OK|OK|
	|GPIO14|OK|OK|
	|GPIO15|OK|OK|
	|GPIO16|NG|NG|

	![config-shutter-2](https://user-images.githubusercontent.com/6020549/193154991-1271a408-2ac4-43ef-a7f4-482cf51985ee.jpg)

- Shutter is TCP Socket   
	ESP32 acts as a TCP server and listens for requests from TCP clients.   
	You can use tcp_send.py as shutter.   
	`python3 ./tcp_send.py`
	![Image](https://github.com/user-attachments/assets/4c301018-2f8c-4644-be3f-417222fb1842)

- Shutter is UDP Socket   
	ESP32 acts as a UDP listener and listens for requests from UDP clients.   
	You can use this command as shutter.   
	`echo -n "take" | socat - UDP-DATAGRAM:255.255.255.255:49876,broadcast`   
	You can use udp_send.py as shutter.   
	Requires netifaces.   
	`python3 ./udp_send.py`
	![Image](https://github.com/user-attachments/assets/3dcd72be-d0ef-4bd9-9273-f420ca88f11b)   
	You can use these devices as shutters.   
	![Image](https://github.com/user-attachments/assets/cc97da4e-6c06-4604-8362-f81c6fb6eb58)   
	Click [here](https://github.com/nopnop2002/esp-idf-selfie-trigger) for details.   

- Shutter is HTTP Request   
	ESP32 acts as an HTTP server and listens for HTTP GET requests.   
	You can use this command as shutter.   
	`curl "http://esp32-camera.local:8080/take/picture"`
	![config-shutter-5](https://user-images.githubusercontent.com/6020549/193154996-4d783d74-6e24-482a-9ebc-681bc40cf570.jpg)

## Flash Light   
ESP32-CAM by AI-Thinker have flash light on GPIO4.   

![config-flash](https://user-images.githubusercontent.com/6020549/122480398-bf6ef300-d007-11eb-929e-88a5b9aa994f.jpg)

## PSRAM   
When using ESP32S3, you need to set the PSRAM type according to the hardware.   
ESP32S3-WROVER CAM has Octal Mode PSRAM.   
UICPAL ESPS3 CAM  has Quad Mode PSRAM.   
![config-psram](https://github.com/nopnop2002/esp-idf-websocket-camera/assets/6020549/ba04f088-c628-46ac-bc5b-2968032753e0)

# Built-in WEB Server
ESP32 works as a web server.   
You can check the pictures taken using the built-in WEB server.   
Enter the ESP32's IP address and port number in the address bar of your browser.   
You can connect using mDNS hostname instead of IP address.   

![browser](https://user-images.githubusercontent.com/6020549/124227364-837a7880-db45-11eb-9d8b-fa15c676adac.jpg)

# Reference
https://github.com/nopnop2002/esp-idf-smb-client
