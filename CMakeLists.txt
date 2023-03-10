cmake_minimum_required(VERSION 3.8.0)
project("softerer")

set (SOFTERER_ROOT_PROJDIR ${CMAKE_CURRENT_LIST_DIR})
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(Vendor/Tether)
add_subdirectory(Applications)

add_library(${PROJECT_NAME} STATIC "Source/Surface.cpp" "Source/Image.cpp" "Source/Platform/Win32Surface.cpp" "Source/Platform/Win32_FramebufferImage.cpp" "Source/Context.cpp" "Source/Queue.cpp"  "Source/Semaphore.cpp")

target_include_directories(${PROJECT_NAME}
PUBLIC
	Headers
)
