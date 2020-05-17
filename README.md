# Toolbox


## Board Bringup

Installing the bootloader for the first time:

```
brew install openocd --HEAD # to support STM32H7 need beyond 0.10.0
```

Format `/home`

```
sl fs.format:path=device@/home
```

The format command might say that it fails. It needs 1 to 2 minutes to complete before trying again.

Copy icon.bmp:

```
sl fs.copy:source=host@StratifyToolbox/assets/icon.bmp,dest=device@/home/icon.bmp
```

```
openocd -f StratifyToolbox/stlink-fixed.cfg -f target/stm32h7x.cfg -c "program ./StratifyToolbox/build_boot_debug/StratifyToolbox.bin 0x08000000; reset run; exit;"
```

Stratify Toolbox BSP

```
sl os.invokebootloader
sl task.signal:id=1,signal=ALARM
```

```
sl os.build:path=StratifyToolbox,build=debug os.process:path=StratifyToolbox,build=debug,padding=524256,hash
sl fs.write:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/drive1,pagesize=256,blank=255
sl fs.verify:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/drive1
sl os.reset
```

```
sl os.invokebootloader
sl fs.write:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/ramdrive,pagesize=2048
sl task.signal:id=1,signal=CONT
```

```
sl os.invokebootloader fs.write:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/ramdrive,pagesize=2048 task.signal:id=1,signal=CONT
```


```
sl --eraseFlashOs && sl --buildProcessFlashOs && sl --writeFlashOs
```


Crash Analysis:

```
sl debug.analyze:os=../StratifyBSP/StratifyToolbox,osBuild=debug,app=toolboxtool
```

Scanning the internal I2C Bus:

```
sl cloud.install:id=2Cd5njdQwfvvxB1yJTK1
sl app.run:path=i2ctool,terminal,args='--action=scan --port=2'
```

## Fonts and Icons

Convert fonts and icons to UX format:

```
cd assets
uxtool --action=process
cd ..
sl fs.copy:source=host@assets/device,dest=device@/home #copy to device
```


