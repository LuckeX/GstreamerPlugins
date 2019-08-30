#ifndef RAWTRANSPORTLISTENERIMPL_H
#define RAWTRANSPORTLISTENERIMPL_H

#include <boost/thread/mutex.hpp>
#include <queue>
#include <iostream>

#include "RawTransport.h"

using namespace std;
using namespace owt_base;



typedef struct {
        // boost::shared_array<char> buffer;
	    char* buffer;
        int length;
} TransportData;

class RawTransportListenerImpl:public RawTransportListener{
    public:
        void onTransportData(char* buf,int len);
        void onTransportError(){printf("transport error in receive\n");}
        void onTransportConnected();
        // ~RawTransportListenerImpl();

    	static std::queue<TransportData> m_receiveQueue;
        static boost::mutex m_receiveQueueMutex;
};

std::queue<TransportData> RawTransportListenerImpl::m_receiveQueue;
boost::mutex RawTransportListenerImpl::m_receiveQueueMutex;

void RawTransportListenerImpl::onTransportData(char* buf,int len){
    TransportData m_receiveData;
    m_receiveData.buffer = new char[len];
    memcpy(m_receiveData.buffer,buf,len);
    m_receiveData.length = len;
    boost::lock_guard<boost::mutex>lock(m_receiveQueueMutex);
    m_receiveQueue.push(m_receiveData);
}

void RawTransportListenerImpl::onTransportConnected(){
    printf("onTransportConnected\n");
}

#endif //


