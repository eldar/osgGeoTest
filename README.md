osgGeoTest
==========


This repository contains example applications and data that use osgGeo libarary.

How to build.
-------------

```bash
    git clone git@github.com:dgbtingdahl/osgGeo.git
    git clone git@github.com:eldar/osgGeoTest.git
    cd osgGeoTest
    mkdir build
    cd build
    cmake ../
    make
```

How to run.
-----------

Horizon example (assuming current directory osgGeoTest/build)

```bash
    ./horizon_test ../data/horizonF3.bin
```
