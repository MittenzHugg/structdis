typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef short s16;
typedef long s32;
typedef long long s64;
typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned long vu32;
typedef volatile unsigned long long vu64;
typedef volatile signed char vs8;
typedef volatile short vs16;
typedef volatile long vs32;
typedef volatile long long vs64;
typedef float f32;
typedef double f64;
typedef unsigned int size_t;
typedef s32 OSPri;
typedef s32 OSId;
typedef union { struct { f32 f_odd; f32 f_even; } f; f64 d; } __OSfp;
typedef struct {
 u64 at, v0, v1, a0, a1, a2, a3;
 u64 t0, t1, t2, t3, t4, t5, t6, t7;
 u64 s0, s1, s2, s3, s4, s5, s6, s7;
 u64 t8, t9, gp, sp, s8, ra;
 u64 lo, hi;
 u32 sr, pc, cause, badvaddr, rcp;
 u32 fpcsr;
 __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
 __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;
typedef struct OSThread_s {
 struct OSThread_s *next;
 OSPri priority;
 struct OSThread_s **queue;
 struct OSThread_s *tlnext;
 u16 state;
 u16 flags;
 OSId id;
 int fp;
 __OSThreadprofile_s *thprof;
 __OSThreadContext context;
} OSThread;
extern void osCreateThread(OSThread *, OSId, void (*)(void *),
           void *, void *, OSPri);
extern void osDestroyThread(OSThread *);
extern void osYieldThread(void);
extern void osStartThread(OSThread *);
extern void osStopThread(OSThread *);
extern OSId osGetThreadId(OSThread *);
extern void osSetThreadPri(OSThread *, OSPri);
extern OSPri osGetThreadPri(OSThread *);
typedef u32 OSEvent;
typedef void * OSMesg;
typedef struct OSMesgQueue_s {
 OSThread *mtqueue;
 OSThread *fullqueue;
 s32 validCount;
 s32 first;
 s32 msgCount;
 OSMesg *msg;
} OSMesgQueue;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern void osSetEventMesg(OSEvent, OSMesgQueue *, OSMesg);
typedef u32 OSIntMask;
typedef u32 OSHWIntr;
extern OSIntMask osGetIntMask(void);
extern OSIntMask osSetIntMask(OSIntMask);
typedef u32 OSPageMask;
extern void osMapTLB(s32, OSPageMask, void *, u32, u32, s32);
extern void osMapTLBRdb(void);
extern void osUnmapTLB(s32);
extern void osUnmapTLBAll(void);
extern void osSetTLBASID(s32);
typedef struct {
 u32 errStatus;
        void *dramAddr;
 void *C2Addr;
 u32 sectorSize;
 u32 C1ErrNum;
 u32 C1ErrSector[4];
} __OSBlockInfo;
typedef struct {
 u32 cmdType;
 u16 transferMode;
 u16 blockNum;
 s32 sectorNum;
 u32 devAddr;
 u32 bmCtlShadow;
 u32 seqCtlShadow;
 __OSBlockInfo block[2];
} __OSTranxInfo;
typedef struct OSPiHandle_s {
        struct OSPiHandle_s *next;
        u8 type;
        u8 latency;
        u8 pageSize;
        u8 relDuration;
        u8 pulse;
 u8 domain;
        u32 baseAddress;
        u32 speed;
        __OSTranxInfo transferInfo;
} OSPiHandle;
typedef struct {
        u8 type;
        u32 address;
} OSPiInfo;
typedef struct {
        u16 type;
        u8 pri;
        u8 status;
 OSMesgQueue *retQueue;
} OSIoMesgHdr;
typedef struct {
 OSIoMesgHdr hdr;
 void * dramAddr;
 u32 devAddr;
 u32 size;
 OSPiHandle *piHandle;
} OSIoMesg;
typedef struct {
        s32 active;
 OSThread *thread;
        OSMesgQueue *cmdQueue;
        OSMesgQueue *evtQueue;
        OSMesgQueue *acsQueue;
        s32 (*dma)(s32, u32, void *, u32);
        s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;
extern OSPiHandle *__osPiTable;
extern u32 osPiGetStatus(void);
extern s32 osPiGetDeviceType(void);
extern s32 osPiWriteIo(u32, u32);
extern s32 osPiReadIo(u32, u32 *);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32,
         OSMesgQueue *);
extern void osCreatePiManager(OSPri, OSMesgQueue *, OSMesg *, s32);
extern s32 osEPiRawStartDma(OSPiHandle *, s32 , u32 , void *, u32 );
extern OSPiHandle *osCartRomInit(void);
extern OSPiHandle *osLeoDiskInit(void);
extern OSPiHandle *osDriveRomInit(void);
extern s32 osEPiDeviceType(OSPiHandle *, OSPiInfo *);
extern s32 osEPiWriteIo(OSPiHandle *, u32 , u32 );
extern s32 osEPiReadIo(OSPiHandle *, u32 , u32 *);
extern s32 osEPiStartDma(OSPiHandle *, OSIoMesg *, s32);
extern s32 osEPiLinkHandle(OSPiHandle *);
extern s32 osPiRawStartDma(s32, u32, void *, u32);
typedef struct {
    u32 ctrl;
    u32 width;
    u32 burst;
    u32 vSync;
    u32 hSync;
    u32 leap;
    u32 hStart;
    u32 xScale;
    u32 vCurrent;
} OSViCommonRegs;
typedef struct {
    u32 origin;
    u32 yScale;
    u32 vStart;
    u32 vBurst;
    u32 vIntr;
} OSViFieldRegs;
typedef struct {
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;
extern OSViMode osViModeTable[];
extern OSViMode osViModeNtscLpn1;
extern OSViMode osViModeNtscLpf1;
extern OSViMode osViModeNtscLan1;
extern OSViMode osViModeNtscLaf1;
extern OSViMode osViModeNtscLpn2;
extern OSViMode osViModeNtscLpf2;
extern OSViMode osViModeNtscLan2;
extern OSViMode osViModeNtscLaf2;
extern OSViMode osViModeNtscHpn1;
extern OSViMode osViModeNtscHpf1;
extern OSViMode osViModeNtscHan1;
extern OSViMode osViModeNtscHaf1;
extern OSViMode osViModeNtscHpn2;
extern OSViMode osViModeNtscHpf2;
extern OSViMode osViModePalLpn1;
extern OSViMode osViModePalLpf1;
extern OSViMode osViModePalLan1;
extern OSViMode osViModePalLaf1;
extern OSViMode osViModePalLpn2;
extern OSViMode osViModePalLpf2;
extern OSViMode osViModePalLan2;
extern OSViMode osViModePalLaf2;
extern OSViMode osViModePalHpn1;
extern OSViMode osViModePalHpf1;
extern OSViMode osViModePalHan1;
extern OSViMode osViModePalHaf1;
extern OSViMode osViModePalHpn2;
extern OSViMode osViModePalHpf2;
extern OSViMode osViModeMpalLpn1;
extern OSViMode osViModeMpalLpf1;
extern OSViMode osViModeMpalLan1;
extern OSViMode osViModeMpalLaf1;
extern OSViMode osViModeMpalLpn2;
extern OSViMode osViModeMpalLpf2;
extern OSViMode osViModeMpalLan2;
extern OSViMode osViModeMpalLaf2;
extern OSViMode osViModeMpalHpn1;
extern OSViMode osViModeMpalHpf1;
extern OSViMode osViModeMpalHan1;
extern OSViMode osViModeMpalHaf1;
extern OSViMode osViModeMpalHpn2;
extern OSViMode osViModeMpalHpf2;
extern OSViMode osViModeFpalLpn1;
extern OSViMode osViModeFpalLpf1;
extern OSViMode osViModeFpalLan1;
extern OSViMode osViModeFpalLaf1;
extern OSViMode osViModeFpalLpn2;
extern OSViMode osViModeFpalLpf2;
extern OSViMode osViModeFpalLan2;
extern OSViMode osViModeFpalLaf2;
extern OSViMode osViModeFpalHpn1;
extern OSViMode osViModeFpalHpf1;
extern OSViMode osViModeFpalHan1;
extern OSViMode osViModeFpalHaf1;
extern OSViMode osViModeFpalHpn2;
extern OSViMode osViModeFpalHpf2;
extern u32 osViGetStatus(void);
extern u32 osViGetCurrentMode(void);
extern u32 osViGetCurrentLine(void);
extern u32 osViGetCurrentField(void);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osViSetXScale(f32);
extern void osViSetYScale(f32);
extern void osViExtendVStart(u32);
extern void osViSetSpecialFeatures(u32);
extern void osViSetMode(OSViMode *);
extern void osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void osViSwapBuffer(void *);
extern void osViBlack(u8);
extern void osViFade(u8, u16);
extern void osViRepeatLine(u8);
extern void osCreateViManager(OSPri);
extern u32 osAiGetStatus(void);
extern u32 osAiGetLength(void);
extern s32 osAiSetFrequency(u32);
extern s32 osAiSetNextBuffer(void *, u32);
typedef u64 OSTime;
typedef struct OSTimer_s {
 struct OSTimer_s *next;
 struct OSTimer_s *prev;
 OSTime interval;
 OSTime value;
 OSMesgQueue *mq;
 OSMesg msg;
} OSTimer;
extern OSTime osGetTime(void);
extern void osSetTime(OSTime);
extern int osSetTimer(OSTimer *, OSTime, OSTime,
       OSMesgQueue *, OSMesg);
extern int osStopTimer(OSTimer *);
typedef struct {
 u16 type;
 u8 status;
 u8 errno;
}OSContStatus;
typedef struct {
 u16 button;
 s8 stick_x;
 s8 stick_y;
 u8 errno;
} OSContPad;
typedef struct {
 void *address;
 u8 databuffer[32];
        u8 addressCrc;
 u8 dataCrc;
 u8 errno;
} OSContRamIo;
extern s32 osContInit(OSMesgQueue *, u8 *, OSContStatus *);
extern s32 osContReset(OSMesgQueue *, OSContStatus *);
extern s32 osContStartQuery(OSMesgQueue *);
extern s32 osContStartReadData(OSMesgQueue *);
extern s32 osContSetCh(u8);
extern void osContGetQuery(OSContStatus *);
extern void osContGetReadData(OSContPad *);
typedef struct {
 int status;
 OSMesgQueue *queue;
 int channel;
 u8 id[32];
 u8 label[32];
 int version;
 int dir_size;
 int inode_table;
 int minode_table;
 int dir_table;
 int inode_start_page;
 u8 banks;
 u8 activebank;
} OSPfs;
typedef struct {
 u32 file_size;
   u32 game_code;
   u16 company_code;
   char ext_name[4];
   char game_name[16];
} OSPfsState;
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsRepairId(OSPfs *);
extern s32 osPfsInit(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsReFormat(OSPfs *, OSMesgQueue *, int);
extern s32 osPfsChecker(OSPfs *);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsGetLabel(OSPfs *, u8 *, int *);
extern s32 osPfsSetLabel(OSPfs *, u8 *);
extern s32 osPfsIsPlug(OSMesgQueue *, u8 *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);
typedef struct {
  u16 fixed1;
  u16 start_address;
  u8 nintendo_chr[0x30];
  u8 game_title[16];
  u16 company_code;
  u8 body_code;
  u8 cart_type;
  u8 rom_size;
  u8 ram_size;
  u8 country_code;
  u8 fixed2;
  u8 version;
  u8 isum;
  u16 sum;
} OSGbpakId;
extern s32 osGbpakInit(OSMesgQueue *, OSPfs *, int);
extern s32 osGbpakPower(OSPfs *, s32);
extern s32 osGbpakGetStatus(OSPfs *, u8 *);
extern s32 osGbpakReadWrite(OSPfs *, u16, u16, u8 *, u16);
extern s32 osGbpakReadId(OSPfs *, OSGbpakId *, u8 *);
extern s32 osGbpakCheckConnector(OSPfs *, u8 *);
typedef struct {
  OSMesgQueue *__mq;
  int __channel;
  s32 __mode;
  u8 cmd_status;
} OSVoiceHandle;
typedef struct {
  u16 warning;
  u16 answer_num;
  u16 voice_level;
  u16 voice_sn;
  u16 voice_time;
  u16 answer[5];
  u16 distance[5];
} OSVoiceData;
extern s32 osVoiceInit(OSMesgQueue *, OSVoiceHandle *, int);
extern s32 osVoiceCheckWord(u8 *data);
extern s32 osVoiceClearDictionary(OSVoiceHandle *, u8);
extern s32 osVoiceControlGain(OSVoiceHandle *, s32, s32);
extern s32 osVoiceSetWord(OSVoiceHandle *, u8 *);
extern s32 osVoiceStartReadData(OSVoiceHandle *);
extern s32 osVoiceStopReadData(OSVoiceHandle *);
extern s32 osVoiceGetReadData(OSVoiceHandle *, OSVoiceData *);
extern s32 osVoiceMaskDictionary(OSVoiceHandle *, u8 *, int);
extern void osVoiceCountSyllables(u8 *, u32 *);
extern void osInvalDCache(void *, s32);
extern void osInvalICache(void *, s32);
extern void osWritebackDCache(void *, s32);
extern void osWritebackDCacheAll(void);
typedef struct {
 u16 *histo_base;
 u32 histo_size;
 u32 *text_start;
 u32 *text_end;
} OSProf;
extern void osProfileInit(OSProf *, u32 profcnt);
extern void osProfileStart(u32);
extern void osProfileFlush(void);
extern void osProfileStop(void);
extern void osThreadProfileClear(OSId);
extern void osThreadProfileInit(void);
extern void osThreadProfileStart(void);
extern void osThreadProfileStop(void);
extern u32 osThreadProfileReadCount(OSId);
extern u32 osThreadProfileReadCountTh(OSThread*);
extern OSTime osThreadProfileReadTime(OSId);
extern OSTime osThreadProfileReadTimeTh(OSThread*);
extern u32 osGetCount(void);
extern s32 osRomType;
extern void *osRomBase;
extern s32 osTvType;
extern s32 osResetType;
extern s32 osCicId;
extern s32 osVersion;
extern u32 osMemSize;
extern s32 osAppNMIBuffer[];
extern u64 osClockRate;
extern OSIntMask __OSGlobalIntMask;
extern void osInitialize(void);
extern void osExit(void);
extern u32 osGetMemSize(void);
extern s32 osAfterPreNMI(void);
extern s32 osEepromProbe(OSMesgQueue *);
extern s32 osEepromRead(OSMesgQueue *, u8, u8 *);
extern s32 osEepromWrite(OSMesgQueue *, u8, u8 *);
extern s32 osEepromLongRead(OSMesgQueue *, u8, u8 *, int);
extern s32 osEepromLongWrite(OSMesgQueue *, u8, u8 *, int);
extern OSPiHandle *osFlashReInit(u8 latency, u8 pulse,
     u8 page_size, u8 rel_duration, u32 start);
extern OSPiHandle *osFlashInit(void);
extern void osFlashReadStatus(u8 *flash_status);
extern void osFlashReadId(u32 *flash_type, u32 *flash_maker);
extern void osFlashClearStatus(void);
extern s32 osFlashAllErase(void);
extern s32 osFlashSectorErase(u32 page_num);
extern s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority,
    void *dramAddr, OSMesgQueue *mq);
extern s32 osFlashWriteArray(u32 page_num);
extern s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num,
    void *dramAddr, u32 n_pages, OSMesgQueue *mq);
extern void osFlashChange(u32 flash_num);
extern void osFlashAllEraseThrough(void);
extern void osFlashSectorEraseThrough(u32 page_num);
extern s32 osFlashCheckEraseEnd(void);
extern void __osInitialize_common(void);
extern s32 osTestHost(void);
extern void osReadHost(void *, u32);
extern void osWriteHost(void *, u32);
extern void osAckRamromRead(void);
extern void osAckRamromWrite(void);
extern void osInitRdb(u8 *sendBuf, u32 sendSize);
extern u32 osVirtualToPhysical(void *);
extern void * osPhysicalToVirtual(u32);
extern u32 osDpGetStatus(void);
extern void osDpSetStatus(u32);
extern void osDpGetCounters(u32 *);
extern s32 osDpSetNextBuffer(void *, u64);
extern s32 osMotorInit(OSMesgQueue *, OSPfs *, int);
extern s32 osMotorStop(OSPfs *);
extern s32 osMotorStart(OSPfs *);
extern void bcopy(const void *, void *, int);
extern int bcmp(const void *, const void *, int);
extern void bzero(void *, int);
extern int sprintf(char *s, const char *fmt, ...);
extern void osSyncPrintf(const char *fmt, ...);
typedef struct _Region_s {
 u8 *r_startBufferAddress;
 u8 *r_endAddress;
 s32 r_bufferSize;
 s32 r_bufferCount;
 u16 r_freeList;
 u16 r_alignSize;
} OSRegion;
extern void *osCreateRegion(void *, u32, u32, u32);
extern void *osMalloc(void *);
extern void osFree(void *, void *);
extern s32 osGetRegionBufCount(void *);
extern s32 osGetRegionBufSize(void *);
extern void rmonMain( void * );
extern void rmonPrintf( const char *, ... );
typedef struct {
 u32 type;
 u32 flags;
 u64 *ucode_boot;
 u32 ucode_boot_size;
 u64 *ucode;
 u32 ucode_size;
 u64 *ucode_data;
 u32 ucode_data_size;
 u64 *dram_stack;
 u32 dram_stack_size;
 u64 *output_buff;
 u64 *output_buff_size;
 u64 *data_ptr;
 u32 data_size;
 u64 *yield_data_ptr;
 u32 yield_data_size;
} OSTask_t;
typedef union {
    OSTask_t t;
    long long int force_structure_alignment;
} OSTask;
typedef u32 OSYieldResult;
extern void osSpTaskLoad(OSTask *tp);
extern void osSpTaskStartGo(OSTask *tp);
extern void osSpTaskYield(void);
extern OSYieldResult osSpTaskYielded(OSTask *tp);
typedef struct {
 short ob[3];
 unsigned short flag;
 short tc[2];
 unsigned char cn[4];
} Vtx_t;
typedef struct {
 short ob[3];
 unsigned short flag;
 short tc[2];
 signed char n[3];
 unsigned char a;
} Vtx_tn;
typedef union {
    Vtx_t v;
    Vtx_tn n;
    long long int force_structure_alignment;
} Vtx;
typedef struct {
  void *SourceImagePointer;
  void *TlutPointer;
  short Stride;
  short SubImageWidth;
  short SubImageHeight;
  char SourceImageType;
  char SourceImageBitSize;
  short SourceImageOffsetS;
  short SourceImageOffsetT;
  char dummy[4];
} uSprite_t;
typedef union {
  uSprite_t s;
  long long int force_structure_allignment[3];
} uSprite;
typedef struct {
 unsigned char flag;
 unsigned char v[3];
} Tri;
typedef long Mtx_t[4][4];
typedef union {
    Mtx_t m;
    long long int force_structure_alignment;
} Mtx;
typedef struct {
 short vscale[4];
 short vtrans[4];
} Vp_t;
typedef union {
    Vp_t vp;
    long long int force_structure_alignment;
} Vp;
typedef struct {
  unsigned char col[3];
  char pad1;
  unsigned char colc[3];
  char pad2;
  signed char dir[3];
  char pad3;
} Light_t;
typedef struct {
  unsigned char col[3];
  char pad1;
  unsigned char colc[3];
  char pad2;
} Ambient_t;
typedef struct {
  int x1,y1,x2,y2;
} Hilite_t;
typedef union {
    Light_t l;
    long long int force_structure_alignment[2];
} Light;
typedef union {
    Ambient_t l;
    long long int force_structure_alignment[1];
} Ambient;
typedef struct {
    Ambient a;
    Light l[7];
} Lightsn;
typedef struct {
    Ambient a;
    Light l[1];
} Lights0;
typedef struct {
    Ambient a;
    Light l[1];
} Lights1;
typedef struct {
    Ambient a;
    Light l[2];
} Lights2;
typedef struct {
    Ambient a;
    Light l[3];
} Lights3;
typedef struct {
    Ambient a;
    Light l[4];
} Lights4;
typedef struct {
    Ambient a;
    Light l[5];
} Lights5;
typedef struct {
    Ambient a;
    Light l[6];
} Lights6;
typedef struct {
    Ambient a;
    Light l[7];
} Lights7;
typedef struct {
    Light l[2];
} LookAt;
typedef union {
    Hilite_t h;
    long int force_structure_alignment[4];
} Hilite;
typedef struct {
 int cmd:8;
 unsigned int par:8;
 unsigned int len:16;
 unsigned int addr;
} Gdma;
typedef struct {
  int cmd:8;
  int pad:24;
  Tri tri;
} Gtri;
typedef struct {
  int cmd:8;
  int pad1:24;
  int pad2:24;
  unsigned char param:8;
} Gpopmtx;
typedef struct {
  int cmd:8;
  int pad0:8;
  int mw_index:8;
  int number:8;
  int pad1:8;
  int base:24;
} Gsegment;
typedef struct {
  int cmd:8;
  int pad0:8;
  int sft:8;
  int len:8;
  unsigned int data:32;
} GsetothermodeL;
typedef struct {
  int cmd:8;
  int pad0:8;
  int sft:8;
  int len:8;
  unsigned int data:32;
} GsetothermodeH;
typedef struct {
  unsigned char cmd;
  unsigned char lodscale;
  unsigned char tile;
  unsigned char on;
  unsigned short s;
  unsigned short t;
} Gtexture;
typedef struct {
  int cmd:8;
  int pad:24;
  Tri line;
} Gline3D;
typedef struct {
  int cmd:8;
  int pad1:24;
  short int pad2;
  short int scale;
} Gperspnorm;
typedef struct {
                int cmd:8;
                unsigned int fmt:3;
                unsigned int siz:2;
                unsigned int pad:7;
                unsigned int wd:12;
                unsigned int dram;
} Gsetimg;
typedef struct {
  int cmd:8;
  unsigned int muxs0:24;
  unsigned int muxs1:32;
} Gsetcombine;
typedef struct {
  int cmd:8;
  unsigned char pad;
  unsigned char prim_min_level;
  unsigned char prim_level;
  unsigned long color;
} Gsetcolor;
typedef struct {
  int cmd:8;
  int x0:10;
  int x0frac:2;
  int y0:10;
  int y0frac:2;
  unsigned int pad:8;
  int x1:10;
  int x1frac:2;
  int y1:10;
  int y1frac:2;
} Gfillrect;
typedef struct {
  int cmd:8;
  unsigned int fmt:3;
  unsigned int siz:2;
  unsigned int pad0:1;
  unsigned int line:9;
  unsigned int tmem:9;
  unsigned int pad1:5;
  unsigned int tile:3;
  unsigned int palette:4;
  unsigned int ct:1;
  unsigned int mt:1;
  unsigned int maskt:4;
  unsigned int shiftt:4;
  unsigned int cs:1;
  unsigned int ms:1;
  unsigned int masks:4;
  unsigned int shifts:4;
} Gsettile;
typedef struct {
  int cmd:8;
  unsigned int sl:12;
  unsigned int tl:12;
  int pad:5;
  unsigned int tile:3;
  unsigned int sh:12;
  unsigned int th:12;
} Gloadtile;
typedef Gloadtile Gloadblock;
typedef Gloadtile Gsettilesize;
typedef Gloadtile Gloadtlut;
typedef struct {
  unsigned int cmd:8;
  unsigned int xl:12;
  unsigned int yl:12;
  unsigned int pad1:5;
  unsigned int tile:3;
  unsigned int xh:12;
  unsigned int yh:12;
  unsigned int s:16;
  unsigned int t:16;
  unsigned int dsdx:16;
  unsigned int dtdy:16;
} Gtexrect;
typedef struct {
    unsigned long w0;
    unsigned long w1;
    unsigned long w2;
    unsigned long w3;
} TexRect;
typedef struct {
 unsigned int w0;
 unsigned int w1;
} Gwords;
typedef union {
 Gwords words;
 Gdma dma;
 Gtri tri;
 Gline3D line;
 Gpopmtx popmtx;
 Gsegment segment;
 GsetothermodeH setothermodeH;
 GsetothermodeL setothermodeL;
 Gtexture texture;
 Gperspnorm perspnorm;
 Gsetimg setimg;
 Gsetcombine setcombine;
 Gsetcolor setcolor;
 Gfillrect fillrect;
 Gsettile settile;
 Gloadtile loadtile;
 Gsettilesize settilesize;
 Gloadtlut loadtlut;
        long long int force_structure_alignment;
} Gfx;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int addr;
} Aadpcm;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int addr;
} Apolef;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Aenvelope;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:8;
 unsigned int dmem:16;
 unsigned int pad2:16;
 unsigned int count:16;
} Aclearbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:8;
 unsigned int pad2:16;
 unsigned int inL:16;
        unsigned int inR:16;
} Ainterleave;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int addr;
} Aloadbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Aenvmixer;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int dmemi:16;
 unsigned int dmemo:16;
} Amixer;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int dmem2:16;
 unsigned int addr;
} Apan;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pitch:16;
 unsigned int addr;
} Aresample;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Areverb;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int addr;
} Asavebuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int pad2:2;
 unsigned int number:4;
 unsigned int base:24;
} Asegment;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int dmemin:16;
 unsigned int dmemout:16;
 unsigned int count:16;
} Asetbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int vol:16;
 unsigned int voltgt:16;
 unsigned int volrate:16;
} Asetvol;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int dmemin:16;
    unsigned int dmemout:16;
    unsigned int count:16;
} Admemmove;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int count:16;
    unsigned int addr;
} Aloadadpcm;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int pad2:16;
    unsigned int addr;
} Asetloop;
typedef struct {
 unsigned int w0;
 unsigned int w1;
} Awords;
typedef union {
 Awords words;
 Aadpcm adpcm;
        Apolef polef;
 Aclearbuff clearbuff;
 Aenvelope envelope;
        Ainterleave interleave;
 Aloadbuff loadbuff;
        Aenvmixer envmixer;
 Aresample resample;
 Areverb reverb;
 Asavebuff savebuff;
 Asegment segment;
 Asetbuff setbuff;
 Asetvol setvol;
        Admemmove dmemmove;
        Aloadadpcm loadadpcm;
        Amixer mixer;
        Asetloop setloop;
        long long int force_union_align;
} Acmd;
typedef short ADPCM_STATE[16];
typedef short POLEF_STATE[4];
typedef short RESAMPLE_STATE[16];
typedef short ENVMIX_STATE[40];
typedef s32 ALMicroTime;
typedef u8 ALPan;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
void alUnlink(ALLink *element);
void alLink(ALLink *element, ALLink *after);
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void *state);
void alCopy(void *src, void *dest, s32 len);
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
void alHeapInit(ALHeap *hp, u8 *base, s32 len);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
s32 alHeapCheck(ALHeap *hp);
typedef u8 ALFxId;
typedef void *ALFxRef;
enum {AL_ADPCM_WAVE = 0,
         AL_RAW16_WAVE};
typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];
} ALADPCMBook;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
} ALADPCMloop;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ALRawLoop;
typedef struct {
    ALMicroTime attackTime;
    ALMicroTime decayTime;
    ALMicroTime releaseTime;
    u8 attackVolume;
    u8 decayVolume;
} ALEnvelope;
typedef struct {
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;
} ALKeyMap;
typedef struct {
    ALADPCMloop *loop;
    ALADPCMBook *book;
} ALADPCMWaveInfo;
typedef struct {
    ALRawLoop *loop;
} ALRAWWaveInfo;
typedef struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
} ALWaveTable;
typedef struct ALSound_s {
    ALEnvelope *envelope;
    ALKeyMap *keyMap;
    ALWaveTable *wavetable;
    ALPan samplePan;
    u8 sampleVolume;
    u8 flags;
} ALSound;
typedef struct {
    u8 volume;
    ALPan pan;
    u8 priority;
    u8 flags;
    u8 tremType;
    u8 tremRate;
    u8 tremDepth;
    u8 tremDelay;
    u8 vibType;
    u8 vibRate;
    u8 vibDepth;
    u8 vibDelay;
    s16 bendRange;
    s16 soundCount;
    ALSound *soundArray[1];
} ALInstrument;
typedef struct ALBank_s {
    s16 instCount;
    u8 flags;
    u8 pad;
    s32 sampleRate;
    ALInstrument *percussion;
    ALInstrument *instArray[1];
} ALBank;
typedef struct {
    s16 revision;
    s16 bankCount;
    ALBank *bankArray[1];
} ALBankFile;
void alBnkfNew(ALBankFile *f, u8 *table);
typedef struct {
    u8 *offset;
    s32 len;
} ALSeqData;
typedef struct {
    s16 revision;
    s16 seqCount;
    ALSeqData seqArray[1];
} ALSeqFile;
void alSeqFileNew(ALSeqFile *f, u8 *base);
typedef ALMicroTime (*ALVoiceHandler)(void *);
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    ALHeap *heap;
    s32 outputRate;
    ALFxId fxType;
    s32 *params;
} ALSynConfig;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    ALVoiceHandler handler;
    ALMicroTime callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct ALVoice_s {
    ALLink node;
    struct PVoice_s *pvoice;
    ALWaveTable *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;
typedef struct ALVoiceConfig_s {
    s16 priority;
    s16 fxBus;
    u8 unityPitch;
} ALVoiceConfig;
typedef struct {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    ALHeap *heap;
    struct ALParam_s *paramList;
    struct ALMainBus_s *mainBus;
    struct ALAuxBus_s *auxBus;
    struct ALFilter_s *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;
void alSynNew(ALSynth *s, ALSynConfig *config);
void alSynDelete(ALSynth *s);
void alSynAddPlayer(ALSynth *s, ALPlayer *client);
void alSynRemovePlayer(ALSynth *s, ALPlayer *client);
s32 alSynAllocVoice(ALSynth *s, ALVoice *v, ALVoiceConfig *vc);
void alSynFreeVoice(ALSynth *s, ALVoice *voice);
void alSynStartVoice(ALSynth *s, ALVoice *voice, ALWaveTable *w);
void alSynStartVoiceParams(ALSynth *s, ALVoice *voice, ALWaveTable *w,
                              f32 pitch, s16 vol, ALPan pan, u8 fxmix,
                              ALMicroTime t);
void alSynStopVoice(ALSynth *s, ALVoice *voice);
void alSynSetVol(ALSynth *s, ALVoice *v, s16 vol, ALMicroTime delta);
void alSynSetPitch(ALSynth *s, ALVoice *voice, f32 ratio);
void alSynSetPan(ALSynth *s, ALVoice *voice, ALPan pan);
void alSynSetFXMix(ALSynth *s, ALVoice *voice, u8 fxmix);
void alSynSetPriority(ALSynth *s, ALVoice *voice, s16 priority);
s16 alSynGetPriority(ALSynth *s, ALVoice *voice);
ALFxRef *alSynAllocFX(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp);
ALFxRef alSynGetFXRef(ALSynth *s, s16 bus, s16 index);
void alSynFreeFX(ALSynth *s, ALFxRef *fx);
void alSynSetFXParam(ALSynth *s, ALFxRef fx, s16 paramID, void *param);
typedef struct {
    ALSynth drvr;
} ALGlobals;
extern ALGlobals *alGlobals;
void alInit(ALGlobals *glob, ALSynConfig *c);
void alClose(ALGlobals *glob);
Acmd *alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);
enum ALMsg {
    AL_SEQ_REF_EVT,
    AL_SEQ_MIDI_EVT,
    AL_SEQP_MIDI_EVT,
    AL_TEMPO_EVT,
    AL_SEQ_END_EVT,
    AL_NOTE_END_EVT,
    AL_SEQP_ENV_EVT,
    AL_SEQP_META_EVT,
    AL_SEQP_PROG_EVT,
    AL_SEQP_API_EVT,
    AL_SEQP_VOL_EVT,
    AL_SEQP_LOOP_EVT,
    AL_SEQP_PRIORITY_EVT,
    AL_SEQP_SEQ_EVT,
    AL_SEQP_BANK_EVT,
    AL_SEQP_PLAY_EVT,
    AL_SEQP_STOP_EVT,
    AL_SEQP_STOPPING_EVT,
    AL_TRACK_END,
    AL_CSP_LOOPSTART,
    AL_CSP_LOOPEND,
    AL_CSP_NOTEOFF_EVT,
    AL_TREM_OSC_EVT,
    AL_VIB_OSC_EVT,
    AL_UNK18_EVT
};
enum AL_MIDIstatus {
    AL_MIDI_ChannelMask = 0x0F,
    AL_MIDI_StatusMask = 0xF0,
    AL_MIDI_ChannelVoice = 0x80,
    AL_MIDI_NoteOff = 0x80,
    AL_MIDI_NoteOn = 0x90,
    AL_MIDI_PolyKeyPressure = 0xA0,
    AL_MIDI_ControlChange = 0xB0,
    AL_MIDI_ChannelModeSelect = 0xB0,
    AL_MIDI_ProgramChange = 0xC0,
    AL_MIDI_ChannelPressure = 0xD0,
    AL_MIDI_PitchBendChange = 0xE0,
    AL_MIDI_SysEx = 0xF0,
    AL_MIDI_SystemCommon = 0xF1,
    AL_MIDI_TimeCodeQuarterFrame = 0xF1,
    AL_MIDI_SongPositionPointer = 0xF2,
    AL_MIDI_SongSelect = 0xF3,
    AL_MIDI_Undefined1 = 0xF4,
    AL_MIDI_Undefined2 = 0xF5,
    AL_MIDI_TuneRequest = 0xF6,
    AL_MIDI_EOX = 0xF7,
    AL_MIDI_SystemRealTime = 0xF8,
    AL_MIDI_TimingClock = 0xF8,
    AL_MIDI_Undefined3 = 0xF9,
    AL_MIDI_Start = 0xFA,
    AL_MIDI_Continue = 0xFB,
    AL_MIDI_Stop = 0xFC,
    AL_MIDI_Undefined4 = 0xFD,
    AL_MIDI_ActiveSensing = 0xFE,
    AL_MIDI_SystemReset = 0xFF,
    AL_MIDI_Meta = 0xFF
};
enum AL_MIDIctrl {
    AL_MIDI_VOLUME_CTRL = 0x07,
    AL_MIDI_PAN_CTRL = 0x0A,
    AL_MIDI_PRIORITY_CTRL = 0x10,
    AL_MIDI_FX_CTRL_0 = 0x14,
    AL_MIDI_FX_CTRL_1 = 0x15,
    AL_MIDI_FX_CTRL_2 = 0x16,
    AL_MIDI_FX_CTRL_3 = 0x17,
    AL_MIDI_FX_CTRL_4 = 0x18,
    AL_MIDI_FX_CTRL_5 = 0x19,
    AL_MIDI_FX_CTRL_6 = 0x1A,
    AL_MIDI_FX_CTRL_7 = 0x1B,
    AL_MIDI_FX_CTRL_8 = 0x1C,
    AL_MIDI_FX_CTRL_9 = 0x1D,
    AL_MIDI_SUSTAIN_CTRL = 0x40,
    AL_MIDI_FX1_CTRL = 0x5B,
    AL_MIDI_FX3_CTRL = 0x5D
};
enum AL_MIDImeta {
    AL_MIDI_META_TEMPO = 0x51,
    AL_MIDI_META_EOT = 0x2f
};
typedef struct {
    u8 *curPtr;
    s32 lastTicks;
    s32 curTicks;
    s16 lastStatus;
} ALSeqMarker;
typedef struct {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
} ALMIDIEvent;
typedef struct {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
} ALTempoEvent;
typedef struct {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
} ALEndEvent;
typedef struct {
    struct ALVoice_s *voice;
} ALNoteEvent;
typedef struct {
    struct ALVoice_s *voice;
    ALMicroTime delta;
    u8 vol;
} ALVolumeEvent;
typedef struct {
    s16 vol;
} ALSeqpVolEvent;
typedef struct {
    ALSeqMarker *start;
    ALSeqMarker *end;
    s32 count;
} ALSeqpLoopEvent;
typedef struct {
    u8 chan;
    u8 priority;
} ALSeqpPriorityEvent;
typedef struct {
    void *seq;
} ALSeqpSeqEvent;
typedef struct {
    ALBank *bank;
} ALSeqpBankEvent;
typedef struct {
    struct ALVoiceState_s *vs;
    void *oscState;
    u8 chan;
} ALOscEvent;
typedef struct {
    float unk0;
    float unk4;
} ALUnk18Event;
typedef struct {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        ALSeqpLoopEvent loop;
        ALSeqpVolEvent spvol;
     ALSeqpPriorityEvent sppriority;
     ALSeqpSeqEvent spseq;
     ALSeqpBankEvent spbank;
        ALOscEvent osc;
        ALUnk18Event unk18;
    } msg;
} ALEvent;
typedef struct {
    ALLink node;
    ALMicroTime delta;
    ALEvent evt;
} ALEventListItem;
typedef struct {
    ALLink freeList;
    ALLink allocList;
    s32 eventCount;
} ALEventQueue;
void alEvtqNew(ALEventQueue *evtq, ALEventListItem *items,
                          s32 itemCount);
ALMicroTime alEvtqNextEvent(ALEventQueue *evtq, ALEvent *evt);
void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt,
                                ALMicroTime delta);
void alEvtqFlush(ALEventQueue *evtq);
void alEvtqFlushType(ALEventQueue *evtq, s16 type);
typedef struct ALVoiceState_s {
    struct ALVoiceState_s *next;
    ALVoice voice;
    ALSound *sound;
    ALMicroTime envEndTime;
    f32 pitch;
    f32 vibrato;
    u8 envGain;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 envPhase;
    u8 phase;
    u8 tremelo;
    u8 flags;
} ALVoiceState;
typedef struct {
    ALInstrument *instrument;
    s16 bendRange;
    ALFxId fxId;
    ALPan pan;
    u8 priority;
    u8 vol;
    u8 unk9;
    u8 fxmix;
    u8 sustain;
    f32 pitchBend;
} ALChanState;
typedef struct ALSeq_s {
    u8 *base;
    u8 *trackStart;
    u8 *curPtr;
    s32 lastTicks;
    s32 len;
    f32 qnpt;
    s16 division;
    s16 lastStatus;
} ALSeq;
typedef struct {
    u32 trackOffset[16];
    u32 division;
} ALCMidiHdr;
typedef struct ALCSeq_s {
    ALCMidiHdr *base;
    u32 validTracks;
    f32 qnpt;
    u32 lastTicks;
    u32 lastDeltaTicks;
    u32 deltaFlag;
    u8 *curLoc[16];
    u8 *curBUPtr[16];
    u8 curBULen[16];
    u8 lastStatus[16];
    u32 evtDeltaTicks[16];
} ALCSeq;
typedef struct {
    u32 validTracks;
    s32 lastTicks;
    u32 lastDeltaTicks;
    u8 *curLoc[16];
    u8 *curBUPtr[16];
    u8 curBULen[16];
    u8 lastStatus[16];
    u32 evtDeltaTicks[16];
} ALCSeqMarker;
typedef struct {
    s32 maxVoices;
    s32 maxEvents;
    u8 maxChannels;
    u8 debugFlags;
    ALHeap *heap;
    void *initOsc;
    void *updateOsc;
    void *stopOsc;
} ALSeqpConfig;
typedef ALMicroTime (*ALOscInit)(void **oscState,f32 *initVal, u8 oscType,
                                   u8 oscRate, u8 oscDepth, u8 oscDelay);
typedef ALMicroTime (*ALOscUpdate)(void *oscState, f32 *updateVal);
typedef void (*ALOscStop)(void *oscState);
typedef struct {
    ALPlayer node;
    ALSynth *drvr;
    ALSeq *target;
    ALMicroTime curTime;
    ALBank *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    ALEvent nextEvent;
    ALEventQueue evtq;
    ALMicroTime frameTime;
    ALChanState *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    ALOscInit initOsc;
    ALOscUpdate updateOsc;
    ALOscStop stopOsc;
    ALSeqMarker *loopStart;
    ALSeqMarker *loopEnd;
    s32 loopCount;
} ALSeqPlayer;
typedef struct {
    ALPlayer node;
    ALSynth *drvr;
    ALCSeq *target;
    ALMicroTime curTime;
    ALBank *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    ALEvent nextEvent;
    ALEventQueue evtq;
    ALMicroTime frameTime;
    ALChanState *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    ALOscInit initOsc;
    ALOscUpdate updateOsc;
    ALOscStop stopOsc;
} ALCSPlayer;
void alSeqNew(ALSeq *seq, u8 *ptr, s32 len);
void alSeqNextEvent(ALSeq *seq, ALEvent *event);
s32 alSeqGetTicks(ALSeq *seq);
f32 alSeqTicksToSec(ALSeq *seq, s32 ticks, u32 tempo);
u32 alSeqSecToTicks(ALSeq *seq, f32 sec, u32 tempo);
void alSeqNewMarker(ALSeq *seq, ALSeqMarker *m, u32 ticks);
void alSeqSetLoc(ALSeq *seq, ALSeqMarker *marker);
void alSeqGetLoc(ALSeq *seq, ALSeqMarker *marker);
void alCSeqNew(ALCSeq *seq, u8 *ptr);
void alCSeqNextEvent(ALCSeq *seq,ALEvent *evt);
s32 alCSeqGetTicks(ALCSeq *seq);
f32 alCSeqTicksToSec(ALCSeq *seq, s32 ticks, u32 tempo);
u32 alCSeqSecToTicks(ALCSeq *seq, f32 sec, u32 tempo);
void alCSeqNewMarker(ALCSeq *seq, ALCSeqMarker *m, u32 ticks);
void alCSeqSetLoc(ALCSeq *seq, ALCSeqMarker *marker);
void alCSeqGetLoc(ALCSeq *seq, ALCSeqMarker *marker);
f32 alCents2Ratio(s32 cents);
void alSeqpNew(ALSeqPlayer *seqp, ALSeqpConfig *config);
void alSeqpDelete(ALSeqPlayer *seqp);
void alSeqpSetSeq(ALSeqPlayer *seqp, ALSeq *seq);
ALSeq *alSeqpGetSeq(ALSeqPlayer *seqp);
void alSeqpPlay(ALSeqPlayer *seqp);
void alSeqpStop(ALSeqPlayer *seqp);
s32 alSeqpGetState(ALSeqPlayer *seqp);
void alSeqpSetBank(ALSeqPlayer *seqp, ALBank *b);
void alSeqpSetTempo(ALSeqPlayer *seqp, s32 tempo);
s32 alSeqpGetTempo(ALSeqPlayer *seqp);
s16 alSeqpGetVol(ALSeqPlayer *seqp);
void alSeqpSetVol(ALSeqPlayer *seqp, s16 vol);
void alSeqpLoop(ALSeqPlayer *seqp, ALSeqMarker *start, ALSeqMarker *end, s32 count);
void alSeqpSetChlProgram(ALSeqPlayer *seqp, u8 chan, u8 prog);
s32 alSeqpGetChlProgram(ALSeqPlayer *seqp, u8 chan);
void alSeqpSetChlFXMix(ALSeqPlayer *seqp, u8 chan, u8 fxmix);
u8 alSeqpGetChlFXMix(ALSeqPlayer *seqp, u8 chan);
void alSeqpSetChlVol(ALSeqPlayer *seqp, u8 chan, u8 vol);
u8 alSeqpGetChlVol(ALSeqPlayer *seqp, u8 chan);
void alSeqpSetChlPan(ALSeqPlayer *seqp, u8 chan, ALPan pan);
ALPan alSeqpGetChlPan(ALSeqPlayer *seqp, u8 chan);
void alSeqpSetChlPriority(ALSeqPlayer *seqp, u8 chan, u8 priority);
u8 alSeqpGetChlPriority(ALSeqPlayer *seqp, u8 chan);
void alSeqpSendMidi(ALSeqPlayer *seqp, s32 ticks, u8 status, u8 byte1, u8 byte2);
void alCSPNew(ALCSPlayer *seqp, ALSeqpConfig *config);
void alCSPDelete(ALCSPlayer *seqp);
void alCSPSetSeq(ALCSPlayer *seqp, ALCSeq *seq);
ALCSeq *alCSPGetSeq(ALCSPlayer *seqp);
void alCSPPlay(ALCSPlayer *seqp);
void alCSPStop(ALCSPlayer *seqp);
s32 alCSPGetState(ALCSPlayer *seqp);
void alCSPSetBank(ALCSPlayer *seqp, ALBank *b);
void alCSPSetTempo(ALCSPlayer *seqp, s32 tempo);
s32 alCSPGetTempo(ALCSPlayer *seqp);
s16 alCSPGetVol(ALCSPlayer *seqp);
void alCSPSetVol(ALCSPlayer *seqp, s16 vol);
void alCSPSetChlProgram(ALCSPlayer *seqp, u8 chan, u8 prog);
s32 alCSPGetChlProgram(ALCSPlayer *seqp, u8 chan);
void alCSPSetChlFXMix(ALCSPlayer *seqp, u8 chan, u8 fxmix);
u8 alCSPGetChlFXMix(ALCSPlayer *seqp, u8 chan);
void alCSPSetChlPan(ALCSPlayer *seqp, u8 chan, ALPan pan);
ALPan alCSPGetChlPan(ALCSPlayer *seqp, u8 chan);
void alCSPSetChlVol(ALCSPlayer *seqp, u8 chan, u8 vol);
u8 alCSPGetChlVol(ALCSPlayer *seqp, u8 chan);
void alCSPSetChlPriority(ALCSPlayer *seqp, u8 chan, u8 priority);
u8 alCSPGetChlPriority(ALCSPlayer *seqp, u8 chan);
void alCSPSendMidi(ALCSPlayer *seqp, s32 ticks, u8 status,
                       u8 byte1, u8 byte2);
typedef struct {
    s32 maxSounds;
    s32 maxEvents;
    ALHeap *heap;
} ALSndpConfig;
typedef struct {
    ALPlayer node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    ALSynth *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    ALMicroTime frameTime;
    ALMicroTime nextDelta;
    ALMicroTime curTime;
} ALSndPlayer;
typedef s16 ALSndId;
void alSndpNew(ALSndPlayer *sndp, ALSndpConfig *c);
void alSndpDelete(ALSndPlayer *sndp);
ALSndId alSndpAllocate(ALSndPlayer *sndp, ALSound *sound);
void alSndpDeallocate(ALSndPlayer *sndp, ALSndId id);
void alSndpSetSound(ALSndPlayer *sndp, ALSndId id);
ALSndId alSndpGetSound(ALSndPlayer *sndp);
void alSndpPlay(ALSndPlayer *sndp);
void alSndpPlayAt(ALSndPlayer *sndp, ALMicroTime delta);
void alSndpStop(ALSndPlayer *sndp);
void alSndpSetVol(ALSndPlayer *sndp, s16 vol);
void alSndpSetPitch(ALSndPlayer *sndp, f32 pitch);
void alSndpSetPan(ALSndPlayer *sndp, ALPan pan);
void alSndpSetPriority(ALSndPlayer *sndp, ALSndId id, u8 priority);
void alSndpSetFXMix(ALSndPlayer *sndp, u8 mix);
s32 alSndpGetState(ALSndPlayer *sndp);
typedef struct {
 unsigned char *base;
 int fmt, siz;
 int xsize, ysize;
 int lsize;
 int addr;
 int w, h;
 int s, t;
} Image;
typedef struct {
 float col[3];
 float pos[3];
 float a1, a2;
} PositionalLight;
extern int guLoadTextureBlockMipMap(Gfx **glist, unsigned char *tbuf, Image *im,
  unsigned char startTile, unsigned char pal, unsigned char cms,
  unsigned char cmt, unsigned char masks, unsigned char maskt,
  unsigned char shifts, unsigned char shiftt, unsigned char cfs,
  unsigned char cft);
extern int guGetDPLoadTextureTileSz (int ult, int lrt);
extern void guDPLoadTextureTile (Gfx *glistp, void *timg,
   int texl_fmt, int texl_size,
   int img_width, int img_height,
   int uls, int ult, int lrs, int lrt,
   int palette,
   int cms, int cmt,
   int masks, int maskt,
   int shifts, int shiftt);
extern void guMtxIdent(Mtx *m);
extern void guMtxIdentF(float mf[4][4]);
extern void guOrtho(Mtx *m, float l, float r, float b, float t,
      float n, float f, float scale);
extern void guOrthoF(float mf[4][4], float l, float r, float b, float t,
       float n, float f, float scale);
extern void guFrustum(Mtx *m, float l, float r, float b, float t,
        float n, float f, float scale);
extern void guFrustumF(float mf[4][4], float l, float r, float b, float t,
         float n, float f, float scale);
extern void guPerspective(Mtx *m, u16 *perspNorm, float fovy,
     float aspect, float near, float far, float scale);
extern void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy,
      float aspect, float near, float far, float scale);
extern void guLookAt(Mtx *m,
   float xEye, float yEye, float zEye,
   float xAt, float yAt, float zAt,
   float xUp, float yUp, float zUp);
extern void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye,
        float xAt, float yAt, float zAt,
        float xUp, float yUp, float zUp);
extern void guLookAtReflect(Mtx *m, LookAt *l,
   float xEye, float yEye, float zEye,
   float xAt, float yAt, float zAt,
   float xUp, float yUp, float zUp);
extern void guLookAtReflectF(float mf[4][4], LookAt *l,
        float xEye, float yEye, float zEye,
        float xAt, float yAt, float zAt,
        float xUp, float yUp, float zUp);
extern void guLookAtHilite(Mtx *m, LookAt *l, Hilite *h,
                float xEye, float yEye, float zEye,
                float xAt, float yAt, float zAt,
                float xUp, float yUp, float zUp,
                float xl1, float yl1, float zl1,
                float xl2, float yl2, float zl2,
  int twidth, int theight);
extern void guLookAtHiliteF(float mf[4][4], LookAt *l, Hilite *h,
  float xEye, float yEye, float zEye,
  float xAt, float yAt, float zAt,
  float xUp, float yUp, float zUp,
  float xl1, float yl1, float zl1,
  float xl2, float yl2, float zl2,
  int twidth, int theight);
extern void guLookAtStereo(Mtx *m,
   float xEye, float yEye, float zEye,
   float xAt, float yAt, float zAt,
   float xUp, float yUp, float zUp,
   float eyedist);
extern void guLookAtStereoF(float mf[4][4],
         float xEye, float yEye, float zEye,
         float xAt, float yAt, float zAt,
         float xUp, float yUp, float zUp,
   float eyedist);
extern void guRotate(Mtx *m, float a, float x, float y, float z);
extern void guRotateF(float mf[4][4], float a, float x, float y, float z);
extern void guRotateRPY(Mtx *m, float r, float p, float y);
extern void guRotateRPYF(float mf[4][4], float r, float p, float h);
extern void guAlign(Mtx *m, float a, float x, float y, float z);
extern void guAlignF(float mf[4][4], float a, float x, float y, float z);
extern void guScale(Mtx *m, float x, float y, float z);
extern void guScaleF(float mf[4][4], float x, float y, float z);
extern void guTranslate(Mtx *m, float x, float y, float z);
extern void guTranslateF(float mf[4][4], float x, float y, float z);
extern void guPosition(Mtx *m, float r, float p, float h, float s,
         float x, float y, float z);
extern void guPositionF(float mf[4][4], float r, float p, float h, float s,
   float x, float y, float z);
extern void guMtxF2L(float mf[4][4], Mtx *m);
extern void guMtxL2F(float mf[4][4], Mtx *m);
extern void guMtxCatF(float m[4][4], float n[4][4], float r[4][4]);
extern void guMtxCatL(Mtx *m, Mtx *n, Mtx *res);
extern void guMtxXFMF(float mf[4][4], float x, float y, float z,
        float *ox, float *oy, float *oz);
extern void guMtxXFML(Mtx *m, float x, float y, float z,
        float *ox, float *oy, float *oz);
extern void guNormalize(float *x, float *y, float *z);
void guPosLight(PositionalLight *pl, Light *l,
                float xOb, float yOb, float zOb);
void guPosLightHilite(PositionalLight *pl1, PositionalLight *pl2,
                Light *l1, Light *l2,
                LookAt *l, Hilite *h,
                float xEye, float yEye, float zEye,
                float xOb, float yOb, float zOb,
                float xUp, float yUp, float zUp,
                int twidth, int theight);
extern int guRandom(void);
extern float sinf(float angle);
extern float cosf(float angle);
extern signed short sins (unsigned short angle);
extern signed short coss (unsigned short angle);
extern float sqrtf(float value);
extern void guParseRdpDL(u64 *rdp_dl, u64 nbytes, u8 flags);
extern void guParseString(char *StringPointer, u64 nbytes);
extern void
guBlinkRdpDL(u64 *rdp_dl_in, u64 nbytes_in,
             u64 *rdp_dl_out, u64 *nbytes_out,
             u32 x, u32 y, u32 radius,
             u8 red, u8 green, u8 blue,
             u8 flags);
extern void guParseGbiDL(u64 *gbi_dl, u32 nbytes, u8 flags);
extern void guDumpGbiDL(OSTask *tp,u8 flags);
typedef struct {
    int dataSize;
    int dlType;
    int flags;
    u32 paddr;
} guDLPrintCB;
void guSprite2DInit(uSprite *SpritePointer,
      void *SourceImagePointer,
      void *TlutPointer,
      int Stride,
      int SubImageWidth,
      int SubImageHeight,
      int SourceImageType,
      int SourceImageBitSize,
      int SourceImageOffsetS,
      int SourceImageOffsetT);
typedef struct {
    long type;
    long length;
    long magic;
    char userdata[(((4096)*6)-(3*sizeof(long)))];
} RamRomBuffer;
// struct bitmap {
//  s16 width;
//  s16 width_img;
//  s16 s;
//  s16 t;
//  void *buf;
//  s16 actualHeight;
//  s16 LUToffset;
// };
// typedef struct bitmap Bitmap;sa
typedef struct bitmap {
 s16 width;
 s16 width_img;
 s16 s;
 s16 t;
 void *buf;
 s16 actualHeight;
 s16 LUToffset;
} Bitmap;
/*
struct sprite {
 s16 x,y;
 s16 width, height;
 f32 scalex, scaley;
 s16 expx, expy;
 u16 attr;
 s16 zdepth;
 u8 red;
 u8 green;
 u8 blue;
 u8 alpha;
 s16 startTLUT;
 s16 nTLUT;
 int *LUT;
 s16 istart;
 s16 istep;
 s16 nbitmaps;
 s16 ndisplist;
 s16 bmheight;
 s16 bmHreal;
 u8 bmfmt;
 u8 bmsiz;
 Bitmap *bitmap;
 Gfx *rsp_dl;
 Gfx *rsp_dl_next;
 s16 frac_s,
  frac_t;
};
typedef struct sprite Sprite;
*/
typedef struct sprite {
 s16 x,y;
 s16 width, height;
 f32 scalex, scaley;
 s16 expx, expy;
 u16 attr;
 s16 zdepth;
 u8 red;
 u8 green;
 u8 blue;
 u8 alpha;
 s16 startTLUT;
 s16 nTLUT;
 int *LUT;
 s16 istart;
 s16 istep;
 s16 nbitmaps;
 s16 ndisplist;
 s16 bmheight;
 s16 bmHreal;
 u8 bmfmt;
 u8 bmsiz;
 Bitmap *bitmap;
 Gfx *rsp_dl;
 Gfx *rsp_dl_next;
 s16 frac_s,
  frac_t;
} Sprite;
void spSetAttribute (Sprite *sp, s32 attr);
void spClearAttribute (Sprite *sp, s32 attr);
void spXMove (Sprite *sp, s32 x, s32 y);
void spScale (Sprite *sp, f32 sx, f32 sy);
void spXSetZ (Sprite *sp, s32 z );
void spColor (Sprite *sp, u8 red, u8 green, u8 blue, u8 alpha);
Gfx *spXDraw (Sprite *sp);
void spXInit( Gfx **glistp );
void spXScissor( s32 xmin, s32 xmax, s32 ymin, s32 ymax );
void spXFinish( Gfx **glistp );
extern long long int rspbootTextStart[], rspbootTextEnd[];
extern long long int gspFast3DTextStart[], gspFast3DTextEnd[];
extern long long int gspFast3DDataStart[], gspFast3DDataEnd[];
extern long long int gspFast3D_dramTextStart[], gspFast3D_dramTextEnd[];
extern long long int gspFast3D_dramDataStart[], gspFast3D_dramDataEnd[];
extern long long int gspFast3D_fifoTextStart[], gspFast3D_fifoTextEnd[];
extern long long int gspFast3D_fifoDataStart[], gspFast3D_fifoDataEnd[];
extern long long int gspF3DNoNTextStart[], gspF3DNoNTextEnd[];
extern long long int gspF3DNoNDataStart[], gspF3DNoNDataEnd[];
extern long long int gspF3DNoN_dramTextStart[];
extern long long int gspF3DNoN_dramTextEnd[];
extern long long int gspF3DNoN_dramDataStart[];
extern long long int gspF3DNoN_dramDataEnd[];
extern long long int gspF3DNoN_fifoTextStart[];
extern long long int gspF3DNoN_fifoTextEnd[];
extern long long int gspF3DNoN_fifoDataStart[];
extern long long int gspF3DNoN_fifoDataEnd[];
extern long long int gspLine3DTextStart[], gspLine3DTextEnd[];
extern long long int gspLine3DDataStart[], gspLine3DDataEnd[];
extern long long int gspLine3D_dramTextStart[], gspLine3D_dramTextEnd[];
extern long long int gspLine3D_dramDataStart[], gspLine3D_dramDataEnd[];
extern long long int gspLine3D_fifoTextStart[], gspLine3D_fifoTextEnd[];
extern long long int gspLine3D_fifoDataStart[], gspLine3D_fifoDataEnd[];
extern long long int gspSprite2DTextStart[], gspSprite2DTextEnd[];
extern long long int gspSprite2DDataStart[], gspSprite2DDataEnd[];
extern long long int gspSprite2D_dramTextStart[], gspSprite2D_dramTextEnd[];
extern long long int gspSprite2D_dramDataStart[], gspSprite2D_dramDataEnd[];
extern long long int gspSprite2D_fifoTextStart[], gspSprite2D_fifoTextEnd[];
extern long long int gspSprite2D_fifoDataStart[], gspSprite2D_fifoDataEnd[];
extern long long int aspMainTextStart[], aspMainTextEnd[];
extern long long int aspMainDataStart[], aspMainDataEnd[];
extern long long int gspF3DEX_fifoTextStart[], gspF3DEX_fifoTextEnd[];
extern long long int gspF3DEX_fifoDataStart[], gspF3DEX_fifoDataEnd[];
extern long long int gspF3DEX_NoN_fifoTextStart[], gspF3DEX_NoN_fifoTextEnd[];
extern long long int gspF3DEX_NoN_fifoDataStart[], gspF3DEX_NoN_fifoDataEnd[];
extern long long int gspF3DLX_fifoTextStart[], gspF3DLX_fifoTextEnd[];
extern long long int gspF3DLX_fifoDataStart[], gspF3DLX_fifoDataEnd[];
extern long long int gspF3DLX_NoN_fifoTextStart[], gspF3DLX_NoN_fifoTextEnd[];
extern long long int gspF3DLX_NoN_fifoDataStart[], gspF3DLX_NoN_fifoDataEnd[];
extern long long int gspF3DLX_Rej_fifoTextStart[], gspF3DLX_Rej_fifoTextEnd[];
extern long long int gspF3DLX_Rej_fifoDataStart[], gspF3DLX_Rej_fifoDataEnd[];
extern long long int gspF3DLP_Rej_fifoTextStart[], gspF3DLP_Rej_fifoTextEnd[];
extern long long int gspF3DLP_Rej_fifoDataStart[], gspF3DLP_Rej_fifoDataEnd[];
extern long long int gspL3DEX_fifoTextStart[], gspL3DEX_fifoTextEnd[];
extern long long int gspL3DEX_fifoDataStart[], gspL3DEX_fifoDataEnd[];
extern long long int gspF3DEX2_fifoTextStart[], gspF3DEX2_fifoTextEnd[];
extern long long int gspF3DEX2_fifoDataStart[], gspF3DEX2_fifoDataEnd[];
extern long long int gspF3DEX2_NoN_fifoTextStart[],gspF3DEX2_NoN_fifoTextEnd[];
extern long long int gspF3DEX2_NoN_fifoDataStart[],gspF3DEX2_NoN_fifoDataEnd[];
extern long long int gspF3DEX2_Rej_fifoTextStart[],gspF3DEX2_Rej_fifoTextEnd[];
extern long long int gspF3DEX2_Rej_fifoDataStart[],gspF3DEX2_Rej_fifoDataEnd[];
extern long long int gspF3DLX2_Rej_fifoTextStart[],gspF3DLX2_Rej_fifoTextEnd[];
extern long long int gspF3DLX2_Rej_fifoDataStart[],gspF3DLX2_Rej_fifoDataEnd[];
extern long long int gspL3DEX2_fifoTextStart[], gspL3DEX2_fifoTextEnd[];
extern long long int gspL3DEX2_fifoDataStart[], gspL3DEX2_fifoDataEnd[];
extern long long int gspF3DEX2_xbusTextStart[], gspF3DEX2_xbusTextEnd[];
extern long long int gspF3DEX2_xbusDataStart[], gspF3DEX2_xbusDataEnd[];
extern long long int gspF3DEX2_NoN_xbusTextStart[],gspF3DEX2_NoN_xbusTextEnd[];
extern long long int gspF3DEX2_NoN_xbusDataStart[],gspF3DEX2_NoN_xbusDataEnd[];
extern long long int gspF3DEX2_Rej_xbusTextStart[],gspF3DEX2_Rej_xbusTextEnd[];
extern long long int gspF3DEX2_Rej_xbusDataStart[],gspF3DEX2_Rej_xbusDataEnd[];
extern long long int gspF3DLX2_Rej_xbusTextStart[],gspF3DLX2_Rej_xbusTextEnd[];
extern long long int gspF3DLX2_Rej_xbusDataStart[],gspF3DLX2_Rej_xbusDataEnd[];
extern long long int gspL3DEX2_xbusTextStart[], gspL3DEX2_xbusTextEnd[];
extern long long int gspL3DEX2_xbusDataStart[], gspL3DEX2_xbusDataEnd[];
typedef void (*OSErrorHandler)(s16, s16, ...);
OSErrorHandler osSetErrorHandler(OSErrorHandler);
typedef struct {
    u32 magic;
    u32 len;
    u32 *base;
    s32 startCount;
    s32 writeOffset;
} OSLog;
typedef struct {
    u32 magic;
    u32 timeStamp;
    u16 argCount;
    u16 eventID;
} OSLogItem;
typedef struct {
    u32 magic;
    u32 version;
} OSLogFileHdr;
void osCreateLog(OSLog *log, u32 *base, s32 len);
void osLogEvent(OSLog *log, s16 code, s16 numArgs, ...);
void osFlushLog(OSLog *log);
u32 osLogFloat(f32);
extern void osDelay(int count);
enum bkprog_e{
    BKPROG_D_BEEHIVE_TEXT = 0xD,
 BKPROG_E_JINJO_TEXT,
    BKPROG_15_ENTER_MMM_TEXT = 0x15,
 BKPROG_1D_MMM_DINNING_ROOM_CUTSCENE = 0x1D,
 BKPROG_1F_CC_LOBBY_PIPE_1_RAISED = 0x1F,
    BKPROG_20_CC_LOBBY_PIPE_2_RAISED,
    BKPROG_21_CC_LOBBY_PIPE_3_RAISED,
    BKPROG_22_WATER_SWITCH_1_PRESSED,
    BKPROG_23_LAIR_WATER_LEVEL_1,
    BKPROG_24_WATER_SWITCH_2_PRESSED,
    BKPROG_25_LAIR_WATER_LEVEL_2 = 0x25,
    BKPROG_26_WATER_SWITCH_3_PRESSED,
    BKPROG_27_LAIR_WATER_LEVEL_3 = 0x27,
 BKPROG_31_MM_OPEN = 0x31,
 BKPROG_32_TTC_OPEN,
 BKPROG_33_CC_OPEN,
 BKPROG_34_BGS_OPEN,
 BKPROG_35_FP_OPEN,
 BKPROG_36_GV_OPEN,
 BKPROG_37_MMM_OPEN,
 BKPROG_38_RBB_OPEN,
 BKPROG_39_CCW_OPEN,
 BKPROG_3A_NOTE_DOOR_50_OPEN,
 BKPROG_3B_NOTE_DOOR_180_OPEN,
 BKPROG_3C_NOTE_DOOR_260_OPEN,
 BKPROG_3D_NOTE_DOOR_350_OPEN,
 BKPROG_3E_NOTE_DOOR_450_OPEN,
 BKPROG_3F_NOTE_DOOR_640_OPEN,
 BKPROG_40_NOTE_DOOR_765_OPEN,
 BKPROG_41_NOTE_DOOR_810_OPEN,
 BKPROG_42_NOTE_DOOR_828_OPEN,
 BKPROG_43_NOTE_DOOR_846_OPEN,
 BKPROG_44_NOTE_DOOR_864_OPEN,
 BKPROG_45_NOTE_DOOR_882_OPEN,
 BKPROG_97_ENTERED_LAIR_TEXT = 0x97,
 BKPROG_98_EXITED_LEVEL_TEXT,
    BKPROG_99_PAST_50_NOTE_DOOR_TEXT,
    BKPROG_9E_CRYPT_COFFIN_LID_OPEN = 0x9E,
    BKPROG_A1_STATUE_HAT_OPEN = 0xA1,
    BKPROG_A2_GV_LOBBY_COFFIN_OPEN,
    BKPROG_A6_FURNACE_FUN_COMPLETE = 0xA6,
    BKPROG_A7_NEAR_PUZZLE_PODIUM_TEXT,
    BKPROG_B0_HAS_ENTERED_MM = 0xb0,
 BKPROG_B1_HAS_ENTERED_BGS,
 BKPROG_B2_HAS_ENTERED_TTC,
 BKPROG_B3_HAS_ENTERED_GV,
 BKPROG_B4_HAS_ENTERED_RBB,
 BKPROG_B5_HAS_ENTERED_CCW,
 BKPROG_B6_HAS_ENTERED_FP,
 BKPROG_B7_HAS_ENTERED_MMM,
 BKPROG_B8_HAS_ENTERED_CC,
 BKPROG_B9_DOUBLE_HEALTH,
 BKPROG_BD_ENTER_LAIR_CUTSCENE = 0xBD,
    BKPROG_BE_CHEATO_BLUEEGGS,
    BKPROG_BF_CHEATO_REDFEATHERS,
    BKPROG_C0_CHEATO_GOLDFEATHERS,
    BKPROG_C2_GRATE_TO_RBB_PUZZLE_OPEN = 0xc2,
    BKPROG_C3_ICE_BALL_TO_CHEATO_BROKEN,
    BKPROG_C4_STATUE_EYE_BROKEN,
    BKPROG_C5_RAREWARE_BOX_BROKEN,
 BKPROG_CD_GRATE_TO_WATER_SWITCH_3_OPEN = 0xcd,
    BKPROG_CE_GRATE_TO_MMM_PUZZLE_OPEN,
    BKPROG_F4_ENTER_FF_CUTSCENE = 0xF4,
 BKPROG_FC_DEFEAT_GRUNTY = 0xFC
};
enum unkflags_1{
    UNKFLAGS1_73_SANDCASTLE_INFINITE_LIVES = 0x73,
    UNKFLAGS1_74_SANDCASTLE_INFINITE_EGGS,
    UNKFLAGS1_75_SANDCASTLE_INFINITE_RED_FEATHERS,
    UNKFLAGS1_76_SANDCASTLE_INFINITE_GOLD_FEATHERS,
    UNKFLAGS1_78_SANDCASTLE_NO_BONUS = 0x78,
    UNKFLAGS1_7F_SANDCASTLE_OPEN_CC = 0x7F,
    UNKFLAGS1_86_SANDCASTLE_SHOCKSPRING_JUMP_UNLOCKED = 0x86,
    UNKFLAGS1_87_SANDCASTLE_OPEN_GV,
    UNKFLAGS1_8A_SANDCASTLE_FLIGHT_UNLOCKED = 0x8A,
    UNKFLAGS1_8B_SANDCASTLE_OPEN_FP,
    UNKFLAGS1_8C_SANDCASTLE_OPEN_MMM,
    UNKFLAGS1_8D_SANDCASTLE_REMOVE_CRYPT_GATE,
    UNKFLAGS1_8E_SANDCASTLE_REMOVE_CRYPT_COFFIN_LID,
    UNKFLAGS1_8F_SANDCASTLE_REMOVE_GRATE_NEAR_WATER_SWITCH,
    UNKFLAGS1_90_SANDCASTLE_OPEN_RBB,
    UNKFLAGS1_93_SANDCASTLE_OPEN_CCW = 0x93,
    UNKFLAGS1_94_SANDCASTLE_INFINITE_HEALTH,
    UNKFLAGS1_95_SANDCASTLE_INFINTE_MUMBO_TOKENS,
    UNKFLAGS1_96_SANDCASTLE_INFINITE_AIR,
    UNKFLAGS1_97_SANDCASTLE_BOTTLES_BONUS_1,
    UNKFLAGS1_98_SANDCASTLE_BOTTLES_BONUS_2,
    UNKFLAGS1_99_SANDCASTLE_BOTTLES_BONUS_3,
    UNKFLAGS1_9A_SANDCASTLE_BOTTLES_BONUS_4,
    UNKFLAGS1_9B_SANDCASTLE_BOTTLES_BONUS_5,
    UNKFLAGS1_9C_SANDCASTLE_BOTTLES_BONUS_6,
    UNKFLAGS1_9D_SANDCASTLE_WISHY_WASHY
};
enum transformation_e
{
    unknown,
    TRANSFORM_1_BANJO,
    TRANSFORM_2_TERMITE,
    TRANSFORM_3_PUMPKIN,
    TRANSFORM_4_WALRUS,
    TRANSFORM_5_CROC,
    TRANSFORM_6_BEE,
    TRANSFORM_7_WISHWASHY
};
enum ABILITY_E
{
    ABILITY_0_BARGE = 0x0,
    ABILITY_1_BEAK_BOMB = 0x1,
    ABILITY_2_BEAK_BUSTER = 0x2,
    ABILITY_3_CAMERA_CONTROL = 0x3,
    ABILITY_4_BEAR_PUNCH = 0x4,
    ABILITY_5_CLIMB = 0x5,
    ABILITY_6_EGGS = 0x6,
    ABILITY_7_FLAP = 0x7,
    ABILITY_8_FLIP = 0x8,
    ABILITY_9_FLY = 0x9,
    ABILITY_A_HOLD_A_JUMP_HIGHER = 0xA,
    ABILITY_B_RATATAT_RAP = 0xB,
    ABILITY_C_ROLL = 0xC,
    ABILITY_D_SHOCK_JUMP = 0xD,
    ABILITY_E_WADING_BOOTS = 0xE,
    ABILITY_F_DIVE = 0xF,
    ABILITY_10_TALON_TROT = 0x10,
    ABILITY_11_TURBO_TALON = 0x11,
    ABILITY_12_WONDERWING = 0x12,
    ABILITY_13_1ST_NOTEDOOR = 0x13
};
enum button_e{
    BUTTON_START = 0x0,
    BUTTON_Z = 0x1,
    BUTTON_L = 0x2,
    BUTTON_R = 0x3,
    BUTTON_D_UP = 0x4,
    BUTTON_D_DOWN = 0x5,
    BUTTON_D_LEFT = 0x6,
    BUTTON_D_RIGHT = 0x7,
    BUTTON_A = 0x8,
    BUTTON_B = 0x9,
    BUTTON_C_LEFT = 0xA,
    BUTTON_C_DOWN = 0xB,
    BUTTON_C_UP = 0xC,
    BUTTON_C_RIGHT = 0xD
};
enum map_e
{
    MAP_1_SM_SPIRAL_MOUNTAIN = 0x01,
    MAP_2_MM_MUMBOS_MOUNTAIN,
    MAP_TTC_BLUBBERS_SHIP = 0x05,
    MAP_TTC_NIPPERS_SHELL = 0x06,
    MAP_7_TTC_TREASURE_TROVE_COVE,
    MAP_A_TTC_SANDCASTLE = 0x0A,
    MAP_B_CC_CLANKERS_CAVERN,
    MAP_MM_TICKERS_TOWER = 0x0C,
    MAP_D_BGS_BUBBLEGLOOP_SWAMP,
    MAP_MM_MUMBOS_SKULL = 0x0E,
    MAP_BGS_MR_VILE = 0x10,
    MAP_BGS_TIPTUP = 0x11,
    MAP_12_GV_GOBIS_VALLEY = 0x12,
    MAP_GV_MEMORY_GAME = 0x13,
    MAP_GV_SANDYBUTTS_MAZE = 0x14,
    MAP_GV_WATER_PYRAMID = 0x15,
    MAP_GV_RUBEES_CHAMBER = 0x16,
    MAP_GV_INSIDE_JINXY = 0x1A,
    MAP_1B_MMM_MAD_MONSTER_MANSION = 0x1B,
    MAP_MMM_CHURCH = 0x1C,
    MAP_MMM_CELLAR = 0x1D,
    MAP_CS_START_NINTENDO = 0x1E,
    MAP_1F_CS_START_RAREWARE = 0x1F,
    MAP_20_CS_END_NOT_100 = 0x20,
    MAP_CC_WITCH_SWITCH_ROOM = 0x21,
    MAP_CC_INSIDE_CLANKER = 0x22,
    MAP_CC_GOLDFEATHER_ROOM = 0x23,
    MAP_MMM_TUMBLARS_SHED = 0x24,
    MAP_MMM_WELL = 0x25,
    MAP_26_MMM_NAPPERS_ROOM = 0x26,
    MAP_27_FP_FREEZEEZY_PEAK = 0x27,
    MAP_MMM_EGG_ROOM = 0x28,
    MAP_MMM_NOTE_ROOM = 0x29,
    MAP_MMM_FEATHER_ROOM = 0x2A,
    MAP_MMM_SECRET_CHURCH_ROOM = 0x2B,
    MAP_MMM_BATHROOM = 0x2C,
    MAP_MMM_BEDROOM = 0x2D,
    MAP_MMM_HONEYCOMB_ROOM = 0x2E,
    MAP_MMM_WATERDRAIN_BARREL = 0x2F,
    MAP_MMM_MUMBOS_SKULL = 0x30,
    MAP_31_RBB_RUSTY_BUCKET_BAY = 0x31,
    MAP_34_RBB_ENGINE_ROOM = 0x34,
    MAP_RBB_WAREHOUSE = 0x35,
    MAP_RBB_BOATHOUSE = 0x36,
    MAP_RBB_CONTAINER_1 = 0x37,
    MAP_RBB_CONTAINER_3 = 0x38,
    MAP_RBB_CREW_CABIN = 0x39,
    MAP_RBB_BOSS_BOOM_BOX = 0x3A,
    MAP_RBB_STORAGE_ROOM = 0x3B,
    MAP_RBB_KITCHEN = 0x3C,
    MAP_RBB_NAVIGATION_ROOM = 0x3D,
    MAP_RBB_CONTAINER_2 = 0x3E,
    MAP_RBB_CAPTAINS_CABIN = 0x3F,
    MAP_40_CCW_HUB = 0x40,
    MAP_FP_BOGGYS_IGLOO = 0x41,
    MAP_43_CCW_SPRING = 0x43,
    MAP_44_CCW_SUMMER = 0x44,
    MAP_45_CCW_AUTUMN = 0x45,
    MAP_46_CCW_WINTER = 0x46,
    MAP_BGS_MUMBOS_SKULL = 0x47,
    MAP_FP_MUMBOS_SKULL = 0x48,
    MAP_CCW_SPRING_MUMBOS_SKULL = 0x4A,
    MAP_CCW_SUMMER_MUMBOS_SKULL = 0x4B,
    MAP_CCW_AUTUMN_MUMBOS_SKULL = 0x4C,
    MAP_CCW_WINTER_MUMBOS_SKULL = 0x4D,
    MAP_FP_CHRISTMAS_TREE = 0x53,
    MAP_CCW_SUMMER_ZUBBA_HIVE = 0x5A,
    MAP_CCW_SPRING_ZUBBA_HIVE = 0x5B,
    MAP_CCW_AUTUMN_ZUBBA_HIVE = 0x5C,
    MAP_CCW_SPRING_NABNUTS_HOUSE = 0x5E,
    MAP_CCW_SUMMER_NABNUTS_HOUSE = 0x5F,
    MAP_CCW_AUTUMN_NABNUTS_HOUSE = 0x60,
    MAP_CCW_WINTER_NABNUTS_HOUSE = 0x61,
    MAP_CCW_WINTER_HONEYCOMB_ROOM = 0x62,
    MAP_CCW_AUTUMN_NABNUTS_WATER_SUPPLY = 0x63,
    MAP_CCW_WINTER_NABNUTS_WATER_SUPPLY = 0x64,
    MAP_CCW_SPRING_WHIPCRACK_ROOM = 0x65,
    MAP_CCW_SUMMER_WHIPCRACK_ROOM = 0x66,
    MAP_CCW_AUTUMN_WHIPCRACK_ROOM = 0x67,
    MAP_CCW_WINTER_WHIPCRACK_ROOM = 0x68,
    MAP_69_GL_MM_LOBBY = 0x69,
    MAP_6A_GL_TTC_AND_CC_PUZZLE = 0x6A,
    MAP_6B_GL_180_NOTE_DOOR = 0x6B,
    MAP_6C_GL_RED_CAULDRON_ROOM = 0x6C,
    MAP_6D_GL_TTC_LOBBY = 0x6D,
    MAP_6E_GL_GV_LOBBY = 0x6E,
    MAP_6F_GL_FP_LOBBY = 0x6F,
    MAP_70_GL_CC_LOBBY = 0x70,
    MAP_71_GL_STATUE_ROOM,
    MAP_72_GL_BGS_LOBBY,
    MAP_74_GL_GV_PUZZLE = 0x74,
    MAP_75_GL_MMM_LOBBY,
    MAP_76_GL_640_NOTE_DOOR,
    MAP_77_GL_RBB_LOBBY,
    MAP_78_GL_RBB_AND_MMM_PUZZLE,
    MAP_79_GL_CCW_LOBBY,
    MAP_7A_GL_CRYPT,
    MAP_7B_CS_INTRO_GL_DINGPOT_1,
    MAP_7C_CS_INTRO_BANJOS_HOUSE_1,
    MAP_7D_CS_SPIRAL_MOUNTAIN_1,
    MAP_7E_CS_SPIRAL_MOUNTAIN_2,
    MAP_7F_FP_WOZZAS_CAVE,
    MAP_80_GL_FF_ENTRANCE,
    MAP_81_CS_INTRO_GL_DINGPOT_2,
    MAP_82_CS_ENTERING_GL_MACHINE_ROOM,
    MAP_83_CS_GAME_OVER_MACHINE_ROOM,
    MAP_84_CS_UNUSED_MACHINE_ROOM,
    MAP_85_CS_SPIRAL_MOUNTAIN_3,
    MAP_86_CS_SPIRAL_MOUNTAIN_4,
    MAP_87_CS_SPIRAL_MOUNTAIN_5,
    MAP_88_CS_SPIRAL_MOUNTAIN_6,
    MAP_89_CS_INTRO_BANJOS_HOUSE_2,
    MAP_8A_CS_INTRO_BANJOS_HOUSE_3,
    MAP_8B_RBB_ANCHOR_ROOM,
    MAP_8C_SM_BANJOS_HOUSE,
    MAP_8D_MMM_INSIDE_LOGGO,
    MAP_8E_GL_FURNACE_FUN,
    MAP_8F_TTC_SHARKFOOD_ISLAND,
    MAP_90_GL_BATTLEMENTS,
    MAP_91_FILE_SELECT,
    MAP_92_GV_SNS_CHAMBER,
    MAP_93_GL_DINGPOT,
    MAP_94_CS_INTRO_SPIRAL_7,
    MAP_95_CS_END_ALL_100,
    MAP_96_CS_END_BEACH_1,
    MAP_97_CS_END_BEACH_2,
    MAP_98_CS_END_SPIRAL_MOUNTAIN_1,
    MAP_99_CS_END_SPIRAL_MOUNTAIN_2
};
enum comusic_e
{
    SFX_DING_A = 0x00,
    MUSIC_FINAL_BATTLE,
    MUSIC_MM,
    MUSIC_FP_TWINKLIES_TALKING,
    MUSIC_MMM_CLOCK_VERSION,
    MUSIC_TTC_VACATION_VERSION,
    MUSIC_BGS,
    MUSIC_CC_MUTANT_CRABS,
    MUSIC_STARTUP_MUSICAL,
    SFX_NOTE_COLLECTED,
    SFX_JINJO_COLLECTED,
    SFX_RED_FEATHER_COLLECTED,
    SFX_EGG_COLLECTED,
    JINGLE_JIGGY_COLLECTED,
    SFX_MMM_BACKGROUND_WIND,
    MUSIC_MMM_ALTERNATIVE,
    MUSIC_SM = 0x10,
    SFX_SEAGULLS,
    MUSIC_TTC_NIPPER,
    MUSIC_INSIDE_SANDCASTLE,
    SFX_GOLD_FEATHER_COLLECTED,
    COMUSIC_15_EXTRA_LIFE_COLLECTED,
    COMUSIC_16_HONEYCOMB_COLLECTED,
    COMUSIC_17_EMPTY_HONEYCOMB_COLLECTED,
    JINGLE_HEALTH_UPGRADE,
    COMUSIC_19_LOW_PITCH_FLUTES,
    JINGLE_DEATH,
    MUSIC_MYSTERIOUS_INDOORS,
    MUSIC_CC_ALTERNATIVE,
    JINGLE_MUMBO_TRANSFORMATION,
    MUSIC_GL_MM_VERSION,
    MUSIC_CC_INSIDE_CLANKER,
    MUSIC_GV_ALTERNATIVE = 0x20,
    MUSIC_MMM_INSIDE_MMM_MANSION,
    MUSIC_MMM,
    MUSIC_MMM_INSIDE_CHURCH,
    MUSIC_GV_INSIDE_PYRAMID,
    MUSIC_USING_GOLD_FEATHERS,
    MUSIC_GV_SANDYBUTT_DANGER,
    JINGLE_GV_RUBEES_SONG,
    MUSIC_SOMETHING_AQUATIC,
    MUSIC_CCW_SUMMER,
    MUSIC_CCW_WINTER,
    COMUSIC_2B_DING_B,
    COMUSIC_2C_BUZZER,
    COMUSIC_2D_PUZZLE_SOLVED_FANFARE,
    MUSIC_CCW_FALL_AQUATIC,
    MUSIC_CCW_HUBROOM,
    COMUSIC_30_5TH_JINJO_COLLECTED = 0x30,
    COMUSIC_31_GAME_OVER,
    COMUSIC_32_STARTUP_LOGO_SCENE,
    COMUSIC_33_RBB_ALTERNATIVE,
    COMUSIC_34_SNACKER_DANGER,
    COMUSIC_35_RBB_MASCHINE_ROOM_AQUATIC,
    COMUSIC_36_100TH_NOTE_COLLECTED,
    COMUSIC_37_DOUBLE_HEALTH_UPGRADE,
    COMUSIC_38_MOTZAND_BEATEN,
    COMUSIC_39_DK64_FUNGI_FOREST,
    COMUSIC_3A_FP_BOGGY_RACE,
    COMUSIC_3B_MINIGAME_VICTORY,
    COMUSIC_3C_MINIGAME_LOSS,
    COMUSIC_3D_JIGGY_SPAWN,
    COMUSIC_3E_SANDYBUTT_FAILURE,
    COMUSIC_3F_MAGIC_CARPET_RISING,
    COMUSIC_40_MAGIC_CARPET_SINKING,
    COMUSIC_41_MUMBOS_HUT,
    COMUSIC_42_NOTEDOOR_OPENING_FANFARE,
    COMUSIC_43_ENTER_LEVEL_GLITTER,
    COMUSIC_44_CCW_NABNUT,
    COMUSIC_45_CCW_NABNUT_ATTIC_A,
    COMUSIC_46_CCW_SPRING,
    COMUSIC_47_BGS_INSIDE_TANKTUP,
    COMUSIC_48_CCW_ALTERNATIVE_A,
    COMUSIC_49_CCW_ALTERNATIVE_QUICK,
    COMUSIC_4A_RBB_INSIDE_CONTAINER,
    COMUSIC_4B_CCW_ZUBBA_FIGHT,
    COMUSIC_4C_RBB_CREWMATE_CABIN,
    COMUSIC_4D_MUMBO_DANCE,
    COMUSIC_4E_IN_TRANSITION,
    COMUSIC_4F_OUT_TRANSITION,
    MUSIC_GL_TTC_VERSION = 0x50,
    MUSIC_GL_CCW_VERSION,
    MUSIC_GL_BGS_RBB_VERSION,
    MUSIC_GL_FP_VERSION_A,
    MUSIC_GL_GV_VERSION,
    MUSIC_BGS_MR_VILE,
    MUSIC_SM_HANGBRIDGE,
    MUSIC_TURBO_TRAINERS,
    MUSIC_WADING_BOOTS,
    MUSIC_GL_FP_VERSION_B,
    MUSIC_FP_IGLOO_SAD,
    MUSIC_FP_IGLOO_HAPPY,
    MUSIC_BETA_GAME_OVER,
    MUSIC_GL_MMM_VERSION,
    MUSIC_GL_MMM_RBB_VERSION,
    MUSIC_CCW_ALTERNATIVE_B,
    MUSIC_NABNUT_ATTIC_B = 0x60,
    JINGLE_CHRISTMAS_TREE_LIGHTS_UP,
    MUSIC_RBB_BOOMBOX,
    MUSIC_GL_FF_VERSION,
    COMUSIC_64_WORLD_OPENING_A,
    JINGLE_WORLD_OPENING_B,
    MUSIC_FP_INSIDE_WOZZAS_CAVE,
    SFX_INSERTING_JIGGY,
    MUSIC_TWINKLY_MINIGAME,
    SFX_FF_WARP,
    MUSIC_MMM_TUMBLARS_SHED,
    MUSIC_FP_ALTERNATIVE,
    MUSIC_INTRO_TOWER_SCENE,
    MUSIC_CCW_GNAWTYS_HOUSE,
    MUSIC_GAME_SELECT,
    COMUSIC_6F_PAUSE_SCREEN,
    MUSIC_MMM_INSIDE_LOGGO = 0x70,
    MUSIC_FF,
    MUSIC_BGS_FLIBBIT_FIGHT,
    COMUSIC_73_GAMEBOY,
    MUSIC_GL_FINAL_SECTION,
    SFX_RED_FEATHER_REFILL,
    SFX_GOLD_FEATHER_REFILL,
    SFX_EGG_REFILL,
    JINGLE_DOOR_OF_GRUNTY_OPENED,
    MUSIC_CHEATO,
    MUSIC_BRENTILDA,
    SFX_STEP_ON_SKULL_TILE,
    SFX_STEP_ON_GRUNTY_TILE,
    SFX_STEP_ON_BK_TILE,
    SFX_STEP_ON_MINIGAME_TILE,
    SFX_STEP_ON_JOKER_TILE,
    MUSIC_GAME_OVER_CUTSCENE = 0x80,
    SFX_ACTIVATING_BRENTILDA,
    SFX_REMOVE_JIGGY,
    MUSIC_GV_SNS,
    MUSIC_TTC_SNS,
    MUSIC_FP_SNS,
    MUSIC_MMM_SNS,
    MUSIC_MMM_SNS_HAPPIER,
    COMUSIC_88_BIG_SNS_FANFARE,
    SFX_JINJO_STATUE_POWERUP,
    JINGLE_GETTING_TURBO_TRAINERS,
    SFX_DEACTIVATE_BRENTILDA,
    SFX_JINJONATOR_POWERUP,
    JINGLE_MENACING_GRUNTILDA_A,
    MUSIC_CREDITS,
    JINGLE_MENACING_GRUNTILDA_B,
    JINGLE_END_OF_INTRO = 0x90,
    MUSIC_GRUNTY_FALLING,
    SFX_GRUNTY_SPELL_POWERUP,
    SFX_AIR_METER_DROPPING,
    MUSIC_BBONUS,
    JINGLE_BBONUS_A,
    SFX_BBONUS_PICKUP_PIECE,
    SFX_BBONUS_DROP_PIECE,
    JINGLE_BBONUS_PIECES_SHUFFLE,
    MUSIC_MUMBO_BBQ,
    SFX_JINJONATOR_HITS_GRUNTY_A,
    SFX_JINJONATOR_HITS_GRUNTY_B,
    SFX_JINJONATOR_HITS_GRUNTY_C,
    SFX_JINJONATOR_HITS_GRUNTY_D,
    SFX_JINJONATOR_HITS_GRUNTY_E,
    SFX_JINJONATOR_HITS_GRUNTY_F,
    SFX_JINJONATOR_HITS_GRUNTY_G = 0xA0,
    SFX_JINJONATOR_HITS_GRUNTY_H,
    SFX_JINJONATOR_HITS_GRUNTY_I,
    SFX_JINJONATOR_HITS_GRUNTY_J,
    JINGLE_JINJONATOR_DRUMMING_A,
    JINGLE_JINJONATOR_DRUMMING_B,
    JINGLE_JINJONATOR_DRUMMING_C,
    JINGLE_JINJONATOR_DRUMMING_D,
    MUSIC_KLUNGO_BY_FALLEN_GRUNTY,
    MUSIC_TOOTY,
    MUSIC_BEACH,
    JINGLE_JINJOATOR_FINAL_HIT,
    JINGLE_GOOD_ENDING = 0xAC
};
enum sfx_e
{
    SFX_0_BLOOP = 0x0000,
    SFX_1_MUMBO_UMENAKA,
    SFX_2_CLAW_SWIPE,
    SFX_3_DULL_CANNON_SHOT,
    SFX_4_KAZOOIE_RUUUUUH,
    SFX_5_BANJO_LANDING_01,
    SFX_6_BANJO_LANDING_02,
    SFX_7_BANJO_LANDING_03,
    SFX_8_BANJO_LANDING_04,
    SFX_9_SQUEAKY_TOY,
    SFX_A_BANJO_LANDING_05,
    SFX_B_BANJO_LANDING_06,
    SFX_C_TAKING_FLIGHT_LIFTOFF,
    SFX_D_EGGSHELL_BREAKING,
    SFX_E_SHOCKSPRING_BOING,
    SFX_F_SMALL_WATER_SPLASH,
    SFX_10_BANJO_LANDING_07,
    SFX_11_WOOD_BREAKING_1,
    SFX_12_WATER_PADDLING_1,
    SFX_13_BEAKBUSTER_GROUND,
    SFX_14_METALLIC_HIT_1,
    SFX_15_METALLIC_HIT_2,
    SFX_16_HEAVY_FALL_VIBRATO,
    SFX_17_JINJO_WHISTLE,
    SFX_18_BIGBUTT_SLIDE,
    SFX_19_BANJO_LANDING_08,
    SFX_1A_BIG_THINGS_FALL_OVER,
    SFX_1B_EXPLOSION_1,
    SFX_1C_ALARMCLOCK,
    SFX_1D_HITTING_AN_ENEMY_1,
    SFX_1E_HITTING_AN_ENEMY_2,
    SFX_1F_HITTING_AN_ENEMY_3,
    SFX_20_METAL_CLANK_1,
    SFX_21_EGG_BOUNCE_1,
    SFX_22_KONGA_NOISE_1,
    SFX_23_KONGA_NOISE_2,
    SFX_24_KONGA_NOISE_3,
    SFX_25_METAL_SLIDING_OVER_SMTH,
    SFX_26_BANJO_LANDING_09,
    SFX_27_JINJO_HI,
    SFX_28_RUSTLING_NOISE,
    SFX_29_GRUBLIN_NYAHAHA,
    SFX_2A_CLOCK_TIC_1,
    SFX_2B_BULL_MOO_1,
    SFX_2C_PULLING_NOISE,
    SFX_2D_KABOING,
    SFX_2E_BIGBUTT_RUNNING,
    SFX_2F_ORANGE_SPLAT,
    SFX_30_MAGIC_POOF,
    SFX_31_BANJO_OHHWAAOOO,
    SFX_32_BANJO_EGHEE,
    SFX_33_BANJO_AHOO,
    SFX_34_BANJO_AGHOAA,
    SFX_35_BANJO_WOAH,
    SFX_36_BANJO_DOH,
    SFX_37_BANJO_OHWW,
    SFX_38_BANJO_AYE_1,
    SFX_39_BANJO_AYE_2,
    SFX_3A_BANJO_HOUW,
    SFX_3B_BANJO_GAAH,
    SFX_3C_BULL_GROWN,
    SFX_3D_TICKER_WALKING,
    SFX_3E_POOP_NOISE,
    SFX_3F_CAULDRON_SQEAK_1,
    SFX_40_CAULDRON_SQEAK_2,
    SFX_41_MUMBO_ERGHHH,
    SFX_42_KAZOOIE_RAH,
    SFX_43_KAZOOIE_RUH,
    SFX_44_KAZOOIE_AUW,
    SFX_45_KAZOOIE_HUGHH,
    SFX_46_KAZOOIE_CHOKING_UP,
    SFX_47_KAZOOIE_HEUGH,
    SFX_48_KAZOOIE_RUUH,
    SFX_49_KAZOOIE_RA,
    SFX_4A_CLUCKER_AHH,
    SFX_4B_GULPING,
    SFX_4C_LIP_SMACK,
    SFX_4D_WET_WIGGLING,
    SFX_4E_KAZOOIE_BRUIII,
    SFX_4F_BANJO_WAHOO,
    SFX_50_KAZOOIE_RRRUH,
    SFX_51_CLOCK_TIC_2,
    SFX_52_BANJO_YAH_OH,
    SFX_53_BANJO_HUIII,
    SFX_54_BANJO_HOO_1,
    SFX_55_BANJO_HOO_2,
    SFX_56_BANJO_HUI,
    SFX_57_KAZOOIE_HEGH,
    SFX_58_CHIMPY_NOISE_1,
    SFX_59_CHIMPY_NOISE_2,
    SFX_5A_CHIMPY_NOISE_3,
    SFX_5B_HEAVY_STUFF_FALLING,
    SFX_5C_HEAVY_STUFF_FALLING_DELAYED,
    SFX_5D_BANJO_RAAOWW,
    SFX_5E_BANJO_PHEWWW,
    SFX_5F_MUMBO_BUGABUGOW_MUFFLED,
    SFX_60_MUMBO_SKIDDING,
    SFX_61_CARTOONY_FALL,
    SFX_62_BANJO_ERGHH,
    SFX_63_BANJO_UWAAAAOOH,
    SFX_64_STAMPEDE,
    SFX_65_METALLIC_SCRATCH,
    SFX_66_BIRD_AUUGHH,
    SFX_67_BEEHIVE_CLONK,
    SFX_68_CLUCKER_AAEEGHH,
    SFX_69_WHIPCRACK_CREAKING,
    SFX_6A_FLAGPOLE_WOBBLE,
    SFX_6B_LOCKUP_OPENING,
    SFX_6C_LOCKUP_CLOSING,
    SFX_6D_CROC_BITE,
    SFX_6E_VILE_EGH,
    SFX_6F_BANJO_HEADSCRATCH,
    SFX_70_WALKING_NOISE_1,
    SFX_WALKING_NOISE_2,
    SFX_WALKING_NOISE_3,
    SFX_WALKING_NOISE_4,
    SFX_WALKING_NOISE_5,
    SFX_WALKING_NOISE_6,
    SFX_WALKING_NOISE_7,
    SFX_WALKING_NOISE_8,
    SFX_EAGLECRY,
    SFX_TICKER_DEATH,
    SFX_TERMITE_DEATH,
    SFX_ICE_BREAKING_1,
    SFX_CHEBOOF,
    SFX_7D_ANCHOR_LIFTING,
    SFX_7E_CREAKY_DOOR_OPENING,
    SFX_7F_HEAVYDOOR_SLAM,
    SFX_80_YUMYUM_CLACK = 0x0080,
    SFX_81_UUU,
    SFX_82_METAL_BREAK,
    SFX_83_BLUBBER_CRYING,
    SFX_84_GOBI_CRYING,
    SFX_85_ROUGH_COUGH,
    SFX_86_TIPTUP_CHORUS_AH,
    SFX_87_TANKTUP_OOOHW,
    SFX_88_WOZZA_NOISE,
    SFX_89_PARTYTOOL,
    SFX_8A_ALTERNATIVE_EGG_SHOT,
    SFX_8B_KAZOOIE_RAWW,
    SFX_8C_BOGGY_WAHEY,
    SFX_8D_BOGGY_OHWW,
    SFX_8E_GRUNTLING_DAMAGE,
    SFX_8F_SNOWBALL_FLYING,
    SFX_90_SWITCH_PRESS,
    SFX_91_METALLIC_SOUND,
    SFX_92_TOILET_FLUSH,
    SFX_93_MICROWAVE,
    SFX_94_COGS_ROTATING,
    SFX_95_BANJO_TALKING,
    SFX_96_HOTSAND_EEL_HISS,
    SFX_97_BLUBBER_BURPS,
    SFX_98_DEAF_THUD,
    SFX_99_METALLIC_THUD,
    SFX_9A_MECHANICAL_CLOSING,
    SFX_9B_BOULDER_BREAKING_1,
    SFX_9C_BOULDER_BREAKING_2,
    SFX_9D_BOULDER_BREAKING_3,
    SFX_9E_BOULDER_BREAKING_4,
    SFX_9F_GENERATOR_RUNNING,
    SFX_A0_COUGHING = 0x00A0,
    SFX_A1_BANJO_COUGHING,
    SFX_A2_BANJO_AHOOA,
    SFX_A3_BANJO_DOUBLE_COUGH,
    SFX_BANJO_SINGLE_COUGH,
    SFX_EYRIE_CRY,
    SFX_MAGICAL_FINISH,
    SFX_WOODEN_SWOSH,
    SFX_BLUBBER_DOUBLE_CRY,
    SFX_BLUBBER_SINGLE_CRY,
    SFX_BGS_EGG_BREAKING_1,
    SFX_BGS_EGG_BREAKING_2,
    SFX_GOLDFEATHER_TALKING,
    SFX_CATERPILLAR_SQUEAK,
    SFX_YUMYUM_TALKING,
    SFX_BANJO_CATCHING_BREATH,
    SFX_B0_SIZZLING_NOISE = 0x00B0,
    SFX_B1_BOGGY_KID_CRYING,
    SFX_B2_BOGGY_KID_HAPPY,
    SFX_B3_ORANGE_TALKING,
    SFX_B4_BOTTLES_TALKING_1,
    SFX_B5_THUNDERBOLT,
    SFX_B6_GLASS_BREAKING_1,
    SFX_B7_GLASS_BREAKING_2,
    SFX_B8_GLASS_BREAKING_3,
    SFX_B9_GLASS_BREAKING_4,
    SFX_BA_GLASS_BREAKING_5,
    SFX_BB_GLASS_BREAKING_6,
    SFX_BC_BOTTLES_TALKING_2,
    SFX_BD_BOTTLES_TALKING_3,
    SFX_BE_WATERFALL,
    SFX_BF_EYRIE_SLEEPING_1,
    SFX_C0_EYRIE_SLEEPING_2,
    SFX_BUZZBOMB_ATTACK,
    SFX_GRUBLIN_EGH,
    SFX_HEGH,
    SFX_TWINKLY_MUNCHER_GRR,
    SFX_C5_TWINKLY_POP,
    SFX_C6_SHAKING_MOUTH,
    SFX_SHWOOP,
    SFX_C8_CRUNCH,
    SFX_C9_PAUSEMENU_ENTER,
    SFX_BANJO_DROWNING_1,
    SFX_BANJO_DROWNING_2,
    SFX_CC_PAUSEMENU_ENTER_SUBMENU,
    SFX_CD_PAUSEMENU_LEAVE_SUBMENU,
    SFX_CE_PAUSEMENU_HOIP,
    SFX_CF_PAUSEMENU_SHWOOP,
    SFX_GRIMLET_SQUEAK = 0x00D0,
    SFX_SNORKEL_WAH,
    SFX_QUIET_METALLIC,
    SFX_JINXIE_SNIFFLING_1,
    SFX_JINXIE_SNIFFLING_2,
    SFX_JINXIE_SNEEZING,
    SFX_UGH,
    SFX_D7_GRABBA_DEATH,
    SFXR_CRANE,
    SFX_WOODEN_CRATE_BREAKING_1,
    SFX_WOODEN_CRATE_BREAKING_2,
    SFX_WOODEN_CRATE_BREAKING_3,
    SFX_IDLE_PADDLING,
    SFX_JINJO_TALKING,
    SFX_WOOD_SQUEAK,
    SFX_KAZOOIE_TALKING_1,
    SFX_KAZOOIE_TALKING_2 = 0x00E0,
    SFX_KAZOOIE_TALKING_3,
    SFX_KAZOOIE_TALKING_4,
    SFX_KONGA_TALKING_1,
    SFX_KONGA_TALKING_2,
    SFX_KONGA_TALKING_3,
    SFX_MEEP_1,
    SFX_MEEP_2,
    SFX_TOOTY_TALKING_1,
    SFX_TOOTY_TALKING_2,
    SFX_EA_GRUNTY_LAUGH_1,
    SFX_GRUNTY_LAUGH_2,
    SFX_GRUNTY_TALKING_1,
    SFX_GRUNTY_TALKING_2,
    SFX_GRUNTY_TALKING_3,
    SFX_TIPTUP_TALKING,
    SFX_CHEATO_IDLE_1 = 0x00F0,
    SFX_CHEATO_IDLE_2,
    SFX_CUCKOO_CLOCK,
    SFX_TRUNKER_TALKING,
    SFX_TAP,
    SFX_BLUBBER_TALKING_1,
    SFX_BLUBBER_TALKING_2,
    SFX_BLUBBER_TALKING_3,
    SFX_BLUBBER_TALKING_4,
    SFX_GRUNTLING_NOISE_1,
    SFX_GRUNTLING_NOISE_2,
    SFX_GRUNTLING_NOISE_3,
    SFX_RUBEE_TALKING_1,
    SFX_RUBEE_TALKING_2,
    SFX_RUBEE_TALKING_3,
    SFX_TANKTUP_TALKING_1,
    SFX_TANKTUP_TALKING_2,
    SFX_TANKTUP_TALKING_3 = 0x0100,
    SFX_LOGGO_TALKING,
    SFX_ZUBBA_TALKING,
    SFX_FLOTSAM_DEATH,
    SFXR_PROPELLER_NOISE,
    SFX_EYRIE_YAWN,
    SFX_EYRIE_LIPSMACK,
    SFX_CAULDRON_ACTIVATION_1,
    SFX_CAULDRON_ACTIVATION_2,
    SFX_LOGGO_LID_CLAP,
    SFX_TUMBLAR_TALKING,
    SFX_GNAWTY_TALKING,
    SFX_MUMMY_TALKING,
    SFX_ANCIENT_ONE_TALKING,
    SFX_TWINKLY_TALKING,
    SFX_BOGGY_TALKING,
    SFX_110_TWINKLY_DEATH = 0x0110,
    SFX_111_WHIPCRACK_DEATH,
    SFX_112_TINKER_ATTENTION,
    SFX_113_PAD_APPEARS,
    SFX_114_BRICKWALL_BREAKING,
    SFX_115_BUZZBOMB_DEATH,
    SFX_116_DEAF_RUSTLING,
    SFX_117_LIMBO_COLLAPSING,
    SFX_118_LIMBO_REASSEMBLING,
    SFX_119_FISH_DEATH,
    SFX_11A_MATROSE_GRUBLIN_AYE,
    SFX_11B_TUMBLAR_DISAPPEARING_1,
    SFX_11C_TUMBLAR_DISAPPEARING_2,
    SFX_11D_TUMBLAR_DISAPPEARING_3,
    SFX_11E_TUMBLAR_DISAPPEARING_4,
    SFX_11F_TUMBLAR_DISAPPEARING_5,
    SFX_120_TUMBLAR_DISAPPEARING_6,
    SFX_AWAWAU,
    SFX_TOOTY_TALKING_3,
    SFX_BANJO_LANDING_10,
    SFX_AUDIENCE_CHEERING_1,
    SFX_AUDIENCE_CHEERING_2,
    SFX_126_AUDIENCE_BOOING,
    SFX_127_AUDIENCE_MIXED,
    SFX_128_FIRE_CRACKING,
    SFX_SWOOSH,
    SFX_GRUNTY_AH,
    SFX_12B_BOILING_AND_BUBBLING,
    SFX_FF_QUESTION_START,
    SFX_CAMERA_ZOOM_CLOSEST,
    SFX_CAMERA_ZOOM_MEDIUM,
    SFX_FUUUCK_YOUUU,
    SFX_GRUNTY_ECHOING_CRY = 0x0130,
    SFX_GRUNTY_WEEEGH,
    SFX_GRUNTY_YOW,
    SFX_GRUNTY_OHW,
    SFX_FREEZING_SHIVER,
    SFX_CARTOONY_SPRING,
    SFX_GAMEBOY_STARTUP,
    SFX_GAMEBOY_BOIN,
    SFX_GAMEBOY_BOIOIN,
    SFX_GAMEBOY_GLITCH,
    SFX_GLASS_BREAKING_7,
    SFX_GRUNTY_FINAL_SCREAM_1,
    SFX_GRUNTY_FINAL_SCREAM_2,
    SFX_GRUNTY_FINAL_SCREAM_3,
    SFX_GRUNTY_FINAL_SCREAM_4,
    SFX_GRUNTY_FINAL_SCREAM_5,
    SFX_GRUNTY_FINAL_SCREAM_6 = 0x0140,
    SFX_141_MECHANICAL_WINCH,
    SFX_GRUNTY_LAUGH_3,
    SFX_143_BULL_DAMAGE,
    SFX_DOUBLE_CAMERA_CLICK,
    SFX_SINGLE_CAMERA_CLICK,
    SFX_GRUNTY_SPELL_ATTACK_1,
    SFX_GRUNTY_SPELL_ATTACK_2,
    SFX_GRUNTY_SPELL_LANDING,
    SFX_CHEATO_TALKING_1,
    SFX_CHEATO_TALKING_2,
    SFX_GRUNTY_LAUGH_4,
    SFX_BRENTILDA_TALKING,
    SFX_BANJO_FREEZING,
    SFX_SOFT_EXPLOSION,
    SFX_FIREWORK_WHISTLING,
    SFX_PORCELAIN_CRASH = 0x0150,
    SFX_CAT_MEOW,
    SFX_MOTOR_BREAKDOWN_01,
    SFX_MOTOR_BREAKDOWN_02,
    SFX_MOTOR_BREAKDOWN_03,
    SFX_MOTOR_BREAKDOWN_04,
    SFX_MOTOR_BREAKDOWN_05,
    SFX_MOTOR_BREAKDOWN_06,
    SFX_MOTOR_BREAKDOWN_07,
    SFX_MOTOR_BREAKDOWN_08,
    SFX_MOTOR_BREAKDOWN_09,
    SFX_MOTOR_BREAKDOWN_10,
    SFX_MOTOR_BREAKDOWN_11,
    SFX_MOTOR_BREAKDOWN_12,
    SFX_MOTOR_BREAKDOWN_13,
    SFX_MOTOR_BREAKDOWN_14,
    SFX_MOTOR_BREAKDOWN_15 = 0x0160,
    SFX_MOTOR_BREAKDOWN_16,
    SFX_MOTOR_RUCKUS,
    SFX_GRUNTY_WILD_SCREAM,
    SFX_EH,
    SFX_HEAVY_THUNDERSTORM_01,
    SFX_HEAVY_THUNDERSTORM_02,
    SFX_HEAVY_THUNDERSTORM_03,
    SFX_HEAVY_THUNDERSTORM_04,
    SFX_HEAVY_THUNDERSTORM_05,
    SFX_HEAVY_THUNDERSTORM_06,
    SFX_HEAVY_THUNDERSTORM_07,
    SFX_HEAVY_THUNDERSTORM_08,
    SFX_HEAVY_THUNDERSTORM_09,
    SFX_170_HEAVY_THUNDERSTORM_10 = 0x170,
    SFX_171_HEAVY_THUNDERSTORM_11,
    SFX_172_HEAVY_THUNDERSTORM_12,
    SFX_173_HEAVY_THUNDERSTORM_13,
    SFX_174_HEAVY_THUNDERSTORM_14,
    SFX_175_HEAVY_THUNDERSTORM_15,
    SFX_176_JINJONATOR_JINJOOO_1,
    SFX_177_JINJONATOR_JINJOOO_2,
    SFX_178_JINJONATOR_JINJOOO_3,
    SFX_179_GRUNTY_DAMAGE,
    SFX_17A_SHIPHORN,
    SFX_17B_AIRPLANE_FALLING,
    SFX_17C_GRUNTY_FALLING_OFF_1,
    SFX_17D_GRUNTY_FALLING_OFF_2,
    SFX_17E_MUMBO_TRANSFORMATION_01,
    SFX_17F_MUMBO_TRANSFORMATION_02,
    SFX_180_MUMBO_TRANSFORMATION_03,
    SFX_MUMBO_TRANSFORMATION_04,
    SFX_MUMBO_TRANSFORMATION_05,
    SFX_MUMBO_TRANSFORMATION_06,
    SFX_MUMBO_TRANSFORMATION_07,
    SFX_MUMBO_TRANSFORMATION_08,
    SFX_MUMBO_TRANSFORMATION_09,
    SFX_MUMBO_TRANSFORMATION_10,
    SFX_MUMBO_TRANSFORMATION_11,
    SFX_MUMBO_TRANSFORMATION_12,
    SFX_MUMBO_TRANSFORMATION_13,
    SFX_MUMBO_TRANSFORMATION_14,
    SFX_EYRIE_MAMA,
    SFX_SEXY_GRUNTY_TALKING_1,
    SFX_SEXY_GRUNTY_TALKING_2,
    SFX_FREEZE_FRAME,
    SFX_GULP = 0x0190,
    SFX_STATIC,
    SFX_3E9_UNKNOWN = 0x3e9,
    SFX_3EA_UNKNOWN,
    SFX_3EB_UNKNOWN,
    SFX_3EC_CCW_DOOR_OPENING,
    SFX_3F1_UNKNOWN = 0x3f1,
    SFX_3F6_UNKNOWN = 0x3f6,
    SFX_3F9_UNKNOWN = 0x3f9,
    SFX_413_UNKNOWN = 0x413
};
enum level_e
{
    LEVEL_1_MUMBOS_MOUNTAIN = 0x1,
    LEVEL_2_TREASURE_TROVE_COVE,
    LEVEL_3_CLANKERS_CAVERN,
    LEVEL_4_BUBBLEGLOOP_SWAMP,
    LEVEL_5_FREEZEEZY_PEAK,
    LEVEL_6_LAIR,
    LEVEL_7_GOBIS_VALLEY,
    LEVEL_8_CLICK_CLOCK_WOOD,
    LEVEL_9_RUSTY_BUCKET_BAY,
    LEVEL_A_MAD_MONSTER_MANSION,
    LEVEL_B_SPIRAL_MOUNTAIN,
    LEVEL_C_BOSS,
    LEVEL_D_CUTSCENE
};
enum jiggy_e
{
    JIGGY_MM_JUJU = 0x04,
    jiggy_mm_huts = 0x05,
    JIGGY_8_MM_ORANGE_PADS = 0x08,
    JIGGY_9_MM_CHIMPY = 0x9,
    JIGGY_13_TTC_LOCKUP = 0x13,
    JIGGY_17_CC_CLANKER_RAISED = 0x17,
    JIGGY_1C_CC_RINGS = 0x1C,
    JIGGY_1D_CC_SLOW_SAWBLADES ,
    JIGGY_20_BGS_ELEVATED_WALKWAY = 0x20,
    JIGGY_21_BGS_PINKEGG,
    JIGGY_22_CROCTUS,
    JIGGY_23_BGS_HUTS,
    JIGGY_24_BGS_FLIBBITS,
    JIGGY_25_BGS_MAZE,
    JIGGY_26_BGS_TANKTUP,
    JIGGY_27_BGS_TIPTUP,
    JIGGY_2F_FP_XMAS_TREE = 0x2f,
    JIGGY_35_LAIR_CC_WITCH_SWITCH = 0x35,
    JIGGY_36_LAIR_TTC_WITCH_SWITCH,
    JIGGY_37_LAIR_BGS_WITCH_SWITCH,
    JIGGY_3E_GV_GRABBA = 0x3e,
    JIGGY_41_GV_MAZE = 0x41,
    JIGGY_49_CCW_EYRIE = 0x49,
    JIGGY_4D_CCW_FLOWER = 0x4D,
    JIGGY_RBB_SNORKEL = 0x53,
    JIGGY_RBB_WHISTLE = 0x54,
    JIGGY_RBB_BOSS_BOOM_BOX = 0x56
};
enum actor_e
{
    ACTOR_4_BIGBUTT = 0x4,
    ACTOR_6_GRUBLIN = 0x6,
    ACTOR_8_CONGA = 0x8,
    ACTOR_9_MM_HUT = 0x9,
    ACTOR_C_MUD_HUT = 0xC,
    ACTOR_D_WOOD_DEMOLISHED,
    ACTOR_E_BULL_INTRO,
    ACTOR_F_CHIMPY = 0xF,
    ACTOR_11_JUJU_CTRL = 0x11,
    ACTOR_12_BEEHIVE,
    ACTOR_14_ORANGE_PROJECTILE = 0x14,
    ACTOR_46_JIGGY = 0x46,
    ACTOR_4A_WOOD_EXPLOSION = 0x4A,
    ACTOR_4C_STEAM = 0x4C,
    ACTOR_4D_STEAM_2 = 0x4D,
    ACTOR_57_ORANGE_PAD = 0x57,
    ACTOR_59_JUJU = 0x59,
    ACTOR_5A_JIGGY_IN_HAND,
    ACTOR_C5_CHIMPY_STUMP = 0xC5,
    actor_leafboat = 0xF1,
    actor_bigalligator = 0xF6,
    ACTOR_124_SIR_SLUSH = 0x124,
    ACTOR_125_SNOWBALL,
    ACTOR_126_SIR_SLUSH_HAT,
    ACTOR_12C_MOLEHILL = 0x12C,
    actor_yellow_flibbit_controller = 0x136,
    actor_yumblie = 0x139,
    ACTOR_14F_DESTROYED_JIGGY = 0x14f,
    ACTOR_COLLYWOBBLE_A = 0x164,
    ACTOR_BAWL_A = 0x165,
    ACTOR_TOPPER_A = 0x166,
    ACTOR_ATTACK_TUTORIAL = 0x167,
    ACTOR_1CC_GRILL_CHOMPA = 0x1CC,
    ACTOR_20E_MM_ENTRANCE_DOOR = 0x20E,
    ACTOR_20F_RBB_ENTRANCE_DOOR,
    ACTOR_210_BGS_ENTRANCE_DOOR,
    ACTOR_211_CHEST_LID,
    ACTOR_212_IRON_BARS,
    ACTOR_234_CCW_ENTRANCE_DOOR = 0x234,
    ACTOR_226_GV_ENTRANCE = 0x226,
    ACTOR_228_INVISIBLE_WALL = 0x228,
    ACTOR_235_FP_ENTANCE_DOOR = 0x235,
    ACTOR_28B_SOUND_SOURCE = 0x28B,
    ACTOR_2E5_WOODEN_DOOR = 0x2e5,
    ACTOR_383_FIRE_FX = 0x383,
    ACTOR_34D_BEE_SWARM = 0x34D,
    ACTOR_354_DRIPS = 0x354,
    ACTOR_COLLYWOBBLE_B = 0x36D,
    ACTOR_BAWL_B = 0x36E,
    ACTOR_TOPPER_B = 0x36F,
    ACTOR_37A_BOTTLES = 0x37a,
    ACTOR_3BA_UNKOWN = 0x3ba
};
enum bs_e
{
    BS_IDLE = 0x1,
    BS_WALK_SLOW = 0x2,
    BS_WALK = 0x3,
    BS_WALK_FAST = 0x4,
    BS_JUMP = 0x5,
    BS_CLAW = 0x6,
    BS_CROUCH = 0x7,
    BS_BTROT_JUMP = 0x8,
    BS_EGG_HEAD = 0x9,
    BS_EGG_ASS = 0xA,
    BS_SKID = 0xC,
    BS_TIMEOUT = 0xD,
    BS_OW = 0xE,
    BS_BBUSTER = 0xF,
    BS_BFLAP = 0x10,
    BS_BPECK = 0x11,
    BS_BFLIP = 0x12,
    BS_BBARGE = 0x13,
    BS_BTROT_ENTER = 0x14,
    BS_BTROT_IDLE = 0x15,
    BS_BTROT_WALK = 0x16,
    BS_BTROT_EXIT = 0x17,
    BS_FLY_KNOCKBACK = 0x18,
    BS_WONDERWING_ENTER = 0x1A,
    BS_1B_WONDERWING_IDLE = 0x1B,
    BS_WONDERWING_WALK = 0x1C,
    BS_WONDERWING_JUMP = 0x1D,
    BS_WONDERWING_EXIT = 0x1E,
    BS_WALK_CREEP = 0x1F,
    BS_LANDING = 0x20,
    BS_BSHOCK_CHARGE = 0x21,
    BS_BSHOCK_JUMP = 0x22,
    BS_FLY_ENTER = 0x23,
    BS_FLY = 0x24,
    BS_LONGLEG_ENTER = 0x25,
    BS_LONGLEG_IDLE = 0x26,
    BS_LONGLEG_WALK = 0x27,
    BS_LONGLEG_JUMP = 0x28,
    BS_LONGLEG_EXIT = 0x29,
    BS_BOMB = 0x2A,
    BS_DIVE_IDLE = 0x2B,
    BS_DIVE_B = 0x2C,
    BS_SWIM_IDLE = 0x2D,
    BS_SWIM = 0x2E,
    BS_FALL = 0x2F,
    BS_DIVE_ENTER = 0x30,
    BS_ROLL = 0x31,
    BS_SLIDE = 0x32,
    BS_JIG_NOTEDOOR = 0x34,
    BS_ANT_IDLE = 0x35,
    BS_ANT_WALK = 0x36,
    BS_ANT_JUMP = 0x37,
    BS_ANT_FALL = 0x38,
    BS_DIVE_A = 0x39,
    BS_CARRY_IDLE = 0x3A,
    BS_CARRY_WALK = 0x3B,
    BS_FALL_TUMBLING = 0x3D,
    BS_3E_ANT_OW = 0x3E,
    BS_DIE = 0x41,
    BS_42_DINGPOT = 0x42,
    BS_43_ANT_DIE = 0x43,
    BS_JIG_JIGGY = 0x44,
    BS_BTROT_SLIDE = 0x45,
    BS_PUMPKIN_IDLE = 0x48,
    BS_PUMPKIN_WALK = 0x49,
    BS_PUMPKIN_JUMP = 0x4A,
    BS_PUMPKIN_FALL = 0x4B,
    BS_LANDING_IN_WATER = 0x4c,
    BS_PUMPKIN_OW = 0x4D,
    BS_PUMPKIN_DIE = 0x4E,
    BS_CLIMB_IDLE = 0x4f,
    BS_CLIMB_MOVE = 0x50,
    BS_CLIMB_EXIT = 0x51,
    BS_TIMEOUT_UNK53 = 0x53,
    BS_54_SWIM_DIE = 0x54,
    BS_LONGLEG_SLIDE = 0x55,
    BS_56_RECOIL = 0x56,
    BS_BOMB_END = 0x57,
    BS_BFLY_UNK59 = 0x59,
    BS_5A_LOADZONE = 0x5A,
    BS_CARRY_THROW = 0x5B,
    BS_CROC_IDLE = 0x5E,
    BS_CROC_WALK = 0x5F,
    BS_CROC_JUMP = 0x60,
    BS_CROC_FALL = 0x61,
    BS_LONGLEG_UNK62 = 0x62,
    BS_CROC_OW = 0x63,
    BS_CROC_DIE = 0x64,
    BS_WALRUS_IDLE = 0x67,
    BS_WALRUS_WALK = 0x68,
    BS_WALRUS_JUMP = 0x69,
    BS_WALRUS_FALL = 0x6A,
    BS_WALRUS_OW = 0x6C,
    BS_WALRUS_DIE = 0x6D,
    BS_CROC_BITE = 0x6E,
    BS_CROC_EAT_BAD = 0x6F,
    BS_70_CROC_EAT_GOOD = 0x70,
    BS_BTROT_FALL = 0x71,
    BS_SPLAT = 0x72,
    BS_BFLY_UNK76 = 0x76,
    BS_WALK_MUD = 0x7A,
    BS_BTROT_OW = 0x7B,
    BS_7D_WALRUS_SLED = 0x7D,
    BS_7E_WALRUS_SLED = 0x7E,
    BS_DIVE_OW = 0x7F,
    BS_BEE_IDLE = 0x85,
    BS_BEE_WALK = 0x86,
    BS_BEE_JUMP = 0x87,
    BS_BEE_FALL = 0x88,
    BS_BEE_OW = 0x89,
    BS_BEE_DIE = 0x8A,
    BS_BEE_FLY = 0x8c,
    BS_CROC_LOCKED = 0x8D,
    BS_PUMPKIN_LOCKED = 0x8F,
    BS_FLY_OW = 0x91,
    BS_ANT_DRONE = 0x92,
    BS_CROC_DRONE = 0x94,
    BS_WALRUS_DRONE = 0x95,
    BS_SWIM_LOCKED = 0x96,
    BS_DIVE_LOCKED = 0x97,
    BS_WALK_DRONE = 0x98,
 BS_BFLY_UNK99= 0x99,
    BS_BTROT_DRONE = 0x9A,
 BS_LONGLEG_DRONE = 0x9B,
    BS_7D_WALRUS_SLED_DRONE = 0x9C,
    BS_BEE_DRONE = 0x9D,
    BS_ANT_BOUNCE = 0x9F,
    BS_PUMPKIN_BOUNCE = 0xA0,
    BS_CROC_BOUNCE = 0xA1,
    BS_WALRUS_BOUNCE = 0xA2,
 BS_BEE_BOUNCE = 0xA3,
    BS_WONDERWING_DRONE = 0xA4,
    BS_WONDERWING_UNKA5 = 0xA5
};
enum map_flags_e
{
    mapflag_mm_main_hit_with_orange = 8
};
enum item_e
{
    ITEM_0_HOURGLASS_TIMER = 0x0,
    ITEM_3_PROPELLOR_TIMER = 0x3,
    ITEM_6_HOURGLASS = 0x6,
    ITEM_9_PROPELLOR = 0x9,
    ITEM_C_NOTE = 0xC,
    ITEM_D_EGGS,
    ITEM_E_JIGGY,
    ITEM_F_RED_FEATHER,
    ITEM_10_GOLD_FEATHER,
    ITEM_12_JINJOS = 0x12,
    ITEM_13_EMPTY_HONEYCOMB,
    ITEM_14_HEALTH,
    ITEM_15_HEALTH_TOTAL,
    ITEM_16_LIFE,
    ITEM_17_AIR,
    ITEM_ORANGE = 0x19,
    item_mumbo_token = 0x1C,
    item_mumbo_token_total = 0x25,
    item_jiggy_total = 0x26
};
enum animctrl_direction_e
{
    mvmt_dir_forwards = 1
};
enum game_mode_e
{
    GAME_MODE_3_NORMAL = 3,
    GAME_MODE_4_PAUSED = 4,
    GAME_MODE_6_FILE_PLAYBACK = 6,
    GAME_MODE_7_ATTRACT_DEMO = 7,
    GAME_MODE_8_BOTTLES_BONUS = 8,
    GAME_MODE_9_BANJO_AND_KAZOOIE = 9,
    GAME_MODE_A_SNS_PICTURE = 10
};
enum asset_e
{
    ANIM_BANJO_CROUCH_ENTER = 0x1,
    ANIM_BANJO_WALK_CREEP = 0x2,
    ANIM_BANJO_WALK = 0x3,
    ANIM_BANJO_BTROT_EXIT = 0x7,
    ANIM_BANJO_JUMP = 0x8,
    ANIM_BANJO_DIE = 0x9,
    ANIM_BANJO_WALK_MUD = 0xb,
    ANIM_BANJO_RUN = 0xc,
    ANIM_BANJO_TURN = 0xe,
    ASSET_10_ANIM_BIGBUTT_RUN = 0x10,
    ANIM_BANJO_WONDERWING_WALK = 0x11,
    ANIM_BANJO_LONGLEG_ENTER_AS_BEAR = 0x16,
    ANIM_BANJO_BFLAP = 0x17,
    ANIM_BANJO_BPECK_ENTER = 0x19,
    ANIM_BANJO_BPECK = 0x1A,
    ANIM_BANJO_WONDERWING_JUMP = 0x1B,
    ANIM_BANJO_BBARGE = 0x1C,
    ANIM_BANJO_BBUSTER = 0x1D,
    ASSET_21_ANIM_BIGBUTT_SLIDE = 0x21,
    ANIM_BANJO_WONDERWING_EXIT = 0x22,
    ANIM_BANJO_WONDERWING_IDLE = 0x23,
    ANIM_BANJO_BTROT_IDLE = 0x26,
    ANIM_BANJO_BTROT_JUMP = 0x27,
    ANIM_TERMITE_OW = 0x28,
    ANIM_TERMITE_DIE = 0x29,
    ANIM_BANJO_BJIG_JIGGY = 0x2e,
    ASSET_32_ANIM_BIGBUTT_ATTACK = 0x32,
    ASSET_33_ANIM_BIGBUTT_EAT,
    ASSET_34_ANIM_BIGBUTT_DIE,
    ASSET_35_ANIM_BIGBUTT_ALERT,
    ASSET_36_ANIM_BIGBUTT_WALK,
    ANIM_BANJO_FLY = 0x38,
    ANIM_BANJO_LONGLEG_JUMP = 0x3D,
    ANIM_BANJO_LONGLEG_ENTER_AS_BIRD = 0x40,
    ANIM_BANJO_LONGLEG_IDLE = 0x41,
    ANIM_BANJO_LONGLEG_WALK = 0x42,
    ANIM_BANJO_FLY_ENTER = 0x45,
    ANIM_BANJO_BSHOCK_CHARGE = 0x48,
    ANIM_BANJO_BSHOCK_JUMP = 0x49,
    ANIM_BANJO_BFLIP = 0x4B,
    ANIM_BANJO_OW = 0x4D,
    anim_mudhut_smashing = 0x4E,
    ANIM_BANJO_ROLL = 0x4F,
    anim_conga_idle = 0x51,
    anim_conga_ow = 0x52,
    ASSET_53_ANIM_CONGA_DEFEAT = 0x53,
    ASSET_54_ANIM_CONGA_THROW,
    ASSET_55_ANIM_CONGA_BEAT_CHEST = 0x55,
    anim_conga_raise_arms = 0x56,
    ANIM_BANJO_SLIDE_BACK = 0x59,
    ANIM_BANJO_SLIDE_FRONT = 0x5A,
    ANIM_TERMITE_IDLE = 0x5E,
    ANIM_TERMITE_WALK = 0x5F,
    ANIM_TERMITE_JUMP = 0x60,
    ASSET_65_ANIM_BEEHIVE_DIE = 0x65,
    ANIM_GRUBLIN_DIE = 0x94,
    ASSET_D2_ANIM_BANJO_GETTING_UP = 0xD2,
    ASSET_108_ANIM_SIR_SLUSH_IDLE = 0x108,
    ASSET_109_ANIM_SIR_SLUSH_ATTACK,
    ANIM_BANJO_CROUCH = 0x10C,
    ANIM_BANJO_CROUCH_NOINPUT = 0x116,
    ASSET_138_ANIM_ZOOMBOX = 0x138,
    ASSET_139_ANIM_BOTTLES_EXIT,
    ASSET_13A_ANIM_BOTTLES_ENTER,
    ASSET_13B_ANIM_BOTTLES_IDLE,
    ASSET_165_ANIM_BEEHIVE_IDLE = 0x165,
    ASSET_1DC_ANIM_BEE_FLY = 0x1dc,
    ASSET_1DD_ANIM_BEE_WALK,
    ASSET_1DE_ANIM_BEE_IDLE,
    ASSET_1E0_ANIM_BEE_OW = 0x1e0,
    ASSET_1E1_ANIM_BEE_DIE,
    ASSET_1E2_ANIM_BEE_JUMP,
    ASSET_220_ANIM_SIR_SLUSH_DIE = 0x220,
    ASSET_233_ANIM_ICECUBE = 0x233,
    ASSET_253_ANIM_BIGBUTT_OW = 0x253,
    ASSET_254_ANIM_BIGBUTT_FALL,
    ASSET_255_ANIM_BIGBUTT_GET_UP,
    model_orange = 0x2d2,
    model_juju = 0x2e6,
    model_orange_pad = 0x2eb,
    ASSET_34F_MODEL_BANJO_TERMITE = 0x34F,
    ASSET_353_MODEL_BIGBUTT = 0x353,
    ASSET_354_MODEL_BULL_INTRO,
    ASSET_356_MODEL_BANJO_WISHYWASHY = 0x356,
    ASSET_359_MODEL_BANJO_WALRUS = 0x359,
    model_conga = 0x35c,
    model_chimpy = 0x35d,
    ASSET_35F_MODEL_JIGGY = 0x35F,
    ASSET_362_MODEL_BANJO_BEE = 0x362,
    ASSET_364_MODEL_BEEHIVE = 0x364,
    ASSET_36F_MODEL_BANJO_PUMPKIN = 0x36f,
    ASSET_374_MODEL_BANJO_CROC = 0x374,
    ASSET_377_MODEL_SIR_SLUSH = 0x377,
    ASSET_378_MODEL_SNOWBALL,
    ASSET_387_MODEL_BOTTLES = 0x387,
    ASSET_388_MODEL_MOLEHILL,
    model_grublin = 0x3c5,
    model_chimpy_stump = 0x3c8,
    ASSET_430_MODEL_GRILL_CHOMPA = 0x430,
    ASSET_47B_MODEL_ROCK = 0x47b,
    MODEL_TOPPER = 0x4ed,
    MODEL_COLLYWOBBLE = 0x4ee,
    MODEL_BAWL = 0x4ef,
    ASSET_504_ICECUBE = 0x504,
    ASSET_505_ICECUBE_CHUNK = 0x505,
    ASSET_526_SPRITE_FIRE = 0x526,
    SPRITE_DIALOG_FONT_ALPHAMASK = 0x6eb,
    SPRITE_BOLD_FONT_LETTERS_ALPHAMASK = 0x6ec,
    SPRITE_BOLD_FONT_NUMBERS_ALPHAMASK = 0x6ed,
    ASSET_700_SPRITE_DUST = 0x700,
    ASSET_701_SPRITE_LENS_SHUTTER_MASK,
    ASSET_702_SPRITE_UNKNOWN,
    ASSET_70B_SPRITE_BUBBLE = 0x70b,
    ASSET_70C_SPRITE_RIPPLE = 0x70c,
    ASSET_70D_SPRITE_SMOKE = 0x70d,
    ASSET_713_SPRITE_SPARKLE_YELLOW = 0x713,
    model_MM_hut = 0x7d7,
    model_mudhut_top = 0x7d8,
    ASSET_8A0_SPRITE_WATER_DROP = 0x8a0,
    ASSET_B37_TEXT_CONGA_SAFE_UP_HERE = 0xb37,
    ASSET_B38_TEXT_CONGA_DEFEAT = 0xb38,
    ASSET_B39_TEXT_CONGA_HIT_BY_EGG = 0xb39,
    ASSET_B3A_TEXT_CONGA_HITS_PLAYER = 0xb3a,
    ASSET_B3B_TEXT_CONGA_ORANGE_PAD_JIGGY = 0xb3b,
    ASSET_B3C_TEXT_CONGA_MEET = 0xb3c,
    ASSET_B3E_TEXT_CONGA_MEET_AS_TERMITE = 0xb3e,
    ASSET_B44_TEXT_JUJU_MEET = 0xb44,
    ASSET_B45_TEXT_JIGGY_COLLECT_10 = 0xb45,
    ASSET_B51_TEXT_BOTTLES_HOW_TO_EXIT_LEVEL = 0xb51,
    text_flibbits_meet = 0xc81,
    text_flibbits_defeat = 0xc82,
    text_flibbits_return = 0xc83,
    ASSET_D96_TEXT_BEEHIVE = 0xd96,
    ASSET_DA6_TEXT_BEEHIVE_WITH_BEES = 0xda6
};
enum overlay_e{
    OVERLAY_1_COSHOW = 0x1,
    OVERLAY_2_WHALE = 0x2,
    OVERLAY_3_HAUNTED = 0x3,
    OVERLAY_4_DESERT = 0x4,
    OVERLAY_5_BEACH = 0x5,
    OVERLAY_6_JUNGLE = 0x6,
    OVERLAY_7_SWAMP = 0x7,
    OVERLAY_8_SHIP = 0x8,
    OVERLAY_9_SNOW = 0x9,
    OVERLAY_A_TREE = 0xA,
    OVERLAY_B_TRAINING = 0xB,
    OVERLAY_C_INTRO = 0xC,
    OVERLAY_D_WITCH = 0xD,
    OVERLAY_E_BATTLE = 0xE
};
typedef struct {
    u8 pad0[0];
} BKModel;
typedef int unk_type;
typedef struct{
    f32 x;
    f32 y;
    f32 z;
} vec3f;
typedef struct variable_length_array{
    s32 elem_size;
    void * begin;
    void * end;
    void * mem_end;
    u8 data[];
}VLA;
typedef struct bk_sprite_s{
    s16 frameCnt;
    s16 type;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC[4];
} BKSprite;
typedef struct{
    Gfx *gfx;
    Vtx *vtx;
}BKSpriteFrameDisplayData;
typedef struct{
    BKSprite *sprite;
    BKSpriteFrameDisplayData frame[];
}BKSpriteDisplayData;
typedef struct bk_sprite_frame_s{
    s16 unk0;
    s16 unk2;
    s16 w;
    s16 h;
    s16 chunkCnt;
    s16 unkA;
    u8 padC[8];
} BKSpriteFrame;
typedef struct bk_sprite_texture_block_s{
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} BKSpriteTextureBlock;
typedef struct model_cache_s{
    void * modelPtr;
    s32 unk4;
    s32 unk8;
    u32 unkC;
    u32 unk10;
} ModelCache;
typedef struct portrait_voice_s{
    u16 sfxIndex;
    u8 pad2[2];
    f32 duration;
} PortraitVoice;
typedef struct portrait_info_s{
    u16 assetIndx;
    u8 pad2[2];
    PortraitVoice voiceInfo[5];
} PortraitInfo;
typedef struct struct_0_s{
    void * model;
    u16 vtx1;
    u16 vtx2;
    u16 vtx3;
    u8 padA[0x6];
    s16 unk10[6];
    f32 unk1C[3];
    f32 unk28[3];
    f32 normX;
    f32 normY;
    f32 normZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    union{
        struct{
            u32 unk4C_0:32;
        };
        u32 unk4C;
    };
    s16 unk50;
    s16 unk52;
    u32 unk54;
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 unk5B;
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 unk5F;
} struct0;
typedef struct struct_1_s{
    u32 unk0;
    u8 pad4[4];
    u32 unk8;
    u8 padC[4];
    u32 unk10;
    u8 pad14[4];
    u32 unk18;
    u8 pad1C[4];
} struct1;
typedef struct struct_2_s{
    f32 unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    u32 unk8;
    s32 unkC;
    s32 unk10;
    void *unk14;
} struct2s;
typedef struct struct_3_s{
    u32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[0x3];
    VLA *unk20;
    s32 unk24;
    s32 unk28;
    void *unk2C;
    f32 unk30;
    u8 unk34;
    u8 pad35[0x1B];
} struct3s;
typedef struct struct_4_s{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 pad19[3];
} struct4s;
typedef struct struct_5_s{
    s32 unk0;
    f32 unk4[3];
    f32 unk10[3];
    f32 unk1C[3];
    f32 unk28[3];
    u8 unk34;
    u8 pad35[0x3];
}struct5s;
typedef struct struct_6_s{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u32 unk18;
    VLA *unk1C;
    s16 unk20;
    u8 unk22;
    u8 pad23[1];
    void *unk24[4];
    u8 unk34;
    u8 pad35[0x3];
    f32 unk38;
}struct6s;
typedef struct struct_7_s{
    u32 unk0;
}struct7s;
typedef struct struct_8_s{
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 unk14;
    s32 unk18;
    f32 unk1C;
    u32 unk20;
    s32 unk24;
    u32 unk28;
    s32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[4];
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    u32 unk50;
    s8 unk54[0xC];
    f32 unk60;
}struct8s;
typedef struct struct_9_s{
    u8 uid;
    u8 unk1;
    u8 unk2;
    u8 pad3[1];
    f32 unk4;
    s32 unk8;
    s32 unkC;
    f32 unk10;
}struct9s;
typedef struct struct_10_s{
    u8 map_indx;
    u8 unk1;
    u8 unk2;
}struct10s;
typedef struct struct_11_s{
    f32 unk0;
    f32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    u8 unk14;
    u8 unk15;
    u8 pad16[0x2];
    s32 unk18;
    s32 unk1C[0xE];
} CoMusic;
typedef struct struct_12_s{
    s32 unk0;
    s32 unk1;
} struct12s;
typedef struct struct_13_s{
    s32 cmd;
    u8* str;
}struct13s;
typedef struct struct_14_s{
    s16 unk0;
    s16 unk2;
    union{ struct{ f32 unk4_x; f32 unk4_y; f32 unk4_z; }; struct{ f32 unk4_pitch; f32 unk4_yaw; f32 unk4_roll; }; f32 unk4[3];};
    struct actorMarker_s *unk10;
    s32 unk14;
    void (*unk18)(struct actorMarker_s *, s32, s32);
    void (*unk1C)(struct actorMarker_s *, s32, s32);
    s32 unk20;
}struct14s;
typedef struct struct_15_s{
    u8 unk0_7:2;
    u8 unk0_5:2;
    u8 pad0_3:4;
}struct15s;
typedef struct struct_16_s{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
}struct16s;
typedef struct struct_18_s{
    s16 uid;
    s16 unk2;
    f32 unk4;
}struct18s;
typedef struct struct_17_s{
    s16 uid;
    s8 unk2;
    s8 unk3;
    struct18s soundInfo[5];
}struct17s;
typedef struct struct_1A_s{
    u8 pad0[0x8];
    u8 *str;
    s16 unkC;
    u8 unkE;
    u8 unkF;
}struct1As;
typedef struct struct_1B_s{
    s16 map;
    s16 exit;
}struct1Bs;
typedef struct struct_1C_s{
    s16 unk0;
    s16 x;
    u8 *string;
}struct1Cs;
typedef struct struct_1D_s{
    struct struct_1F_s **unk0;
    s16 unk4;
    u8 unk6;
    u8 pad7[1];
    u8 pad8[0xA0];
}struct1Ds;
typedef struct struct_1E_s{
    void (* unk0)(void * arg0, s32 arg1, s32 arg2, s32 arg3);
    void (* unk4)(void * arg0, s32 arg1, s32 arg2);
    void (* unk8)(void * arg0);
}struct1Es;
typedef struct struct_20_s{
    s16 uid;
    s16 count;
    s16 data[];
}struct20s;
typedef struct struct_1F_s{
    s16 count;
    struct20s unk2[];
}struct1Fs;
typedef struct struct_21_s{
    s32 unk0;
    void * unk1;
}struct21s;
typedef struct struct_22_s{
    f32 unk0[3];
    u8 padC[0xC];
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x8];
    s32 unk28[3];
    u8 unk34;
    u8 pad35[0x3];
} struct22s;
typedef struct struct_2F_s{
    f32 unk0[3];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 position_18[3];
    f32 unk24[3];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C[3];
    f32 unk48;
    f32 lifetime_4C;
    f32 unk50[3];
    u8 unk5C;
    u8 pad5D[3];
} Particle;
typedef struct struct_30_s{
    u32 pad0_31:8;
    u32 doneSpawning_0_23:7;
    u32 unk0_16:1;
    u32 assetId_0_15:14;
    u32 unk0_1:1;
    u32 unk0_0:1;
    f32 unk4[3];
    f32 unk10;
    f32 unk14;
    s32 unk18;
    Sprite *sprite_1C;
    void *unk20;
    f32 particleSpawnTimer_24;
    f32 postion_28[3];
    s32 unk34;
    f32 spawnIntervalTimer_38;
    s32 unk3C[3];
    u8 unk48;
    u8 unk49;
    u8 pad4A[0x2];
    f32 unk4C[3];
    f32 unk58[3];
    s16 unk64;
    s16 unk66;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    s32 unk7C;
    void (*particleCallback_80)(struct struct_30_s *, f32 (*)[3]);
    s32 unk84;
    s32 unk88;
    f32 unk8C;
    f32 unk90;
    f32 unk94[3];
    f32 unkA0[3];
    f32 unkAC[2];
    f32 unkB4[2];
    f32 unkBC[3];
    f32 unkC8[3];
    f32 unkD4;
    f32 unkD8;
    f32 unkDC[2];
    f32 unkE4[2];
    f32 unkEC[2];
    f32 unkF4[2];
    f32 unkFC;
    s32 unk100;
    s16 unk104;
    u8 pad106[0x2];
    f32 unk108;
    f32 unk10C[3];
    f32 unk118[3];
    Particle *pList_start_124;
    Particle *pList_end_128;
    Particle *pList_capacity_12C;
    Particle data[];
} ParticleCtrl;
typedef struct struct_31_s{
    f32 unk0[2];
    f32 unk8[2];
    f32 unk10[2];
    f32 unk18[2];
    f32 unk20;
    f32 unk24;
} struct31s;
typedef struct struct_32_s{
    f32 unk0[2];
    f32 unk8[2];
    f32 unk10[2];
} struct32s;
typedef struct struct_33_s{
    f32 unk0[3];
    f32 unkC[3];
} struct33s;
typedef struct struct_34_s{
    f32 unk0[3];
    f32 unkC[3];
} struct34s;
typedef struct struct_40_s{
    struct31s unk0;
    f32 unk28;
    f32 unk2C;
} struct40s;
typedef struct struct_41_s{
    struct32s unk0;
    struct33s unk18;
} struct41s;
typedef struct struct_42_s{
    struct32s unk0;
    struct34s unk18;
} struct42s;
typedef struct struct_43_s{
    struct32s unk0;
    struct33s unk18;
    struct34s unk30;
} struct43s;
typedef struct {
    u32 unk0_31:11;
    u32 unk0_20:10;
    u32 unk0_10:11;
}struct44s;
typedef struct {
    s32 unk0;
    s32 unk4;
}struct46s;
typedef struct {
    s16 unk0;
    u8 pad2[0x2];
    f32 unk4;
    f32 unk8;
}struct47s;
typedef struct {
    s16 map;
    u8 pad2[2];
    struct47s unk4[3];
}struct48s;
typedef struct{
    f32 unk0[3];
    f32 unkC[3];
    u8 unk18;
    u8 pad19[3];
    struct struct_4D_s *unk1C;
}struct4Cs;
typedef struct struct_4D_s{
    u8 unk0[0x18];
}struct4Ds;
typedef struct{
    u8 unk0;
    u8 pad1[0x3];
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s16 unk38;
    s16 unk3A;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
}struct4Es;
typedef struct{
    u8 unk0;
    u8 unk1;
    f32 unk4[3];
}struct50s;
typedef struct{
    s16 unk0;
    u8 pad2[4];
    s16 unk6;
    u8 pad8[4];
}struct51s;
typedef struct{
    s32 unk0;
    u8 pad4[8];
}struct52s;
typedef struct{
    u8 unk0;
    u8 unk1;
} struct53s;
typedef struct{
    struct53s *unk0;
    f32 unk4;
    f32 unk8;
    u32 unkC_31:7;
    u32 unkC_24:7;
    u32 unkC_17:7;
    u32 unkC_10:7;
    u32 unkC_3:2;
    u32 unkC_1:1;
    u32 unkC_0:1;
} struct54s;
typedef struct{
    u8 pad0[0xC];
}struct55s;
typedef struct{
    s32 unk0;
    u8 pad4[4];
    f32 unk8[0][3];
} struct56s;
extern u8 D_80378F60;
extern u8 D_80378F64;
extern u8 D_80378F68;
extern u8 D_80378F6C;
extern u8 D_80378F70;
void strcat(u8 * dst, u8 *src);
void strcatc(u8 * dst, u8 src);
void strFToA(u8* dst, f32 val);
void _strFToA(u8* dst, f32 val, s32 radix);
void strIToA(u8 *str, s32 num);
void _strIToA(u8 *str, s32 num, s8 base);
void strcpy(u8 *dst, u8 *src);
s32 strlen(u8 * str);
void strToUpper(u8 *str);
f32 randf (void);
f32 randf2(f32 min, f32 max);
s32 randi2(s32 min, s32 max);
enum animctrl_playback_e{
    ANIMCTRL_ONCE = 1,
    ANIMCTRL_LOOP = 2,
    ANIMCTRL_STOPPED = 3,
    ANIMCTRL_SUBRANGE_LOOP = 4
};
typedef struct animation_s{
    void (* matrices)(s32, s32);
    s32 unk4;
    u8 unk8;
    u8 unk9;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u32 index;
    f32 timer;
    f32 duration;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
} Animation;
typedef struct animctrl_s{
    Animation *animation;
    f32 timer;
    f32 subrange_start;
    f32 subrange_end;
    f32 animation_duration;
    f32 transition_duration;
    float unk18;
    s32 index;
    u8 playback_type;
    u8 playback_direction;
    u8 smooth_transition;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
} AnimCtrl;
typedef struct actorAnimCtrl_s{
    AnimCtrl animctrl;
    Animation animation;
} ActorAnimCtrl;
AnimCtrl *animctrl_new(s32 arg0);
void animctrl_free(AnimCtrl * this);
void animctrl_update(AnimCtrl *this);
AnimCtrl *func_80287434(AnimCtrl *this);
void animctrl_setIndex(AnimCtrl *this, enum asset_e index);
Animation *animctrl_getAnimPtr(AnimCtrl *this);
void func_8028746C(AnimCtrl *this, void (* arg1)(s32,s32));
void func_8028748C(AnimCtrl *this, s32 arg1);
void animctrl_reset(AnimCtrl *this);
void func_8028752C(AnimCtrl *this);
void func_802875AC(AnimCtrl * this, char *file, s32 line);
void func_8028764C(AnimCtrl *this, f32 timer);
void animctrl_setPlaybackType(AnimCtrl *this, enum animctrl_playback_e arg1);
void animctrl_setDirection(AnimCtrl *this, s32 arg1);
void animctrl_setSmoothTransition(AnimCtrl *this, s32 arg1);
void animctrl_setDuration(AnimCtrl *this, f32 arg1);
void animctrl_setTransitionDuration(AnimCtrl *this, f32 arg1);
void animctrl_setSubRange(AnimCtrl *this, f32 start, f32 end);
void animctrl_getSubRange(AnimCtrl *this, f32 *startPtr, f32 *endPtr);
void func_8028774C(AnimCtrl *this, f32 arg1);
void func_80287784(AnimCtrl *this, s32 arg1);
enum asset_e animctrl_getIndex(AnimCtrl *this);
enum animctrl_playback_e animctrl_getPlaybackType(AnimCtrl *this);
s32 animctrl_isPlayedForwards(AnimCtrl *this);
s32 animctrl_isSmoothTransistion(AnimCtrl *this);
f32 animctrl_getDuration(AnimCtrl *this);
f32 animctrl_getTransistionDuration(AnimCtrl *this);
f32 func_802877D8(AnimCtrl *this);
f32 func_802877F8(AnimCtrl *this);
void func_80287800(AnimCtrl *this, f32 arg1);
s32 func_8028780C(AnimCtrl *this, s32 arg1);
s32 func_8028781C(AnimCtrl *this, f32 *arg1, s32 arg2);
s32 animctrl_isStopped(AnimCtrl *this);
int animctrl_isAt(AnimCtrl *this, f32 arg1);
s32 animctrl_isContiguous(AnimCtrl *this);
typedef struct sprite_prop_s{
    u32 unk0_31:0xC;
    u32 pad0_19:0xA;
    u32 unk0_9:0x8;
    u32 pad0_1:0x2;
    u8 pad4[0x4];
    u32 pad8_31: 27;
    u32 unk8_4: 1;
    u32 pad8_3: 2;
    u32 unk8_1:1;
    u32 unk8_0:1;
} SpriteProp;
typedef struct prop_prop_s{
    u32 unk0_31:0xC;
    u32 pad0_19:0x14;
    u8 pad4[0x6];
    u8 unkA;
    u8 padB[1];
} PropProp;
typedef struct actor_prop_s{
    struct actorMarker_s* marker;
    s16 x;
    s16 y;
    s16 z;
    u16 pad8_15:11;
    u16 unk8_4:1;
    u16 unk8_3:1;
    u16 unk8_2:1;
    u16 unk8_1:1;
    u16 unk8_0:1;
} ActorProp;
typedef void(*MarkerCollisionFunc)(struct actorMarker_s *this, struct actorMarker_s *other);
typedef struct actorMarker_s{
    ActorProp* propPtr;
    struct cude_s* cubePtr;
    s32 unk8;
    MarkerCollisionFunc unkC;
    MarkerCollisionFunc unk10;
    u32 yaw:9;
    u32 pad14_22:1;
    u32 unk14_21:1;
    u32 unk14_20:10;
    u32 unk14_10:11;
    u32 unk18;
    MarkerCollisionFunc unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u32 actrArrayIdx:11;
    u32 pitch:9;
    u32 roll:9;
    u32 unk2C_2:1;
    u32 unk2C_1:1;
    u32 collidable:1;
    void (*unk30)(struct actor_s *);
    s32 unk34;
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    u16 pad3E_15:1;
    u16 modelId:13;
    u16 unk3E_1:1;
    u16 unk3E_0:1;
    u32 unk40_31:4;
    u32 pad40_27:1;
    u32 unk40_26:3;
    u32 unk40_23:1;
    u32 unk40_22:1;
    u32 unk40_21:1;
    u32 unk40_20:1;
    u32 unk40_19:1;
    u32 pad40_18:19;
    u32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
} ActorMarker;
typedef struct chmrvile_s{
    u8 pad0[0x4];
    u32 unk4;
    void *unk8;
    u8 unkC;
    u8 padD[0x7];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} ActorLocal_MrVile;
typedef struct chpinkegg_s{
    u32 unk0;
    u32 unk4;
} ActorLocal_PinkEgg;
typedef struct chyumblie_s{
    f32 unk0;
    u8 unk4;
    u8 pad7[3];
    f32 unk8;
    u32 unkC;
} ActorLocal_Yumblie;
typedef struct ch_bgs_2270_s{
    u32 unk0;
    VLA *unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    f32 unkC;
} ActorLocal_BGS_2270;
typedef struct ch_bgs_6730_s{
    u32 unk0;
    u32 unk4;
    s32 unk8;
    s32 unkC;
} ActorLocal_BGS_6730;
typedef struct chflibbit_s{
    u8 pad0;
    u8 unk1;
    s16 unk2[3];
    s16 unk8[3];
}ActorLocal_Flibbit;
typedef struct chtanktupbody_s{
    s32 unk0[4];
    s32 unk10;
}ActorLocal_TanktupBody;
typedef struct chleafboat_s{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
} ActorLocal_Leafboat;
typedef struct actor_anim_info_s{
    u32 index;
    f32 duration;
} ActorAnimationInfo;
typedef struct chgrublin_s{
    f32 unk0;
    f32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u32 unkC_31:3;
    u32 unkC_28:1;
    u32 padC_27:28;
    s16 unk10;
    s16 unk12;
    f32 unk14;
    u8 unk18[0x18];
    s32 unk30;
    void (*unk34)(ActorMarker *, s32);
}ActorLocal_Grublin;
typedef struct chconga_s{
    union{ struct{ s32 orangeSpawnPosition_x; s32 orangeSpawnPosition_y; s32 orangeSpawnPosition_z; }; struct{ s32 orangeSpawnPosition_pitch; s32 orangeSpawnPosition_yaw; s32 orangeSpawnPosition_roll; }; s32 orangeSpawnPosition[3];};
    s32 unkC;
    s32 unk10;
    u8 pad14[0x4];
    s32 unk18;
    s32 unk1C;
}ActorLocal_Conga;
typedef struct jinjo_s{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
}ActorLocal_Jinjo;
typedef struct juju_hitbox_s{
    u8 pad0[0x4];
    s32 unk4;
    ActorMarker *unk8[4];
    f32 unk18;
}ActorLocal_JujuHitbox;
typedef struct juju_s{
    s32 unk0;
    s32 unk4;
    union{ struct{ f32 unk8_x; f32 unk8_y; f32 unk8_z; }; struct{ f32 unk8_pitch; f32 unk8_yaw; f32 unk8_roll; }; f32 unk8[3];};
    f32 unk14;
    s32 unk18;
}ActorLocal_Juju;
typedef struct ch_fight_180_s{
    u8 unk0;
    u8 pad1;
    u8 pad2;
    u8 pad3;
    u8 pad4;
    u8 pad5;
    u8 pad6;
    u8 pad7;
    u8 pad8;
    u8 pad9;
    u8 padA;
    u8 padB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
}ActorLocal_fight_180;
typedef struct ch_sm_4070{
    s32 unk0;
}ActorLocal_SM_4070;
typedef struct actor_s{
    ActorMarker* marker;
    union{ struct{ f32 position_x; f32 position_y; f32 position_z; }; struct{ f32 position_pitch; f32 position_yaw; f32 position_roll; }; f32 position[3];};
    u32 state:6;
    u32 unk10_25:7;
    u32 unk10_18:6;
    u32 unk10_12:4;
    u32 unk10_8:1;
    u32 unk10_7:1;
    u32 unk10_6:2;
    u32 unk10_4:1;
    u32 unk10_3:2;
    u32 unk10_1:1;
    u32 unk10_0:1;
    AnimCtrl *animctrl;
    ActorAnimationInfo *unk18;
    union{ struct{ f32 unk1C_x; f32 unk1C_y; f32 unk1C_z; }; struct{ f32 unk1C_pitch; f32 unk1C_yaw; f32 unk1C_roll; }; f32 unk1C[3];};
    f32 unk28;
    union{ struct{ f32 velocity_x; f32 velocity_y; f32 velocity_z; }; struct{ f32 velocity_pitch; f32 velocity_yaw; f32 velocity_roll; }; f32 velocity[3];};
    u32 unk38_31:10;
    u32 unk38_21:9;
    u32 unk38_13:9;
    u32 stored_animctrl_playbackType_:3;
    u32 unk38_0:1;
    u32 unk3C;
    s32 unk40;
    u32 unk44_31:8;
    u32 modelCacheIndex:10;
    s32 unk44_14:10;
    u32 despawn_flag:1;
    u32 unk44_2:1;
    u32 unk44_1:1;
    u32 unk44_0:1;
    f32 unk48;
    f32 unk4C;
               f32 yaw;
    f32 unk54;
    u32 unk58_31: 15;
    u32 stored_animctrl_index: 14;
    u32 unk58_2: 1;
    u32 unk58_1: 1;
    u32 unk58_0: 1;
    f32 unk5C;
    f32 unk60;
    f32 yaw_moving;
    f32 pitch;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    u32 unk78_31:9;
    u32 unk78_22:9;
    u32 unk78_13:12;
    u32 stored_animctrl_forwards:1;
    u32 stored_animctrl_smoothTransistion:1;
    union
    {
        ActorLocal_MrVile mrVile;
        ActorLocal_PinkEgg pinkEgg;
        ActorLocal_Yumblie yumblie;
        ActorLocal_Leafboat leafboat;
        ActorLocal_BGS_2270 bgs_2270;
        ActorLocal_BGS_6730 bgs_6730;
        ActorLocal_Flibbit flibbit;
        ActorLocal_TanktupBody tanktup;
        ActorLocal_Grublin grublin;
        ActorLocal_Conga conga;
        ActorLocal_Jinjo jinjo;
        ActorLocal_JujuHitbox juju_ctl;
        ActorLocal_Juju juju;
        ActorLocal_fight_180 fight_180;
        ActorLocal_SM_4070 sm_4070;
        u8 local[1];
        struct{
            u8 unk7C[0x40];
            u8 unkBC[0x30];
        };
    };
    f32 unkEC;
    f32 stored_animctrl_duration;
    u32 unkF4_31:1;
    u32 unkF4_30:1;
    u32 unkF4_29:1;
    u32 unkF4_28:1;
    u32 unkF4_27:1;
    u32 unkF4_26:1;
    u32 stored_marker_collidable:1;
    u32 unkF4_24:2;
    u32 unkF4_22:1;
    u32 initialized:1;
    u32 unkF4_20:12;
    u32 unkF4_8:9;
    f32 stored_animctrl_subrangeMin;
    f32 stored_animctrl_subrangeMax;
    ActorMarker *unk100;
    ActorMarker *unk104;
    void ( *unk108)(struct actorMarker_s *, s32);
    s32 unk10C;
    f32 roll;
    f32 sound_timer;
    union{ struct{ f32 spawn_position_x; f32 spawn_position_y; f32 spawn_position_z; }; struct{ f32 spawn_position_pitch; f32 spawn_position_yaw; f32 spawn_position_roll; }; f32 spawn_position[3];};
    u32 unk124_31:12;
    u32 unk124_19:8;
    u32 unk124_11:2;
    u32 unk124_9:2;
    u32 unk124_7:1;
    u32 unk124_6:1;
    u32 unk124_5:2;
    u32 unk124_3:3;
    u32 unk124_0:1;
    f32 scale;
                struct actor_info *actor_info;
    void * unk130;
    VLA **unk134;
    u32 unk138_31:1;
    u32 unk138_30:1;
    u32 unk138_29:1;
    u32 unk138_28:1;
    u32 unk138_27:2;
    u32 unk138_25:1;
    u32 unk138_24:1;
    u32 unk138_23:1;
    u32 unk138_22:1;
    u32 unk138_21:1;
    u32 unk138_20:1;
    u32 unk138_19:10;
    u32 unk138_9:1;
    u32 unk138_8:1;
    u32 unk138_7:4;
    u32 unk138_3:4;
    void (*unk13C)(struct actorMarker_s *);
    f32 unk140;
    f32 unk144;
    void *unk148;
    void *unk14C;
    void *unk150;
    u32 unk154;
    void *unk158;
    void *unk15C;
    s32 unk160;
    s8 unk164[0x2];
    u8 unk166;
    s8 pad167[0x1];
    s32 unk168;
    u32 unk16C_31:27;
    u32 unk16C_4:1;
    u32 unk16C_3:1;
    u32 unk16C_2:1;
    u32 unk16C_1:1;
    u32 unk16C_0:1;
    f32 unk170;
    f32 unk174;
    f32 unk178;
    u32 unk17C_31:1;
    u32 pad17C_30:31;
} Actor;
typedef struct actor_info_s{
    s16 unk0;
    s16 actorId;
    s16 modelId;
    s16 startAnimation;
    ActorAnimationInfo* animations;
    void (* update_func)(Actor *);
    void (* unk10)(Actor *);
    Actor* (* draw_func)(ActorMarker *, Gfx **, Mtx**, s32);
    u8 pad18[4];
    f32 shadow_scale;
    u8 pad20[4];
} ActorInfo;
typedef struct actor_spawn_s{
    ActorInfo *infoPtr;
    Actor *(*spawnFunc)(s32[3], s32, ActorInfo*, u32);
    s32 unk8;
} ActorSpawn;
typedef union prop_s
{
    ActorProp actorProp;
    SpriteProp spriteProp;
    PropProp propProp;
    struct{
        u8 pad0[6];
        s16 unk6;
        s32 pad8_31: 29;
        u32 unk8_2: 1;
        u32 unk8_1: 1;
        u32 markerFlag: 1;
    };
} Prop;
typedef struct cude_s{
    u32 x:5;
    u32 y:5;
    u32 z:5;
    u32 prop1Cnt:6;
    u32 prop2Cnt:6;
    u32 pad0:5;
    Prop *prop1Ptr;
    Prop *prop2Ptr;
}Cube;
typedef struct actor_array{
    s32 cnt;
    s32 max_cnt;
    Actor data[];
}ActorArray;
void memcpy(void * dst, void *src, int size);
void func_80254630(void * dst, void *src, int size);
typedef void (* TFQM0)(void);
typedef void (* TFQM1)(s32);
typedef void (* TFQM2)(s32, s32);
typedef void (* TFQM3)(s32, s32, s32);
typedef void (* TFQM4)(s32, s32, s32, s32);
typedef void (* TFQM5)(s32, s32, s32, s32, s32);
typedef void (* TFQM6)(void *);
void timedFunc_set_0(f32 time, TFQM0 funcPtr);
void timedFunc_set_1(f32 time, TFQM1 funcPtr, s32 arg0);
void timedFunc_set_2(f32 time, TFQM2 funcPtr, s32 arg0, s32 arg1);
void timedFunc_set_3(f32 time, TFQM3 funcPtr, s32 arg0, s32 arg1, s32 arg2);
void timedFunc_set_4(f32 time, TFQM4 funcPtr, s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void timedFunc_set_5(f32 time, TFQM5 funcPtr, s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void timedFunc_set_6(f32 time, TFQM6 funcPtr, void* argPtr );
void timedJiggySpawn(f32 time, s32 jiggyId, f32 *position);
void gcbound_draw(Gfx **dl);
void gcbound_alpha(s32 a);
void gcbound_color(u8 r, u8 g, u8 b);
void gcbound_reset(void);
void gctransition_8030B740(void);
void gctransition_draw(Gfx **arg0, Mtx **arg1, Vtx **arg2);
void gctransition_8030BD4C(void);
f32 gctransition_8030BD88(void);
int gctransition_8030BD98(void);
int gctransition_8030BDAC(void);
int gctransition_8030BDC0(void);
void gctransition_8030BE3C(void);
void gctransition_8030BE60(void);
void gctransition_8030BEA4(s32 arg0);
void gctransition_update(void);
void gctransition_reset(void);
typedef struct {
    u8 unk0[0x30];
    u8 unk30[0x30];
    u8 unk60[0x30];
    u8 unk90[0x20];
    u8 unkB0[0x40];
    void *unkF0;
    AnimCtrl *unkF4;
    BKSprite *unkF8;
    BKSpriteDisplayData *unkFC;
    BKSprite *unk100;
    BKSpriteDisplayData *unk104;
    u8 unk108[5];
    u8 pad10D[3];
    f32 unk110[5];
    s16 unk124[5];
    s16 unk12E;
    void (*unk130)(s32, s32);
    u8 unk134;
    u8 unk135;
    u8 portrait_id;
    u8 unk137;
    u8 unk138;
    u8 unk139;
    u8 unk13A;
    u8 unk13B;
    s32 unk13C[8];
    u8 unk15C;
    u8 unk15D;
    u8 unk15E;
    u8 pad15F[0x1];
    s32 unk160;
    s16 unk164;
    s16 unk166;
    s16 unk168;
    s16 unk16A;
    s16 unk16C;
    s16 unk16E;
    s16 unk170;
    s16 unk172;
    s16 unk174;
    s8 unk176;
    s8 unk177;
    s8 unk178;
    s8 unk179;
    u8 pad17A[0x2];
    f32 unk17C;
    u8 unk180;
    s8 unk181;
    u8 unk182;
    u8 unk183;
    u8 unk184;
    u8 unk185;
    s8 unk186;
    u8 unk187;
    u8 unk188;
    u8 unk189;
    u8 unk18A;
    u8 unk18B;
    f32 unk18C;
    f32 unk190;
    f32 unk194;
    f32 unk198;
    f32 unk19C;
    f32 unk1A0;
    u32 unk1A4_31:1;
    u32 unk1A4_30:1;
    u32 unk1A4_29:1;
    u32 unk1A4_28:1;
    u32 unk1A4_27:1;
    u32 unk1A4_26:1;
    u32 unk1A4_25:1;
    u32 unk1A4_24:1;
    u32 unk1A4_23:1;
    u32 unk1A4_22:1;
    u32 unk1A4_21:1;
    u32 unk1A4_20:1;
    u32 unk1A4_19:1;
    u32 unk1A4_18:1;
    u32 unk1A4_17:1;
    u32 unk1A4_16:1;
    u32 unk1A4_15:1;
    u32 unk1A4_14:1;
    u32 unk1A4_13:1;
    u32 unk1A4_12:1;
    u32 unk1A4_11:1;
    u32 unk1A4_10:1;
    u32 pad1A4_9:10;
}gczoombox_t;
void gczoombox_draw(gczoombox_t *this, Gfx **gdl, Mtx ** mptr, void *vptr);
void gczoombox_free(gczoombox_t* this);
gczoombox_t *gczoombox_new(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void (*arg4)(s32, s32));
    void print_bold_overlapping(s32 x, s32 y, f32 arg2, u8* string);
    void print_bold_spaced(s32 x, s32 y, u8* string);
    void print_dialog(s32 x, s32 y, u8* string);
    void print_dialog_w_bg(s32 x, s32 y, u8* string);
    void print_dialog_gradient(s32 x, s32 y, u8* string, u8 arg3, u8 arg4);
    void func_802F79D0(s32 x, s32 y, u8* string, s32 arg3, s32 arg4);
void ml_vec3f_normalize_copy(f32 arg0[3], f32 arg1[3]);
void ml_vec3f_yaw_rotate_copy(f32 arg0[3], f32 arg1[3], f32 arg2);
void ml_vec3f_diff_copy(f32 dst[3], f32 src1[3], f32 src2[3]);
void ml_vec3f_set_length_copy(f32 dst[3], f32 src[3], f32 len);
void ml_vec3f_set_length(f32 arg0[3], f32 arg2);
void mlMtxIdent(void);
void mlMtxRotPitch(f32 pitch);
void mlMtxRotYaw(f32 yaw);
void mlMtxRotRoll(f32 roll);
void mlMtxScale(f32 x, f32 y, f32);
void mlMtxTranslate(f32 x, f32 y, f32 z);
void func_802B5350(void);
void func_80296608(void);
void func_80296590(void);
void bsstand_init(void);
void bsstand_update(void);
void bsstand_end(void);
void bswalk_slow_init(void);
void bswalk_slow_upate(void);
void bswalk_init(void);
void bswalk_update(void);
void bswalk_fast_init(void);
void bswalk_fast_update(void);
void bswalk_fast_end(void);
void bsjump_init(void);
void bsjump_update(void);
void bsjump_end(void);
int bsclaw_hitboxActive(void);
void bsclaw_init(void);
void bsclaw_update(void);
void bsclaw_end(void);
void bscrouch_init(void);
void bscrouch_update(void);
void bscrouch_end(void);
void bsbtrot_jump_init(void);
void bsbtrot_jump_update(void);
void bsbtrot_jump_end(void);
void bsturn_init(void);
void bsturn_update(void);
void bsturn_end(void);
void func_802B63F8(void);
void func_802B64D0(void);
void func_802B6500(void);
void bsow_init(void);
void bsow_update(void);
void bsow_end(void);
void bsbbuster_init(void);
void bsbbuster_update(void);
void bsbbuster_end(void);
void bsbflap_init(void);
void bsbflap_update(void);
void bsbflap_end(void);
void bsbpeck_init(void);
void bsbpeck_update(void);
void bsbpeck_end(void);
void bsbflip_init(void);
void bsbflip_update(void);
void bsbflip_end(void);
void bsbarge_init(void);
void bsbarge_update(void);
void bsbarge_end(void);
void bsbtrot_enter_init(void);
void bsbtrot_enter_update(void);
void bsbtrot_enter_end(void);
void bsbtrot_stand_init(void);
void bsbtrot_stand_update(void);
void bsbtrot_stand_end(void);
void bsbtrot_walk_init(void);
void bsbtrot_walk_update(void);
void bsbtrot_walk_end(void);
void bsbtrot_exit_init(void);
void bsbtrot_exit_update(void);
void bsbtrot_exit_end(void);
void func_802A4D90(void);
void func_802A4EC8(void);
void func_802A4F44(void);
void bsbwhirl_enter_init(void);
void bsbwhirl_enter_update(void);
void bsbwhirl_enter_end(void);
void bsbwhirl_stand_init(void);
void bsbwhirl_stand_update(void);
void bsbwhirl_stand_end(void);
void bsbwhirl_walk_init(void);
void bsbwhirl_walk_update(void);
void bsbwhirl_walk_end(void);
void bsbwhirl_jump_init(void);
void bsbwhirl_jump_update(void);
void bsbwhirl_jump_end(void);
void bsbwhirl_exit_init(void);
void bsbwhirl_exit_update(void);
void bsbwhirl_exit_end(void);
void bsegghead_init(void);
void bsegghead_update(void);
void bsegghead_end(void);
void bseggass_init(void);
void bseggass_update(void);
void bseggass_end(void);
void bswalk_creep_init(void);
void bswalk_creep_update(void);
void bsstand_landing_init(void);
void bsstand_landing_update(void);
void bsbshock_charge_init(void);
void bsbshock_charge_update(void);
void bsbshock_charge_end(void);
void bsbshock_init(void);
void bsbshock_update(void);
void bsbshock_end(void);
void bsbfly_enter_init(void);
void bsbfly_enter_update(void);
void bsbfly_enter_end(void);
void func_802A505C(void);
void bsbfly_init(void);
void bsbfly_update(void);
void func_802A3F70(void);
void bsblongleg_enter_init(void);
void bsblongleg_enter_update(void);
void bsblongleg_enter_end(void);
void bsblongleg_stand_enter(void);
void bsblongleg_stand_update(void);
void bsblongleg_stand_end(void);
void bsblongleg_walk_init(void);
void bsblongleg_walk_update(void);
void bsblongleg_walk_end(void);
void bsblongleg_jump_init(void);
void bsblongleg_jump_update(void);
void bsblongleg_jump_end(void);
void bsblongleg_exit_init(void);
void bsblongleg_exit_update(void);
void bsblongleg_exit_end(void);
void func_802A3F9C(void);
void func_802A411C(void);
void func_802A4404(void);
void func_802A762C(void);
void func_802A7674(void);
void func_802A7718(void);
void func_802A7738(void);
void func_802A7838(void);
void func_802A7A2C(void);
void func_802B5774(void);
void func_802B5950(void);
void func_802B5AF8(void);
void func_802B5B18(void);
void func_802B5C40(void);
void func_802B5E10(void);
void bsjump_fall_init(void);
void bsjump_fall_update(void);
void bsjump_fall_end(void);
void func_802A7DAC(void);
void func_802A7E2C(void);
void func_802A7F4C(void);
int bstwirl_hitboxActive(void);
void bstwirl_init(void);
void bstwirl_update(void);
void bstwirl_end(void);
void bsslide_init(void);
void bsslide_update(void);
void bsslide_end(void);
void func_802B9ACC(void);
void func_802B9B14(void);
void func_802B9AAC(void);
void func_802B9D00(void);
void bsjig_notedoor_init(void);
void bsjig_notedoor_update(void);
void bsjig_notedoor_end(void);
void bsant_idle_init(void);
void bsant_idle_update(void);
void bsant_idle_end(void);
void bsant_walk_init(void);
void bsant_walk_update(void);
void bsant_walk_end(void);
void bsant_jump_init(void);
void bsant_jump_update(void);
void bsant_jump_end(void);
void func_802A7A54(void);
void func_802A7AB0(void);
void func_802A7BA8(void);
void bscarry_idle_init(void);
void bscarry_idle_update(void);
void bscarry_idle_end(void);
void bscarry_interrupt(void);
void bscarry_walk_init(void);
void bscarry_walk_update(void);
void bscarry_walk_end(void);
void func_802B6130(void);
void func_802B61E0(void);
void func_802B6218(void);
void func_802B6220(void);
void bsjump_tumble_init(void);
void bsjump_tumble_update(void);
void bsjump_tumble_end(void);
void bsant_fall_init(void);
void bsant_fall_update(void);
void bsant_fall_end(void);
void bsant_ow_init(void);
void bsant_ow_update(void);
void bsant_ow_end(void);
void func_802B1BF4(void);
void func_802B1CF8(void);
void func_802B1DA4(void);
void func_802B2BF0(void);
void func_802B2C58(void);
void func_802B2D50(void);
void bsdie_init(void);
void bsdie_update(void);
void bsdie_end(void);
void func_802A5120(void);
void func_802A5190(void);
void func_802A51C0(void);
void bsant_die_init(void);
void bsant_die_update(void);
void bsant_die_end(void);
void bsjig_jiggy_init(void);
void bsjig_jiggy_update(void);
void bsjig_jiggy_end(void);
void bsjig_jiggy_interrupt(void);
void bsbtrot_slide_init(void);
void bsbtrot_slide_update(void);
void bsbtrot_slide_end(void);
void func_802A2098(void);
void func_802A2130(void);
void func_802A2054(void);
void func_802B2384(void);
void func_802B242C(void);
void func_802B24AC(void);
void func_802B24D4(void);
void func_802B2580(void);
void func_802B2610(void);
void func_802B2638(void);
void func_802B2750(void);
void func_802B2990(void);
void func_802B29C0(void);
void func_802B2A5C(void);
void func_802B2BD0(void);
void func_802A846C(void);
void func_802A85EC(void);
void func_802A872C(void);
void func_802B2FDC(void);
void func_802B2FFC(void);
void func_802B301C(void);
void func_802B309C(void);
void func_802B3240(void);
void func_802B3448(void);
void func_802AB830(void);
void func_802AB8DC(void);
void func_802ABA10(void);
void func_802ABD60(void);
void func_802ABA30(void);
void func_802ABAA4(void);
void func_802ABCAC(void);
void func_802B1928(void);
void func_802B1A54(void);
void func_802B1BCC(void);
void func_802B5FD0(void);
void func_802B6064(void);
void func_802B60D0(void);
void func_802B60D8(void);
void func_802B6270(void);
void func_802B6314(void);
void func_802B63C8(void);
void bsblongleg_slide_init(void);
void bsblongleg_slide_update(void);
void bsblongleg_slide_end(void);
void func_802B3868(void);
void func_802B3954(void);
void func_802B3A20(void);
void func_802A4430(void);
void func_802A4548(void);
void func_802A4664(void);
void func_802A505C(void);
void func_802A4748(void);
void func_802A48B4(void);
void func_802A4A40(void);
void func_802A4CD0(void);
void func_802A4CF0(void);
void func_802A4D10(void);
void func_802A505C(void);
void func_802A7F6C(void);
void func_802A8098(void);
void func_802A82D4(void);
void bsthrow_init(void);
void bsthrow_update(void);
void bsthrow_end(void);
void bsthrow_interrupt(void);
void bscroc_idle_init(void);
void bscroc_idle_update(void);
void bscroc_idle_end(void);
void bscroc_walk_init(void);
void bscroc_walk_update(void);
void bscroc_walk_end(void);
void bscroc_jump_init(void);
void bscroc_jump_update(void);
void bscroc_jump_end(void);
void bscroc_fall_init(void);
void bscroc_fall_update(void);
void bscroc_fall_end(void);
void func_802A6394(void);
void func_802A63F0(void);
void func_802A6450(void);
void bscroc_ow_init(void);
void bscroc_ow_update(void);
void bscroc_ow_end(void);
void bscroc_die_init(void);
void bscroc_die_update(void);
void bscroc_die_end(void);
void bswalrus_idle_init(void);
void bswalrus_idle_update(void);
void bswalrus_idle_end(void);
void bswalrus_walk_init(void);
void bswalrus_walk_update(void);
void bswalrus_walk_end(void);
void bswalrus_jump_init(void);
void bswalrus_jump_update(void);
void bswalrus_jump_end(void);
void bswalrus_fall_init(void);
void bswalrus_fall_update(void);
void bswalrus_fall_end(void);
void func_802A1F6C(void);
void func_802A1FC8(void);
void func_802A2014(void);
void bswalrus_ow_init(void);
void bswalrus_ow_update(void);
void bswalrus_ow_end(void);
void bswalrus_die_init(void);
void bswalrus_die_update(void);
void bswalrus_die_end(void);
void bscroc_bite_init(void);
void bscroc_bite_update(void);
void bscroc_bite_end(void);
void bscroc_eat_bad_init(void);
void bscroc_eat_bad_update(void);
void bscroc_eat_bad_end(void);
void bscroc_eat_good_init(void);
void bscroc_eat_good_update(void);
void bscroc_eat_good_end(void);
void bsbtrot_fall_init(void);
void bsbtrot_fall_update(void);
void bsbtrot_fall_end(void);
void bssplat_init(void);
void bssplat_update(void);
void bssplat_end(void);
void func_802B3CEC(void);
void func_802B3D1C(void);
void func_802B3D6C(void);
void func_802B3E2C(void);
void func_802B3E64(void);
void func_802B3EF4(void);
void func_802B3D8C(void);
void func_802B3DBC(void);
void func_802B3E0C(void);
void func_802A4F74(void);
void func_802A4FC8(void);
void func_802A503C(void);
void func_802B5E8C(void);
void func_802B5EFC(void);
void func_802B5F38(void);
void func_802A83C0(void);
void func_802A8410(void);
void func_802A844C(void);
void bsbtrot_unk79_init(void);
void bsbtrot_unk79_update(void);
void bsbtrot_unk79_end(void);
void bswalk_mud_init(void);
void bswalk_mud_update(void);
void bsbtrot_ow_init(void);
void bsbtrot_ow_update(void);
void bsbtrot_ow_end(void);
void func_802B3FA4(void);
void func_802B4004(void);
void func_802B4038(void);
void func_802B4080(void);
void bswalrus_sled_init(void);
void bswalrus_sled_update(void);
void bswalrus_sled_end(void);
void func_802B98C0(void);
void bswalrus_sled_jump_init(void);
void bswalrus_sled_jump_update(void);
void bswalrus_sled_jump_end(void);
void func_802B98C0(void);
void func_802A7BD0(void);
void func_802A7CA8(void);
void func_802A7D74(void);
void func_802B978C(void);
void func_802B9830(void);
void func_802B9880(void);
void func_802B90D0(void);
void func_802B9130(void);
void func_802B917C(void);
void func_802B98C0(void);
void func_802B95A0(void);
void func_802B963C(void);
void func_802B976C(void);
void func_802B98C0(void);
void func_802A1080(void);
void func_802A10D4(void);
void func_802A117C(void);
void func_802A11A4(void);
void func_802A1214(void);
void func_802A12D4(void);
void func_802A12FC(void);
void func_802A1438(void);
void func_802A163C(void);
void func_802A1664(void);
void func_802A170C(void);
void func_802A18C8(void);
void func_802A1B68(void);
void func_802A1B88(void);
void func_802A1BA8(void);
void bsbeemain_die_init(void);
void func_802A1DD8(void);
void func_802A1F2C(void);
void func_802A0590(void);
void func_802A0630(void);
void func_802A0704(void);
void bsbeefly_enter(void);
void bsbeefly_update(void);
void bsbeefly_end(void);
void func_802AD56C(void);
void func_802AD5C0(void);
void func_802AD614(void);
void func_8029F398(void);
void func_8029F3F4(void);
void func_8029F440(void);
void func_802B34A0(void);
void func_802B34F8(void);
void func_802B353C(void);
void func_802A4D30(void);
void func_802A4D50(void);
void func_802A4D70(void);
void func_802A505C(void);
void bsant_drone_init(void);
void bsant_drone_update(void);
void bsant_drone_end(void);
void bsdrone_interrupt(void);
void func_802B3564(void);
void func_802B3584(void);
void func_802B35A4(void);
void bsdrone_interrupt(void);
void bscroc_drone_init(void);
void bscroc_drone_update(void);
void bscroc_drone_end(void);
void bsdrone_interrupt(void);
void bswalrus_drone_init(void);
void bswalrus_drone_update(void);
void bswalrus_drone_end(void);
void bsdrone_interrupt(void);
void func_802B5F58(void);
void func_802B5F80(void);
void func_802B5FA0(void);
void bsdrone_interrupt(void);
void func_802A874C(void);
void func_802A8774(void);
void func_802A8794(void);
void bsdrone_interrupt(void);
void bswalk_drone_init(void);
void bswalk_drone_update(void);
void bswalk_drone_end(void);
void bsdrone_interrupt(void);
void func_802A50B0(void);
void func_802A50D8(void);
void func_802A50F8(void);
void bsdrone_interrupt(void);
void bsbtrot_drone_init(void);
void bsbtrot_drone_update(void);
void bsbtrot_drone_end(void);
void bsdrone_interrupt(void);
void bsblongleg_drone_init(void);
void bsblongleg_drone_update(void);
void bsblongleg_drone_end(void);
void bsdrone_interrupt(void);
void bswalrus_sled_drone_init(void);
void bswalrus_sled_drone_update(void);
void bswalrus_sled_drone_end(void);
void bsdrone_interrupt(void);
void bsbee_drone_init(void);
void bsbee_drone_update(void);
void bsbee_drone_end(void);
void bsdrone_interrupt(void);
void func_802ABCCC(void);
void func_802ABD0C(void);
void func_802ABD40(void);
void func_802ABD60(void);
void bsant_bounce_init(void);
void bsant_bounce_update(void);
void bsant_bounce_end(void);
void func_802B303C(void);
void func_802B305C(void);
void func_802B307C(void);
void bscroc_bounce_init(void);
void bscroc_bounce_update(void);
void bscroc_bounce_end(void);
void bswalrus_bounce_init(void);
void bswalrus_bounce_update(void);
void bswalrus_bounce_end(void);
void func_802A1BC8(void);
void func_802A1BE8(void);
void func_802A1C08(void);
void bsbwhirl_drone_init(void);
void bsbwhirl_drone_update(void);
void bsbwhirl_drone_end(void);
void func_802AADBC(void);
void func_802AAE08(void);
void func_802AAE4C(void);
void func_802AEDE8(void);
void func_802AEE48(void);
void func_802AEE9C(void);
void func_802AEEF4(void);
void func_802AEFB0(void);
void func_802AF164(void);
void bsdronexform_init(void);
void bsdronexform_update(void);
void bsdronexform_end(void);
void bsdronexform_interrupt(void);
void func_802AE9C8(void);
void func_802AEA2C(void);
void func_802AEB24(void);
void func_802AF604(void);
void func_802AF668(void);
void func_802AF768(void);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)
void func_80241304(Mtx *m, float x, float y, float z);
void _guMtxIdentF_80245D44(float mf[4][4]);
void * malloc(s32 size);
void free(void*);
void *realloc(void* ptr, s32 size);
f32 ml_map_f(f32 val, f32 in_min, f32 in_max, f32 out_min, f32 out_max);
float mlNormalizeAngle(float);
f32 max_f(f32, f32);
f32 min_f(f32, f32);
void ml_vec3f_copy(f32 dst[3], f32 src[3]);
void ml_vec3f_add(f32 dst[3], f32 src1[3], f32 src2[3]);
void ml_vec3f_scale(f32 vec[3], f32 scale);
void ml_vec3f_scale_copy(f32 dst[3], f32 src[3], f32 scale);
float gu_sqrtf(float val);
BKSpriteFrame *spriteGetFramePtr(BKSprite *, u32);
s32 func_80289FE8(f32);
void func_8028A180(s32 animation, f32 duration);
void func_8028A37C(f32);
int player_inWater(void);
ActorMarker *player_getMarker(void);
u32 player_getTransformation(void);
void func_8028E7EC(f32 (* arg0)[3]);
void _player_getPosition(f32 (* dst)[3]);
void player_getPosition(f32 (* dst)[3]);
void player_getRotation(f32 *dst);
void func_80291A60(s32 arg0, f32* arg1);
int button_pressed(s32);
u32 button_held(s32);
void pitch_setIdeal(f32);
f32 pitch_get(void);
f32 player_getYPosition(void);
void func_80297970(f32);
f32 func_80297AAC(void);
void climbGetBottom(f32 (* dst)[3]);
void yaw_setIdeal(f32);
void func_80299BFC(f32);
f32 roll_get(void);
f32 yaw_get(void);
f32 yaw_getIdeal(void);
void bs_clearState(void);
void bs_setState(s32 state_id);
s32 bs_getPrevState(void);
s32 bs_getState(void);
s32 bs_getNextState(void);
void bs_updateState(void);
s32 bs_checkInterrupt(s32 arg0);
void func_8029A86C(s32 arg0);
s32 bs_getInterruptType(void);
void vla_clear(VLA *this);
void * vla_getBegin(VLA *this);
void * vla_at(VLA *this, u32 n);
s32 vla_getIndex(VLA *this, void *element);
s32 vla_size(VLA *this);
void * vla_getEnd(VLA *this);
void * vla_pushBackNew(VLA **thisPtr);
void * vla_insertNew(VLA **thisPtr, s32 indx);
void vla_free(VLA *this);
VLA * vla_new(u32 elemSize, u32 cnt);
void vla_remove(VLA *this, u32 indx);
void vla_popBack_n(VLA *this, u32 n);
void vla_assign(VLA *this, s32 indx, void* value);
VLA * vla_802ED9E0(VLA *this);
void actor_collisionOff(Actor *);
void *assetcache_get(s32 arg0);
Actor *actor_new(s32 (* position)[3], s32 yaw, ActorInfo *actorInfo, u32 flags);
Actor *func_802C8A54(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *func_802C8AA8(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *func_802C8AF8(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *func_802C8B4C(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *func_802C8BA8(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *func_802C8C04(s32 (* position)[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
Actor *marker_getActor(ActorMarker *);
f32 time_getDelta(void);
void jiggySpawn(u32 indx, f32 (* pos)[3]);
struct8s *func_802FD320(s32);
void func_802FD330(s32, struct8s *);
void func_802FD33C(s32, struct8s *, Gfx**, Mtx**, s32);
void func_802FD350(s32, struct8s *);
struct8s *func_802FD7B0(s32);
void func_802FD80C(s32, struct8s *);
void func_802FDAF4(s32, struct8s *, Gfx**, Mtx**, s32);
void func_802FDC80(s32, struct8s *);
struct8s *func_802FDE2C(s32);
void func_802FE844(s32, struct8s *);
void func_802FDEE0(s32, struct8s *, Gfx**, Mtx**, s32*);
void func_802FDDC4(s32, struct8s *);
struct8s *func_802FF090(s32);
void func_802FFA50(s32, struct8s *);
void func_802FF3B8(s32, struct8s *, Gfx**, Mtx**, s32);
void func_802FF358(s32, struct8s *);
struct8s *func_802FFE4C(s32);
void func_803005BC(s32, struct8s *);
void func_802FFF34(s32, struct8s *, Gfx**, Mtx**, s32);
void func_802FFED4(s32, struct8s *);
struct8s *func_803007C0(s32);
void func_8030081C(s32, struct8s *);
void func_80300974(s32, struct8s *, Gfx**, Mtx**, s32);
void func_80300BB4(s32, struct8s *);
struct8s *func_80300CD8(s32);
void func_80301348(s32, struct8s *);
void func_80300D0C(s32, struct8s *, Gfx**, Mtx**, s32);
void func_80300C70(s32, struct8s *);
struct8s *func_8030179C(s32);
void func_80301DE4(s32, struct8s *);
void func_803017D0(s32, struct8s *, Gfx**, Mtx**, s32);
void func_80301754(s32, struct8s *);
void marker_despawn(ActorMarker *marker);
Actor * spawn_child_actor(enum actor_e id, Actor ** parent);
void func_80324D2C(f32, enum comusic_e);
void particleEmitter_setSprite(ParticleCtrl *, enum asset_e);
void particleEmitter_setPosition(ParticleCtrl *, f32 (*)[3]);
ParticleCtrl *func_802F0BD0(u32);
void func_802BB3DC(s32, f32, f32);
void func_802C3F04(void (* arg0)(void), s32, s32, s32, s32);
void func_802C4140();
void func_8030DA44(u8);
void func_802EF3F4(ParticleCtrl *, f32(*)[3], f32(*)[3], s32);
void particleEmitter_setParticleAccelerationRange(ParticleCtrl *, f32, f32, f32, f32, f32, f32);
void func_802EF9F8(ParticleCtrl *, f32);
void func_802EFA18(ParticleCtrl *, s32);
void func_802EFA5C(ParticleCtrl *, f32, f32);
void func_802EFA70(ParticleCtrl *, s32);
void particleEmitter_setStartingFrameRange(ParticleCtrl *this, s32 arg1, s32 arg2);
void particleEmitter_setParticleFramerateRange(ParticleCtrl *, f32, f32);
void particleEmitter_setParticleSpawnPositionRange(ParticleCtrl *, f32, f32, f32, f32, f32, f32);
void func_802EFB70(ParticleCtrl *, f32, f32);
void func_802EFB84(ParticleCtrl *, f32, f32);
void func_802EFB98(ParticleCtrl *, struct31s *);
void func_802EFC84(ParticleCtrl *, struct41s *);
void particleEmitter_setPositionAndVelocityRanges(ParticleCtrl *this, struct42s *arg1);
void func_802EFE24(ParticleCtrl *, f32, f32, f32, f32, f32, f32);
void particleEmitter_setSpawnIntervalRange(ParticleCtrl *, f32, f32);
void func_802EFEC0(ParticleCtrl *, f32, f32);
void particleEmitter_setParticleVelocityRange(ParticleCtrl *, f32, f32, f32, f32, f32, f32);
void func_802EFF50(ParticleCtrl *, f32);
void func_802EFFA8(ParticleCtrl *this, s32 (*arg1)[3]);
void particleEmitter_setSpawnInterval(ParticleCtrl *, f32);
void func_8024E3A8(f32 (*)[3], f32);
OSMesgQueue *func_8024F344(void);
void ml_vec3f_clear(f32 dst[3]);
void func_80250D94(f32, f32, f32);
f32 func_80257A44(f32, f32);
f32 func_80257C48(f32, f32, f32);
int func_80257F18(f32 src[3], f32 target[3], f32 *yaw);
f32 mlAbsF(f32);
f32 mlClamp_f(f32, f32, f32);
f32 func_802588B0(f32, f32);
void func_802589E4(f32 dst[3], f32 yaw, f32 length);
f32 mlDiffDegF(f32, f32);
void func_8025A6EC(enum comusic_e, s32);
void func_8025A70C(enum comusic_e);
void comusic_8025AB44(enum comusic_e comusic_id, s32 arg1, s32 arg2);
f32 cosf(f32);
struct54s * func_80287CA8(void);
void func_80287F50(struct54s *, struct53s *, s32);
void func_80287FD0(struct54s *, f32);
void func_80287F7C(struct54s *arg0, s32 arg1);
void func_80287F98(struct54s *arg0, s32 arg1);
void func_80287FDC(struct54s *arg0, s32 arg1);
void func_8028A010(enum asset_e anim_id, f32 duration);
void func_8028A100(enum asset_e anim_id, f32, f32);
void func_8028A1F4(s32, f32, f32);
void func_8028A274(enum asset_e, f32);
void func_8028A3B8(f32, f32);
int func_8028AED4(f32*, f32);
f32 func_80291670(s32);
f32 func_80291684(s32);
u32 func_80291698(int);
int func_80291700(s32, f32);
void func_802917E4(s32, f32);
void func_80292078(s32, f32);
void func_80292158(f32);
f32 func_80292230(void);
void func_802927E0(f32, f32);
void func_80292974(f32, f32, f32);
void func_80292900(f32, f32);
void func_80292E80(s32, f32);
void func_80293D48(f32, f32);
f32 func_80294438(void);
f32 func_80294500(void);
void func_80294980(f32 (* arg0)[3]);
f32 get_slope_timer(void);
f32 func_80294A40(void);
void func_80295C08(void (* arg0)(void));
void func_802978DC(int);
void func_80297970(f32);
void func_8029797C(f32);
void player_setYVelocity(f32);
void func_802979AC(f32, f32);
f32 func_80297A64(void);
f32 func_80297A7C(void);
f32 func_80297AB8(void);
f32 func_80297AF0(void);
void gravity_set(f32);
void func_80297BF8(f32);
int func_80297C04(f32);
void func_80297CCC(f32);
f32 pitch_getIdeal(void);
void pitch_setAngVel(f32, f32);
void func_80298528(f32);
f32 func_802987C4(void);
f32 func_802987D4(void);
f32 func_802987E4(void);
void roll_setIdeal(f32);
void roll_setAngularVelocity(f32, f32);
void yaw_set(f32);
void func_802991FC(void);
void func_80299234(f32, f32);
void func_80299254(f32);
void func_8029932C(f32);
void func_80299594(s32, f32);
void func_80299628(s32);
void func_80299650(f32, f32);
void func_80299B58(f32, f32);
void func_80299CF4(enum sfx_e, f32, s32);
void func_80299D2C(s32, f32, s32);
void func_80289EC8(f32, f32, f32, f32);
void func_80289EF8(f32);
f32 func_8029A900(void);
f32 func_8029A90C(void);
void func_8029A968(f32);
void func_8029A980(s32);
void func_8029A974(f32);
void func_8029AD28(f32, s32);
f32 func_8029B2E8(void);
f32 func_8029B30C(void);
void func_8029B324(s32, f32);
f32 func_8029B33C(void);
f32 func_8029B41C(void);
ParticleCtrl *func_8029B950(f32[3],f32);
void func_8029C3E8(f32, f32);
void func_8029CF48(s32, s32, f32);
f32 func_8029DFC8(void);
f32 func_8029DFD4(void);
void func_8029E090(s32, f32);
void func_8029E0C4(f32);
void func_8029E0D0(f32);
void func_8029E180(s32, f32);
void func_8029E3C0(s32, f32);
f32 func_8029E270(s32);
void func_802BD8A4(f32, f32, f32);
void func_802BF2C0(f32);
void func_802C1B20(f32);
int func_802C1DB0(f32);
void func_802EFA20(ParticleCtrl *, f32, f32);
ParticleCtrl *func_802F0D74(ParticleCtrl *);
u8 func_8030D90C(void);
void func_8030DA80(u8 indx, enum sfx_e uid);
void func_8030DBB4(u8, f32);
void func_8030DD14(u8, int);
void func_8030DF68(u8, f32[3]);
void func_8030DFF0(u8, s32);
void func_8030E04C(u8, f32, f32, f32);
void func_8030E2C4(u8);
void func_8030E394(u8 indx);
void func_8030E484(enum sfx_e uid);
void func_8030E4E4(enum sfx_e uid);
void func_8030E510(enum sfx_e uid, s32 arg1);
void func_8030E540(enum sfx_e uid);
void func_8030E560(enum sfx_e uid, s32 arg1);
void func_8030E58C(enum sfx_e uid, f32 arg1);
void func_8030E5F4(enum sfx_e uid, f32 arg1);
void func_8030E624(u32);
void func_8030E6A4(enum sfx_e uid, f32 arg1, s32 arg2);
void func_8030E6D4(enum sfx_e uid);
void func_8030E704(enum sfx_e uid);
void func_8030E760(enum sfx_e uid, f32 arg1, s32 arg2);
void func_8030E878(enum sfx_e uid, f32 arg1, u32 arg2, f32 (*arg3)[3], f32 arg4, f32 arg5);
void func_8030E988(enum sfx_e uid, f32 arg1, u32 arg2, f32 (*arg3)[3], f32 arg4, f32 arg5);
void func_8030E9C4(enum sfx_e uid, f32 arg1, u32 arg2, f32 (*arg3)[3], f32 arg4, f32 arg5);
void func_8030EAAC(enum sfx_e uid, f32 arg1, s32 arg2, s32 arg3);
void func_8030EB00(enum sfx_e uid, f32, f32);
void func_8030EB88(enum sfx_e uid, f32 arg1, f32 arg2);
void func_8030EBC8(enum sfx_e uid, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_8030EC20(enum sfx_e uid, f32 arg1, f32 arg2, u32 arg3, u32 arg4);
u8 func_8030ED2C(enum sfx_e uid, s32 arg1);
void func_8032728C(f32*, f32, s32, s32(*)(Actor *));
int func_8032863C(AnimCtrl *, f32, f32);
int func_80328A2C(Actor *, f32, s32, f32);
void func_80328A84(Actor *, u32);
int func_80328B38(Actor *, s32, f32);
void func_80328B8C(Actor *, s32, f32, s32);
int func_80328BD4(Actor *, s32, f32, s32, f32 );
void func_80328CEC(Actor *, s32, s32, s32);
void func_80328FB0(Actor *, f32);
int func_80329030(Actor *, s32);
int func_80329078(Actor *, s32, s32);
int func_80329480(Actor *);
s32 func_80329784(Actor *);
void func_80329878(Actor *, f32);
int func_803391A4(Gfx**, Mtx**, f32 (*)[3], f32(*)[3], f32, f32*, void*);
void func_80346C10(enum bs_e *retVal, enum bs_e fail_state, enum bs_e success_state, enum item_e item_id, int use_item);
void func_80352CF4(f32 *, f32 *, f32, f32);
AnimCtrl *player_getAnimCtrlPtr(void);
void _get_velocity(f32 (*dst)[3]);
void player_setYPosition(f32);
void *func_80304C38(s32 arg0, Actor *arg1);
Actor *func_80326D68(f32 (*)[3], s32, s32, f32 *);
Actor *func_80329980(Actor *);
void ml_vec3f_pitch_rotate_copy(f32 dst[3], f32 src[3], f32 pitch);
int func_8025773C(f32 *arg0, f32 arg1);
Actor *func_80325888(ActorMarker *, Gfx**, Mtx**, s32);
void func_80335924(void *, s32, f32, f32);
extern Actor *func_80325340(ActorMarker *, Gfx**, Mtx**, s32);
void func_8032AA58(Actor *, f32);
void func_8034DDF0(void *, f32 (*)[3], f32 (*)[3], f32, s32);
void func_80324E38(f32, s32);
void func_8034E1A4(void *, s32, f32, f32);
void func_80324C88(f32, s32, f32, s32);
f32 func_80256064(f32 [3], f32 [3]);
void func_80324E60(f32, s32);
void func_80324E88(f32);
void func_8034DFB0(s32, s32(*)[4], s32(*)[4], f32);
int actor_animationIsAt(Actor*, f32);
void func_80250E94(f32, f32, f32, f32, f32, f32);
void func_8034DC08(void *, f32 (*)[3], f32(*)[3], f32, s32);
void func_802C8F70(f32);
void func_802F9DB8(s32, f32, f32, f32);
void func_802F9F80(s32, f32, f32, f32);
void func_802FA060(s32, s32, s32, f32);
Actor *func_80326EEC(s32);
f32 func_8038A6B8(ActorMarker *);
void func_80255FE4(f32 [3], f32 [3], f32 [3], f32);
void func_8030DEB4(u8, f32, f32);
void func_8030DB04(u8, s32, f32(*)[3], f32, f32);
void func_80258A4C(f32 [3], f32, f32 [3], f32 *, f32 *, f32 *);
void func_803300A8(ActorMarker *, void (*)(ActorMarker *, s32), void (*)(ActorMarker *, s32), void (*)(ActorMarker *, s32));
void func_8033568C(void *, f32 *, f32*);
void func_802E4078(enum map_e map, s32 exit, s32 transition);
void levelSpecificFlags_set(s32, s32);
void func_803228D8(void);
void func_803253A0(Actor *);
void mapSpecificFlags_set(s32, s32);
struct0 *func_8031B9D8(void);
extern int func_80259808(f32);
void func_8034A174(s32, s32, f32[3]);
void actor_playAnimationOnce(Actor *);
void actor_loopAnimation(Actor *);
void func_80326224(Actor *this);
void func_803300D8(ActorMarker *, void (*)(Actor *));
void func_8034DE60(s32, f32, f32, f32, s32);
void func_802F9FD0(s32, f32, f32, f32);
void func_80324D54(f32, s32, f32, s32, f32 (*)[3], f32, f32);
void glcrc_calc_checksum(void *start, void *end, u32 *checksum);
f32 climbGetBottomY(void);
f32 climbGetTopY(void);
void func_802596AC(f32 a0[3], f32 a1[3], f32 a2[3], f32 a3[3]);
void func_8024E55C(s32, void *);
void func_802C3C88(void (*)(ActorMarker *), ActorMarker *);
void func_802FAD64(enum item_e);
void func_80304D68(s16 *, f32[3]);
int func_80311480(s32 text_id, s32 arg1, f32 *pos, ActorMarker *marker, void(*callback)(ActorMarker *, enum asset_e, s32), void(*arg5)(ActorMarker *, enum asset_e, s32));
void func_80347A14(s32);
void ability_unlock(enum ABILITY_E);
struct Overlay {
    void *start;
    void *end;
};
extern struct Overlay gOverlayTable[];
extern s32 D_80276588;
extern s32 D_8027658C;
extern s8 D_8037BFBA;
extern f32 climbPoleBottom[3];
extern f32 climbPoleTop[3];
extern void func_803012F8(void);
extern f32 func_80256280(f32 arg0[3], f32 arg1[3]);
extern void func_8028BA00(s32);
extern void func_802C418C(void);
extern void func_802A6388(f32);
extern f32 func_802D6E4C(Actor *);
extern void func_80294AE8(f32);
extern f32 func_802CA748(Actor *);
extern int func_80259254(f32 vec[3], f32 x, f32 z, f32 val);
extern void func_80324DBC(f32, s32, s32, s32, s32, s32, s32);
extern void func_802EE354(Actor *, s32, s32, s32, f32, f32, f32, f32 arg8[3], s32, f32 arg10[2]);
extern void func_802D6264(f32, s32, s32, s32, s32, s32);
extern void func_8035644C(s32);
extern void func_8035646C(s32 arg0);
extern void func_80291634(ActorMarker *, s32);
extern void func_80291610(ActorMarker *, s32);
extern void func_80291AAC(void);
extern void func_8028D7B8(s32 arg0, ActorMarker *arg1, s32 arg2);
extern s32 carriedObject_actorID;
extern u8 D_8037BF60;
extern u8 D_8037BF61;
extern u8 D_8037BF62;
extern ActorMarker *playerMarker;
extern s32 D_8037BF74;
extern s32 D_8037BF7C;
extern s32 D_8037BF88;
extern ActorMarker *D_8037BF8C;
extern u8 D_8037BF90;
void func_8028D638(s32 arg0, s32 arg1);
s32 can_beak_barge(void){
    return func_802957D8(ABILITY_0_BARGE);
}
s32 func_8028A980(void){
    return func_802957D8(ABILITY_1_BEAK_BOMB);
}
s32 can_beak_bust(void){
    return func_802957D8(ABILITY_2_BEAK_BUSTER);
}
s32 func_8028A9C0(void){
    return func_802957D8(ABILITY_3_CAMERA_CONTROL);
}
s32 can_claw(void){
    return func_802957D8(ABILITY_4_BEAR_PUNCH);
}
s32 func_8028AA00(void){
    return func_802957D8(ABILITY_5_CLIMB);
}
int func_8028AA20(void){
    return func_802957D8(ABILITY_F_DIVE)
        && !func_8029D66C()
        && 100.0f < func_80294500() - func_80294438();
}
s32 can_egg(void){
    return func_802957D8(ABILITY_6_EGGS);
}
int can_flap(void){
    return func_802933D0(0x12)
        && func_802933D0(0x5)
        && func_802957D8(ABILITY_7_FLAP);
}
s32 can_flip(void){
    return func_802957D8(ABILITY_8_FLIP);
}
s32 func_8028AB28(void){
    return func_802957D8(ABILITY_9_FLY);
}
s32 func_8028AB48(void){
    return func_802957D8(ABILITY_A_HOLD_A_JUMP_HIGHER);
}
int can_peck(void){
    return func_802933D0(0x5)
        && func_802933D0(0x12)
        && func_802957D8(ABILITY_B_RATATAT_RAP);
}
int func_8028ABB8(void){
        if(func_802933C0(0x14) || func_802933C0(0x19))
            return 0;
        if(bs_getState() == BS_56_RECOIL)
            return 0;
        return 1;
}
s32 func_8028AC18(void){
    return func_802957D8(ABILITY_C_ROLL);
}
s32 func_8028AC38(void){
    return func_802957D8(ABILITY_D_SHOCK_JUMP);
}
s32 func_8028AC58(void){
    return func_802957D8(ABILITY_E_WADING_BOOTS);
}
s32 can_trot(void){
    return func_802957D8(ABILITY_10_TALON_TROT);
}
s32 func_8028AC98(void){
    return func_802957D8(ABILITY_10_TALON_TROT);
}
s32 can_wonderwing(void){
    return func_802957D8(ABILITY_12_WONDERWING);
}
int func_8028ACD8(void){
    if( func_803348C0() == MAP_27_FP_FREEZEEZY_PEAK && mapSpecificFlags_get(0xd)){
        return 0;
    }
    if(func_802C2B00() == 3)
        return 0;
    if(!func_8028B2E8() && !player_inWater())
        return 0;
    return 1;
}
int func_8028AD64(void){
    return bsant_inSet(bs_getState());
}
int func_8028AD8C(void){
    return bspumpkin_inSet(bs_getState());
}
int func_8028ADB4(void){
    return func_8032190C() && func_803348C0() != MAP_1_SM_SPIRAL_MOUNTAIN;
}
int func_8028ADF0(void){
    return func_803203FC(0x9d);
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028AE10.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028AED4.s")
int func_8028B094(void){
    return (60.0f < player_getYPosition() - func_80294438());
}
int func_8028B0E0(f32 arg0[3], f32 arg1){
    f32 sp1C[3];
    _player_getPosition(sp1C);
    return func_80259254(sp1C, arg0[0], arg0[2], arg1);
}
s32 func_8028B120(void){return 0;}
int func_8028B128(void){
    return func_802933C0(0x13);
}
int func_8028B148(void){
    return level_get() == LEVEL_9_RUSTY_BUCKET_BAY;
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B16C.s")
int player_shouldSlideTrot(void){
    return func_80291698(6)
        || (1.0 == get_slope_timer() && func_80294610(0x40));
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B254.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B2E8.s")
int player_isSliding(void){
    return func_80291698(5)
        || 1.0 == get_slope_timer();
}
int func_8028B394(void){
    return func_8029CF20(4);
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B3B4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B424.s")
extern s32 D_8037BF80[2];
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B470.s")
int func_8028B4C4(void){
    return 135.0f < mlAbsF(mlDiffDegF(yaw_getIdeal(), yaw_get()));
}
int player_inWater(void){
    return D_8037BF61;
}
int func_8028B528(void){
    return D_8037BF62;
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B534.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B59C.s")
void func_8028B6FC(void){
    D_8037BF60 = 1;
    D_8037BF61 = 0;
    D_8037BF62 = 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028B71C.s")
s32 D_80363630[2] = {0x20, 0x1C};
s32 D_80363638[2] = {0x1C, 0x16};
s32 D_80363640[2] = {0x20, 0x01};
s32 D_80363648[2] = {0x1C, 0x18};
s32 D_80363650[2] = {0x20, 0x21};
s32 D_80363658[2] = {0x20, 0x28};
f32 D_80363660[3] = {350.0f, 0.0f, 600.0f};
u8 D_8036366C[] = {
    0xff, 0xff, 0x00,
    0xff, 0x00, 0x00,
    0x00, 0xff, 0x00,
    0x00, 0x00, 0xff,
    0xff, 0x00, 0xff,
    0x00, 0xff, 0xff,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0xff,
    0x00, 0x00, 0x00,
    0x00, 0xff, 0x00,
    0x00, 0x00, 0xc8
};
s32 *func_8028B750(void){
    if(D_8037BF80[0]){
        return D_8037BF80;
    }
    else{
        switch(player_getTransformation()){
            case TRANSFORM_2_TERMITE:
                return &D_80363638;
                break;
            case TRANSFORM_3_PUMPKIN:
                return &D_80363640;
                break;
            case TRANSFORM_4_WALRUS:
                return &D_80363650;
                break;
            case TRANSFORM_5_CROC:
                return &D_80363648;
                break;
            case TRANSFORM_6_BEE:
                return &D_80363658;
                break;
            case TRANSFORM_1_BANJO:
            default:
                return &D_80363630;
                break;
        }
    }
}
void func_8028B7F4(void){
    yaw_setIdeal(func_8029B41C());
    func_802991A8(1);
    func_8029957C(3);
    func_802978DC(2);
    func_80297970(0.0f);
    func_8029151C(0xC);
}
void func_8028B848(void){
    s32 s0 = 0;
    if(func_803348C0() == MAP_69_GL_MM_LOBBY){
        s0 = 0xf57;
    }
    else if(func_803348C0() == MAP_2_MM_MUMBOS_MOUNTAIN){
        s0 = 0xb45;
    }
    if(s0){
        if(player_getTransformation() == TRANSFORM_1_BANJO){
            func_8028B7F4();
        }
        func_80311480(s0, 0x2A, 0, 0, 0, 0);
    }
}
void func_8028B8DC(void){
    func_8028F918(1);
    D_8037BF88 = 1;
}
void func_8028B904(s32 arg0, s32 arg1, s32 arg2, s32 arg3){
    f32 sp1C[3];
    sp1C[0] = (*((s16 *)&arg0));
    sp1C[1] = (*((s16 *)&arg1));
    sp1C[2] = (*((s16 *)&arg2));
    func_80296CC0(&sp1C);
    func_80296CB4(arg3);
    if(func_802933C0(7)){
        func_8029CDA0();
    }
    else{
        if(bs_checkInterrupt(9) == 1){
            func_8029CDA0();
        }
    }
}
void func_8028B9A8(s32 arg0){
    s16 *tmp_v0;
    s32 sp18[3];
    tmp_v0 = func_803049CC(0x1F6, 0);
    func_80304D4C(tmp_v0, &sp18);
    func_802CA1CC(arg0);
    func_8032811C(0x47, &sp18, 0);
    func_8025A6EC(COMUSIC_2B_DING_B, 28000);
}
void func_8028BA00(s32 arg0){
    func_802C3C88(func_8028B9A8, arg0);
    if(arg0);
}
void func_8028BA2C(s32 arg0) {
    if (func_802FADD4(0x1B) == 0) {
        func_80345F24(ITEM_C_NOTE);
    } else {
        func_803463F4(ITEM_C_NOTE, 1);
    }
    if (item_getCount(ITEM_C_NOTE) < 100) {
        func_8025A6EC(SFX_NOTE_COLLECTED, 0x3E80);
        timedFunc_set_1(0.75f, func_8035644C, 3);
    }
    func_802F379C(arg0 + 4);
}
void func_8028BAB0(enum jiggy_e jiggy_id, s32 arg1, s32 arg2, s32 arg3){
    if(jiggyscore_isCollected(jiggy_id))
        return;
    if(mapSpecificFlags_get(arg1) && mapSpecificFlags_get(arg2)){
        mapSpecificFlags_set(arg3, 1);
    }
    mapSpecificFlags_set(arg1, 1);
    func_8030E6D4(SFX_90_SWITCH_PRESS);
}
void func_8028BB1C(s32 arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6){
    u32 sp24;
    if(arg0 != 1)
        return;
    if(func_8028ECAC() == 1)
        return;
    if(arg1 & 0x400000){
        sp24 = arg1 + 0xFFC00000;
        if(!func_803203FC(sp24)){
            func_803204E4(sp24, 1);
            func_8030E6D4(SFX_90_SWITCH_PRESS);
            func_803204E4(0xBF, 1);
            func_802D6264(1.0f, arg2, arg3, arg4, arg5, arg6);
        }
    }
    else{
        if(!func_8031FF1C(arg1)){
            func_80320004(arg1, 1);
            func_8030E6D4(SFX_90_SWITCH_PRESS);
            func_803204E4(0xBF, 1);
            func_802D6264(1.0f, arg2, arg3, arg4, arg5, arg6);
        }
    }
}
int func_8028BC20(ActorMarker *marker){
    return D_8037BF90 == 1 && func_8028E76C(marker) == 0x5;
}
int func_8028BC60(void){
    return D_8037BF90 == 1 && func_8028E76C(0) == 0x5;
}
void func_8028BCA0(Prop *prop){
    if(*((u32*)(((u32)prop) + 8)) & 1){
        s32 spCC = 0;
        s32 spC8 = 0;
        s32 spC4;
        s32 tmp_v0_2;
        ActorMarker *marker = prop->actorProp.marker;
        Actor *actor = 0;
        f32 spAC[3];
        f32 spA0[3];
        s32 sp9C;
        s32 sp98;
        volatile s32 sp94;
        s32 sp88[3];
        s32 sp7C;
        s32 sp78;
        f32 sp70;
        vec3f sp64;
        s32 sp60;
        if(marker->unk3E_0){
            actor = marker_getActor(marker);
            if(actor->despawn_flag)
                return;
            if(actor->unk10_0 && func_802C9C14(actor))
                return;
        }
        spC4 = func_8028E76C(marker);
        if(func_8033D410(playerMarker, marker))
            return;
        switch(marker->unk14_20){
            case 0x125:
            case 0x126:
                spC8 = 1;
                break;
            case 0x97:
                spCC = 1;
                spC8 = 1;
                break;
            case 0xBA:
                if( marker->unk40_31 == 1
                    || marker->unk40_31 == 2
                    || marker->unk40_31 == 3
                    || marker->unk40_31 == 4
                    || marker->unk40_31 == 5
                    || marker->unk40_31 == 6
                ){
                    spC8 = 1;
                }
                break;
            case 0xB5:
                if(func_8028BC20(marker))
                    return;
                if(func_802D8E68(prop)){
                    marker_despawn(marker);
                }
                break;
            case 0x1E5:
                if(func_8028BC20(marker))
                    return;
                if(func_802D8EDC(prop)){
                    marker_despawn(marker);
                }
                break;
            case 0x9E:
            case 0xA1:
                if(spC4 == 1){
                    func_802A02B4(1);
                    spC8 = 2;
                }
                break;
            case 0x28:
                if( spC4 == 6){
                    func_8030E484(0x20);
                }
                else{
                    func_8030E6D4(0x65);
                }
                if(spC4 != 6){
                    spCC = 2;
                }
                break;
            case 0xF5:
                if(spC4 == 1)
                    func_8028BAB0(JIGGY_20_BGS_ELEVATED_WALKWAY, 1, 3, 7);
                break;
            case 0xFD:
                if(spC4 == 1)
                    func_8028BAB0(JIGGY_25_BGS_MAZE, 0xa, 0xc, 8);
                break;
            case 0xEC:
                if(spC4 == 1){
                    mapSpecificFlags_set(3, 1);
                    func_8030E6D4(SFX_90_SWITCH_PRESS);
                }
                break;
            case 0xF2:
                if(spC4 == 1){
                    if(!mapSpecificFlags_get(0xD)){
                        mapSpecificFlags_set(0xD, 1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                        func_802BAFE4(0x14);
                        timedFunc_set_1(1.5f, func_8028BA00, 0xB);
                    }
                }
                break;
            case 0x15F:
                if(spC4 == 1){
                    if(!mapSpecificFlags_get(0)){
                        mapSpecificFlags_set(0,1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                        func_802BAFE4(0x7E);
                        timedFunc_set_1(1.5f, func_8028BA00, 0xf);
                    }
                }
                break;
            case 0xF1:
                if(func_8028ECAC() == 1)
                    return;
                if(!mapSpecificFlags_get(5)){
                    mapSpecificFlags_set(5, 1);
                    func_8030E6D4(SFX_90_SWITCH_PRESS);
                }
                break;
            case 0x231:
            case 0x244:
                {
                    if(func_8028ECAC() == 1)
                        return;
                    player_getPosition(spAC);
                    spAC[1] += 40.0f;
                    if(func_802458E0(spAC, actor, 0x87) == 0)
                        return;
                    func_803204E4(0x1E, 1);
                    if(func_8031FF1C(((actor->unkF4_8 - 1) ^ 1) + 0x49)){
                        actor->unk10_12 = 2;
                    }
                    else{
                        actor->unk10_12 = 1;
                    }
                }
                break;
            case 0xFE:
                if(spC4 == 1){
                    if(func_8028ECAC() == 1)
                        return;
                    if(!mapSpecificFlags_get(0)){
                        mapSpecificFlags_set(0,1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                    }
                }
                break;
            case 0x23F:
                if(spC4 == 1){
                    if(func_8028ECAC() == 1)
                        return;
                    if(!mapSpecificFlags_get(0)){
                        mapSpecificFlags_set(0,1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                    }
                }
                break;
            case 0x110:
                if(spC4 == 1){
                    if(func_8028ECAC() == 1)
                        return;
                    if(!mapSpecificFlags_get(0)){
                        mapSpecificFlags_set(0, 1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                    }
                }
                break;
            case 0x113:
                if(spC4 == 1){
                    if(func_8028ECAC() == 1)
                        return;
                    if(!mapSpecificFlags_get(1)){
                        mapSpecificFlags_set(1,1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                    }
                }
                break;
            case 0x115:
                if(spC4 == 1){
                    if(func_8028ECAC() == 1)
                        return;
                    if(!mapSpecificFlags_get(2)){
                        mapSpecificFlags_set(2,1);
                        func_8030E6D4(SFX_90_SWITCH_PRESS);
                    }
                }
                break;
            case 0x241:
                func_8028BB1C(spC4, 0xC6, 0x6E, 0x81, 0x2C, 0xA, 0xC7);
                break;
            case 0x23C:
                func_8028BB1C(spC4, 0xA3, 0x92, 0x7F, 0x1A, 0xA, 0xA4);
                break;
            case 0x161:
                func_8028BB1C(spC4, 0x4000BE, 0x6E, 0x7D, 0x19, 0x14, 0xA0);
                break;
            case 0x162:
                func_8028BB1C(spC4, 0x4000BD, 0x71, 0x7C, 0x18, 0x14, 0x9F);
                break;
            case 0x166:
                func_8028BB1C(spC4, 0x4000BC, 0x6A, 0x7A, 0x17, 0x14, 0x9A);
                break;
            case 0x22B:
                func_8028BB1C(spC4, 0x4000BB, 0x6F, 0x3A, 0x13, 0x15, 0x47);
                break;
            case 0x22A:
                func_8028BB1C(spC4, 0x4000BA, 0x79, 0x39, 0x12, 0x15, 0x46);
                break;
            case 0x103:
                func_8028BB1C(spC4, 0x4000B6, 0x69, 0x26, 1, 4, 0x18);
                break;
            case 0x104:
                func_8028BB1C(spC4, 0x4000B7, 0x6F, 0x27, 2, 0x14, 0x19);
                break;
            case 0x105:
                func_8028BB1C(spC4, 0x4000B8, 0x6D, 0x28, 3, 0x14, 0x1A);
                break;
            case 0x106:
                func_8028BB1C(spC4, 0x4000B9, 0x76, 0x29, 4, 0xe, 0x1C);
                break;
            case 0x11B:
                func_8028BB1C(spC4, 0x22, 0x77, 0x2D, 5, 0xA, 0x23);
                break;
            case 0x11C:
                func_8028BB1C(spC4, 0x24, 0x77, 0x2E, 6, 0xA, 0x25);
                break;
            case 0x11D:
                func_8028BB1C(spC4, 0x26, 0x76, 0x2F, 0x7, 0xA, 0x27);
                break;
            case 0x232:
                func_8028BB1C(spC4, 0x53, 0x6b, 0x3b, 0x14, 0x12, 0x54);
                break;
            case 0xF3:
                if(spC4 == 3){
                    mapSpecificFlags_set(6,1);
                    spC8 = 1;
                }
                break;
            case 0x52:
                {
                    if(func_8028BC20(marker))
                        return;
                    player_getPosition(spA0);
                    sp9C = func_802C8088(actor);
                    if( sp9C != 0x2F
                        || (func_8028B2E8() && !(3600.0f < func_80256280(actor->position, spA0)))
                    ){
                        jiggyscore_8032108C(sp9C, 1);
                        func_803463F4(0x26, 1);
                        if(sp9C == 0x20 || sp9C == 0x25){
                            func_802D6924();
                        }
                        if(jiggyscore_total() < 3){
                            func_8028B848();
                        }
                        if(func_8025AD7C(0x30)){
                            func_8028B8DC();
                        }
                        func_802C3F04(func_8028B904, (*((u32 *)&prop->actorProp.x)), (*((u32 *)&prop->actorProp.y)), (*((u32 *)&prop->actorProp.z)), sp9C);
                        marker_despawn(marker);
                    }
                }
                break;
            case 0x53:
                {
                    if(func_8028BC20(marker))
                        return;
                    sp98 = func_802CA1C4(marker_getActor(marker));
                    if(sp98 != 0x12 || player_getTransformation() == TRANSFORM_3_PUMPKIN)
                    {
                        func_80321364(sp98, 1);
                        func_8025A6EC(COMUSIC_17_EMPTY_HONEYCOMB_COLLECTED, 28000);
                        timedFunc_set_1(2.0f, func_8035644C, 0xB);
                        func_80345F24(ITEM_13_EMPTY_HONEYCOMB);
                        if(!(item_getCount(ITEM_13_EMPTY_HONEYCOMB) < 6)){
                            func_80314AC8(0);
                        }
                        func_802F36DC(&prop->actorProp.x);
                        marker_despawn(marker);
                    }
                }
                break;
            case 0x54:
                func_8025A6EC(COMUSIC_19_LOW_PITCH_FLUTES, 28000);
                func_803012F8();
                func_802C3F04(func_802C418C, 0x4E, (*((u32 *)&prop->actorProp.x)), (*((u32 *)&prop->actorProp.y)), (*((u32 *)&prop->actorProp.z)));
                marker_despawn(marker);
                break;
            case 0x55:
                if(func_8028BC20(marker))
                    return;
                if( func_803348C0() == MAP_8E_GL_FURNACE_FUN
                    && func_803203FC(0)
                    && !func_8031FF1C(BKPROG_A6_FURNACE_FUN_COMPLETE)
                ){
                    func_80356540(BKPROG_A6_FURNACE_FUN_COMPLETE);
                    func_8030E6D4(SFX_126_AUDIENCE_BOOING);
                }
                func_8025A6EC(COMUSIC_16_HONEYCOMB_COLLECTED, 28000);
                timedFunc_set_1(0.75f, func_8035644C, 0xA);
                func_80345F24(ITEM_14_HEALTH);
                func_802F373C(&prop->actorProp.x);
                marker_despawn(marker);
                break;
            case 0x169:
                {
                    switch (func_803348C0())
                    {
                    case 0x1D:
                        sns_set_item_and_update_payload(6, 0, 1);
                        break;
                    case 0x61:
                        sns_set_item_and_update_payload(1, 0, 1);
                        break;
                    case 0x2C:
                        sns_set_item_and_update_payload(3, 0, 1);
                        break;
                    case 0x3F:
                        sns_set_item_and_update_payload(2, 0, 1);
                        break;
                    case 0x92:
                        sns_set_item_and_update_payload(4, 0, 1);
                        break;
                    case 0x8F:
                        sns_set_item_and_update_payload(5, 0, 1);
                        break;
                    }
                    func_8025A70C(COMUSIC_88_BIG_SNS_FANFARE);
                    func_8030E624( ((unsigned int) (((unsigned int)(0x332) & ((0x01 << (11)) - 1)) << (21))) | ((unsigned int) (((unsigned int)(0x1D4) & ((0x01 << (10)) - 1)) << (11))) | ((unsigned int) (((unsigned int)(SFX_114_BRICKWALL_BREAKING) & ((0x01 << (11)) - 1)) << (0))));
                    sp70 = 0.9f;
                    {
                        tmp_v0_2 = 3*(actor->unkF4_8 - 1);
                        sp64 = *((vec3f*) &D_80363660);
                        sp78 = 0xA;
                        sp94 = 0xAA;
                        for(sp7C = 0; sp7C < 4; sp7C++){
                            int i;
                            for(i = 0 ; i < 3; i++){
                                sp88[i] = D_8036366C[tmp_v0_2 + i];
                            }
                            func_802EE354(actor, 0x3ED, 0x23, sp78, 0.2f, sp70, 3.0f, sp88, 0, &sp64);
                            for(i = 0 ; i < 3; i++){
                                sp88[i] = 0xFF;
                            }
                            func_802EE354(actor, 0x3ED, 0xe, sp78, 0.2f, sp70, 3.0f, sp88, 0, &sp64);
                            sp78 += 0x32;
                            sp70 += -0.15;
                            if(sp70 < 0.01){
                                sp70 = 0.01f;
                            }
                            sp64.x -= 50.0f;
                            sp64.z += 260.0f;
                            sp94 -= 0x1e;
                        }
                        sp60 = sns_get_item_state(1, 0) + sns_get_item_state(2, 0) + sns_get_item_state(3, 0)
                            + sns_get_item_state(4, 0) + sns_get_item_state(5, 0) + sns_get_item_state(6, 0);
                        if(sp60 < 3){
                            func_80324DBC(2.5f, 0xDB2 + sp60, 0x20, 0, 0, 0, 0);
                        }
                    }
                    marker_despawn(marker);
                }
                break;
            case 0x168:
                sns_set_item_and_update_payload(7, 0, 1);
                func_8025A70C(COMUSIC_88_BIG_SNS_FANFARE);
                func_80324DBC(2.5f, 0xDB5, 0x20, 0, 0, 0, 0);
                marker_despawn(marker);
                break;
            case 0x5F:
                if(func_8028BC20(marker))
                    return;
                func_8028BA2C(prop);
                marker_despawn(marker);
                break;
            case 0x60:
                if(func_8028BC20(marker))
                    return;
                if(func_802D8DF0(prop)){
                    marker_despawn(marker);
                }
                break;
            case 0x6B:
                if(func_8028EE84() == 2){
                    func_803463D4(ITEM_17_AIR, func_80301DBC(2));
                }
                break;
            case 0x61:
                if(func_8028BC20(marker))
                    return;
                if( func_803348C0() == MAP_8E_GL_FURNACE_FUN
                    && func_803203FC(0)
                    && !func_8031FF1C(BKPROG_A6_FURNACE_FUN_COMPLETE)
                ){
                    func_80356540(BKPROG_A7_NEAR_PUZZLE_PODIUM_TEXT);
                    func_8030E6D4(SFX_127_AUDIENCE_MIXED);
                }
                func_8025A6EC(COMUSIC_15_EXTRA_LIFE_COLLECTED, 0x7FFF);
                timedFunc_set_1(1.5f, func_8035646C, 0xC);
                func_802F3B3C(&prop->actorProp.x);
                func_80345F24(ITEM_16_LIFE);
                marker_despawn(marker);
                break;
            case 0xD4:
            case 0x242:
                if(ability_isUnlocked(ABILITY_D_SHOCK_JUMP)){
                    func_802933E8(2);
                }else{
                    if(!func_80320454(0xC, 1)){
                        func_80311480(0xA24, 4, 0, 0, 0, 0);
                    }
                }
                break;
            case 0x45:
            case 0x240:
            case 0x261:
                if(ability_isUnlocked(ABILITY_9_FLY)){
                    func_802933E8(1);
                }
                else if(! func_80320454(0xD, 1)){
                    func_80311480(0xA25, 4, 0, 0, 0, 0);
                }
                break;
            case 0x11:
                if(func_8028BC20(marker))
                    return;
                if(func_8029A8F4() != TRANSFORM_1_BANJO)
                    return;
                if(func_8028F170())
                    return;
                if(func_8028F25C())
                    return;
                if(func_802D6E0C(actor) == 0)
                    return;
                func_802933E8(0xe);
                func_802A6388(func_802D6E4C(actor));
                bs_checkInterrupt(0x1B);
                func_802C3F04(func_802C418C, 0x4E, (*((u32 *)&prop->actorProp.x)), (*((u32 *)&prop->actorProp.y)), (*((u32 *)&prop->actorProp.z)));
                func_802D6E54(actor);
                break;
            case 0x38:
                {
                    s32 tmp;
                    if(func_8028BC20(marker))
                        return;
                    tmp = func_8029A8F4();
                    if(tmp != TRANSFORM_1_BANJO && tmp != TRANSFORM_5_CROC)
                        return;
                    if(func_8028F25C())
                        return;
                    if(func_8028F170())
                        return;
                    if(func_802CA708(actor) == 0)
                        return;
                    func_802933E8(0x10);
                    func_80294AE8(func_802CA748(actor));
                    bs_checkInterrupt(0x1A);
                    func_802C3F04(func_802C418C, 0x4E, (*((u32 *)&prop->actorProp.x)), (*((u32 *)&prop->actorProp.y)), (*((u32 *)&prop->actorProp.z)));
                    func_802CA750(actor);
                }
                break;
            case 0x1AE:
                switch(spC4){
                    case 1:
                    case 2:
                    case 5:
                    case 6:
                        spC8 = 2;
                        break;
                    default:
                        spCC = 2;
                        break;
                }
                break;
            case 0x1B1:
                if(spC4 == 1)
                    spC8 = 1;
                break;
        }
        if(func_80297C6C() == 3){
            spCC = 0;
        }
        if(spC8){
            func_802933E8(8);
        }
        func_8032FFF4(playerMarker, marker, spCC);
        func_8032FFF4(marker, playerMarker, spC8);
        if(marker->unk3E_0){
            func_8032B258(actor, spC8);
        }
    }
    else if(prop->unk8_1)
    {
        s32 tmp = prop->propProp.unk0_31 + 0x2D1;
        switch (tmp)
        {
        case 0x2E8:
            func_802933E8(1);
            break;
        case 0x2DD:
            func_802933E8(2);
            break;
        default:
            func_80332790(tmp);
            break;
        }
    }
    else{
        s32 tmp = prop->spriteProp.unk0_31 + 0x572;
        switch (tmp)
        {
        case 0x6D6:
            if(!func_8028BC60()){
                prop->spriteProp.unk8_4 = 0;
                func_8028BA2C(prop);
            }
            break;
        case 0x6D7:
            if(!func_8028BC60()){
                prop->spriteProp.unk8_4 = 0;
                func_802D8DF0(prop);
            }
            break;
        case 0x580:
            if(!func_8028BC60()){
                prop->spriteProp.unk8_4 = 0;
                func_802D8E68(prop);
            }
            break;
        case 0x6D1:
            if(!func_8028BC60()){
                prop->spriteProp.unk8_4 = 0;
                func_802D8EDC(prop);
            }
            break;
        default:
            func_80332790(tmp);
            break;
        }
    }
}
void func_8028D1FC(void){
    f32 sp1C[3];
    _player_getPosition(sp1C);
    playerMarker = func_8032FBE4(sp1C, func_80291AAC, 1, 0);
    playerMarker->unk2C_1 = 1;
    func_803300A8(playerMarker, 0, func_80291634, func_80291610);
    func_803300B8(playerMarker, func_8028D7B8);
    func_802933FC(1);
    func_802933FC(2);
    func_802933FC(8);
    func_8028D638(0,0);
    func_8033D2F4();
    D_8037BF8C = 0;
    D_8037BF90 = 0xff;
}
void func_8028D2BC(void){
    marker_free(playerMarker);
    playerMarker = 0;
}
void func_80320ED8(ActorMarker *, f32, s32);
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028D2E4.s")
ActorMarker *player_getMarker(void){
    return playerMarker;
}
void func_8028D5DC(void){
    playerMarker->collidable = 0;
}
void func_8028D5F4(void){
    playerMarker->collidable = 1;
}
s32 func_8028D60C(void){
    return (playerMarker->collidable)? 1 : 0;
}
void func_8028D638(s32 arg0, s32 arg1){
    D_8037BF80[0] = arg0;
    D_8037BF80[1] = arg1;
}
void func_8028D64C(s32 arg0){
    D_8037BF74 = arg0;
}
void carriedObject_setActorID(s32 arg0){
    carriedObject_actorID = arg0;
}
s32 func_8028D664(void){
    return D_8037BF74;
}
s32 carriedObject_getActorID(void){
    return carriedObject_actorID;
}
void func_8028D67C(s32 arg0){
    D_8037BF7C = arg0;
}
s32 func_8028D688(void){
    return D_8037BF7C;
}
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028D694.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028D6F0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028D71C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_39D0/func_8028D7B8.s")
