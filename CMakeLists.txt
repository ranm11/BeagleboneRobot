cmake_minimum_required(VERSION 3.15)

#include(${ANDROID_NDK_LOCATION}/build/cmake/android.toolchain.cmake)
#set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
#include(${CMAKE_TOOLCHAIN_FILE})
message("build For Platform ${BUILD_PLATFORM}")
if(${BUILD_PLATFORM} STREQUAL "ANDROID")
	set(ANDROID_TOOLCHAIN "clang")
	set(ANDROID_PLATFORM android-16)

	#For arm64 device
	#set(ANDROID_ABI "arm64-v8a")
	#set(ANDROID_ARM_MODE "arm")
	#
	#For Simulator
	set(ANDROID_ABI "x86_64")
	
  
	include( C:/Users/rminervy/git/Toolchains/toolchains/Android/android-ndk-r22b-windows-x86_64/android-ndk-r22b/build/cmake/android.toolchain.cmake)
	set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
		project (hello)
	#set(CMAKE_CXX_STANDARD 11)
	add_library(hello SHARED)
	set(thread_pool_src ${thread_pool_src} 
	"Android_src/helloworld.cpp"
	"Android_src/wrapper.cpp"
	)

	target_sources(hello PRIVATE ${thread_pool_src})
	
		
	set_target_properties(hello PROPERTIES CXX_STANDARD 17)

elseif(${BUILD_PLATFORM} STREQUAL "WIN")

project (thread_pool)
#set(CMAKE_CXX_STANDARD 11)
add_executable(thread_pool )
#target_sources(hello PRIVATE helloworld.cpp)
#target_compile_features(hello PUBLIC cxx_std_11)
#set_target_properties(hello PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
include_directories (thread_pool_src/gpio)
	include_directories (thread_pool_src/motor)
set(thread_pool_src ${thread_pool_src} 
	"thread_pool_src/BlockingQueue.h"
	"thread_pool_src/Command.cpp"
	"thread_pool_src/ParseCommand.cpp"
	"thread_pool_src/NetworkUdpReadCommand_win.cpp"
	"thread_pool_src/Command.h"
	"thread_pool_src/main.cpp"
	"thread_pool_src/ThreadPool.h"
	"thread_pool_src/ThreadPool.cpp"
	"thread_pool_src/Worker.cpp"
	"thread_pool_src/Worker.h"
	"thread_pool_src/SPIReadCommand.cpp"
	"thread_pool_src/SpiCommandSimulator.cpp"
	"thread_pool_src/EngineCommand.cpp"
	"thread_pool_src/SpoonCommand.cpp"
	"thread_pool_src/gpio/GPIO_mock.cpp"
	"thread_pool_src/gpio/util_mock.cpp"
	"thread_pool_src/gpio/PWM_mock.cpp"
	"thread_pool_src/motor/DCMotor.cpp"
	"thread_pool_src/motor/Servo.cpp"
	"thread_pool_src/bus/BusDevice.cpp"
	"thread_pool_src/bus/SPIDevice_mock.cpp"
	"thread_pool_src/WheelCommand.cpp"
	"thread_pool_src/rf24/RF24_mock.cpp"
	"thread_pool_src/rf24/compatibility_mock.c"
	"thread_pool_src/RF24ReadCommand.cpp"
	#"thread_pool_src/motor/StepperMotor.h"
	#"thread_pool_src/motor/StepperMotor.cpp"
	)
	target_sources(thread_pool PRIVATE ${thread_pool_src})
	set_target_properties(thread_pool PROPERTIES CXX_STANDARD 11)
elseif(${BUILD_PLATFORM} STREQUAL "BBB")
	include(BBB_toolchail/beaglebone_toolchain.cmake )
	include_directories (thread_pool_src/gpio)
	include_directories (thread_pool_src/motor)
	project (thread_pool)
	#set(CMAKE_CXX_STANDARD 11)
	#set (PROJECT_LIBS libEBBLibrary.a)
	#link_directories(gpio_lib)
	add_executable(thread_pool)
	set(thread_pool_src ${thread_pool_src} 
	"thread_pool_src/BlockingQueue.h"
	"thread_pool_src/Command.cpp"
	"thread_pool_src/ParseCommand.cpp"
	"thread_pool_src/NetworkUdpReadCommand.cpp"
	"thread_pool_src/Command.h"
	"thread_pool_src/main.cpp"
	"thread_pool_src/ThreadPool.h"
	"thread_pool_src/ThreadPool.cpp"
	"thread_pool_src/Worker.cpp"
	"thread_pool_src/Worker.h"
	"thread_pool_src/EngineCommand.cpp"
	"thread_pool_src/SpoonCommand.cpp"
	"thread_pool_src/SPIReadCommand.cpp"
	"thread_pool_src/gpio/GPIO.h"
	"thread_pool_src/gpio/GPIO.cpp"
	"thread_pool_src/gpio/PWM.cpp"
	"thread_pool_src/gpio/PWM.h"
	"thread_pool_src/gpio/util.cpp"
	"thread_pool_src/gpio/util.h"
	"thread_pool_src/motor/DCMotor.cpp"
	"thread_pool_src/motor/DCMotor.h"
	"thread_pool_src/motor/Servo.h"
	"thread_pool_src/motor/Servo.cpp"
	"thread_pool_src/motor/StepperMotor.h"
	"thread_pool_src/SpiCommandSimulator.cpp"
	"thread_pool_src/motor/StepperMotor.cpp"
	"thread_pool_src/bus/BusDevice.cpp"
	"thread_pool_src/bus/SPIDevice.cpp"
	"thread_pool_src/WheelCommand.cpp"
	"thread_pool_src/RF24ReadCommand.cpp"
	"thread_pool_src/rf24/RF24.cpp"
	"thread_pool_src/rf24/compatibility.c"
	"thread_pool_src/StickAnalogRead/AnalogRead.cpp"
	)

	target_sources(thread_pool PRIVATE ${thread_pool_src})
	target_compile_options(thread_pool PUBLIC "-pthread")
	target_link_libraries(thread_pool pthread)#link library
	#target_link_libraries(thread_pool ${PROJECT_LIBS})
		
	set_target_properties(thread_pool PROPERTIES CXX_STANDARD 17)
elseif(${BUILD_PLATFORM} STREQUAL "EMSCRIPTEN")
	message("Emscripten compile...")
	project(fetch_file)
	add_executable(${PROJECT_NAME} )
	target_sources(${PROJECT_NAME} PRIVATE upload_file_emscripten.cpp)
	
	target_link_options(${PROJECT_NAME} PRIVATE "SHELL: -lGL -lGLU -lglut -s FETCH  -s LEGACY_GL_EMULATION=1 -s WASM=1 -s EXPORTED_FUNCTIONS=['_main','_malloc','_free']")
	
install(
  TARGETS ${PROJECT_NAME}
)

install(FILES   ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.js 
                ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm
                DESTINATION .)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
    ${CMAKE_SOURCE_DIR}
    CACHE PATH "..." FORCE)
endif()
message("Cmake Exit")
endif()