#!/bin/bash
BAKDIR='bak'$(date +%Y-%m-%d_%k_%M)
rm core
echo cp * to $BAKDIR
mkdir $BAKDIR
cp * $BAKDIR
