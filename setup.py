#!/usr/bin/env python

from __future__ import annotations

from setuptools import find_packages
from skbuild import setup
import platform

if platform.system() == "Windows":
    libs = [ 
        "lib/ISOBMFF_DLL_v143.dll", 
        "lib/avcodec-59.dll", 
        "lib/avfilter-8.dll", 
        "lib/avformat-59.dll", 
        "lib/avutil-57.dll",
        "lib/swresample-4.dll",
        "lib/swscale-6.dll",
    ]
else:
    libs = []

setup(
    name="heic2rgb",
    version="0.3.0",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    cmake_install_dir="src/heic2rgb",
    package_data={"heic2rgb": ["py.typed"]},
    data_files=[('lib/site-packages/heic2rgb', libs)],
)
