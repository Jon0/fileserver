set(MODULE_NAME "template")
add_definitions(-std=c++14)

# main library cpp files
file(GLOB SRC_FILES
	"*.cpp"
)

file(GLOB INCLUDE_FILES
	"*.h"
)

add_library(
	${MODULE_NAME}
	SHARED
	${SRC_FILES}
	${INCLUDE_FILES}
)

target_link_libraries(
	${MODULE_NAME}
)
