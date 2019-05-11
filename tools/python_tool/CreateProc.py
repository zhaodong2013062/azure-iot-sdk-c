"""
Sample test logic to try out Python and a C test executable interacting
"""

import subprocess
import logging
import sys

logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s')

stdOutProcName="E:\\developer\\jspaith\\scratch\\iot\\cmake\\stdout\\Debug\\stdout.exe foo barbar"

process = subprocess.Popen(stdOutProcName,
                           stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT)

while process.poll() is None:
   logging.error("from-test-exe::" + process.stdout.readline().decode('utf8').rstrip('\n'))
