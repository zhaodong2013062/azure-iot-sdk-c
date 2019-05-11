"""
Sample test logic to try out Python and a C test executable interacting
"""

import subprocess
import logging
import sys
import time 
import random
import string
import os

from iothub_service_client import IoTHubRegistryManager, IoTHubRegistryManagerAuthMethod
from queue import Queue
from subprocess import PIPE, Popen
from threading import Thread

import override_iot_test_environment
import direct_eventhub_api

#
#  Global Variables
#
# TEST_DEVICENAME_PREFIX specifies prefix for all test devices of this test suite; e.g. DigitalTwin_Test_{RANDOM_VALUES}
TEST_DIGITALTWIN_DEVICENAME_PREFIX = "DigitalTwin_Test_"
# TEST_IOT_C_SDK_EXE specifies the name of the process running the Azure IoT C SDK part of the test
TEST_IOT_C_SDK_EXE = "E:\\developer\\jspaith\\scratch\\iot\\cmake\\clientTest\\Debug\\clientTest.exe"
# TEST_MODULE_ID is the predefined ModuleId that we will use when testing modules instead of devices.
# It is OK to use the same ModuleID always because modules ids are scoped to a given DeviceId.
TEST_MODULE_ID = "TestModuleId"
# Handle of the subprocess
TEST_PROCESS_UTIL = None
# Maximum time to wait for a new message, in seconds
MAXIMUM_WAIT_FOR_NEW_MESSAGE = 5
# Logger used by this test suite.
TEST_LOGGER = None

#
#  The AzureIotTestLogger class is used to route log calls through.  We use a custom 
#  log stream here so that we can control verbosity independently of the 'root'
#  logger, which other modules we import also use.
#
class AzureIotTestLogger:
    # logger is a Python logger log for streaming
    logger = None

    def __init__(self):
        # Creates a custom logger for this class
        self.logger = logging.getLogger("azure-sdk-test")
        # Set filter level to INFO
        self.logger.setLevel(logging.INFO)
        # Custom loggers *must* explicitly tie a handler to stdout for the filtering to be respected.  Do so now.
        handler = logging.StreamHandler(sys.stdout)
        # Set the format that logs have: <time>: <message>.
        formatter = logging.Formatter('%(asctime)s:  %(message)s', "%Y-%m-%d %H:%M:%S")
        handler.setFormatter(formatter)
        # Tie our logger to custom handler we've created
        self.logger.addHandler(handler)

    def log_message_from_subprocess(self, message):
        self.logger.info("TEST-EXE:" + message)

    def log_info(self, message):
        self.logger.info(message)

    def log_error(self, message):
        self.logger.error("ERROR:" + message)

    def log_warning(self, message):
        self.logger.warning(message)



#
# TestSubprocessUtil is used to manage the subprocess / EXE that hosts the C Device SDK test
#
class TestSubprocessUtil:
    test_process = None

    # 
    # is_subprocess_running indicates whether the spun up C SDK test subprocess is active or not.
    #
    def is_subprocess_running(self):
        if (self.test_process.poll() is None):
            return True
        else:
            TEST_LOGGER.log_info("subprocess has completed.  Beginning shutdown of test script")
            return False

    # 
    # get_subprocess_exit_code retrieves the exit code of the subprocess invoked.
    #
    def get_subprocess_exit_code(self):
        if (self.test_process is None):
            TEST_LOGGER.log_error("TestProcess was never initiated.  Unknown error code")
            return 1
        elif (self.test_process.poll() is None):
            TEST_LOGGER.log_error("TestProcess is still running")
            return 2
        else:
            TEST_LOGGER.log_info("TestProcess returned <{0}>".format(self.test_process.returncode))
            return self.test_process.returncode

    #
    # __read_output__ is tied to a given pipe (stdout or stdin) and passes output to the 
    # logger.  Note that if we ever moved to stdout directly, multiple threads writing 
    # to it would risk garbled data.  Python's logging has proper locking, however.
    #
    def __read_output__(self, pipe):
        for line in iter(pipe.readline, b''):
            TEST_LOGGER.log_message_from_subprocess(line.decode('utf-8'))

        pipe.close()

    #
    # start_test_executable invokes the subprocess of the C Test executable.  It also 
    # ties into the created process's stdout/stderr to output content to the script itself's stdout.
    #
    def start_test_executable(self, command, hub_connection_string):
        # Create the C Test Executable
        self.test_process = Popen([command, hub_connection_string], stdout=PIPE, stderr=PIPE)

        # Creates threads to receive stdout/stderr.  Note that the ", " business is in 
        # fact required since this needs to be an iterable.
        stdout_thread = Thread(target=self.__read_output__, args=(self.test_process.stdout, ))
        stderr_thread = Thread(target=self.__read_output__, args=(self.test_process.stderr, ))

        for t in (stdout_thread, stderr_thread):
            t.daemon = True
            t.start()

#
# run_test is the main test "driver" that acts as the test's main
#
class IoTHubUtil:
    # EventHub allows test to retrieve messages from EventHub associated with this IoTHub.
    event_hub = None
    # test_device is a representation of the device we've created for testing
    test_device = None
    # Name of the IoTHub to use for tests.  Read from environment variable 'IOTHUB_CONNECTION_STRING'
    iothub_connection_string = None
    # RegistryManager for this IoTHub
    registry_manager = None
    # test_devicename_prefix specifies the prefix that all devices this object will have, e.g. "test-device-<Random#'s>"
    test_devicename_prefix = None

    #
    # read_test_environment_variables retrieves environment variables, currently just the IOTHUB_CONNECTION_STRING.
    #
    def __read_test_environment_variables(self):
        try:
            self.iothub_connection_string = os.environ["IOTHUB_CONNECTION_STRING"]
            TEST_LOGGER.log_info ("iothub_connection_string: <{0}>".format(self.iothub_connection_string))
        except:
            TEST_LOGGER.log_error ("Could not get all the environment variables...")
            raise

    def __init__(self, test_devicename_prefix):
        self.test_devicename_prefix = test_devicename_prefix
        # Retrieve the environment variable of our IoTHub's connection string.  We use environment variables here,
        # and NOT a more simple command line, so that our gated runs can pre-populate these environment variables from KeyVault.
        # Passing these secrets on the command line on a gate is not as secure.
        self.__read_test_environment_variables()
        # Creating IoTHubRegistryManager is a simple constructor only, no network I/O, so OK to do here.
        self.registry_manager = IoTHubRegistryManager(self.iothub_connection_string)

        # Disconnect from the EventHub.  NOTE: Python runtime will NOT terminate if any
        # of these worker threads are still active, even though we explicitly call sys.exit().


    #
    # connect_to_eventhub creates an underlying object that manages EventHub and connects to it. 
    # NOTE: This takes the *IOTHUB* connection string, not the EventHub connection string.
    #       Underlying API is able to perform appropriate translations based on IoTHub value.
    # NOTE: This can be slow, so should only be used by applications that truly need it.
    #
    def connect_to_eventhub(self):
        self.event_hub = direct_eventhub_api.EventHubApi()
        self.event_hub.connect(self.iothub_connection_string)

    def close_eventhub(self):
        if not (self.event_hub is None):
            TEST_LOGGER.log_info("Disconnecting event hub client")
            self.event_hub.disconnect()

    #
    # generate_device_name will return the name of the test device/module this test run will use.
    # The name is prefixed by the variable TEST_DEVICENAME_PREFIX and joined to random ASCII letters.
    #
    def __generate_device_name(self):
        postfix =  ''.join([random.choice(string.ascii_letters) for n in range(12)])
        return (self.test_devicename_prefix + postfix)

    #
    # For a given device_or_module object, return a connection string for devices to associate with.
    #
    def get_device_connection_string(self, testing_modules):
        host_name_start = self.iothub_connection_string.find("HostName")
        host_name_end = self.iothub_connection_string.find(";", host_name_start)
        host_name = self.iothub_connection_string[host_name_start:host_name_end + 1]

        connection_string = host_name + "DeviceId=" + self.test_device.deviceId + ";" + "SharedAccessKey=" + self.test_device.primaryKey

        if (testing_modules):
            connection_string += ";ModuleId=" + TEST_MODULE_ID

        TEST_LOGGER.log_info("Using device with connection_string <{0}>".format(connection_string))
        return connection_string

    #
    # create_test_device creates a new, random device on IoTHub.  It returns an object representing the newly created identity.
    #
    def create_test_device(self):
        new_device_id = self.__generate_device_name()
        TEST_LOGGER.log_info("About to create new deviceId <{0}>".format(new_device_id))
    
        self.test_device = self.registry_manager.create_device(new_device_id, "", "", IoTHubRegistryManagerAuthMethod.SHARED_PRIVATE_KEY)

        TEST_LOGGER.log_info("Successfully created new deviceId <{0}>".format(new_device_id))

    # 
    # delete_test_device removes test device in IoTHub
    #
    def delete_test_device(self):
        if not (self.test_device is None):
            TEST_LOGGER.log_info("Removing device <{0}>".format(self.test_device.deviceId))
            self.registry_manager.delete_device(self.test_device.deviceId)

    def wait_for_next_event(self):
        received_event = self.event_hub.wait_for_next_event(self.test_device.deviceId, MAXIMUM_WAIT_FOR_NEW_MESSAGE)
        return received_event


class IoTHubTests:
    # Simple test demonstrating waiting for test exe to send messages to an EventHub
    def run_eventhub_tests(self, hub_util, testing_modules):
        global TEST_PROCESS_UTIL 

        new_device_or_module_connection_string = hub_util.get_device_connection_string(testing_modules)
        TEST_PROCESS_UTIL.start_test_executable(TEST_IOT_C_SDK_EXE, new_device_or_module_connection_string)

        i = 0
        while i < 20 and TEST_PROCESS_UTIL.is_subprocess_running():
            receivedEvent = hub_util.wait_for_next_event()
            TEST_LOGGER.log_info("Message received = <{0}>, Index = <{1}>".format(receivedEvent, i))
            i += 1

        TEST_LOGGER.log_info("Exited eventhub loop tests")

#
# verify_test_prerequisites makes sure that any additional assets required for test are present
#
def verify_test_prerequisites():
    if not (os.path.isfile(TEST_IOT_C_SDK_EXE)):
        raise Exception("File <{0}> does not exist".format(TEST_IOT_C_SDK_EXE))

def main():
    global TEST_LOGGER
    global TEST_PROCESS_UTIL

    hub_util = None
    hub_tests = None

    try:
        verify_test_prerequisites()

        TEST_LOGGER = AzureIotTestLogger()
        TEST_LOGGER.log_info("Beginning C SDK e2e test driver.")

        TEST_PROCESS_UTIL = TestSubprocessUtil()

        hub_util = IoTHubUtil(TEST_DIGITALTWIN_DEVICENAME_PREFIX)

        hub_util.create_test_device()
        hub_util.connect_to_eventhub()

        hub_tests = IoTHubTests()
        hub_tests.run_eventhub_tests(hub_util, False)

        TEST_LOGGER.log_info("Completed C SDK e2e test driver.  SubProcess return code = {0}".format(TEST_PROCESS_UTIL.get_subprocess_exit_code()))
        return_code = 0

    except Exception as e:
        logging.error("***** TESTS FAILED *****")
        logging.exception(e)
        return_code = 1

    finally:
        # Delete the temporary device and connections created.  It is critical that both of these are
        # explicitly invoked; removing the device to keep Hub clean, and closing eventhub connections so process can exit.
        if not (hub_util is None):
            hub_util.close_eventhub()
            hub_util.delete_test_device()
    
    return return_code

if __name__ == '__main__':
    sys.exit(main())
