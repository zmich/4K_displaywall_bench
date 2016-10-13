#!/usr/bin/env python

import os
import subprocess

local_cwd = os.path.dirname(os.path.realpath(__file__))
print local_cwd
os.chdir(local_cwd)

#basic 3x 4K viewport test
subprocess.call(["./4K_displaywall_bench",  "-width", "11520", "-height", "2160", "-swap_interval", "0"], cwd=local_cwd)

#1 image
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)

#2 images
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)

#3 images
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)

#4 images
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)

#5 images
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)
#10 images
subprocess.call(["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                           "-swap_interval", "0",
                                           "-mipmap", "true", "-layout", "true",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           "-i", "./images/bubble4K.png",
                                           ], cwd=local_cwd)
