#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <mmsystem.h> // Thu vi?n cho �m thanh
#pragma comment(lib, "winmm.lib") // Link thu vi?n Windows Multimedia
#include <math.h>
#include <string.h>

#define MAX_GA 5
#define MAX_ACTIVE_EGGS 2  // Gi?i h?n s? lu?ng tr?ng roi c�ng l�c
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_NAME_LENGTH 20 // �? d�i t?i da c?a t�n ngu?i choi

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
    int direction; // Hu?ng di chuy?n c?a g�
    int moveTimer; // �?m th?i gian d? d?i hu?ng
};

// C?u tr�c cho r? h?ng
struct Basket {
    int x, y;
    int width, height;
    int slowEffect;       // Th?i gian hi?u ?ng l�m ch?m c�n l?i
    int magnetEffect;     // Th?i gian hi?u ?ng nam ch�m c�n l?i
    int magnetRadius;     // B�n k�nh h�t c?a nam ch�m
};

// C?u tr�c cho h�nh ?nh n?n
struct Background {
    int mountainHeight;
    int grassHeight;
    int skyColor;
    int grassColor;
    int mountainColor;
};

// C?u tr�c d? luu k? l?c
struct HighScore {
    char name[MAX_NAME_LENGTH];
    int score;
};

// H�m d?i m�u s?c ng?u nhi�n cho g�
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
    
    // V? tuy?t tr�n d?nh n�i
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
    
    // V? d�y di?n
    setcolor(BLACK);
    line(0, 80, SCREEN_WIDTH, 80);
}

// V? r? h?ng
void drawBasket(Basket basket) {
    // V? r?
    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    
    // V? h�nh n?a h�nh tr�n l?t ngu?c
    arc(basket.x + basket.width/2, basket.y, 0, 180, basket.width/2);
    line(basket.x, basket.y, basket.x, basket.y + basket.height);
    line(basket.x + basket.width, basket.y, basket.x + basket.width, basket.y + basket.height);
    line(basket.x, basket.y + basket.height, basket.x + basket.width, basket.y + basket.height);
    
    // V? c�c du?ng ngang
    setcolor(BROWN);
    for (int i = 1; i < 3; i++) {
        line(basket.x, basket.y + i*(basket.height/3), 
             basket.x + basket.width, basket.y + i*(basket.height/3));
    }
    
    // V? c�c du?ng d?c
    for (int i = 1; i < 3; i++) {
        line(basket.x + i*(basket.width/3), basket.y, 
             basket.x + i*(basket.width/3), basket.y + basket.height);
    }
    
    floodfill(basket.x + basket.width/2, basket.y + basket.height/2, BROWN);
}

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
    
    // V? tr?ng l� h�nh oval
    fillellipse(e.x, e.y, e.width, e.height);
    
    if (e.type == TRUNG_THUONG) {
        // Th�m chi ti?t cho tr?ng thu?ng
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
        line(e.x-5, e.y, e.x+5, e.y);  // D?u d?ng h? ngang
        line(e.x, e.y-5, e.x, e.y+5);  // �u?ng d?c
    } else if (e.type == TRUNG_NAM_CHAM) {
        // Th�m chi ti?t cho tr?ng nam ch�m
        setcolor(WHITE);
        // V? h�nh nam ch�m don gi?n
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
    
    // V? d?u g�
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
    
    // V? l�ng tr?ng tr?ng
    fillellipse(x, y + 5, 15, 5);
    
    // V? l�ng d? tr?ng
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

// Kh?i t?o l?i tr?ng khi roi xu?ng ho?c du?c h?ng
void resetEgg(Egg *egg, int chickenX, int level) {
    egg->x = chickenX;
    egg->y = 100;
    egg->speed = 2 + rand() % 3 + level/10;
    egg->active = 0;  // M?c d?nh l� kh�ng ho?t d?ng
    egg->width = 8;
    egg->height = 12;
    // Lo?i tr?ng s? du?c quy?t d?nh khi tr?ng du?c k�ch ho?t
}

// H�m ki?m tra va ch?m gi?a tr?ng v� r?
int checkCollision(Egg egg, Basket basket) {
    return (egg.y + egg.height >= basket.y && 
            egg.y <= basket.y + basket.height &&
            egg.x + egg.width >= basket.x &&
            egg.x - egg.width <= basket.x + basket.width);
}

// Hi?n th? di?m v� m?ng s?ng
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
    
    // Hi?n th? c�c hi?u ?ng d?c bi?t n?u c�
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
    
    // Hi?n th? bi?u tu?ng tr?ng v� s? lu?ng c�n l?i
    for (int i = 0; i < lives; i++) {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(10 + i*20, 30, 5, 7);
    }
}

// H�m d? ph�t hi?u ?ng �m thanh
void playEffectSound(const char* filename) {
    char command[256];
    // ��ng �m thanh hi?u ?ng tru?c d� n?u c�
    mciSendString("close effect", NULL, 0, NULL);
    
    // T?o l?nh m? file �m thanh m?i
    sprintf(command, "open \"%s\" type waveaudio alias effect", filename);
    mciSendString(command, NULL, 0, NULL);
    
    // Ph�t �m thanh hi?u ?ng
    mciSendString("play effect", NULL, 0, NULL);
}

// H�m v? m�n h�nh b?t d?u
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
    outtextxy(200, 220, "Tam dung: Phim Space");
    outtextxy(200, 240, "Thoat: Phim ESC");
    
    setcolor(LIGHTGREEN);
    outtextxy(180, 300, "Nhan phim bat ky de bat dau...");
    
    // V? h�nh g� v� tr?ng
    for (int i = 0; i < 3; i++) {
        int chickenColor;
        randomChickenColor(&chickenColor);
        drawChicken(100 + i*200, 350, chickenColor);
    }
    
    getch();
}

// H�m nh?p t�n ngu?i choi
void inputPlayerName(char *playerName) {
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    outtextxy(150, 150, "Nhap ten nguoi choi:");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(150, 200, "Toi da 20 ky tu, nhan Enter de xac nhan");
    
    int index = 0;
    char ch;
    playerName[index] = '\0';
    
    // V? khung d? hi?n th? t�n dang nh?p
    setcolor(WHITE);
    rectangle(200, 250, 400, 270);
    
    while (1) {
        if (kbhit()) {
            ch = getch();
            if (ch == 13 && index > 0) { // Enter d? x�c nh?n (ch? khi d� nh?p �t nh?t 1 k� t?)
                playerName[index] = '\0';
                break;
            }
            if (ch == 8 && index > 0) { // Backspace d? x�a k� t?
                index--;
                playerName[index] = '\0';
                // X�a khung v� v? l?i t�n
                setcolor(BLUE);
                setfillstyle(SOLID_FILL, BLUE);
                bar(202, 252, 398, 268);
                setcolor(WHITE);
                rectangle(200, 250, 400, 270);
                outtextxy(205, 255, playerName);
            }
            if (index < MAX_NAME_LENGTH - 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))) {
                playerName[index++] = ch;
                playerName[index] = '\0';
                // Hi?n th? t�n dang nh?p
                setcolor(WHITE);
                outtextxy(205, 255, playerName);
            }
        }
    }
}

// H�m d?c k? l?c t? file
void readHighScore(HighScore *highScore) {
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) {
        strcpy(highScore->name, "Unknown");
        highScore->score = 0;
        return;
    }
    fscanf(file, "%s %d", highScore->name, &highScore->score);
    fclose(file);
}

// H�m luu k? l?c v�o file
void saveHighScore(const char *playerName, int score) {
    HighScore currentHighScore;
    readHighScore(&currentHighScore);
    
    // Ch? luu n?u di?m s? m?i cao hon ho?c file chua t?n t?i
    if (score > currentHighScore.score) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%s %d", playerName, score);
            fclose(file);
        }
    }
}

// H�m v? m�n h�nh k?t th�c
void drawGameOverScreen(int score, const char *playerName) {
    // �?c k? l?c cao nh?t
    HighScore highScore;
    readHighScore(&highScore);
    
    // Luu di?m s? n?u cao hon k? l?c
    saveHighScore(playerName, score);
    
    // �?t l?i trang ho?t d?ng v� hi?n th?
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
    sprintf(finalScore, "Diem cua %s: %d", playerName, score);
    outtextxy(180, 200, finalScore);
    
    // Hi?n th? k? l?c cao nh?t
    char highScoreText[50];
    sprintf(highScoreText, "Ky luc: %s - %d", highScore.name, highScore.score);
    outtextxy(180, 240, highScoreText);
    
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(180, 280, "Nhan phim bat ky de thoat...");
    
    // V? tr?ng v?
    for (int i = 0; i < 5; i++) {
        drawBrokenEgg(100 + i*100, 350);
    }
    
    // �?m b?o m�n h�nh du?c hi?n th?
    delay(100);
    getch();
}

// H�m d? ch?n ng?u nhi�n m?t s? lu?ng g� d? th? tr?ng
void selectRandomChickensToLayEggs(Chicken chickens[]) {
    int activeCount = 0;
    
    // �?m s? tr?ng dang ho?t d?ng
    for (int i = 0; i < MAX_GA; i++) {
        if (chickens[i].egg.active) {
            activeCount++;
        }
    }
    
    // N?u chua d? 2 tr?ng ho?t d?ng, th�m tr?ng m?i
    if (activeCount < MAX_ACTIVE_EGGS) {
        // Danh s�ch con g� chua c� tr?ng ho?t d?ng
        int inactiveChickens[MAX_GA];
        int inactiveCount = 0;
        
        for (int j = 0; j < MAX_GA; j++) {
            if (!chickens[j].egg.active) {
                inactiveChickens[inactiveCount++] = j;
            }
        }
        
        // N?u c� g� chua c� tr?ng ho?t d?ng
        if (inactiveCount > 0) {
            // Ch?n ng?u nhi�n m?t con g�
            int selectedIndex = inactiveChickens[rand() % inactiveCount];
            
            // Quy?t d?nh ng?u nhi�n lo?i tr?ng (c� 5 lo?i)
            int eggType = rand() % 10;  // T? l? xu?t hi?n c�c lo?i tr?ng
            if (eggType < 4)          // 40% tr?ng thu?ng
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

// V? hi?u ?ng nam ch�m n?u dang ho?t d?ng
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
        chickens[i].moveTimer = rand() % 100 + 50;    // Th?i gian d?i hu?ng ng?u nhi�n
        resetEgg(&chickens[i].egg, chickens[i].x, 1);
    }
    
    // Kh?i t?o n?n
    Background bg;
    bg.mountainHeight = 100;
    bg.grassHeight = 50;
    bg.skyColor = LIGHTBLUE;
    bg.grassColor = GREEN;
    bg.mountainColor = DARKGRAY;
    
    // Kh?i t?o t�n ngu?i choi
    char playerName[MAX_NAME_LENGTH];
    
    int ch, score = 0, lives = 3, level = 1;
    int oldScore = 0;
    int basketSpeed = 10;
    int eggDropTimer = 0;  // Th?i gian d?m ngu?c d? th? tr?ng ti?p theo
    int paused = 0;        // Bi?n tr?ng th�i t?m d?ng (0: kh�ng t?m d?ng, 1: t?m d?ng)
    
    // Choi nh?c n?n v?i PlaySound
    PlaySound(TEXT("C:\\Users\\HIEU\\Downloads\\gagay1.wav"), NULL, SND_ASYNC | SND_LOOP);
    
    // Hi?n th? m�n h�nh b?t d?u
    drawStartScreen();
    
    // Nh?p t�n ngu?i choi
    inputPlayerName(playerName);
    
    // Thi?t l?p trang d? h?a
    int page = 0;
    
    while (lives > 0) {
        // �?t trang active l� trang hi?n dang v?
        setactivepage(page);
        // Trang hi?n th? l� trang d?i di?n
        setvisualpage(1 - page);
        
        // X�a m�n h�nh
        cleardevice();
        
        // V? n?n
        drawBackground(bg);
        
        // V? r?
        drawBasket(basket);
        
        // Hi?n th? di?m v� m?ng
        displayStatus(score, lives, level, basket);
        
        // Ki?m tra d?u v�o b�n ph�m
        if (kbhit()) {
            ch = getch();
            if (ch == 27) break; // ESC d? tho�t
            if (ch == 32) {      // Ph�m Space (m� ASCII 32)
                paused = !paused; // Chuy?n d?i tr?ng th�i t?m d?ng
                if (paused) {
                    // D?ng nh?c n?n
                    PlaySound(NULL, NULL, 0);
                    // Hi?n th? th�ng b�o "Game dang d?ng"
                    setcolor(YELLOW);
                    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
                    outtextxy(200, 200, "Game dang dung");
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                    outtextxy(180, 250, "Nhan Space de tiep tuc...");
                } else {
                    // Ti?p t?c nh?c n?n
                    PlaySound(TEXT("C:\\Users\\HIEU\\Downloads\\gagay1.wav"), NULL, SND_ASYNC | SND_LOOP);
                }
            }
            
            // Ch? x? l� di chuy?n r? khi kh�ng t?m d?ng
            if (!paused && ch == 0) {
                ch = getch();
                // Di chuy?n r?
                if (ch == 75 && basket.x > 10) basket.x -= basketSpeed; // Tr�i
                if (ch == 77 && basket.x < SCREEN_WIDTH - basket.width - 10) basket.x += basketSpeed; // Ph?i
                if (ch == 72 && basket.y > 200) basket.y -= basketSpeed; // L�n
                if (ch == 80 && basket.y < SCREEN_HEIGHT - basket.height - 10) basket.y += basketSpeed; // Xu?ng
            }
        }
        
        // Ch? c?p nh?t logic game khi kh�ng t?m d?ng
        if (!paused) {
            // Ki?m tra v� tang c?p d?
            if (score >= oldScore + 50) {
                level++;
                oldScore = score;
                // Ph�t �m thanh tang c?p
                playEffectSound("C:\\Users\\HIEU\\Downloads\\level_up.wav");
            }
            
            // X? l� timer th? tr?ng
            eggDropTimer--;
            if (eggDropTimer <= 0) {
                // Ch?n ng?u nhi�n c�c con g� d? th? tr?ng
                selectRandomChickensToLayEggs(chickens);
                
                // �?t l?i th?i gian d?m ngu?c
                eggDropTimer = 80 - level * 5;
                if (eggDropTimer < 30) eggDropTimer = 30; // Kh�ng d? qu� nhanh
            }
            
            // X? l� hi?u ?ng l�m ch?m
            if (basket.slowEffect > 0) {
                basket.slowEffect--;
            }
            
            // X? l� hi?u ?ng nam ch�m
            if (basket.magnetEffect > 0) {
                basket.magnetEffect--;
                // V? hi?u ?ng nam ch�m
                drawMagnetEffect(basket);
            }
            
            // X? l� v� v? g� v� tr?ng
            for (int i = 0; i < MAX_GA; i++) {
                // Di chuy?n g�
                chickens[i].moveTimer--;
                if (chickens[i].moveTimer <= 0) {
                    chickens[i].direction *= -1;
                    chickens[i].moveTimer = rand() % 100 + 50; // �?t l?i th?i gian d?i hu?ng
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
                
                // X? l� tr?ng
                Egg *e = &chickens[i].egg;
                if (e->active) {
                    // C?p nh?t v? tr� tr?ng theo g� khi tr?ng m?i xu?t hi?n
                    if (e->y == 100) {
                        e->x = chickens[i].x;
                    }
                    
                    // T�nh to�n t?c d? th?c t? c?a tr?ng
                    int actualSpeed = e->speed;
                    if (basket.slowEffect > 0) {
                        actualSpeed = e->speed / 2;  // Gi?m t?c d? tr?ng roi c�n 1/2
                        if (actualSpeed < 1) actualSpeed = 1;
                    }
                    
                    // Hi?u ?ng nam ch�m
                    if (basket.magnetEffect > 0 && e->type != TRUNG_BOM) {
                        // T�nh kho?ng c�ch t? tr?ng d?n r?
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
                    
                    // Di chuy?n tr?ng v?i t?c d? th?c t?
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
                                basket.slowEffect = 300;  // Hi?u ?ng l�m ch?m k�o d�i 10 gi�y
                                playEffectSound("C:\\Users\\HIEU\\Downloads\\tangdiem1.wav");
                                break;
                            case TRUNG_NAM_CHAM:
                                basket.magnetEffect = 450;  // Hi?u ?ng nam ch�m k�o d�i 15 gi�y
                                playEffectSound("C:\\Users\\HIEU\\Downloads\\tangdiem1.wav");
                                break;
                        }
                        resetEgg(e, chickens[i].x, level);
                    }
                    // Tr?ng roi xu?ng d?t
                    else if (e->y > SCREEN_HEIGHT) {
                        if (e->type != TRUNG_BOM) {
                            lives--;
                            drawBrokenEgg(e->x, SCREEN_HEIGHT - bg.grassHeight);
                            playEffectSound("C:\\Users\\HIEU\\Downloads\\tiengno.wav");
                        }
                        resetEgg(e, chickens[i].x, level);
                    }
                }
            }
        }
        
        // �?i trang
        page = 1 - page;
        
        // Th?i gian tr? d? game kh�ng qu� nhanh
        delay(30);
    }
    
    // Hi?n th? m�n h�nh k?t th�c game
    PlaySound(NULL, NULL, 0);
    playEffectSound("C:\\Users\\HIEU\\Downloads\\gameover1.wav");
    drawGameOverScreen(score, playerName);
    
    closegraph();
    return 0;
}
