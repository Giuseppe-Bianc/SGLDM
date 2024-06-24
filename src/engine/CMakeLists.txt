add_executable(engine main.cpp)

target_link_libraries(
  engine
  PRIVATE SGLDM::SGLDM_options
          SGLDM::SGLDM_warnings)

target_link_system_libraries(
        engine
        PRIVATE
        CLI11::CLI11
        Core
)

target_include_directories(engine PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
