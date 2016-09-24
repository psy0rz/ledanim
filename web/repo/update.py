#!/usr/bin/env python3

import os
import json
from collections import OrderedDict

repo=OrderedDict()
for (path, dirs, files) in os.walk("."):
    category=path[2:]
    if category:
        repo[category]=OrderedDict()
        for filename in files:
            with open(path+"/"+filename) as fh:
                desc=fh.readline()
                if desc[0]=="/":
                    desc=desc.lstrip(" /").rstrip()
                else:
                    desc=""
                repo[category][filename]=desc

        repo[category]=sorted(repo[category].items())


repo=sorted(repo.items())

with open("index.json",'w') as fh:
    fh.write(json.dumps(repo))
