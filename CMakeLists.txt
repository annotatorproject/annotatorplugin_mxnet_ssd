
# 
# External dependencies
# 

execute_process(COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mxnet/amalgamation)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mxnet/lib/libmxnet_predict.so DESTINATION ${PROJECT_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(mxnet REQUIRED)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
set_policy(CMP0020 NEW)


# 
# Executable name and options
# 

# Target name
set(target mxnet_ssd)

# Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND)
    message(STATUS "Plugin ${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "Plugin ${target}")
endif()


# 
# Sources
# 

file(GLOB sources
"src/*.cpp" "src/*.h" "src/*.hpp" "src/*.ui" "src/*.qrc")


# 
# Create executable
# 

# Build executable
add_library(${target}
    ${sources}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}/source/include
    ${mxnet_INCLUDE_DIRS}
)


# 
# Libraries
# 

#if(NOT ${mxnet_LIBRARIES})
#    set( mxnet_LIBRARIES mxnet_predict)
#    message(STATUS "mxnet library: " ${mxnet_LIBRARIES} )
#endif()

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${mxnet_LIBRARIES}
    annotator::annotatorplugin
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}/plugins    COMPONENT plugins
    LIBRARY DESTINATION ${INSTALL_SHARED}/plugins COMPONENT plugins
    BUNDLE  DESTINATION ${INSTALL_BIN}/plugins COMPONENT plugins
)
