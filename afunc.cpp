#include "main.h"

void Draw_Board() {
    c_Cell *p;
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            p = &Map[i][j];
            glColor3fv(Color_White);
            Map_Texture(&Img_Dot);
            Draw_Rect(&p->Rct_Dot);
            for (int k = 0; k < 3; k++) {
                if (p->Ring_Value[k] != 0) {
                    glColor3fv(Ring_Color[p->Ring_Value[k]]);
                    Map_Texture(&Img_Ring[k]);
                    Draw_Rect(&p->Rct_Ring[k]);
                }
            }
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
                Game_State = GAME_STT_RING_SPAWN;
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

void Game_Display_Play() {
    Draw_Board();
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(&Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
}

void Game_Display_Ring_Explode() {
    Draw_Board();
    for (std::list<c_Zoom_Ring>::iterator it = List_Zoom_Ring.begin(); it != List_Zoom_Ring.end(); it++) {
        Map_Texture(&Img_Ring[it->Type]);
        glColor3fv(Ring_Color[it->Color]);
        Draw_Rect(&it->Rct);
    }
    Map_Texture(&Img_Dot);
    for (std::list<c_Explode_Dot>::iterator it = List_Explode_Dot.begin(); it != List_Explode_Dot.end(); it++) {
        glColor3fv(Ring_Color[it->Color]);
        Draw_Rect(&it->Rct);
    }
    Map_Texture(&Img_Line);
    for (std::list<c_Line>::iterator it = List_Line.begin(); it != List_Line.end(); it++)
        it->Draw();
}

void Game_Display_Ring_Spawn() {
    Draw_Board();
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(&Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
}

// Process

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
        Game_State = GAME_STT_PLAY;
    }
}