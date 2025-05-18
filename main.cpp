#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>

#define MAX_GA 3
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Lo?i tr?ng
#define TRUNG_THUONG 0
#define TRUNG_VANG   1
#define TRUNG_BOM    2

struct Egg {
    int x, y;
    int speed;
    int active;
    int type;
};

struct Chicken {
    int x;
    Egg egg;
};

void drawBasket(int x) {
    setcolor(WHITE);
    rectangle(x, 440, x + 80, 460);
}

void drawEgg(Egg e) {
    switch (e.type) {
        case TRUNG_THUONG:
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            break;
        case TRUNG_VANG:
            setcolor(LIGHTCYAN);
            setfillstyle(SOLID_FILL, LIGHTCYAN);
            break;
        case TRUNG_BOM:
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            break;
    }
    fillellipse(e.x, e.y, 10, 15);
}

void drawChicken(int x) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    circle(x, 50, 15); // d?u
    fillellipse(x, 50, 15, 15);
    setcolor(RED);
    circle(x, 65, 5); // m?
}

void resetEgg(Egg *egg, int chickenX) {
    egg->x = chickenX;
    egg->y = 70;
    egg->speed = 2 + rand() % 3;
    egg->type = rand() % 10 == 0 ? TRUNG_VANG : (rand() % 8 == 0 ? TRUNG_BOM : TRUNG_THUONG);
    egg->active = 1;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int basketX = 300;
    int ch, score = 0, lives = 3;

    Chicken chickens[MAX_GA];
    srand(time(NULL));

    for (int i = 0; i < MAX_GA; i++) {
        chickens[i].x = 100 + i * 200;
        resetEgg(&chickens[i].egg, chickens[i].x);
    }

    while (1) {
        if (kbhit()) {
            ch = getch();
            if (ch == 27) break; // ESC d? thoát
            if (ch == 0) {
                ch = getch();
                if (ch == 75 && basketX > 10) basketX -= 20;
                if (ch == 77 && basketX < SCREEN_WIDTH - 90) basketX += 20;
            }
        }

        cleardevice();
        drawBasket(basketX);

        for (int i = 0; i < MAX_GA; i++) {
            drawChicken(chickens[i].x);

            Egg *e = &chickens[i].egg;
            if (e->active) {
                drawEgg(*e);
                e->y += e->speed;

                // H?ng tr?ng
                if (e->y >= 440 && e->y <= 460 &&
                    e->x >= basketX && e->x <= basketX + 80) {

                    switch (e->type) {
                        case TRUNG_THUONG: score += 1; break;
                        case TRUNG_VANG:   score += 3; break;
                        case TRUNG_BOM:    lives--; break;
                    }
delay(50); // ho?c gi? nguyên delay

                    resetEgg(e, chickens[i].x);
                }

                // Roi xu?ng d?t
                else if (e->y > SCREEN_HEIGHT) {
                    if (e->type != TRUNG_BOM) {
                        lives--;delay(50); // ho?c gi? nguyên delay

                    }
                    resetEgg(e, chickens[i].x);
                }
            }
        }

        // Ði?m và m?ng
        setcolor(WHITE);
        char status[50];
        sprintf(status, "Score: %d   Lives: %d", score, lives);
        outtextxy(10, 10, status);

        if (lives <= 0) {
            cleardevice();
            setcolor(RED);
            outtextxy(250, 200, "GAME OVER");
            char msg[50];
            sprintf(msg, "Final Score: %d", score);
            outtextxy(250, 230, msg);
            getch();
            break;
        }

        delay(50);
    }

    closegraph();
    return 0;
}

