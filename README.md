# heic2rgb

heic2rgb is a Python package which does exactly one thing: convert the main image in a HEIC file to raw RGB pixels.

## Build
### Linux
**0\)** Install prerequisites:
```sh
sudo apt install cmake g++ ffmpeg libavcodec-dev libswscale-dev
pip install scikit-build
```

**1\)** Clone heic2rgb:
```sh
git clone https://github.com/sk-zk/heic2rgb.git
```

**2\)** Clone and build ISOBMFF:
```sh
git clone https://github.com/sk-zk/ISOBMFF.git --recursive
cd ISOBMFF
make
cp ./Build/Release/Products/x86_64/libISOBMFF.a ../heic2rgb/lib/
cp -r ./ISOBMFF/include/* ../heic2rgb/lib/headers/
```

**3\)** Build heic2rgb:
```sh
cd ../heic2rgb
python3 setup.py bdist_wheel
pip install ./dist/[name of wheel]
```

### Windows
TODO: Describe Windows build steps

## Credits
heic2rgb is based on [tifig](https://github.com/monostream/tifig) by monostream GmbH.

This software uses code of [FFmpeg](http://ffmpeg.org) licensed under the [LGPLv2.1](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).