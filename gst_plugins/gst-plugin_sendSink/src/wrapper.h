#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H


#ifdef __cplusplus
extern "C" {
#endif


void* call_instance(int initBuf,int flag);

void call_connect(void* instance, char* ip,int port);
void call_sendData(void* instance, char *buf, int len);

void call_close(void* instance);



#ifdef __cplusplus
}
#endif



#endif //PIPELINE_WRAPPER_H