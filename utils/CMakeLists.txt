CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_LIBRARY(utils STATIC
    inc/utils/fileoperations.h
    src/fileoperations.cpp
    inc/utils/functiontraits.h
    inc/utils/log.h
    inc/utils/numericoperations.h
    inc/utils/signal.h
    inc/utils/simplesubscriber.h
    inc/utils/stringoperations.h
    inc/utils/subscriber.h
    inc/utils/timeoperations.h
    inc/utils/trace.h
    inc/utils/types.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc)
SET (UTILS_LIBRARIES utils)

SET (CMAKE_CXX_FLAGS "-std=c++0x")
SET (CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Werror -Wfatal-errors -Wno-deprecated-declarations")

IF (UNIX OR APPLE)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCONSOLE_SUPPORTS_COLOR")
ENDIF (UNIX OR APPLE)

ADD_SUBDIRECTORY(test)
