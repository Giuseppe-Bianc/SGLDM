include(GenerateExportHeader)

find_package(glm REQUIRED)
add_library(Core Core.cpp)

add_library(SGLDM::Core ALIAS Core)

# Retrieve the target name
get_target_property(target_name Core NAME)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  target_compile_options(${target_name} PRIVATE -march=native)
  message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${target_name} PRIVATE -march=native)
  message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
  target_compile_options(${target_name} PRIVATE /arch:AVX2)
  message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
else ()
  message(STATUS "Impossible to set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif ()


target_link_libraries(Core PRIVATE
        SGLDM_options
        SGLDM_warnings)
target_link_libraries(Core PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
        Glad
        glfw
)

target_include_directories(Core ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(Core PUBLIC cxx_std_20)

set_target_properties(
        Core
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(Core EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/SGLDM/core_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(Core PUBLIC CORE_STATIC_DEFINE)
endif()