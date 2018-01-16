#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for ${theseTestsName}
cmake_minimum_required(VERSION 2.8.11)

compileAsC99()
set(theseTestsName schemaserializer_2_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/schemaserializer.c
../../src/codefirst.c
${SHARED_UTIL_SRC_FOLDER}/gballoc.c
${SHARED_UTIL_SRC_FOLDER}/crt_abstractions.c
${SHARED_UTIL_SRC_FOLDER}/vector.c
${LOCK_C_FILE}
real_parson.c
real_schema.c
)

include_directories(${SERIALIZER_SRC_FOLDER})


set(${theseTestsName}_h_files
real_parson.h
real_schema.h
)

if(MSVC)
    set_source_files_properties(real_parson.c PROPERTIES COMPILE_FLAGS "/wd4244 /wd4232")
endif()

build_c_test_artifacts(${theseTestsName} ON "tests/UnitTests")