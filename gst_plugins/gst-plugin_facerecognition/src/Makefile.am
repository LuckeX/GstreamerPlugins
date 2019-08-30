# Note: plugindir is set in configure

#########################################################
INCLUDES= -I./include
plugin_LTLIBRARIES = libgstfacerecognition.la

libgstfacerecognition_la_SOURCES = gstfacerecognition.c \
                                   FaceRecognition/wrapper.cpp \
                                   FaceRecognition/wrapper.h \
                                   FaceRecognition/myplugin.h \
                                   FaceRecognition/myplugin.cc
                            

libgstfacerecognition_la_CFLAGS = $(GST_CFLAGS)

libgstfacerecognition_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
#add the path to the library you want to link to
libgstfacerecognition_la_LDFLAGS += -L../../plugin_lib

libgstfacerecognition_la_LIBADD = $(GST_LIBS) 
libgstfacerecognition_la_LIBADD += -lmyplugin

libgstfacerecognition_la_LIBTOOLFLAGS = --tag=disable-static

noinst_HEADERS = gstfacerecognition.h 
