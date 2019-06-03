#!/usr/bin/env python

# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for
# full license information.
import time
import json
import ast
from azure.eventhub import EventHubClient
from azure.eventhub.common import Offset
#from print import print

# our receive loop cycles through our 4 partitions, waiting for
# RECEIVE_CYCLE_TIME seconds at each partition for a message to arrive
RECEIVE_CYCLE_TIME = 0.25

object_list = []


def json_is_same(a, b):
    # If either parameter is a string, convert it to an object.
    # use ast.literal_eval because they might be single-quote delimited which fails with json.loads.
    if isinstance(a, str):
        a = ast.literal_eval(a)
    if isinstance(b, str):
        b = ast.literal_eval(b)
    return a == b


class EventHubApi:
    def __init__(self):
        global object_list
        self.client = None
        self.receivers = []
        object_list.append(self)

    def add_receiver_for_partition(self, eventhub_client, partition_id):
        print("EventHubApi: adding receiver for partition {}".format(partition_id))
        receiver = eventhub_client.add_receiver(
            "$default", partition_id, operation="/messages/events", offset=Offset("@latest")
        )
        return receiver
        

    def connect(self, connection_string):
        print("EventHubApi: connecting EventHubClient")
        # Create client_for_query only to get partition count.
        # By design of the EventHub API, we must listen on partition 0 *first* before we can get_eventhub_info()
        # to get the partition count.
        client_for_query = EventHubClient.from_iothub_connection_string(connection_string)

        self.add_receiver_for_partition(client_for_query, 0)
        client_for_query.run()
        
        event_hub_info = client_for_query.get_eventhub_info()
        num_partitions = event_hub_info["partition_count"]
        print("EventHubApi: EventHub uses {0} partitions".format(num_partitions))

        # Now that we know the partition count, we can now listen on all its ids
        self.client = EventHubClient.from_iothub_connection_string(connection_string)

        # Now that we know the number of eventhub partitions, listen on the remainder.
        for id in range(0, num_partitions):
            self.receivers.append(self.add_receiver_for_partition(self.client, id))

        self.client.run()
        print("EventHubApi: ready")

    def disconnect(self):
        if self in object_list:
            object_list.remove(self)
        if self.client:
            self.client.stop()
            self.client = None

    def wait_for_next_event(self, device_id, timeout, expected=None):
        print("EventHubApi: waiting for next event for {}".format(device_id))
        start_time = time.time()
        while (time.time() - start_time) < timeout:
            for receiver in self.receivers:
                batch = receiver.receive(max_batch_size=1, timeout=RECEIVE_CYCLE_TIME)
                if batch and (batch[0].device_id.decode("ascii") == device_id):
                    print(
                        "EventHubApi: received event: {}".format(batch[0].body_as_str())
                    )
                    received = batch[0].body_as_json()
                    if expected:
                        if json_is_same(expected, received):
                            print("EventHubApi: message received as expected")
                            return received
                        else:
                            print("EventHubApi: unexpected message.  skipping")
                    else:
                        return received
