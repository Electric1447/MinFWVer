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
set(VITA_VERSION  "01.10")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

link_directories(
	${CMAKE_BINARY_DIR}/minfwver_kernel_stubs
	${CMAKE_BINARY_DIR}/minfwver_user_stubs
)

add_executable(minfwver
	main.c
	scr_printf.c
	font.c
)

add_dependencies(minfwver minfwver_kernel.skprx)

target_link_libraries(minfwver
	SceCtrl_stub
	SceDisplay_stub
	SceVshBridge_stub
	taihen_stub
	minfwver_user_stub_weak
)

vita_create_self(eboot.bin minfwver UNSAFE)

add_executable(minfwver_kernel
	kernel.c
)

set_target_properties(minfwver_kernel
	PROPERTIES LINK_FLAGS
	-nostdlib
)

target_link_libraries(minfwver_kernel
	SceSysrootForKernel_stub
)

vita_create_self(minfwver_kernel.skprx minfwver_kernel
	CONFIG ${CMAKE_SOURCE_DIR}/kernel_exports.yml
	UNSAFE
)

vita_create_stubs(minfwver_kernel_stubs minfwver_kernel ${CMAKE_SOURCE_DIR}/kernel_exports.yml KERNEL)

add_executable(minfwver_user
	user.c
)

set_target_properties(minfwver_user
	PROPERTIES LINK_FLAGS
	-nostartfiles
)

target_link_libraries(minfwver_user
	minfwver_kernel_stub
)

vita_create_self(minfwver_user.suprx minfwver_user
	CONFIG ${CMAKE_SOURCE_DIR}/user_exports.yml
	UNSAFE
)

vita_create_stubs(minfwver_user_stubs minfwver_user ${CMAKE_SOURCE_DIR}/user_exports.yml)

add_dependencies(minfwver_user minfwver_kernel_stubs)
add_dependencies(minfwver minfwver_kernel.skprx)
add_dependencies(minfwver minfwver_user_stubs)
add_dependencies(minfwver minfwver_user.suprx)

vita_create_vpk(minfwver.vpk ${VITA_TITLEID} eboot.bin
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}
	
	FILE ${CMAKE_BINARY_DIR}/minfwver_kernel.skprx minfwver_kernel.skprx
	FILE ${CMAKE_BINARY_DIR}/minfwver_user.suprx   minfwver_user.suprx
	
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/icon0.png                      sce_sys/icon0.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/bg.png       sce_sys/livearea/contents/bg.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/startup.png  sce_sys/livearea/contents/startup.png
	FILE ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
