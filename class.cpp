#include "main.h"

void c_Spawn_Ring::Reload_Value() {
    int n = rand() % 2 + 1; // 1 - 2 Rings per spawn
    for (int i = 0; i < n; i++)
        Ring_Value[i] = rand() % MAX_COLOR + 1;
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
    this->Lst_Save.push_back(c_Save_Ring(x, y, Type));
}

void c_Match_Ring::Clear() {
    this->Count = 1;
    this->Lst.clear();
    this->Lst.insert(Lst.end(), Lst_Save.begin(), Lst_Save.end());
}
