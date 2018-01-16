// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include "azure_c_shared_utility/gballoc.h"

#include <stddef.h>
#include "schemaserializer.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/macro_utils.h"
#include "parson.h"
#include "schema.h"

DEFINE_ENUM_STRINGS(SCHEMA_SERIALIZER_RESULT, SCHEMA_SERIALIZER_RESULT_VALUES);

#define LOG_SCHEMA_SERIALIZER_ERROR(result) LogError("(result = %s)", ENUM_TO_STRING(SCHEMA_SERIALIZER_RESULT, (result)))

static const char* ConvertType(const char* sourceType)
{
    /* Codes_SRS_SCHEMA_SERIALIZER_01_016: ["ascii_char_ptr" shall be translated to "string".] */
    if (strcmp(sourceType, "ascii_char_ptr") == 0)
    {
        return "string";
    }
    else
    {
        /* Codes_SRS_SCHEMA_SERIALIZER_01_017: [All other types shall be kept as they are.] */
        return sourceType;
    }
}

/* Codes_SRS_SCHEMA_SERIALIZER_01_001: [SchemaSerializer_SerializeCommandMetadata shall serialize a specific model to a string using JSON as format.] */
SCHEMA_SERIALIZER_RESULT SchemaSerializer_SerializeCommandMetadata(SCHEMA_MODEL_TYPE_HANDLE modelHandle, STRING_HANDLE schemaText)
{
    SCHEMA_SERIALIZER_RESULT result;

    /* Codes_SRS_SCHEMA_SERIALIZER_01_013: [If the modelHandle argument is NULL, SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_INVALID_ARG.] */
    if ((modelHandle == NULL) ||
        /* Codes_SRS_SCHEMA_SERIALIZER_01_014: [If the schemaText argument is NULL, SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_INVALID_ARG.] */
        (schemaText == NULL))
    {
        result = SCHEMA_SERIALIZER_INVALID_ARG;
        LogError("(result = %s), modelHandle = %p, schemaText = %p", ENUM_TO_STRING(SCHEMA_SERIALIZER_RESULT, result), modelHandle, schemaText);
    }
    else
    {
        size_t commandCount;

        /* Codes_SRS_SCHEMA_SERIALIZER_01_002: [Only commands shall be serialized, the properties of a model shall be ignored.] */

        /* Codes_SRS_SCHEMA_SERIALIZER_01_003: [The output JSON shall have an array, where each array element shall represent a command.] */
        /* Codes_SRS_SCHEMA_SERIALIZER_01_011: [The JSON text shall be built into the string indicated by the schemaText argument by using String APIs.] */
        if ((STRING_concat(schemaText, "[") != 0) ||
            /* Codes_SRS_SCHEMA_SERIALIZER_01_006: [The object for a command shall have a member named Name, whose value shall be the command name as obtained by using Schema APIs.] */
            (Schema_GetModelActionCount(modelHandle, &commandCount) != SCHEMA_OK))
        {
            /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
            result = SCHEMA_SERIALIZER_ERROR;
            LOG_SCHEMA_SERIALIZER_ERROR(result);
        }
        else
        {
            size_t i;

            for (i = 0; i < commandCount; i++)
            {
                SCHEMA_ACTION_HANDLE actionHandle = Schema_GetModelActionByIndex(modelHandle, i);
                const char* commandName;
                size_t argCount;
                size_t j;

                /* Codes_SRS_SCHEMA_SERIALIZER_01_005: [Each array element shall be a JSON object.] */
                if ((actionHandle == NULL) ||
                    (STRING_concat(schemaText, "{ \"Name\":\"") != 0) ||
                    ((commandName = Schema_GetModelActionName(actionHandle)) == NULL) ||
                    (STRING_concat(schemaText, commandName) != 0) ||
                    /* Codes_SRS_SCHEMA_SERIALIZER_01_007: [The object for a command shall also have a "Parameters" member.] */
                    (STRING_concat(schemaText, "\", \"Parameters\":[") != 0) ||
                    (Schema_GetModelActionArgumentCount(actionHandle, &argCount) != SCHEMA_OK))
                {
                    /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                    LogError("Failed encoding action.");
                    break;
                }
                else
                {
                    for (j = 0; j < argCount; j++)
                    {
                        /* Codes_SRS_SCHEMA_SERIALIZER_01_008: [The parameters member shall be an array, where each entry is a command parameter.] */
                        SCHEMA_ACTION_ARGUMENT_HANDLE argHandle = Schema_GetModelActionArgumentByIndex(actionHandle, j);
                        const char* argName;
                        const char* argType;

                        /* Codes_SRS_SCHEMA_SERIALIZER_01_009: [Each command parameter shall have a member named "Name", that should have as value the command argument name as obtained by using Schema APIs.] */
                        if ((argHandle == NULL) ||
                            (STRING_concat(schemaText, "{\"Name\":\"") != 0) ||
                            ((argName = Schema_GetActionArgumentName(argHandle)) == NULL) ||
                            (STRING_concat(schemaText, argName) != 0) ||
                            /* Codes_SRS_SCHEMA_SERIALIZER_01_010: [Each command parameter shall have a member named "Type", that should have as value the command argument type as obtained by using Schema APIs.] */
                            (STRING_concat(schemaText, "\",\"Type\":\"") != 0) ||
                            ((argType = Schema_GetActionArgumentType(argHandle)) == NULL) ||
                            (STRING_concat(schemaText, ConvertType(argType)) != 0))
                        {
                            /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                            LogError("Failed encoding argument.");
                            break;
                        }
                        else
                        {
                            if (j + 1 < argCount)
                            {
                                if (STRING_concat(schemaText, "\"},") != 0)
                                {
                                    /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                                    LogError("Failed to concatenate arg end.");
                                    break;
                                }
                            }
                            else
                            {
                                if (STRING_concat(schemaText, "\"}") != 0)
                                {
                                    /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                                    LogError("Failed to concatenate arg end.");
                                    break;
                                }
                            }
                        }
                    }

                    if (j < argCount)
                    {
                        break;
                    }

                    if (i + 1 < commandCount)
                    {
                        if (STRING_concat(schemaText, "]},") != 0)
                        {
                            /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                            LogError("Failed to concatenate.");
                            break;
                        }
                    }
                    else
                    {
                        if (STRING_concat(schemaText, "]}") != 0)
                        {
                            /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                            LogError("Failed to concatenate.");
                            break;
                        }
                    }
                }
            }

            if (i < commandCount)
            {
                result = SCHEMA_SERIALIZER_ERROR;
            }
            else if (STRING_concat(schemaText, "]") != 0)
            {
                /* Codes_SRS_SCHEMA_SERIALIZER_01_015: [If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR.] */
                LogError("Failed to concatenate commands object end.");
                result = SCHEMA_SERIALIZER_ERROR;
            }
            else
            {
                /* Codes_SRS_SCHEMA_SERIALIZER_01_012: [On success SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_OK.] */
                result = SCHEMA_SERIALIZER_OK;
            }
        }
    }

    return result;
}

static struct ctype_to_json_type_tag
{
    const char* c_type;
    const char* json_type;
} mapOfCTypesToJSONTypes[] = 
{ 
    {"double", "double"},
    {"int", "int"}, 
    {"float", "float"}, 
    {"long", "int"},
    {"int8_t", "int8"},
    {"uint8_t", "uint8"},
    {"int16_t", "int16"}, 
    {"int32_t", "int32"},
    {"int64_t", "int64"},
    {"bool", "boolean"}, 
    {"ascii_char_ptr", "string"}, 
    {"EDM_DATE_TIME_OFFSET", "dataTime"},
    {"EDM_GUID", "guid"},
    {"EDM_BINARY", "binary"}
};

/*returns non-zero when it fails*/
/*returns 0 when it did not fail and writes in JSON_Value** output a NULL pointer if type is not C type*/
/*returns 0 when it did not fail and writes in JSON_Value** output a non-NULL pointer if type is C type*/
static int ctype_to_json_type(const char* ctype, JSON_Value** output)
{
    int result;
    if (strcmp(ctype, "void") == 0) /*void is the most special c type*/
    {
        *output = json_value_init_object();
        if (*output == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_init_object");
            result = __FAILURE__;
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        size_t i;
        for (i = 0;i < sizeof(mapOfCTypesToJSONTypes) / sizeof(mapOfCTypesToJSONTypes[0]);i++)
        {
            if (strcmp(mapOfCTypesToJSONTypes[i].c_type, ctype) == 0)
            {
                break;
            }
        }

        if (i < sizeof(mapOfCTypesToJSONTypes) / sizeof(mapOfCTypesToJSONTypes[0]))
        {
            *output = json_value_init_string(mapOfCTypesToJSONTypes[i].json_type);
            if (*output == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_string");
                result = __FAILURE__;
            }
            else
            {
                result = 0;
            }
        }
        else
        {
            result = 0; /*not a failure, but also not a C type*/
            *output = NULL;
        }
    }
    return result;
}

/*returns non-NULL when it does not fail*/
/*this returns a JSON_Value* that is a string that contains the JSON datatype corresponding to a data type from the model*/
/*it will convert C types to JSON types, and structured types will return "as they are"*/
/*this is used to indicate telemetry type and method return types (when they exist)*/
static JSON_Value* ctype_to_json_type_including_structured(SCHEMA_HANDLE schemaHandle, const char* ctype)
{
    JSON_Value* result;
    if (ctype_to_json_type(ctype, &result) != 0)
    {
        LogError("failure in ctype_to_json_type");
        result = NULL;
    }
    else
    {
        if (result != NULL)
        {
            /*return as is, it was a C type*/
        }
        else
        {
            /*not an error, but the type is not a C type, so it must be a structured one*/
            if (Schema_GetStructTypeByName(schemaHandle, ctype) == NULL)
            {
                result = NULL;
                LogError("no such structType by name %s", ctype);
            }
            else
            {
                result = json_value_init_string(ctype);
                if (result == NULL)
                {
                    LogError("failure in json_value_init_string");
                    /*return as is*/
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}



/*
this creates:
{ "dataType": "double" }
*/
static JSON_Value* createDataTypesValueFieldsValueFieldType(SCHEMA_PROPERTY_HANDLE propertyHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            const char* type = Schema_GetPropertyType(propertyHandle);
            if (type == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetPropertyType");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                JSON_Value* json_type;
                if (ctype_to_json_type(type, &json_type) != 0) 
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in ctype_to_json_type");
                    json_value_free(result);
                    result = NULL;

                }
                else
                {
                    if (json_type == NULL)
                    {
                        /*not a C type*/
                        json_type = json_value_init_string(type);
                        if (json_type == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_value_init_string");
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            if (json_object_set_value(object, "dataType", json_type) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(json_type);
                                json_value_free(result);
                                result = NULL;
                            }
                            else
                            {
                                /*return as is*/
                            }
                        }
                    }
                    else
                    {
                        if (json_object_set_value(object, "dataType", json_type) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(json_type);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
        }
    }
    return result;
}

/*
this creates:

   {
        "x": { "dataType": "double" },
        "y": { "dataType": "double" },
        "z": { "dataType": "double" }
    }
*/
static JSON_Value* createDataTypesValueFieldsValue(SCHEMA_STRUCT_TYPE_HANDLE structTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object *object = json_value_get_object(result);
        if(object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            size_t nFields;
            if (Schema_GetStructTypePropertyCount(structTypeHandle, &nFields) != SCHEMA_OK)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetStructTypePropertyCount");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                size_t iField;
                size_t nProcessedFields = 0;
                for (iField = 0; iField < nFields;iField++)
                {
                    SCHEMA_PROPERTY_HANDLE propertyHandle = Schema_GetStructTypePropertyByIndex(structTypeHandle, iField);
                    const char* propertyName = Schema_GetPropertyName(propertyHandle);
                    if (propertyHandle == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetStructTypePropertyByIndex");
                        iField = nFields; /*force it out*/
                    }
                    else
                    {
                        JSON_Value* v = createDataTypesValueFieldsValueFieldType(propertyHandle);
                        if (v == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in createDataTypesValueFieldsValueFieldType");
                            iField = nFields; /*force it out*/
                        }
                        else
                        {
                            if (json_object_set_value(object, propertyName, v) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(v);
                                iField = nFields;
                            }
                            else
                            {
                                nProcessedFields++;
                            }
                        }
                    }
                }

                if(nProcessedFields != nFields)
                { 
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("not all fields processed!");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}


/*
this creates:
{
    "fields": {
        "x": { "dataType": "double" },
        "y": { "dataType": "double" },
        "z": { "dataType": "double" }
    }
*/
static JSON_Value* createDataTypesValueFields(SCHEMA_STRUCT_TYPE_HANDLE structTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            JSON_Value* fieldsValue = createDataTypesValueFieldsValue(structTypeHandle);
            if (fieldsValue == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in createDataTypesValueFieldsValue");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                if (json_object_set_value(object, "fields", fieldsValue) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_object_set_value");
                    json_value_free(fieldsValue);
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}
/*
this creates:
{
    "accelerometer": {
        "fields": {
            "x": { "dataType": "double" },
            "y" : { "dataType": "double" },
            "z" : { "dataType": "double" }
        }
    }
}
*/
static JSON_Value* createDataTypesValue(SCHEMA_HANDLE schemaHandle, SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    (void)modelHandle;
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            size_t nStructs;
            if (Schema_GetStructTypeCount(schemaHandle, &nStructs) != SCHEMA_OK)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetStructTypeCount");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                size_t iStruct;
                size_t nProcessedStructs = 0;
                for (iStruct = 0;iStruct < nStructs;iStruct++)
                {
                    SCHEMA_STRUCT_TYPE_HANDLE structType = Schema_GetStructTypeByIndex(schemaHandle, iStruct);
                    if (structType == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetStructTypeByIndex");
                        iStruct = nStructs;
                    }
                    else
                    {
                        const char* structTypeName = Schema_GetStructTypeName(structType);
                        if (structTypeName == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in Schema_GetStructTypeName");
                            iStruct = nStructs;
                        }
                        else
                        {
                            JSON_Value* fieldsObject = createDataTypesValueFields(structType);
                            if (fieldsObject == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in createDataTypesValueFields");
                                iStruct = nStructs;
                            }
                            else
                            {
                                if (json_object_set_value(object, structTypeName, fieldsObject) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(fieldsObject);
                                    iStruct = nStructs;
                                }
                                else
                                {
                                    nProcessedStructs++;
                                }
                            }
                        }
                    }
                }

                if (nProcessedStructs != nStructs)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure processing all structs");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*all is fine, return as is*/
                }
            }
        }
    }
    
    return result;
}

/*this creates
{ "dataType": "string" }
*/
static JSON_Value* createPropertiesValuePropertiesReportedValuePropertiesValueValue(SCHEMA_REPORTED_PROPERTY_HANDLE reportedPropertyHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            const char* type = Schema_GetReportedPropertyType(reportedPropertyHandle);
            if (type == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetReportedPropertyType");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                JSON_Value* json_type;
                if (ctype_to_json_type(type, &json_type) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in ctype_to_json_type");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {

                    if (json_type == NULL)
                    {
                        json_type = json_value_init_string(type);
                        if (json_type == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_value_init_string");
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            if (json_object_set_value(object, "dataType", json_type) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(json_type);
                                json_value_free(result);
                                result = NULL;
                            }
                            else
                            {
                                /*return as is*/
                            }
                        }
                    }
                    else
                    {
                        if (json_object_set_value(object, "dataType", json_type) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(json_type);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
            
        }
    }
    return result;
}

/*this creates 
{
    "serialNumber": { "dataType": "string" },
    "lastReportedTemperature": { "dataType": "int16" },
    "lastReportedIceLevel": { "dataType": "int8" },
    "lastReportedIceAcceleration": { "dataType": "accelerometer" }
}
*/
/*return 0 if everything went ok. output can be NULL in case there are no reportedProperties*/
static int createPropertiesValuePropertiesReportedValuePropertiesValue(SCHEMA_MODEL_TYPE_HANDLE modelHandle, JSON_Value** output)
{
    int result;
    size_t nReportedProperties;
    if (Schema_GetModelReportedPropertyCount(modelHandle, &nReportedProperties) != SCHEMA_OK)
    {
        LogError("failure in Schema_GetModelReportedPropertyCount");
        result = __FAILURE__;
    }
    else
    {
        if (nReportedProperties == 0)
        {
            *output = NULL;
            result = 0;
        }
        else
        {
            JSON_Value* temp = json_value_init_object();
            if (temp == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_object");
                result = __FAILURE__;
            }
            else
            {
                JSON_Object* object = json_value_get_object(temp);
                if (object == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_value_get_object");
                    json_value_free(temp);
                    result = __FAILURE__;
                }
                else
                {
                    size_t iReportedProperty;
                    size_t nProcessedReportedProperties = 0;
                    for (iReportedProperty = 0; iReportedProperty < nReportedProperties; iReportedProperty++)
                    {
                        SCHEMA_REPORTED_PROPERTY_HANDLE reportedPropertyHandle = Schema_GetModelReportedPropertyByIndex(modelHandle, iReportedProperty);
                        if (reportedPropertyHandle == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in Schema_GetModelReportedPropertyByIndex");
                            iReportedProperty = nReportedProperties; /*force termination of loop*/
                        }
                        else
                        {

                            JSON_Value* v = createPropertiesValuePropertiesReportedValuePropertiesValueValue(reportedPropertyHandle);
                            if (v == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in createPropertiesValuePropertiesReportedValuePropertiesValueValue");
                                iReportedProperty = nReportedProperties;
                            }
                            else
                            {
                                const char* reportedPropertyName = Schema_GetReportedPropertyName(reportedPropertyHandle);
                                if (json_object_set_value(object, reportedPropertyName, v) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(v);
                                    iReportedProperty = nReportedProperties;
                                }
                                else
                                {
                                    nProcessedReportedProperties++;
                                }
                            }
                        }
                    }

                    if (nProcessedReportedProperties != nReportedProperties)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("not all reported properties have been put into json");
                        json_value_free(temp);
                        result = __FAILURE__;
                    }
                    else
                    {
                        *output = temp;
                        result = 0;
                    }
                }
            }
        }
    }
    return result;
}

/*this creates
{
     "properties": {
         "serialNumber": { "dataType": "string" },
         "lastReportedTemperature": { "dataType": "int16" },
         "lastReportedIceLevel": { "dataType": "int8" },
         "lastReportedIceAcceleration": { "dataType": "accelerometer" }
     }
}
*/
/*return 0 if everything went ok. output==NULL is a valid value for when there are no reported properties.*/
static int createPropertiesValuePropertiesReportedValue(SCHEMA_MODEL_TYPE_HANDLE modelHandle, JSON_Value** output)
{
    int result;
    JSON_Value* v;
    if (createPropertiesValuePropertiesReportedValuePropertiesValue(modelHandle, &v) != 0)
    {
        LogError("failure in createPropertiesValuePropertiesReportedValuePropertiesValue");
        result = __FAILURE__;
    }
    else
    {
        if (v == NULL)
        {
            *output = NULL;
            result = 0;
        }
        else
        {
            JSON_Value* temp = json_value_init_object();
            if (temp == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_object");
                json_value_free(v);
                result = __FAILURE__;
            }
            else
            {
                JSON_Object* object = json_value_get_object(temp);
                if (object == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_value_get_object");
                    json_value_free(temp);
                    json_value_free(v);
                    result = __FAILURE__;
                }
                else
                {
                    if(json_object_set_value(object, "properties", v) != 0)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in json_object_set_value");
                        json_value_free(v);
                        json_value_free(temp);
                        result = __FAILURE__;
                    }
                    else
                    {
                        *output = temp;
                        result = 0;
                    }
                }
            }
        }
    }

    return result;
}

/*this creates
{ "dataType": "int16" }
*/
static JSON_Value* createPropertiesValuePropertiesDesiredValuePropertiesValueValue(SCHEMA_DESIRED_PROPERTY_HANDLE desiredPropertyHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            const char* type = Schema_GetDesiredPropertyType(desiredPropertyHandle);
            if (type == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetDesiredPropertyType");
                json_value_free(result);
                result = NULL;
            }
            else
            {

                JSON_Value* json_type;
                if (ctype_to_json_type(type, &json_type) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in ctype_to_json_type");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    if (json_type == NULL)
                    {
                        json_type = json_value_init_string(type);
                        if (json_type == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_value_init_string");
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            if (json_object_set_value(object, "dataType", json_type) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(json_type);
                                json_value_free(result);
                                result = NULL;
                            }
                            else
                            {
                                /*return as is*/
                            }
                        }
                    }
                    else
                    {
                        if (json_object_set_value(object, "dataType", json_type) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(json_type);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
            
        }
    }
    return result;
}

/*this creates
{
    "desiredTemperature": { "dataType": "int16" }
}
*/
/*return 0 if OK. *output can be NULL, in which case there aren o desired properties*/
static int createPropertiesValuePropertiesDesiredValuePropertiesValue(SCHEMA_MODEL_TYPE_HANDLE modelHandle, JSON_Value** output)
{
    int result;
    size_t nDesiredProperties;
    if (Schema_GetModelDesiredPropertyCount(modelHandle, &nDesiredProperties) != SCHEMA_OK)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in Schema_GetModelDesiredPropertyCount");
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_get_object");
        result = __FAILURE__;
    }
    else
    {
        if (nDesiredProperties == 0)
        {
            *output = NULL;
            result = 0;
        }
        else
        {
            JSON_Value* temp = json_value_init_object();
            if (temp == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_object");
                result = __FAILURE__;
            }
            else
            {
                JSON_Object* object = json_value_get_object(temp);
                if (object == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_value_get_object");
                    json_value_free(temp);
                    result = __FAILURE__;
                }
                else
                {
                    size_t iDesiredProperty;
                    size_t nProcessedDesiredProperties = 0;
                    for (iDesiredProperty = 0; iDesiredProperty < nDesiredProperties; iDesiredProperty++)
                    {
                        SCHEMA_DESIRED_PROPERTY_HANDLE desiredPropertyHandle = Schema_GetModelDesiredPropertyByIndex(modelHandle, iDesiredProperty);
                        if (desiredPropertyHandle == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in Schema_GetModelDesiredPropertyByIndex");
                            iDesiredProperty = nDesiredProperties; /*force termination of loop*/
                        }
                        else
                        {
                            JSON_Value* v = createPropertiesValuePropertiesDesiredValuePropertiesValueValue(desiredPropertyHandle);
                            if (v == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in createPropertiesValuePropertiesDesiredValuePropertiesValueValue");
                                iDesiredProperty = nDesiredProperties;
                            }
                            else
                            {
                                const char* desiredPropertyName = Schema_GetDesiredPropertyName(desiredPropertyHandle);
                                if (json_object_set_value(object, desiredPropertyName, v) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(v);
                                    iDesiredProperty = nDesiredProperties;
                                }
                                else
                                {
                                    nProcessedDesiredProperties++;
                                }
                            }
                        }
                    }

                    if (nProcessedDesiredProperties != nDesiredProperties)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("not all desired properties have been put into json");
                        json_value_free(temp);
                        result = __FAILURE__;
                    }
                    else
                    {
                        *output = temp;
                        result = 0;
                    }
                }
            }
        }
    }


    return result;
}

/*this creates
{
    "properties": {
        "desiredTemperature": { "dataType": "int16" }
    }
}
*/
/*return 0 if everything is OK. *output can be NULL, in which case there are no desiredProperties*/
static int createPropertiesValuePropertiesDesiredValue(SCHEMA_MODEL_TYPE_HANDLE modelHandle, JSON_Value** output)
{
    int result;
    JSON_Value *v;
    if (createPropertiesValuePropertiesDesiredValuePropertiesValue(modelHandle, &v) != 0)
    {
        LogError("failure in createPropertiesValuePropertiesDesiredValuePropertiesValue");
        result = __FAILURE__;
    }
    else
    {
        if(v == NULL)
        {
            /*do nothing, no desired properties*/
            *output = NULL;
            result = 0;
        }
        else
        {
            JSON_Value *temp;
            temp = json_value_init_object();
            if (temp == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_object");
                json_value_free(v);
                result = __FAILURE__;
            }
            else
            {
                JSON_Object* object = json_value_get_object(temp);
                if (object == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_value_get_object");
                    json_value_free(temp);
                    json_value_free(v);
                    result = __FAILURE__;
                }
                else
                {
                    if(json_object_set_value(object, "properties", v) != 0)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in json_object_set_value");
                        json_value_free(v);
                        json_value_free(temp);
                        result = __FAILURE__;
                    }
                    else
                    {
                        *output = temp;
                        result = 0;
                    }
                }
            }
        }
    }

    return result;
}

/*this creates
{
    "reported": {
        "properties": {
            "serialNumber": { "dataType": "string" },
            "lastReportedTemperature": { "dataType": "int16" },
            "lastReportedIceLevel": { "dataType": "int8" },
            "lastReportedIceAcceleration": { "dataType": "accelerometer" }
        }
    },
    "desired": {
        "properties": {
            "desiredTemperature": { "dataType": "int16" }
        }
    }
}
*/
static JSON_Value* createPropertiesValue(SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            JSON_Value* reported;
            if (createPropertiesValuePropertiesReportedValue(modelHandle, &reported) != 0)
            {
                LogError("failure in createPropertiesValuePropertiesReportedValue");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                if ((reported != NULL) && (json_object_set_value(object, "reported", reported) != 0))
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_object_set_value");
                    json_value_free(reported);
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    JSON_Value* desired;
                    if (createPropertiesValuePropertiesDesiredValue(modelHandle, &desired) != 0)
                    {
                        LogError("failure in createPropertiesValuePropertiesDesiredValue");
                        json_value_free(result);
                        result = NULL;
                    }
                    else
                    {
                        if ((desired != NULL) && (json_object_set_value(object, "desired", desired) != 0))
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(desired);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
        }
    }
    return result;
}


/*this creates
{ "dataType": "string" }
*/
static JSON_Value* createcloudToDeviceMethodsValueMethodValueArgumentValue(SCHEMA_METHOD_ARGUMENT_HANDLE argumentHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            const char* argumentType = Schema_GetMethodArgumentType(argumentHandle);
            if (argumentType == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("fail;reu in Schema_GetMethodArgumentType");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                JSON_Value* json_type;
                if (ctype_to_json_type(argumentType, &json_type) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in ctype_to_json_type");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    if (json_type == NULL)
                    {
                        json_type = json_value_init_string(argumentType);
                        if (json_type == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_value_init_string");
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            if (json_object_set_value(object, "dataType", json_type) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(json_type);
                                json_value_free(result);
                                result = NULL;
                            }
                            else
                            {
                                /*return as is*/
                            }
                        }
                    }
                    else
                    {
                        if (json_object_set_value(object, "dataType", json_type) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(json_type);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
        }
    }
    return result;
}

/*this creates
{
    "packageName": { "dataType": "string" }
}
*/
static JSON_Value* createcloudToDeviceMethodsValueMethodValueRequestValue(SCHEMA_METHOD_WITH_RETURN_TYPE_HANDLE methodWithReturnTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            size_t nArguments;
            if (Schema_GetModelMethodWithReturnTypeArgumentCount(methodWithReturnTypeHandle, &nArguments) != SCHEMA_OK)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetModelMethodArgumentCount");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                size_t iArgument;
                size_t nProcessedArguments = 0;
                for (iArgument = 0;iArgument< nArguments;iArgument++)
                {
                    SCHEMA_METHOD_ARGUMENT_HANDLE argumentHandle = Schema_GetModelMethodWithReturnTypeArgumentByIndex(methodWithReturnTypeHandle, iArgument);
                    if (argumentHandle == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetModelMethodArgumentByIndex");
                        iArgument = nArguments;
                    }
                    else
                    {
                        JSON_Value *argumentValue = createcloudToDeviceMethodsValueMethodValueArgumentValue(argumentHandle);
                        if (argumentValue == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in createcloudToDeviceMethodsValueMethodValueArgumentValue");
                            iArgument = nArguments;
                        }
                        else
                        {
                            const char* argumentName = Schema_GetMethodArgumentName(argumentHandle);
                            if (argumentName == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failrue in Schema_GetMethodArgumentName");
                                json_value_free(argumentValue);
                                iArgument = nArguments;
                            }
                            else
                            {
                                if (json_object_set_value(object, argumentName, argumentValue) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(argumentValue);
                                    iArgument = nArguments;
                                }
                                else
                                {
                                    nProcessedArguments++;
                                }
                            }
                        }
                    }
                }

                if (nProcessedArguments != nArguments)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("not all method argumetn has been processed");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}

/*this creates
{
    "packageName": { "dataType": "string" }
}
*/
static JSON_Value* createcloudToDeviceMethodsWithNoReturnTypeValueMethodValueRequestValue(SCHEMA_METHOD_HANDLE methodWithNoReturnTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            size_t nArguments;
            if (Schema_GetModelMethodArgumentCount(methodWithNoReturnTypeHandle, &nArguments) != SCHEMA_OK)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetModelMethodArgumentCount");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                size_t iArgument;
                size_t nProcessedArguments = 0;
                for (iArgument = 0;iArgument< nArguments;iArgument++)
                {
                    SCHEMA_METHOD_ARGUMENT_HANDLE argumentHandle = Schema_GetModelMethodArgumentByIndex(methodWithNoReturnTypeHandle, iArgument);
                    if (argumentHandle == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetModelMethodArgumentByIndex");
                        iArgument = nArguments;
                    }
                    else
                    {
                        JSON_Value *argumentValue = createcloudToDeviceMethodsValueMethodValueArgumentValue(argumentHandle);
                        if (argumentValue == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in createcloudToDeviceMethodsValueMethodValueArgumentValue");
                            iArgument = nArguments;
                        }
                        else
                        {
                            const char* argumentName = Schema_GetMethodArgumentName(argumentHandle);
                            if (argumentName == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failrue in Schema_GetMethodArgumentName");
                                json_value_free(argumentValue);
                                iArgument = nArguments;
                            }
                            else
                            {
                                if (json_object_set_value(object, argumentName, argumentValue) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(argumentValue);
                                    iArgument = nArguments;
                                }
                                else
                                {
                                    nProcessedArguments++;
                                }
                            }
                        }
                    }
                }

                if (nProcessedArguments != nArguments)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("not all method argumetn has been processed");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}


/*this creates
{
"firmwareDetails": { "dataType": "string" }
}
*/
static JSON_Value* createcloudToDeviceMethodsValueMethodValueResponseValue(SCHEMA_HANDLE schemaHandle, const char* returnType)
{
    JSON_Value* result;

    JSON_Value* maybe_c_type=NULL;
    /*only "void" is a valid type here, all the others, such as "int" is invalid, because method return types need to be structured*/
    if ( 
        (strcmp("void", returnType)==0) && 
        (ctype_to_json_type(returnType, &maybe_c_type) != 0)
    )
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in ctype_to_json_type");
        result = NULL;
    }
    else
    {
        if (maybe_c_type != NULL)
        {
            result = maybe_c_type;
        }
        else /*not a regular C type, must be a type defined in the schema by a STRUCT*/
        {
            SCHEMA_STRUCT_TYPE_HANDLE structTypeHandle = Schema_GetStructTypeByName(schemaHandle, returnType);
            if (structTypeHandle == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("type not found : [%s], all method return types need to be either void or structured types", returnType);
                result = NULL;
            }
            else
            {
                result = createDataTypesValueFieldsValue(structTypeHandle);
                if (result == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in createDataTypesValueFieldsValue");
                }
                else
                {
                    /*all is fine, return as is*/
                }
            }
        }
    }
    return result;

}
/*this creates:
{
     "request": {
         "packageName": { "dataType": "string" }
     },
     "response": {
         "firmwareDetails": "firmwareDetails_t"
     }
}
*/
static JSON_Value* createcloudToDeviceMethodsWithReturnTypeValueMethodValue(SCHEMA_HANDLE schemaHandle, SCHEMA_METHOD_WITH_RETURN_TYPE_HANDLE methodWithReturnTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            JSON_Value* request = createcloudToDeviceMethodsValueMethodValueRequestValue(methodWithReturnTypeHandle);
            if (request == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in createcloudToDeviceMethodsValueMethodValueRequestValue");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                if (json_object_set_value(object, "request", request) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_object_set_value");
                    json_value_free(request);
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    const char* returnType = Schema_GetModelMethodWithReturnTypeReturnType(methodWithReturnTypeHandle);
                    if (returnType == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetModelMethodWithReturnTypeReturnType");
                        json_value_free(result);
                        result = NULL;
                    }
                    else
                    {
                        JSON_Value* response = createcloudToDeviceMethodsValueMethodValueResponseValue(schemaHandle, returnType);
                        if (response == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in createcloudToDeviceMethodsValueMethodValueResponseValue");
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            if (json_object_set_value(object, "response", response) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_string");
                                json_value_free(response);
                                json_value_free(result);
                                result = NULL;
                            }
                            else
                            {
                                /*return as is*/
                            }
                        }
                    }
                }
            }
        }
        
    }
    return result;
}

/*this creates:
{
    {
        "request": { },
        "response": "any"
    }

    or 

    {
        "request": {
            "quantity": { "dataType": "double" },
            "recipe": { "dataType": "string }
        },
        "response": "any"
    }
}
*/
static JSON_Value* createcloudToDeviceMethodsWithNoReturnTypeValueMethodValue(SCHEMA_METHOD_HANDLE methodWithNoReturnTypeHandle)
{
    JSON_Value* result;
    result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in json_value_get_object");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            JSON_Value* request = createcloudToDeviceMethodsWithNoReturnTypeValueMethodValueRequestValue(methodWithNoReturnTypeHandle);
            if (request == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in createcloudToDeviceMethodsValueMethodValueRequestValue");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                if (json_object_set_value(object, "request", request) != 0)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in json_object_set_value");
                    json_value_free(request);
                    json_value_free(result);
                    result = NULL;
                }
                else
                {

                    if (json_object_set_string(object, "response", "any") != 0)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in json_object_set_string");
                        json_value_free(result);
                        result = NULL;
                    }
                    else
                    {
                        /*return as is*/
                    }
                }
            }
        }
    }
    return result;
}

/*This creates:

    "preheat": {
        "request": { },
        "response": "any"
    },
    "make_soda": {
        "request": {
            "quantity": { "dataType": "double" },
            "recipe": { "dataType": "string }
        },
        "response": "any"
    }

    and puts that in "methods" parameter

    return 0 when success
*/
static int createcloudToDeviceMethodsWithNoReturnTypeValue(JSON_Object* methods, SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    int result;
    size_t nMethodsWithNoReturnType;
    if (Schema_GetModelMethodCount(modelHandle, &nMethodsWithNoReturnType) != SCHEMA_OK)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in Schema_GetModelMethodWithReturnTypeCount");
        result = __FAILURE__;
    }
    else
    {
        size_t nProcessedMethodsWithNoReturnType = 0;
        size_t iMethodWithNoReturnType;
        for (iMethodWithNoReturnType = 0; iMethodWithNoReturnType < nMethodsWithNoReturnType;iMethodWithNoReturnType++)
        {
            SCHEMA_METHOD_HANDLE methodWithNoReturnTypeHandle = Schema_GetModelMethodByIndex(modelHandle, iMethodWithNoReturnType);
            if (methodWithNoReturnTypeHandle == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetModelMethodByIndex");
                iMethodWithNoReturnType = nMethodsWithNoReturnType;
            }
            else
            {
                JSON_Value* methodValue = createcloudToDeviceMethodsWithNoReturnTypeValueMethodValue(methodWithNoReturnTypeHandle);
                if (methodValue == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in createcloudToDeviceMethodsWithReturnTypeValueMethodValue");
                    iMethodWithNoReturnType = nMethodsWithNoReturnType;
                }
                else
                {
                    const char* methodName = Schema_GetModelMethodName(methodWithNoReturnTypeHandle);
                    if (methodName == NULL)
                    {
                        json_value_free(methodValue);
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetMethodName");
                        iMethodWithNoReturnType = nMethodsWithNoReturnType;
                    }
                    else
                    {
                        if (json_object_set_value(methods, methodName, methodValue) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(methodValue);
                            iMethodWithNoReturnType = nMethodsWithNoReturnType;
                        }
                        else
                        {
                            nProcessedMethodsWithNoReturnType++;
                        }
                    }
                }
            }
        }

        if (nProcessedMethodsWithNoReturnType < nMethodsWithNoReturnType)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("not all methods have been put into JSON");
            result = __FAILURE__;
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

/*This creats:

     "reboot": {
         "request": { },
         "response": { }
     },
     "updateFirmware": {
         "request": {
             "packageName": { "dataType": "string" }
         },
         "response": {
             "firmwareDetails": { "dataType": "string" }
     }

     and puts that in "methods" parameter

     return 0 when succeess. any other value is failure
*/
static int createcloudToDeviceMethodsWithReturnTypeValue(SCHEMA_HANDLE schemaHandle, JSON_Object* methods, SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    int result;
    size_t nMethodsWithReturnType;
    if (Schema_GetModelMethodWithReturnTypeCount(modelHandle, &nMethodsWithReturnType) != SCHEMA_OK)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in Schema_GetModelMethodWithReturnTypeCount");
        result = __FAILURE__;
    }
    else
    {
        size_t nProcessedMethodsWithReturnType = 0;
        size_t iMethodWithReturnType;
        for (iMethodWithReturnType = 0; iMethodWithReturnType < nMethodsWithReturnType;iMethodWithReturnType++)
        {
            SCHEMA_METHOD_WITH_RETURN_TYPE_HANDLE methodWithReturnTypeHandle = Schema_GetModelMethodWithReturnTypeByIndex(modelHandle, iMethodWithReturnType);
            if (methodWithReturnTypeHandle == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetModelMethodWithReturnTypeByIndex");
                iMethodWithReturnType = nMethodsWithReturnType;
            }
            else
            {
                JSON_Value* methodValue = createcloudToDeviceMethodsWithReturnTypeValueMethodValue(schemaHandle, methodWithReturnTypeHandle);
                if (methodValue == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in createcloudToDeviceMethodsWithReturnTypeValueMethodValue");
                    iMethodWithReturnType = nMethodsWithReturnType;
                }
                else
                {
                    const char* methodName = Schema_GetMethodWithReturnTypeName(methodWithReturnTypeHandle);
                    if (methodName == NULL)
                    {
                        json_value_free(methodValue);
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetMethodName");
                        iMethodWithReturnType = nMethodsWithReturnType;
                    }
                    else
                    {
                        if (json_object_set_value(methods, methodName, methodValue) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(methodValue);
                            iMethodWithReturnType = nMethodsWithReturnType;
                        }
                        else
                        {
                            nProcessedMethodsWithReturnType++;
                        }
                    }
                }
            }
        }

        if (nProcessedMethodsWithReturnType < nMethodsWithReturnType)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            result = __FAILURE__;
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

/*
{
    "preheat": {
         "request": { },
         "response": "any"
     },
     "make_soda": {
         "request": {
             "quantity": { "dataType": "double" },
             "recipe": { "dataType": "string }
         },
         "response": "any"
    },
    "reboot": {
         "request": { },
         "response": { }
     },
     "updateFirmware": {
         "request": {
             "packageName": { "dataType": "string" }
         },
         "response": {
            "firmwareDetails": {
                "dataType": "string"
            }
         }
     }
}*/
static JSON_Value* createcloudToDeviceMethodsValue(SCHEMA_HANDLE schemaHandle, SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    JSON_Value* result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
        /*return as is*/
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("unexpected error in parson");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            if (createcloudToDeviceMethodsWithReturnTypeValue(schemaHandle, object, modelHandle) != 0)
            {
                LogError("failure in createcloudToDeviceMethodsWithReturnTypeValue");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                if (createcloudToDeviceMethodsWithNoReturnTypeValue(object, modelHandle) != 0)
                {
                    LogError("failure in createcloudToDeviceMethodsWithNoReturnTypeValue");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*all is fine, return as is*/
                }
            }
        }
    }
    return result;
}

JSON_Value* createTelemetryValueValue(SCHEMA_HANDLE schemaHandle, SCHEMA_PROPERTY_HANDLE propertyHandle)
{
    JSON_Value* result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
        result = NULL;
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("unexpected error in parson");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            const char* type = Schema_GetPropertyType(propertyHandle);
            if (type == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failuire in Schema_GetPropertyType");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                SCHEMA_STRUCT_TYPE_HANDLE structTypeHandle = Schema_GetStructTypeByName(schemaHandle, type);
                if (structTypeHandle == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("failure in Schema_GetStructTypeByName (telemetry can only be of structured type)");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    JSON_Value* fields = createDataTypesValueFieldsValue(structTypeHandle);
                    if (fields == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in createDataTypesValueFieldsValue");
                        json_value_free(result);
                        result = NULL;
                    }
                    else
                    {
                        if (json_object_set_value(object, "properties", fields) != 0)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in json_object_set_value");
                            json_value_free(fields);
                            json_value_free(result);
                            result = NULL;
                        }
                        else
                        {
                            /*return as is*/
                        }
                    }
                }
            }
        }
    }
    return result;
}

/*this creates*/
/*{
        "iceDispenseCount": {
            "properties": {
                "count": { "dataType": "int64" }
            }
        },
        "pressure": {
            "properties": {
                "temperature": { "dataType": "int16" },
                "waterPressure": { "dataType": "double" }
            }
        }
    }*/
/*every WITH_DATA from the model creates its own telemetry entry. */
static JSON_Value* createTelemetryValue(SCHEMA_HANDLE schemaHandle, SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    JSON_Value* result = json_value_init_object();
    if (result == NULL)
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
        LogError("failure in json_value_init_object");
        /*return as is*/
    }
    else
    {
        JSON_Object* object = json_value_get_object(result);
        if (object == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("unexpected error in parson");
            json_value_free(result);
            result = NULL;
        }
        else
        {
            size_t nWithData;
            if (Schema_GetModelPropertyCount(modelHandle, &nWithData) != SCHEMA_OK)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in Schema_GetModelPropertyCount");
                json_value_free(result);
                result = NULL;
            }
            else
            {
                size_t iWithData;
                size_t nProcessedWithData = 0;
                for (iWithData = 0; iWithData < nWithData;iWithData++)
                {
                    SCHEMA_PROPERTY_HANDLE propertyHandle = Schema_GetModelPropertyByIndex(modelHandle, iWithData);
                    if (propertyHandle == NULL)
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in Schema_GetModelPropertyByIndex");
                        iWithData = nWithData;
                    }
                    else
                    {
                        JSON_Value* telemetryValueValue = createTelemetryValueValue(schemaHandle, propertyHandle);
                        if (telemetryValueValue == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failed to createTelemetryValueValue");
                            iWithData = nWithData;
                        }
                        else
                        {
                            const char* telemetryName = Schema_GetPropertyName(propertyHandle);
                            if (telemetryName == NULL)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in Schema_GetPropertyName");
                                json_value_free(telemetryValueValue);
                                iWithData = nWithData;
                            }
                            else
                            {
                                if (json_object_set_value(object, telemetryName, telemetryValueValue) != 0)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in json_object_set_value");
                                    json_value_free(telemetryValueValue);
                                    iWithData = nWithData;
                                }
                                else
                                {
                                    nProcessedWithData++;
                                }
                            }
                        }
                    }
                }

                if (nProcessedWithData != nWithData)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("not all WITH_DATA made it");
                    json_value_free(result);
                    result = NULL;
                }
                else
                {
                    /*return as is*/
                }
            }
        }
    }
    return result;
}

/*this creates
{
    "schemaVersion": "1.0.0",
    "id": "cf7eecbd-1435-45e0-a144-c42bb8c88849",
    "version": "1.2.3",
    "name": "Ice Machine",
    "dataTypes": {
        "accelerometer": {
            "fields": {
                "x": { "dataType": "double" },
                "y": { "dataType": "double" },
                "z": { "dataType": "double" }
            }
        }
    },
    "properties": {
        "reported": {
            "properties": {
                "serialNumber": { "dataType": "string" },
                "lastReportedTemperature": { "dataType": "int16" },
                "lastReportedIceLevel": { "dataType": "int8" },
                "lastReportedIceAcceleration": { "dataType": "accelerometer" }
            }
        },
        "desired": {
            "properties": {
                "desiredTemperature": { "dataType": "int16" }
            }
        }
    },
    "cloudToDeviceMethods": {
        "reboot": {
            "request": { },
            "response": { }
        },
        "updateFirmware": {
            "request": {
                "packageName": { "dataType": "string" }
            },
            "response": {
                "firmwareDetails": { "dataType": "string" }
        }
    },
    "deviceToCloudMethods": {
        "getTime": {
            "request": {
                "format": { "dataType": "string" }
            },
            "response": {
                "currentTime": { "dataType": "dateTime" }
        }
    },
    "telemetry": {
        "iceDispenseCount": {
            "properties": {
                "count": { "dataType": "int64" }
            }
        },
        "pressure": {
            "properties": {
                "temperature": { "dataType": "int16" },
                "waterPressure": { "dataType": "double" }
            }
        }
    }
}
*/
char* SchemaSerializer_SerializeModelToJSON(SCHEMA_MODEL_TYPE_HANDLE modelHandle)
{
    char* result;
    /*Codes_SRS_SCHEMA_SERIALIZER_02_001: [ If handle is NULL then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
    if (modelHandle == NULL)
    {
        LogError("invalid argument SCHEMA_MODEL_TYPE_HANDLE handle = %p", modelHandle);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_SCHEMA_SERIALIZER_02_002: [ SchemaSerializer_SerializeModelToJSON shall parse the model using Schema APIs and shall produce a string representation of the model by using parson APIs. ]*/
        SCHEMA_HANDLE schemaHandle = Schema_GetModelSchema(modelHandle);
        if (schemaHandle == NULL)
        {
            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
            LogError("failure in Schema_GetModelSchema");
            result = NULL;
        }
        else
        {
            JSON_Value* value = json_value_init_object();
            if (value == NULL)
            {
                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                LogError("failure in json_value_init_object");
                result = NULL;
            }
            else
            {
                JSON_Object* object = json_value_get_object(value);
                if (object == NULL)
                {
                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                    LogError("unexpected error in parson");
                    result = NULL;
                }
                else
                {
                    if (!(
                        (json_object_set_string(object, "name", Schema_GetModelName(modelHandle)) == 0) &&
                        (json_object_set_string(object, "schemaVersion", Schema_GetModelSchemaVersion(modelHandle)) == 0) &&
                        (json_object_set_string(object, "id", Schema_GetModelId(modelHandle)) == 0) &&
                        (json_object_set_string(object, "version", Schema_GetModelVersion(modelHandle)) == 0) &&
                        (json_object_set_string(object, "description", Schema_GetModelDescription(modelHandle)) == 0)
                        ))
                    {
                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                        LogError("failure in json_object_set_string");
                        result = NULL;
                    }
                    else
                    {
                        JSON_Value* v = createDataTypesValue(schemaHandle, modelHandle);
                        if (v == NULL)
                        {
                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                            LogError("failure in createDataTypesValue");
                            result = NULL;
                        }
                        else
                        {
                            /*add dataTypes*/
                            if (json_object_set_value(object, "dataTypes", v) != 0)
                            {
                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                LogError("failure in json_object_set_value");
                                json_value_free(v);
                                result = NULL;
                            }
                            else
                            {
                                /*add properties*/
                                JSON_Value* properties = createPropertiesValue(modelHandle);
                                if (properties == NULL)
                                {
                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                    LogError("failure in createPropertiesValue");
                                    result = NULL;
                                }
                                else
                                {
                                    if (json_object_set_value(object, "properties", properties) != 0)
                                    {
                                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                        LogError("failure in json_object_set_value");
                                        json_value_free(properties);
                                        result = NULL;
                                    }
                                    else
                                    {
                                        /*add cloudToDeviceMethods*/
                                        JSON_Value* cloudToDeviceMethodsValue = createcloudToDeviceMethodsValue(schemaHandle, modelHandle);
                                        if (cloudToDeviceMethodsValue == NULL)
                                        {
                                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                            LogError("failure in createcloudToDeviceMethodsValue");
                                            result = NULL;
                                        }
                                        else
                                        {
                                            if (json_object_set_value(object, "cloudToDeviceMethods", cloudToDeviceMethodsValue) != 0)
                                            {
                                                /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                                LogError("failure in json_object_set_value");
                                                json_value_free(cloudToDeviceMethodsValue);
                                                result = NULL;
                                            }
                                            else
                                            {
                                                JSON_Value* telemetry = createTelemetryValue(schemaHandle, modelHandle);
                                                if (telemetry == NULL)
                                                {
                                                    /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                                    LogError("failure in createTelemetryValue");
                                                    result = NULL;
                                                }
                                                else
                                                {

                                                    if (json_object_set_value(object, "telemetry", telemetry) != 0)
                                                    {
                                                        /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                                        LogError("failure in json_object_set_value");
                                                        json_value_free(telemetry);
                                                        result = NULL;
                                                    }
                                                    else
                                                    {
                                                        result = json_serialize_to_string(value);
                                                        if (result == NULL)
                                                        {
                                                            /*Codes_SRS_SCHEMA_SERIALIZER_02_003: [ If any Schema or parson APIs fail then SchemaSerializer_SerializeModelToJSON shall fail and return NULL. ]*/
                                                            LogError("failure in json_serialize_to_string");
                                                            /*return as is*/
                                                        }
                                                        else
                                                        {
                                                            /*Codes_SRS_SCHEMA_SERIALIZER_02_004: [ Otherwise, SchemaSerializer_SerializeModelToJSON shall return a null terminated string. ]*/
                                                            /*return as is*/
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                json_value_free(value);
            }
        }
        
    }
    return result;
}
