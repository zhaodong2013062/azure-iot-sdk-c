
# Using Azure Provisioning Custom HSM to create a secure connection

Securing your IoT hardware device is becoming more important as we see headlines of devices getting compromised.
  Unfortunately creating secure solutions is hard to achieve, and ensuring devices don't leak secrets to the world is
is not a trivial exercise.  This is where the Azure IoT SDK will make this task a little easier.

In this tutorial we will walk you through how to setup a hardware HSM with the Zymbit HSM device, provision your device
with the Azure Device Provisioning Service and send telemetry with to the Azure IoTHub.

## Setting up the hardware

For this tutorial we'll be using the following hardware:

- [Zymbit zymkey HSM device](https://www.zymbit.com/zymkey/)

  - This device is a hardware HSM that will create the certificate and store the public key.

- [Raspberry pi 3](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)

To setup the devices and hardware follow the [instructions](https://community.zymbit.com/t/getting-started-with-zymkey-4i/202) to setup the Raspberry Pi and Zymkey device.

## The Code for this Tutorial

All the code for this tutorial can be found in a branch on the [Azure IotHub SDK's Github Repository](https://github.com/Azure/azure-iot-sdk-c/tree/hsm_secure)

To download the code and modify them for your setup you will need to do the following:

- Download the branch from the git repository

- Initialize the SDK's submodules

- create two directories to house the SDK build and the custom HSM

```Shell
git clone https://github.com/Azure/azure-iot-sdk-c.git -b hsm_secure
cd azure-iot-sdk-c
git submodule update --init
mkdir cmake
cd cmake
mkdir custom_hsm
mkdir dps
```

**Note: For simplicity we shall assume that the directory is in your home directory.  This is by no means a requirement please adjust your paths, as necessary.

## Generating a Certificate

The Azure IoTHub SDK use x.509 certificates for authentication between the device and the IoTHub server. The Azure SDK accepts x.509 certificates in ECC and RSA formats.
  There are detailed instructions on how to create the Certificate on the [Zymbit website](https://community.zymbit.com/t/openssl-apache-setup-generating-csr/107) using OpenSSL so we won't go into detail here,
 but the general operations go as follows here:

- Generate a Certificate Signing Request (CSR)

- Use the CSR to generate a Self-Signed Certificate

- Convert the crt to a PEM format

```Shell
openssl x509 -in mycert.crt -out mycert.pem -outform PEM
```

** Note: The Zymbit website create their certificates in a docker container and we ran this on the Raspberry Pi

Once you have the certificate in PEM format, you are read to create your Custom HSM.

## Create the Custom HSM

The Azure Provisioning SDK uses a custom HSM to communicate with secure hardware to obtain the public certificates and a private alias key.
  In depth details on how to write this the library can be found in the [custom hsm provisioning documentation](https://github.com/Azure/azure-iot-sdk-c/blob/master/provisioning_client/devdoc/using_custom_hsm.md).  There are 3 steps that the custom HSM will need to do:

- Obtain the Zymkey device public certificate information

- Return an Alias Key for the device

- Return the Common Name from the certificate

The SDK ships an [example custom HSM file](https://github.com/Azure/azure-iot-sdk-c/blob/hsm_secure/provisioning_client/samples/custom_hsm_example/custom_hsm_example.c) that we will modify here to use with the zymbit system.

Since the private keys for the certificate never leave the zymbit hardware in the HSM code we return an 'unknown' key value to the Azure SDK.  The SDK treats this value as a black box and will send the value unmodified to the tlsio adapter in the SDK.

Fire up your favorite editor and modify the custom HSM file (~/azure-iot-sdk-c/provisioning_client/samples/custom_hsm_example/custom_hsm_example.c) replacing the COMMON_NAME and CERTIFICATE variables at the top of the file.
  You can change the PRIVATE_KEY value to anything you like, the SDK use the value.

```C
// This sample is provided for sample only.
// For more information please see the devdoc using_custom_hsm.md
static const char* const COMMON_NAME = "<COMMON_NAME_ON_CERT";
static const char* const CERTIFICATE = "-----BEGIN CERTIFICATE-----""\n"
"MIIBpDCCAUoCCQC...""\n"
"<OPENSSL Generated Zymbit Cert>""\n"
"-----END CERTIFICATE-----";
static const char* const PRIVATE_KEY = "unknown_key";
```

**Note: When you copy the certificate into the file you will need to take special attention to the line ending on each line of the certificate.  Notice the ""\n" are required after each line or you will have parsing issues later.

### Building the Custom HSM

After you saved the custom_hsm_example file you will need to build the project.  To into the custom_hsm directory and generate the make files by issuing the cmake commands:

```Shell
cd ~/azure-iot-sdk-c/cmake/
cmake ../../provisioning_client/samples/custom_hsm_example
make
```

This will build the custom HSM as a dynamic library (libcustom_hsm_example.a).  After this we focus on the Azure IoT SDK.

## Update tlsio_openssl to use an engine

The Azure SDK ships with many different TLS adapters to connect with the Azure IoTHub Server.  For this tutorial we'll be working with the OpenSSL adapter
that is configured to use the private key from the Zymbit device for TLS communications.  This is based on the default tlsio_openssl.c and x509_openssl.c adapters that does generic TLS
communications using Openssl.  There were two changes to these file that the SDK requires to use the Zymbit device:

In the OpenSSL TLS adapter we need to load the Zymbit OpenSSL engine into memory

```C
int tlsio_openssl_init(void)
{
    int result;

    ENGINE_load_dynamic();
    ENGINE* e = ENGINE_by_id("zymkey_ssl");
    if (ENGINE_init(e) != 1)
    {
        ..
    }
    else
    {
        ...
        ENGINE_set_default(e, ENGINE_METHOD_EC);
        ...
```

And we needed to remove the passing the certificate private key in the x509_openssl_secure.c since we're going to use the keys in the Zymbit module.

## Connect to DPS Service

Once the Azure SDK changes are complete you should take this opportunity to setup the Device Provisioning Service instance.
  All the information that you need is at your fingertips in the [Azure Device Provisioning Service Documentation](https://docs.microsoft.com/en-us/azure/iot-dps/quick-setup-auto-provision).
  For this exercise we'll be creating an individual enrollment which means that we'll be uploading the certificate that we created in the previous steps to the DPS Portal.  You can follow
   the steps for [enrolling your device into the DPS service](https://docs.microsoft.com/en-us/azure/iot-dps/tutorial-provision-device-to-hub#enroll-the-device)

## Prepare Provisioning Sample

The Azure SDK includes a sample to connect to the DPS Service client, register the device with the IoTHub, get back the iothub credentials and send telemetry to the IoTHub.  To enable this
you just need to add your DPS ID Scope to the sample.  Open your editor and edit the id_scope variable in the [prov_dev_client_ll_sample.c](https://github.com/Azure/azure-iot-sdk-c/blob/master/provisioning_client/samples/prov_dev_client_ll_sample/prov_dev_client_ll_sample.c#L68) sample

```C
static const char* id_scope = "[ID Scope]";
```

By default the sample is configured to use the MQTT protocol and the x.509 authentication, so you don't need to change either of those.

## Building the SDK with the key

After all this is complete you can build the SDK with the custom HSM.  Issue the following commands to start the process:

```Shell
cd ~/azure-iot-sdk-c/cmake/dps
cmake -Duse_prov_client=ON -Duse_amqp=OFF -Duse_http=OFF -Ddont_use_uploadtoblob=ON  -Dhsm_custom_lib=<PATH_TO_REPO>/azure-iot-sdk-c/cmake/custom_hsm/libcustom_hsm_example.a ../..
make
```

This creates the binary that will be used to send the data to DPS.  All that's left is to issue the command to
registers the data

```Shell
./provisioning_client/samples/prov_dev_client_ll_sample/prov_dev_client_ll_sample
```

Your device should register with the DPS Service and then connect to the IoTHub Service and send 2 messages to the Hub.
