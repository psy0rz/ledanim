#!/usr/bin/env python3

import os
import json

repo={}
for (path, dirs, files) in os.walk("."):
    category=path[2:]
    if category:
        if not category in repo:
            repo[category]={}
        for filename in files:
            with open(path+"/"+filename) as fh:
                desc=fh.readline()
                if desc[0]=="/":
                    desc=desc.lstrip(" /").rstrip()
                else:
                    desc=""
                repo[category][filename]=desc

with open("index.json",'w') as fh:
    fh.write(json.dumps(repo))
