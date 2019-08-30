#include "RawTransport.h"

using namespace owt_base;

class RawTransportListenerImpl:public RawTransportListener{
    public:
        void onTransportData(char* buf,int len){}
        void onTransportError(){printf("transport error in send\n");}
        void onTransportConnected(){}
        // ~RawTransportListenerImpl();
};

