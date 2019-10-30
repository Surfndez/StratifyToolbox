# Toolbox
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
sl fs.write:source=host@StratifyToolbox/build_debug/TangoBSP.bin,dest=device@/dev/ramdrive,pagesize=2048
sl task.signal:id=1,signal=CONT
```
