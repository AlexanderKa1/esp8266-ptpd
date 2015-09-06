#!/bin/bash

find -name *.[ch] > .cscope_files_to_scan.txt
find ../include -name *.[ch] >> .cscope_files_to_scan.txt
cscope -bckR -i .cscope_files_to_scan.txt
