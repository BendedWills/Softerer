cmake_minimum_required(VERSION 3.8.0)
project("Triangle")

add_executable(${PROJECT_NAME} "Source/Main.cpp")
add_dependencies(${PROJECT_NAME} tether softerer)

target_include_directories(${PROJECT_NAME}
PUBLIC
	${SOFTERER_ROOT_PROJDIR}/Headers
	${SOFTERER_ROOT_PROJDIR}/Vendor/Tether/Headers
)

target_link_libraries(${PROJECT_NAME} tether softerer)
