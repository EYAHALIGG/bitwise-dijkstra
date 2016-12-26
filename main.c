/// https://www.youtube.com/watch?v=cLPvEp9d06s
 
#include <iostream>
#include <Windows.h>
using namespace std;
 
#define x64 unsigned long long
#define width 8
#define height width
/// bit map (x,y) [0...7]
#define b1 (x64)1
#define left ((b1)|(b1<<8)|(b1<<16)|(b1<<24)|(b1<<32)|(b1<<40)|(b1<<48)|(b1<<56))
#define right (left<<7)
#define getcell(x,y) (b1 << (y*width+x))
#define setblock(x,y) blocks|=getcell(x,y)
#define last(a) (a & ~(a-1))
#define rendersize 10
 
HDC hDC;
 
void drawbox(int x1, int y1, int s, DWORD col)
{
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
            SetPixelV(hDC, x1+j, y1+i, col);
    }
}
 
void printbits(x64 x)
{
    for (int i = 0; i < 8; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            cout << (x & 1);
            x >>= 1;
        }
        cout << "" << endl;
    }
    cout << "" << endl;
}
 
void renderbits(x64 x, DWORD col)
{
    for (int i = 0; i < 8; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (x & 1)
                drawbox(i*rendersize,k*rendersize, rendersize,col);
            x >>= 1;
        }
    }
}
 
int main()
{
    hDC = GetDC(GetConsoleWindow());
 
 
    x64 blocks = 0, closed = 0;
    x64 follow[300];
    unsigned int fCounter = 0;
    setblock(1, 6);
    setblock(2, 6);
    setblock(3, 6);
    setblock(4, 6);
    setblock(5, 6);
    setblock(3, 3);
    setblock(4, 3);
    setblock(6, 3);
    setblock(7, 3);
    x64 source = getcell(5, 0);
    x64 target = getcell(0, 7);
    closed |= blocks;
    closed |= source;
    x64 a = source;
    while (!(closed&target))
    {
        closed |= follow[fCounter++] = a = (((a >> 1)&~right) | ((a << 1)&~left) | (a << 8) | (a >> 8)) & ~closed;
    }
    fCounter-=2;
    x64 path = 0;
    a = target;
    for (int i = fCounter; i >= 0; i--)
    {
        a = (follow[i] & ((a >> 1) | (a << 1) | (a << 8) | (a >> 8)));
        a = last(a);
        path |= a;
    }
    renderbits(path, RGB(0, 255, 255));
    renderbits(blocks, RGB(255, 0, 0));
    renderbits(source|target, RGB(0, 255, 0));
    while (!(GetAsyncKeyState(VK_ESCAPE) && GetForegroundWindow() == GetConsoleWindow()));
    return 0;
}
