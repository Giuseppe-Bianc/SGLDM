include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(SGLDM_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(SGLDM_setup_options)
  option(SGLDM_ENABLE_HARDENING "Enable hardening" ON)
  option(SGLDM_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    SGLDM_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    SGLDM_ENABLE_HARDENING
    OFF)

  SGLDM_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR SGLDM_PACKAGING_MAINTAINER_MODE)
    option(SGLDM_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(SGLDM_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(SGLDM_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(SGLDM_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(SGLDM_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(SGLDM_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(SGLDM_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(SGLDM_ENABLE_PCH "Enable precompiled headers" OFF)
    option(SGLDM_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(SGLDM_ENABLE_IPO "Enable IPO/LTO" ON)
    option(SGLDM_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(SGLDM_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(SGLDM_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(SGLDM_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(SGLDM_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(SGLDM_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(SGLDM_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(SGLDM_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(SGLDM_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(SGLDM_ENABLE_PCH "Enable precompiled headers" OFF)
    option(SGLDM_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      SGLDM_ENABLE_IPO
      SGLDM_WARNINGS_AS_ERRORS
      SGLDM_ENABLE_USER_LINKER
      SGLDM_ENABLE_SANITIZER_ADDRESS
      SGLDM_ENABLE_SANITIZER_LEAK
      SGLDM_ENABLE_SANITIZER_UNDEFINED
      SGLDM_ENABLE_SANITIZER_THREAD
      SGLDM_ENABLE_SANITIZER_MEMORY
      SGLDM_ENABLE_UNITY_BUILD
      SGLDM_ENABLE_CLANG_TIDY
      SGLDM_ENABLE_CPPCHECK
      SGLDM_ENABLE_COVERAGE
      SGLDM_ENABLE_PCH
      SGLDM_ENABLE_CACHE)
  endif()

  SGLDM_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (SGLDM_ENABLE_SANITIZER_ADDRESS OR SGLDM_ENABLE_SANITIZER_THREAD OR SGLDM_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(SGLDM_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(SGLDM_global_options)
  if(SGLDM_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    SGLDM_enable_ipo()
  endif()

  SGLDM_supports_sanitizers()

  if(SGLDM_ENABLE_HARDENING AND SGLDM_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR SGLDM_ENABLE_SANITIZER_UNDEFINED
       OR SGLDM_ENABLE_SANITIZER_ADDRESS
       OR SGLDM_ENABLE_SANITIZER_THREAD
       OR SGLDM_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${SGLDM_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${SGLDM_ENABLE_SANITIZER_UNDEFINED}")
    SGLDM_enable_hardening(SGLDM_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(SGLDM_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(SGLDM_warnings INTERFACE)
  add_library(SGLDM_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  SGLDM_set_project_warnings(
    SGLDM_warnings
    ${SGLDM_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(SGLDM_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    SGLDM_configure_linker(SGLDM_options)
  endif()

  include(cmake/Sanitizers.cmake)
  SGLDM_enable_sanitizers(
    SGLDM_options
    ${SGLDM_ENABLE_SANITIZER_ADDRESS}
    ${SGLDM_ENABLE_SANITIZER_LEAK}
    ${SGLDM_ENABLE_SANITIZER_UNDEFINED}
    ${SGLDM_ENABLE_SANITIZER_THREAD}
    ${SGLDM_ENABLE_SANITIZER_MEMORY})

  set_target_properties(SGLDM_options PROPERTIES UNITY_BUILD ${SGLDM_ENABLE_UNITY_BUILD})

  if(SGLDM_ENABLE_PCH)
    target_precompile_headers(
      SGLDM_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(SGLDM_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    SGLDM_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(SGLDM_ENABLE_CLANG_TIDY)
    SGLDM_enable_clang_tidy(SGLDM_options ${SGLDM_WARNINGS_AS_ERRORS})
  endif()

  if(SGLDM_ENABLE_CPPCHECK)
    SGLDM_enable_cppcheck(${SGLDM_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(SGLDM_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    SGLDM_enable_coverage(SGLDM_options)
  endif()

  if(SGLDM_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(SGLDM_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(SGLDM_ENABLE_HARDENING AND NOT SGLDM_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR SGLDM_ENABLE_SANITIZER_UNDEFINED
       OR SGLDM_ENABLE_SANITIZER_ADDRESS
       OR SGLDM_ENABLE_SANITIZER_THREAD
       OR SGLDM_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    SGLDM_enable_hardening(SGLDM_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
