# SchemaSerializer Requirements


## Overview

SchemaSerializer is a module that serializes the schema for one device to a JSON text.
## Exposed API
```c
#define SCHEMA_SERIALIZER_RESULT_VALUES	\
    SCHEMA_SERIALIZER_OK,			\
    SCHEMA_SERIALIZER_INVALID_ARG,  \
    SCHEMA_SERIALIZER_ERROR

DEFINE_ENUM(SCHEMA_SERIALIZER_RESULT, SCHEMA_SERIALIZER_RESULT_VALUES)

#include "azure_c_shared_utility/umock_c_prod.h"
MOCKABLE_FUNCTION(,SCHEMA_SERIALIZER_RESULT, SchemaSerializer_SerializeCommandMetadata, SCHEMA_MODEL_TYPE_HANDLE, modelHandle, STRING_HANDLE, schemaText);
MOCKABLE_FUNCTION(, char*, SchemaSerializer_SerializeModelToJSON, SCHEMA_MODEL_TYPE_HANDLE, handle)
```


SchemaSerializer_SerializeCommandMetadata
```c
extern SCHEMA_SERIALIZER_RESULT SchemaSerializer_SerializeCommandMetadata(SCHEMA_MODEL_TYPE_HANDLE modelHandle, STRING_HANDLE schemaText);
```

**SRS_SCHEMA_SERIALIZER_01_001: [** SchemaSerializer_SerializeCommandMetadata shall serialize a specific model to a string using JSON as format. **]**

A sample JSON result looks like:
```json
[
      {
         "Name":"ChangeKey",
         "Parameters":[
            {
               "Name":"Key",
               "Type":"string"
            },
            {
               "Name":"Key2",
               "Type":"string"
            }
         ]
      },
      {
         "Name":"ChangeConfig",
         "Parameters":[
            {
               "Name":"AppConfig",
               "Type":"string"
            }
         ]
      }
]
```

**SRS_SCHEMA_SERIALIZER_01_002: [** Only commands shall be serialized, the properties of a model shall be ignored. **]**
**SRS_SCHEMA_SERIALIZER_01_003: [** The output JSON shall have an array, where each array element shall represent a command. **]**
**SRS_SCHEMA_SERIALIZER_01_005: [** Each array element shall be a JSON object. **]**
**SRS_SCHEMA_SERIALIZER_01_006: [** The object for a command shall have a member named "Name", whose value shall be the command name as obtained by using Schema APIs. **]**
**SRS_SCHEMA_SERIALIZER_01_007: [** The object for a command shall also have a "Parameters" member. **]**
**SRS_SCHEMA_SERIALIZER_01_008: [** The parameters member shall be an array, where each entry is a command parameter. **]**
**SRS_SCHEMA_SERIALIZER_01_009: [** Each command parameter shall have a member named "Name", that should have as value the command argument name as obtained by using Schema APIs. **]**
**SRS_SCHEMA_SERIALIZER_01_010: [** Each command parameter shall have a member named "Type", that should have as value the command argument type as obtained by using Schema APIs. **]**

**SRS_SCHEMA_SERIALIZER_01_011: [** The JSON text shall be built into the string indicated by the schemaText argument by using String APIs. **]**

**SRS_SCHEMA_SERIALIZER_01_012: [** On success SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_OK. **]**

**SRS_SCHEMA_SERIALIZER_01_013: [** If the modelHandle argument is NULL, SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_INVALID_ARG. **]**

**SRS_SCHEMA_SERIALIZER_01_014: [** If the schemaText argument is NULL, SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_INVALID_ARG. **]**

**SRS_SCHEMA_SERIALIZER_01_015: [** If any of the Schema or String APIs fail then SchemaSerializer_SerializeCommandMetadata shall return SCHEMA_SERIALIZER_ERROR. **]**

The following mappings should be applied between when serializing the argument type:
**SRS_SCHEMA_SERIALIZER_01_016: [** "ascii_char_ptr" shall be translated to "string". **]**

**SRS_SCHEMA_SERIALIZER_01_017: [** All other types shall be kept as they are. **]**

### SchemaSerializer_SerializeModelToJSON
```c
char* SchemaSerializer_SerializeModelToJSON(SCHEMA_MODEL_TYPE_HANDLE handle)
```

SchemaSerializer_SerializeModelToJSON produces a null terminated string containing the JSON representation of the MODEL.
The produced string needs to be freed by the called by a call to `json_free_serialized_string`.

**SRS_SCHEMA_SERIALIZER_02_001: [** If `handle` is `NULL` then `SchemaSerializer_SerializeModelToJSON` shall fail and return `NULL`. **]**

**SRS_SCHEMA_SERIALIZER_02_002: [** `SchemaSerializer_SerializeModelToJSON` shall parse the model using `Schema` APIs and shall produce a string representation of the model by using `parson` APIs. **]**

These are the transformations that will occur:
- schemaVersion of `handle` becomes the value of "schemaVersion" in the output
- id of `handle` becomes the value of "id" in the output
- version of `handle` becomes the value of "version" in the output
- description of `handle` becomes the value of "description" in the output
- the name of the model becomes the value of "name" in the output
- a "dataTypes" name will be created that. For every structType `handle`' namespace, a separate name/value will be created in "dataTypes". 
    a) the name will be structType's name
    b) the value will be {"fields": {"nameOfTheField": "type"}}
- reported properties and desired properties will each be serializer into json in "properties": { "reported": {"properties": ... }, "desired": { "properties": ... } } section.
    a) every reported/desired property will have a "name" and a "type"
- cloud to device methods will be serialized in "cloudToDevice" name: value. The value will be a json object.
    a) every entry in the json object will have as name: the name of the method, value shall be a json object with
        1. "request": parameters
        2. "response": parameters
- telemetry shall be serialized under "telemetry" name, indicating its name and type

**SRS_SCHEMA_SERIALIZER_02_003: [** If any `Schema` or `parson` APIs fail then `SchemaSerializer_SerializeModelToJSON` shall fail and return `NULL`. **]**

**SRS_SCHEMA_SERIALIZER_02_004: [** Otherwise, `SchemaSerializer_SerializeModelToJSON` shall return a null terminated string. **]**



