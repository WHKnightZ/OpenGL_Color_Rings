#ifndef MAIN_H
#define MAIN_H

#include "../../Library/loadpng.h"
#include "../../Library/process_image.h"

#include <list>

#include <math.h>

#include <GL/glut.h>

#include "../../Library/gl_texture.h"

/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

#define WIDTH 480
#define HEIGHT 640
#define MAX_X 3
#define MAX_Y 3
#define START_Y 110.0f
#define SPAWN_RING_Y 540.0f
#define SPACE_SIZE 140.0f
#define RCT_CHECK_HALF_SIZE 50.0f
#define MAX_COLOR 6
#define MOVE_VELOCITY_BASE 8.0f
#define INTERVAL 15

int POS_X, POS_Y;
int Max_X = MAX_X - 1, Max_Y = MAX_Y - 1;

enum GAME_STATE {
    GAME_STT
};

enum RING_COLOR {
    NONE,
    RED,
    GREEN,
    BLUE,
    CYAN,
    PINK,
    YELLOW
};

class c_Cell {
  public:
    Rect Rct_Dot;
    Rect Rct_Check;
    int Ring_Value[3]; // Small - Medium - Large
    Rect Rct_Ring[3];
};

class c_Spawn_Ring {
  public:
    float x, y;
    Rect Rct_Check;
    int Ring_Value[3];
    Rect Rct_Ring[3];
    void Reload_Value();
    void Reload_Pos(float x, float y);
};

class c_Save_Ring {
  public:
    c_Save_Ring(int x, int y, int Type) {
        this->x = x;
        this->y = y;
        this->Type = Type;
    }
    int x, y, Type;
};

class c_Match_Ring {
  public:
    bool Is_Add;
    int Color, Count;
    std::list<c_Save_Ring> Lst_Save;
    std::list<c_Save_Ring> Lst;
    void Init(int Color, int x, int y, int Type);
    void Clear();
};

c_Match_Ring Match_Ring[3];
int Match_Ring_Count;

class c_Explode_Ball {
  public:
    float x, y, r, vx, vy;
    int Color, Stt;
    Rect Rct;
};

// Prototype

// afunc.h

// class.h
// find.h
int Find_In_List(int &Color);
void Find_Matching(int &x, int &y);
void Push_Front(int &x, int &y);
void Push_Back(int &x, int &y);
void Find_Matching_SW_NE(int &x, int &y);
void Find_Matching_W_E(int &x, int &y);
void Find_Matching_NW_SE(int &x, int &y);
void Find_Matching_N_S(int &x, int &y);

// init.h
void Swap(int *x, int *y);
// main.h

// Function_Pointer
void (*Find_Matching_Func[])(int &x, int &y) = {Find_Matching_SW_NE, Find_Matching_W_E, Find_Matching_NW_SE, Find_Matching_N_S};

// Variable
float Color_White[] = {1.0f, 1.0f, 1.0f};

float Ring_Color[MAX_COLOR + 1][3] = {
    {1.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f}};

float Spawn_Ring_X, Spawn_Ring_Y;
float Ring_Size[3], Ring_Offset[3];

float Start_X, Start_Y, Start_X_Rct, Start_Y_Rct;
Image Img_Ring[3];
Image Img_Dot;
c_Cell Map[MAX_Y][MAX_X];
c_Spawn_Ring Spawn_Ring;
bool Is_Hold_Mouse = false, Is_Move_Ring = false, Is_Put = false;
int Move_Timer, Move_Max;
int Dest_X, Dest_Y;
float Move_Velocity_X, Move_Velocity_Y;
float Move_Dest_X, Move_Dest_Y;

// including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "class.cpp"
#include "find.cpp"
#include "init.cpp"

#endif
