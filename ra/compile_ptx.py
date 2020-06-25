import subprocess
from pathlib import Path
import argparse
import json
import os

# parse input arguments
parser = argparse.ArgumentParser(description="PTX compilation script")
parser.add_argument("--sources", required=True, dest="sources", help="List of CUDA sources to compile")
parser.add_argument("--ccbin", dest="ccbin", help="C++ host compiler to be used by NVCC")
parser.add_argument("--xcompiler", default="", dest="xcompiler", help="Options that will be forwared to the host C++ compiler")
parser.add_argument("--nvcc", required=True, dest="nvcc", help="Path to NVCC compiler")
parser.add_argument("--nvcc_opts", default="", dest="nvcc_opts", help="Options that will be forwarded to NVCC compiler")
parser.add_argument("--include-directories", dest="include_directories", help="Include directories to be forwarded to NVCC")
parser.add_argument("--header", required=True, dest="header", help="Path where to put C++ header accompanying compiled CUDA sources")
parser.add_argument("--settings", required=True, dest="settings", help="Path to the JSON settings file")
args = parser.parse_args()

# generate compilation destinations
sources = [Path(p) for p in args.sources.split(sep=";")]
destinations = []
for s in sources:
    d = s.parent.joinpath("ptx")
    
    if d.exists() is False:
        d.mkdir()
    
    d = Path(d.as_posix() + "/" + s.stem + ".ptx")
    destinations.append(d)

# configure library settings JSON
settings = Path(args.settings)
if settings.exists() is False:
    settings_content = {}
    settings_content["logging_path"] = "ra_log.html"
    settings_content["num_entry_points"] = 1
    settings_content["context_stack_size"] = 8192
    settings_content["asset_directories"] = [d.parent.as_posix() for d in destinations]
else:
    with open(settings, 'r') as f:
        settings_content = json.loads(f.read())
    settings_content["asset_directories"] = [d.as_posix() for d in destinations]
with open(settings, 'w') as f:
        f.write(json.dumps(settings_content))

# generate accompanying C++ header
ptx_header_source_code = \
"""
#ifndef PTX_H 
#define PTX_H

// Standardized shader string names
#define OX_SHADER_ENTRY_INTERSECTION   "__ra_intersect__"
#define OX_SHADER_ENTRY_BOUNDING_BOX   "__ra_aabb__"
#define OX_SHADER_ENTRY_RAY_GENERATION "__ra_generate__"
#define OX_SHADER_ENTRY_CLOSEST_HIT    "__ra_closest_hit__"
#define OX_SHADER_ENTRY_ANY_HIT        "__ra_any_hit__"
#define OX_SHADER_ENTRY_MISS           "__ra_miss__"
#define OX_SHADER_ENTRY_SELECTOR       "__ra_selector__"
#define OX_SHADER_ENTRY_CALLABLE       "__ra_callable__"
"""

for d in destinations:
    source_subclass = d.parents[1].name
    ptx_header_source_code += "// " + source_subclass + "\n"
    ptx_header_source_code += "#define PTX_" + d.stem.upper() + ' "' + d.as_posix() + '"\n'

ptx_header_source_code += "#endif"

with open(Path(args.header).as_posix(), 'w') as f:
    f.write(ptx_header_source_code)

# create command line tokens
commands_list = list()
for idx, d in enumerate(destinations):
    cmd = '"' + Path(args.nvcc).as_posix() + '" --ptx'

    if hasattr(args, "include_directories"):
        for include_dir in args.include_directories.split(sep=";"):
            cmd += ' -I "' + include_dir.strip() + '"'

    if hasattr(args, "ccbin"):
        cmd += ' -ccbin "' + Path(args.ccbin).as_posix() + '"'

    if hasattr(args, "nvcc_options"):
        cmd += ' ' + args.nvcc_options

    if hasattr(args, "xcompiler"):
        cmd += ' -Xcompiler "' + args.xcompiler.lstrip() + '"'

    output = d.as_posix()
    source = sources[idx].as_posix()

    cmd += ' "' + source + '" -o "' + output + '"'
    commands_list.append(cmd)

# execute commands
for command in commands_list:
    subprocess.call(command, shell=True)
