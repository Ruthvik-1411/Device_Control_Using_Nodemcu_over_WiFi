# Device_Control_Using_Nodemcu_over_WiFi
The aim here is to control home appliances over wifi while we are in the same house. Say you are in one room and want to off the devices in another room but are too
 lazy to move. All you have to do is enter the ipaddress in the search bar and you will be directed to a website where you can monitor your devices and control them.<br><br>
<b>Components Used:</b><br>
<li>Nodemcu &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&ensp;x1
<li>Device/Appliances/Leds  &emsp;x4(+1)
<li>Relay Module&ensp;(according to the number of devices)
<li>Jumper Wires
<li>WiFi and a smart phone/device with browser
<li>Power Supply(DC or AC)
 <br><br>
<b> Working and explanation:</b><br>
Let us the see the working of the nodemcu code and then the effect on website correspondingly.
 Firstly, when the nodemcu is powered on, We want it to disconnect to the existing connection (as it is off it will be none in this case). Now that everything is clean 
 we will be assigning the mode of our pins and make sure that all our devices are OFF at first. Now we want to operate over wifi, so we need to connect to the wifi,
 for that we need the ssid and password of our wifi to connect and that is included in the code. This makes it kind of limiting because we can't carry our device and connect 
 to a new network, we have to change the ssid,password and also if we accidently change the wifi password we need to edit the code again. To overcome this we can use dynamic wifi
 page and that's a bit excess but it can be added. However, I did not include this part in this project.<br><br>
 Now that we are connected to the wifi we want to know it physically. So there is something I added as Device 0 that tells us the current condition. I used an LED here, so after
 connecting to the wifi, Device 0 Blinks for 10 times. <br><br>
For all the configuring and creating servers we use WiFi libraries. Esp8266wifi for Handling espmodule in nodemcu, ESP8266wifiserver and Wificlient Libraries to create a webserver
and handling clients. Now using the functions from these libraries like server.on we can tell the code to do 'that' when 'this' action is performed on the server. Let's talk about it later.<br><br>
Now we gave the server the instructions to perform, we need to start the server. After starting it, to know it physically Device 0 glows with increasing brightness.We tell the nodemcu to handle the client constantly, so we put it in the loop. As explained before when the server is started something must happen. So first we reach the root(Home) page which is represented by '/' and when the server is in this page the function 'handleRoot' is executed. To physically know that we are in home page the Device 0 glows. We also want to know the status of our device whether they are ON/OFF. So we read their status from the pins they are connected to and decide whether they are active or not.
Two functions statusread and decision are called. Most of the times Devices are connected to the Relay with NO(Normally Open) terminal and common terminal. So the pins status is
slightly different from normal way i.e we assume that a high means ON and LOW means OFF. However when using a relay it will be reversed. After getting the status of all the devices we sent the data back to the server. HTTP status 200 means everything is OK and we send the html instructions to perform on the sever.<br><br>
Nothing was there on the page till now. All that happened is decision making in the nodemcu. Now we send all this data and html code that puts the webpage layout on the site.
We then see four buttons(+refresh i.e to get back to home) when we reach the site. If a button is pressed then the page which is at '..../' changes to '..../dev1' or'..../dev2' like that. When this page is reached on the server the corresponding function is executed. As we are no longer in the home page the Device 0 goes off.<br><br>
Initially all the devices are inactive, when we press a button then the corresponding device gets active. Now what if we press the button again, we want it to go off right! For that I used an if else condition that monitors if the device is already active or inactive. If it is already active then the button will result in Deactivating the device and vice-versa.The button's status is read and the decision is taken again and the data is sent to the server to put it on the page. The status of our devices must be independent of each other, so their status(variable that stores) is made global so that even if multiple device are active they remain independent.<br><br>
There is also a case when we can't reach our server. In that case a function 'handleNotFound' is made so that it prints the message from html accordingly.<br>
<b>YouTube Demo Link</b>: https://youtu.be/JlqAk_7v4O0 <br>
The website looks like this <br>
<img src="https://github.com/Ruthvik-1411/Device_Control_Using_Nodemcu_over_WiFi/blob/main/dcnw%20webpage%20layout.jpg"><br>
The .ino code and .html code can be found above along with some pictures of the project.
