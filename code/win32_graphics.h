#if !defined(WIN32_GRAPHICS_H)

#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <stdio.h>

//Windows Callback
LRESULT Win32WindowsCallback(HWND WindowHandler,
                             UINT Message,
                             WPARAM WParam,
                             LPARAM LParam);

//Additional functions
void ProcessMessage(MSG Message);
void UpdateClientRectSize();


typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef size_t mm;
typedef uintptr_t umm;

typedef int32_t b32;

#define global static
#define local_global static


#define Assert(Expression) if (!(Expression)) {__debugbreak();}
#define InvalidCodePath Assert(!"Invalid Code Path")

struct Screen
{
    u32 srcWidth;
    u32 srcHeight; 
};

struct FrameBuffer
{
    u32 Width;
    u32 Height;
    u32* Pixels;
};

struct global_state
{
    b32 IsRunning;
    HWND WindowHandler;
    HDC DeviceContext;
    FrameBuffer FrameBufferS;
    Screen ScreenS;
};







#define WIN32_GRAPHICS_H
#endif
