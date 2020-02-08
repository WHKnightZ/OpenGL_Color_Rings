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
            Match_Ring[Index].Lst.push_front(c_Save_Ring(x, y, i));
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
            Match_Ring[Index].Lst.push_back(c_Save_Ring(x, y, i));
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
                Match_Ring[Index].Lst_Save.push_back(c_Save_Ring(x, y, i));
            }
        }
    }

    std::list<c_Save_Ring> Lst;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < Match_Ring_Count; j++)
            Match_Ring[j].Clear();
        Find_Matching_Func[i](x, y);
        for (int j = 0; j < Match_Ring_Count; j++)
            if (Match_Ring[j].Count == 3)
                Lst.insert(Lst.end(), Match_Ring[j].Lst.begin(), Match_Ring[j].Lst.end());
    }

    if (Count_Type == 3 && Match_Ring_Count == 1 && Lst.empty()) {
        for (int i = 0; i < 3; i++)
            Lst.push_front(c_Save_Ring(x, y, i));
    }

    for (std::list<c_Save_Ring>::iterator it = Lst.begin(); it != Lst.end(); it++)
        Map[it->y][it->x].Ring_Value[it->Type] = 0;
}
