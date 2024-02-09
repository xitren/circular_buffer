cmake_minimum_required(VERSION 3.16)

set(LIBRARY_NAME
    circullar_buffer
)

set(GLOBAL_NAMESPACE
    xitren
)

project(${LIBRARY_NAME}
	DESCRIPTION "Circullar buffer library"
    VERSION 1.0.0
	LANGUAGES CXX
)

include(GNUInstallDirs)
add_library(${LIBRARY_NAME} INTERFACE)
add_library(xitren::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME}
        INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

message(STATUS "building project ${LIBRARY_NAME}")

target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_20)

# Install
install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${LIBRARY_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
if(NOT INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR include)
endif()

install(EXPORT ${LIBRARY_NAME}_Targets
    FILE ${LIBRARY_NAME}Targets.cmake
    NAMESPACE xitren::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${LIBRARY_NAME}/cmake
)

install(
    FILES "${PROJECT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${LIBRARY_NAME}/cmake
)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(GTest)

#if(GTEST_LIBRARY)

enable_testing()
include(GoogleTest)

file(GLOB EXAMPLES tests/*.cpp)

foreach(file ${EXAMPLES})
    get_filename_component(tgt ${file} NAME_WE)
    message(STATUS "Adding ${tgt} example")
    add_executable(${tgt} ${file})
    target_compile_features(${tgt} INTERFACE cxx_std_20)
    target_include_directories(${tgt}
            PRIVATE
                ${LIBRARY_NAME}  ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${tgt} PRIVATE ${LIBRARY_NAME} GTest::gtest_main -pthread)
    gtest_add_tests(TARGET ${tgt} AUTO)
endforeach()

#endif()
