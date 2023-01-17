find_path(GLFW_INCLUDE_DIRS
	NAMES glfw3.h
	PATH_SUFFIXES GLFW
)

find_library(GLFW_LIBRARIES
	NAMES glfw glfw3
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_INCLUDE_DIRS GLFW_LIBRARIES)
