add_library(delaunay delaunay.cpp include/delaunay/delaunay.hpp)

set_property(TARGET delaunay PROPERTY CXX_STANDARD 20)
target_include_directories(delaunay PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

find_package(OpenCV REQUIRED)
target_link_libraries(delaunay PRIVATE opencv_core opencv_highgui)

install(TARGETS delaunay)