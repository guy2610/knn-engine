cmake_minimum_required(VERSION 3.20)

project(
    knn_engine
    VERSION 0.1.0
    DESCRIPTION "A high-performance KNN and nearest-neighbor engine"
    LANGUAGES CXX
)

include(cmake/ProjectWarnings.cmake)

add_library(knn_engine INTERFACE)

target_compile_features(knn_engine INTERFACE cxx_std_20)

target_include_directories(
    knn_engine
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

include(CTest)

if(BUILD_TESTING)
    add_executable(toolchain_smoke tests/toolchain_smoke.cpp)

    target_link_libraries(
            toolchain_smoke
            PRIVATE
            knn_engine
            project_warnings
    )

    add_test(
        NAME toolchain_smoke
        COMMAND toolchain_smoke
    )
endif()
