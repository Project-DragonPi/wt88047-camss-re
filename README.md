## Reverse Engneering On Wt88047 Camss

This repository include some stuff related to userspace camera driver on Redmi 2.

* `jni`  includes camera driver sources for wt88047 and QCDG.
*  `libs` includes some blobs generated from `jni`. (Only for reference)
* `output` includes some MIUI9 libmmcamera blobs info generated by QCDG.

## Status

* QCDG can print all data structure (exclude libchromatix stuff) in libmmcamera_XX.so
* libmmcamera sources in `jni` return same output in QCDG compare to libmmcamera blobs in MIUI9.

## Limitation

* For some reason,Libmmcamera blobs generated by android-ndk not work for now.
* QCDG (Qualcomm Camera Driver Generator) only works on MIUI 9(android 5.1).

## P.S

* Libmmcamera sources and some headers from amss are collected from other github repos.

