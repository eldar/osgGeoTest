osgGeoTest contains example applications and data that use osgGeo libarary.

How to build.

git clone git@github.com:dgbtingdahl/osgGeo.git
git clone git@github.com:eldar/osgGeoTest.git
cd osgGeoTest
mkdir build
cd build
cmake ../
make

How to run.

Horizon example (assuming current directory osgGeoTest/build)
./horizon_test ../data/horizonF3.bin
