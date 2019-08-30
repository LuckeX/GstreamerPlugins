//
// Created by xyk on 19-7-9.
//

#ifndef CPPGSTREAMER_VideoGstAnalyzer_H
#define CPPGSTREAMER_VideoGstAnalyzer_H

#include <gst/gst.h>
#include <glib-object.h>
#include <gst/pbutils/encoding-profile.h>
#include <string>

namespace mcu {

class VideoGstAnalyzer{
public:
    VideoGstAnalyzer();
    virtual ~VideoGstAnalyzer();
    int createPipeline();
    int getListeningPort();
    void emit_ListenTo(int minPort,int maxPort);
    int setPlaying();

    void emit_ConnectTo(int connectionID, char* ip, int remotePort);

    int addElementMany();

    void setOutputParam(std::string codec, int width, int height, 
    int framerate, int bitrate, int kfi, std::string algo, std::string pluginName);

    void stopLoop();

    void disconnect(int connectionID);

    static void pad_added_handler(GstElement *src, GstPad *new_pad, GstElement *data);
    GstEncodingProfile* create_ogg_theora_profile();

protected:
    static void main_loop_thread(gpointer);
    static GMainLoop *loop;

private:
    GstElement *pipeline, *source, *receive,*analyzer,*decodebin,*x264enc,*capsfilter,*sendsink;
    
    GstStateChangeReturn ret;

    GThread *m_thread;

    int listeningPort;
    int connectPort;

    //param
    std::string codec;
    std::string algo,pluginName;
    std::string resolution;
    int width,height;
    int framerate,bitrate;
    int kfi; //keyFrameInterval
};

}

#endif //CPPGSTREAMER_VideoGstAnalyzer_H
