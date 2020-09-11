#! /bin/bash

set -e

mkdir dir

mount -t tmpfs -o mand,size=1m tmpfs ./dir
echo hello > dir/lockfile
chmod g+s,g-x dir/lockfile
