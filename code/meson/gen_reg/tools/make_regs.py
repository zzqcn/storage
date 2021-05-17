#!/usr/bin/env python3

import os
import sys
import re

preamble = """\
/*
 * Do not modify this file. Changes will be overwritten.
 *
 * Generated automatically using \"make_regs.py\".
 */

"""

def gen_plugin_types(funcs):
    output = ""
    for f in funcs:
        output += "int {}(void);\n".format(f)
    return output

def gen_array(funcs, name):
    output = "{}[] = {{\n".format(name)
    for f in funcs:
        output += "    {{ \"{0}\", {0} }},\n".format(f)
    output += "    { NULL, NULL }\n};\n"
    return output

def scan_files(infiles, regs):
    for path in infiles:
        with open(path, 'r', encoding='utf8') as f:
            source = f.read()
            for array, regex in regs:
                matches = re.findall(regex, source)
                array.extend(matches)

def make_plugins(outfile, infiles):
    plugins = []
    plugins_regex = r"int\s+(plugin_register_[\w]+)\s*\(\s*void\s*\)\s*{"

    scan_files(infiles, [(plugins, plugins_regex)])
    if len(plugins) < 1:
        sys.exit("No plugin registrations found.")
    plugins.sort()

    output = preamble
    output += """\
#include "plugin_reg.h"

const unsigned g_plugins_count = {0};

""".format(len(plugins))

    output += gen_plugin_types(plugins)
    output += "\n"
    output += gen_array(plugins, "struct plugin_reg g_plugins")
    output += "\n"

    with open(outfile, "w") as f:
        f.write(output)

    print("Found {0} registrations.".format(len(plugins)))


def print_usage():
    sys.exit("Usage: {0} <plugins_directory> <outfile>\n".format(sys.argv[0]))

def get_files(parent, files):
    for item in os.listdir(parent):
        path = os.path.join(parent, item)
        if path.endswith(".c"):
            files.append(path)
        if os.path.isdir(path):
            get_files(path)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print_usage()

    srcdir = sys.argv[1]
    outfile = sys.argv[2]
    infiles = []

    get_files(srcdir, infiles)
    make_plugins(outfile, infiles)
