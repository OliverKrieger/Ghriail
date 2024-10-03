import json

from .paths import unreal_uproject_fpath

def load_uproject_contents() -> dict:
    f = open(unreal_uproject_fpath, "r")
    fjson:dict = json.loads(f)
    f.close()
    return fjson

def write_uproject_contents(data:dict):
    with open(unreal_uproject_fpath, 'w') as f:
        f.write(json.dumps(data, indent=4))
    f.close()