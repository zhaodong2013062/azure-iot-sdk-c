// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


/*these tests exist because schemaserializer uses 2 very different "collections": SCHEMA and parson*/
/*effectively mocking with simulators schema calls or parson calls are notoriously difficult because*/
/*frankly, "human mind". So instead the real schema and the real parson are used*/
/*while being detour'd*/

/*there are 3 models exploited in this file:
1) an empty model - ns1 - model 1
2) the Ice machine model - Contoso, Soda_Machine_Device_Model
3) a very complex model. Contoso_plus_plus, Soda_Machine_Device_Model_Plus_Plus. Details follow*/

/*the complex model has 

A) 2 data types
    1) data_a: a structure of a string and int
    2) data_b: a structure of data_a , another string and int

B) reported properties of 
    1) reported_int   : a reported property of type int
    2) reported_data_a: a reported property of type data_a
    3) reported_data_b: a reported property of type data_b

C) desired properties of
    1) desired_int: a desired property of type int
    2) desired_data_a: a desired property of type data_a
    3) desired_data_b: a desired property of type data_b

D)  C2D methods of:

    1) returning void:
        1.1 with no parameters
    2) returning void, taking 1 parameter
        2.1 with 1x int
        2.2 with 1x data_a
        2.3 with 1x data_b
    3) returning void, taking 2 parameters
        3.1 with int / int
        3.2 with int / data_a
        3.3 with int / data_b
        3.4 with data_a / int
        3.5 with data_a / data_a
        3.6 with data_a / data_b
        3.7 with data_b / int
        3.8 with data_b / data_a
        3.9 with data_b / data_b

    -- since methods can only return structured types, the below methods are invalid --
    --INVALID--4) returning int
    --INVALID--    4.1 with no parameters
    --INVALID--5) returning int, taking 1 parameter
    --INVALID--    5.1 with 1x int
    --INVALID--    5.2 with 1x data_a
    --INVALID--    5.3 with 1x data_b
    --INVALID--6) returning int, taking 2 parameters
    --INVALID--    6.1 with int / int
    --INVALID--    6.2 with int / data_a
    --INVALID--    6.3 with int / data_b
    --INVALID--    6.4 with data_a / int
    --INVALID--    6.5 with data_a / data_a
    --INVALID--    6.6 with data_a / data_b
    --INVALID--    6.7 with data_b / int
    --INVALID--    6.8 with data_b / data_a
    --INVALID--    6.9 with data_b / data_b

    7) returning data_a
        7.1 with no parameters
    8) returning data_a, taking 1 parameter
        8.1 with 1x int
        8.2 with 1x data_a
        8.3 with 1x data_b
    9) returning data_a, taking 2 parameters
        9.1 with int / int
        9.2 with int / data_a
        9.3 with int / data_b
        9.4 with data_a / int
        9.5 with data_a / data_a
        9.6 with data_a / data_b
        9.7 with data_b / int
        9.8 with data_b / data_a
        9.9 with data_b / data_b

    10) returning data_b
        10.1 with no parameters
    11) returning data_b, taking 1 parameter
        11.1 with 1x int
        11.2 with 1x data_a
        11.3 with 1x data_b
    12) returning data_b, taking 2 parameters
        12.1 with int / int
        12.2 with int / data_a
        12.3 with int / data_b
        12.4 with data_a / int
        12.5 with data_a / data_a
        12.6 with data_a / data_b
        12.7 with data_b / int
        12.8 with data_b / data_a
        12.9 with data_b / data_b
    13) returning "any"
        13.1 with no parameters
        13.2 taking 1 parameter:
            13.2.1 with 1x int
            13.2.2 with 1x data_a
            13.2.3 with 1x data_b
        13.3 taking 2 parameters:
            13.3.1 with int / int
            13.3.2 with int / data_a
            13.3.3 with int / data_b
            13.3.4 with data_a / int
            13.3.5 with data_a / data_a
            13.3.6 with data_a / data_b
            13.3.7 with data_b / int
            13.3.8 with data_b / data_a
            13.3.9 with data_b / data_b
            

E) telemetry of type (telemetry is always structured)
    1) data_a
    2) data_b
*/

#ifdef __cplusplus
#include <cstdlib>
#include <cstddef>
#else
#include <stdlib.h>
#include <stddef.h>
#endif



static void* my_gballoc_malloc(size_t t)
{
    return malloc(t);
}

static void* my_gballoc_realloc(void* v, size_t t)
{
    return realloc(v, t);
}

static void my_gballoc_free(void * t)
{
    free(t);
}

#include "azure_c_shared_utility/gballoc.h"
#include "testrunnerswitcher.h"
#include "macro_utils.h"
#include "umock_c.h"
#include "umocktypes_charptr.h"
#include "umocktypes_bool.h"
#include "umocktypes_stdint.h"
#include "umock_c_negative_tests.h"

#define ENABLE_MOCKS
#include "agenttypesystem.h"
MOCKABLE_FUNCTION(, void, zz, int *, a);
#undef ENABLE_MOCKS

#include "real_parson.h"
#include "real_schema.h"

#define ENABLE_MOCKS
#undef SCHEMA_H
#include "schema.h"
#include "iotdevice.h"
#ifdef __cplusplus
extern "C" {
#endif

    MOCKABLE_FUNCTION(, JSON_Value *, json_parse_file, const char* , filename);
    MOCKABLE_FUNCTION(, JSON_Value *, json_parse_file_with_comments, const char* , filename);
    MOCKABLE_FUNCTION(, JSON_Value *, json_parse_string, const char *, string);
    MOCKABLE_FUNCTION(, JSON_Value *, json_parse_string_with_comments, const char *, string);
    MOCKABLE_FUNCTION(, size_t      , json_serialization_size, const JSON_Value*, value); 
    MOCKABLE_FUNCTION(, JSON_Status, json_serialize_to_buffer, const JSON_Value *, value, char*, buf, size_t, buf_size_in_bytes);
    MOCKABLE_FUNCTION(, JSON_Status, json_serialize_to_file, const JSON_Value *,value, const char *,filename);
    MOCKABLE_FUNCTION(, char *      ,json_serialize_to_string, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, size_t      , json_serialization_size_pretty, const JSON_Value *, value); 
    MOCKABLE_FUNCTION(, JSON_Status, json_serialize_to_buffer_pretty, const JSON_Value *, value, char *, buf, size_t ,buf_size_in_bytes);
    MOCKABLE_FUNCTION(, JSON_Status, json_serialize_to_file_pretty, const JSON_Value *, value, const char *, filename);
    MOCKABLE_FUNCTION(, char *      , json_serialize_to_string_pretty, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, void        , json_free_serialized_string, char *, string); 
    MOCKABLE_FUNCTION(, int , json_value_equals, const JSON_Value *, a, const JSON_Value *, b);
    MOCKABLE_FUNCTION(, JSON_Status, json_validate, const JSON_Value *, schema, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Value  * , json_object_get_value, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, const char  * , json_object_get_string, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Object * , json_object_get_object, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Array  * ,json_object_get_array, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, double        , json_object_get_number, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, int           , json_object_get_boolean, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Value  * , json_object_dotget_value, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, const char  * , json_object_dotget_string, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Object * , json_object_dotget_object, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Array  * , json_object_dotget_array, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, double        , json_object_dotget_number, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, int           , json_object_dotget_boolean, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, size_t        , json_object_get_count, const JSON_Object *, object);
    MOCKABLE_FUNCTION(, const char  * , json_object_get_name, const JSON_Object *, object, size_t, index);
    MOCKABLE_FUNCTION(, JSON_Value  * , json_object_get_value_at, const JSON_Object *, object, size_t, index);
    MOCKABLE_FUNCTION(, int, json_object_has_value, const JSON_Object *, object, const char *,name);
    MOCKABLE_FUNCTION(, int, json_object_has_value_of_type, const JSON_Object *, object, const char *, name, JSON_Value_Type, type);
    MOCKABLE_FUNCTION(, int ,json_object_dothas_value, const JSON_Object *, object, const char *, name);
    MOCKABLE_FUNCTION(, int , json_object_dothas_value_of_type, const JSON_Object *, object, const char *, name, JSON_Value_Type, type);
    MOCKABLE_FUNCTION(, JSON_Status, json_object_set_value, JSON_Object *, object, const char *, name, JSON_Value *, value);

    MOCKABLE_FUNCTION(, JSON_Status ,json_object_set_string, JSON_Object*, object, const char *, name, const char *, string);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_set_number, JSON_Object*, object, const char *, name, double, number);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_set_boolean, JSON_Object*, object, const char *, name, int, boolean);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_set_null, JSON_Object*, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotset_value, JSON_Object*, object, const char *, name, JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotset_string, JSON_Object*, object, const char *, name, const char *, string);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotset_number, JSON_Object*, object, const char *, name, double, number);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotset_boolean, JSON_Object*, object, const char *, name, int, boolean);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotset_null, JSON_Object*, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_remove, JSON_Object*, object, const char *, name);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_dotremove, JSON_Object*, object, const char *, key);
    MOCKABLE_FUNCTION(, JSON_Status ,json_object_clear, JSON_Object*, object);
    MOCKABLE_FUNCTION(, JSON_Value  * ,json_array_get_value,const JSON_Array *, array, size_t, index);
    MOCKABLE_FUNCTION(, const char  * ,json_array_get_string,const JSON_Array *, array, size_t, index);
    MOCKABLE_FUNCTION(, JSON_Object * ,json_array_get_object,const JSON_Array *, array, size_t, index);
    MOCKABLE_FUNCTION(, JSON_Array  * ,json_array_get_array,const JSON_Array *, array, size_t, index);
    MOCKABLE_FUNCTION(, double        ,json_array_get_number,const JSON_Array *, array, size_t, index); 
    MOCKABLE_FUNCTION(, int           ,json_array_get_boolean,const JSON_Array *, array, size_t, index);
    MOCKABLE_FUNCTION(, size_t        ,json_array_get_count,const JSON_Array *, array);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_remove,JSON_Array *, array, size_t,  i);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_replace_value,JSON_Array *, array, size_t,  i, JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_replace_string,JSON_Array *, array, size_t,  i, const char*, string);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_replace_number,JSON_Array *, array, size_t,  i, double, number);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_replace_boolean,JSON_Array *, array, size_t,  i, int, boolean);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_replace_null,JSON_Array *, array, size_t,  i);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_clear,JSON_Array *, array);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_append_value,JSON_Array *, array, JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_append_string,JSON_Array *, array, const char *, string);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_append_number,JSON_Array *, array, double, number);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_append_boolean,JSON_Array *, array, int, boolean);
    MOCKABLE_FUNCTION(, JSON_Status ,json_array_append_null,JSON_Array *, array);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_object);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_array);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_string, const char *, string);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_number, double, number);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_boolean, int, boolean);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_init_null);
    MOCKABLE_FUNCTION(, JSON_Value * ,json_value_deep_copy, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, void         ,json_value_free, JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Value_Type ,json_value_get_type, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Object *   ,json_value_get_object, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Array  *   ,json_value_get_array, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, const char  *   ,json_value_get_string, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, double          ,json_value_get_number, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, int             ,json_value_get_boolean, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Value_Type ,json_type, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Object *   ,json_object, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, JSON_Array  *   ,json_array, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, const char  *   ,json_string, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, double          ,json_number, const JSON_Value *, value);
    MOCKABLE_FUNCTION(, int             ,json_boolean, const JSON_Value *, value);
#ifdef __cplusplus
}
#endif

#undef ENABLE_MOCKS

#include "schemaserializer.h"


#include "serializer.h"

#include "codefirst.h"

#define SCHEMA_VERSION "1.0.0"
#define ID "cf7eecbd-1435-45e0-a144-c42bb8c88849"
#define VERSION "1.2.3"
#define DESCRIPTION "Ice Machine"

BEGIN_NAMESPACE(ns1);
DECLARE_MODEL(model1,
    WITH_INFORMATION(SCHEMA_VERSION, ID, VERSION, DESCRIPTION)
);

END_NAMESPACE(ns1);

BEGIN_NAMESPACE(Contoso);

DECLARE_STRUCT(accelerometer,
double, x,
double, y,
double, z
);

DECLARE_STRUCT(pressure_t,
int16_t, temperature,
double, waterPressure
);

DECLARE_STRUCT(iceDispenseCount_t,
int64_t, count
);

DECLARE_STRUCT(firmwareDetails_t,
ascii_char_ptr, firmwareDetails);

DECLARE_MODEL(Soda_Machine_Device_Model,

WITH_INFORMATION("1.0.0", "cf7eecbd-1435-45e0-a144-c42bb8c88849", "1.2.3", "Ice Machine"),
WITH_REPORTED_PROPERTY(ascii_char_ptr, serialNumber),
WITH_REPORTED_PROPERTY(int16_t, lastReportedTemperature),
WITH_REPORTED_PROPERTY(int8_t, lastReportedIceLevel),
WITH_REPORTED_PROPERTY(accelerometer, lastReportedIceAcceleration),

WITH_METHOD(firmwareDetails_t, updateFirmware, ascii_char_ptr, packageName),
WITH_METHOD(void, reboot),
WITH_DESIRED_PROPERTY(int16_t, desiredTemperature),

WITH_DATA(iceDispenseCount_t, iceDispenseCount),
WITH_DATA(pressure_t, pressure)
);

END_NAMESPACE(Contoso);

void reboot(Soda_Machine_Device_Model* soda)
{
    (void)(soda);
    printf("rebooting... \n");
    return;
}


firmwareDetails_t updateFirmware(Soda_Machine_Device_Model* soda, ascii_char_ptr string)
{
    (void)(soda);
    firmwareDetails_t result;
    result.firmwareDetails = "0xDEADBEEF";
    printf("updating firmware... received: %s\n", P_OR_NULL(string));
    return result;
}

/*Contoso2 is the same as Contoso except is has NO WITH_INFORMATION*/
BEGIN_NAMESPACE(Contoso2);

DECLARE_STRUCT(accelerometer2,
double, x2,
double, y2,
double, z2
);

DECLARE_STRUCT(pressure_t2,
int16_t, temperature2,
double, waterPressure2
);

DECLARE_STRUCT(iceDispenseCount_t2,
int64_t, count2
);

DECLARE_STRUCT(firmwareDetails_t2,
ascii_char_ptr, firmwareDetails2);

DECLARE_MODEL(Soda_Machine_Device_Model2,

WITH_REPORTED_PROPERTY(ascii_char_ptr, serialNumber2),
WITH_REPORTED_PROPERTY(int16_t, lastReportedTemperature2),
WITH_REPORTED_PROPERTY(int8_t, lastReportedIceLevel2),
WITH_REPORTED_PROPERTY(accelerometer2, lastReportedIceAcceleration2),

WITH_METHOD(firmwareDetails_t2, updateFirmware2, ascii_char_ptr, packageName2),
WITH_METHOD(void, reboot2),
WITH_DESIRED_PROPERTY(int16_t, desiredTemperature2),

WITH_DATA(iceDispenseCount_t2, iceDispenseCount2),
WITH_DATA(pressure_t2, pressure2)
);

END_NAMESPACE(Contoso2);

void reboot2(Soda_Machine_Device_Model2* soda)
{
    (void)(soda);
    printf("rebooting... \n");
    return;
}


firmwareDetails_t2 updateFirmware2(Soda_Machine_Device_Model2* soda, ascii_char_ptr string)
{
    (void)(soda);
    firmwareDetails_t2 result;
    result.firmwareDetails2 = "0xDEADBEEF";
    printf("updating firmware... received: %s\n", P_OR_NULL(string));
    return result;
}


#define SCHEMA_VERSION_PLUS_PLUS "1.0.0"
#define ID_PLUS_PLUS "cf7eecbd-1435-45e0-a144-c42bb8c88849"
#define VERSION_PLUS_PLUS "1.2.3"
#define DESCRIPTION_PLUS_PLUS "Ice Machine with a tint of soda"

BEGIN_NAMESPACE(Contoso_plus_plus);

DECLARE_STRUCT(data_a,
    int, a_int,
    ascii_char_ptr, a_string
);

DECLARE_STRUCT(data_b,
    data_a, b_data_a,
    ascii_char_ptr, b_string, 
    int, b_int
);
DECLARE_MODEL(Soda_Machine_Device_Model_Plus_Plus,

WITH_INFORMATION(SCHEMA_VERSION_PLUS_PLUS, ID_PLUS_PLUS, VERSION_PLUS_PLUS, DESCRIPTION_PLUS_PLUS),

WITH_REPORTED_PROPERTY(int,    reported_int   ),
WITH_REPORTED_PROPERTY(data_a, reported_data_a),
WITH_REPORTED_PROPERTY(data_b, reported_data_b),

WITH_DESIRED_PROPERTY(int,    desired_int   ),
WITH_DESIRED_PROPERTY(data_a, desired_data_a),
WITH_DESIRED_PROPERTY(data_b, desired_data_b),

/*rule of naming the methods: returnType##parameterTypes*/
/*returning void*/
WITH_METHOD(void, void_void), 
WITH_METHOD(void, void_int, int, a), 
WITH_METHOD(void, void_data_a, data_a, a),
WITH_METHOD(void, void_data_b, data_b, b),
WITH_METHOD(void, void_int_int, int, x, int, y),
WITH_METHOD(void, void_int_data_a, int, x, data_a, y),
WITH_METHOD(void, void_int_data_b, int, x, data_b, y),
WITH_METHOD(void, void_data_a_int, data_a, x, int, y),
WITH_METHOD(void, void_data_a_data_a, data_a, x, data_a, y),
WITH_METHOD(void, void_data_a_data_b, data_a, x, data_b, y),
WITH_METHOD(void, void_data_b_int, data_b, x, int, y),
WITH_METHOD(void, void_data_b_data_a, data_b, x, data_a, y),
WITH_METHOD(void, void_data_b_data_b, data_b, x, data_b, y),

/*returning int*/
/*int and other C types are no longer valid return types for methods*/
/*
WITH_METHOD(int, int_void),
WITH_METHOD(int, int_int, int, a),
WITH_METHOD(int, int_data_a, data_a, a),
WITH_METHOD(int, int_data_b, data_b, b),
WITH_METHOD(int, int_int_int, int, x, int, y),
WITH_METHOD(int, int_int_data_a, int, x, data_a, y),
WITH_METHOD(int, int_int_data_b, int, x, data_b, y),
WITH_METHOD(int, int_data_a_int, data_a, x, int, y),
WITH_METHOD(int, int_data_a_data_a, data_a, x, data_a, y),
WITH_METHOD(int, int_data_a_data_b, data_a, x, data_b, y),
WITH_METHOD(int, int_data_b_int, data_b, x, int, y),
WITH_METHOD(int, int_data_b_data_a, data_b, x, data_a, y),
WITH_METHOD(int, int_data_b_data_b, data_b, x, data_b, y),
*/

/*returning data_a*/
WITH_METHOD(data_a, data_a_void),
WITH_METHOD(data_a, data_a_int, int, a),
WITH_METHOD(data_a, data_a_data_a, data_a, a),
WITH_METHOD(data_a, data_a_data_b, data_b, b),
WITH_METHOD(data_a, data_a_int_int, int, x, int, y),
WITH_METHOD(data_a, data_a_int_data_a, int, x, data_a, y),
WITH_METHOD(data_a, data_a_int_data_b, int, x, data_b, y),
WITH_METHOD(data_a, data_a_data_a_int, data_a, x, int, y),
WITH_METHOD(data_a, data_a_data_a_data_a, data_a, x, data_a, y),
WITH_METHOD(data_a, data_a_data_a_data_b, data_a, x, data_b, y),
WITH_METHOD(data_a, data_a_data_b_int, data_b, x, int, y),
WITH_METHOD(data_a, data_a_data_b_data_a, data_b, x, data_a, y),
WITH_METHOD(data_a, data_a_data_b_data_b, data_b, x, data_b, y),

/*returning data_b*/
WITH_METHOD(data_b, data_b_void),
WITH_METHOD(data_b, data_b_int, int, a),
WITH_METHOD(data_b, data_b_data_a, data_a, a),
WITH_METHOD(data_b, data_b_data_b, data_b, b),
WITH_METHOD(data_b, data_b_int_int, int, x, int, y),
WITH_METHOD(data_b, data_b_int_data_a, int, x, data_a, y),
WITH_METHOD(data_b, data_b_int_data_b, int, x, data_b, y),
WITH_METHOD(data_b, data_b_data_a_int, data_a, x, int, y),
WITH_METHOD(data_b, data_b_data_a_data_a, data_a, x, data_a, y),
WITH_METHOD(data_b, data_b_data_a_data_b, data_a, x, data_b, y),
WITH_METHOD(data_b, data_b_data_b_int, data_b, x, int, y),
WITH_METHOD(data_b, data_b_data_b_data_a, data_b, x, data_a, y),
WITH_METHOD(data_b, data_b_data_b_data_b, data_b, x, data_b, y),

/*returning "any"*/

WITH_METHOD(any),
WITH_METHOD(any_int, int, a),
WITH_METHOD(any_data_a, data_a, a),
WITH_METHOD(any_data_b, data_b, b),
WITH_METHOD(any_int_int, int, x, int, y),
WITH_METHOD(any_int_data_a, int, x, data_a, y),
WITH_METHOD(any_int_data_b, int, x, data_b, y),
WITH_METHOD(any_data_a_int, data_a, x, int, y),
WITH_METHOD(any_data_a_data_a, data_a, x, data_a, y),
WITH_METHOD(any_data_a_data_b, data_a, x, data_b, y),
WITH_METHOD(any_data_b_int, data_b, x, int, y),
WITH_METHOD(any_data_b_data_a, data_b, x, data_a, y),
WITH_METHOD(any_data_b_data_b, data_b, x, data_b, y),

/*telemetry*/
WITH_DATA(data_a, a),
WITH_DATA(data_b, b)
);

END_NAMESPACE(Contoso_plus_plus);

void void_void(Soda_Machine_Device_Model_Plus_Plus* soda)
{
    /*do nothing*/
    (void)soda;
}

void void_int(Soda_Machine_Device_Model_Plus_Plus* soda, int a)
{
    (void)soda;
    (void)a;
}

void void_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a a)
{
    (void)soda;
    (void)a;
}

void void_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b b)
{
    (void)soda;
    (void)b;
}

void void_int_int(Soda_Machine_Device_Model_Plus_Plus* soda, int x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_int_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_int_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_a_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_a_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_a_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_b_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_b_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
}

void void_data_b_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
}

static const data_a return_data_a = { 10 /*because 10 in hex is 'A'*/, "A" };

data_a data_a_void(Soda_Machine_Device_Model_Plus_Plus* soda)
{
    /*do nothing*/
    (void)soda;
    return return_data_a;
}

data_a data_a_int(Soda_Machine_Device_Model_Plus_Plus* soda, int a)
{
    (void)soda;
    (void)a;
    return return_data_a;
}

data_a data_a_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a a)
{
    (void)soda;
    (void)a;
    return return_data_a;
}

data_a data_a_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b b)
{
    (void)soda;
    (void)b;
    return return_data_a;
}

data_a data_a_int_int(Soda_Machine_Device_Model_Plus_Plus* soda, int x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_int_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_int_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_a_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_a_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_a_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_b_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_b_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

data_a data_a_data_b_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_a;
}

static const data_b return_data_b = { {11 /*because 11 in hex is 'B'*/, "B"}, "BB", 1111 };

data_b data_b_void(Soda_Machine_Device_Model_Plus_Plus* soda)
{
    /*do nothing*/
    (void)soda;
    return return_data_b;
}

data_b data_b_int(Soda_Machine_Device_Model_Plus_Plus* soda, int a)
{
    (void)soda;
    (void)a;
    return return_data_b;
}

data_b data_b_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a a)
{
    (void)soda;
    (void)a;
    return return_data_b;
}

data_b data_b_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b b)
{
    (void)soda;
    (void)b;
    return return_data_b;
}

data_b data_b_int_int(Soda_Machine_Device_Model_Plus_Plus* soda, int x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_int_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_int_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_a_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_a_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_a_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_b_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_b_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

data_b data_b_data_b_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return return_data_b;
}

METHODRETURN_HANDLE any(Soda_Machine_Device_Model_Plus_Plus* soda)
{
    (void)soda;
    return NULL;
}

METHODRETURN_HANDLE any_int(Soda_Machine_Device_Model_Plus_Plus* soda, int a)
{
    (void)soda;
    (void)a;
    return NULL;
}

METHODRETURN_HANDLE any_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a a)
{
    (void)soda;
    (void)a;
    return NULL;
}

METHODRETURN_HANDLE any_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b b)
{
    (void)soda;
    (void)b;
    return NULL;
}

METHODRETURN_HANDLE any_int_int(Soda_Machine_Device_Model_Plus_Plus* soda, int x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_int_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_int_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, int x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_a_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_a_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_a_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_a x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_b_int(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, int y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_b_data_a(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_a y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}

METHODRETURN_HANDLE any_data_b_data_b(Soda_Machine_Device_Model_Plus_Plus* soda, data_b x, data_b y)
{
    (void)soda;
    (void)x;
    (void)y;
    return NULL;
}


TEST_DEFINE_ENUM_TYPE(SCHEMA_SERIALIZER_RESULT, SCHEMA_SERIALIZER_RESULT_VALUES);
IMPLEMENT_UMOCK_C_ENUM_TYPE(SCHEMA_SERIALIZER_RESULT, SCHEMA_SERIALIZER_RESULT_VALUES);

DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)
static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    char temp_str[256];
    (void)snprintf(temp_str, sizeof(temp_str), "umock_c reported error :%s", ENUM_TO_STRING(UMOCK_C_ERROR_CODE, error_code));
    ASSERT_FAIL(temp_str); 
}

static TEST_MUTEX_HANDLE g_testByTest;
static TEST_MUTEX_HANDLE g_dllByDll;

#define REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(returnValue, function, hookFunction) \
    REGISTER_GLOBAL_MOCK_HOOK  (function                                          ,hookFunction                        ); \
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(function, returnValue);

BEGIN_TEST_SUITE(SchemaSerializer_2_ut)

    TEST_SUITE_INITIALIZE(TestClassInitialize)
    {
        TEST_INITIALIZE_MEMORY_DEBUG(g_dllByDll);
        g_testByTest = TEST_MUTEX_CREATE();
        ASSERT_IS_NOT_NULL(g_testByTest);

        (void)umock_c_init(on_umock_c_error);
        (void)umocktypes_bool_register_types();
        (void)umocktypes_charptr_register_types();
        (void)umocktypes_stdint_register_types();

        REGISTER_UMOCK_ALIAS_TYPE(STRING_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_MODEL_TYPE_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_ACTION_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_ACTION_ARGUMENT_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_STRUCT_TYPE_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(pfDesiredPropertyFromAGENT_DATA_TYPE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(pfDesiredPropertyInitialize, void*);
        REGISTER_UMOCK_ALIAS_TYPE(pfDesiredPropertyDeinitialize, void*);
        REGISTER_UMOCK_ALIAS_TYPE(pfOnDesiredProperty, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_METHOD_WITH_RETURN_TYPE_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_PROPERTY_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_REPORTED_PROPERTY_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_DESIRED_PROPERTY_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_METHOD_ARGUMENT_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SCHEMA_METHOD_HANDLE, void*);
        
        
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_parse_file                                          ,real_json_parse_file                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_parse_file_with_comments                            ,real_json_parse_file_with_comments          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_parse_string                                        ,real_json_parse_string                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_parse_string_with_comments                          ,real_json_parse_string_with_comments        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0   ,             json_serialization_size                                  ,real_json_serialization_size                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_serialize_to_buffer                                 ,real_json_serialize_to_buffer               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_serialize_to_file                                   ,real_json_serialize_to_file                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_serialize_to_string                                 ,real_json_serialize_to_string               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_serialization_size_pretty                           ,real_json_serialization_size_pretty         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_serialize_to_buffer_pretty                          ,real_json_serialize_to_buffer_pretty        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_serialize_to_file_pretty                            ,real_json_serialize_to_file_pretty          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_serialize_to_string_pretty                          ,real_json_serialize_to_string_pretty        );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_free_serialized_string                              ,real_json_free_serialized_string            );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_value_equals                                        ,real_json_value_equals                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_validate                                            ,real_json_validate                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_value                                    ,real_json_object_get_value                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_string                                   ,real_json_object_get_string                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_object                                   ,real_json_object_get_object                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_array                                    ,real_json_object_get_array                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_object_get_number                                   ,real_json_object_get_number                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(-1,               json_object_get_boolean                                  ,real_json_object_get_boolean                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_dotget_value                                 ,real_json_object_dotget_value               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_dotget_string                                ,real_json_object_dotget_string              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_dotget_object                                ,real_json_object_dotget_object              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_dotget_array                                 ,real_json_object_dotget_array               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_object_dotget_number                                ,real_json_object_dotget_number              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(-1,               json_object_dotget_boolean                               ,real_json_object_dotget_boolean             );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_object_get_count                                    ,real_json_object_get_count                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_name                                     ,real_json_object_get_name                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object_get_value_at                                 ,real_json_object_get_value_at               );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_object_has_value                                    ,real_json_object_has_value                  );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_object_has_value_of_type                            ,real_json_object_has_value_of_type          );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_object_dothas_value                                 ,real_json_object_dothas_value               );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_object_dothas_value_of_type                         ,real_json_object_dothas_value_of_type       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_set_value                                    ,real_json_object_set_value                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_set_string                                   ,real_json_object_set_string                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_set_number                                   ,real_json_object_set_number                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_set_boolean                                  ,real_json_object_set_boolean                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_set_null                                     ,real_json_object_set_null                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotset_value                                 ,real_json_object_dotset_value               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotset_string                                ,real_json_object_dotset_string              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotset_number                                ,real_json_object_dotset_number              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotset_boolean                               ,real_json_object_dotset_boolean             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotset_null                                  ,real_json_object_dotset_null                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_remove                                       ,real_json_object_remove                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_dotremove                                    ,real_json_object_dotremove                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_object_clear                                        ,real_json_object_clear                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_array_get_value                                     ,real_json_array_get_value                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_array_get_string                                    ,real_json_array_get_string                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_array_get_object                                    ,real_json_array_get_object                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_array_get_array                                     ,real_json_array_get_array                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_array_get_number                                    ,real_json_array_get_number                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(-1,               json_array_get_boolean                                   ,real_json_array_get_boolean                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_array_get_count                                     ,real_json_array_get_count                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_remove                                        ,real_json_array_remove                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_replace_value                                 ,real_json_array_replace_value               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_replace_string                                ,real_json_array_replace_string              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_replace_number                                ,real_json_array_replace_number              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_replace_boolean                               ,real_json_array_replace_boolean             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_replace_null                                  ,real_json_array_replace_null                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_clear                                         ,real_json_array_clear                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_append_value                                  ,real_json_array_append_value                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_append_string                                 ,real_json_array_append_string               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_append_number                                 ,real_json_array_append_number               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_append_boolean                                ,real_json_array_append_boolean              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONFailure,      json_array_append_null                                   ,real_json_array_append_null                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_object                                   ,real_json_value_init_object                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_array                                    ,real_json_value_init_array                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_string                                   ,real_json_value_init_string                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_number                                   ,real_json_value_init_number                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_boolean                                  ,real_json_value_init_boolean                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_init_null                                     ,real_json_value_init_null                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_deep_copy                                     ,real_json_value_deep_copy                   );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_value_free                                          ,real_json_value_free                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONError,        json_value_get_type                                      ,real_json_value_get_type                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_get_object                                    ,real_json_value_get_object                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_get_array                                     ,real_json_value_get_array                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_value_get_string                                    ,real_json_value_get_string                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_value_get_number                                    ,real_json_value_get_number                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(-1,               json_value_get_boolean                                   ,real_json_value_get_boolean                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(JSONError,        json_type                                                ,real_json_type                              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_object                                              ,real_json_object                            );
        REGISTER_GLOBAL_MOCK_HOOK(                                   json_array                                               ,real_json_array                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             json_string                                              ,real_json_string                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(0,                json_number                                              ,real_json_number                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(-1,               json_boolean                                             ,real_json_boolean                           );



        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_Create                                                         ,real_Schema_Create                                                                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetMetadata                                                    ,real_Schema_GetMetadata                                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetSchemaCount                                                 ,real_Schema_GetSchemaCount                                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetSchemaByNamespace                                           ,real_Schema_GetSchemaByNamespace                                                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetSchemaForModel                                              ,real_Schema_GetSchemaForModel                                                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetSchemaNamespace                                             ,real_Schema_GetSchemaNamespace                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddDeviceRef                                                   ,real_Schema_AddDeviceRef                                                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_ReleaseDeviceRef                                               ,real_Schema_ReleaseDeviceRef                                                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_CreateModelType                                                ,real_Schema_CreateModelType                                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetSchemaForModelType                                          ,real_Schema_GetSchemaForModelType                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelName                                                   ,real_Schema_GetModelName                                                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_CreateStructType                                               ,real_Schema_CreateStructType                                                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetStructTypeName                                              ,real_Schema_GetStructTypeName                                                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddStructTypeProperty                                          ,real_Schema_AddStructTypeProperty                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelProperty                                               ,real_Schema_AddModelProperty                                                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelReportedProperty                                       ,real_Schema_AddModelReportedProperty                                                );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelDesiredProperty                                        ,real_Schema_AddModelDesiredProperty                                                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelModel                                                  ,real_Schema_AddModelModel                                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelInformation                                            ,real_Schema_AddModelInformation                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelSchemaVersion                                          ,real_Schema_GetModelSchemaVersion                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelId                                                     ,real_Schema_GetModelId                                                              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelVersion                                                ,real_Schema_GetModelVersion                                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDescription                                            ,real_Schema_GetModelDescription                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_CreateModelAction                                              ,real_Schema_CreateModelAction                                                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_CreateModelMethod                                              ,real_Schema_CreateModelMethod                                                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelActionArgument                                         ,real_Schema_AddModelActionArgument                                                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelMethodArgument                                         ,real_Schema_AddModelMethodArgument                                                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredProperty_pfDesiredPropertyFromAGENT_DATA_TYPE   ,real_Schema_GetModelDesiredProperty_pfDesiredPropertyFromAGENT_DATA_TYPE            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredProperty_pfOnDesiredProperty                    ,real_Schema_GetModelDesiredProperty_pfOnDesiredProperty                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelModelByName_Offset                                     ,real_Schema_GetModelModelByName_Offset                                              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelModelByName_OnDesiredProperty                          ,real_Schema_GetModelModelByName_OnDesiredProperty                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelModelByIndex_Offset                                    ,real_Schema_GetModelModelByIndex_Offset                                             );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_GetModelDesiredProperty_offset                                 ,real_Schema_GetModelDesiredProperty_offset                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredPropertyType                                    ,real_Schema_GetModelDesiredPropertyType                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredProperty_pfDesiredPropertyDeinitialize          ,real_Schema_GetModelDesiredProperty_pfDesiredPropertyDeinitialize                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredProperty_pfDesiredPropertyInitialize            ,real_Schema_GetModelDesiredProperty_pfDesiredPropertyInitialize                     );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_GetModelElementByName                                          ,real_Schema_GetModelElementByName                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelCount                                                  ,real_Schema_GetModelCount                                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelByName                                                 ,real_Schema_GetModelByName                                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelByIndex                                                ,real_Schema_GetModelByIndex                                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelPropertyCount                                          ,real_Schema_GetModelPropertyCount                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelPropertyByName                                         ,real_Schema_GetModelPropertyByName                                                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelPropertyByIndex                                        ,real_Schema_GetModelPropertyByIndex                                                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelReportedPropertyCount                                  ,real_Schema_GetModelReportedPropertyCount                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelReportedPropertyByName                                 ,real_Schema_GetModelReportedPropertyByName                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelReportedPropertyByIndex                                ,real_Schema_GetModelReportedPropertyByIndex                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelDesiredPropertyCount                                   ,real_Schema_GetModelDesiredPropertyCount                                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredPropertyByName                                  ,real_Schema_GetModelDesiredPropertyByName                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelDesiredPropertyByIndex                                 ,real_Schema_GetModelDesiredPropertyByIndex                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelModelCount                                             ,real_Schema_GetModelModelCount                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelModelByName                                            ,real_Schema_GetModelModelByName                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelModelyByIndex                                          ,real_Schema_GetModelModelyByIndex                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelModelPropertyNameByIndex                               ,real_Schema_GetModelModelPropertyNameByIndex                                        );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_ModelPropertyByPathExists                                      ,real_Schema_ModelPropertyByPathExists                                               );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_ModelReportedPropertyByPathExists                              ,real_Schema_ModelReportedPropertyByPathExists                                       );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_ModelDesiredPropertyByPathExists                               ,real_Schema_ModelDesiredPropertyByPathExists                                        );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelActionCount                                            ,real_Schema_GetModelActionCount                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelActionByName                                           ,real_Schema_GetModelActionByName                                                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodByName                                           ,real_Schema_GetModelMethodByName                                                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelActionByIndex                                          ,real_Schema_GetModelActionByIndex                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelActionArgumentCount                                    ,real_Schema_GetModelActionArgumentCount                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelMethodArgumentCount                                    ,real_Schema_GetModelMethodArgumentCount                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelActionName                                             ,real_Schema_GetModelActionName                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelActionArgumentByName                                   ,real_Schema_GetModelActionArgumentByName                                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelActionArgumentByIndex                                  ,real_Schema_GetModelActionArgumentByIndex                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodArgumentByIndex                                  ,real_Schema_GetModelMethodArgumentByIndex                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetActionArgumentName                                          ,real_Schema_GetActionArgumentName                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetMethodArgumentName                                          ,real_Schema_GetMethodArgumentName                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetActionArgumentType                                          ,real_Schema_GetActionArgumentType                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetMethodArgumentType                                          ,real_Schema_GetMethodArgumentType                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetStructTypeCount                                             ,real_Schema_GetStructTypeCount                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetStructTypeByName                                            ,real_Schema_GetStructTypeByName                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetStructTypeByIndex                                           ,real_Schema_GetStructTypeByIndex                                                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetStructTypePropertyCount                                     ,real_Schema_GetStructTypePropertyCount                                              );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetStructTypePropertyByName                                    ,real_Schema_GetStructTypePropertyByName                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetStructTypePropertyByIndex                                   ,real_Schema_GetStructTypePropertyByIndex                                            );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetPropertyName                                                ,real_Schema_GetPropertyName                                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetPropertyType                                                ,real_Schema_GetPropertyType                                                         );
        REGISTER_GLOBAL_MOCK_HOOK(                                   Schema_Destroy                                                        ,real_Schema_Destroy                                                                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_DestroyIfUnused                                                ,real_Schema_DestroyIfUnused                                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetReportedPropertyType                                        ,real_Schema_GetReportedPropertyType                                                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetReportedPropertyName                                        ,real_Schema_GetReportedPropertyName                                                 );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetDesiredPropertyType                                         ,real_Schema_GetDesiredPropertyType                                                  );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetDesiredPropertyName                                         ,real_Schema_GetDesiredPropertyName                                                  );

        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelMethodWithReturnTypeArgumentCount                      ,real_Schema_GetModelMethodWithReturnTypeArgumentCount                               );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodWithReturnTypeArgumentByIndex                    ,real_Schema_GetModelMethodWithReturnTypeArgumentByIndex                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelMethodWithReturnTypeCount                              ,real_Schema_GetModelMethodWithReturnTypeCount                                       );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodWithReturnTypeByIndex                            ,real_Schema_GetModelMethodWithReturnTypeByIndex                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetMethodWithReturnTypeName                                    ,real_Schema_GetMethodWithReturnTypeName                                             );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelSchema                                                 ,real_Schema_GetModelSchema                                                          );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_CreateModelMethodWithReturnType                                ,real_Schema_CreateModelMethodWithReturnType                                         );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_AddModelMethodWithReturnTypeArgument                           ,real_Schema_AddModelMethodWithReturnTypeArgument                                    );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodWithReturnTypeByName                             ,real_Schema_GetModelMethodWithReturnTypeByName                                      );

        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodArgumentByIndex                                  ,real_Schema_GetModelMethodArgumentByIndex                                           );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodName                                             ,real_Schema_GetModelMethodName                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(SCHEMA_ERROR,     Schema_GetModelMethodCount                                            ,real_Schema_GetModelMethodCount                                                     );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodByIndex                                          ,real_Schema_GetModelMethodByIndex                                                   );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodName                                             ,real_Schema_GetModelMethodName                                                      );
        REGISTER_GLOBAL_MOCK_HOOK_WITH_FAIL_RETURN(NULL,             Schema_GetModelMethodWithReturnTypeReturnType                         ,real_Schema_GetModelMethodWithReturnTypeReturnType                                  );
        
}

    TEST_SUITE_CLEANUP(TestClassCleanup)
    {
        umock_c_deinit();

        TEST_MUTEX_DESTROY(g_testByTest);
    }

    TEST_FUNCTION_INITIALIZE(TestMethodInitialize)
    {
        if (TEST_MUTEX_ACQUIRE(g_testByTest))
        {
            ASSERT_FAIL("our mutex is ABANDONED. Failure in test framework");
        }

        umock_c_reset_all_calls();
    }

    TEST_FUNCTION_CLEANUP(TestMethodCleanup)
    {
        TEST_MUTEX_RELEASE(g_testByTest);
        TEST_DEINITIALIZE_MEMORY_DEBUG(g_dllByDll);
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_001: [ If handle is NULL then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    TEST_FUNCTION(SchemaSerializer_SerializeModelToJSON_with_NULL_handle_fails)
    {
        ///arrange

        ///act
        char* result = SchemaSerializer_SerializeModelToJSON(NULL);

        ///assert
        ASSERT_IS_NULL(result);

        ///cleanup
    }

    void SchemaSerializer_2_with_empty_model_inert_path(void)
    {
STRICT_EXPECTED_CALL(Schema_GetModelSchema(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "name", "model1"));
STRICT_EXPECTED_CALL(Schema_GetModelSchemaVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "schemaVersion", "1.0.0"));
STRICT_EXPECTED_CALL(Schema_GetModelId(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "id", "cf7eecbd-1435-45e0-a144-c42bb8c88849"));
STRICT_EXPECTED_CALL(Schema_GetModelVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "version", "1.2.3"));
STRICT_EXPECTED_CALL(Schema_GetModelDescription(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "description", "Ice Machine"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataTypes", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "cloudToDeviceMethods", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "telemetry", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_serialize_to_string(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_free(IGNORED_PTR_ARG));
    }
    
    /*Tests_SRS_SCHEMA_SERIALIZER_02_002: [ SchemaSerializer_SerializeModelToJSON shall parse the model using Schema APIs and shall produce a string representation of the model by using parson APIs. ]*/
    /*Tests_SRS_SCHEMA_SERIALIZER_02_004: [ Otherwise, SchemaSerializer_SerializeModelToJSON shall return a null terminated string. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_empty_model_happy_path)
    {
        ///arrange
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("ns1", & ALL_REFLECTED(ns1));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "model1");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_empty_model_inert_path();

        ///act
        char* result = SchemaSerializer_SerializeModelToJSON(model);

        ///assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
        
        ///cleanup
        json_free_serialized_string(result);
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_empty_model_unhappy_paths)
    {
        ///arrange
        umock_c_negative_tests_init();
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("ns1", &ALL_REFLECTED(ns1));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "model1");
        umock_c_reset_all_calls();
        
        SchemaSerializer_2_with_empty_model_inert_path();

        umock_c_negative_tests_snapshot();

        for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
        {
            if (i == 32) /*JSON_value_free*/
            {
                continue;
            }

            umock_c_negative_tests_reset();

            umock_c_negative_tests_fail_call(i);

            char temp_str[128];
            sprintf(temp_str, "On failed call %zu", i);

            ///act
            char* result = SchemaSerializer_SerializeModelToJSON(model);

            ///assert
            ASSERT_IS_NULL_WITH_MSG(result, temp_str);
        }

        ///cleanup
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
        umock_c_negative_tests_deinit();
    }


    static void SchemaSerializer_2_with_soda_model_inert_path(void)
    {
STRICT_EXPECTED_CALL(Schema_GetModelSchema(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG,"name","Soda_Machine_Device_Model"));
STRICT_EXPECTED_CALL(Schema_GetModelSchemaVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG,"schemaVersion","1.0.0"));
STRICT_EXPECTED_CALL(Schema_GetModelId(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG,"id","cf7eecbd-1435-45e0-a144-c42bb8c88849"));
STRICT_EXPECTED_CALL(Schema_GetModelVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG,"version","1.2.3"));
STRICT_EXPECTED_CALL(Schema_GetModelDescription(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG,"description","Ice Machine"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"firmwareDetails",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"fields",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"firmwareDetails_t",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int64"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"count",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"fields",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"iceDispenseCount_t",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG,2));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"waterPressure",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"temperature",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"fields",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"pressure_t",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG,3));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"z",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"y",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"x",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"fields",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"accelerometer",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataTypes",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("accelerometer"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"lastReportedIceAcceleration",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int8"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"lastReportedIceLevel",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG,2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"lastReportedTemperature",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG,3));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"serialNumber",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"properties",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"reported",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"desiredTemperature",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"properties",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"desired",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"properties",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"request",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"response",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"reboot",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"packageName",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"request",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG,"firmwareDetails_t"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"firmwareDetails",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"response",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"updateFirmware",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"cloudToDeviceMethods",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG,"pressure_t"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"waterPressure",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"temperature",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"properties",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"pressure",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG,1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG,"iceDispenseCount_t"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG,IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG,0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int64"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"dataType",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"count",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"properties",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"iceDispenseCount",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG,"telemetry",IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_serialize_to_string(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_free(IGNORED_PTR_ARG));
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_002: [ SchemaSerializer_SerializeModelToJSON shall parse the model using Schema APIs and shall produce a string representation of the model by using parson APIs. ]*/
    /*Tests_SRS_SCHEMA_SERIALIZER_02_004: [ Otherwise, SchemaSerializer_SerializeModelToJSON shall return a null terminated string. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_soda_model_happy_path)
    {
        ///arrange
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso", &ALL_REFLECTED(Contoso));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_soda_model_inert_path();

        ///act
        char* result = SchemaSerializer_SerializeModelToJSON(model);

        ///assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        ///cleanup
        json_free_serialized_string(result);
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_soda_model_unhappy_paths)
    {
        ///arrange
        umock_c_negative_tests_init();
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso", &ALL_REFLECTED(Contoso));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_soda_model_inert_path();

        umock_c_negative_tests_snapshot();

        for (size_t i = 0; i < umock_c_negative_tests_call_count(); i+=10)
        {
            if (i == 265) /*JSON_value_free*/
            {
                continue;
            }

            umock_c_negative_tests_reset();

            umock_c_negative_tests_fail_call(i);

            char temp_str[128];
            sprintf(temp_str, "On failed call %zu", i);

            ///act
            char* result = SchemaSerializer_SerializeModelToJSON(model);

            ///assert
            ASSERT_IS_NULL_WITH_MSG(result, temp_str);
        }

        ///cleanup
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
        umock_c_negative_tests_deinit();
    }

    static void SchemaSerializer_2_with_soda_model_inert_path2(void)
    {
STRICT_EXPECTED_CALL(Schema_GetModelSchema(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "name", "Soda_Machine_Device_Model2"));
STRICT_EXPECTED_CALL(Schema_GetModelSchemaVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "schemaVersion", ""));
STRICT_EXPECTED_CALL(Schema_GetModelId(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "id", "00000000-0000-0000-0000-000000000000"));
STRICT_EXPECTED_CALL(Schema_GetModelVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "version", ""));
STRICT_EXPECTED_CALL(Schema_GetModelDescription(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "description", ""));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "firmwareDetails2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "firmwareDetails_t2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int64"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "count2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "iceDispenseCount_t2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "waterPressure2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "temperature2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "pressure_t2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 3));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "z2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "accelerometer2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataTypes", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("accelerometer2"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "lastReportedIceAcceleration2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int8"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "lastReportedIceLevel2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "lastReportedTemperature2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 3));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "serialNumber2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reported", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desiredTemperature2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desired", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reboot2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "packageName2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "firmwareDetails_t2"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "firmwareDetails2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "updateFirmware2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "cloudToDeviceMethods", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "pressure_t2"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("double"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "waterPressure2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int16"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "temperature2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "pressure2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "iceDispenseCount_t2"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int64"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "count2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "iceDispenseCount2", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "telemetry", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_serialize_to_string(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_free(IGNORED_PTR_ARG));
    }


    /*Tests_SRS_SCHEMA_SERIALIZER_02_002: [ SchemaSerializer_SerializeModelToJSON shall parse the model using Schema APIs and shall produce a string representation of the model by using parson APIs. ]*/
    /*Tests_SRS_SCHEMA_SERIALIZER_02_004: [ Otherwise, SchemaSerializer_SerializeModelToJSON shall return a null terminated string. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_soda_model_happy_path2)
    {
        ///arrange
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso2", &ALL_REFLECTED(Contoso2));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model2");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_soda_model_inert_path2();

        ///act
        char* result = SchemaSerializer_SerializeModelToJSON(model);

        ///assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        ///cleanup
        json_free_serialized_string(result);
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_soda_model_unhappy_paths2)
    {
        ///arrange
        umock_c_negative_tests_init();
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso2", &ALL_REFLECTED(Contoso2));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model2");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_soda_model_inert_path2();

        umock_c_negative_tests_snapshot();

        for (size_t i = 0; i < umock_c_negative_tests_call_count(); i+=10)
        {
            if (i == 265) /*JSON_value_free*/
            {
                continue;
            }

            umock_c_negative_tests_reset();

            umock_c_negative_tests_fail_call(i);

            char temp_str[128];
            sprintf(temp_str, "On failed call %zu", i);

            ///act
            char* result = SchemaSerializer_SerializeModelToJSON(model);

            ///assert
            ASSERT_IS_NULL_WITH_MSG(result, temp_str);
        }

        ///cleanup
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
        umock_c_negative_tests_deinit();
    }

    void SchemaSerializer_2_with_complex_model_inert_path(void)
    {
STRICT_EXPECTED_CALL(Schema_GetModelSchema(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "name", "Soda_Machine_Device_Model_Plus_Plus"));
STRICT_EXPECTED_CALL(Schema_GetModelSchemaVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "schemaVersion", "1.0.0"));
STRICT_EXPECTED_CALL(Schema_GetModelId(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "id", "cf7eecbd-1435-45e0-a144-c42bb8c88849"));
STRICT_EXPECTED_CALL(Schema_GetModelVersion(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "version", "1.2.3"));
STRICT_EXPECTED_CALL(Schema_GetModelDescription(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "description", "Ice Machine with a tint of soda"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetStructTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "fields", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataTypes", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reported_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reported_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelReportedPropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetReportedPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reported_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "reported", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desired_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desired_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelDesiredPropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetDesiredPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desired_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "desired", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_b_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 3));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_a_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 4));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_a_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 5));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 6));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_int_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 7));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_int_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 8));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_int_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 9));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 10));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 11));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 12));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_b_void", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 13));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_b_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 14));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 15));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 16));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_a_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 17));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_a_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 18));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 19));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_int_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 20));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_int_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 21));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_int_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 22));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 23));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 24));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 25));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "data_a_void", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 26));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_b_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 27));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 28));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 29));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_a_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 30));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_a_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 31));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 32));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_int_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 33));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_int_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 34));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_int_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 35));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 36));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 37));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeByIndex(IGNORED_PTR_ARG, 38));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodWithReturnTypeReturnType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "response", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodWithReturnTypeName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "void_void", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_b_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 3));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_a_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 4));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_a_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 5));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 6));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_int_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 7));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_int_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 8));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "x", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "y", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_int_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 9));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_b"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 10));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 11));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetMethodArgumentName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodByIndex(IGNORED_PTR_ARG, 12));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelMethodArgumentCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "request", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_string(IGNORED_PTR_ARG, "response", "any"));
STRICT_EXPECTED_CALL(Schema_GetModelMethodName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "any", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "cloudToDeviceMethods", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_b"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 2));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("data_a"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b_data_a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "b", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetModelPropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypeByName(IGNORED_PTR_ARG, "data_a"));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyCount(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 0));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("string"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_string", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetStructTypePropertyByIndex(IGNORED_PTR_ARG, 1));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_object());
STRICT_EXPECTED_CALL(json_value_get_object(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyType(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_init_string("int"));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "dataType", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a_int", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "properties", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(Schema_GetPropertyName(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "a", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_object_set_value(IGNORED_PTR_ARG, "telemetry", IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_serialize_to_string(IGNORED_PTR_ARG));
STRICT_EXPECTED_CALL(json_value_free(IGNORED_PTR_ARG));
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_002: [ SchemaSerializer_SerializeModelToJSON shall parse the model using Schema APIs and shall produce a string representation of the model by using parson APIs. ]*/
    /*Tests_SRS_SCHEMA_SERIALIZER_02_004: [ Otherwise, SchemaSerializer_SerializeModelToJSON shall return a null terminated string. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_complex_model_happy_path)
    {
        ///arrange
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso_plus_plus", &ALL_REFLECTED(Contoso_plus_plus));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model_Plus_Plus");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_complex_model_inert_path();

        ///act
        char* result = SchemaSerializer_SerializeModelToJSON(model);

        ///assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        ///cleanup
        json_free_serialized_string(result);
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
    }

    /*Tests_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    TEST_FUNCTION(SchemaSerializer_2_with_complex_model_unhappy_paths)
    {
        ///arrange
        umock_c_negative_tests_init();
        CodeFirst_Init(NULL);
        SCHEMA_HANDLE schemaHandle = CodeFirst_RegisterSchema("Contoso_plus_plus", &ALL_REFLECTED(Contoso_plus_plus));
        SCHEMA_MODEL_TYPE_HANDLE model = Schema_GetModelByName(schemaHandle, "Soda_Machine_Device_Model_Plus_Plus");
        umock_c_reset_all_calls();

        SchemaSerializer_2_with_complex_model_inert_path();

        umock_c_negative_tests_snapshot();

        for (size_t i = 0; i < umock_c_negative_tests_call_count(); i+=80) /*running these tests with ++ takes 971 seconds under valgrind and that's not acceptable for the gate.*/
        {
            if (i == 2080) /*JSON_value_free*/
            {
                continue;
            }

            umock_c_negative_tests_reset();

            umock_c_negative_tests_fail_call(i);

            char temp_str[128];
            sprintf(temp_str, "On failed call %zu", i);

            ///act
            char* result = SchemaSerializer_SerializeModelToJSON(model);

            ///assert
            ASSERT_IS_NULL_WITH_MSG(result, temp_str);
        }

        ///cleanup
        Schema_Destroy(schemaHandle);
        CodeFirst_Deinit();
        umock_c_negative_tests_deinit();
    }

END_TEST_SUITE(SchemaSerializer_2_ut)

