#include "main.h"

int Find_In_List(int &Color) {
    for (int i = 0; i < Match_Ring_Count; i++)
        if (Color == Match_Ring[i].Color)
            return i;
    return -1;
}

void Push_Front(int &x, int &y) {
    c_Cell *p;
    int Index;
    p = &Map[y][x];
    for (int i = 0; i < Match_Ring_Count; i++)
        Match_Ring[i].Is_Add = false;
    for (int i = 0; i < 3; i++) {
        Index = Find_In_List(p->Ring_Value[i]);
        if (Index > -1) {
            if (!Match_Ring[Index].Is_Add) {
                Match_Ring[Index].Is_Add = true;
                Match_Ring[Index].Count++;
            }
            Match_Ring[Index].Lst.push_front(c_Save_Ring(x, y, i, Match_Ring[Index].Color));
        }
    }
}

void Push_Back(int &x, int &y) {
    c_Cell *p;
    int Index;
    p = &Map[y][x];
    for (int i = 0; i < Match_Ring_Count; i++)
        Match_Ring[i].Is_Add = false;
    for (int i = 0; i < 3; i++) {
        Index = Find_In_List(p->Ring_Value[i]);
        if (Index > -1) {
            if (!Match_Ring[Index].Is_Add) {
                Match_Ring[Index].Is_Add = true;
                Match_Ring[Index].Count++;
            }
            Match_Ring[Index].Lst.push_back(c_Save_Ring(x, y, i, Match_Ring[Index].Color));
        }
    }
}

void Find_Matching_SW_NE(int &x, int &y) {
    int xx = x, yy = y;
    while (xx > 0 && yy < Max_Y) {
        xx -= 1;
        yy += 1;
        Push_Front(xx, yy);
    }
    xx = x;
    yy = y;
    while (xx < Max_X && yy > 0) {
        xx += 1;
        yy -= 1;
        Push_Back(xx, yy);
    }
}

void Find_Matching_W_E(int &x, int &y) {
    int xx = x, yy = y;
    while (xx > 0) {
        xx -= 1;
        Push_Front(xx, yy);
    }
    xx = x;
    while (xx < Max_X) {
        xx += 1;
        Push_Back(xx, yy);
    }
}

void Find_Matching_NW_SE(int &x, int &y) {
    int xx = x, yy = y;
    while (xx > 0 && yy > 0) {
        xx -= 1;
        yy -= 1;
        Push_Front(xx, yy);
    }
    xx = x;
    yy = y;
    while (xx < Max_X && yy < Max_Y) {
        xx += 1;
        yy += 1;
        Push_Back(xx, yy);
    }
}

void Find_Matching_N_S(int &x, int &y) {
    int xx = x, yy = y;
    while (yy > 0) {
        yy -= 1;
        Push_Front(xx, yy);
    }
    yy = y;
    while (yy < Max_Y) {
        yy += 1;
        Push_Back(xx, yy);
    }
}

void Find_Matching(int &x, int &y) {
    Match_Ring_Count = 0;
    c_Cell *p = &Map[y][x];
    int Count_Type = 0, Color;
    int Index;
    for (int i = 0; i < 3; i++) {
        Color = p->Ring_Value[i];
        if (Color > 0) {
            Count_Type++;
            Index = Find_In_List(Color);
            if (Index == -1) {
                Match_Ring[Match_Ring_Count].Init(Color, x, y, i);
                Match_Ring_Count++;
            } else {
                Match_Ring[Index].Lst_Save.push_back(c_Save_Ring(x, y, i, Color));
            }
        }
    }

    std::list<c_Save_Ring> Lst;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < Match_Ring_Count; j++)
            Match_Ring[j].Clear();
        Find_Matching_Func[i](x, y);
        for (int j = 0; j < Match_Ring_Count; j++)
            if (Match_Ring[j].Count == 3) {
                List_Line.push_front(c_Line(Match_Ring[j].Lst.front(), Match_Ring[j].Lst.back()));
                Lst.insert(Lst.end(), Match_Ring[j].Lst.begin(), Match_Ring[j].Lst.end());
            }
    }

    if (Count_Type == 3 && Match_Ring_Count == 1 && Lst.empty()) {
        Combo++;
        for (int i = 0; i < 3; i++)
            Lst.push_front(c_Save_Ring(x, y, i, Match_Ring[0].Color));
    }

    Count_List_Spawn_Dot = 0;
    for (std::list<c_Save_Ring>::iterator it = Lst.begin(); it != Lst.end(); it++) {
        List_Zoom_Ring.push_front(c_Zoom_Ring(it->x, it->y, it->Type, it->Color));
        if (Find_In_List_Spawn_Dot(it->x, it->y) == -1) {
            List_Spawn_Dot[Count_List_Spawn_Dot].Init(it->x, it->y, it->Color);
            Count_List_Spawn_Dot++;
        }
        Map[it->y][it->x].Ring_Value[it->Type] = 0;
    }

    if (!Lst.empty()) {
        Explode_Dot_Stt = 0;
        Zoom_Ring_Stt = 0;
        Line_Stt = 0;
        Game_State = STATE_RING_EXPLODE;
        Combo += List_Line.size();
        if (Combo >= 2)
            Update_Combo();
        Score += Lst.size() * Combo;
        Update_Score();
        Is_Update_Score = true;
        Score_Time = 0;
    } else {
        Combo = 0;
        Spawn_Ring_Stt = 0;
        Game_State = STATE_RING_SPAWN;
    }
}
