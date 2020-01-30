cmake_minimum_required(VERSION 3.13)
project(FaceControl)
set(CMAKE_CXX_STANDARD 14)
add_executable(FaceControl main.cpp FaceDetector.cpp FaceDetector.h LineChart.cpp LineChart.h)

# -----qt 配置-----
# Mac上只需要brew install qt即可
IF (APPLE)
    MESSAGE("On Mac")
    set(Qt_Dir /usr/local/Cellar/qt/5.12.2)
ELSEIF (WIN32)
    # 在windows下，在Qt官网上下载二进制包进行安装，此外：
    # 1. 一定要使用Qt里面Tools的mingw，而不是其他的mingw，否则可能找不到库，因为这里mingw是64位的
    # 2. 可能闪退，此时找到cmake-build里面的二进制文件，打开看看，有可能是QtWidgetXXX.dll没有找到这种错误，需要加环境变量
    #    比如加：C:\Qt\Qt5.13.2\5.13.2\mingw73_64\bin
    #    注意加完一定要重启clion，才能应用新的环境变量
    MESSAGE("On Win")
    set(Qt_Dir C:/Qt/Qt5.13.2/5.13.2/mingw73_64) # qt二进制文件安装后的路径，要选择mingw
ENDIF ()
set(CMAKE_PREFIX_PATH ${Qt_Dir}/lib/cmake)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)
find_package(Qt5Gui)
target_link_libraries(FaceControl Qt5::Widgets)


# -----dlib相关-windows下-----
# 下载dlib即可，其根目录下的CMakeLists就是编译example的，所以可以按照里面的内容copy过来修改
add_subdirectory(C:/Users/Administrator/Documents/dlib dlib_bulid)
target_link_libraries(FaceControl dlib::dlib)

# -----opencv相关-windows下预编译-----
# 预编译版本直接使用，比如在https://github.com/huihut/OpenCV-MinGW-Build下载预编译版本
set(OPENCV_PATH C:\\Users\\Administrator\\Documents\\OpenCV-MinGW-Build-OpenCV-4.1.1-x64)
# 找到所有.a文件，存储到ALL_OPENCV_LIBS
FILE(GLOB ALL_OPENCV_LIBS ${OPENCV_PATH}/lib/*.a)
include_directories(${OPENCV_PATH}/include)
link_directories(${OPENCV_PATH}/lib)
link_libraries(${ALL_OPENCV_LIBS})

#target_link_libraries(FaceControl ${OPENSSH_PATH}/lib/libcrypto.a)

# -----opencv相关-windows下-----
# opencv官网下载安装解压，然后需要使用cmake进行编译，编译完成后设置好路径（注意编译好过后不能移动文件夹，否则需要重新编译）
# 注意两个易错点，一个是根据报错提示设置OpenCV_DIR或者Opencv_DIR变量，另一个是在OpencvDir路径下把OpenCVConfig.cmake重命名为报错中提示的需要的OpencvConfig.cmake
# 可以参考https://www.jianshu.com/p/4e1f1edabfda编译
#set(Opencv_DIR C:\\Users\\Administrator\\Documents\\opencv\\build)
#find_package(Opencv REQUIRED)
## 找到过后就有了OpenCV_LIBS等变量，进行设置
#INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(FaceControl ${OpenCV_LIBS})

# -----opencv相关-mac下-----
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIBRARY_DIRS})
#target_link_libraries(FaceControl ${OpenCV_LIBS})


# -----dlib相关-mac下-----
#add_subdirectory(/Users/mac/Documents/dlib dlib_build)
#target_link_libraries(FaceControl dlib::dlib)


