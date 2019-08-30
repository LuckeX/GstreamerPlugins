# INCLUDE = -I./include

plugin_LTLIBRARIES = libgstreceivedata.la
libgstreceivedata_la_SOURCES = gstreceivedata.c \
                             wrapper.h \
                             wrapper.cpp \
                             RawTransport.h \
                             RawTransport.cpp \
                             RawTransportListenImpl.h \
                             logger.h  


# compiler and linker flags used to compile this plugin, set in configure.ac
libgstreceivedata_la_CFLAGS = $(GST_CFLAGS)

libgstreceivedata_la_LIBADD = $(GST_LIBS)
libgstreceivedata_la_LIBADD += -lpthread -lboost_thread -lboost_system -llog4cxx

libgstreceivedata_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstreceivedata_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstreceivedata.h
