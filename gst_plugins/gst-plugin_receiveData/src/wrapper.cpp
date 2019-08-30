//
// Created by webrtc on 19-7-22.
//

#include "wrapper.h"

#include "RawTransport.h"
#include "RawTransportListenerImpl.h"
#include <iostream>
#include <queue>
#include <memory>


using namespace std;
using namespace owt_base;

enum FeedbackType {
    VIDEO_FEEDBACK,
    AUDIO_FEEDBACK
};

enum FeedbackCmd {
    REQUEST_KEY_FRAME,
    SET_BITRATE,
    RTCP_PACKET  // FIXME: Temporarily use FeedbackMsg to carry audio rtcp-packets due to the premature AudioFrameConstructor implementation.
};

struct FeedbackMsg {
    FeedbackType type;
    FeedbackCmd  cmd;
    union {
        unsigned short kbps;
        struct RtcpPacket{// FIXME: Temporarily use FeedbackMsg to carry audio rtcp-packets due to the premature AudioFrameConstructor implementation.
            uint32_t len;
            char     buf[128];
        } rtcp;
    } data;
    FeedbackMsg(FeedbackType t, FeedbackCmd c) : type{t}, cmd{c} {}
};

void* call_instance(int initBuf,int flag){
    bool tag;
    if(flag)tag = true;
    else tag = false;

    return new RawTransport<TCP>(new RawTransportListenerImpl(),initBuf,tag);
}

void call_listenTo(void* instance,int minPort,int maxPort){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->listenTo(minPort,maxPort);
}

unsigned short call_getListeningPort(void* instance){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    return rt->getListeningPort();
}

char *call_getBuffer(int *len){
    if(!RawTransportListenerImpl::m_receiveQueue.empty()){
        boost::lock_guard<boost::mutex>lock(RawTransportListenerImpl::m_receiveQueueMutex);
        *len = RawTransportListenerImpl::m_receiveQueue.front().length;
        // char *p = new char[*len];
        // memcpy(p,RawTransportListenerImpl::m_receiveQueue.front().buffer.get(),*len);
        char *p = RawTransportListenerImpl::m_receiveQueue.front().buffer;
        RawTransportListenerImpl::m_receiveQueue.pop();
        return p;
    }
    return NULL;
}

void call_connect(void* instance,char* ip,int port){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->createConnection(ip,port);
}

void call_requestKeyFrame(void* instance){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    FeedbackMsg msg(VIDEO_FEEDBACK,REQUEST_KEY_FRAME);
    char sendBuffer[512];
    sendBuffer[0] = TDT_FEEDBACK_MSG;
    memcpy(&sendBuffer[1], reinterpret_cast<char*>(const_cast<FeedbackMsg*>(&msg)), sizeof(FeedbackMsg));
    rt->sendData((char*)sendBuffer, sizeof(FeedbackMsg) + 1);
}