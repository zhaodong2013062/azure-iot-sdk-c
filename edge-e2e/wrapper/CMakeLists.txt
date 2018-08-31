cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

set ( C_SDK_ROOT "${PROJECT_SOURCE_DIR}/../.." )
message("Using C SDK from ${C_SDK_ROOT}" )

set(use_edge_modules ON CACHE BOOL "include edge module code")
set(BUILD_SSL OFF CACHE BOOL "use SSL for restbed server")

add_subdirectory( "${C_SDK_ROOT}" azure-iot-sdk-c )
add_subdirectory( "${PROJECT_SOURCE_DIR}/deps/restbed/" )

include_directories("${C_SDK_ROOT}/deps/parson") #this brings in parson from the source
include_directories(.)
include_directories(${SHARED_UTIL_INC_FOLDER})
include_directories(${IOTHUB_SERVICE_CLIENT_INC_FOLDER})
include_directories(${IOTHUB_CLIENT_INC_FOLDER})
include_directories("${PROJECT_SOURCE_DIR}/deps/restbed/source")

set(iothub_client_libs
    iothub_client
    iothub_service_client
    iothub_client_amqp_transport
    iothub_client_mqtt_transport
    iothub_client_http_transport
    uamqp
    umqtt
    aziotsharedutil
    parson
)

set(generated_cpp_files
  ModuleApi.cpp
  WrapperApi.cpp
  RegistryApi.cpp
  ServiceApi.cpp
  DeviceApi.cpp
)

set(generated_h_files
  ModuleApi.h
  WrapperApi.h
  RegistryApi.h
  ServiceApi.h
  DeviceApi.h
)

set(glue_cpp_files
  GlueUtils.cpp
  InternalGlue.cpp
  DeviceGlue.cpp
  ModuleGlue.cpp
  WrapperGlue.cpp
  RegistryGlue.cpp
  ServiceGlue.cpp
)

set(glue_h_files
  GlueUtils.h
  InternalGlue.h
  DeviceGlue.h
  ModuleGlue.h
  WrapperGlue.h
  RegistryGlue.h
  ServiceGlue.h
)

add_executable(edge_e2e_rest_server
  Main.cpp
  ${generated_cpp_files}
  ${generated_h_files}
  ${glue_cpp_files}
  ${glue_h_files}
)

target_link_libraries(edge_e2e_rest_server
  ${iothub_client_libs}
  restbed-static
)

