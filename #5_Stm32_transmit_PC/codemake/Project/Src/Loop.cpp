#include "Loop.h"
#ifdef __cplusplus
extern "C" { 
    void InitC() { Init(); } 
    void LoopC() { while(1) { Loop(); } } 
}
#endif

#include "TieSys.h"
TieSystem TieSys;
void Init() { TieSys.Init(); }
void Loop() { TieSys.Tick(); }
