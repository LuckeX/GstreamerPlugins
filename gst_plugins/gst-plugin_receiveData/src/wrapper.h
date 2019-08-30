#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H


#ifdef __cplusplus
extern "C" {
#endif

void* call_instance(int initBuf,int flag);
void call_listenTo(void* instance,int minPort,int maxPort);
unsigned short call_getListeningPort(void* instance);

void call_connect(void*instance,char* ip,int port);

char *call_getBuffer(int *len);

void call_requestKeyFrame(void* instance);

#ifdef __cplusplus
}
#endif



#endif //PIPELINE_WRAPPER_H