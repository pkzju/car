#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef __FALSE
#define __FALSE   (0)
#endif

#ifndef __TRUE
#define __TRUE    (1)
#endif

typedef char               S8;
typedef unsigned char      U8;
typedef short              S16;
typedef unsigned short     U16;
typedef int                S32;
typedef unsigned int       U32;
typedef long long          S64;
typedef unsigned long long U64;

#define PROTOCAL_tHeader        0x5A
#define S_IDLE     0
#define S_tHeader   1
#define S_LEN      2
#define S_VERSION  3
#define S_PAYLOAD  4
#define S_CRC      5
/**************************************** CMD List **********************************************/

#define CMD_ID_POSITION_INFO      0x40
#define CMD_ID_POSITION_NO        0x20

/**************************************** Data Struct ****************************************/

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    char     flag;
    char     cmd;
    char     p_x;
    char     p_y;
    char     p_z;
    unsigned char     crc8;
}tHeader;      
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    unsigned short    crc16;
}tCheckSum;    //common tail
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct Position_Data
{
    signed short p_x:16; 
    signed short p_y:16;  
    signed short p_z:16; 
}Position_Data;
#pragma pack(pop)



#ifdef  __cpluscplus
extern "C"
{
#endif

#define PROTOCAL_FRAME_MAX_SIZE  50

unsigned char FrameUnpack(unsigned char token, unsigned char* pBuffer);
unsigned char FramePack(unsigned char* pData);
//unsigned char  FramePack(unsigned char* pDataIn, unsigned char len, unsigned char* pDataOut);
unsigned char* ScanHeader(unsigned char pData);
#ifdef  __cpluscplus
}
#endif


#endif  //__PROTOCAL_H__

