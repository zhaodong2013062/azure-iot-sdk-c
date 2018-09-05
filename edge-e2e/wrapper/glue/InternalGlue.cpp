// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <string>
#include <mutex>
#include <condition_variable>

#define USE_EDGE_MODULES
#define USE_PROV_MODULE
#include "iothub_client_edge.h"
#include "iothub_module_client.h"
#include "iothubtransportamqp.h"
#include "iothubtransporthttp.h"
#include "iothubtransportmqtt.h"
#include "azure_c_shared_utility/platform.h"
#include "parson.h"

#include "GlueUtils.h"
#include "InternalGlue.h"

void ThrowIfFailed(IOTHUB_CLIENT_RESULT ret, string functionName)
{
    if (ret != IOTHUB_CLIENT_OK)
    {
        throw new runtime_error(functionName + " returned " + ENUM_TO_STRING(IOTHUB_CLIENT_RESULT, ret));
    }
}

InternalGlue::InternalGlue()
{
    platform_init();
}

InternalGlue::~InternalGlue()
{
}

struct twin_callback_struct {
    std::condition_variable cv;
    std::mutex m;
    string latest_payload;
    string current_complete;
};

string InternalGlue::Connect(string transportType, string connectionString, string caCertificate)
{
    cout << "InternalGlue::Connect for " << transportType << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client;
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;
    if ((client = IoTHubModuleClient_CreateFromConnectionString(connectionString.c_str(), protocol)) == NULL)
    {
        throw new runtime_error("failed to create client");
    }
    else
    {
        string clientId = getNextClientId();
        this->clientMap[clientId] = (void*)client;
        string ret = "{ \"connectionId\" : \"" + clientId + "\"}";
        cout << "returning " << ret << endl;
        return ret;
    }
}

string InternalGlue::ConnectFromEnvironment(string transportType)
{
    cout << "InternalGlue::ConnectFromEnvironment for " << transportType << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client;
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;
    if ((client = IoTHubModuleClient_CreateFromEnvironment(protocol)) == NULL)
    {
        throw new runtime_error("failed to create client");
    }
    else
    {
        string clientId = getNextClientId();
        this->clientMap[clientId] = (void*)client;
        string ret = "{ \"connectionId\" : \"" + clientId + "\"}";
        cout << "returning " << ret << endl;
        return ret;
    }
}

void InternalGlue::Disconnect(string connectionId)
{
    cout << "InternalGlue::Disconnect for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (client)
    {
        this->clientMap.erase(connectionId);
        IoTHubModuleClient_Destroy(client);
    }
    twin_callback_struct* twin_cb = (twin_callback_struct*)this->twinMap[connectionId];
    if (twin_cb)
    {
        this->twinMap.erase(connectionId);
        delete twin_cb;
    }
}

void InternalGlue::EnableInputMessages(string connectionId)
{
    cout << "InternalGlue::EnableInputMessages for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

}

void InternalGlue::EnableMethods(string connectionId)
{
    cout << "InternalGlue::EnableMethods for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }
}

string patch_twin(string prev_complete_twin, string patch)
{
    JSON_Value* twin_root_value;
    JSON_Value* patch_root_value;
    JSON_Object* twin_root_object;
    if ((twin_root_value = json_parse_string(prev_complete_twin.c_str())) == NULL)
    {
        throw new runtime_error("parson error");
    }
    else if ((patch_root_value = json_parse_string(patch.c_str())) == NULL)
    {
        throw new runtime_error("parson error");
    }
    else if ((twin_root_object = json_value_get_object(twin_root_value)) == NULL)
    {
        throw new runtime_error("parson error");
    }
    else if ((json_object_set_value(twin_root_object, "desired", patch_root_value)) != JSONSuccess)
    {
        throw new runtime_error("parson error");
    }

    string updated_twin_s = string(json_serialize_to_string(twin_root_value));
    json_value_free(twin_root_value); //implicitly frees twin_root_object and patch_root_value as well

    return updated_twin_s;
}

void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char* payLoad, size_t size, void* userContextCallback)
{
    cout << "deviceTwinCallback called" << endl;
    twin_callback_struct* response = (twin_callback_struct*)userContextCallback;
    response->latest_payload = string(reinterpret_cast<const char*>(payLoad), size);
    if (update_state == DEVICE_TWIN_UPDATE_COMPLETE)
    {
        response->current_complete = string(reinterpret_cast<const char*>(payLoad), size);
    }
    else
    {
        response->current_complete = patch_twin(response->current_complete, response->latest_payload);
    }
    response->cv.notify_one();
}

void InternalGlue::EnableTwin(string connectionId)
{
    IOTHUB_CLIENT_RESULT ret;

    cout << "InternalGlue::EnableTwin for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    twin_callback_struct* resp = new twin_callback_struct;
    ret = IoTHubModuleClient_SetModuleTwinCallback(client, deviceTwinCallback, resp);
    ThrowIfFailed(ret, "IoTHubModuleClient_SetModuleTwinCallback");

    cout << "waiting for initial Twin response" << endl;
    {
        std::unique_lock<std::mutex> lk(resp->m);
        resp->cv.wait(lk);
    }
    cout << "initial Twin response received" << endl;

    if (resp->latest_payload.empty())
    {
        throw new runtime_error("twin not enabled");
    }
    this->twinMap[connectionId] = (void*)resp;
}

void sendEventCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    cout << "sendEventCallback called with " << ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result) << endl;
    std::condition_variable *cv = (std::condition_variable*)userContextCallback;
    cv->notify_one();
}

void InternalGlue::SendEvent(string connectionId, string eventBody)
{
    cout << "InternalGlue::SendEvent for " << connectionId << endl;
    cout << eventBody << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    std::mutex m;
    std::condition_variable cv;

    IOTHUB_MESSAGE_HANDLE message = IoTHubMessage_CreateFromString(eventBody.c_str());
    cout << "calling IoTHubClient_SendEventAsync" << endl;
    IOTHUB_CLIENT_RESULT ret = IoTHubModuleClient_SendEventAsync(client, message, sendEventCallback, &cv);
    ThrowIfFailed(ret, "IoTHubModuleClient_SendEventAsync");

    cout << "waiting for send confirmation" << endl;
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
    }
    cout << "send confirmation received" << endl;
}

void InternalGlue::SendOutputEvent(string connectionId, string outputName, string eventBody)
{
    cout << "InternalGlue::SendOutputEvent for " << connectionId << " and " << outputName << endl;
    cout << eventBody << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    std::mutex m;
    std::condition_variable cv;

    IOTHUB_MESSAGE_HANDLE message = IoTHubMessage_CreateFromString(eventBody.c_str());
    cout << "calling IoTHubClient_SendEventAsync" << endl;
    IOTHUB_CLIENT_RESULT ret = IoTHubModuleClient_SendEventToOutputAsync(client, message, outputName.c_str(), sendEventCallback, &cv);
    ThrowIfFailed(ret, "IoTHubModuleClient_SendEventToOutputAsync");

    cout << "waiting for send confirmation" << endl;
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
    }
    cout << "send confirmation received";
}

struct message_response_struct {
    std::mutex m;
    std::condition_variable cv;
    string response_string;
};

IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    cout << "receiveMessageCallback called" << endl;
    message_response_struct *response = (message_response_struct*)userContextCallback;
    const char *str = IoTHubMessage_GetString(message);
    if (str)
    {
        response->response_string = str;
    }
    else
    {
        const unsigned char* buffer;
        size_t size;
        IOTHUB_MESSAGE_RESULT ret = IoTHubMessage_GetByteArray(message, &buffer, &size);
        if (ret == IOTHUB_MESSAGE_OK)
        {
            response->response_string.assign((const char*)buffer, size);
        }
        else
        {
            response->response_string = string("WARNING: IoTHubMessage_GetByteArray returned ") + ENUM_TO_STRING(IOTHUB_MESSAGE_RESULT, ret);
            cout << response->response_string << endl;
        }
    }
    cout << "string == " << response->response_string << endl;
    response->cv.notify_one();
    return IOTHUBMESSAGE_ACCEPTED;
}

string InternalGlue::WaitForInputMessage(string connectionId, string inputName)
{
    IOTHUB_CLIENT_RESULT ret;

    cout << "InternalGlue::WaitForInputMessage for " << connectionId << " and " << inputName << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    message_response_struct resp;
    ret = IoTHubModuleClient_SetInputMessageCallback(client, inputName.c_str(), receiveMessageCallback, &resp);
    ThrowIfFailed(ret, "IoTHubModuleClient_SetInputMessageCallback");

    cout << "waiting for input message" << endl;
    try
    {
        std::unique_lock<std::mutex> lk(resp.m);
        resp.cv.wait(lk);
    }
    catch (...)
    {
        ret = IoTHubModuleClient_SetInputMessageCallback(client, inputName.c_str(), NULL, NULL);
        ThrowIfFailed(ret, "IoTHubModuleClient_SetInputMessageCallback(NULL)");
        throw;
    }

    cout << "input message received" << endl;
    ret = IoTHubModuleClient_SetInputMessageCallback(client, inputName.c_str(), NULL, NULL);
    ThrowIfFailed(ret, "IoTHubModuleClient_SetInputMessageCallback(NULL)");

    return resp.response_string;
}

struct method_callback_struct
{
    std::mutex m;
    std::condition_variable cv;
    string expected_method_name;
    string expected_request_payload;
    string actual_method_name;
    string actual_request_payload;
    string response;
    int status_code;
};

int moduleMethodCallback(const char* method_name, const unsigned char* payload, size_t size, unsigned char** response, size_t* response_size, void* userContextCallback)
{
    cout << "moduleMethodCallback called" << endl;
    int result;

    method_callback_struct *cb_data = (method_callback_struct*)userContextCallback;
    cb_data->actual_method_name = string(reinterpret_cast<const char*>(method_name));
    cb_data->actual_request_payload = string(reinterpret_cast<const char*>(payload), size);

    if (cb_data->actual_method_name.compare(cb_data->expected_method_name) == 0)
    {
        if (cb_data->expected_request_payload.compare(cb_data->actual_request_payload) == 0)
        {
            cout << "method and payload matched.  returning response" << endl;
            *response_size = cb_data->response.length();
            *response = (unsigned char*)malloc(*response_size);
            (void)memcpy(*response, cb_data->response.c_str(), *response_size);
            result = cb_data->status_code;
        }
        else
        {
            cout << "request payload doesn't match" << endl;
            cout << "expected: " << cb_data->expected_request_payload << endl;
            cout << "received: " << cb_data->actual_request_payload << endl;
            result = 500;
        }
    }
    else
    {
        cout << "method name doesn't match" << endl;
        cout << "expected: " << cb_data->expected_method_name << endl;
        cout << "received: " << cb_data->actual_method_name << endl;
        result = 404;
    }

    cb_data->cv.notify_one();
    return result;
}

void parseMethodRequestAndResponse(string requestAndResponse, string *expectedRequest, string *response, int *statusCode)
{
    JSON_Value* root_value;
    JSON_Object* root_object;
    if ((root_value = json_parse_string(requestAndResponse.c_str())) == NULL)
    {
        throw new runtime_error("parson error");
    }
    else if ((root_object = json_value_get_object(root_value)) == NULL)
    {
        throw new runtime_error("parson error");
    }
    *expectedRequest = getJsonObjectAsString(root_object, "requestPayload.payload");
    *response = getJsonObjectAsString(root_object, "responsePayload");
    *statusCode = (int)json_object_get_number(root_object, "statusCode");
    json_value_free(root_value); //implicitly frees root_object as well
}


void InternalGlue::RoundTripMethodCall(string connectionId, string methodName, string requestAndResponse)
{
    IOTHUB_CLIENT_RESULT ret;

    cout << "InternalGlue::RoundTripMethodCall for " << connectionId << " and " << methodName << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    string expectedRequest, response;
    int statusCode;
    parseMethodRequestAndResponse(requestAndResponse, &expectedRequest, &response, &statusCode);

    method_callback_struct cb_data;
    cb_data.expected_method_name = methodName;
    cb_data.expected_request_payload = expectedRequest;
    cb_data.response = response;
    cb_data.status_code = statusCode;
    ret = IoTHubModuleClient_SetModuleMethodCallback(client, moduleMethodCallback, &cb_data);
    ThrowIfFailed(ret, "IoTHubModuleClient_SetModuleMethodCallback");

    cout << "waiting for method call" << endl;
    {
        std::unique_lock<std::mutex> lk(cb_data.m);
        cb_data.cv.wait(lk);
    }
    cout << "method call received" << endl;
}

struct method_invoke_response
{
    std::mutex m;
    std::condition_variable cv;
    int statusCode;
    string payload;
};

void method_invoke_callback(IOTHUB_CLIENT_RESULT result, int responseStatus, unsigned char* responsePayload, size_t responsePayloadSize, void* context)
{
    (void)result;
    (void)responseStatus;
    method_invoke_response* resp = (method_invoke_response*)context;
    resp->statusCode = responseStatus;
    resp->payload = string(reinterpret_cast<const char*>(responsePayload), responsePayloadSize);

    resp->cv.notify_one();
}

string InternalGlue::InvokeModuleMethod(string connectionId, string deviceId, string moduleId, string methodInvokeParameters)
{
    cout << "InternalGlue::InvokeModuleMethod for " << connectionId << " and " << deviceId << " and " << moduleId << endl;
    cout << methodInvokeParameters << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    method_invoke_response response;
    string methodName;
    string payload;
    unsigned int timeout;
    parseMethodInvokeParameters(methodInvokeParameters, &methodName, &payload, &timeout);
    IoTHubModuleClient_ModuleMethodInvokeAsync(client, deviceId.c_str(), moduleId.c_str(), methodName.c_str(), payload.c_str(), timeout, method_invoke_callback, &response);

    cout << "waiting for module method invoke response" << endl;
    {
        std::unique_lock<std::mutex> lk(response.m);
        response.cv.wait(lk);
    }
    cout << "module method invoke response received" << endl;
    return makeInvokeResponse(response.statusCode, response.payload);
}

string InternalGlue::InvokeDeviceMethod(string connectionId, string deviceId, string methodInvokeParameters)
{
    cout << "InternalGlue::InvokeDeviceMethod for " << connectionId << " and " << deviceId << endl;
    cout << methodInvokeParameters << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    method_invoke_response response;
    string methodName;
    string payload;
    unsigned int timeout;
    parseMethodInvokeParameters(methodInvokeParameters, &methodName, &payload, &timeout);
    IoTHubModuleClient_DeviceMethodInvokeAsync(client, deviceId.c_str(), methodName.c_str(), payload.c_str(), timeout, method_invoke_callback, &response);

    cout << "waiting for device method invoke response" << endl;
    {
        std::unique_lock<std::mutex> lk(response.m);
        response.cv.wait(lk);
    }
    cout << "device method invoke response received" << endl;

    return makeInvokeResponse(response.statusCode, response.payload);
}

string InternalGlue::WaitForDesiredPropertyPatch(string connectionId)
{
    cout << "InternalGlue::WaitForDesiredPropertyPatch for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    twin_callback_struct* resp = (twin_callback_struct*)(this->twinMap[connectionId]);
    if (!resp)
    {
        throw new runtime_error("no twin callback struct");
    }

    cout << "waiting for Twin patch response" << endl;
    {
        std::unique_lock<std::mutex> lk(resp->m);
        resp->cv.wait(lk);
    }
    cout << "Twin patch response received" << endl;

    return resp->latest_payload;
}

string InternalGlue::GetTwin(string connectionId)
{
    cout << "InternalGlue::GetTwin for " << connectionId << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    twin_callback_struct* resp = (twin_callback_struct*)(this->twinMap[connectionId]);
    if (!resp)
    {
        throw new runtime_error("no twin callback struct");
    }

    /* The C SDK's version of get (for now) doesn't actually give the full twin, just the properties.
    Here we wrap those properties within a twin structure*/
    string full_twin = "{ \"properties\": " + resp->current_complete + "}";

    return full_twin;
}

void reportedStateCallback(int status_code, void* userContextCallback)
{
    cout << "reportedStateCallback called with " << status_code << endl;
    std::condition_variable *cv = (std::condition_variable*)userContextCallback;
    cv->notify_one();
}

void InternalGlue::SendTwinPatch(string connectionId, string props)
{
    cout << "InternalGlue::SendTwinPatch for " << connectionId << endl;
    cout << props << endl;
    IOTHUB_MODULE_CLIENT_HANDLE client = (IOTHUB_MODULE_CLIENT_HANDLE)this->clientMap[connectionId];
    if (!client)
    {
        throw new runtime_error("client is not opened");
    }

    std::mutex m;
    std::condition_variable cv;
    const unsigned char* reportedState = (const unsigned char*)props.c_str();
    size_t reportedStateSize = props.length();

    IOTHUB_CLIENT_RESULT res = IoTHubModuleClient_SendReportedState(client, reportedState, reportedStateSize, reportedStateCallback, &cv);
    ThrowIfFailed(res, "IoTHubModuleClient_SendReportedState");

    cout << "waiting for send reported state confirmation" << endl;
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
    }
    cout << "send reported state confirmation received" << endl;
}

void InternalGlue::CleanupResources()
{
    cout << "InternalGlue::CleanupResources called" << endl;
    // copy the map since we're removing things from it while we're iterating over it.
    map<string, void*> mapCopy = this->clientMap;
    for (auto iter = mapCopy.begin(); iter != mapCopy.end(); ++iter)
    {
        cout << "missed cleanup of " << iter->first << endl;
        this->Disconnect(iter->first);
    }
}

