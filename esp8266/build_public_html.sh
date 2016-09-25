#!/bin/bash
set -e

#we only have 1Mb memory for our public_html stuff, thats why we need to gzip it.
cd data
rsync --delete -ax ../../web/public_html .
gzip -r public_html
