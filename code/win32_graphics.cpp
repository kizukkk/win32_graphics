#include <windows.h>
#include "win32_graphics.h"
#include <windows.h>
#include "win32_graphics.h"

//Structure of global varibles 
global global_state GlobalState;

//Start point of application
int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR     lpCmdLine,
            int       nShowCmd)
{
    //Init runn flag
    GlobalState.IsRunning = true;

    //Create base window structure    
    WNDCLASSA WindowClass = {};
    {        
        //Init window class structure params
        WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      //Styles of application
        WindowClass.lpfnWndProc = Win32WindowsCallback;              //Application  event callback 
        WindowClass.cbClsExtra = 0;                                  //NULL by default
        WindowClass.cbWndExtra = 0;                                  //NULL by default
        WindowClass.hInstance = hInstance;                           //Instance of application
        WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
        WindowClass.hbrBackground = NULL;                            //Background color (Null for custom window view)
        WindowClass.lpszMenuName = NULL;                             //Application menu names
        WindowClass.lpszClassName = "My Application";  //Window (application) title

        //Catch window class create faile
        if (!RegisterClassA(&WindowClass))
        {
            InvalidCodePath;
        }
    }    
    
    //End

    //Init window of application
    {
        GlobalState.WindowHandler = CreateWindowExA(
            0,                                              //dwExStyle - additional application style
            WindowClass.lpszClassName,                      //lpClassName - application Class Name
            WindowClass.lpszClassName,                      //lpWindowName - application Window name 
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,               //dwStyle - another application style
            CW_USEDEFAULT,                                  //X - x postion of window on screen
            CW_USEDEFAULT,                                  //Y - y positiob of window on screen
            720,                                            //nWidth - width of application window
            480,                                            //nHeight - hight of application window
            NULL,                                           //hWndParent - instance of parent
            NULL,                                           //hMenu = instance of menu 
            hInstance,                                      //hInstance - instacne of application
            NULL                                            //lParam - link to another application params
                                                    );

        //Catch faile execution of window init
        if (!GlobalState.WindowHandler)
        {
            InvalidCodePath;
        }

        //Get DeviceContext (area of screen on window)
        GlobalState.DeviceContext = GetDC(GlobalState.WindowHandler);   
    }
    //End

    //Dedicate FrameBuffer size
    {
        UpdateClientRectSize();
        
        //Calc memory size for FrameBuffer
        GlobalState.FrameBufferS.Width = GlobalState.ScreenS.srcWidth;
        GlobalState.FrameBufferS.Height = GlobalState.ScreenS.srcHeight;
        GlobalState.FrameBufferS.Pixels = (u32*) malloc(sizeof(u32)
                                                        * GlobalState.FrameBufferS.Width
                                                        * GlobalState.FrameBufferS.Height);

        //Iterate FrameBuffer and set pixels color
        //Create UA flag <:
        for (u32 Y = 0; Y < GlobalState.FrameBufferS.Height; ++Y)
        {
            for(u32 X = 0; X < GlobalState.FrameBufferS.Width; ++X)
            {
                //Calc row shift like 2D matrix
                u32 PixelID = GlobalState.FrameBufferS.Width * Y + X;
                if (Y > GlobalState.FrameBufferS.Height / 2)
                {
                    GlobalState.FrameBufferS.Pixels[PixelID] = 0xFF005BBB;
                }
                else
                {
                    GlobalState.FrameBufferS.Pixels[PixelID] = 0xFFFFD500;
                }
                
            }
        }

        //First init
        PastFrameBuffer();

    }
    //End
 
    //Main loop
    while(GlobalState.IsRunning)
    {
        MSG Message = {};

        ProcessMessage(Message);

        PastFrameBuffer();
       
    }
    //End
    
    return 1;
}


LRESULT Win32WindowsCallback(HWND WindowHandler,
                             UINT Message,
                             WPARAM WParam,
                             LPARAM LParam
                             )
{

    LRESULT Result = {};
    
    switch(Message)
    {
        //Handle Window size change
        case WM_SIZE:
        {
            UpdateClientRectSize();
            
        } break;

        case WM_SIZING:
        {
            PastFrameBuffer();
        } break;
        
        //Handle window destroy and close event
        case WM_DESTROY:
        case WM_CLOSE:
        {
            GlobalState.IsRunning = false;
        } break;
        
        default:
        {
            //Default Window event handler
            Result = DefWindowProcA(WindowHandler, Message, WParam, LParam);
        } break;
    }
      
    return Result;
}


void ProcessMessage(MSG Message)
{
    while(PeekMessageA(&Message, GlobalState.WindowHandler, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_QUIT:
            {
                GlobalState.IsRunning = false;
            } break;
                
            default:
            {
                //Default message handler 
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            } break;
        }
    }
}

void UpdateClientRectSize()
{
    //Create RECT and get client rect area by pointer
    RECT ClientRect = {};
    if (!GetClientRect(GlobalState.WindowHandler, &ClientRect))
    {
        return ;
    }
    
    //Get size of area by RECT params
    GlobalState.ScreenS.srcWidth = ClientRect.right - ClientRect.left; 
    GlobalState.ScreenS.srcHeight = ClientRect.bottom - ClientRect.top;
}


void PastFrameBuffer()
{
    //Create BITMAP Struct
    //BITMAPHEADER
    BITMAPINFOHEADER BitmapHeader = {
        sizeof(BITMAPINFOHEADER),                  //Bit size of sctruct
        (LONG)GlobalState.FrameBufferS.Width,        //Width of buffer
        (LONG)GlobalState.FrameBufferS.Height,       //Heght of buffer
        1,
        32,
        BI_RGB,
        0,0,0,0,0
    };

    BITMAPINFO BitmapInfo = {
        BitmapHeader,
        NULL,
    };
        
        
    //Copies BITMAP (pixels) into screen buffer
    Assert(StretchDIBits(
               GlobalState.DeviceContext,
               0,
               0,
               GlobalState.ScreenS.srcWidth,
               GlobalState.ScreenS.srcHeight,
               0,
               0,
               GlobalState.FrameBufferS.Width,
               GlobalState.FrameBufferS.Height,
               GlobalState.FrameBufferS.Pixels,
               &BitmapInfo,
               DIB_RGB_COLORS,
               SRCCOPY));    
}
