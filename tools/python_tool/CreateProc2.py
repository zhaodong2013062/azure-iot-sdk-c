"""
Sample test logic to try out Python and a C test executable interacting
"""

import subprocess
import logging
import sys
import time 


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s')

#stdOutProcName="E:\\developer\\jspaith\\scratch\\iot\\cmake\\stdout\\Debug\\stdout.exe foo barbar"

#process = subprocess.Popen(stdOutProcName,
                           #stdout=subprocess.PIPE,
                           #stderr=subprocess.STDOUT)

#while process.poll() is None:
#   logging.error("from-test-exe::" + process.stdout.readline().decode('utf8').rstrip('\n'))


#!/usr/bin/python3
# -*- coding: utf-8 -*-


import sys
from queue import Queue
from subprocess import PIPE, Popen
from threading import Thread


def read_output(pipe, funcs):
    for line in iter(pipe.readline, b''):
        for func in funcs:
            func(line.decode('utf-8'))
    pipe.close()


def write_output(get):
    for line in iter(get, None):
        sys.stdout.write(line)


def run_cmd(command):
    outs, errs = None, None

    proc = Popen(command, close_fds=True, stdout=PIPE, stderr=PIPE, bufsize=1)

    outs, errs = [], [] 

    q = Queue()

    stdout_thread = Thread(target=read_output, args=(proc.stdout, [q.put, outs.append]) )
    stderr_thread = Thread(target=read_output, args=(proc.stderr, [q.put, errs.append]) )
    writer_thread = Thread(target=write_output, args=(q.get,))

    for t in (stdout_thread, stderr_thread, writer_thread):
        t.daemon = True
        t.start()

    proc.wait()

    # This step is required iif we want to merge the results later.  Only really needed 
    #for t in (stdout_thread, stderr_thread):
    #    t.join()

    q.put(None)

    outs = ' '.join(outs)
    errs = ' '.join(errs)

    rc = proc.returncode

    return (rc, (outs, errs))


print("Starting Program...")
stdOutProcName="E:\\developer\\jspaith\\scratch\\iot\\cmake\\stdout\\Debug\\stdout.exe foo barbar"
run_cmd(stdOutProcName)
time.sleep(2)



