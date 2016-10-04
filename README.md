
# 4K Displaywall Bench

While most benchmarks are intended for the gaming industry, this simple app is for testing fillrate-bound applications such as those that need to drive high resolution displays, or many displays, or both.

#Building

cmake .

make

Use cmake flag -DCMAKE_BUILD_TYPE=Debug to enable OpenGL debug output extension (4.3 and higher)


#Running

./4K_displaywall_bench -help

./4K_displaywall_bench -width 11520 -height 2160 -swap_interval 0

Use the width and height to define the viewport, which may span multiple displays.

If your driver does not support OpenGL 4.4 (GLXBadFBConfig), run with -gl 3 for a version 3.3 context.

#Troubleshooting

you may need to do this:

sudo wget -P /usr/include/GL http://www.opengl.org/registry/api/GL/glcorearb.h

tested on Ubuntu 16.04, you might need:

sudo apt-get install libxrandr-dev
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev


#License

4K_displaywall_bench is freely available for download under the terms of the [BSD3](https://github.com/Oblong/4K_displaywall_bench/blob/master/COPYING "COPYING") License.
