cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	endif()
endif()

project(minfwver)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "MinFWVer")
set(VITA_TITLEID  "MINFW0000")
set(VITA_VERSION  "02.10")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(minfwver
	main.c
	scr_printf.c
	font.c
)

target_link_libraries(minfwver
	SceCtrl_stub
	SceDisplay_stub
	SceVshBridge_stub
	SceShellSvc_stub 
)

vita_create_self(eboot.bin minfwver UNSAFE)

vita_create_vpk(minfwver.vpk ${VITA_TITLEID} eboot.bin
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}
	
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/icon0.png                      sce_sys/icon0.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/bg.png       sce_sys/livearea/contents/bg.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/startup.png  sce_sys/livearea/contents/startup.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
