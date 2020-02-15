#include "main.h"

void Draw_Arrow() {
    Map_Texture(&Img_Arrow_Up);
    for (int i = 0; i < 6; i++) {
        if (Arrow_Up_Alpha[i] > 0) {
            glColor4f(1.0f, 1.0f, 1.0f, Arrow_Up_Alpha[i] / 100.0f);
            Draw_Rect(&Rct_Arrow_Up[i]);
        }
    }
    glColor3fv(Color_White);
    Map_Texture(&Img_Arrow_Left);
    Draw_Rect(&Rct_Arrow_Left[Arrow_LR_Stt]);
    Map_Texture(&Img_Arrow_Right);
    Draw_Rect(&Rct_Arrow_Right[Arrow_LR_Stt]);
}

void Draw_Board() {
    c_Cell *p;
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            p = &Map[i][j];
            glColor3fv(Color_White);
            Map_Texture(Img_Dot);
            Draw_Rect(&p->Rct_Dot);
            for (int k = 0; k < 3; k++) {
                if (p->Ring_Value[k] != 0) {
                    glColor3fv(Ring_Color[p->Ring_Value[k]]);
                    Map_Texture(Img_Ring[k]);
                    Draw_Rect(&p->Rct_Ring[k]);
                }
            }
        }
    }
}

void Draw_Score() {
    glColor3fv(Color_White);
    for (int i = 0; i < Score_Length; i++) {
        Map_Texture(&Img_Score[Score_Array[i]]);
        Draw_Rect(&Rct_Score[i]);
    }
}

void Draw_Combo() {
    if (Combo >= 2) {
        for (int i = 0; i < Combo_Length; i++) {
            Map_Texture(&Img_Combo[Combo_Array[i]]);
            Draw_Rect(&Rct_Combo[i]);
        }
    }
}

void Ring_Explode_Timer() {
    std::list<c_Explode_Dot>::iterator it = List_Explode_Dot.begin();
    bool End;
    while (it != List_Explode_Dot.end()) {
        End = false;
        it->x += it->vx;
        it->y += it->vy;
        it->Reload();
        if (it->Stt == Explode_Dot_Radius_Center) {
            it->Move++;
            if (it->Move == it->Max)
                it->Stt++;
        } else {
            it->Stt++;
            if (it->Stt == Explode_Dot_Radius_Max)
                End = true;
        }
        if (End) {
            List_Explode_Dot.erase(it++);
            if (List_Explode_Dot.empty()) {
                Spawn_Ring_Stt = 0;
                Game_State = STATE_RING_SPAWN;
            }
        } else
            it++;
    }
    if (Explode_Dot_Stt < Explode_Dot_Max) {
        for (int i = 0; i < Explode_Dot_Full[Explode_Dot_Stt]; i++) {
            for (int j = 0; j < Count_List_Spawn_Dot; j++) {
                c_Spawn_Dot *p = &List_Spawn_Dot[j];
                List_Explode_Dot.push_front(c_Explode_Dot(p->x, p->y, Explode_Dot_Stt, i, p->Color));
            }
        }
        Explode_Dot_Stt++;
    }
}

// Display

void Game_Display_Menu() {
    Draw_Board();
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
    Draw_Arrow();
    Map_Texture(&Img_Logo);
    Draw_Rect(&Rct_Logo);
}

void Game_Display_Play() {
    Draw_Board();
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
    Draw_Score();
    Draw_Combo();
    Map_Texture(&Img_Home);
    Draw_Rect(&Rct_Home);
}

void Game_Display_Ring_Explode() {
    Draw_Board();
    for (std::list<c_Zoom_Ring>::iterator it = List_Zoom_Ring.begin(); it != List_Zoom_Ring.end(); it++) {
        Map_Texture(Img_Ring[it->Type]);
        glColor3fv(Ring_Color[it->Color]);
        Draw_Rect(&it->Rct);
    }
    Map_Texture(Img_Dot);
    for (std::list<c_Explode_Dot>::iterator it = List_Explode_Dot.begin(); it != List_Explode_Dot.end(); it++) {
        glColor3fv(Ring_Color[it->Color]);
        Draw_Rect(&it->Rct);
    }
    Map_Texture(&Img_Line);
    for (std::list<c_Line>::iterator it = List_Line.begin(); it != List_Line.end(); it++)
        it->Draw();
    Draw_Score();
    Draw_Combo();
    Map_Texture(&Img_Home);
    Draw_Rect(&Rct_Home);
}

void Game_Display_Ring_Spawn() {
    Draw_Board();
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
    Draw_Score();
    Draw_Combo();
    Map_Texture(&Img_Home);
    Draw_Rect(&Rct_Home);
}

void Game_Display_GameOver() {
    Game_Display_Play();
    glColor3fv(Color_White);
    Map_Texture(&Img_Game_Over);
    Draw_Rect(&Rct_Game_Over[Game_Over_Stt]);
    Map_Texture(&Img_Home);
    Draw_Rect(&Rct_Home);
}

// Process

void Game_Process_Menu() {
    for (int i = 0; i < 6; i++) {
        if (Arrow_Up_Alpha[i] > 0)
            Arrow_Up_Alpha[i] -= 4;
    }
    Arrow_Up_Timer++;
    if (Arrow_Up_Timer == 4) {
        Arrow_Up_Timer = 0;
        Arrow_Up_Alpha[Arrow_Up_Current] = 100;
        Arrow_Up_Current++;
        if (Arrow_Up_Current == 6) {
            Arrow_Up_Current = 0;
            Arrow_Up_Timer = -30;
        }
    }
    Arrow_LR_Timer++;
    if (Arrow_LR_Timer == 2) {
        Arrow_LR_Timer = 0;
        Arrow_LR_Stt++;
        if (Arrow_LR_Stt == 20)
            Arrow_LR_Stt = 0;
    }
    if (Is_Move_Ring && Move_Timer < Move_Max) {
        Move_Timer++;
        if (Move_Timer < Move_Max) {
            float x = Spawn_Ring.x + Move_Velocity_X;
            float y = Spawn_Ring.y + Move_Velocity_Y;
            Spawn_Ring.Reload_Pos(x, y);
        } else {
            if (Is_Put) {
                for (int i = 0; i < 3; i++)
                    if (Spawn_Ring.Ring_Value[i])
                        Map[Dest_Y][Dest_X].Ring_Value[i] = Spawn_Ring.Ring_Value[i];
                Find_Matching(Dest_X, Dest_Y);
                Reload_Play();
                Spawn_Ring.Reload_Value();
                Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Begin);
            } else
                Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Y);
            Is_Move_Ring = false;
        }
    }
    glutPostRedisplay();
}

void Game_Process_Play() {
    bool Redraw = false;
    if (Is_Hold_Mouse)
        Redraw = true;
    if (Is_Move_Ring && Move_Timer < Move_Max) {
        Move_Timer++;
        if (Move_Timer < Move_Max) {
            float x = Spawn_Ring.x + Move_Velocity_X;
            float y = Spawn_Ring.y + Move_Velocity_Y;
            Spawn_Ring.Reload_Pos(x, y);
        } else {
            if (Is_Put) {
                for (int i = 0; i < 3; i++)
                    if (Spawn_Ring.Ring_Value[i])
                        Map[Dest_Y][Dest_X].Ring_Value[i] = Spawn_Ring.Ring_Value[i];
                Find_Matching(Dest_X, Dest_Y);
                Spawn_Ring.Reload_Value();
                Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Begin);
            } else
                Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Y);
            Is_Move_Ring = false;
        }
        Redraw = true;
    }
    if (Redraw)
        glutPostRedisplay();
}

void Game_Process_Ring_Explode() {
    Ring_Explode_Timer();
    if (Zoom_Ring_Stt < MAX_ZOOM_TIME) {
        for (std::list<c_Zoom_Ring>::iterator it = List_Zoom_Ring.begin(); it != List_Zoom_Ring.end(); it++)
            it->Reload();
        Zoom_Ring_Stt++;
        if (Zoom_Ring_Stt == MAX_ZOOM_TIME)
            List_Zoom_Ring.clear();
    }
    if (Line_Stt < MAX_LINE_TIME) {
        for (std::list<c_Line>::iterator it = List_Line.begin(); it != List_Line.end(); it++)
            it->Reload();
        Line_Stt++;
        if (Line_Stt == MAX_LINE_TIME)
            List_Line.clear();
    }
    if (Is_Update_Score) {
        Score_Time++;
        if (Score_Time < MAX_SCORE_TIME) {
            Score_Factor = Score_Factor_Init[Score_Time];
        } else {
            Score_Factor = 1.0f;
            Is_Update_Score = false;
        }
        Update_Rect_Score();
    }
    glutPostRedisplay();
}

void Game_Process_Ring_Spawn() {
    if (Spawn_Ring_Stt < MAX_SPAWN_RING_TIME) {
        Spawn_Ring_Stt++;
        float x = Spawn_Ring.x;
        float y = Spawn_Ring.y - Spawn_Ring_Offset;
        Spawn_Ring.Reload_Pos(x, y);
        glutPostRedisplay();
    } else {
        if (Check_Game_Over()) {
            Mix_PlayChannel(-1, Sound_Game_Over, 0);
            Game_Over_Stt = 0;
            Game_State = STATE_GAMEOVER;
        } else
            Game_State = STATE_PLAY;
    }
}

void Game_Process_GameOver() {
    if (Game_Over_Stt < 70) {
        Game_Over_Stt++;
    }
    glutPostRedisplay();
}
