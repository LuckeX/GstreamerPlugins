plugin_LTLIBRARIES = libgstsendsink.la

libgstsendsink_la_SOURCES = gstsendsink.c \
                            wrapper.h \
                            wrapper.cpp \
                            RawTransport.h \
                            RawTransport.cpp \
                            logger.h 
	
libgstsendsink_la_CFLAGS = $(GST_CFLAGS) $(ZMQ_CFLAGS)
libgstsendsink_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)

libgstsendsink_la_LIBADD = $(GST_BASE_LIBS) $(GST_LIBS) 
libgstsendsink_la_LIBADD += -lpthread -lboost_thread -lboost_system -llog4cxx

libgstsendsink_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

noinst_HEADERS = gstsendsink.h 
  


CLEANFILES = $(BUILT_SOURCES)
