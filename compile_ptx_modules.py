import sys
import os
import traceback
import subprocess
import xml.etree.ElementTree as XML

if(len(sys.argv) < 2):
    print("{0:*^80}".format("USAGE"))
    print("python " + __name__ + " <source_of_configuration_script.xml>")
    sys.exit()
elif(len(sys.argv) >= 3):
    print("Don't know what to do with {}".format(argv[2]))
    sys.exit()

configuration_script = sys.argv[1];    # configuration script in XML format
try:
    extension_start_index = len(configuration_script) - configuration_script[-1:-len(configuration_script)-1:-1].index('.')
    extension = configuration_script[extension_start_index::].lower()    # extension of the configuration script
    if(extension != "xml"):
        print("Unable to verify format extension of the configuration script: must be 'xml'")
except:
    print("Cannot determine extension of the configuration script")


# parse XML-tree of provided configuration file
try:
    tree = XML.parse(configuration_script)
    root = tree.getroot()

    if(root.tag != "config"):
        print("Invalid configuration file: the outermost element must have source 'config'")
        sys.exit()

    environment = root.find("environment")

    # retrieve path to requested C++ compiler and path to OptiX SDK
    ccbin = environment.find("ccbin")
    optix_sdk = environment.find("OptiX_SDK")

    # the "ppheader" element defined is not required to exist
    ppheader = environment.find("ppheader")

    # retrive information regarding the CUDA modules to compile
    modules = environment.find("modules")
    if(ppheader != None):
        try:
            f = open(ppheader.text, "rt")
            lines = f.readlines()
        except:
            print("Unable to read requested C++ header {0}".format(ppheader.text))
            sys.exit()
        else:
            f.close()

        defines = dict()
        for l in lines:
            l = l.strip(' \t\n')
            if(l[0:7] == "#define"):
                definition_pair = l[7:]
                definition_pair = definition_pair.expandtabs(4).lstrip(' ')
                tokens = definition_pair.rpartition(' ')
                defines[tokens[0].rstrip(' ')] = tokens[2]

        modules_list = [{"source":m.get("source"), "output":defines[str(m.get("output"))], "nvcc_options":str(m.get("nvcc_options")), "xcompiler":str(m.get("xcompiler"))} for m in modules]
    else:
        modules_list = [{"source":str(m.get("source")), "output":str(m.get("output")), "nvcc_options":str(m.get("nvcc_options")), "xcompiler":str(m.get("xcompiler"))} for m in modules]

except Exception as e:
    print("Unable to parse configuration file: {0}\n{1}".format(e, traceback.format_exc()))
    sys.exit()


# aggregate a command from the parsed data
commands_list = list()
temporary_files = list()
for module_entry in modules_list:
    # this is workaround addressing the issue in Windows version of NVCC, which does not like presense of terminating slash in CCBIN path
    ccbin_text = ccbin.text
    if(ccbin_text[len(ccbin_text) - 1] == '\\' or ccbin_text[len(ccbin_text) - 1] == '/'):
        ccbin_text = ccbin_text[0:len(ccbin_text)-1]

    cmd = 'nvcc --ptx' + ' -ccbin "' + ccbin_text + '" -I "' + optix_sdk.text + '"'

    nvcc_options = module_entry["nvcc_options"]
    xcompiler = module_entry["xcompiler"]
    output = module_entry["output"]
    source = module_entry["source"]

    if(nvcc_options != None):
        cmd += ' ' + module_entry["nvcc_options"]
    if(xcompiler != None):
        cmd += ' -Xcompiler "' + module_entry["xcompiler"] + '"'
    if(source == None):
        print("A CUDA module is specified but the source code file is not provided")
        sys.exit()
    if(output == None):
        print("PTX outlet is not specified for CUDA module {0}".format(source))
        sys.exit()

    #account for the case where source lists several files and assemble them into one temporary file
    list_of_source_file_names = module_entry["source"].split(',')
    full_source = ''
    temporary_file_name = ''
    for source_file_name in list_of_source_file_names:
        source_file_name = source_file_name.strip("' ")
        temporary_file_name += os.path.basename(source_file_name) + '_'
        with open(source_file_name, 'rt') as f:
            source = f.read()
            full_source += source + '\n\n\n'
    temporary_file_name += 'tmp.cu'
    #print(temporary_file_name)
    temporary_files.append(temporary_file_name)
    with open(temporary_file_name, 'w') as f:
        f.write(full_source)

    cmd += ' "' + temporary_file_name + '" -o "' + module_entry["output"] + '"'
    #print(cmd)

    commands_list.append(cmd)


# execute commands
for command in commands_list:
    subprocess.call(command, shell = True)

#clean up the temporary files
for temp_file in temporary_files:
    os.remove(temp_file)