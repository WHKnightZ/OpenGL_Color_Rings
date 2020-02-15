#include "main.h"

void Swap(int *x, int *y) {
    int Tmp = *x;
    *x = *y;
    *y = Tmp;
}

// Menu

void Init_Arrow() {
    // Arrow Up
    Load_Texture(&Img_Arrow_Up, "Images/Arrow.png");
    float Left = (WIDTH - Img_Arrow_Up.w) / 2;
    float Right = Left + Img_Arrow_Up.w;
    float Start_Y = SPAWN_RING_Y - 100.0f;
    float Offset = 24.0f;
    Rect *p;
    for (int i = 0; i < 6; i++) {
        p = &Rct_Arrow_Up[i];
        p->Left = Left;
        p->Right = Right;
        p->Bottom = Start_Y - Offset * i;
        p->Top = p->Bottom + Img_Arrow_Up.h;
    }

    // Arrow Left, Right
    Rotate_Left(&Img_Arrow_Up, &Img_Arrow_Left);
    Rotate_Right(&Img_Arrow_Up, &Img_Arrow_Right);
    float s = 0.0f, v = 0.0f, a = 0.2f;
    float Arrow_LR_Offset[20];
    for (int i = 0; i <= 10; i++) {
        Arrow_LR_Offset[i] = (int)s;
        v += a;
        s += v;
    }
    for (int i = 11; i < 20; i++) {
        Arrow_LR_Offset[i] = Arrow_LR_Offset[20 - i];
    }
    float Center = WIDTH / 2.0f, Width = Img_Arrow_Left.w;
    float Bottom = SPAWN_RING_Y - Img_Arrow_Left.h / 2;
    float Top = Bottom + Img_Arrow_Left.h;
    Offset = 70.0f;
    for (int i = 0; i < 20; i++) {
        p = &Rct_Arrow_Left[i];
        p->Right = Center - Arrow_LR_Offset[i] - Offset;
        p->Left = p->Right - Width;
        p->Bottom = Bottom;
        p->Top = Top;
        p = &Rct_Arrow_Right[i];
        p->Left = Center + Arrow_LR_Offset[i] + Offset;
        p->Right = p->Left + Width;
        p->Bottom = Bottom;
        p->Top = Top;
    }
    Rct_Check_Arrow_Left.Right = Center - Offset - 10.0f;
    Rct_Check_Arrow_Left.Left = Rct_Check_Arrow_Left.Right - 30.0f;
    Rct_Check_Arrow_Right.Left = Center + Offset + 10.0f;
    Rct_Check_Arrow_Right.Right = Rct_Check_Arrow_Right.Left + 30.0f;
    Rct_Check_Arrow_Left.Bottom = Rct_Check_Arrow_Right.Bottom = Bottom - 10.0f;
    Rct_Check_Arrow_Left.Top = Rct_Check_Arrow_Right.Top = Top + 10.0f;
}

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
    float Array[4];
    int Max;
    FILE *f = fopen("Effects/Ring_Explode.effect", "rb");
    fread(&Explode_Dot_Max, sizeof(int), 1, f);
    for (int i = 0; i < Explode_Dot_Max; i++) {
        fread(&Explode_Dot_Full[i], sizeof(int), 1, f);
        for (int j = 0; j < Explode_Dot_Full[i]; j++) {
            fread(Array, sizeof(float), 4, f);
            fread(&Max, sizeof(int), 1, f);
            Explode_Dot_Default[i][j].Init(Array[0], Array[1], Array[2], Array[3], Max);
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

// Score

void Init_Img_Score() {
    char Str[50];
    Image Img_Full, Img_Crop;
    Load_Texture(&Img_Full, "Fonts/Score.png");
    FILE *f = fopen("Fonts/Score.fnt", "r");
    int i, n;
    int Id, x, y, w, h, xo, yo, wf, hf;
    fscanf(f, "%d %d", &n, &hf);
    for (i = 0; i < n; i++) {
        fscanf(f, "%d %d %d %d %d %d %d %d", &Id, &x, &y, &w, &h, &xo, &yo, &wf);
        Crop_Image(&Img_Full, &Img_Crop, x, y, w, h);
        Create_Image(&Img_Score[Id], wf, hf);
        Mix_Image(&Img_Score[Id], &Img_Crop, xo, yo);
        Delete_Image(&Img_Crop);
    }
    Score_Height = (float)hf;
    Delete_Image(&Img_Full);
    n = MAX_SCORE_TIME / 2;
    for (int i = 0; i < n; i++)
        Score_Factor_Init[i] = 1.0f + 0.03f * i;
    for (int i = n; i < MAX_SCORE_TIME; i++)
        Score_Factor_Init[i] = Score_Factor_Init[MAX_SCORE_TIME - i - 1];
}

void Update_Score() {
    int x = Score;
    Score_Length = 0;
    int Size = 0, Size_s;
    do {
        Score_Array[Score_Length] = x % 10;
        Size += Img_Score[Score_Array[Score_Length]].w;
        x /= 10;
        Score_Length++;
    } while (x > 0);
    Score_Width = (float)Size;
}

void Update_Rect_Score() {
    float Width = Score_Width * Score_Factor;
    float Height = Score_Height * Score_Factor;
    float Size;
    float Start_X = (int)(WIDTH - Width) / 2;
    float Bottom = START_SCORE_Y - (int)Height / 2;
    float Top = Bottom + Height;
    Rect *p;
    for (int i = Score_Length - 1; i >= 0; i--) {
        p = &Rct_Score[i];
        p->Left = Start_X;
        Size = Img_Score[Score_Array[i]].w * Score_Factor;
        p->Right = p->Left + Size;
        Start_X += Size;
        p->Bottom = Bottom;
        p->Top = Top;
    }
}

void Init_Img_Combo() {
    char Str[50];
    Image Img_Full, Img_Crop;
    Load_Texture(&Img_Full, "Fonts/Combo.png");
    FILE *f = fopen("Fonts/Combo.fnt", "r");
    int i, n;
    int Id, x, y, w, h, xo, yo, wf, hf;
    fscanf(f, "%d %d", &n, &hf);
    for (i = 0; i < n; i++) {
        fscanf(f, "%d %d %d %d %d %d %d %d", &Id, &x, &y, &w, &h, &xo, &yo, &wf);
        Crop_Image(&Img_Full, &Img_Crop, x, y, w, h);
        Create_Image(&Img_Combo[Id], wf, hf);
        Mix_Image(&Img_Combo[Id], &Img_Crop, xo, yo);
        Delete_Image(&Img_Crop);
    }
    Combo_Height = (float)hf;
    Delete_Image(&Img_Full);
    Combo_Bottom = START_COMBO_Y - (int)Combo_Height / 2;
    Combo_Top = Combo_Bottom + Combo_Height;
}

void Update_Combo() {
    int x = Combo;
    Combo_Length = 0;
    int Size = 0, Size_s;
    do {
        Combo_Array[Combo_Length] = x % 10;
        Size += Img_Combo[Combo_Array[Combo_Length]].w;
        x /= 10;
        Combo_Length++;
    } while (x > 0);
    Combo_Array[Combo_Length] = 10;
    Size += Img_Combo[10].w;
    Combo_Length++;
    Combo_Width = (float)Size;
    float Start_X = (int)(WIDTH - Combo_Width) / 2;
    Rect *p;
    for (int i = Combo_Length - 1; i >= 0; i--) {
        p = &Rct_Combo[i];
        p->Left = Start_X;
        Size = Img_Combo[Combo_Array[i]].w;
        p->Right = p->Left + Size;
        Start_X += Size;
        p->Bottom = Combo_Bottom;
        p->Top = Combo_Top;
    }
}

// Other

bool Check_In_Rect(Rect *Rct, int x, int y) {
    return (x > Rct->Left && x < Rct->Right && y > Rct->Bottom && y < Rct->Top);
}

bool Check_Can_Put(int Cell_X, int Cell_Y, int x, int y) {
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

bool Check_Game_Over() {
    c_Cell *p;
    bool Check;
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            p = &Map[i][j];
            Check = false;
            for (int k = 0; k < 3; k++)
                if (Spawn_Ring.Ring_Value[k] && p->Ring_Value[k]) {
                    Check = true;
                    break;
                }
            if (!Check)
                return false;
        }
    }
    return true;
}

void Init_Game_Over() {
    float s = 0.0f, v = 0.0f, g = 1.4f;
    float S[71], Max = HEIGHT / 2.0f - 20.0f;
    float Left = (WIDTH - Img_Game_Over.w) / 2.0f;
    float Right = Left + Img_Game_Over.w;
    int i = 0;
    float Friction = 0.5f;
    while (i < 71) {
        s += v;
        v += g;
        if (s > Max) {
            s = Max;
            v -= g;
            v *= -Friction;
        }
        S[i] = s;
        i++;
    }
    Rect *p;
    for (i = 0; i < 71; i++) {
        p = &Rct_Game_Over[i];
        p->Left = Left;
        p->Right = Right;
        p->Bottom = (int)S[i] - Img_Game_Over.h / 2;
        p->Top = p->Bottom + Img_Game_Over.h;
    }
}

void Change_Skin() {
    Img_Dot = &Img_Dot_Full[Skin];
    for (int i = 0; i < 3; i++)
        Img_Ring[i] = &Img_Ring_Full[Skin][i];

    float Dot_Size = (float)Img_Dot->w;
    float Dot_Offset = Dot_Size / 2;
    for (int i = 0; i < 3; i++) {
        Ring_Size[i] = (float)Img_Ring[i]->w;
        Ring_Offset[i] = Ring_Size[i] / 2;
    }

    Spawn_Ring.Reload_Pos(Spawn_Ring.x, Spawn_Ring.y);

    Rect *p;
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
}

void Reload_Menu() {
    Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Y);

    Max_Color = 3;
    Game_Count = 0;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                Map[i][j].Ring_Value[k] = 0;

    int Color[] = {1, 4, 5};
    Map[0][0].Ring_Value[0] = Color[0];
    Map[0][1].Ring_Value[2] = Color[1];
    Map[0][2].Ring_Value[1] = Color[0];
    Map[2][0].Ring_Value[0] = Color[0];
    Map[2][0].Ring_Value[1] = Color[0];
    Map[2][1].Ring_Value[1] = Color[1];
    Map[2][2].Ring_Value[2] = Color[0];
    Map[1][0].Ring_Value[2] = Color[2];
    Map[1][0].Ring_Value[0] = Color[2];
    Map[1][2].Ring_Value[0] = Color[2];
    Map[1][2].Ring_Value[1] = Color[2];

    Spawn_Ring.Ring_Value[0] = Color[0];
    Spawn_Ring.Ring_Value[1] = Color[1];
    Spawn_Ring.Ring_Value[2] = Color[2];

    glutMouseFunc(Mouse_Menu);

    Game_State = STATE_MENU;
}

void Reload_Play() {
    Score = 0;
    Score_Factor = 1.0f;
    Combo = 0;
    Update_Combo();
    Update_Score();
    Update_Rect_Score();
    Is_Update_Score = false;
    glutMouseFunc(Mouse_Game);
}

void Init_Game() {
    srand(time(NULL));
    Load_Texture(&Img_Logo, "Images/Logo.png");
    Rct_Logo.Left = (WIDTH - Img_Logo.w) / 2.0f;
    Rct_Logo.Right = Rct_Logo.Left + Img_Logo.w;
    Rct_Logo.Bottom = START_LOGO_Y - Img_Logo.h / 2;
    Rct_Logo.Top = Rct_Logo.Bottom + Img_Logo.h;
    Load_Texture(&Img_Home, "Images/Home.png");
    Rct_Home.Left = WIDTH - 80.0f;
    Rct_Home.Right = Rct_Home.Left + Img_Home.w;
    Rct_Home.Bottom = 32.0f;
    Rct_Home.Top = Rct_Home.Bottom + Img_Home.h;
    Load_Texture(&Img_Line, "Images/Line.png");
    char Str[40];
    for (int i = 0; i < MAX_RING_SKIN; i++) {
        sprintf(Str, "Images/%s_Dot.png", Skin_Text[i]);
        Load_Texture(&Img_Dot_Full[i], Str);
        sprintf(Str, "Images/%s_Small.png", Skin_Text[i]);
        Load_Texture(&Img_Ring_Full[i][0], Str);
        sprintf(Str, "Images/%s_Medium.png", Skin_Text[i]);
        Load_Texture(&Img_Ring_Full[i][1], Str);
        sprintf(Str, "Images/%s_Large.png", Skin_Text[i]);
        Load_Texture(&Img_Ring_Full[i][2], Str);
    }
    Load_Texture(&Img_Game_Over, "Images/Game_Over.png");
    Init_Effect();
    Init_Img_Score();
    Init_Img_Combo();
    Init_Arrow();
    Start_X = (WIDTH - (MAX_X - 1) * SPACE_SIZE) / 2;
    Start_Y = START_Y;
    Start_X_Rct = Start_X - SPACE_SIZE / 2;
    Start_Y_Rct = Start_Y - SPACE_SIZE / 2;

    Spawn_Ring_X = WIDTH / 2.0f;
    Spawn_Ring_Y = SPAWN_RING_Y;
    Spawn_Ring_Begin = SPAWN_RING_Y_BEGIN;
    Spawn_Ring_Offset = (SPAWN_RING_Y_BEGIN - SPAWN_RING_Y) / MAX_SPAWN_RING_TIME;

    Arrow_Up_Timer = 0;
    Arrow_Up_Current = 0;
    for (int i = 0; i < 6; i++) {
        Arrow_Up_Alpha[i] = 0;
    }
    Arrow_LR_Stt = 0;
    Arrow_LR_Timer = 0;

    Init_Game_Over();
    Init_Sound();

    Change_Skin();
}

void Init_GL() {
    glClearColor(BG_Color[0], BG_Color[1], BG_Color[2], 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WIDTH, HEIGHT);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    glEnable(GL_POINT_SMOOTH);
    //    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    //    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //    glEnable(GL_POLYGON_SMOOTH);
    //    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
}
