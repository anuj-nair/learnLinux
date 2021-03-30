#!/bin/env bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
