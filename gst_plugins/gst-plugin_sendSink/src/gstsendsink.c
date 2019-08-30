/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2019 webrtc <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-sendsink
 *
 * FIXME:Describe sendsink here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! sendsink ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include "gstsendsink.h"

#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_schar (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#define g_marshal_value_peek_variant(v)  g_value_get_variant (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_variant(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */

/* VOID:INT,STRING,INT (VOID__INT_STRING_INT.txt:1) */
void
g_cclosure_user_marshal_VOID__INT_STRING_INT (GClosure     *closure,
                                              GValue       *return_value G_GNUC_UNUSED,
                                              guint         n_param_values,
                                              const GValue *param_values,
                                              gpointer      invocation_hint G_GNUC_UNUSED,
                                              gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__INT_STRING_INT) (gpointer data1,
                                                     gint arg1,
                                                     gpointer arg2,
                                                     gint arg3,
                                                     gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__INT_STRING_INT callback;

  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__INT_STRING_INT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_int (param_values + 1),
            g_marshal_value_peek_string (param_values + 2),
            g_marshal_value_peek_int (param_values + 3),
            data2);
}


GST_DEBUG_CATEGORY_STATIC (gst_send_sink_debug);
#define GST_CAT_DEFAULT gst_send_sink_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-h264, "
        "framerate = (fraction) [0/1, MAX], "
        "width = (int) [ 1, MAX ], " "height = (int) [ 1, MAX ], "
        "stream-format = (string) { byte-stream }, "
        "alignment = (string) au, "
        "profile = (string) { constrained-baseline }")
    );


// static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
//     GST_PAD_SINK,
//     GST_PAD_ALWAYS,
//     GST_STATIC_CAPS ("ANY")
//     );


#define gst_send_sink_parent_class parent_class
G_DEFINE_TYPE (GstSendSink, gst_send_sink, GST_TYPE_BASE_SINK);

static void gst_send_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_send_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static void gst_send_sink_finalize(GObject * gobject);
static gboolean gst_send_sink_start(GstBaseSink * sink);
static gboolean gst_send_sink_stop(GstBaseSink * sink);
static GstFlowReturn gst_send_sink_render(GstBaseSink * sink,GstBuffer * buffer);


void notify_connect_to(GstSendSink *send, gint connectionID, gchar *ip, gint remotePort, gpointer userdata){
  pthread_t tid;
  tid = pthread_self();
  g_print("connect to %s:%d success in connection %d in thread:%u\n",ip, remotePort, connectionID, (unsigned int)tid);
  send->instance[connectionID] = call_instance(1600,1);
  call_connect(send->instance[connectionID],ip,remotePort);
  send->sendNum += 1;
  // g_print("send instance address %p \n",send->instance[connectionID]);
}

void notify_disconnect(GstSendSink *send, gint connectionID, gpointer userdata){
  if(send->instance[connectionID] != NULL){
    g_print("now release connect(%d)\n",connectionID);
    call_close(send->instance[connectionID]);
    send->sendNum -= 1;
  }
  else{
    g_print("connect(%d) is not exist\n",connectionID);
  }
}
/* initialize the sendsink's class */
static void
gst_send_sink_class_init (GstSendSinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbasesink_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;
  gstbasesink_class = (GstBaseSinkClass *)klass;

  gobject_class->set_property = gst_send_sink_set_property;
  gobject_class->get_property = gst_send_sink_get_property;
  gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_send_sink_finalize);

  klass->notifyConnect = notify_connect_to;
  klass->notifyDisconnect = notify_disconnect;
  g_signal_new("notifyConnectTo",G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET(GstSendSinkClass,notifyConnect),
    NULL,NULL,
    g_cclosure_user_marshal_VOID__INT_STRING_INT,
    G_TYPE_NONE,
    3,
    G_TYPE_INT,
    G_TYPE_STRING,
    G_TYPE_INT);

  g_signal_new("notifyDisconnect",G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET(GstSendSinkClass,notifyDisconnect),
    NULL,NULL,
    g_cclosure_marshal_VOID__INT,
    G_TYPE_NONE,
    1,
    G_TYPE_INT);

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE));

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));

  gst_element_class_set_static_metadata (gstelement_class,
    "send sink", "Sink/send data",
    "Send data to mcu",
    "xyk");

  gstbasesink_class->start = GST_DEBUG_FUNCPTR (gst_send_sink_start);
  gstbasesink_class->stop = GST_DEBUG_FUNCPTR (gst_send_sink_stop);
  gstbasesink_class->render = GST_DEBUG_FUNCPTR (gst_send_sink_render);

  GST_DEBUG_CATEGORY_INIT (gst_send_sink_debug,"sendsink",0,"send sink");
  
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_send_sink_init (GstSendSink * sink)
{
  sink->silent = FALSE;
  sink->sendNum = 0;
  sink->fp = NULL;

  g_print("============init instance address to NULL\n");
  for(int i=0;i<10;i++){
    sink->instance[i] = NULL;
  }
}

static void 
gst_send_sink_finalize(GObject * gobject){
  G_OBJECT_CLASS (parent_class)->finalize (gobject);
}

static void
gst_send_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstSendSink *filter = GST_SENDSINK (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_send_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstSendSink *filter = GST_SENDSINK (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstFlowReturn
gst_send_sink_render(GstBaseSink * basesink, GstBuffer * buffer){
  GstFlowReturn retval = GST_FLOW_OK;
  GstSendSink *sink = GST_SENDSINK (basesink);
  GstMapInfo info;
  gst_buffer_map(buffer,&info,GST_MAP_READ);
  if(info.size && sink->sendNum){
    g_print("in sendsink render, connect num = %d, buf len = %ld\n",sink->sendNum,info.size);

    
    fwrite(info.data,sizeof(char),info.size,sink->fp);
    // char* buf = (char*)malloc(sizeof(char)*150);
    // fread(buf,sizeof(char),150,sink->fp);

    for(int i = 0;i < 10;i++){
      if(sink->instance[i] != NULL){
        // g_print("sendsink instance address %p \n",sink->instance[i]);
        call_sendData(sink->instance[i], info.data, info.size);
        // call_sendData(sink->instance[i],buf,150);
      }
    }
  }
  gst_buffer_unmap(buffer,&info);
  // gst_buffer_unref(buffer);
  return retval;
}

static gboolean
gst_send_sink_start(GstBaseSink * basesink){
  gboolean retval = TRUE;
  GstSendSink *sink = GST_SENDSINK (basesink);
  if(sink->fp = fopen("/home/webrtc/Desktop/video/send","w+b"))
    g_print("open the send file\n");
  return retval;
}

static gboolean
gst_send_sink_stop(GstBaseSink * basesink){
  gboolean retval = TRUE;
  GstSendSink *sink = GST_SENDSINK (basesink);
  fclose(sink->fp);
  return retval;
}



/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
sendsink_init (GstPlugin * sendsink)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template sendsink' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_send_sink_debug, "sendsink",
      0, "Template sendsink");

  return gst_element_register (sendsink, "sendsink", GST_RANK_NONE,
      GST_TYPE_SENDSINK);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstsendsink"
#endif

/* gstreamer looks for this structure to register sendsinks
 *
 * exchange the string 'Template sendsink' with your sendsink description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    sendsink,
    "Template sendsink",
    sendsink_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
