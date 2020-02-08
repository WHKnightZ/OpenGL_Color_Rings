#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
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
    for (int i = 0; i < 3; i++) {
        if (Spawn_Ring.Ring_Value[i] != 0) {
            glColor3fv(Ring_Color[Spawn_Ring.Ring_Value[i]]);
            Map_Texture(&Img_Ring[i]);
            Draw_Rect(&Spawn_Ring.Rct_Ring[i]);
        }
    }
    glLoadIdentity();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Timer(int value) {
    if (Is_Hold_Mouse)
        glutPostRedisplay();
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
            }
            Spawn_Ring.Reload_Pos(Spawn_Ring_X, Spawn_Ring_Y);
            Is_Move_Ring = false;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (Check_In_Rect(&Spawn_Ring.Rct_Check, x, y)) {
                Is_Hold_Mouse = true;
                Spawn_Ring.Reload_Pos(x, y);
            }
        } else if (Is_Hold_Mouse) {
            int a = (x - Start_X_Rct) / SPACE_SIZE;
            int b = (y - Start_Y_Rct) / SPACE_SIZE;
            if (Check_Can_Put(a, b, x, y)) {
                Is_Put = true;
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
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
