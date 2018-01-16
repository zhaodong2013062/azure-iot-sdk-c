// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_SCHEMA_H
#define REAL_SCHEMA_H

#define Schema_Create                                                         real_Schema_Create
#define Schema_GetMetadata                                                    real_Schema_GetMetadata
#define Schema_GetSchemaCount                                                 real_Schema_GetSchemaCount
#define Schema_GetSchemaByNamespace                                           real_Schema_GetSchemaByNamespace
#define Schema_GetSchemaForModel                                              real_Schema_GetSchemaForModel
#define Schema_GetSchemaNamespace                                             real_Schema_GetSchemaNamespace
#define Schema_AddDeviceRef                                                   real_Schema_AddDeviceRef
#define Schema_ReleaseDeviceRef                                               real_Schema_ReleaseDeviceRef
#define Schema_CreateModelType                                                real_Schema_CreateModelType
#define Schema_GetSchemaForModelType                                          real_Schema_GetSchemaForModelType
#define Schema_GetModelName                                                   real_Schema_GetModelName
#define Schema_CreateStructType                                               real_Schema_CreateStructType
#define Schema_GetStructTypeName                                              real_Schema_GetStructTypeName
#define Schema_AddStructTypeProperty                                          real_Schema_AddStructTypeProperty
#define Schema_AddModelProperty                                               real_Schema_AddModelProperty
#define Schema_AddModelReportedProperty                                       real_Schema_AddModelReportedProperty
#define Schema_AddModelDesiredProperty                                        real_Schema_AddModelDesiredProperty
#define Schema_AddModelModel                                                  real_Schema_AddModelModel
#define Schema_AddModelInformation                                            real_Schema_AddModelInformation
#define Schema_GetModelSchemaVersion                                          real_Schema_GetModelSchemaVersion
#define Schema_GetModelId                                                     real_Schema_GetModelId
#define Schema_GetModelVersion                                                real_Schema_GetModelVersion
#define Schema_GetModelDescription                                            real_Schema_GetModelDescription
#define Schema_CreateModelAction                                              real_Schema_CreateModelAction
#define Schema_CreateModelMethod                                              real_Schema_CreateModelMethod
#define Schema_AddModelActionArgument                                         real_Schema_AddModelActionArgument
#define Schema_AddModelMethodArgument                                         real_Schema_AddModelMethodArgument
#define Schema_GetModelDesiredProperty_pfDesiredPropertyFromAGENT_DATA_TYPE   real_Schema_GetModelDesiredProperty_pfDesiredPropertyFromAGENT_DATA_TYPE
#define Schema_GetModelDesiredProperty_pfOnDesiredProperty                    real_Schema_GetModelDesiredProperty_pfOnDesiredProperty
#define Schema_GetModelModelByName_Offset                                     real_Schema_GetModelModelByName_Offset
#define Schema_GetModelModelByName_OnDesiredProperty                          real_Schema_GetModelModelByName_OnDesiredProperty
#define Schema_GetModelModelByIndex_Offset                                    real_Schema_GetModelModelByIndex_Offset
#define Schema_GetModelDesiredProperty_offset                                 real_Schema_GetModelDesiredProperty_offset
#define Schema_GetModelDesiredPropertyType                                    real_Schema_GetModelDesiredPropertyType
#define Schema_GetModelDesiredProperty_pfDesiredPropertyDeinitialize          real_Schema_GetModelDesiredProperty_pfDesiredPropertyDeinitialize
#define Schema_GetModelDesiredProperty_pfDesiredPropertyInitialize            real_Schema_GetModelDesiredProperty_pfDesiredPropertyInitialize
#define Schema_GetModelElementByName                                          real_Schema_GetModelElementByName
#define Schema_GetModelCount                                                  real_Schema_GetModelCount
#define Schema_GetModelByName                                                 real_Schema_GetModelByName
#define Schema_GetModelByIndex                                                real_Schema_GetModelByIndex
#define Schema_GetModelPropertyCount                                          real_Schema_GetModelPropertyCount
#define Schema_GetModelPropertyByName                                         real_Schema_GetModelPropertyByName
#define Schema_GetModelPropertyByIndex                                        real_Schema_GetModelPropertyByIndex
#define Schema_GetModelReportedPropertyCount                                  real_Schema_GetModelReportedPropertyCount
#define Schema_GetModelReportedPropertyByName                                 real_Schema_GetModelReportedPropertyByName
#define Schema_GetModelReportedPropertyByIndex                                real_Schema_GetModelReportedPropertyByIndex
#define Schema_GetModelDesiredPropertyCount                                   real_Schema_GetModelDesiredPropertyCount
#define Schema_GetModelDesiredPropertyByName                                  real_Schema_GetModelDesiredPropertyByName
#define Schema_GetModelDesiredPropertyByIndex                                 real_Schema_GetModelDesiredPropertyByIndex
#define Schema_GetModelModelCount                                             real_Schema_GetModelModelCount
#define Schema_GetModelModelByName                                            real_Schema_GetModelModelByName
#define Schema_GetModelModelyByIndex                                          real_Schema_GetModelModelyByIndex
#define Schema_GetModelModelPropertyNameByIndex                               real_Schema_GetModelModelPropertyNameByIndex
#define Schema_ModelPropertyByPathExists                                      real_Schema_ModelPropertyByPathExists
#define Schema_ModelReportedPropertyByPathExists                              real_Schema_ModelReportedPropertyByPathExists
#define Schema_ModelDesiredPropertyByPathExists                               real_Schema_ModelDesiredPropertyByPathExists
#define Schema_GetModelActionCount                                            real_Schema_GetModelActionCount
#define Schema_GetModelActionByName                                           real_Schema_GetModelActionByName
#define Schema_GetModelMethodByName                                           real_Schema_GetModelMethodByName
#define Schema_GetModelActionByIndex                                          real_Schema_GetModelActionByIndex
#define Schema_GetModelActionArgumentCount                                    real_Schema_GetModelActionArgumentCount
#define Schema_GetModelMethodArgumentCount                                    real_Schema_GetModelMethodArgumentCount
#define Schema_GetModelActionName                                             real_Schema_GetModelActionName
#define Schema_GetModelActionArgumentByName                                   real_Schema_GetModelActionArgumentByName
#define Schema_GetModelActionArgumentByIndex                                  real_Schema_GetModelActionArgumentByIndex
#define Schema_GetModelMethodArgumentByIndex                                  real_Schema_GetModelMethodArgumentByIndex
#define Schema_GetActionArgumentName                                          real_Schema_GetActionArgumentName
#define Schema_GetMethodArgumentName                                          real_Schema_GetMethodArgumentName
#define Schema_GetActionArgumentType                                          real_Schema_GetActionArgumentType
#define Schema_GetMethodArgumentType                                          real_Schema_GetMethodArgumentType
#define Schema_GetStructTypeCount                                             real_Schema_GetStructTypeCount
#define Schema_GetStructTypeByName                                            real_Schema_GetStructTypeByName
#define Schema_GetStructTypeByIndex                                           real_Schema_GetStructTypeByIndex
#define Schema_GetStructTypePropertyCount                                     real_Schema_GetStructTypePropertyCount
#define Schema_GetStructTypePropertyByName                                    real_Schema_GetStructTypePropertyByName
#define Schema_GetStructTypePropertyByIndex                                   real_Schema_GetStructTypePropertyByIndex
#define Schema_GetPropertyName                                                real_Schema_GetPropertyName
#define Schema_GetPropertyType                                                real_Schema_GetPropertyType
#define Schema_Destroy                                                        real_Schema_Destroy
#define Schema_DestroyIfUnused                                                real_Schema_DestroyIfUnused
#define Schema_GetReportedPropertyType                                        real_Schema_GetReportedPropertyType
#define Schema_GetReportedPropertyName                                        real_Schema_GetReportedPropertyName
#define Schema_GetDesiredPropertyType                                         real_Schema_GetDesiredPropertyType
#define Schema_GetDesiredPropertyName                                         real_Schema_GetDesiredPropertyName
#define Schema_GetModelMethodWithReturnTypeArgumentCount                      real_Schema_GetModelMethodWithReturnTypeArgumentCount
#define Schema_GetModelMethodWithReturnTypeArgumentByIndex                    real_Schema_GetModelMethodWithReturnTypeArgumentByIndex
#define Schema_GetModelMethodWithReturnTypeCount                              real_Schema_GetModelMethodWithReturnTypeCount
#define Schema_GetModelMethodWithReturnTypeByIndex                            real_Schema_GetModelMethodWithReturnTypeByIndex
#define Schema_GetMethodWithReturnTypeName                                    real_Schema_GetMethodWithReturnTypeName
#define Schema_GetModelSchema                                                 real_Schema_GetModelSchema
#define Schema_CreateModelMethodWithReturnType                                real_Schema_CreateModelMethodWithReturnType
#define Schema_AddModelMethodWithReturnTypeArgument                           real_Schema_AddModelMethodWithReturnTypeArgument
#define Schema_GetModelMethodWithReturnTypeByName                             real_Schema_GetModelMethodWithReturnTypeByName
#define Schema_GetModelMethodWithReturnTypeArgumentByIndex                    real_Schema_GetModelMethodWithReturnTypeArgumentByIndex
#define Schema_GetModelMethodWithReturnTypeName                               real_Schema_GetModelMethodWithReturnTypeName
#define Schema_GetModelMethodWithReturnTypeReturnType                         real_Schema_GetModelMethodWithReturnTypeReturnType
#define Schema_GetModelMethodCount                                            real_Schema_GetModelMethodCount
#define Schema_GetModelMethodByIndex                                          real_Schema_GetModelMethodByIndex
#define Schema_GetModelMethodName                                             real_Schema_GetModelMethodName

#include "schema.h"

#ifndef COMPILING_REAL_SCHEMA_C
#undef Schema_Create
#undef Schema_GetMetadata
#undef Schema_GetSchemaCount
#undef Schema_GetSchemaByNamespace
#undef Schema_GetSchemaForModel
#undef Schema_GetSchemaNamespace
#undef Schema_AddDeviceRef
#undef Schema_ReleaseDeviceRef
#undef Schema_CreateModelType
#undef Schema_GetSchemaForModelType
#undef Schema_GetModelName
#undef Schema_CreateStructType
#undef Schema_GetStructTypeName
#undef Schema_AddStructTypeProperty
#undef Schema_AddModelProperty
#undef Schema_AddModelReportedProperty
#undef Schema_AddModelDesiredProperty
#undef Schema_AddModelModel
#undef Schema_AddModelInformation
#undef Schema_GetModelSchemaVersion
#undef Schema_GetModelId
#undef Schema_GetModelVersion
#undef Schema_GetModelDescription
#undef Schema_CreateModelAction
#undef Schema_CreateModelMethod
#undef Schema_AddModelActionArgument
#undef Schema_AddModelMethodArgument
#undef Schema_GetModelDesiredProperty_pfDesiredPropertyFromAGENT_DATA_TYPE
#undef Schema_GetModelDesiredProperty_pfOnDesiredProperty
#undef Schema_GetModelModelByName_Offset
#undef Schema_GetModelModelByName_OnDesiredProperty
#undef Schema_GetModelModelByIndex_Offset
#undef Schema_GetModelDesiredProperty_offset
#undef Schema_GetModelDesiredPropertyType
#undef Schema_GetModelDesiredProperty_pfDesiredPropertyDeinitialize
#undef Schema_GetModelDesiredProperty_pfDesiredPropertyInitialize
#undef Schema_GetModelElementByName
#undef Schema_GetModelCount
#undef Schema_GetModelByName
#undef Schema_GetModelByIndex
#undef Schema_GetModelPropertyCount
#undef Schema_GetModelPropertyByName
#undef Schema_GetModelPropertyByIndex
#undef Schema_GetModelReportedPropertyCount
#undef Schema_GetModelReportedPropertyByName
#undef Schema_GetModelReportedPropertyByIndex
#undef Schema_GetModelDesiredPropertyCount
#undef Schema_GetModelDesiredPropertyByName
#undef Schema_GetModelDesiredPropertyByIndex
#undef Schema_GetModelModelCount
#undef Schema_GetModelModelByName
#undef Schema_GetModelModelyByIndex
#undef Schema_GetModelModelPropertyNameByIndex
#undef Schema_ModelPropertyByPathExists
#undef Schema_ModelReportedPropertyByPathExists
#undef Schema_ModelDesiredPropertyByPathExists
#undef Schema_GetModelActionCount
#undef Schema_GetModelActionByName
#undef Schema_GetModelMethodByName
#undef Schema_GetModelActionByIndex
#undef Schema_GetModelActionArgumentCount
#undef Schema_GetModelMethodArgumentCount
#undef Schema_GetModelActionName
#undef Schema_GetModelActionArgumentByName
#undef Schema_GetModelActionArgumentByIndex
#undef Schema_GetModelMethodArgumentByIndex
#undef Schema_GetActionArgumentName
#undef Schema_GetMethodArgumentName
#undef Schema_GetActionArgumentType
#undef Schema_GetMethodArgumentType
#undef Schema_GetStructTypeCount
#undef Schema_GetStructTypeByName
#undef Schema_GetStructTypeByIndex
#undef Schema_GetStructTypePropertyCount
#undef Schema_GetStructTypePropertyByName
#undef Schema_GetStructTypePropertyByIndex
#undef Schema_GetPropertyName
#undef Schema_GetPropertyType
#undef Schema_Destroy
#undef Schema_DestroyIfUnused
#undef Schema_GetReportedPropertyType
#undef Schema_GetReportedPropertyName
#undef Schema_GetDesiredPropertyType
#undef Schema_GetDesiredPropertyName
#undef Schema_GetModelMethodWithReturnTypeArgumentCount
#undef Schema_GetModelMethodWithReturnTypeArgumentByIndex
#undef Schema_GetModelMethodWithReturnTypeCount
#undef Schema_GetModelMethodWithReturnTypeByIndex
#undef Schema_GetMethodWithReturnTypeName
#undef Schema_GetModelSchema
#undef Schema_CreateModelMethodWithReturnType
#undef Schema_AddModelMethodWithReturnTypeArgument
#undef Schema_GetModelMethodWithReturnTypeByName
#undef Schema_GetModelMethodWithReturnTypeArgumentByIndex
#undef Schema_GetModelMethodWithReturnTypeName
#undef Schema_GetModelMethodWithReturnTypeReturnType
#undef Schema_GetModelMethodCount
#undef Schema_GetModelMethodByIndex
#undef Schema_GetModelMethodName

#endif

#endif
