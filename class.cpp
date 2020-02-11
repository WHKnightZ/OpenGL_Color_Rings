#include "main.h"

void c_Spawn_Ring::Reload_Value() {
    int n = rand() % 2 + 1; // 1 - 2 Rings per spawn
    for (int i = 0; i < n; i++)
        Ring_Value[i] = rand() % 3 + 1;
    for (int i = n; i < 3; i++)
        Ring_Value[i] = 0;
    for (int i = 0; i < 3; i++) {
        n = rand() % 3;
        Swap(&Ring_Value[i], &Ring_Value[n]);
    }
}

void c_Spawn_Ring::Reload_Pos(float x, float y) {
    this->x = x;
    this->y = y;
    Rect *p;
    p = &Rct_Check;
    p->Left = x - RCT_CHECK_HALF_SIZE;
    p->Right = x + RCT_CHECK_HALF_SIZE;
    p->Bottom = y - RCT_CHECK_HALF_SIZE;
    p->Top = y + RCT_CHECK_HALF_SIZE;
    for (int i = 0; i < 3; i++) {
        p = &Rct_Ring[i];
        p->Left = x - Ring_Offset[i];
        p->Bottom = y - Ring_Offset[i];
        p->Right = p->Left + Ring_Size[i];
        p->Top = p->Bottom + Ring_Size[i];
    }
}

void c_Match_Ring::Init(int Color, int x, int y, int Type) {
    this->Color = Color;
    this->Count = 1;
    this->Lst_Save.clear();
    this->Lst_Save.push_back(c_Save_Ring(x, y, Type, Color));
}

void c_Match_Ring::Clear() {
    this->Count = 1;
    this->Lst.clear();
    this->Lst.insert(Lst.end(), Lst_Save.begin(), Lst_Save.end());
}

// Effect

c_Explode_Dot::c_Explode_Dot(int x, int y, int i, int j, int Color) {
    Stt = Move = 0;
    c_Explode_Dot *p = &Explode_Dot_Default[i][j];
    this->x = Start_X + x * SPACE_SIZE + p->x;
    this->y = Start_Y + y * SPACE_SIZE + p->y;
    this->vx = p->vx;
    this->vy = p->vy;
    this->Max = p->Max;
    this->Color = Color;
    Reload();
}

void c_Explode_Dot::Init(float x, float y, float vx, float vy, int Max) {
    Stt = Move = 0;
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->Max = Max;
    Reload();
}

void c_Explode_Dot::Reload() {
    Rct.Left = x - Explode_Dot_Radius[Stt];
    Rct.Right = x + Explode_Dot_Radius[Stt];
    Rct.Bottom = y - Explode_Dot_Radius[Stt];
    Rct.Top = y + Explode_Dot_Radius[Stt];
}

c_Zoom_Ring::c_Zoom_Ring(int x, int y, int Type, int Color) {
    this->x = x;
    this->y = y;
    this->Type = Type;
    this->Color = Color;
    Rct = Map[y][x].Rct_Ring[Type];
    Offset = Ring_Offset[Type] / MAX_ZOOM_TIME;
}
void c_Zoom_Ring::Reload() {
    Rct.Left += Offset;
    Rct.Right -= Offset;
    Rct.Bottom += Offset;
    Rct.Top -= Offset;
}
