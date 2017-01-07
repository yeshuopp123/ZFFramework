project should follow this structure:

(`~` stands for root path of ZFFramework)

```
// for ZF libs
~/LibName/
    zfproj/
        Android/
            LibName_jni/
        iOS/
            LibName/
                LibName.xcodeproj/
        Qt/
            LibName/
                LibName.pro
    zfres/
    zfsrc/

// for ZF impl libs
~/ImplLibName/
    zfproj/
        Android/
            ImplLibName/
            ImplLibName_jni/
        iOS/
            ImplLibName/
                ImplLibName.xcodeproj/
        Qt/
            ImplLibName/
                ImplLibName.pro
    zfres/
    zfsrc/
```

