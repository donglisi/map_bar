#!/bin/bash

insmod map_bar.ko

id=$(cat /proc/devices | grep map_bar | cut -d" " -f 1)

mknod /dev/map_bar c $id 0
