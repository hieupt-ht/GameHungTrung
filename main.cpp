#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <mmsystem.h> // Th� vi?n cho �m thanh
#pragma comment(lib, "winmm.lib") // Link th� vi?n Windows Multimedia
#include<math.h>
#define MAX_GA 5
#define MAX_ACTIVE_EGGS 2  // Gi?i h?n s? l�?ng tr?ng r�i c�ng l�c
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Lo?i tr?ng
#define TRUNG_THUONG 0
#define TRUNG_VANG   1
#define TRUNG_BOM    2
#define TRUNG_CHAM   3  // Tr?ng l�m ch?m
#define TRUNG_NAM_CHAM 4  // Tr?ng nam ch�m
// C?u tr�c cho tr?ng
struct Egg {
    int x, y;
    int speed;
    int active;
    int type;
    int width;
    int height;
};

// C?u tr�c cho g�
struct Chicken {
    int x;
    Egg egg;
    int direction; // H�?ng di chuy?n c?a g�
    int moveTimer; // �?m th?i gian �? �?i h�?ng
};

// C?u tr�c cho r? h?ng
// C?u tr�c cho r? h?ng
struct Basket {
    int x, y;
    int width, height;
    int slowEffect;       // Th?i gian hi?u ?ng l�m ch?m c?n l?i
    int magnetEffect;     // Th?i gian hi?u ?ng nam ch�m c?n l?i
    int magnetRadius;     // B�n k�nh h�t c?a nam ch�m
};

// C?u tr�c cho h?nh ?nh n?n
struct Background {
    int mountainHeight;
    int grassHeight;
    int skyColor;
    int grassColor;
    int mountainColor;
};

// H�m �?i m�u s?c ng?u nhi�n cho g�
void randomChickenColor(int *color) {
    int colors[] = {WHITE, LIGHTGRAY, LIGHTCYAN, LIGHTRED, YELLOW};
    *color = colors[rand() % 5];
}

// V? n?n tr?i, n�i v� c?
void drawBackground(Background bg) {
    // V? b?u tr?i
    setfillstyle(SOLID_FILL, bg.skyColor);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight);
    
    // V? n�i
    setfillstyle(SOLID_FILL, bg.mountainColor);
    int mountainPoints[] = {
        100, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight,
        200, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/2,
        300, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight,
        400, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.5,
        500, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight,
        600, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.2,
        640, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/3,
        640, SCREEN_HEIGHT - bg.grassHeight,
        0, SCREEN_HEIGHT - bg.grassHeight,
        0, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight
    };
    fillpoly(10, mountainPoints);
    
    // V? tuy?t tr�n �?nh n�i
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    int snowPoints[] = {
        150, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.2,
        200, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/2,
        250, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.2
    };
    fillpoly(3, snowPoints);
    
    int snowPoints2[] = {
        430, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.7,
        400, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.5,
        370, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight/1.7
    };
    fillpoly(3, snowPoints2);
    
    // V? c?
    setfillstyle(SOLID_FILL, bg.grassColor);
    bar(0, SCREEN_HEIGHT - bg.grassHeight, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // V? c�y th�ng b�n tr�i
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, BROWN);
    int tree1[] = {
        50, SCREEN_HEIGHT - bg.grassHeight - 100,
        80, SCREEN_HEIGHT - bg.grassHeight - 150,
        110, SCREEN_HEIGHT - bg.grassHeight - 100
    };
    fillpoly(3, tree1);
    
    int tree2[] = {
        50, SCREEN_HEIGHT - bg.grassHeight - 70,
        80, SCREEN_HEIGHT - bg.grassHeight - 120,
        110, SCREEN_HEIGHT - bg.grassHeight - 70
    };
    fillpoly(3, tree2);
    
    int tree3[] = {
        50, SCREEN_HEIGHT - bg.grassHeight - 40,
        80, SCREEN_HEIGHT - bg.grassHeight - 90,
        110, SCREEN_HEIGHT - bg.grassHeight - 40
    };
    fillpoly(3, tree3);
    
    // Th�n c�y
    setfillstyle(SOLID_FILL, BROWN);
    bar(75, SCREEN_HEIGHT - bg.grassHeight - 40, 85, SCREEN_HEIGHT - bg.grassHeight);
    
    // V? c�y l� r?ng b�n ph?i
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(550, SCREEN_HEIGHT - bg.grassHeight - 80, 40, 60);
    setfillstyle(SOLID_FILL, BROWN);
    bar(545, SCREEN_HEIGHT - bg.grassHeight - 30, 555, SCREEN_HEIGHT - bg.grassHeight);
    
    // V? d�y �i?n
    setcolor(BLACK);
    line(0, 80, SCREEN_WIDTH, 80);
}

// V? r? h?ng
void drawBasket(Basket basket) {
    // V? r?
    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    
    // V? h?nh n?a h?nh tr?n l?t ng�?c
    arc(basket.x + basket.width/2, basket.y, 0, 180, basket.width/2);
    line(basket.x, basket.y, basket.x, basket.y + basket.height);
    line(basket.x + basket.width, basket.y, basket.x + basket.width, basket.y + basket.height);
    line(basket.x, basket.y + basket.height, basket.x + basket.width, basket.y + basket.height);
    
    // V? c�c ��?ng ngang
    setcolor(BROWN);
    for (int i = 1; i < 3; i++) {
        line(basket.x, basket.y + i*(basket.height/3), 
             basket.x + basket.width, basket.y + i*(basket.height/3));
    }
    
    // V? c�c ��?ng d?c
    for (int i = 1; i < 3; i++) {
        line(basket.x + i*(basket.width/3), basket.y, 
             basket.x + i*(basket.width/3), basket.y + basket.height);
    }
    
    floodfill(basket.x + basket.width/2, basket.y + basket.height/2, BROWN);
}

// V? tr?ng v?i c�c lo?i kh�c nhau
// V? tr?ng v?i c�c lo?i kh�c nhau
void drawEgg(Egg e) {
    switch (e.type) {
        case TRUNG_THUONG:
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);
            break;
        case TRUNG_VANG:
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            break;
        case TRUNG_BOM:
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            break;
        case TRUNG_CHAM:
            setcolor(CYAN);
            setfillstyle(SOLID_FILL, CYAN);
            break;
        case TRUNG_NAM_CHAM:
            setcolor(MAGENTA);
            setfillstyle(SOLID_FILL, MAGENTA);
            break;
    }
    
    // V? tr?ng l� h?nh oval
    fillellipse(e.x, e.y, e.width, e.height);
    
    if (e.type == TRUNG_THUONG) {
        // Th�m chi ti?t cho tr?ng th�?ng
        setcolor(LIGHTGRAY);
        arc(e.x, e.y - 3, 0, 180, 5);
    } else if (e.type == TRUNG_VANG) {
        // Th�m chi ti?t cho tr?ng v�ng
        setcolor(WHITE);
        circle(e.x, e.y, 3);
    } else if (e.type == TRUNG_BOM) {
        // Th�m chi ti?t cho tr?ng bom
        setcolor(BLACK);
        line(e.x-5, e.y-5, e.x+5, e.y+5);
        line(e.x+5, e.y-5, e.x-5, e.y+5);
    } else if (e.type == TRUNG_CHAM) {
        // Th�m chi ti?t cho tr?ng l�m ch?m
        setcolor(BLUE);
        circle(e.x, e.y, 3);
        line(e.x-5, e.y, e.x+5, e.y);  // D?u �?ng h? ngang
        line(e.x, e.y-5, e.x, e.y+5);  // ��?ng d?c
    } else if (e.type == TRUNG_NAM_CHAM) {
        // Th�m chi ti?t cho tr?ng nam ch�m
        setcolor(WHITE);
        // V? h?nh nam ch�m ��n gi?n
        arc(e.x, e.y-3, 0, 180, 4);
        line(e.x-4, e.y-3, e.x-4, e.y+3);
        line(e.x+4, e.y-3, e.x+4, e.y+3);
    }
}

// V? g�
void drawChicken(int x, int y, int color) {
    // V? th�n g�
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, 20, 15);
    
    // V? �?u g�
    fillellipse(x - 15, y - 5, 10, 10);
    
    // V? m?t g�
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - 18, y - 8, 2, 2);
    
    // V? m? g�
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    int beakPoints[] = {x - 25, y - 8, x - 30, y - 3, x - 25, y};
    fillpoly(3, beakPoints);
    
    // V? ch�n g�
    setcolor(YELLOW);
    line(x, y + 15, x - 5, y + 25);
    line(x + 5, y + 15, x + 10, y + 25);
    
    // V? m�o g�
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int combPoints[] = {x - 18, y - 13, x - 15, y - 20, x - 10, y - 13};
    fillpoly(3, combPoints);
}

// V? hi?u ?ng tr?ng v?
void drawBrokenEgg(int x, int y) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    
    // V? l?ng tr?ng tr?ng
    fillellipse(x, y + 5, 15, 5);
    
    // V? l?ng �? tr?ng
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x, y + 5, 5, 5);
    
    // V? m?nh v? tr?ng
    setcolor(WHITE);
    arc(x - 10, y, 0, 180, 7);
    arc(x + 10, y, 0, 180, 7);
}

// V? hi?u ?ng n?
void drawExplosion(int x, int y) {
    for (int size = 5; size <= 30; size += 5) {
        setcolor(YELLOW);
        circle(x, y, size);
        setcolor(RED);
        circle(x, y, size-2);
        delay(50);
    }
}

// Kh?i t?o l?i tr?ng khi r�i xu?ng ho?c ��?c h?ng
void resetEgg(Egg *egg, int chickenX, int level) {
    egg->x = chickenX;
    egg->y = 100;
    egg->speed = 2 + rand() % 3 + level/10;
    egg->active = 0;  // M?c �?nh l� kh�ng ho?t �?ng
    egg->width = 8;
    egg->height = 12;
    // Lo?i tr?ng s? ��?c quy?t �?nh khi tr?ng ��?c k�ch ho?t
}

// H�m ki?m tra va ch?m gi?a tr?ng v� r?
int checkCollision(Egg egg, Basket basket) {
    return (egg.y + egg.height >= basket.y && 
            egg.y <= basket.y + basket.height &&
            egg.x + egg.width >= basket.x &&
            egg.x - egg.width <= basket.x + basket.width);
}

// Hi?n th? �i?m v� m?ng s?ng
void displayStatus(int score, int lives, int level, Basket basket) {
    char status[50];
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, 300, 30);
    
    sprintf(status, "Score: %d", score);
    outtextxy(10, 10, status);
    
    sprintf(status, "Lives: %d", lives);
    outtextxy(100, 10, status);
    
    sprintf(status, "Level: %d", level);
    outtextxy(180, 10, status);
    
    // Hi?n th? c�c hi?u ?ng �?c bi?t n?u c�
    if (basket.slowEffect > 0) {
        setcolor(CYAN);
        sprintf(status, "Slow: %d", basket.slowEffect/30);
        outtextxy(250, 10, status);
    }
    
    if (basket.magnetEffect > 0) {
        setcolor(MAGENTA);
        sprintf(status, "Magnet: %d", basket.magnetEffect/30);
        outtextxy(350, 10, status);
    }
    
    // Hi?n th? bi?u t�?ng tr?ng v� s? l�?ng c?n l?i
    for (int i = 0; i < lives; i++) {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(10 + i*20, 30, 5, 7);
    }
}

// H�m �? ph�t hi?u ?ng �m thanh
void playEffectSound(const char* filename) {
    char command[256];
    // ��ng �m thanh hi?u ?ng tr�?c �� n?u c�
    mciSendString("close effect", NULL, 0, NULL);
    
    // T?o l?nh m? file �m thanh m?i
    sprintf(command, "open \"%s\" type waveaudio alias effect", filename);
    mciSendString(command, NULL, 0, NULL);
    
    // Ph�t �m thanh hi?u ?ng
    mciSendString("play effect", NULL, 0, NULL);
}

// H�m v? m�n h?nh b?t �?u
void drawStartScreen() {
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy(120, 100, "CHICKEN & EGGS");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 200, "Di chuyen: Phim mui ten");
    outtextxy(200, 220, "Huy truong: Phim Space");
    outtextxy(200, 240, "Thoat: Phim ESC");
    
    setcolor(LIGHTGREEN);
    outtextxy(180, 300, "Nhan phim bat ky de bat dau...");
    
    // V? h?nh g� v� tr?ng
    for (int i = 0; i < 3; i++) {
        int chickenColor;
        randomChickenColor(&chickenColor);
        drawChicken(100 + i*200, 350, chickenColor);
    }
    
    getch();
}

// H�m v? m�n h?nh k?t th�c
// H�m v? m�n h?nh k?t th�c
void drawGameOverScreen(int score) {
    // �?t l?i trang ho?t �?ng v� hi?n th?
    setactivepage(0);
    setvisualpage(0);
    
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    
    setcolor(RED);
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy(180, 150, "GAME OVER");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char finalScore[50];
    sprintf(finalScore, "Final Score: %d", score);
    outtextxy(220, 220, finalScore);
    
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(180, 280, "Nhan phim bat ky de thoat...");
    
    // V? tr?ng v?
    for (int i = 0; i < 5; i++) {
        drawBrokenEgg(100 + i*100, 350);
    }
    
    // �?m b?o m�n h?nh ��?c hi?n th?
    delay(100);
    getch();
}

// H�m �? ch?n ng?u nhi�n m?t s? l�?ng g� �? th? tr?ng
void selectRandomChickensToLayEggs(Chicken chickens[]) {
    int activeCount = 0;
    
    // �?m s? tr?ng �ang ho?t �?ng
    for (int i = 0; i < MAX_GA; i++) {
        if (chickens[i].egg.active) {
            activeCount++;
        }
    }
    
    // N?u ch�a �? 2 tr?ng ho?t �?ng, th�m tr?ng m?i
    if (activeCount < MAX_ACTIVE_EGGS) {
        // Danh s�ch con g� ch�a c� tr?ng ho?t �?ng
        int inactiveChickens[MAX_GA];
        int inactiveCount = 0;
        
        for (int j = 0; j < MAX_GA; j++) {
            if (!chickens[j].egg.active) {
                inactiveChickens[inactiveCount++] = j;
            }
        }
        
        // N?u c� g� ch�a c� tr?ng ho?t �?ng
        if (inactiveCount > 0) {
            // Ch?n ng?u nhi�n m?t con g�
            int selectedIndex = inactiveChickens[rand() % inactiveCount];
            
            // Quy?t �?nh ng?u nhi�n lo?i tr?ng (c� 5 lo?i)
            int eggType = rand() % 10;  // T? l? xu?t hi?n c�c lo?i tr?ng
            if (eggType < 4)          // 40% tr?ng th�?ng
                chickens[selectedIndex].egg.type = TRUNG_THUONG;
            else if (eggType < 6)     // 20% tr?ng v�ng
                chickens[selectedIndex].egg.type = TRUNG_VANG;
            else if (eggType < 8)     // 20% tr?ng bom
                chickens[selectedIndex].egg.type = TRUNG_BOM;
            else if (eggType < 9)     // 10% tr?ng l�m ch?m
                chickens[selectedIndex].egg.type = TRUNG_CHAM;
            else                      // 10% tr?ng nam ch�m
                chickens[selectedIndex].egg.type = TRUNG_NAM_CHAM;
            
            // K�ch ho?t tr?ng
            chickens[selectedIndex].egg.active = 1;
        }
    }
}
// V? hi?u ?ng nam ch�m n?u �ang ho?t �?ng
void drawMagnetEffect(Basket basket) {
    if (basket.magnetEffect > 0) {
        setcolor(MAGENTA);
        setlinestyle(DOTTED_LINE, 0, NORM_WIDTH);
        circle(basket.x + basket.width/2, basket.y + basket.height/2, basket.magnetRadius);
        setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    }
}
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    
    // Kh?i t?o h?t gi?ng cho s? ng?u nhi�n
    srand(time(NULL));
    
    // Kh?i t?o m?ng m�u cho g�
    int chickenColors[MAX_GA];
    for (int i = 0; i < MAX_GA; i++) {
        randomChickenColor(&chickenColors[i]);
    }
    
    // Kh?i t?o r? h?ng
    Basket basket;
    basket.x = 300;
    basket.y = 400;
    basket.width = 50;
    basket.height = 30;
    basket.slowEffect = 0;
    basket.magnetEffect = 0;
    basket.magnetRadius = 100;  // B�n k�nh h�t l� 100 pixel
    
    // Kh?i t?o g�
    Chicken chickens[MAX_GA];
    for (int i = 0; i < MAX_GA; i++) {
        chickens[i].x = 100 + i * 120;
        chickens[i].direction = (rand() % 2) * 2 - 1; // -1 ho?c 1
        chickens[i].moveTimer = rand() % 100 + 50;    // Th?i gian �?i h�?ng ng?u nhi�n
        resetEgg(&chickens[i].egg, chickens[i].x, 1);
    }
    
    // Kh?i t?o n?n
    Background bg;
    bg.mountainHeight = 100;
    bg.grassHeight = 50;
    bg.skyColor = LIGHTBLUE;
    bg.grassColor = GREEN;
    bg.mountainColor = DARKGRAY;
    
    int ch, score = 0, lives = 3, level = 1;
    int oldScore = 0;
    int basketSpeed = 10;
    int eggDropTimer = 0;  // Th?i gian �?m ng�?c �? th? tr?ng ti?p theo
    
    // Ch�i nh?c n?n v?i PlaySound
    PlaySound(TEXT("C:\\Users\\HIEU\\Downloads\\gagay1.wav"), NULL, SND_ASYNC | SND_LOOP);
    
    // Hi?n th? m�n h?nh b?t �?u
    drawStartScreen();
    
    // Thi?t l?p trang �? h?a
    int page = 0;
    
    while (lives > 0) {
        // �?t trang active l� trang hi?n �ang v?
        setactivepage(page);
        // Trang hi?n th? l� trang �?i di?n
        setvisualpage(1 - page);
        
        // X�a m�n h?nh
        cleardevice();
        
        if (kbhit()) {
            ch = getch();
            if (ch == 27) break; // ESC �? tho�t
            
            if (ch == 0) {
                ch = getch();
                // Di chuy?n r?
                if (ch == 75 && basket.x > 10) basket.x -= basketSpeed; // Tr�i
                if (ch == 77 && basket.x < SCREEN_WIDTH - basket.width - 10) basket.x += basketSpeed; // Ph?i
                // Th�m di chuy?n l�n xu?ng
                if (ch == 72 && basket.y > 200) basket.y -= basketSpeed; // L�n
                if (ch == 80 && basket.y < SCREEN_HEIGHT - basket.height - 10) basket.y += basketSpeed; // Xu?ng
            }
        }
        
        // V? n?n
        drawBackground(bg);
        
        // Ki?m tra v� t�ng c?p �?
        if (score >= oldScore + 50) {
            level++;
            oldScore = score;
            // Ph�t �m thanh t�ng c?p
            playEffectSound("C:\\Users\\HIEU\\Downloads\\level_up.wav");
        }
        
        // V? r?
        drawBasket(basket);
        
        // X? l? timer th? tr?ng
        eggDropTimer--;
        if (eggDropTimer <= 0) {
            // Ch?n ng?u nhi�n c�c con g� �? th? tr?ng
            selectRandomChickensToLayEggs(chickens);
            
            // �?t l?i th?i gian �?m ng�?c (gi?m d?n theo c?p �? �? game kh� h�n)
            eggDropTimer = 80 - level * 5;
            if (eggDropTimer < 30) eggDropTimer = 30; // Kh�ng �? qu� nhanh
        }
        
        // X? l? hi?u ?ng l�m ch?m
        if (basket.slowEffect > 0) {
            basket.slowEffect--;
        }
        
        // X? l? hi?u ?ng nam ch�m
        if (basket.magnetEffect > 0) {
            basket.magnetEffect--;
            // V? hi?u ?ng nam ch�m
            drawMagnetEffect(basket);
        }
        
        // X? l? v� v? g� v� tr?ng
        for (int i = 0; i < MAX_GA; i++) {
            // Di chuy?n g�
            chickens[i].moveTimer--;
            if (chickens[i].moveTimer <= 0) {
                chickens[i].direction *= -1;
                chickens[i].moveTimer = rand() % 100 + 50; // �?t l?i th?i gian �?i h�?ng
            }
            
            // Gi?i h?n di chuy?n c?a g�
            chickens[i].x += chickens[i].direction * 2;
            if (chickens[i].x < 20) {
                chickens[i].x = 20;
                chickens[i].direction = 1;
            } else if (chickens[i].x > SCREEN_WIDTH - 20) {
                chickens[i].x = SCREEN_WIDTH - 20;
                chickens[i].direction = -1;
            }
            
            // V? g�
            drawChicken(chickens[i].x, 80, chickenColors[i]);
            
            // X? l? tr?ng
            Egg *e = &chickens[i].egg;
            if (e->active) {
                // C?p nh?t v? tr� tr?ng theo g� khi tr?ng m?i xu?t hi?n
                if (e->y == 100) {
                    e->x = chickens[i].x;
                }
                
                // T�nh to�n t?c �? th?c t? c?a tr?ng (b? ch?m l?i n?u c� hi?u ?ng)
                int actualSpeed = e->speed;
                if (basket.slowEffect > 0) {
                    actualSpeed = e->speed / 2;  // Gi?m t?c �? tr?ng r�i c?n 1/2
                    if (actualSpeed < 1) actualSpeed = 1;
                }
                
                // Hi?u ?ng nam ch�m
                if (basket.magnetEffect > 0 && e->type != TRUNG_BOM) {
                    // T�nh kho?ng c�ch t? tr?ng �?n r?
                    int dx = (basket.x + basket.width/2) - e->x;
                    int dy = (basket.y + basket.height/2) - e->y;
                    int distance = sqrt(dx*dx + dy*dy);
                    
                    // N?u tr?ng trong ph?m vi h�t
                    if (distance < basket.magnetRadius) {
                        // Di chuy?n tr?ng v? ph�a r?
                        e->x += dx / 10;
                        e->y += dy / 10;
                    }
                }
                
                // Di chuy?n tr?ng v?i t?c �? th?c t?
                e->y += actualSpeed;
                
                // V? tr?ng
                drawEgg(*e);
                
                // Ki?m tra va ch?m v?i r?
                if (checkCollision(*e, basket)) {
                    switch (e->type) {
                        case TRUNG_THUONG:
                            score += 1;
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\tangdiem1.wav");
                            break;
                        case TRUNG_VANG:
                            score += 5;
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\trungvang.wav");
                            break;
                        case TRUNG_BOM:
                            lives--;
                            drawExplosion(e->x, e->y);
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\trungno.wav");
                            break;
                        case TRUNG_CHAM:
                            basket.slowEffect = 300;  // Hi?u ?ng l�m ch?m k�o d�i 10 gi�y (300/30)
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\tangdiem1.wav");  // Thay b?ng �m thanh ph� h?p n?u c�
                            break;
                        case TRUNG_NAM_CHAM:
                            basket.magnetEffect = 450;  // Hi?u ?ng nam ch�m k�o d�i 15 gi�y (450/30)
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\tangdiem1.wav");  // Thay b?ng �m thanh ph� h?p n?u c�
                            break;
                    }
                    resetEgg(e, chickens[i].x, level);
                }
                // Tr?ng r�i xu?ng �?t
                else if (e->y > SCREEN_HEIGHT) {
                    if (e->type != TRUNG_BOM) {
                        lives--;
                        drawBrokenEgg(e->x, SCREEN_HEIGHT - bg.grassHeight);
                        // Ph�t �m thanh tr?ng v?
                        playEffectSound("C:\\Users\\HIEU\\Downloads\\tiengno.wav");
                    }
                    resetEgg(e, chickens[i].x, level);
                }
            }
        }
        
        // Hi?n th? �i?m v� m?ng
        // Hi?n th? �i?m v� m?ng
displayStatus(score, lives, level, basket);
        
        // �?i trang
        page = 1 - page;
        
        // Th?i gian tr? �? game kh�ng qu� nhanh
        delay(30);
    }
    
    // Hi?n th? m�n h?nh k?t th�c game
    // D?ng nh?c n?n v� ph�t �m thanh k?t th�c game
    PlaySound(NULL, NULL, 0);
    playEffectSound("C:\\Users\\HIEU\\Downloads\\gameover1.wav");
    drawGameOverScreen(score);
    
    closegraph();
    return 0;
}
