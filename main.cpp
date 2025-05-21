#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <mmsystem.h> // Thu vi?n cho âm thanh
#pragma comment(lib, "winmm.lib") // Link thu vi?n Windows Multimedia
#include <math.h>
#include <string.h>

#define MAX_GA 5
#define MAX_ACTIVE_EGGS 2  // Gi?i h?n s? lu?ng tr?ng roi cùng lúc
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_NAME_LENGTH 20 // Ð? dài t?i da c?a tên ngu?i choi

// Lo?i tr?ng
#define TRUNG_THUONG 0
#define TRUNG_VANG   1
#define TRUNG_BOM    2
#define TRUNG_CHAM   3  // Tr?ng làm ch?m
#define TRUNG_NAM_CHAM 4  // Tr?ng nam châm
#define TRUNG_TANG_KICH_THUOC 5  // Tr?ng t?ng kích thu?c

// C?u trúc cho tr?ng
struct Egg {
    int x, y;
    int speed;
    int active;
    int type;
    int width;
    int height;
};

// C?u trúc cho gà
struct Chicken {
    int x;
    Egg egg;
    int direction; // Hu?ng di chuy?n c?a gà
    int moveTimer; // Ð?m th?i gian d? d?i hu?ng
};

// C?u trúc cho r? h?ng
struct Basket {
    int x, y;
    int width, height;
    int originalWidth, originalHeight; // Lýu kích thu?c g?c
    int slowEffect;       // Th?i gian hi?u ?ng làm ch?m còn l?i
    int magnetEffect;     // Th?i gian hi?u ?ng nam châm còn l?i
    int sizeEffect;       // Th?i gian hi?u ?ng t?ng kích thu?c
    int magnetRadius;     // Bán kính hút c?a nam châm
};

// C?u trúc cho hình ?nh n?n
struct Background {
    int mountainHeight;
    int grassHeight;
    int skyColor;
    int grassColor;
    int mountainColor;
};

// C?u trúc d? luu k? l?c
struct HighScore {
    char name1[MAX_NAME_LENGTH];
    char name2[MAX_NAME_LENGTH];
    int score;
};

void drawChicken(int x, int y, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, 20, 15);
    fillellipse(x - 15, y - 5, 10, 10);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - 18, y - 8, 2, 2);
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    int beakPoints[] = {x - 25, y - 8, x - 30, y - 3, x - 25, y};
    fillpoly(3, beakPoints);
    setcolor(YELLOW);
    line(x, y + 15, x - 5, y + 25);
    line(x + 5, y + 15, x + 10, y + 25);
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int combPoints[] = {x - 18, y - 13, x - 15, y - 20, x - 10, y - 13};
    fillpoly(3, combPoints);
}

// Hàm d?i màu s?c ng?u nhiên cho gà
void randomChickenColor(int *color) {
    int colors[] = {WHITE, LIGHTGRAY, LIGHTCYAN, LIGHTRED, YELLOW};
    *color = colors[rand() % 5];
}

// V? n?n tr?i, núi và c?
void drawBackground(Background bg) {
    // V? b?u tr?i
    setfillstyle(SOLID_FILL, bg.skyColor);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - bg.grassHeight - bg.mountainHeight);
    
    // V? núi
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
    
    // V? tuy?t trên d?nh núi
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
    
    // V? cây thông bên trái
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
    
    // Thân cây
    setfillstyle(SOLID_FILL, BROWN);
    bar(75, SCREEN_HEIGHT - bg.grassHeight - 40, 85, SCREEN_HEIGHT - bg.grassHeight);
    
    // V? cây lá r?ng bên ph?i
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(550, SCREEN_HEIGHT - bg.grassHeight - 80, 40, 60);
    setfillstyle(SOLID_FILL, BROWN);
    bar(545, SCREEN_HEIGHT - bg.grassHeight - 30, 555, SCREEN_HEIGHT - bg.grassHeight);
    
    // V? dây di?n
    setcolor(BLACK);
    line(0, 80, SCREEN_WIDTH, 80);
}

// V? r? h?ng
void drawBasket(Basket basket, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    
    arc(basket.x + basket.width/2, basket.y, 0, 180, basket.width/2);
    line(basket.x, basket.y, basket.x, basket.y + basket.height);
    line(basket.x + basket.width, basket.y, basket.x + basket.width, basket.y + basket.height);
    line(basket.x, basket.y + basket.height, basket.x + basket.width, basket.y + basket.height);
    
    setcolor(color);
    for (int i = 1; i < 3; i++) {
        line(basket.x, basket.y + i*(basket.height/3), 
             basket.x + basket.width, basket.y + i*(basket.height/3));
    }
    
    for (int i = 1; i < 3; i++) {
        line(basket.x + i*(basket.width/3), basket.y, 
             basket.x + i*(basket.width/3), basket.y + basket.height);
    }
    
    floodfill(basket.x + basket.width/2, basket.y + basket.height/2, color);
}

// V? tr?ng v?i các lo?i khác nhau
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
        case TRUNG_TANG_KICH_THUOC:
            setcolor(LIGHTGREEN);
            setfillstyle(SOLID_FILL, LIGHTGREEN);
            break;
    }
    
    // V? tr?ng là hình oval
    fillellipse(e.x, e.y, e.width, e.height);
    
    if (e.type == TRUNG_THUONG) {
        // Thêm chi ti?t cho tr?ng thu?ng
        setcolor(LIGHTGRAY);
        arc(e.x, e.y - 3, 0, 180, 5);
    } else if (e.type == TRUNG_VANG) {
        // Thêm chi ti?t cho tr?ng vàng
        setcolor(WHITE);
        circle(e.x, e.y, 3);
    } else if (e.type == TRUNG_BOM) {
        // Thêm chi ti?t cho tr?ng bom
        setcolor(BLACK);
        line(e.x-5, e.y-5, e.x+5, e.y+5);
        line(e.x+5, e.y-5, e.x-5, e.y+5);
    } else if (e.type == TRUNG_CHAM) {
        // Thêm chi ti?t cho tr?ng làm ch?m
        setcolor(BLUE);
        circle(e.x, e.y, 3);
        line(e.x-5, e.y, e.x+5, e.y);  // D?u d?ng h? ngang
        line(e.x, e.y-5, e.x, e.y+5);  // Ðu?ng d?c
    } else if (e.type == TRUNG_NAM_CHAM) {
        // Thêm chi ti?t cho tr?ng nam châm
        setcolor(WHITE);
        // V? hình nam châm don gi?n
        arc(e.x, e.y-3, 0, 180, 4);
        line(e.x-4, e.y-3, e.x-4, e.y+3);
        line(e.x+4, e.y-3, e.x+4, e.y+3);
    } else if (e.type == TRUNG_TANG_KICH_THUOC) {
        // Thêm chi ti?t cho tr?ng t?ng kích thu?c
        setcolor(WHITE);
        rectangle(e.x-5, e.y-5, e.x+5, e.y+5);
    }
}

// V? hi?u ?ng tr?ng v?
void drawBrokenEgg(int x, int y) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    
    // V? lòng tr?ng tr?ng
    fillellipse(x, y + 5, 15, 5);
    
    // V? lòng d? tr?ng
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
    egg->active = 0;  // M?c d?nh là không ho?t d?ng
    egg->width = 8;
    egg->height = 12;
    // Lo?i tr?ng s? du?c quy?t d?nh khi tr?ng du?c kích ho?t
}

// Hàm ki?m tra va ch?m gi?a tr?ng và r?
int checkCollision(Egg egg, Basket basket) {
    return (egg.y + egg.height >= basket.y && 
            egg.y <= basket.y + basket.height &&
            egg.x + egg.width >= basket.x &&
            egg.x - egg.width <= basket.x + basket.width);
}

// Hi?n th? di?m và m?ng s?ng
void displayStatus(int score1, int score2, int lives1, int lives2, int level, Basket basket1, Basket basket2, int survivalTime, int gameMode) {
    char status[50];
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, SCREEN_WIDTH, 50);
    
    if (gameMode == 3) { // Survival mode
        sprintf(status, "P1 Score: %d", score1);
        outtextxy(10, 10, status);
        sprintf(status, "P2 Score: %d", score2);
        outtextxy(10, 30, status);
        sprintf(status, "Time: %d s", survivalTime / 30);
        outtextxy(150, 10, status);
        sprintf(status, "Level: %d", level);
        outtextxy(250, 10, status);
    } else {
        sprintf(status, "P1 Score: %d", score1);
        outtextxy(10, 10, status);
        sprintf(status, "P1 Lives: %d", lives1);
        outtextxy(100, 10, status);
        sprintf(status, "P2 Score: %d", score2);
        outtextxy(10, 30, status);
        sprintf(status, "P2 Lives: %d", lives2);
        outtextxy(100, 30, status);
        sprintf(status, "Level: %d", level);
        outtextxy(200, 10, status);
        
        for (int i = 0; i < lives1; i++) {
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(300 + i*20, 10, 5, 7);
        }
        for (int i = 0; i < lives2; i++) {
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(300 + i*20, 30, 5, 7);
        }
    }
    
    if (basket1.slowEffect > 0) {
        setcolor(CYAN);
        sprintf(status, "P1 Slow: %d", basket1.slowEffect/30);
        outtextxy(400, 10, status);
    }
    if (basket1.magnetEffect > 0) {
        setcolor(MAGENTA);
        sprintf(status, "P1 Magnet: %d", basket1.magnetEffect/30);
        outtextxy(500, 10, status);
    }
    if (basket1.sizeEffect > 0) {
        setcolor(LIGHTGREEN);
        sprintf(status, "P1 Size: %d", basket1.sizeEffect/30);
        outtextxy(600, 10, status);
    }
    if (gameMode != 1) {
        if (basket2.slowEffect > 0) {
            setcolor(CYAN);
            sprintf(status, "P2 Slow: %d", basket2.slowEffect/30);
            outtextxy(400, 30, status);
        }
        if (basket2.magnetEffect > 0) {
            setcolor(MAGENTA);
            sprintf(status, "P2 Magnet: %d", basket2.magnetEffect/30);
            outtextxy(500, 30, status);
        }
        if (basket2.sizeEffect > 0) {
            setcolor(LIGHTGREEN);
            sprintf(status, "P2 Size: %d", basket2.sizeEffect/30);
            outtextxy(600, 30, status);
        }
    }
}

// Hàm d? phát hi?u ?ng âm thanh
void playEffectSound(const char* filename) {
    char command[256];
    // Ðóng âm thanh hi?u ?ng tru?c dó n?u có
    mciSendString("close effect", NULL, 0, NULL);
    
    // T?o l?nh m? file âm thanh m?i
    sprintf(command, "open \"%s\" type waveaudio alias effect", filename);
    mciSendString(command, NULL, 0, NULL);
    
    // Phát âm thanh hi?u ?ng
    mciSendString("play effect", NULL, 0, NULL);
}

// Hàm v? màn hình b?t d?u
void drawStartScreen(int *gameMode) {
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy(120, 100, "CHICKEN & EGGS");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 200, "1: Single Player");
    outtextxy(200, 220, "2: Two Players");
    outtextxy(200, 240, "3: Survival Mode");
    outtextxy(200, 260, "P1: Arrow keys, P2: WASD");
    outtextxy(200, 280, "Pause: Space, Exit: ESC");
    
    setcolor(LIGHTGREEN);
    outtextxy(180, 320, "Press 1, 2, or 3 to select mode...");
    
    for (int i = 0; i < 3; i++) {
        int chickenColor;
        randomChickenColor(&chickenColor);
        drawChicken(100 + i*200, 350, chickenColor);
    }
    
    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '1') { *gameMode = 1; break; }
            if (ch == '2') { *gameMode = 2; break; }
            if (ch == '3') { *gameMode = 3; break; }
        }
    }
}

// Hàm nh?p tên ngu?i choi
void inputPlayerName(char *playerName1, char *playerName2, int gameMode) {
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    outtextxy(150, 100, "Enter Player 1 Name:");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(150, 150, "Max 20 chars, press Enter");
    
    int index = 0;
    char ch;
    playerName1[index] = '\0';
    setcolor(WHITE);
    rectangle(200, 200, 400, 220);
    
    while (1) {
        if (kbhit()) {
            ch = getch();
            if (ch == 13 && index > 0) {
                playerName1[index] = '\0';
                break;
            }
            if (ch == 8 && index > 0) {
                index--;
                playerName1[index] = '\0';
                setcolor(BLUE);
                setfillstyle(SOLID_FILL, BLUE);
                bar(202, 202, 398, 218);
                setcolor(WHITE);
                rectangle(200, 200, 400, 220);
                outtextxy(205, 205, playerName1);
            }
            if (index < MAX_NAME_LENGTH - 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))) {
                playerName1[index++] = ch;
                playerName1[index] = '\0';
                setcolor(WHITE);
                outtextxy(205, 205, playerName1);
            }
        }
    }
    
    if (gameMode != 1) { // Input for Player 2 if not single-player
        cleardevice();
        setcolor(YELLOW);
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        outtextxy(150, 100, "Enter Player 2 Name:");
        
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(150, 150, "Max 20 chars, press Enter");
        
        index = 0;
        playerName2[index] = '\0';
        setcolor(WHITE);
        rectangle(200, 200, 400, 220);
        
        while (1) {
            if (kbhit()) {
                ch = getch();
                if (ch == 13 && index > 0) {
                    playerName2[index] = '\0';
                    break;
                }
                if (ch == 8 && index > 0) {
                    index--;
                    playerName2[index] = '\0';
                    setcolor(BLUE);
                    setfillstyle(SOLID_FILL, BLUE);
                    bar(202, 202, 398, 218);
                    setcolor(WHITE);
                    rectangle(200, 200, 400, 220);
                    outtextxy(205, 205, playerName2);
                }
                if (index < MAX_NAME_LENGTH - 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))) {
                    playerName2[index++] = ch;
                    playerName2[index] = '\0';
                    setcolor(WHITE);
                    outtextxy(205, 205, playerName2);
                }
            }
        }
    } else {
        strcpy(playerName2, "");
    }
}

// Hàm d?c k? l?c t? file
void readHighScore(HighScore *highScore, int gameMode) {
    const char *filename = (gameMode == 3) ? "highscore_survival.txt" : "highscore.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        strcpy(highScore->name1, "Unknown");
        strcpy(highScore->name2, "");
        highScore->score = 0;
        return;
    }
    fscanf(file, "%s %s %d", highScore->name1, highScore->name2, &highScore->score);
    fclose(file);
}

// Hàm luu k? l?c vào file

void saveHighScore(const char *playerName1, const char *playerName2, int score, int gameMode) {
    HighScore currentHighScore;
    readHighScore(&currentHighScore, gameMode);
    
    if (score > currentHighScore.score) {
        const char *filename = (gameMode == 3) ? "highscore_survival.txt" : "highscore.txt";
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "%s %s %d", playerName1, playerName2, score);
            fclose(file);
        }
    }
}

// Hàm v? màn hình k?t thúc
void drawGameOverScreen(int score1, int score2, int survivalTime, const char *playerName1, const char *playerName2, int gameMode) {
    HighScore highScore;
    readHighScore(&highScore, gameMode);
    
    int finalScore = (gameMode == 3) ? survivalTime / 30 : (score1 > score2 ? score1 : score2);
    saveHighScore(playerName1, playerName2, finalScore, gameMode);
    
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
    char finalScoreText[50];
    if (gameMode == 3) {
        sprintf(finalScoreText, "%s: %d s", playerName1, survivalTime / 30);
        outtextxy(180, 200, finalScoreText);
        if (gameMode != 1 && strlen(playerName2) > 0) {
            sprintf(finalScoreText, "%s: %d s", playerName2, survivalTime / 30);
            outtextxy(180, 240, finalScoreText);
        }
    } else {
        sprintf(finalScoreText, "%s Score: %d", playerName1, score1);
        outtextxy(180, 200, finalScoreText);
        if (gameMode != 1 && strlen(playerName2) > 0) {
            sprintf(finalScoreText, "%s Score: %d", playerName2, score2);
        }
    }
    
    char highScoreText[50];
    if (strlen(highScore.name2) > 0) {
        sprintf(highScoreText, "High Score: %s & %s - %d", highScore.name1, highScore.name2, highScore.score);
    } else {
        sprintf(highScoreText, "High Score: %s - %d", highScore.name1, highScore.score);
    }
    outtextxy(180, 280, highScoreText);
    
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(180, 320, "Press any key to exit...");
    
    for (int i = 0; i < 5; i++) {
        drawBrokenEgg(100 + i*100, 350);
    }
    
    delay(100);
    getch();
}

// Hàm d? ch?n ng?u nhiên m?t s? lu?ng gà d? th? tr?ng
void selectRandomChickensToLayEggs(Chicken chickens[]) {
    int activeCount = 0;
    
    // Ð?m s? tr?ng dang ho?t d?ng
    for (int i = 0; i < MAX_GA; i++) {
        if (chickens[i].egg.active) {
            activeCount++;
        }
    }
    
    // N?u chua d? 2 tr?ng ho?t d?ng, thêm tr?ng m?i
    if (activeCount < MAX_ACTIVE_EGGS) {
        // Danh sách con gà chua có tr?ng ho?t d?ng
        int inactiveChickens[MAX_GA];
        int inactiveCount = 0;
        
        for (int j = 0; j < MAX_GA; j++) {
            if (!chickens[j].egg.active) {
                inactiveChickens[inactiveCount++] = j;
            }
        }
        
        // N?u có gà chua có tr?ng ho?t d?ng
        if (inactiveCount > 0) {
            // Ch?n ng?u nhiên m?t con gà
            int selectedIndex = inactiveChickens[rand() % inactiveCount];
            
            // Quy?t d?nh ng?u nhiên lo?i tr?ng (có 6 lo?i)
            int eggType = rand() % 11;  // T? l? xu?t hi?n các lo?i tr?ng
            if (eggType < 4)          // ~36% tr?ng thu?ng
                chickens[selectedIndex].egg.type = TRUNG_THUONG;
            else if (eggType < 6)     // ~18% tr?ng vàng
                chickens[selectedIndex].egg.type = TRUNG_VANG;
            else if (eggType < 8)     // ~18% tr?ng bom
                chickens[selectedIndex].egg.type = TRUNG_BOM;
            else if (eggType < 9)     // ~9% tr?ng làm ch?m
                chickens[selectedIndex].egg.type = TRUNG_CHAM;
            else if (eggType < 10)    // ~9% tr?ng nam châm
                chickens[selectedIndex].egg.type = TRUNG_NAM_CHAM;
            else                      // ~9% tr?ng t?ng kích thu?c
                chickens[selectedIndex].egg.type = TRUNG_TANG_KICH_THUOC;
            
            // Kích ho?t tr?ng
            chickens[selectedIndex].egg.active = 1;
        }
    }
}

// V? hi?u ?ng nam châm n?u dang ho?t d?ng
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
    
    srand(time(NULL));
    
    int chickenColors[MAX_GA];
    for (int i = 0; i < MAX_GA; i++) {
        randomChickenColor(&chickenColors[i]);
    }
    
    Basket basket1, basket2;
    basket1.x = 200;
    basket1.y = 400;
    basket1.width = 50;
    basket1.height = 30;
    basket1.originalWidth = basket1.width;
    basket1.originalHeight = basket1.height;
    basket1.slowEffect = 0;
    basket1.magnetEffect = 0;
    basket1.sizeEffect = 0;
    basket1.magnetRadius = 100;
    
    basket2.x = 400;
    basket2.y = 400;
    basket2.width = 50;
    basket2.height = 30;
    basket2.originalWidth = basket2.width;
    basket2.originalHeight = basket2.height;
    basket2.slowEffect = 0;
    basket2.magnetEffect = 0;
    basket2.sizeEffect = 0;
    basket2.magnetRadius = 100;
    
    Chicken chickens[MAX_GA];
    for (int i = 0; i < MAX_GA; i++) {
        chickens[i].x = 100 + i * 120;
        chickens[i].direction = (rand() % 2) * 2 - 1;
        chickens[i].moveTimer = rand() % 100 + 50;
        resetEgg(&chickens[i].egg, chickens[i].x, 1);
    }
    
    Background bg;
    bg.mountainHeight = 100;
    bg.grassHeight = 50;
    bg.skyColor = LIGHTBLUE;
    bg.grassColor = GREEN;
    bg.mountainColor = DARKGRAY;
    
    char playerName1[MAX_NAME_LENGTH], playerName2[MAX_NAME_LENGTH];
    int gameMode = 1; // 1: Single, 2: Two-player, 3: Survival
    int score1 = 0, score2 = 0, lives1 = 3, lives2 = 3;
    int oldScore = 0, level = 1, survivalTime = 0;
    int basketSpeed = 10;
    int eggDropTimer = 0;
    int paused = 0;
    
    PlaySound(TEXT("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\gagay1.wav"), NULL, SND_ASYNC | SND_LOOP);
    
    drawStartScreen(&gameMode);
    inputPlayerName(playerName1, playerName2, gameMode);
    
    int page = 0;
    int gameOver = 0;
    
    while (!gameOver) {
        setactivepage(page);
        setvisualpage(1 - page);
        cleardevice();
        
        drawBackground(bg);
        drawBasket(basket1, BROWN);
        if (gameMode != 1) drawBasket(basket2, BLUE);
        
        displayStatus(score1, score2, lives1, lives2, level, basket1, basket2, survivalTime, gameMode);
        
        if (kbhit()) {
            char ch = getch();
            if (ch == 27) break; // ESC
            if (ch == 32) {
                paused = !paused;
                if (paused) {
                    PlaySound(NULL, NULL, 0);
                    setcolor(YELLOW);
                    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
                    outtextxy(200, 200, "Game Paused");
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                    outtextxy(180, 250, "Press Space to continue...");
                } else {
                    PlaySound(TEXT("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\gagay1.wav"), NULL, SND_ASYNC | SND_LOOP);
                }
            }
            
            if (!paused && ch == 0) {
                ch = getch();
                if (ch == 75 && basket1.x > 10) basket1.x -= basketSpeed; // Left
                if (ch == 77 && basket1.x < SCREEN_WIDTH - basket1.width - 10) basket1.x += basketSpeed; // Right
                if (ch == 72 && basket1.y > 200) basket1.y -= basketSpeed; // Up
                if (ch == 80 && basket1.y < SCREEN_HEIGHT - basket1.height - 10) basket1.y += basketSpeed; // Down
            } else if (!paused && gameMode != 1) {
                if (ch == 'a' && basket2.x > 10) basket2.x -= basketSpeed; // A
                if (ch == 'd' && basket2.x < SCREEN_WIDTH - basket2.width - 10) basket2.x += basketSpeed; // D
                if (ch == 'w' && basket2.y > 200) basket2.y -= basketSpeed; // W
                if (ch == 's' && basket2.y < SCREEN_HEIGHT - basket2.height - 10) basket2.y += basketSpeed; // S
            }
        }
        
        if (!paused) {
            if (gameMode == 3) {
                survivalTime++;
                if (survivalTime % 300 == 0) { // Increase level every 10 seconds
                    level++;
                }
            } else if (score1 + score2 >= oldScore + 50) {
                level++;
                oldScore = score1 + score2;
                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\level_up.wav");
            }
            
            eggDropTimer--;
            if (eggDropTimer <= 0) {
                selectRandomChickensToLayEggs(chickens);
                eggDropTimer = (gameMode == 3) ? (80 - level * 10) : (80 - level * 5);
                if (eggDropTimer < 20) eggDropTimer = 20;
            }
            
            if (basket1.slowEffect > 0) basket1.slowEffect--;
            if (basket1.magnetEffect > 0) {
                basket1.magnetEffect--;
                drawMagnetEffect(basket1);
            }
            if (basket1.sizeEffect > 0) {
                basket1.sizeEffect--;
                if (basket1.sizeEffect == 0) {
                    basket1.width = basket1.originalWidth;
                    basket1.height = basket1.originalHeight;
                }
            }
            
            if (gameMode != 1) {
                if (basket2.slowEffect > 0) basket2.slowEffect--;
                if (basket2.magnetEffect > 0) {
                    basket2.magnetEffect--;
                    drawMagnetEffect(basket2);
                }
                if (basket2.sizeEffect > 0) {
                    basket2.sizeEffect--;
                    if (basket2.sizeEffect == 0) {
                        basket2.width = basket2.originalWidth;
                        basket2.height = basket2.originalHeight;
                    }
                }
            }
            
            for (int i = 0; i < MAX_GA; i++) {
                chickens[i].moveTimer--;
                if (chickens[i].moveTimer <= 0) {
                    chickens[i].direction *= -1;
                    chickens[i].moveTimer = rand() % 100 + 50;
                }
                
                chickens[i].x += chickens[i].direction * 2;
                if (chickens[i].x < 20) {
                    chickens[i].x = 20;
                    chickens[i].direction = 1;
                } else if (chickens[i].x > SCREEN_WIDTH - 20) {
                    chickens[i].x = SCREEN_WIDTH - 20;
                    chickens[i].direction = -1;
                }
                
                drawChicken(chickens[i].x, 80, chickenColors[i]);
                
                Egg *e = &chickens[i].egg;
                if (e->active) {
                    if (e->y == 100) e->x = chickens[i].x;
                    
                    int actualSpeed = e->speed;
                    if (basket1.slowEffect > 0 || (gameMode != 1 && basket2.slowEffect > 0)) {
                        actualSpeed = e->speed / 2;
                        if (actualSpeed < 1) actualSpeed = 1;
                    }
                    
                    if (basket1.magnetEffect > 0 && e->type != TRUNG_BOM) {
                        int dx = (basket1.x + basket1.width/2) - e->x;
                        int dy = (basket1.y + basket1.height/2) - e->y;
                        int distance = sqrt(dx*dx + dy*dy);
                        if (distance < basket1.magnetRadius) {
                            e->x += dx / 10;
                            e->y += dy / 10;
                        }
                    }
                    if (gameMode != 1 && basket2.magnetEffect > 0 && e->type != TRUNG_BOM) {
                        int dx = (basket2.x + basket2.width/2) - e->x;
                        int dy = (basket2.y + basket2.height/2) - e->y;
                        int distance = sqrt(dx*dx + dy*dy);
                        if (distance < basket2.magnetRadius) {
                            e->x += dx / 10;
                            e->y += dy / 10;
                        }
                    }
                    
                    e->y += actualSpeed;
                    drawEgg(*e);
                    
                    int caughtByPlayer1 = checkCollision(*e, basket1);
                    int caughtByPlayer2 = (gameMode != 1) ? checkCollision(*e, basket2) : 0;
                    
                    if (caughtByPlayer1 || caughtByPlayer2) {
                        int *score = caughtByPlayer1 ? &score1 : &score2;
                        Basket *basket = caughtByPlayer1 ? &basket1 : &basket2;
                        switch (e->type) {
                            case TRUNG_THUONG:
                                *score += 1;
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\tangdiem1.wav");
                                break;
                            case TRUNG_VANG:
                                *score += 5;
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\trungvang.wav");
                                break;
                            case TRUNG_BOM:
                                if (gameMode != 3) {
                                    if (caughtByPlayer1) lives1--;
                                    else lives2--;
                                }
                                drawExplosion(e->x, e->y);
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\trungno.wav");
                                break;
                            case TRUNG_CHAM:
                                basket->slowEffect = 300;
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\tangdiem1.wav");
                                break;
                            case TRUNG_NAM_CHAM:
                                basket->magnetEffect = 450;
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\tangdiem1.wav");
                                break;
                            case TRUNG_TANG_KICH_THUOC:
                                basket->width = basket->originalWidth * 1.5;
                                basket->height = basket->originalHeight * 1.5;
                                basket->sizeEffect = 300;
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\tangdiem1.wav");
                                break;
                        }
                        resetEgg(e, chickens[i].x, level);
                    } else if (e->y > SCREEN_HEIGHT) {
                        if (e->type != TRUNG_BOM) {
                            if (gameMode == 3) {
                                gameOver = 1; // End game in survival mode
                            } else {
                                lives1--;
                                if (gameMode != 1) lives2--;
                                drawBrokenEgg(e->x, SCREEN_HEIGHT - bg.grassHeight);
                                playEffectSound("D:\\DuongCongTien\\Kì 224\\TH_DoHoaPC\GameHungTrung-main\\GameHungTrung\\tiengno.wav");
                            }
                        }
                        resetEgg(e, chickens[i].x, level);
                    }
                }
            }
            
            if (gameMode != 3 && (lives1 <= 0 || (gameMode != 1 && lives2 <= 0))) {
                gameOver = 1;
            }
        }
        
        page = 1 - page;
        delay(30);
    }
    
    PlaySound(NULL, NULL, 0);
    playEffectSound("C:\\Users\\HIEU\\Downloads\\gameover1.wav");
    drawGameOverScreen(score1, score2, survivalTime, playerName1, playerName2, gameMode);
    
    closegraph();
    return 0;
}
