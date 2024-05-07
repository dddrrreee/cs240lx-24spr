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

  - [EKG input](https://twitter.com/ingarobotics/status/1635486902068584448?t=XhXbmr6DiyfUDR1IwZVU0Q&s=19)

<p align="center">
<img src="./ekg.jpg" width="450" />
</p>

  - [Water clock](https://twitter.com/Rainmaker1973/status/1638255091726577664?t=_bQHFrQQ8swiuk2H27Ik8g&s=19)

<p align="center">
<img src="./water-clock.jpg" width="450" />
</p>

  - make a pi cluster like (https://twitter.com/ExtElec/status/1641839012976422916?t=BIczeyLC6jGq1SPQY2ovRw&s=19).  Parthiv made cluster boards.

<p align="center">
<img src="./pi-cluster.jpg" width="450" />
</p>

  - [pi watch](https://www.raspberrypi.com/news/how-to-build-your-own-raspberry-pi-watch/)

<p align="center">
<img src="./pi-watch.jpg" width="450" />
</p>

  - [nixie light controller](https://www.hackster.io/news/charles-lohr-turns-a-0-10-risc-v-microcontroller-into-a-software-defined-flyback-for-nixie-tubes-aabafa76aa78)

<p align="center">
<img src="./nixie.jpg" width="450" />
</p>

  - [ethernet](https://vanhunteradams.com/Pico/Ethernet/UPD_TX.html)

<p align="center">
<img src="./pi-ethernet.jpg" width="450" />
</p>

  - [Leaking bits using PMU](https://www.zdnet.com/article/academics-turn-pc-power-unit-into-a-speaker-to-leak-secrets-from-air-gapped-systems/)


<p align="center">
<img src="./pmu-xmit.jpg" width="450" />
</p>

  - [realtime magnetic imaging](https://twitter.com/giammaiot2/status/1658066614225436672?t=6f5wfOGh8VAopwXZLIIL0g&s=19)

<p align="center">
<img src="./realtime-magnetic-field.jpg" width="450" />
</p>

  - [self balancing platform](https://twitter.com/Rainmaker1973/status/1658045259287343106?t=bqFBhwwcl_iN5KdP_BN1sA&s=19)

<p align="center">
<img src="./self-balance-platform.jpg" width="450" />
</p>

  - Use multiple cheap displays to make a [solid state volumetric display](https://twitter.com/idlehandsdev/status/1661343789527490560?t=Nu2ctoQwNTfaGB15K8XY_A&s=19).  
     
<p align="center">
<img src="./ss-volumetric.jpg" width="450" />
</p>

  - [networked subwoofer](https://www.audiokinesis.com/the-swarm-subwoofer-system-1.html)

<p align="center">
<img src="./networked-subwoofer.jpg" width="450" />
</p>

  - [cute signal jammer](https://twitter.com/giammaiot2/status/1662541683404206080?t=k-eKGufcn9Yc_oGn5CCbUw&s=19)

<p align="center">
<img src="./signal-jammer.jpg" width="450" />
</p>

  - [RF or wifi scanner](https://twitter.com/lozaning/status/1663575202461110273?t=7YQENE0C9Qn2iungYQ53CQ&s=19)

<p align="center">
<img src="./signal-jammer.jpg" width="450" />
</p>

  - [Art display](https://twitter.com/cryptograffiti/status/1663237774500036625?t=W4NET2jMKwsw3LcKz5bDzA&s=19)

<p align="center">
<img src="./art-display.jpg" width="450" />
</p>

  - [clean little oscilliscope](https://hackaday.com/2023/02/27/diy-stm32-scope-is-simple-cheap-and-featureful/)

<p align="center">
<img src="./clean-oscilliscope.jpg" width="450" />
</p>

  - [pi raid](https://www.geeky-gadgets.com/raspberry-pi-nas-60-hhds-23-06-2022/)

<p align="center">
<img src="./pi-raid.jpg" width="450" />
</p>

  - [wireless mouse](https://www.hackster.io/news/wireless-mouse-ring-interface-d34ef0beb831)

<p align="center">
<img src="./wireless-mouse.jpg" width="450" />
</p>

  - OS on ipod mini: [some starter code](https://github.com/dwelch67/ipod_mini)

  - [Make a tiny bootloader](https://twitter.com/cnlohr/status/1635082283798659072?t=uw8rDkMPdz1VJdoLMNzjJA&s=19)

<p align="center">
<img src="./small-bootloader.jpg" width="450" />
</p>

  - Fun music: tactile mini.

    https://twitter.com/diyelectromusic/status/1632505624222224384?t=kbn3nrGA_E6khPO6IEUlPw&s=19
    https://twitter.com/diyelectromusic/status/1634928271673667584?t=oR7l8HI1uOoBXodx11zYPw&s=19

<p align="center">
<img src="./tactile-mini.jpg" width="450" />
</p>
