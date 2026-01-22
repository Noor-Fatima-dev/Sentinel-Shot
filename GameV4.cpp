#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Calculations
void gotoxy(int x, int y);
char getCharAtxy(short int x, short int y);
int centerIt(int a);
void dimensions();
void setColor(int color);


// Maze
void printBoundary();
void updateScorePannel();

void xTiles(int x1, int x2, int y);
void yTiles(int y1, int y2, int x);
void floatingTiles();
    //Levels
    void printMazeL1();
    void printMazeL2();
    void printMazeL3();
    void printMazeL4();


//pills
void updateScore();
void updatePills();
void printPills();


// Main Character
void printPlayer();
void clearPlayer();
void movePlayer();
void player();
void shootPlayer(int a);
void updateBullets();
void updatePlayerHealth();

// Enemy
void printEnemy();
void clearEnemy();
void moveEnemyFollow();
void enemy();

void updateEnemy(int starting, int ending, int h);
void moveXEnemy();
void moveYEnemy();
void enemyHealth(int x, int y, int index);

// Winning Logic
void winningDoor();

// Gameover
void checkGameover();


// Enemy Array
const int enemySize = 30;
int enemies[enemySize][7];  // start, end, height, dir, len, bulletCount, active
int enemyCount = 0;

// Pills Array
const int pillSize = 20;
int pills[pillSize][3];    // x, y, active
int pillCount = 0;

// Bullets Array
const int bulletSize = 50;
int bullets[bulletSize][4];    // x, y, dirX, active
int bulletCount = 0;


// Global varibles
int currentLevel = 1;
int x_axis, y_axis;
int score=0;
int pX=5, pY=30, health=100, direction; bool onLand = false;
int eX=7, eY=9, enemyMoveCounter = 0;

int topY = 15, bottomY = 33; // Extreme positions of floating tiles
int winX , winY;  // Winning Door Position



// Floating Tiles
const int tileSize = 10;
int tiles[tileSize][4];    // x, movingY, onTile, control
int tileCount = 0;

// Player Dimensions
const int playerDia = 3;
string playerArr[playerDia][playerDia] = {
    " ◉ " ,
    "/█\\" ,
    "/ \\" 
};

bool onTiles = false;
void onFloatingTiles();

main() {
    for(int i=0; i<tileSize; i++)
        tiles[i][2] = false;

    system("cls");
    system("chcp 65001 > nul");  // force UTF-8 mode in CMD
    srand(time(0));
    printBoundary();

    // Levels
    if(currentLevel == 1)
        printMazeL1();
    else if(currentLevel == 2)
        printMazeL2();
    else if(currentLevel == 3)
        printMazeL3();
    else if(currentLevel == 4)
        printMazeL4();

    // Printing Pills
    for(int i=1; i<=10; i++)
        updatePills();
    
    while(true)
    {
        updatePlayerHealth();
        updateScorePannel();
        player();
        moveXEnemy();
        floatingTiles();
        if(GetAsyncKeyState(VK_SPACE)) 
            shootPlayer(direction);
        updateScore();
        updateBullets();
        winningDoor();
        checkGameover();
        Sleep(50);
    }
    
    
    gotoxy(0, y_axis+2);
    
}

// Calculations
void gotoxy(int x, int y) {
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}
char getCharAtxy(short int x, short int y) {
    CHAR_INFO ci;
    COORD xy = {0, 0};
    SMALL_RECT rect = {x, y, x, y};
    COORD coordBufSize;
    coordBufSize.X = 1;
    coordBufSize.Y = 1;
    return ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, coordBufSize, xy, &rect) ? ci.Char.AsciiChar: ' ';
}
int centerIt(int a) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    a = a/2;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int x_coor = csbi.srWindow.Right / 2 - a; // a is half width of string here
}
void dimensions() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x_axis = csbi.srWindow.Right;
    y_axis = csbi.srWindow.Bottom - 5;
}
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


// Maze
void printBoundary() {
    setColor(1);
    dimensions();
    for(int i=0; i<=x_axis; i++) {
        gotoxy(i, 0);
        cout << "═";
    }
    for(int i=0; i<=x_axis; i++) {
        gotoxy(i, y_axis);
        cout << "═";
    }
    for(int i=0; i<=y_axis+4; i++) {
        gotoxy(0, i);
        cout << "║";
    }
    for(int i=0; i<=y_axis+4; i++) {
        gotoxy(x_axis, i);
        cout << "║";
    }
    gotoxy(0, 0);
    cout << "╔";
    gotoxy(0, y_axis);
    cout << "╠";
    gotoxy(x_axis, 0);
    cout << "╗";
    gotoxy(x_axis, y_axis);
    cout << "╣";
    
    winX = x_axis-4;
    winY = y_axis-3;
 
    // Score Pannel
    for(int i=0; i<=x_axis; i++) {
        gotoxy(i, y_axis+4);
        cout << "═";
    }
    gotoxy(0, y_axis+4);
    cout << "╚";
    gotoxy(x_axis, y_axis+4);
    cout << "╝";
}
void updateScorePannel() {
    gotoxy(3, y_axis+1);
    cout << "Score: " << score;
    gotoxy(3, y_axis+3);
    cout << "Health: ";
    gotoxy(11, y_axis+3);
    cout << "   ";
    gotoxy(11, y_axis+3);
    cout << health;

}


void xTiles(int x1, int x2, int y) {
    y = y_axis - y;
    updateEnemy(x1,x2,y-1); // But Why??
    updateEnemy(x_axis-x2,x_axis-x1,y-1);
    for(int i=x1; i<x2; i++) {
        gotoxy(i, y);
        cout << "█";
        gotoxy(x_axis-i, y);
        cout << "█";
    }
}
void yTiles(int y1, int y2, int x) {
    y1 = y_axis - y1;
    y2 = y_axis - y2;

    for(int i=y1; i>=y2; i--) {
        gotoxy(x,i);
        cout << "██";
    }
}

void floatingTiles() {
    setColor(1);

    for(int i=0; i<tileCount; i++) {
        gotoxy(tiles[i][0], tiles[i][1]);
        cout << "     ";
        tiles[i][1] += tiles[i][3];
    
        gotoxy(tiles[i][0], tiles[i][1]); 
        cout << "=====";
    
        if(tiles[i][1] == topY)
            tiles[i][3] = 1;
        if(tiles[i][1] == bottomY)
            tiles[i][3] = -1;
    
        // Player over tiles
        if(tiles[i][1] >= pY+3 && tiles[i][1] <= pY+5 && pX >= tiles[i][0]-2 && pX <= tiles[i][0]+4) {
            tiles[i][2] = true;
        }
        else
            tiles[i][2] = false;

    }
    
}
void onFloatingTiles() {
    int index;
    for(int i=0; i<tileCount; i++) {
        if(tiles[i][2] == true)
            index = i;
    }

    if(tiles[index][3] < 0)
        pY = tiles[index][1] - 4;
    else
        pY = tiles[index][1] - 3;
        
    onLand = true;
    if(GetAsyncKeyState(VK_UP)) {
        for(int i = 0; i < 6; i++) {
            if(
                (getCharAtxy(pX, pY-1) == ' ' && getCharAtxy(pX+1, pY-1) == ' ' && getCharAtxy(pX+2, pY-1) == ' ') ||
                (getCharAtxy(pX, pY-1) == '`' || getCharAtxy(pX+1, pY-1) == '`' || getCharAtxy(pX+2, pY-1) == '`')
            ) {
                pY--;
            } 
            else {
                break; 
            }
        }
        tiles[index][2] = false; 
        Sleep(100); 
    }
    if(
        (getCharAtxy(pX-1, pY) == ' ' && getCharAtxy(pX-1, pY+1) == ' ' && getCharAtxy(pX-1, pY+2) == ' ') ||
        (getCharAtxy(pX-1, pY) == '`' || getCharAtxy(pX-1, pY+1) == '`' || getCharAtxy(pX-1, pY+2) == '`')
    ) 
    {
        if(GetAsyncKeyState(VK_LEFT)) {
            pX--;
            direction = -1;
        }
    }
    if(
        (getCharAtxy(pX+3, pY) == ' ' && getCharAtxy(pX+3, pY+1) == ' ' && getCharAtxy(pX+3, pY+2) == ' ') ||
        (getCharAtxy(pX+3, pY) == '`' || getCharAtxy(pX+3, pY+1) == '`' || getCharAtxy(pX+3, pY+2) == '`') ||
        (getCharAtxy(pX+3, pY) == '|' || getCharAtxy(pX+3, pY+1) == '|' || getCharAtxy(pX+3, pY+2) == '|') 
    ) {
        if(GetAsyncKeyState(VK_RIGHT)) {
            pX++;
            direction = 1;
        }
        
    }
}

void printMazeL1() {
    tileCount = 0;
    setColor(1);
    dimensions();

    int x=10, y=4;

    for(int i=1; i<=6; i++) {
        xTiles(x,x+20,y);
        x+=12;
        y+=4;
    }   
    yTiles(1,y-4,x_axis/2);

    setColor(1);
}
void printMazeL2() {
    setColor(1);
    dimensions();

    topY = 15;
    bottomY = 33;
    tileCount = 2;

    for(int i=0; i<tileCount; i++) { // These properties are constant for all tiles
        tiles[i][1] = bottomY;
        tiles[i][2] = false;
        tiles[i][3] = -1;
    }
    tiles[0][0] = 8;
    tiles[1][0] = x_axis-8;


    int x=15, y=21;

    for(int i=1; i<=6; i++) {
        xTiles(x,x+12,y);
        x+=12;
        y-=4;
    } 
    setColor(1);
}
void printMazeL3() {
    setColor(1);
    dimensions();

    topY = 5;
    bottomY = 32;
    tileCount = 7;

    for(int i=0; i<tileCount; i++) {
        tiles[i][2] = false;
        tiles[i][3] = -1;
    }
    
    tiles[0][0] = 45;
    tiles[0][1] = bottomY;
    
    tiles[1][0] = 55;
    tiles[1][1] = topY + 3;
    
    tiles[2][0] = 65;
    tiles[2][1] = bottomY - 5;

    tiles[3][0] = 75;
    tiles[3][1] = bottomY;
    
    tiles[4][0] = 85;
    tiles[4][1] = topY + 7;

    tiles[5][0] = 95;
    tiles[5][1] = bottomY - 4;
    
    tiles[6][0] = 105;
    tiles[6][1] = topY + 2;

  
    
   xTiles(8, 20, 4);
   xTiles(13, 25, 8);
   xTiles(18, 30, 12);
   xTiles(23, 35, 16);
    
 
    
    setColor(1);
}
void printMazeL4() {
    setColor(1);
    dimensions();

    xTiles(10, 48, 4);
    xTiles(10, 38, 20);
    xTiles(10, 33, 24);
    xTiles(10, 28, 28);

    xTiles(45, 70, 8);
    xTiles(70, 90, 12); //12, 24

    yTiles(20, 28, 8);
    yTiles(4, 24, x_axis-9);

    topY = y_axis-24;
    bottomY = y_axis-10;
    tileCount = 2;

    tiles[0][0] = 40;
    tiles[0][1] = bottomY;
    tiles[0][2] = 0;
    tiles[0][3] = -1;


    tiles[1][0] = 105;
    tiles[1][1] = bottomY;
    tiles[1][2] = 0;
    tiles[1][3] = -1;



    setColor(1);
}

// Pills
void updateScore() {
    setColor(3);
    // Check collision with pills
   for(int i=0; i<pillCount; i++) {
        if((pills[i][0] == pX || pills[i][0] == pX+1 || pills[i][0] == pX+2) && (pills[i][1] == pY || pills[i][1] == pY+1 || pills[i][1] == pY+2)) {
            score++;
            pills[i][2] = false;
            updatePills();
            
        }
       
    }

}
void updatePills() {
    bool validPosition = false;

    while (!validPosition) {
        pills[pillCount][0] = rand() % (x_axis - 4) + 2;
        pills[pillCount][1] = rand() % (y_axis - 4) + 2;

        // Check if space is free
        if (getCharAtxy(pills[pillCount][0], pills[pillCount][1]) == ' ') {
            validPosition = true;
        }
    }

    pills[pillCount][2] = true;

    pillCount++;

    printPills();
}
void printPills() {
    setColor(2);
    // Remove inactive pills First
    for(int i = pillCount - 1; i >= 0; i--) {
        if(!pills[i][2]) {
            for(int j=0; j<3; j++) {
                pills[i][j] = pills[pillCount][j];
            }
            pillCount--;
        }
    }

    for (int i = 0; i < pillCount; i++) {
        gotoxy(pills[i][0], pills[i][1]);
        cout << "`";
    }


}


// Main character
void printPlayer() {
    setColor(6);
    for(int i=0; i<playerDia; i++) {
        for(int j=0; j<playerDia; j++) {
            gotoxy(pX+i, pY+j);
            cout << playerArr[i][j];
        }
    }
}
void clearPlayer() {
    for(int i=0; i<playerDia; i++) {
        for(int j=0; j<playerDia; j++) {
            gotoxy(pX+i, pY+j);
            cout << " ";
        }
    }
}
void movePlayer() {
    if(
        (getCharAtxy(pX-1, pY) == ' ' && getCharAtxy(pX-1, pY+1) == ' ' && getCharAtxy(pX-1, pY+2) == ' ') ||
        (getCharAtxy(pX-1, pY) == '`' || getCharAtxy(pX-1, pY+1) == '`' || getCharAtxy(pX-1, pY+2) == '`')
    ) 
    {
        if(GetAsyncKeyState(VK_LEFT)) {
            pX--;
            direction = -1;
        }
    }
    if(
        (getCharAtxy(pX+3, pY) == ' ' && getCharAtxy(pX+3, pY+1) == ' ' && getCharAtxy(pX+3, pY+2) == ' ') ||
        (getCharAtxy(pX+3, pY) == '`' || getCharAtxy(pX+3, pY+1) == '`' || getCharAtxy(pX+3, pY+2) == '`') ||
        (getCharAtxy(pX+3, pY) == '|' || getCharAtxy(pX+3, pY+1) == '|' || getCharAtxy(pX+3, pY+2) == '|') 
    ) {
        if(GetAsyncKeyState(VK_RIGHT)) {
            pX++;
            direction = 1;
        }
        
    }
    if(GetAsyncKeyState(VK_UP) && onLand) {
        for(int i = 0; i < 6; i++) {
            if(
                (getCharAtxy(pX, pY-1) == ' ' && getCharAtxy(pX+1, pY-1) == ' ' && getCharAtxy(pX+2, pY-1) == ' ') ||
                (getCharAtxy(pX, pY-1) == '`' || getCharAtxy(pX+1, pY-1) == '`' || getCharAtxy(pX+2, pY-1) == '`')
            ) {
                pY--;
            } 
            else {
                break; // Hit ceiling
            }
        }
        onLand = false; 
        Sleep(100); 
    }

    // Player over tiles
    // if(getCharAtxy(pX, pY+3) == '#' || getCharAtxy(pX+1, pY+3) == '#' || getCharAtxy(pX+2, pY+3) == '#' || getCharAtxy(pX, pY+4) == '#' || getCharAtxy(pX+1, pY+4) == '#' || getCharAtxy(pX+2, pY+4) == '#') {
    //     if(control < 0)
    //         pY += control*1;
    //     onLand = true;
    // }
    

    // Gravity
    if(
        (getCharAtxy(pX, pY+3) == ' ' && getCharAtxy(pX+1, pY+3) == ' ' && getCharAtxy(pX+2, pY+3) == ' ' ) ||
        (getCharAtxy(pX, pY+3) == '`' || getCharAtxy(pX+1, pY+3) == '`' || getCharAtxy(pX+2, pY+3) == '`' )
    ) {
            pY++;
            onLand = false;
    }
    else {
        onLand = true;
    }
}
void player(){
    clearPlayer();
    if(tiles[0][2] || tiles[1][2] || tiles[2][2] || tiles[3][2] || tiles[4][2]) {
        onFloatingTiles();
    }
    else movePlayer();
    printPlayer();
}

void shootPlayer(int a) {
    if (bulletCount >= bulletSize) return;
    bullets[bulletCount][0] = (a == 1) ? pX + a*3 : pX + a;    // sets starting point of bullet as it is diff in left and right
    bullets[bulletCount][1] = pY+2;
    bullets[bulletCount][2] = a;
    bullets[bulletCount][3] = 1;  // true
    
    bulletCount++;
}
void updateBullets() {
    setColor(7);
    for(int i = 0; i < bulletCount; i++) {
        if(bullets[i][3]) {
            gotoxy(bullets[i][0], bullets[i][1]);
            cout << " ";
            
            bullets[i][0] += bullets[i][2];
            
            char c = getCharAtxy(bullets[i][0], bullets[i][1]);
            if(c != ' ') {
                bullets[i][3] = 0;  // false
            } else {
                // Draw bullet
                gotoxy(bullets[i][0], bullets[i][1]);
                if(bullets[i][2] != 0) cout << "."; 
            }
        }
    }
    
    // Remove inactive bullets
    for(int i = bulletCount - 1; i >= 0; i--) {
        if(!bullets[i][3]) {
            

            for(int j=0; j<4; j++) {
                bullets[i][j] = bullets[bulletCount-1][j];
            }
            bulletCount--;
        }
    }
}

void updatePlayerHealth() {
    if(
        (getCharAtxy(pX-1, pY+2) == '}') ||
        (getCharAtxy(pX+3, pY+2) == '{')
    ) 
    {
        health = health - 20;
    }
}

// Enemy
void printEnemy() {
    setColor(4);
    gotoxy(eX,eY);
    cout << "{O}" ;
}
void clearEnemy() {
    gotoxy(eX, eY);
    cout << "   ";
}
void moveEnemyFollow() {
    enemyMoveCounter++;
    
    // To slow enemy's speed
    if(enemyMoveCounter >= 3) {
        enemyMoveCounter = 0;
        
        int distX = pX - eX;
        int distY = pY - eY;
        
        if(abs(distX) > abs(distY)) {
            if(distX > 0) {
                if(getCharAtxy(eX + 3, eY) == ' ' && getCharAtxy(eX + 3, eY + 1) == ' ' && getCharAtxy(eX + 3, eY + 2) == ' ') {
                    eX++;
                }
            } 
            else {
                if(getCharAtxy(eX - 1, eY) == ' ' && getCharAtxy(eX - 1, eY + 1) == ' ' && getCharAtxy(eX - 1, eY + 2) == ' ') {
                    eX--;
                }
            }
        } 
        else {
            if(distY > 0) {
                if(getCharAtxy(eX, eY + 3) == ' ' && getCharAtxy(eX + 1, eY + 3) == ' ' && getCharAtxy(eX + 2, eY + 3) == ' ') {
                    eY++;
                }
            } else {
                if(getCharAtxy(eX, eY - 1) == ' ' && getCharAtxy(eX + 1, eY - 1) == ' ' && getCharAtxy(eX + 2, eY - 1) == ' ') {
                    eY--;
                }
            }
        }
    }
}
void enemy() {
    clearEnemy();
    moveEnemyFollow();
    printEnemy();

}
void updateEnemy(int starting, int ending, int h) {
    enemies[enemyCount][0] = starting;
    enemies[enemyCount][1] = ending;
    enemies[enemyCount][2] = h;
    enemies[enemyCount][3] = 1;  // right
    enemies[enemyCount][4] = ending-starting;
    enemies[enemyCount][5] = 1;  // true
    enemies[enemyCount][6] = 1;  // true

    enemyCount++;  
}
void moveXEnemy() {
    // start, end, height, dir, len, bulletCount, active
    setColor(4);
    for(int i=0; i<enemyCount; i++) {
        
        gotoxy(enemies[i][0], enemies[i][2]);
        cout << "   ";

        enemies[i][0] += enemies[i][3];

        gotoxy(enemies[i][0], enemies[i][2]);
        cout << "{0}";
        
        
        if(enemies[i][0] == enemies[i][1]-3 ){
            enemies[i][3] = -1;
        }
        if(enemies[i][0] == (enemies[i][1]-enemies[i][4]) && enemies[i][3] == -1) {
            enemies[i][3] = 1;
        }

        if(enemies[i][3]==-1 && getCharAtxy(enemies[i][0]-1, enemies[i][2]) != ' ')
            enemies[i][3] = 1;
        if(enemies[i][3]==1 && getCharAtxy(enemies[i][0]+3, enemies[i][2]) != ' ')
            enemies[i][3] = -1;

        enemyHealth(enemies[i][0], enemies[i][2], i);

        
    }
    // Remove inactive Enemies
    for(int i = enemyCount - 1; i >= 0; i--) {
        if(!enemies[i][6]) {

            for(int j=0; j<7; j++) {
                enemies[i][j] = enemies[enemyCount-1][j];
            }
            enemyCount--;

          
        }
    }

}
// void moveYEnemy();
void enemyHealth(int x, int y, int index) {
    if(getCharAtxy(x+3,y) == '.' || getCharAtxy(x-1,y) == '.')
        enemies[index][5]++;

    if(enemies[index][5] == 5) {
        enemies[index][6] = 0;
        gotoxy(x,y);
        cout << "   ";
        score+=5;
    }
    
}

// Winning Logic
void winningDoor() {
    setColor(6);
    gotoxy(winX, winY);
    cout << "|[]";
    gotoxy(winX, winY+1);
    cout << "|[]";
    gotoxy(winX, winY+2);
    cout << "|[]";

    if((pX == winX || pX+1 == winX || pX+2 == winX) && enemies[0][6] == 0) {
        system("cls");
        
        dimensions();
        int mid = x_axis/2 - 25;
        int midY = y_axis/2 - 12;

        // Outer border - Gold
        setColor(14);
        for(int i=mid; i<=mid+50; i++) {
            gotoxy(i, midY);
            cout << "═";
            gotoxy(i, midY+24);
            cout << "═";
        }
        for(int i=midY; i<=midY+24; i++) {
            gotoxy(mid, i);
            cout << "║";
            gotoxy(mid+50, i);
            cout << "║";
        }
        gotoxy(mid, midY);
        cout << "╔";
        gotoxy(mid, midY+24);
        cout << "╚";
        gotoxy(mid+50, midY);
        cout << "╗";
        gotoxy(mid+50, midY+24);
        cout << "╝";

        // Inner border - Cyan
        setColor(11);
        for(int i=mid+2; i<=mid+48; i++) {
            gotoxy(i, midY+2);
            cout << "─";
            gotoxy(i, midY+22);
            cout << "─";
        }
        for(int i=midY+2; i<=midY+22; i++) {
            gotoxy(mid+2, i);
            cout << "│";
            gotoxy(mid+48, i);
            cout << "│";
        }
        gotoxy(mid+2, midY+2);
        cout << "┌";
        gotoxy(mid+2, midY+22);
        cout << "└";
        gotoxy(mid+48, midY+2);
        cout << "┐";
        gotoxy(mid+48, midY+22);
        cout << "┘";

        // Clear inner area
        for(int i=midY+3; i<midY+22; i++) {
            for(int j=mid+3; j<mid+48; j++) {
                gotoxy(j,i);
                cout << " ";
            }
        }

        // Victory Banner
        setColor(10);
        string banner = "╔════════════════════════════════════════╗";
        gotoxy(mid+5, midY+4);
        cout << banner;
        banner = "║          🎉 LEVEL COMPLETE! 🎉         ║";
        gotoxy(mid+5, midY+5);
        cout << banner;
        banner = "╚════════════════════════════════════════╝";
        gotoxy(mid+5, midY+6);
        cout << banner;

        // Stats section
        setColor(14);
        string msg = "╭───────── STATS ─────────╮";
        gotoxy(mid+12, midY+9);
        cout << msg;
        
        setColor(15);
        gotoxy(mid+14, midY+11);
        cout << "Level:  " << currentLevel;
        gotoxy(mid+14, midY+12);
        cout << "Score:  " << score;
        gotoxy(mid+14, midY+13);
        cout << "Health: " << health;
        
        setColor(14);
        msg = "╰─────────────────────────╯";
        gotoxy(mid+12, midY+14);
        cout << msg;

        // Message
        setColor(11);
        msg = "Outstanding Performance!";
        gotoxy(mid + 13, midY+16);
        cout << msg;
        
        setColor(6);
        msg = "Ready for the next challenge?";
        gotoxy(mid + 10, midY+18);
        cout << msg;

        // Prompt
        setColor(13);
        msg = "Press any key to continue...";
        gotoxy(mid + 11, midY+20);
        cout << msg;

        cin.ignore();
        getch();

        // Reset for next level
        pX = 5;
        pY = 30;
        enemyCount = 0;
        pillCount = 0;
        bulletCount = 0;
        health = 100;
        tileCount = 0;

        if(currentLevel < 4)
            currentLevel++;
        else {
            // Game completed
            system("cls");
            dimensions();
            int mid = x_axis/2 - 25;
            int midY = y_axis/2 - 12;

            // Outer border - Green
            setColor(10);
            for(int i=mid; i<=mid+50; i++) {
                gotoxy(i, midY);
                cout << "═";
                gotoxy(i, midY+24);
                cout << "═";
            }
            for(int i=midY; i<=midY+24; i++) {
                gotoxy(mid, i);
                cout << "║";
                gotoxy(mid+50, i);
                cout << "║";
            }
            gotoxy(mid, midY);
            cout << "╔";
            gotoxy(mid, midY+24);
            cout << "╚";
            gotoxy(mid+50, midY);
            cout << "╗";
            gotoxy(mid+50, midY+24);
            cout << "╝";

            // Clear inner area
            for(int i=midY+3; i<midY+22; i++) {
                for(int j=mid+3; j<mid+48; j++) {
                    gotoxy(j,i);
                    cout << " ";
                }
            }

            // Victory Message
            setColor(10);
            string banner = "╔════════════════════════════════════════╗";
            gotoxy(mid+5, midY+10);
            cout << banner;
            banner = "║         🎊 CONGRATULATIONS! 🎊         ║";
            gotoxy(mid+5, midY+11);
            cout << banner;
            banner = "║      YOU HAVE COMPLETED THE GAME!      ║";
            gotoxy(mid+5, midY+12);
            cout << banner;
            banner = "╚════════════════════════════════════════╝";
            gotoxy(mid+5, midY+13);
            cout << banner;

            // Prompt
            setColor(7);
            string msg = "Press any key to exit...";
            gotoxy(mid + 13, midY+16);
            cout << msg;

            getch();
            exit(0);
        }
        main();
    }
}

// Game over
void checkGameover() {
    if(health <= 0) {
        
        dimensions();
        int mid = x_axis/2 - 25;
        int midY = y_axis/2 - 12;

        // Outer border - Red
        setColor(12);
        for(int i=mid; i<=mid+50; i++) {
            gotoxy(i, midY);
            cout << "═";
            gotoxy(i, midY+24);
            cout << "═";
        }
        for(int i=midY; i<=midY+24; i++) {
            gotoxy(mid, i);
            cout << "║";
            gotoxy(mid+50, i);
            cout << "║";
        }
        gotoxy(mid, midY);
        cout << "╔";
        gotoxy(mid, midY+24);
        cout << "╚";
        gotoxy(mid+50, midY);
        cout << "╗";
        gotoxy(mid+50, midY+24);
        cout << "╝";

        // Inner border - Dark Red
        setColor(4);
        for(int i=mid+2; i<=mid+48; i++) {
            gotoxy(i, midY+2);
            cout << "─";
            gotoxy(i, midY+22);
            cout << "─";
        }
        for(int i=midY+2; i<=midY+22; i++) {
            gotoxy(mid+2, i);
            cout << "│";
            gotoxy(mid+48, i);
            cout << "│";
        }
        gotoxy(mid+2, midY+2);
        cout << "┌";
        gotoxy(mid+2, midY+22);
        cout << "└";
        gotoxy(mid+48, midY+2);
        cout << "┐";
        gotoxy(mid+48, midY+22);
        cout << "┘";

        // Clear inner area
        for(int i=midY+3; i<midY+22; i++) {
            for(int j=mid+3; j<mid+48; j++) {
                gotoxy(j,i);
                cout << " ";
            }
        }

        // Game Over Banner
        setColor(12);
 string banner = "╔════════════════════════════════════════╗";
        gotoxy(mid+5, midY+4);
        cout << banner;
        banner = "║            ☠  GAME OVER! ☠             ║";
        gotoxy(mid+5, midY+5);
        cout << banner;
        banner = "╚════════════════════════════════════════╝";
        gotoxy(mid+5, midY+6);
        cout << banner;

        // Stats section
        setColor(14);
        string msg = "╭───────── FINAL STATS ─────────╮";
        gotoxy(mid+9, midY+9);
        cout << msg;
        
        setColor(15);
        gotoxy(mid+14, midY+11);
        cout << "Level Reached: " << currentLevel;
        gotoxy(mid+14, midY+12);
        cout << "Final Score:   " << score;
        gotoxy(mid+14, midY+13);
        cout << "Health:        0";
        
        setColor(14);
        msg = "╰───────────────────────────────╯";
        gotoxy(mid+9, midY+14);
        cout << msg;

        // Message
        setColor(4);
        msg = "You fought bravely!";
        gotoxy(mid + 15, midY+16);
        cout << msg;
        
        setColor(8);
        msg = "Better luck next time...";
        gotoxy(mid + 13, midY+18);
        cout << msg;

        // Prompt
        setColor(7);
        msg = "Press any key to exit...";
        gotoxy(mid + 13, midY+20);
        cout << msg;

        getch();
        exit(0);
    }
}
