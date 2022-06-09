# Set the system name
set(CMAKE_SYSTEM_NAME Linux)
# C Compiler
set(COMPILER_PATH C:/SysGCC/Beaglebone/bin)
set(COMPILER_FRONT arm-linux-gnueabihf-)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER ${COMPILER_PATH}/${COMPILER_FRONT}gcc.exe)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/${COMPILER_FRONT}g++.exe)
# Sysroot location
# It is mounted using sshfs
set(CMAKE_SYSROOT C:/SysGCC/Beaglebone/arm-linux-gnueabihf/sysroot)
# These lines are necessary to let cmake know to only look in the
# beaglebone for the libraries instead of looking in host pc.
# sshfs debian@192.168.7.2:/ /home/kalyan/Beaglegone-Sysroot/ -o transform_symlinks
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
