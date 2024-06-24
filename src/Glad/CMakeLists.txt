include(GenerateExportHeader)


add_library(Glad
        glad.c)



add_library(SGLDM::Glad ALIAS Glad)

target_link_libraries(Glad PRIVATE SGLDM_options SGLDM_warnings)

target_include_directories(Glad ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(Glad PUBLIC cxx_std_20)

set_target_properties(
  Glad
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

#generate_export_header(../../include/SGLDM/Gladsss EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/SGLDM/Glad_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(Glad PUBLIC Glad_STATIC_DEFINE)
endif()
