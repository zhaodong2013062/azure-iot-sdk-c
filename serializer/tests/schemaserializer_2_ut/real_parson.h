// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_PARSON_H
#define REAL_PARSON_H
#define json_parse_file                                          real_json_parse_file
#define json_parse_file_with_comments                            real_json_parse_file_with_comments
#define json_parse_string                                        real_json_parse_string
#define json_parse_string_with_comments                          real_json_parse_string_with_comments
#define json_serialization_size                                  real_json_serialization_size
#define json_serialize_to_buffer                                 real_json_serialize_to_buffer
#define json_serialize_to_file                                   real_json_serialize_to_file
#define json_serialize_to_string                                 real_json_serialize_to_string
#define json_serialization_size_pretty                           real_json_serialization_size_pretty
#define json_serialize_to_buffer_pretty                          real_json_serialize_to_buffer_pretty
#define json_serialize_to_file_pretty                            real_json_serialize_to_file_pretty
#define json_serialize_to_string_pretty                          real_json_serialize_to_string_pretty
#define json_free_serialized_string                              real_json_free_serialized_string
#define json_value_equals                                        real_json_value_equals
#define json_validate                                            real_json_validate
#define json_object_get_value                                    real_json_object_get_value
#define json_object_get_string                                   real_json_object_get_string
#define json_object_get_object                                   real_json_object_get_object
#define json_object_get_array                                    real_json_object_get_array
#define json_object_get_number                                   real_json_object_get_number
#define json_object_get_boolean                                  real_json_object_get_boolean
#define json_object_dotget_value                                 real_json_object_dotget_value
#define json_object_dotget_string                                real_json_object_dotget_string
#define json_object_dotget_object                                real_json_object_dotget_object
#define json_object_dotget_array                                 real_json_object_dotget_array
#define json_object_dotget_number                                real_json_object_dotget_number
#define json_object_dotget_boolean                               real_json_object_dotget_boolean
#define json_object_get_count                                    real_json_object_get_count
#define json_object_get_name                                     real_json_object_get_name
#define json_object_get_value_at                                 real_json_object_get_value_at
#define json_object_has_value                                    real_json_object_has_value
#define json_object_has_value_of_type                            real_json_object_has_value_of_type
#define json_object_dothas_value                                 real_json_object_dothas_value
#define json_object_dothas_value_of_type                         real_json_object_dothas_value_of_type
#define json_object_set_value                                    real_json_object_set_value
#define json_object_set_string                                   real_json_object_set_string
#define json_object_set_number                                   real_json_object_set_number
#define json_object_set_boolean                                  real_json_object_set_boolean
#define json_object_set_null                                     real_json_object_set_null
#define json_object_dotset_value                                 real_json_object_dotset_value
#define json_object_dotset_string                                real_json_object_dotset_string
#define json_object_dotset_number                                real_json_object_dotset_number
#define json_object_dotset_boolean                               real_json_object_dotset_boolean
#define json_object_dotset_null                                  real_json_object_dotset_null
#define json_object_remove                                       real_json_object_remove
#define json_object_dotremove                                    real_json_object_dotremove
#define json_object_clear                                        real_json_object_clear
#define json_array_get_value                                     real_json_array_get_value
#define json_array_get_string                                    real_json_array_get_string
#define json_array_get_object                                    real_json_array_get_object
#define json_array_get_array                                     real_json_array_get_array
#define json_array_get_number                                    real_json_array_get_number
#define json_array_get_boolean                                   real_json_array_get_boolean
#define json_array_get_count                                     real_json_array_get_count
#define json_array_remove                                        real_json_array_remove
#define json_array_replace_value                                 real_json_array_replace_value
#define json_array_replace_string                                real_json_array_replace_string
#define json_array_replace_number                                real_json_array_replace_number
#define json_array_replace_boolean                               real_json_array_replace_boolean
#define json_array_replace_null                                  real_json_array_replace_null
#define json_array_clear                                         real_json_array_clear
#define json_array_append_value                                  real_json_array_append_value
#define json_array_append_string                                 real_json_array_append_string
#define json_array_append_number                                 real_json_array_append_number
#define json_array_append_boolean                                real_json_array_append_boolean
#define json_array_append_null                                   real_json_array_append_null
#define json_value_init_object                                   real_json_value_init_object
#define json_value_init_array                                    real_json_value_init_array
#define json_value_init_string                                   real_json_value_init_string
#define json_value_init_number                                   real_json_value_init_number
#define json_value_init_boolean                                  real_json_value_init_boolean
#define json_value_init_null                                     real_json_value_init_null
#define json_value_deep_copy                                     real_json_value_deep_copy
#define json_value_free                                          real_json_value_free
#define json_value_get_type                                      real_json_value_get_type
#define json_value_get_object                                    real_json_value_get_object
#define json_value_get_array                                     real_json_value_get_array
#define json_value_get_string                                    real_json_value_get_string
#define json_value_get_number                                    real_json_value_get_number
#define json_value_get_boolean                                   real_json_value_get_boolean
#define json_type                                                real_json_type
#define json_object                                              real_json_object
#define json_array                                               real_json_array
#define json_string                                              real_json_string
#define json_number                                              real_json_number
#define json_boolean                                             real_json_boolean

#include "parson.h"

#ifndef COMPILING_REAL_PARSON_C
#undef json_parse_file
#undef json_parse_file_with_comments
#undef json_parse_string
#undef json_parse_string_with_comments
#undef json_serialization_size
#undef json_serialize_to_buffer
#undef json_serialize_to_file
#undef json_serialize_to_string
#undef json_serialization_size_pretty
#undef json_serialize_to_buffer_pretty
#undef json_serialize_to_file_pretty
#undef json_serialize_to_string_pretty
#undef json_free_serialized_string
#undef json_value_equals
#undef json_validate
#undef json_object_get_value
#undef json_object_get_string
#undef json_object_get_object
#undef json_object_get_array
#undef json_object_get_number
#undef json_object_get_boolean
#undef json_object_dotget_value
#undef json_object_dotget_string
#undef json_object_dotget_object
#undef json_object_dotget_array
#undef json_object_dotget_number
#undef json_object_dotget_boolean
#undef json_object_get_count
#undef json_object_get_name
#undef json_object_get_value_at
#undef json_object_has_value
#undef json_object_has_value_of_type
#undef json_object_dothas_value
#undef json_object_dothas_value_of_type
#undef json_object_set_value
#undef json_object_set_string
#undef json_object_set_number
#undef json_object_set_boolean
#undef json_object_set_null
#undef json_object_dotset_value
#undef json_object_dotset_string
#undef json_object_dotset_number
#undef json_object_dotset_boolean
#undef json_object_dotset_null
#undef json_object_remove
#undef json_object_dotremove
#undef json_object_clear
#undef json_array_get_value
#undef json_array_get_string
#undef json_array_get_object
#undef json_array_get_array
#undef json_array_get_number
#undef json_array_get_boolean
#undef json_array_get_count
#undef json_array_remove
#undef json_array_replace_value
#undef json_array_replace_string
#undef json_array_replace_number
#undef json_array_replace_boolean
#undef json_array_replace_null
#undef json_array_clear
#undef json_array_append_value
#undef json_array_append_string
#undef json_array_append_number
#undef json_array_append_boolean
#undef json_array_append_null
#undef json_value_init_object
#undef json_value_init_array
#undef json_value_init_string
#undef json_value_init_number
#undef json_value_init_boolean
#undef json_value_init_null
#undef json_value_deep_copy
#undef json_value_free
#undef json_value_get_type
#undef json_value_get_object
#undef json_value_get_array
#undef json_value_get_string
#undef json_value_get_number
#undef json_value_get_boolean
#undef json_type
#undef json_object
#undef json_array
#undef json_string
#undef json_number
#undef json_boolean

#endif

#endif