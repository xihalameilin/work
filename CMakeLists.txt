cmake_minimum_required(VERSION 2.8)

PROJECT(LaneDetection_Demo)

FIND_PACKAGE(OpenCV REQUIRED)

SET(SRC
 main.cpp
)

execute_process(COMMAND sh ../GPIO.sh demo.cpp)

SET(PROJECT_NAME
 demo
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${OpenCV_LIBS})
