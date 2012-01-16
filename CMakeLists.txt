project(osgGeoTest)

cmake_minimum_required(VERSION 2.6)

add_subdirectory( ../osgGeo osgGeoBuild)

include_directories(
    ${OSG_INCLUDE_DIR}
    ${OSGGEO_INCLUDE_DIR}
)

add_executable(horizon_test horizon_test.cpp)

target_link_libraries(horizon_test
    ${OSG_LIBRARY}
    ${OPENTHREADS_LIBRARY}
    ${OSGUTIL_LIBRARY}
    ${OSGVIEWER_LIBRARY}
    osgGeo
)
