# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for
# full license information.

import sys, getopt
import subprocess
import logging
import time
import random
import string
import os

from provisioningserviceclient import ProvisioningServiceClient
from provisioningserviceclient.models import IndividualEnrollment, AttestationMechanism

from queue import Queue
from subprocess import PIPE, Popen
from threading import Thread

def execute_x509_registration(connection_string, x509_cert):
    psc = ProvisioningServiceClient.create_from_connection_string(connection_string)
    att = AttestationMechanism.create_with_x509_client_certs(x509_cert)
    ie = IndividualEnrollment(registration_id=registration_id, attestation=att)
    ie = psc.create_or_update(ie)

def execute_tpm_registration():
    print ("Currently tpm is not supported")
    sys.exit()

def execute_symmetric_key_registration():
    print ("executing symmetric key registration")

def run_executable():
    print ("run exe")

def usage():
    print ( "Usage: prov_test.py -c <connection_string>/r/n-e <test executable>/r/n-t <auth type x509|tpm|symm_key>" )
    print ( "  connectionstring: <HostName=<host_name>;SharedAccessKeyName=<shared_access_key_name>;SharedAccessKey=<shared_access_key>> " )

def main():
    conn_string = ""
    test_exe = ""
    test_type = ""

    try:
        opts, args = getopt.getopt(sys.argv[1:], "e:ht:c:d:", ["conn_str="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-c", "conn_string"):
            conn_string = arg
        elif opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt in '-e':
            test_exe = arg
        elif opt in '-t':
            test_type = arg

    if len(conn_string) == 0:
        print( " connection string is not valid {0}".format(conn_string))
        sys.exit()

    if test_type == "x509":
        execute_x509_registration()
    elif test_type == "tpm":
        execute_tpm_registration()
    elif test_type == "symm_key":
        execute_symmetric_key_registration()
    else:
        print("Unknown test type specified: {0}".format(test_type))
        sys.exit()

    if len(test_exe) > 0:
        run_executable()

if __name__ == '__main__':
    print ( "" )
    print ( "Python {0}".format(sys.version) )
    print ( "" )
    main()
