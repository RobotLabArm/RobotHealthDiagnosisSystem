#if !defined FCIOAPI
#define FCIOAPI

extern "C"
_declspec(dllexport) 
long _stdcall StopImageUpdate(void);

extern "C"
_declspec(dllexport) 
long _stdcall StartImageUpdate(int portNo, int time, int outpLength, int inpLength);

extern "C"
_declspec(dllexport) 
long _stdcall StartImageUpdateWithWd(int portNo, int time, int outpLength, int inpLength, int wdTime);

extern "C"
_declspec(dllexport) 
long _stdcall ReadInputs(int offset, int length, unsigned char * pData);

extern "C"
_declspec(dllexport) 
long _stdcall WriteOutputs(int offset, int length, unsigned char * pData);

#endif