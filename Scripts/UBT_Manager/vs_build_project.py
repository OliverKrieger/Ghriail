import subprocess

from Scripts.Utils.paths import unreal_sln_path
from Scripts.Utils.paths_external import MSbuild_fpath

def build_project():
    nuget_restore_command = [
        "dotnet",
        "restore",
        unreal_sln_path
    ]
    subprocess.run(nuget_restore_command, check=True)

    msbuild_command = [
        MSbuild_fpath,
        unreal_sln_path,
        "/p:Configuration=Development",
        "/p:Platform=Win64"
    ]
    subprocess.run(msbuild_command, check=True)
    print("Project build complete.")