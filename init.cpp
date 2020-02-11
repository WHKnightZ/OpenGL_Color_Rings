#include "main.h"

// Effect

void Init_Effect() {
    int n = 0;
    float r = 5.0f;
    while (r < 7.0f) {
        Explode_Dot_Radius[n] = r;
        r *= 1.08f;
        n++;
    }
    Explode_Dot_Radius_Center = n;
    while (r > 2.0f) {
        Explode_Dot_Radius[n] = r;
        r /= 1.08f;
        n++;
    }
    Explode_Dot_Radius_Max = n;
    Explode_Dot_Radius[n - 1] = 0.0f;
    for (int i = 0; i < Explode_Dot_Radius_Max; i++) {
        Explode_Dot_Radius[i] = (int)Explode_Dot_Radius[i];
    }
    float x, y, vx, vy;
    int Max;
    FILE *f = fopen("Effects/Ring_Explode.txt", "r");
    fscanf(f, "%d", &Explode_Dot_Max);
    for (int i = 0; i < Explode_Dot_Max; i++) {
        fscanf(f, "%d", &Explode_Dot_Full[i]);
        for (int j = 0; j < Explode_Dot_Full[i]; j++) {
            fscanf(f, "%f%f%f%f%d", &x, &y, &vx, &vy, &Max);
            Explode_Dot_Default[i][j].Init(x, y, vx, vy, Max);
        }
    }
    fclose(f);
}

int Find_In_List_Spawn_Dot(int x, int y) {
    for (int i = 0; i < Count_List_Spawn_Dot; i++)
        if (List_Spawn_Dot[i].x == x && List_Spawn_Dot[i].y == y)
            return i;
    return -1;
}

// End Effect

void Swap(int *x, int *y) {
    int Tmp = *x;
    *x = *y;
    *y = Tmp;
}

int Check_In_Rect(Rect *Rct, int x, int y) {
    return (x > Rct->Left && x < Rct->Right && y > Rct->Bottom && y < Rct->Top);
}

int Check_Can_Put(int Cell_X, int Cell_Y, int x, int y) {
    if (Cell_X < 0 || Cell_Y < 0 || Cell_X >= MAX_X || Cell_Y >= MAX_Y)
        return false;
    c_Cell *p = &Map[Cell_Y][Cell_X];
    if (!Check_In_Rect(&p->Rct_Check, x, y))
        return false;
    for (int i = 0; i < 3; i++)
        if (p->Ring_Value[i] && Spawn_Ring.Ring_Value[i])
            return false;
    return true;
}

void Init_Game() {
    srand(time(NULL));
    Rect *p;
    Load_Texture(&Img_Dot, "Images/Dot.png");
    Load_Texture(&Img_Line, "Images/Line.png");
    Load_Texture(&Img_Ring[0], "Images/Skin_Circle_Small.png");
    Load_Texture(&Img_Ring[1], "Images/Skin_Circle_Medium.png");
    Load_Texture(&Img_Ring[2], "Images/Skin_Circle_Large.png");
    Start_X = (WIDTH - (MAX_X - 1) * SPACE_SIZE) / 2;
    Start_Y = START_Y;
    Start_X_Rct = Start_X - SPACE_SIZE / 2;
    Start_Y_Rct = Start_Y - SPACE_SIZE / 2;
    float Dot_Size = (float)Img_Dot.w;
    float Dot_Offset = Dot_Size / 2;
    for (int i = 0; i < 3; i++) {
        Ring_Size[i] = (float)Img_Ring[i].w;
        Ring_Offset[i] = Ring_Size[i] / 2;
    }
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            p = &Map[i][j].Rct_Dot;
            p->Left = Start_X + j * SPACE_SIZE - Dot_Offset;
            p->Bottom = Start_Y + i * SPACE_SIZE - Dot_Offset;
            p->Right = p->Left + Dot_Size;
            p->Top = p->Bottom + Dot_Size;
            p = &Map[i][j].Rct_Check;
            p->Left = Start_X + j * SPACE_SIZE - RCT_CHECK_HALF_SIZE;
            p->Bottom = Start_Y + i * SPACE_SIZE - RCT_CHECK_HALF_SIZE;
            p->Right = Start_X + j * SPACE_SIZE + RCT_CHECK_HALF_SIZE;
            p->Top = Start_Y + i * SPACE_SIZE + RCT_CHECK_HALF_SIZE;
            for (int k = 0; k < 3; k++) {
                p = &Map[i][j].Rct_Ring[k];
                p->Left = Start_X + j * SPACE_SIZE - Ring_Offset[k];
                p->Bottom = Start_Y + i * SPACE_SIZE - Ring_Offset[k];
                p->Right = p->Left + Ring_Size[k];
                p->Top = p->Bottom + Ring_Size[k];
            }
        }
    }
    Spawn_Ring_X = WIDTH / 2.0f;
    Spawn_Ring_Y = SPAWN_RING_Y;
    Spawn_Ring_Begin = SPAWN_RING_Y_BEGIN;
    Spawn_Ring_Offset = (SPAWN_RING_Y_BEGIN - SPAWN_RING_Y) / MAX_SPAWN_RING_TIME;
    Spawn_Ring.Reload_Value();
    Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Y);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                Map[i][j].Ring_Value[k] = 0;
    Init_Effect();
}

void Init_GL() {
    glClearColor(0.165f, 0.141f, 0.212f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WIDTH, HEIGHT);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
}
