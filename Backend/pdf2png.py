__author__ = 'Praveen Kumar, Kuntal Das'

#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#   Copyright (C) 2009-2012 Jan-Philip Gehrcke
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.


import subprocess
import os
import traceback
import sys
#import argparse


# Absolute path to Ghostscript executable here or command name if Ghostscript is
# in your PATH.
GHOSTSCRIPTCMD = "gs"


def usage_exit():
        sys.exit("Usage: %s png_resolution pdffile1 pdffile2 ..." %
            os.path.basename(sys.argv[0]))


def main():
#    print(len(sys.argv))
    if not len(sys.argv) >= 3:
        usage_exit()
    try:
        resolution = int(sys.argv[1])
    except ValueError:
        usage_exit()
    for filepath in sys.argv[1:]:
        (name, ext) = os.path.splitext(filepath)
        if ext.lower().endswith("pdf"):
            print("*** Converting %s..." % filepath)
            gs_pdf_to_png(os.path.join(os.getcwd(), filepath), resolution)


def gs_pdf_to_png(pdffilepath, resolution):
    if not os.path.isfile(pdffilepath):
        print("'%s' is not a file. Skip." % pdffilepath)
    pdffiledir = os.path.dirname(pdffilepath)
    pdffilename = os.path.basename(pdffilepath)
    pdfname, ext = os.path.splitext(pdffilepath)

    pdfname_without_ext = pdffilename.split('.')[0]
    try:
        # Change the "-rXXX" option to set the PNG's resolution.
        # http://ghostscript.com/doc/current/Devices.htm#File_formats
        # For other commandline options see
        # http://ghostscript.com/doc/current/Use.htm#Options
#        pdfname="free221.pdf"
        arglist = [GHOSTSCRIPTCMD,
                  "-dBATCH",
                  "-dNOPAUSE",
                  "-sOutputFile=../Images/Input/test_%03d.png",
                  "-sDEVICE=png16m",
                  "-dINTERPOLATE",
                  "-r%s" % resolution,
                  pdffilepath]
        print("Running command:\n%s" % ' '.join(arglist))
        sp = subprocess.Popen(
            args=arglist,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
    except OSError:
        sys.exit("Error executing Ghostscript ('%s'). Is it in your PATH?" %
            GHOSTSCRIPTCMD)
    except:
        print("Error while running Ghostscript subprocess. Traceback:")
        print("Traceback:\n%s"%traceback.format_exc())

    stdout, stderr = sp.communicate()
    print("Ghostscript stdout:\n'%s'" % stdout)
    if stderr:
        print("Ghostscript stderr:\n'%s'" % stderr)


if __name__ == "__main__":
    main()
