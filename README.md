# ESP32 CoreDump to Flash with Arduino framework 1.0.6!

# 1) Create an Arduino project with PlatformIO
Your "platformio.ini" file should have at least these lines:
>platform = espressif32
>framework = arduino
>board_build.partitions = partitions.csv

(like in the repo) 

# 2) Add the partition file
Copy the "partitions.csv" file to the root of your project. (like in the repo) 

# 3) Build your project
Either with the "PlatformIO Build" button or the "pio run" command.

# 4) Update your package folder
1- Go to "/home/yourName/.platformio/packages/framework-arduinoespressif32" folder.
2- In the root of the repo, you can find the "out_01.zip" file, unzip the content of the "/out" folder in the "framework-arduinoespressif32" folder.
# 5) Clean the build and build again
1) Either with the "PlatformIO Clean" button or the "pio run -t clean" command.
2) Either with the "PlatformIO Build" button or the "pio run" command.
# 6) Example application
You can find in /src/main.cpp, an example application. This code should crash after 10s and print the CoreDump in the setup() function.
Run it and wait for the crash. At the next boot, you will see in the serial console the bytes of the coreDump:
>4c160000010000000700000064010000
>5880fb3fd01efb3f5420fb3f701efb3ff01
>ffb3f2d270000b00dfc3fb00dfc3f5880fb
>......
>3fa80dfc3f18000000147efb3f147efb3f5
>880fb3f00000000010000005800fb3f6c
>0000000000000000000000000000000

Copy it and paste the base64 conversion to a file called "coreDump.b64" in the root of your project.
# 7) Install esp-idf, xtensa and libncurses5 
## 1- Install esp-idf
As described in the website, install esp-idf: https://docs.espressif.com/projects/esp-idf/en/release-v3.3/get-started/index.html#get-esp-idf
>cd ~/esp

>git clone -b release/v3.3 --recursive https://github.com/espressif/esp-idf.git

>cd esp-idf

>. ./install.sh

>. ./export.sh

>git submodule update --init --recursive

>python -m pip install --user -r $IDF_PATH/requirements.txt

## 2- Install xtensa
Download package (link to the website here:https://docs.espressif.com/projects/esp-idf/en/release-v3.3/get-started/linux-setup.html):
-   for 64-bit Linux:
    [https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz](https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz)
    
-   for 32-bit Linux:
    [https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-97-gc752ad5-5.2.0.tar.gz](https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-97-gc752ad5-5.2.0.tar.gz)

Unzip it :
-   for 64-bit Linux:
    >cd ~/esp
    
    >tar -xzf ~/Downloads/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz
    
-   for 32-bit Linux:
    >cd ~/esp
    
    >tar -xzf ~/Downloads/xtensa-esp32-elf-linux32-1.22.0-97-gc752ad5-5.2.0.tar.gz


## 3-Add path to your profile
>nano ~/.profile

Add these lines to the bottom of the file and save it :
>export PATH="\$HOME/esp/xtensa-esp32-elf/bin:\$PATH"
>export IDF_PATH=~/esp/esp-idf
>export PATH="\$IDF_PATH/tools:\$PATH"
>export PATH="\$IDF_PATH/components/espcoredump:\$PATH"

Reload the profile with the following command :
> source ~/.profile

## 4-libncurses5 
On Linux, you must install libncurses5 with the following command:
>sudo apt-get install libncurses5

# 8) Decode your coreDump
1- Open a terminal in your project folder.
2- To be sure that the path is loaded, you can reload it manually with:
>source ~/.profile

3- Decode the coreDump with this command:
>espcoredump.py info_corefile -c ./coreDump.txt -t b64 -rom-elf ./.pio/build/esp32dev/firmware.elf
# 9) Understanding the output
Here is an example output: 

<details>
  <summary>Click to expand!</summary>
  
  >espcoredump.py v0.3-dev
===============================================================
==================== ESP32 CORE DUMP START ====================
================== CURRENT THREAD REGISTERS ===================
pc             0x400d0dd4	0x400d0dd4 <loop()+60>
lbeg           0x400014fd	1073747197
lend           0x4000150d	1073747213
lcount         0xffffffff	4294967295
sar            0x1e	30
ps             0x60720	395040
threadptr      <unavailable>
br             <unavailable>
scompare1      <unavailable>
acclo          <unavailable>
acchi          <unavailable>
m0             <unavailable>
m1             <unavailable>
m2             <unavailable>
m3             <unavailable>
expstate       <unavailable>
f64r_lo        <unavailable>
f64r_hi        <unavailable>
f64s           <unavailable>
fcr            <unavailable>
fsr            <unavailable>
a0             0x400d2258	1074602584
a1             0x3ffb1f90	1073422224
a2             0x0	0
a3             0x3ffc0280	1073480320
a4             0x20	32
a5             0x80000020	-2147483616
a6             0x8	8
a7             0x1	1
a8             0x800d0dcf	-2146628145
a9             0x3ffb1f70	1073422192
a10            0xc	12
a11            0x3f400132	1061159218
a12            0x4	4
a13            0x3	3
a14            0x3	3
a15            0x0	0
==================== CURRENT THREAD STACK =====================
#0  0x400d0dd4 in loop () at src/main.cpp:30
#1  0x400d2258 in loopTask (pvParameters=<optimized out>) at /home/mathieu/.platformio/packages/framework-arduinoespressif32/cores/esp32/main.cpp:23
#2  0x40086f40 in vPortTaskWrapper (pxCode=0x400d2240 <loopTask(void*)>, pvParameters=0x0) at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/freertos/port.c:143
======================== THREADS INFO =========================
  Id   Target Id         Frame 
  7    process 6         0x40081e08 in esp_crosscore_int_send_yield (core_id=0) at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/crosscore_int.c:117
  6    process 5         0x40081d91 in esp_crosscore_int_send (core_id=<optimized out>, reason_mask=<optimized out>) at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/crosscore_int.c:109
  5    process 4         0x40081da1 in esp_crosscore_int_send (core_id=<optimized out>, reason_mask=<optimized out>) at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/crosscore_int.c:111
  4    process 3         0x40081e08 in esp_crosscore_int_send_yield (core_id=0) at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/crosscore_int.c:117
  3    process 2         0x400ea1ea in esp_pm_impl_waiti () at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/pm_esp32.c:492
  2    process 1         0x400ea1ea in esp_pm_impl_waiti () at /home/mathieu/Bureau/lib_builder/esp32-arduino-lib-builder/esp-idf/components/esp32/pm_esp32.c:492
>* 1    <main task>       0x400d0dd4 in loop () at src/main.cpp:30

>======================= ALL MEMORY REGIONS ========================
Name   Address   Size   Attrs
.rtc.text 0x400c0000 0x0 RW  
.rtc.dummy 0x3ff80000 0x0 RW  
.rtc.force_fast 0x3ff80000 0x0 RW  
.rtc_noinit 0x50000200 0x0 RW  
.rtc.force_slow 0x50000200 0x0 RW  
.iram0.vectors 0x40080000 0x400 R XA
.iram0.text 0x40080400 0xb900 RWXA
.dram0.data 0x3ffbdb60 0x26dc RW A
.noinit 0x3ffc023c 0x0 RW  
.flash.rodata 0x3f400020 0xc82c RW A
.flash.text 0x400d0018 0x1a46c R XA
.coredump.tasks.data 0x3ffb8058 0x164 RW 
.coredump.tasks.data 0x3ffb1ed0 0x184 RW 
.coredump.tasks.data 0x3ffbc30c 0x164 RW 
.coredump.tasks.data 0x3ffbc150 0x1a8 RW 
.coredump.tasks.data 0x3ffbc890 0x164 RW 
.coredump.tasks.data 0x3ffbc6d0 0x1ac RW 
.coredump.tasks.data 0x3ffbd318 0x164 RW 
.coredump.tasks.data 0x3ffbd140 0x1c4 RW 
.coredump.tasks.data 0x3ffba3d8 0x164 RW 
.coredump.tasks.data 0x3ffba1e0 0x1e4 RW 
.coredump.tasks.data 0x3ffb9e54 0x164 RW 
.coredump.tasks.data 0x3ffb9c60 0x1e0 RW 
.coredump.tasks.data 0x3ffb796c 0x164 RW 
.coredump.tasks.data 0x3ffb7790 0x1c8 RW 
===================== ESP32 CORE DUMP END =====================
===============================================================
Done!
</details>

There is a lot of details that you do not necessarily need to understand why the code crashed.
I usually go to the part with:
>==================== CURRENT THREAD STACK =====================

And I try to find a function name that I have in my program. In that case, the crash occured in the loop() function at line 30.
>#0  0x400d0dd4 in loop () at src/main.cpp:30

At line 30 of my program, we can find this line:
>27: Serial.println("RESTART ::");
28: int  a = 0;
29: int  b = 4;
**30: Serial.printf("%d\n", b / a);**
31: }

# And there you go !

