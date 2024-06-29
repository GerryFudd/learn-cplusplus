#!/bin/bash

if [ -z "$1" ]
then
  echo "Usage: ${0} <exercise number>"
  exit 1;
fi

./exercises/${1}.sh
