#include "main.h"

void c_Spawn_Ring::Reload_Value() {
    int n = rand() % 3;
    if (n == 0)
        n = 1; // 1 - 2 Rings per spawn but 1 has greater chance
    for (int i = 0; i < n; i++)
        Ring_Value[i] = rand() % Max_Color + 1;
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

c_Line::c_Line(c_Save_Ring &c1, c_Save_Ring &c2) {
    float xx1 = Start_X + c1.x * SPACE_SIZE;
    float yy1 = Start_Y + c1.y * SPACE_SIZE;
    float xx2 = Start_X + c2.x * SPACE_SIZE;
    float yy2 = Start_Y + c2.y * SPACE_SIZE;
    if (c1.y == c2.y) {
        x1 = xx1 - LINE_WIDTH;
        y1 = yy1 - LINE_THICKNESS;
        x2 = xx2 + LINE_WIDTH;
        y2 = yy2 - LINE_THICKNESS;
        x3 = xx2 + LINE_WIDTH;
        y3 = yy2 + LINE_THICKNESS;
        x4 = xx1 - LINE_WIDTH;
        y4 = yy1 + LINE_THICKNESS;
        Offset1 = 0.0f;
        Offset2 = -LINE_THICKNESS / MAX_LINE_TIME;
    } else if (c1.x == c2.x) {
        x1 = xx1 + LINE_THICKNESS;
        y1 = yy1 - LINE_WIDTH;
        x2 = xx2 + LINE_THICKNESS;
        y2 = yy2 + LINE_WIDTH;
        x3 = xx2 - LINE_THICKNESS;
        y3 = yy2 + LINE_WIDTH;
        x4 = xx1 - LINE_THICKNESS;
        y4 = yy1 - LINE_WIDTH;
        Offset1 = LINE_THICKNESS / MAX_LINE_TIME;
        Offset2 = 0.0f;
    } else if (c1.y < c2.y) {
        x1 = xx1 - Line_Width_2 + Line_Thickness_2;
        y1 = yy1 - Line_Width_2 - Line_Thickness_2;
        x2 = xx2 + Line_Width_2 + Line_Thickness_2;
        y2 = yy2 + Line_Width_2 - Line_Thickness_2;
        x3 = xx2 + Line_Width_2 - Line_Thickness_2;
        y3 = yy2 + Line_Width_2 + Line_Thickness_2;
        x4 = xx1 - Line_Width_2 - Line_Thickness_2;
        y4 = yy1 - Line_Width_2 + Line_Thickness_2;
        Offset1 = Line_Thickness_2 / MAX_LINE_TIME;
        Offset2 = -Offset1;
    } else {
        x1 = xx1 - Line_Width_2 - Line_Thickness_2;
        y1 = yy1 + Line_Width_2 - Line_Thickness_2;
        x2 = xx2 + Line_Width_2 - Line_Thickness_2;
        y2 = yy2 - Line_Width_2 - Line_Thickness_2;
        x3 = xx2 + Line_Width_2 + Line_Thickness_2;
        y3 = yy2 - Line_Width_2 + Line_Thickness_2;
        x4 = xx1 - Line_Width_2 + Line_Thickness_2;
        y4 = yy1 + Line_Width_2 + Line_Thickness_2;
        Offset1 = -Line_Thickness_2 / MAX_LINE_TIME;
        Offset2 = Offset1;
    }
    Color = c1.Color;
}

void c_Line::Reload() {
    x1 -= Offset1;
    y1 -= Offset2;
    x2 -= Offset1;
    y2 -= Offset2;
    x3 += Offset1;
    y3 += Offset2;
    x4 += Offset1;
    y4 += Offset2;
}

void c_Line::Draw() {
    glColor3fv(Ring_Color[Color]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((int)x1, (int)y1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((int)x2, (int)y2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((int)x3, (int)y3);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((int)x4, (int)y4);
    glEnd();
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
