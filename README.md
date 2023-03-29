# BeagleboneRobot

this code is a cross platform design,
can be compiled for the beaglebone black embeded libux board 
and for windows for debugging and testing.


compile for  Windows platform (only for debug and initial expierience purpose) :
1. mkdir win_build
2. cd win_build 
3. cmake  ..  -DBUILD_PLATFORM="WIN"
4. cmake --build .

compile for Beaglebone black microcontroler:
1. download and install toolchain for beaglebone black from : https://gnutoolchains.com/beaglebone/
2. updte sysroot path and compiler path in the BBB_toolchain/beaglebone_toolchain.cmake
3. mkdir bbb_build 
4. cd bbb_build
5. cmake .. -G "Unix Makefiles"   -DBUILD_PLATFORM="BBB"               OR    cmake .. -G "Ninja"   -DBUILD_PLATFORM="BBB" 
6. cmake --build .


Network is supported within 3 channels :
1. udp channel
2. SPI channel 
3. RF24 wirless channel


Hardware components list:

 n20 DC motors  - 2 pcs : https://www.aliexpress.com/item/33022320164.html?spm=a2g0o.order_list.order_list_main.4.6c7f1802XEEMoE
 
 rechargble lithium battery : https://www.aliexpress.com/item/1005001310695209.html?spm=a2g0o.order_list.order_list_main.19.6c7f1802XEEMoE
 
 optional : Type-C lithium battery charger :https://www.aliexpress.com/item/1005001961975062.html?spm=a2g0o.order_list.order_list_main.50.6c7f1802XEEMoE
 
 Wireless NRF24L01 : https://www.aliexpress.com/item/4001136122641.html?spm=a2g0o.order_list.order_list_main.15.6c7f1802XEEMoE
 
 Motor Mx1508 Driver : https://www.aliexpress.com/item/1005001636421978.html?spm=a2g0o.order_list.order_list_main.100.6c7f1802XEEMoE
 Microcontroller: Beaglebone black 
 
 for the controller :
 Dual Axis Stick : https://www.aliexpress.com/item/32701378948.html?spm=a2g0o.order_list.order_list_main.94.6c7f1802XEEMoE
