# heic2rgb

heic2rgb is a tiny companion library for [sk-zk/lookaround-map](https://github.com/sk-zk/lookaround-map).
It can do exactly two things: decode the main image in a HEIC file to raw RGB pixels, or remux the HEVC bitstream of the main image into an MP4 container.

## Usage
### Decoding
Simply pass the HEIC file as `bytes` to the decode function:

```python
import heic2rgb
image = heic2rgb.decode(heic_bytes)
```

This will return a `RgbImage` object with `width`, `height` and `data` properties, where `data` is the decoded image as `bytes`.

### Remuxing
```python
mp4_bytes = heic2rgb.to_mp4(heic_bytes)
````

## Benchmark
Decoding a 5632x4352 image on a i5-4440:

|Library|Mean|Std. dev.|
|-------|----|---------|
|heic2rgb 0.2.0|0.58 s|0.006 s|
|[pillow-heif](https://github.com/bigcat88/pillow_heif) 0.13.0|1.31 s|0.02 s|
|[pyheif](https://github.com/carsales/pyheif) 0.7.1|1.55 s|0.07 s|

## Build
### Linux
**0\)** Install prerequisites:
```sh
sudo apt update
sudo apt install make cmake g++ python3-pip ffmpeg libavcodec-dev libavformat-dev libswscale-dev
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
