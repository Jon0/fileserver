add_definitions(-std=c++14)

# executable files
file(GLOB MAIN_FILES
	"*.cpp"
)

set(EXECUTABLE_NAME "start")
add_executable(${EXECUTABLE_NAME} ${MAIN_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${CORE_LIBRARY_NAME} ${MAIN_LIBRARY_NAME})
