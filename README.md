# esp-idf-smb-camera
Take a picture and Publish it via SMB.   
You can access a picture using Windows shared folder.   
This project use [ESP32 Camera Driver](https://github.com/espressif/esp32-camera).   
This project use [SMB2/3 userspace client](https://github.com/sahlberg/libsmb2).   

![slide-0001](https://user-images.githubusercontent.com/6020549/119617916-454eb280-be3d-11eb-8ba4-d898780ed01d.jpg)
![slide-0002](https://user-images.githubusercontent.com/6020549/119617924-45e74900-be3d-11eb-83ae-03dd86cfd407.jpg)

# Software requirements
esp-idf v4.4 or later.   
The mDNS strict mode [issue](https://github.com/espressif/esp-idf/issues/6190) has been resolved.   

# Installation
Use a USB-TTL converter.   

|ESP-32|USB-TTL|
|:-:|:-:|
|U0TXD|RXD|
|U0RXD|TXD|
|GPIO0|GND|
|5V|5V|
|GND|GND|


```
git clone https://github.com/nopnop2002/esp-idf-smb-camera
cd esp-idf-smb-camera
git clone https://github.com/espressif/esp32-camera components/esp32-camera
git clone https://github.com/sahlberg/libsmb2 components/libsmb2
idf.py set-target esp32
idf.py menuconfig
idf.py flash monitor
```

# Start firmware
Change GPIO0 to open and press the RESET button.

# Configuration
Set the following items using menuconfig.

![config-main](https://user-images.githubusercontent.com/6020549/66692052-c17e9b80-ecd5-11e9-8316-075350ceb2e9.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/119619751-441e8500-be3f-11eb-858a-5a6ce8318d40.jpg)

## Wifi Setting

![config-wifi-1](https://user-images.githubusercontent.com/6020549/119243503-529c4080-bba2-11eb-92c5-b59f66f9fea6.jpg)

You can use the mDNS hostname instead of the IP address.   
![config-wifi-2](https://user-images.githubusercontent.com/6020549/119243504-5334d700-bba2-11eb-8c77-f958251d8611.jpg)

You can use static IP.   
![config-wifi-3](https://user-images.githubusercontent.com/6020549/119243505-5334d700-bba2-11eb-9677-47cb6d1f9536.jpg)



## SMB Server Setting

- CONFIG_ESP_SMB_USER   
Username with shared folder permissions.
- CONFIG_ESP_NEED_PASSWORD   
Shared access requires password.
- CONFIG_ESP_SMB_PASSWORD   
Password of Username.
- CONFIG_ESP_SMB_HOST   
IP address of shared host.   
__mDMS name cannot be used.__   
- CONFIG_ESP_SMB_PATH   
Shared path name.

![config-smb-1](https://user-images.githubusercontent.com/6020549/119619823-58fb1880-be3f-11eb-9176-6d6e28e874b4.jpg)

When a password is required to access the shared folder.
![config-smb-2](https://user-images.githubusercontent.com/6020549/119619913-6f08d900-be3f-11eb-9ea1-3a8ac966ed19.jpg)


## File Name Setting

You can select the file name to write to the shared folder from the following.   
- Always the same file name   
- File name based on date and time   
When you choose date and time file name, you will need an NTP server.   
The file name will be YYYYMMDD-hhmmss.jpg.   

![config-filename-1](https://user-images.githubusercontent.com/6020549/119243498-5203aa00-bba2-11eb-87d5-053636dbb85a.jpg)
![config-filename-2](https://user-images.githubusercontent.com/6020549/119243499-5203aa00-bba2-11eb-8c0f-6bb42d125d64.jpg)

- Add FrameSize to Remote file Name   
When this is enabled, FrameSize is added to remote file name like this.   
`20210520-165740_800x600.jpg`   

![config-filename-3](https://user-images.githubusercontent.com/6020549/119243501-529c4080-bba2-11eb-8ba4-85cdd764b0fc.jpg)

## Select Frame Size
Large frame sizes take longer to take a picture.   

![config-framesize-1](https://user-images.githubusercontent.com/6020549/122480400-c0078980-d007-11eb-9e91-75b30dad065c.jpg)
![config-framesize-2](https://user-images.githubusercontent.com/6020549/122480403-c0a02000-d007-11eb-9cd7-ee1a4250d120.jpg)

## Select Shutter

You can choose one of the following shutter methods

- Shutter is the Enter key on the keyboard   
For operation check

![config-shutter-1](https://user-images.githubusercontent.com/6020549/99890068-db432e00-2c9e-11eb-84e2-4e6c5f05fb7a.jpg)

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

![config-shutter-2](https://user-images.githubusercontent.com/6020549/99897437-d2714d00-2cdc-11eb-8e59-c8bf4ef25d62.jpg)

- Shutter is TCP Socket   
You can use tcp_send.py.   
`python ./tcp_send.py`

![config-shutter-3](https://user-images.githubusercontent.com/6020549/99890070-dc745b00-2c9e-11eb-9ae8-45ac11db5db5.jpg)

- Shutter is UDP Socket   
You can use udp_send.py.   
`python ./udp_send.py`

![config-shutter-4](https://user-images.githubusercontent.com/6020549/99889941-658a9280-2c9d-11eb-8bc7-06f2b67af3cb.jpg)

- Shutter is HTTP Request   
You can use this command.   
`curl "http://esp32-camera.local:8080/take_picture"`

![config-shutter-5](https://user-images.githubusercontent.com/6020549/99889881-b6e65200-2c9c-11eb-96c2-6fdde929dbe0.jpg)


## Flash Light

ESP32-CAM by AI-Thinker have flash light on GPIO4.   

![config-flash](https://user-images.githubusercontent.com/6020549/122480398-bf6ef300-d007-11eb-929e-88a5b9aa994f.jpg)

# Reference
https://github.com/nopnop2002/esp-idf-smb-client
