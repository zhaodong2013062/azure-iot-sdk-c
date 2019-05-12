#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 2.8.11)

compileAsC11()
set(theseTestsName prov_x509_e2etests)

set(${theseTestsName}_test_files
    ${theseTestsName}.c
    ../common_test_files/min_prov_e2e_common.c
)

set(${theseTestsName}_h_files
    ../common_test_files/min_prov_e2e_common.h
)

if(${use_sample_trusted_cert})
    set(${theseTestsName}_c_files
        ../../../certs/certs.c
    )
    add_definitions(-DSET_TRUSTED_CERT_IN_SAMPLES)
endif()

include_directories(${DEV_AUTH_MODULES_CLIENT_INC_FOLDER})
include_directories(${SHARED_UTIL_INC_FOLDER})
include_directories(../common_test_files)

#file(COPY ../common_prov_e2e/prov_valgrind_suppression.supp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
build_c_test_artifacts(${theseTestsName} ON "tests/azure_prov_e2e_tests" VALGRIND_SUPPRESSIONS_FILE prov_valgrind_suppression.supp)

set(prov_transport)
if (${use_http})
    add_definitions(-DUSE_HTTP)
    set(prov_transport ${prov_transport} prov_http_transport)
endif()
if (${use_mqtt})
    add_definitions(-DUSE_MQTT)
    set(prov_transport ${prov_transport} prov_mqtt_transport prov_mqtt_ws_transport)
endif()
if (${use_amqp})
    add_definitions(-DUSE_AMQP)
    set(prov_transport ${prov_transport} prov_amqp_transport prov_amqp_ws_transport)
endif()
if (${use_mqtt})
    add_definitions(-DUSE_MQTT)
    set(prov_transport ${prov_transport} prov_mqtt_transport)
endif()

if(WIN32)
    if(TARGET ${theseTestsName}_dll)
        target_link_libraries(${theseTestsName}_dll
            prov_device_ll_client
            ${prov_transport}
            rpcrt4
        )
    endif()

    if(TARGET ${theseTestsName}_exe)
        target_link_libraries(${theseTestsName}_exe
            prov_device_ll_client
            ${prov_transport}
            rpcrt4
        )
    endif()
else()
    if(TARGET ${theseTestsName}_exe)
        target_link_libraries(${theseTestsName}_exe
            prov_device_ll_client
            ${prov_transport}
        )
    endif()
endif()

link_security_client(${theseTestsName}_exe)
linkSharedUtil(${theseTestsName}_exe)
