//
// Created by xyk on 19-5-9.
//

#include "wrapper.h"
#include "myplugin.h"
#include <iostream>

using  namespace std;

void *call_MyPlugin_Create()
{
    MyPlugin* r = new MyPlugin();
    unordered_map<std::string, std::string> params;
    r->PluginInit(params);
    return r;
}

void call_Stream_Analystic(void* myPlugin,void* buf,int width,int height)
{
    unique_ptr<owt::analytics::AnalyticsBuffer> buffer(new owt::analytics::AnalyticsBuffer());
    buffer->buffer = static_cast<uint8_t *>(buf); 
    buffer->width = width;
    buffer->height = height;
    static_cast<MyPlugin *>(myPlugin)->ProcessFrameAsync(move(buffer));
}