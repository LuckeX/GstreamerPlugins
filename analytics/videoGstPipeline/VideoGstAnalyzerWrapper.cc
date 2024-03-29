
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "VideoGstAnalyzerWrapper.h"
#include "VideoHelper.h"
#include <iostream>

using namespace v8;
using namespace owt_base;

Persistent<Function> VideoGstAnalyzer::constructor;
VideoGstAnalyzer::VideoGstAnalyzer() {};
VideoGstAnalyzer::~VideoGstAnalyzer() {};

void VideoGstAnalyzer::Init(Handle<Object> exports, Handle<Object> module) {
  Isolate* isolate = exports->GetIsolate();
  printf("Wrapper init");
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "VideoGstAnalyzer"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getListeningPort", getListeningPort);
  NODE_SET_PROTOTYPE_METHOD(tpl, "createPipeline", createPipeline);
  NODE_SET_PROTOTYPE_METHOD(tpl, "emit_ListenTo", emit_ListenTo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "emit_ConnectTo", emit_ConnectTo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "addElementMany", addElementMany);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setPlaying", setPlaying);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stopLoop", stopLoop);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setOutputParam", setOutputParam);
  NODE_SET_PROTOTYPE_METHOD(tpl, "disconnect", disconnect);

  constructor.Reset(isolate, tpl->GetFunction());
  module->Set(String::NewFromUtf8(isolate, "exports"), tpl->GetFunction());
}

void VideoGstAnalyzer::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  printf("Wrapper new");
  

  VideoGstAnalyzer* obj = new VideoGstAnalyzer();
  obj->me = new mcu::VideoGstAnalyzer();

  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void VideoGstAnalyzer::close(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;
  delete me;
}

void VideoGstAnalyzer::getListeningPort(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  uint32_t port = me->getListeningPort();

  args.GetReturnValue().Set(Number::New(isolate, port));
}

void VideoGstAnalyzer::createPipeline(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  me->createPipeline();
}

void VideoGstAnalyzer::emit_ListenTo(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  unsigned int minPort = 0, maxPort = 0;

    minPort = args[0]->Uint32Value();
    maxPort = args[1]->Uint32Value();

  me->emit_ListenTo(minPort,maxPort);
}

void VideoGstAnalyzer::emit_ConnectTo(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  unsigned int connectionID;
  connectionID = args[0]->Uint32Value();
  
  String::Utf8Value param0(args[1]->ToString());
  std::string ip = std::string(*param0);

  unsigned int remotePort;
  remotePort = args[2]->Uint32Value();

  me->emit_ConnectTo(connectionID, (char*)ip.c_str(),remotePort);
}



void VideoGstAnalyzer::addElementMany(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  me->addElementMany();
}

void VideoGstAnalyzer::setPlaying(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;
  
  me->setPlaying();
}

void VideoGstAnalyzer::stopLoop(const v8::FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  me->stopLoop();
}

void VideoGstAnalyzer::disconnect(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  int connectionID;
  connectionID = args[0]->Uint32Value();

  me->disconnect(connectionID);
}

void VideoGstAnalyzer::setOutputParam(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  String::Utf8Value param0(args[0]->ToString());
  std::string codec = std::string(*param0);

  String::Utf8Value param1(args[1]->ToString());
  std::string resolution = std::string(*param1);
  VideoSize vSize{0, 0};
  VideoResolutionHelper::getVideoSize(resolution, vSize);
  unsigned int width = vSize.width;
  unsigned int height = vSize.height;

  unsigned int framerateFPS = args[2]->Uint32Value();
  unsigned int bitrateKbps = args[3]->Uint32Value();
  unsigned int keyFrameIntervalSeconds = args[4]->Uint32Value();

  String::Utf8Value param6(args[5]->ToString());
  std::string algorithm = std::string(*param6);

  String::Utf8Value param7(args[6]->ToString());
  std::string pluginName = std::string(*param7);

  me->setOutputParam(codec,width,height,framerateFPS,bitrateKbps,
                       keyFrameIntervalSeconds,algorithm,pluginName);
}