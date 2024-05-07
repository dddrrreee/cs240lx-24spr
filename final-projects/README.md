### Final projects


Some final project ideas:

 - [led earring](https://twitter.com/cnlohr/status/1760205375989760182?t=rpdSMSyzZ7YBwPwGIN8aVw&s=19).  Interesting project --- you'd need to boot up a small microcontroller and figure out packaging and battery.

<p align="center">
<img src="./led-earring.jpg" width="450" />
</p>


 - I haven't tried this so buyer beware but:
   XMR has a weird asic resistant mining method (you run weird little
   programs that touch a lot of memory).   It would be interesting if
   you could implement on the pi and maybe do vm hacks to speed up and/or
   use parthivs cluster board.

   No idea if feasible but looks interesting.

 - High-speed secondary memory interface (from akshay):

   The Pi has what's called a "secondary memory interface", which allows
   high-speed parallel communication between two Pis or a Pi and an
   external device.  This isn't documented in the Broadcom datasheet,
   except for some cryptic references in the alt mode table, but there
   are a couple of blog posts using it:

     - https://iosoft.blog/2020/07/16/raspberry-pi-smi/
     - https://iosoft.blog/2020/09/29/raspberry-pi-multi-channel-ws2812/

   Getting this working baremetal and making a really high-speed link
   between two Pis (or talking to something with a parallel interface,
   like a camera or LCD) would be a cool final project!

￼

  - (From Joe): We have 1 spare [pinetime](https://wiki.pine64.org/wiki/PineTime, https://pine64.com/product/pinetime-dev-kit/) and I'm sure can get more if people are interested and shipping permits

    The pinetime is an open source smartwatch with a touchscreen,
    heartrate monitor, vibration motor, button, accelerometer,  and
    bluetooth.

    It might be cool to wear a watch that you wrote every line of code
    for. Also has interesting problems like how to conserve battery
    while keeping it usable, etc.
￼
  - [Booting on a cheap microcontroller](https://hackaday.com/2023/02/16/puya-py32-the-cheapest-flash-microcontroller-you-can-buy-is-actually-an-arm-cortex-m0/)
  - Dunno if feasible but seems cool: 
    [Date recordings using eletrical noise](https://twitter.com/isaach/status/1628869010329837575?t=GAaYWN-bVkQfgIzBXDAd-Q&s=19)

<p align="center">
<img src="./date-images-with-audio.jpg" width="450" />
</p>
