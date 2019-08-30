//
// Created by xyk on 19-7-9.
//

#include <zconf.h>
#include "VideoGstAnalyzer.h"
#include <iostream>
#include <string>


namespace mcu {

GMainLoop* VideoGstAnalyzer::loop = g_main_loop_new(NULL,FALSE);

VideoGstAnalyzer::VideoGstAnalyzer() {
    printf("video gst Analyzer constructor");
}

VideoGstAnalyzer::~VideoGstAnalyzer() {
    printf("video gst Analyzer deconstructor");
}

int VideoGstAnalyzer::createPipeline() {
    /* Initialize GStreamer */
    gst_init(NULL, NULL);
    printf("createPipeline\n");

    /* Create the elements */
    source = gst_element_factory_make("fakesrc", "source");
    receive = gst_element_factory_make("receivedata", "receive");
    decodebin = gst_element_factory_make("decodebin","decode");
    x264enc = gst_element_factory_make("x264enc","x264enc");
    analyzer = gst_element_factory_make("facerecognition","analyzer"); 
    capsfilter = gst_element_factory_make("capsfilter","capsfilter");
    sendsink = gst_element_factory_make("sendsink","sendsink");

    loop = g_main_loop_new(NULL, FALSE);

    /* Create the empty VideoGstAnalyzer */
    pipeline = gst_pipeline_new("test-pipeline");

    if (!receive) {
        printf("receive element coule not be created\n");
        return -1;
    }

    if (!sendsink) {
        printf("send element coule not be created\n");
        return -1;
    }

    if (!analyzer){
        printf("analyzer element coule not be created\n");
        return -1;
    }

    if (!pipeline || !source || !decodebin || !capsfilter || !x264enc) {
        printf("pipeline or source or decodebin or or capsfilter or x264enc elements could not be created.\n");
        return -1;
    }

    return 0;
};

void VideoGstAnalyzer::pad_added_handler(GstElement *src, GstPad *new_pad, GstElement *data){
    GstPad *sink_pad = gst_element_get_static_pad(data, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    if (gst_pad_is_linked(sink_pad)) {
        g_print("We are already linked. Ignoring.\n");
        goto exit;
    }
    new_pad_caps = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type = gst_structure_get_name(new_pad_struct);
    if (!g_str_has_prefix(new_pad_type, "video/x-raw")) {
        g_print("It has type '%s' which is not raw video. Ignoring.\n", new_pad_type);
        goto exit;
    }

    ret = gst_pad_link(new_pad,sink_pad);
    if(GST_PAD_LINK_FAILED(ret)){
        g_print("Type is '%s' but link failed.\n",new_pad_type);
    }
    else{
        g_print("Link succeeded(type '%s').\n",new_pad_type);
    }

    exit:
    if(new_pad_caps != NULL)
        gst_caps_unref(new_pad_caps);

    gst_object_unref(sink_pad);
}



int VideoGstAnalyzer::addElementMany() {
    printf("add elements source,receive,decodebin,analyzer,x264enc,sendsink\n");

    #if 1
    GstCaps *gstcaps = gst_caps_from_string("video/x-h264, "
        "stream-format = (string) { byte-stream }, "
        "alignment = (string) au, "
        "profile = (string) { constrained-baseline }");
    
    g_object_set(x264enc,"bitrate",1200,NULL);
    g_object_set(capsfilter,"caps",gstcaps,NULL);

    gst_bin_add_many(GST_BIN (pipeline), source,receive,decodebin,analyzer,x264enc,capsfilter,sendsink, NULL);
    if (gst_element_link_many(source,receive,decodebin,NULL) != TRUE) {
        g_printerr("Elements source,receive,decodebin could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    
    if(gst_element_link_many(analyzer,x264enc,capsfilter,sendsink,NULL) !=TRUE){
        g_printerr("Elements analyzer,x264enc,capsfilter,sendsink could not be linked. \n");
        gst_object_unref(pipeline);
        return -1;
    }

    //the src pad of decodebin is a sometimes pad, so you need to add it dynimic
     g_signal_connect (decodebin, "pad-added", G_CALLBACK(pad_added_handler), analyzer);

    #else
    /*only receive an send pipeline */
    gst_bin_add_many(GST_BIN (pipeline), source,receive,sendsink, NULL);
    if (gst_element_link_many(source,receive,sendsink,NULL) != TRUE) {
        g_printerr("Elements source,receive,sendsink could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    #endif

    return 0;
}


void VideoGstAnalyzer::stopLoop(){
    if(loop){
        printf("main loop quit\n");
        g_main_loop_quit(loop);
    }
    g_thread_join(m_thread);
}

void VideoGstAnalyzer::disconnect(int connectionID){
    g_signal_emit_by_name(sendsink,"notifyDisconnect",connectionID);
}

void VideoGstAnalyzer::main_loop_thread(gpointer data){
    g_main_loop_run(loop);
    printf("first thread end\n");
    g_thread_exit(0);
}

int VideoGstAnalyzer::setPlaying() {
    printf("pipeline play");

    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    m_thread = g_thread_create((GThreadFunc)main_loop_thread,NULL,TRUE,NULL);

    return 0;
}

void VideoGstAnalyzer::emit_ListenTo(int minPort, int maxPort) {
    g_signal_emit_by_name(receive, "notifyListenTo", minPort, maxPort, &this->listeningPort);
    printf("signal ret port = %d\n", listeningPort);
}

void VideoGstAnalyzer::emit_ConnectTo(int connectionID, char* ip, int remotePort){
    printf("connect to remoteIp=%s:%d\n",ip,remotePort);
    g_signal_emit_by_name(sendsink,"notifyConnectTo",connectionID, ip, remotePort);
}

int VideoGstAnalyzer::getListeningPort() { return listeningPort; }

void VideoGstAnalyzer::setOutputParam(std::string codec, int width, int height, 
    int framerate, int bitrate, int kfi, std::string algo, std::string pluginName){

    this->codec = codec;
    this->width = width;
    this->height = height;
    this->framerate = framerate;
    this->bitrate = bitrate;
    this->kfi = kfi;
    this->algo = algo;
    this->pluginName = pluginName;

    std::cout<<"setting param,codec="<<this->codec<<",width="<<this->width<<",height="
        <<this->height<<",framerate="<<this->framerate<<",bitrate="<<this->bitrate<<",kfi="
        <<this->kfi<<",algo="<<this->algo<<",pluginName="<<this->pluginName<<std::endl;
}

}
