import subprocess
import os

from typing import Literal

from Scripts.Utils.paths import unreal_sln_path, unreal_uproject_path, engine_batch_files_path, project_name
from Scripts.Utils.paths_external import MSbuild_fpath

def MSbuild_project(
        target:Literal["Editor, Server, Game"] = "Editor",
        platform:Literal["Win64", "Wing64-arm64", "Wing64-arm64ec"] = "Win64",
        config:Literal["DeubgGame", "Development", "Shipping"] = "Development"
    ):
    nuget_restore_command = [
        "dotnet",
        "restore",
        unreal_sln_path
    ]
    subprocess.run(nuget_restore_command, check=True)

    msbuild_command = [
        MSbuild_fpath,
        unreal_sln_path,
        f"/t:rebuild",
        f"/p:Configuration={config}",
        f"/p:Platform={platform}"
    ]
    subprocess.run(msbuild_command, check=True)
    print("Project build complete.")

def UBTbuild_project(
        build_type:Literal['Rebuild', 'Build'], 
        target:Literal["Editor, Server, Game"] = "Editor", 
        platform:Literal["Win64", "Wing64-arm64", "Wing64-arm64ec"] = "Win64",
        config:Literal["DeubgGame", "Development", "Shipping"] = "Development"
    ):

    bat_path = os.path.join(engine_batch_files_path, (build_type+'.bat'))

    UBTbuild_command = [
        bat_path,
        (project_name+target),
        platform,
        config,
        "-Project="+unreal_uproject_path,
        "-WaitMutex",
        "-FromMsBuild",
        "-architecture=x64"
    ]

    subprocess.run(UBTbuild_command, check=True)