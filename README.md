# Analytics and Gstreamer Pipeline
## Overview 
There is already a function of stream analysis in MCU, but now we use the gstreamer pipeline to achieve it. We use the gstreamer pipeline to access the webrtc stream, then decode,analyzer,encode, and finally send it back to MCU.

## How to install gstreamer environment
1. Install on Fedora  
  `dnf install gstreamer1-devel gstreamer1-plugins-base-tools gstreamer1-devel-docs gstreamer1-plugins-base-devel gstreamer1-plugins-base-devel-docs gstreamer1-plugins-good gstreamer1-plugins-good-extras gstreamer1-plugins-ugly gstreamer1-plugins-ugly-devel-docs  gstreamer1-plugins-bad-free gstreamer1-plugins-bad-free-devel gstreamer1-plugins-bad-free-extras`
2. Install on Ubuntu or Debian  
`apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio`  

3. For more install of other platform, please ref this [gstreamer install link](https://gstreamer.freedesktop.org/documentation/installing/index.html?gi-language=c)

## How to compile the plugin
You can use the following command:
> `cd gst_plugins/gst-plugin_receiveData/src`  
`./autogen.sh && ./configure && make && sudo make install`  

Then, you will find the plugin in the directory of `./gst-plugin_receiveData/src/.libs` or `/usr/local/lib/gstreamer-1.0`. You can use `gst-inspect-1.0 [plugin name]` to view the information about the gstreamer plugin.   
> note: you should `export GST_PLUGIN_PATH = [/usr/local/lib/gstreamer-1.0]` to your environment.

## Get Start
1. You should clone our `owt-server` from git.
2. Using this `analytics` to replace `owt-server/source/agent/analytics` , and then compile it.  
  
> `./scripts/build.js -t gst-pipeline && ./scripts/pack.js -t analytics-agent`  
`./dist/analytics_agent/install_openh264.sh`




## Note 
1. For facerecognition plugin, it needs to link dynamic libraries, so you should add the `plugin_lib/libmyplugin.so  ` to your path using one of the following :
- add LIBDIR to the 'LD_LIBRARY_PATH' environment variable during execution
- add LIBDIR to the 'LD_RUN_PATH' environment variable during linking
- use the '-Wl,-rpath -Wl,LIBDIR' linker flag
- have your system administrator add LIBDIR to '/etc/ld.so.conf'
  
2. When you write a pipeline in the application, you should specify the library(or header) file path. For example: 
   > You can use this `pkg-config --cflags --libs gstreamer-1.0` command in terminal to find the library file and the path of header file to be included.
