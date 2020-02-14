#ifndef MAIN_H
#define MAIN_H

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include <list>

#include <time.h>

#include <math.h>

#include <GL/glut.h>

#include "../Library/gl_texture.h"

/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

#define WIDTH 440
#define HEIGHT 650
#define MAX_X 3
#define MAX_Y 3
#define START_Y 194.0f
#define START_SCORE_Y 54.0f
#define START_COMBO_Y 112.0f
#define SPAWN_RING_Y 574.0f
#define SPAWN_RING_Y_BEGIN 674.0f
#define SPACE_SIZE 126.0f
#define RCT_CHECK_HALF_SIZE 50.0f
#define MAX_COLOR 6
#define MOVE_VELOCITY_BASE 8.0f
#define MAX_SPAWN_RING_TIME 20
#define MAX_ZOOM_TIME 20
#define MAX_LINE_TIME 20
#define MAX_SCORE_TIME 20
#define MAX_RING_SKIN 4
#define COUNT_TO_INCREASE_COLOR 12

#define SQRT_2 1.414213f

#define LINE_WIDTH 70.0f
#define LINE_THICKNESS 40.0f

#define INTERVAL 15

int POS_X, POS_Y;
int Max_X = MAX_X - 1, Max_Y = MAX_Y - 1;
int Max_Color = 3, Game_Count = 0;

enum GAME_STATE {
    STATE_MENU,
    STATE_PLAY,
    STATE_RING_EXPLODE,
    STATE_RING_SPAWN,
    STATE_GAMEOVER
};

enum RING_SKIN {
    CIRCLE,
    SQUARE,
    DIAMOND,
    HEXAGON
};

char Skin_Text[MAX_RING_SKIN][16] = {"Circle", "Square", "Diamond", "Hexagon"};

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
    c_Save_Ring(int x, int y, int Type, int Color) {
        this->x = x;
        this->y = y;
        this->Type = Type;
        this->Color = Color;
    }
    int x, y, Type, Color;
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

class c_Zoom_Ring {
  public:
    c_Zoom_Ring(int x, int y, int Type, int Color);
    int x, y, Type, Color;
    float Offset;
    Rect Rct;
    void Reload();
};

int Zoom_Ring_Stt;
std::list<c_Zoom_Ring> List_Zoom_Ring;

class c_Line {
  public:
    c_Line(c_Save_Ring &c1, c_Save_Ring &c2);
    float x1, y1, x2, y2, x3, y3, x4, y4;
    float Offset1, Offset2;
    int Color;
    void Reload();
    void Draw();
};

int Line_Stt;
std::list<c_Line> List_Line;
float Line_Width_2 = LINE_WIDTH / SQRT_2, Line_Thickness_2 = LINE_THICKNESS / SQRT_2;

class c_Explode_Dot {
  public:
    c_Explode_Dot() {}
    c_Explode_Dot(int x, int y, int i, int j, int Color);
    void Init(float x, float y, float vx, float vy, int Max);
    float x, y, vx, vy;
    int Color, Stt, Move, Max;
    Rect Rct;
    void Reload();
};

class c_Spawn_Dot {
  public:
    int x, y, Color;
    void Init(int x, int y, int Color) {
        this->x = x;
        this->y = y;
        this->Color = Color;
    }
};

// Prototype

// afunc.h
void Draw_Board();
void Game_Display_Menu();
void Game_Display_Play();
void Game_Display_Ring_Explode();
void Game_Display_Ring_Spawn();
void Game_Display_GameOver();
void Game_Process_Menu();
void Game_Process_Play();
void Game_Process_Ring_Explode();
void Game_Process_Ring_Spawn();
void Game_Process_GameOver();

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
void Update_Score();
void Update_Rect_Score();
void Update_Combo();
// main.h

// Function_Pointer
void (*Find_Matching_Func[])(int &x, int &y) = {Find_Matching_SW_NE, Find_Matching_W_E, Find_Matching_NW_SE, Find_Matching_N_S};
void (*Game_Display_Func[])() = {Game_Display_Menu, Game_Display_Play, Game_Display_Ring_Explode, Game_Display_Ring_Spawn, Game_Display_GameOver};
void (*Game_Process_Func[])() = {Game_Process_Menu, Game_Process_Play, Game_Process_Ring_Explode, Game_Process_Ring_Spawn, Game_Process_GameOver};

// Variable

int Game_State;

float Color_White[] = {1.0f, 1.0f, 1.0f};

//float BG_Color[]={0.165f, 0.141f, 0.212f};
//
//float Ring_Color[MAX_COLOR + 1][3] = {
//    {1.000f, 1.000f, 1.000f},
//    {0.000f, 1.000f, 0.000f},
//    {0.000f, 1.000f, 1.000f},
//    {1.000f, 0.000f, 0.808f},
//    {1.000f, 0.890f, 0.000f},
//    {1.000f, 0.000f, 0.000f},
//    {0.075f, 0.216f, 1.000f}};

float BG_Color[] = {0.145f, 0.192f, 0.220f};

float Ring_Color[][3] = {
    {1.000f, 1.000f, 1.000f},
    {0.380f, 0.786f, 0.408f},
    {1.000f, 0.906f, 0.322f},
    {0.224f, 0.620f, 0.996f},
    {1.000f, 0.380f, 0.286f},
    {0.588f, 0.494f, 0.824f},
    {1.000f, 0.596f, 0.702f},
    {0.463f, 0.839f, 0.929f},
    {0.557f, 0.439f, 0.392f}};

int Skin = CIRCLE;

// Menu
Image Img_Logo;
Image Img_Arrow_Up, Img_Arrow_Left, Img_Arrow_Right;
int Arrow_Up_Alpha[6], Arrow_Up_Current, Arrow_Up_Timer;
Rect Rct_Arrow_Up[6];
int Arrow_LR_Timer;
Rect Rct_Arrow_Left[20], Rct_Arrow_Right[20];
int Arrow_LR_Loop[20];

// Game
float Spawn_Ring_X, Spawn_Ring_Y, Spawn_Ring_Begin, Spawn_Ring_Offset;
int Spawn_Ring_Stt;
float Ring_Size[3], Ring_Offset[3];

float Start_X, Start_Y, Start_X_Rct, Start_Y_Rct;
Image Img_Ring_Full[MAX_RING_SKIN][3], Img_Dot_Full[MAX_RING_SKIN];

Image *Img_Ring[3];
Image *Img_Dot, Img_Line;

c_Cell Map[MAX_Y][MAX_X];
c_Spawn_Ring Spawn_Ring;

bool Is_Hold_Mouse = false, Is_Move_Ring = false, Is_Put = false;
int Move_Timer, Move_Max;
int Dest_X, Dest_Y;
float Move_Velocity_X, Move_Velocity_Y;
float Move_Dest_X, Move_Dest_Y;

// Ring Explode
int Find_In_List_Spawn_Dot(int x, int y);
std::list<c_Explode_Dot> List_Explode_Dot;
c_Explode_Dot Explode_Dot_Default[33][3];
int Explode_Dot_Full[33];
int Explode_Dot_Stt, Explode_Dot_Max, Explode_Dot_Radius_Center, Explode_Dot_Radius_Max;
float Explode_Dot_Radius[22];
c_Spawn_Dot List_Spawn_Dot[9];
int Count_List_Spawn_Dot;

// Text
Image Img_Score[10];
int Score, Score_Array[3], Score_Length;
float Score_Width, Score_Height, Score_Factor;
Rect Rct_Score[3];
int Score_Time;
bool Is_Update_Score = false;
float Score_Factor_Init[MAX_SCORE_TIME];

Image Img_Combo[11];
int Combo, Combo_Array[3], Combo_Length;
float Combo_Width, Combo_Height, Combo_Bottom, Combo_Top;
Rect Rct_Combo[3];

// Including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "class.cpp"
#include "find.cpp"
#include "init.cpp"

#endif
