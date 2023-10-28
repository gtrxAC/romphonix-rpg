import os, re

def parse(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Search file for C function declarations
    pattern = re.compile('^((\*?)[\w]+(\*?) )*\*?(\w+)\((|void|\w+\*? .*?)\)(?=[ ;\{]|$)', re.M)
    return re.finditer(pattern, content)

def find_functions(directory):
    global out
    for root, _, files in os.walk(directory):
        for file in files:
            if "tinyfiledialogs" in file: continue
            if file.endswith('.c') or file.endswith('.h'):
                file_path = os.path.join(root, file)
                out.write(f"// {file_path}\n")
                function_declarations = parse(file_path)
                for declaration in function_declarations:
                    out.write(declaration.group(0) + ';\n')
                out.write("\n")

out = open('src/functions.h', 'w')
out.write("// _____________________________________________________________________________\n")
out.write("//\n")
out.write("//  ROMphonix RPG - functions.h\n")
out.write("//\n")
out.write("//  This is an auto-generated file containing every function in the code base.\n")
out.write("//  This file is updated every time you run 'make'. See scripts/funcmap.py.\n")
out.write("// _____________________________________________________________________________\n")
out.write("//\n")
find_functions('src')
find_functions('assets/maps')
out.close()