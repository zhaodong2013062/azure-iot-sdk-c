// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

'use strict';
var prov_svc_client = require("./node_prov_service.js")
var uuid = require('uuid');

const { spawn } = require('child_process');

function execute_test_process(test_process, cmd_args)
{
    const test_ps = spawn(test_process, cmd_args);

    //ls.stdout.on('data', (data) => {
     // console.log(`stdout: ${data}`);
    //});

    //ls.stderr.on('data', (data) => {
    //  console.log(`stderr: ${data}`);
    //});

    test_ps.on('close', (code) => {
      console.log(`child process exited with code ${code}`);
    });
}

var argv = require('yargs')
    .usage('Usage: $0 --connectionstring <DEVICE PROVISIONING CONNECTION STRING> --webhookurl <URL OF THE AZURE FUNCTION> --deviceid <Devcie Id> -t <test type>')
    .option('connectionstring', {
        alias: 'c',
        describe: 'The connection string for the Device Provisioning instance',
        type: 'string',
        demandOption: true
    })
    .option('test_type', {
        alias: 't',
        describe: 'The type of test that is to be completed: x509, tpm, symm_key',
        type: 'string',
        demandOption: true
    })
    .option('deviceid', {
        alias: 'd',
        describe: 'Unique identifier for the device that shall be created',
        type: 'string',
        demandOption: false
    })
    .option('symmKey', {
        alias: 's',
        describe: 'Symmetric Key to be register',
        type: 'string',
        demandOption: false
    })
    .option('x509Cert', {
        alias: 'x',
        describe: 'x.509 certificate to be register',
        type: 'string',
        demandOption: false
    })
    .option('webhookurl', {
        alias: 'w',
        describe: 'The URL of the Azure Webhook to use',
        type: 'string',
        demandOption: false
    })
    .argv;

var device_id = argv.deviceid;
var conn_string = argv.connectionstring;
var webhookuri = argv.webhookurl;
var test_type = argv.test_type;
var test_exe = "";

if (typeof conn_string == 'undefined' || conn_string.length == 0) {
    console.log("Invalid connection string specified");
}
else {
    if (typeof device_id ==  'undefined' || device_id.length == 0)
    {
        // Generate a device name
        device_id = "prov_e2e_dev_" + uuid.v4();
    }

    if (test_type == "x509") {
        var x509_cert = argv.x509Cert;
        if (typeof x509_cert == 'undefined' || x509_cert.length == 0) {
            console.log("Invalid x509 certificate specified");
        }
        else {
            prov_svc_client.create_x509_registration(conn_string, device_id, x509_cert, webhookuri);
        }
    }
    else if (test_type == "symm_key") {
        var symm_key = argv.symmKey;
        prov_svc_client.create_symm_key_registration(conn_string, device_id, symm_key, webhookuri);
    }
    else if (test_type == "tpm") {
        prov_svc_client.create_tpm_registration(conn_string, device_id, webhookuri);
    }

    if (test_exe.length > 0) {
        execute_test_process(test_process, cmd_args);
    }
    // open the iot hub and wait for a telemetry message from the device
}