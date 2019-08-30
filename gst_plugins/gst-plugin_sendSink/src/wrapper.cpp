#include "wrapper.h"
// #include "Server.h"
#include "RawTransport.h"
#include "RawTransportListenerImpl.h"
#include <iostream>
#include <queue>


using namespace std;

using namespace owt_base;

void* call_instance(int initBuf,int flag){
    bool tag;
    if(flag)tag = true;
    else tag = false;

    return new RawTransport<TCP>(new RawTransportListenerImpl(),initBuf,tag);
}


void call_connect(void* instance,char* ip,int port){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->createConnection(ip,port);
}

void call_sendData(void* instance, char *buf, int len){
    RawTransport<TCP> *sc = static_cast<RawTransport<TCP>*>(instance);
    sc->sendData(buf,len);
}

void call_close(void* instance){
	RawTransport<TCP> *sc = static_cast<RawTransport<TCP>*>(instance);
	delete sc;//destructor will call close() to release ...
	sc = NULL;
}