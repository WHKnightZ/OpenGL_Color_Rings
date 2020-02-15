#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game_Display_Func[Game_State]();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Timer(int value) {
    Game_Process_Func[Game_State]();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Mouse_Menu(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (Check_In_Rect(&Spawn_Ring.Rct_Check, x, y)) {
                Is_Hold_Mouse = true;
                Spawn_Ring.Reload_Pos(x, y);
            } else if (Check_In_Rect(&Rct_Check_Arrow_Left, x, y)) {
                Skin--;
                if (Skin < 0)
                    Skin = MAX_RING_SKIN - 1;
                Change_Skin();
            } else if (Check_In_Rect(&Rct_Check_Arrow_Right, x, y)) {
                Skin++;
                if (Skin == MAX_RING_SKIN)
                    Skin = 0;
                Change_Skin();
            }
        } else if (Is_Hold_Mouse) {
            Mix_PlayChannel(-1, Sound_Tick, 0);
            int a = (x - Start_X_Rct) / SPACE_SIZE;
            int b = (y - Start_Y_Rct) / SPACE_SIZE;
            if (Check_Can_Put(a, b, x, y)) {
                Is_Put = true;
                Game_Count++;
                if (Game_Count % COUNT_TO_INCREASE_COLOR == 0 && Max_Color < MAX_COLOR)
                    Max_Color++;
                Dest_X = a;
                Dest_Y = b;
                Move_Dest_X = Start_X + a * SPACE_SIZE;
                Move_Dest_Y = Start_Y + b * SPACE_SIZE;
            } else {
                Is_Put = false;
                Move_Dest_X = Spawn_Ring_X;
                Move_Dest_Y = Spawn_Ring_Y;
            }
            float Offset_X = Move_Dest_X - x;
            float Offset_Y = Move_Dest_Y - y;
            float Distance = sqrt(Offset_X * Offset_X + Offset_Y * Offset_Y);
            if (Distance < 3.0f) // Distance must be different from zero
                Distance = 3.0f;
            float Velocity = MOVE_VELOCITY_BASE;
            if (Distance > 150.0f)
                Velocity *= 2;
            Move_Velocity_X = Velocity * Offset_X / Distance;
            Move_Velocity_Y = Velocity * Offset_Y / Distance;
            Move_Timer = 0;
            Move_Max = (int)(Distance / Velocity + 0.99f);
            Is_Move_Ring = true;
            Is_Hold_Mouse = false;
        }
    }
}

void Mouse_Game(int button, int state, int x, int y) {
    if (Game_State == STATE_PLAY && button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (Check_In_Rect(&Spawn_Ring.Rct_Check, x, y)) {
                Is_Hold_Mouse = true;
                Spawn_Ring.Reload_Pos(x, y);
            }
        } else if (Is_Hold_Mouse) {
            Mix_PlayChannel(-1, Sound_Tick, 0);
            int a = (x - Start_X_Rct) / SPACE_SIZE;
            int b = (y - Start_Y_Rct) / SPACE_SIZE;
            if (Check_Can_Put(a, b, x, y)) {
                Is_Put = true;
                Game_Count++;
                if (Game_Count % COUNT_TO_INCREASE_COLOR == 0 && Max_Color < MAX_COLOR)
                    Max_Color++;
                Dest_X = a;
                Dest_Y = b;
                Move_Dest_X = Start_X + a * SPACE_SIZE;
                Move_Dest_Y = Start_Y + b * SPACE_SIZE;
            } else {
                Is_Put = false;
                Move_Dest_X = Spawn_Ring_X;
                Move_Dest_Y = Spawn_Ring_Y;
            }
            float Offset_X = Move_Dest_X - x;
            float Offset_Y = Move_Dest_Y - y;
            float Distance = sqrt(Offset_X * Offset_X + Offset_Y * Offset_Y);
            if (Distance < 3.0f) // Distance must be different from zero
                Distance = 3.0f;
            float Velocity = MOVE_VELOCITY_BASE;
            if (Distance > 150.0f)
                Velocity *= 2;
            Move_Velocity_X = Velocity * Offset_X / Distance;
            Move_Velocity_Y = Velocity * Offset_Y / Distance;
            Move_Timer = 0;
            Move_Max = (int)(Distance / Velocity + 0.99f);
            Is_Move_Ring = true;
            Is_Hold_Mouse = false;
        }
    }
    if (Check_In_Rect(&Rct_Home, x, y)) {
        Reload_Menu();
    }
}

void Motion(int x, int y) {
    if (Is_Hold_Mouse) {
        Spawn_Ring.Reload_Pos(x, y);
    }
}

int main(int argc, char **argv) {
    Init_Game();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Color Rings");
    Init_GL();
    Reload_Menu();
    glutDisplayFunc(Display);
    glutMotionFunc(Motion);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
