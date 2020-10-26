cmake_minimum_required(VERSION 3.4.1)
add_library(seanffmpeg
        SHARED
        native-lib.cpp)
find_library(log-lib
        log)
find_library(android-lib
        android)
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/armeabi-v7a)


add_library(avutil-55
        SHARED
        IMPORTED)
set_target_properties(avutil-55
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libavutil-55.so)

add_library(swresample-2
        SHARED
        IMPORTED)
set_target_properties(swresample-2
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libswresample-2.so)

add_library(avcodec-57
        SHARED
        IMPORTED)
set_target_properties(avcodec-57
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libavcodec-57.so)

add_library(avfilter-6
        SHARED
        IMPORTED)
set_target_properties(avfilter-6
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libavfilter-6.so)

add_library(swscale-4
        SHARED
        IMPORTED)
set_target_properties(swscale-4
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libswscale-4.so)

add_library(avformat-57
        SHARED
        IMPORTED)
set_target_properties(avformat-57
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libavformat-57.so)


set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
include_directories(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/include)

target_link_libraries(seanffmpeg
        avutil-55       #工具库
        swresample-2    #音频采样数据格式转换
        avcodec-57      #编解码
        avfilter-6      #滤镜特效处理
        swscale-4       #视频像素数据格式转换
        avformat-57     #封装格式处理
        ${log-lib})