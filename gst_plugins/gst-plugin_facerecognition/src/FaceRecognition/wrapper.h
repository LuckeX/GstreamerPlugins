//
// Created by xyk on 19-5-9.
//

#ifndef STREAMEANALYSTIC_WRAPPER_H
#define STREAMEANALYSTIC_WRAPPER_H


#ifdef __cplusplus
extern "C" {
#endif

void *call_MyPlugin_Create();
void call_Stream_Analystic(void*,void*,int,int);

#ifdef __cplusplus
}
#endif



#endif //STREAMEANALYSTIC_WRAPPER_H
