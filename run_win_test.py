
import os
import subprocess

local_cwd = os.path.dirname(os.path.realpath(__file__))
os.chdir(local_cwd)
dcmd = local_cwd + "\\Debug\\4K_displaywall_bench.exe"
print dcmd

#basic 3x 4K viewport test
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160", "-swap_interval", "0"])


#1 image
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
                                    "-swap_interval", "0",
                                    "-mipmap", "true", "-layout", "true",
                                    "-i", "./images/bubble4K.png",
                                    ], cwd=local_cwd)

#2 images
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
                                    "-swap_interval", "0",
                                    "-mipmap", "true", "-layout", "true",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    ], cwd=local_cwd)

#3 images
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
                                    "-swap_interval", "0",
                                    "-mipmap", "true", "-layout", "true",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    ], cwd=local_cwd)

#4 images
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
                                    "-swap_interval", "0",
                                    "-mipmap", "true", "-layout", "true",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    ], cwd=local_cwd)

#5 images
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
                                    "-swap_interval", "0",
                                    "-mipmap", "true", "-layout", "true",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    "-i", "./images/bubble4K.png",
                                    ], cwd=local_cwd)
#10 images
subprocess.call(["cmd", "/c", dcmd, "-width", "11520", "-height", "2160",
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
