
Building:

cmake .

make

./4K_displaywall_bench -width 11520 -height 2160 -swap_interval 0



you may need to do this:
sudo wget -P /usr/include/GL http://www.opengl.org/registry/api/GL/glcorearb.h

tested on Ubuntu 16.04, you might need:
sudo apt-get install libxrandr-dev
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev

If your driver does not support OpenGL 4.4 (GLXBadFBConfig), run with -gl 3 for a version 3.3 context.


