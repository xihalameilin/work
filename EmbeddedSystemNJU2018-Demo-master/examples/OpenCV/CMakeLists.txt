cmake_minimum_required(VERSION 2.8)

PROJECT(LaneDetection_Demo)

FIND_PACKAGE(OpenCV REQUIRED)

SET(SRC
 main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/Basic Principle/GPIO/servo.cpp)
include_directories(${PROJECT_SOURCE_DIR}/lib/GPIOlib.h)

SET(PROJECT_NAME
 demo
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${OpenCV_LIBS})
