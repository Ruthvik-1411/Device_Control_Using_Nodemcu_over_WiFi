# Device_Control_Using_Nodemcu_over_WiFi
The aim here is to control home appliances over wifi while we are in the same house. Say you are in one room and want to off the devices in another room but are too
 lazy to move. All you have to do is enter the ipaddress in the search bar and you will be directed to a website where you can monitor your devices and control them.<br><br>
<b> Working</b><br>
Let us the see the working of the nodemcu code and then the effect on website correspondingly.
 Firstly, when the nodemcu is powered on, We want it to disconnect to the existing connection (as it is off it will be none in this case). Now that everything is clean 
 we will be assigning the mode of our pins and make sure that all our devices are OFF at first. Now we want to operate over wifi, so we need to connect to the wifi,
 for that we need the ssid and password of our wifi to connect and that is included in the code. This makes it kind of limiting because we cant carry our device and connect 
 to a new network, we had to change the ssid,password and also if we accidently change the wifi password we need edit the code again. To overcome this we can use dynamic wifi
 page and that's a bit excess but it can be added. However, I did not include this part in this project.<br>
 Now that we are connected
