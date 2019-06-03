// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

'use strict';
var provisioningServiceClient = require('azure-iot-provisioning-service').ProvisioningServiceClient;

function create_symm_key_registration(conn_string, device_id, symm_key, webhookuri) {
    var prov_svc = provisioningServiceClient.fromConnectionString(conn_string);

    var enrollment = {
        registrationId: device_id,
        deviceID: device_id,
        attestation: {
            type: 'symmetricKey',
            primaryKey: symm_key
        },
        provisioningStatus: 'enabled',
        allocationPolicy: 'custom',
        customAllocationDefinition: {
            webhookUrl: webhookuri,
            apiVersion: '2019-03-31'
        }
    };

    prov_svc.createOrUpdateIndividualEnrollment(enrollment, function(err, enrollmentResponse) {
        if (err) {
            console.log('error creating the enrollment: ' + err);
        }
        else {
            console.log("enrollment record returned: " + JSON.stringify(enrollmentResponse, null, 2))
        }
    });
}

function create_x509_registration(conn_string, device_id, x509_cert, webhookuri) {
    var prov_svc = provisioningServiceClient.fromConnectionString(conn_string);

    var enrollment = {
        registrationId: deviceID,
        deviceID: deviceID,
        attestation: {
            type: 'x509',
            x509: {
                clientCertificates: {
                    primary: {
                        certificate: x509_cert
                    }
                }
            }
        },
        provisioningStatus: 'enabled',
        allocationPolicy: 'custom',
        customAllocationDefinition: {
            webhookUrl: webhookuri,
            apiVersion: '2019-03-31'
        }
    };

    prov_svc.createOrUpdateIndividualEnrollment(enrollment, function(err, enrollmentResponse) {
        if (err) {
            console.log('error creating the enrollment: ' + err);
        }
        else {
            console.log("enrollment record returned: " + JSON.stringify(enrollmentResponse, null, 2))
        }
    });
}

function create_tpm_registration(conn_string, ek, device_id, webhookuri) {
    console.log("Register tpm file");
}

module.exports = {
    create_x509_registration: create_x509_registration,
    create_tpm_registration: create_tpm_registration,
    create_symm_key_registration: create_symm_key_registration
};