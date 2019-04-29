cmake_minimum_required(VERSION 3.11)

if(WIN32)
message(FATAL_ERROR "This CMake file is only support Linux builds!")
endif()

set(AZUREIOT_INC_FOLDER ".." "/usr/include/azureiot" "/usr/include/azureiot/inc")
find_package(azure_iot_sdks REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${AZUREIOT_INC_FOLDER})

set(iothub_c_files
../iothub_ll_telemetry_sample.c
../../../../certs/certs.c
)

add_definitions(-DUSE_HTTP)
add_definitions(-DUSE_AMQP)
add_definitions(-DUSE_MQTT)

add_definitions(-DSET_TRUSTED_CERT_IN_SAMPLES)
include_directories("../../../../certs")

add_executable(iothub_ll_telemetry_sample ${iothub_c_files})
find_library(CURL NAMES curl-d curl)
target_link_libraries(iothub_ll_telemetry_sample
iothub_client_mqtt_transport
iothub_client_amqp_transport
iothub_client
parson
aziotsharedutil
umqtt
${CURL}
pthread
OpenSSL::SSL
OpenSSL::Crypto
m
ZLIB::ZLIB
)
