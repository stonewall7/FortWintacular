#include "HUD.h"
#include "backInstructions.h"
#include "backLevel0.h"
#include "backLevel1.h"
#include "backLevel2.h"
#include "backLevel3.h"
#include "backLose.h"
#include "backPause.h"
#include "backScene0.h"
#include "backSplash.h"
#include "backWin.h"
#include "collisionLevel0.h"
#include "collisionLevel1.h"
#include "collisionLevel2.h"
#include "collisionLevel3.h"
#include "myLib.h"
#include "soundAdventure.h"
#include "soundHurt.h"
#include "soundJump.h"
#include "soundLose.h"
#include "soundSelect.h"
#include "soundShieldDown.h"
#include "soundShieldUp.h"
#include "soundVictory.h"
#include "spriteSheet.h"

unsigned int buttons;
unsigned int oldButtons;

OBJ_ATTR shadowOAM[128];

#define ROWMASK 0xFF
#define COLMASK 0x1FF
#define SHIFTUP(i) (i<<8)
#define SHIFTDOWN(i) (i>>8)
#define SCREENHEIGHT 160


typedef struct{
    unsigned char* data;
    int length;
    int frequency;
    int isPlaying;
    int loops;
    int duration;
}SOUND;

SOUND soundA;
SOUND soundB;
int vbCountA;
int vbCountB;

typedef struct  {
	int row;                //Row on screen
	int col;                //Column on screen
        int bigRow;             //Row on map
        int bigCol;             //Column on map
        int rdel;               //Vertical velocity
        int cdel;               //Horizontal velocity
	int width;              //Sprite width
        int height;             //Sprite height
        int health;             //Sprite life
        int invincible;         //Boolean to cheat
        int damage;             //Sprite attack power
        int hurtTimer;          //Invincibility after hurt time
        int isAirborne;         //Boolean
        int aniCounter;         //Count until next animation
        int aniDelay;           //Delay for animation
        int aniState;           //Current animation state
        int prevAniState;       //Last animation state
        int currFrame;          //Current animation frame
        int maxFrame;           //Last animation frame
        int reloadTimer;        //Reload time
        int fireRate;
        int type;               //Type of sprite (Player, turret, etc.)
        int active;             //Boolean to deactivate sprite
} MOVOBJ;

#define MAX_ENEMY_BULLETS 10
#define NUM_TURRETS 8

MOVOBJ p1;
MOVOBJ cursor;
MOVOBJ heart;
MOVOBJ flag;
MOVOBJ shield;
MOVOBJ turrets[NUM_TURRETS];
MOVOBJ enemyBullets[MAX_ENEMY_BULLETS];

void splash();
void instructions();
void game();
void pause();
void win();
void lose();
void initialize();
void hideSprites();
int winCondition();
void updateMovement();
void updateGravity();
void updateBullets();
void updateOAM();
void enemyFire(int);
void shootTurrets();
void changeState();
void knockBack(int force, MOVOBJ* obj);
int isOnScreen(MOVOBJ* obj);

void setupSounds();
void playSoundA( const unsigned char* sound, int length, int frequency,int isLooping);
void playSoundB( const unsigned char* sound, int length, int frequency,int isLooping);

void setupInterrupts();
void interruptHandler();

void muteSound();
void unmuteSound();
void stopSound();

enum {SPLASHSCREEN,INSTRUCTIONSSCREEN,GAMESCREEN,PAUSESCREEN,WINSCREEN,LOSESCREEN}; //game states
enum {PLAYER,TURRET,BULLET};
enum {FACERIGHT,FACELEFT,FACEIDLE}; //Animation states
enum {INACTIVE,ACTIVE};

int hOff;
int vOff;
int MAPWIDTH = 512;
int MAPHEIGHT = 256;

int state;
int currLevel;

int main(){
    
    setupInterrupts();
    setupSounds();
    
    initSplash();
    
    while(1)
    {
        oldButtons = buttons;
        buttons = BUTTONS;
        
        switch(state)
        {
            case SPLASHSCREEN:
                splash();
                break;
            case INSTRUCTIONSSCREEN:
                instructions();
                break;
            case GAMESCREEN:
                game();
                break;
            case PAUSESCREEN:
                pause();
                break;
            case WINSCREEN:
                win();
                break;
            case LOSESCREEN:
                lose();
                break;
	}
                
	waitForVblank();
    }
}

void splash(){
    
    hideSprites();
    
    if(BUTTON_PRESSED(BUTTON_UP)){
        if(cursor.row != 75){
            cursor.row=75;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    if(BUTTON_PRESSED(BUTTON_DOWN)){
        if(cursor.row != 93){
            cursor.row=93;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    
    shadowOAM[0].attr0 = cursor.row;
    shadowOAM[0].attr1 = cursor.col;
    shadowOAM[0].attr2 = SPRITEOFFSET16(31,0);
    
    DMANow(3,shadowOAM,OAM,512);
    
    if(BUTTON_PRESSED(BUTTON_START)||BUTTON_PRESSED(BUTTON_A)){
        if(cursor.row == 75){
            initLevel0();
            p1.invincible = -1;
        }else{
            initInstructions();
        }
    }
}

void instructions(){
    if(BUTTON_PRESSED(BUTTON_START)||BUTTON_PRESSED(BUTTON_A)||BUTTON_PRESSED(BUTTON_B)){
        initSplash();
    }
}

void game(){
    
    hideSprites();
    
    updatePlayer();
    updateGravity();
    shootTurrets();
    updateBullets();
    updateOAM();
    changeState();
}

void pause(){
    hideSprites();
    
    if(BUTTON_PRESSED(BUTTON_UP)){
        if(cursor.row > 86){
            cursor.row-=13;
            cursor.col+=23;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    if(BUTTON_PRESSED(BUTTON_DOWN)){
        if(cursor.row < 112){
            cursor.row+=13;
            cursor.col-=23;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    
    shadowOAM[0].attr0 = cursor.row;
    shadowOAM[0].attr1 = cursor.col;
    shadowOAM[0].attr2 = SPRITEOFFSET16(31,0);
    
    DMANow(3,shadowOAM,OAM,512);
    
    if(BUTTON_PRESSED(BUTTON_START)||BUTTON_PRESSED(BUTTON_A)){
        if(cursor.row == 86){
            if(currLevel==0){
                state = GAMESCREEN;

                REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
                REG_BG0CNT = CBB(0) | SBB(27) | BG_SIZE0 | COLOR256;
                REG_BG1CNT = CBB(1) | SBB(28) | BG_SIZE1 | COLOR256;
                REG_BG2CNT = CBB(2) | SBB (30) | BG_SIZE0 | COLOR256;

                loadPalette(backLevel0Pal);

                DMANow(3,HUDTiles, &CHARBLOCKBASE[0], HUDTilesLen/2);
                DMANow(3,HUDMap, &SCREENBLOCKBASE[27], HUDMapLen/2);

                DMANow(3,backLevel0Tiles,&CHARBLOCKBASE[1],backLevel0TilesLen/2);
                DMANow(3,backLevel0Map,&SCREENBLOCKBASE[28],backLevel0MapLen/2);

                DMANow(3,backScene0Tiles,&CHARBLOCKBASE[2],backScene0TilesLen/2);
                DMANow(3,backScene0Map,&SCREENBLOCKBASE[30],backScene0MapLen/2);
            }else if(currLevel==1){
                state = GAMESCREEN;

                REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
                REG_BG1CNT = CBB(0) | SBB(30) | BG_SIZE1 | COLOR256;
                REG_BG0CNT = CBB(1) | SBB(28) | BG_SIZE0 | COLOR256;

                loadPalette(backLevel1Pal);
                DMANow(3,backLevel1Tiles,&CHARBLOCKBASE[0],backLevel1TilesLen/2);
                DMANow(3,backLevel1Map,&SCREENBLOCKBASE[30],backLevel1MapLen/2);

                DMANow(3,HUDTiles, &CHARBLOCKBASE[1], HUDTilesLen/2);
                DMANow(3,HUDMap, &SCREENBLOCKBASE[28], HUDMapLen/2);
            }else if(currLevel==2){
                state = GAMESCREEN;

                REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
                REG_BG1CNT = CBB(0) | SBB(30) | BG_SIZE1 | COLOR256;
                REG_BG0CNT = CBB(1) | SBB(26) | BG_SIZE0 | COLOR256;

                loadPalette(backLevel2Pal);
                DMANow(3,backLevel2Tiles,&CHARBLOCKBASE[0],backLevel2TilesLen/2);
                DMANow(3,backLevel2Map,&SCREENBLOCKBASE[30],backLevel2MapLen/2);

                DMANow(3,HUDTiles, &CHARBLOCKBASE[1], HUDTilesLen/2);
                DMANow(3,HUDMap, &SCREENBLOCKBASE[26], HUDMapLen/2);
            }else if(currLevel==3){
                state = GAMESCREEN;

                REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
                REG_BG1CNT = CBB(0) | SBB(27) | BG_SIZE2 | COLOR256;
                REG_BG0CNT = CBB(1) | SBB(30) | BG_SIZE0 | COLOR256;

                loadPalette(backLevel3Pal);
                DMANow(3,backLevel3Tiles,&CHARBLOCKBASE[0],backLevel3TilesLen/2);
                DMANow(3,backLevel3Map,&SCREENBLOCKBASE[27],backLevel3MapLen/2);

                DMANow(3,HUDTiles, &CHARBLOCKBASE[1], HUDTilesLen/2);
                DMANow(3,HUDMap, &SCREENBLOCKBASE[30], HUDMapLen/2);
            }
        }else if(cursor.row == 99){
            restartLevel();
        }else{
            initSplash();
        }
    }
}

void win(){
    if(BUTTON_PRESSED(BUTTON_START)){
        initSplash();
    }
}

void lose(){
    hideSprites();
    
    if(BUTTON_HELD(BUTTON_UP)){
        if(cursor.row != 110){
            cursor.row = 110;
            cursor.col = 57;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    if(BUTTON_HELD(BUTTON_DOWN)){
        if(cursor.row != 122){
            cursor.row=122;
            cursor.col=36;
            playSoundB(soundSelect,SOUNDSELECTLEN,SOUNDSELECTFREQ,0);
        }
    }
    
    if(heart.aniCounter%heart.aniDelay == 0){             //Change frame every 10th iteration
        if(heart.currFrame<7){
            heart.currFrame++;
        }else{
            heart.currFrame=0;
        }
    }
    heart.aniCounter++;
    
    if(p1.aniCounter%20== 0){
        p1.row += p1.rdel;
    }
    if(p1.row == 80){
        p1.rdel = 1;
    }else if(p1.row == 83){
        p1.rdel = -1;
    }
    p1.aniCounter++;
    
    shadowOAM[0].attr0 = heart.row | ATTR0_TALL;
    shadowOAM[0].attr1 = heart.col | ATTR1_SIZE32;
    shadowOAM[0].attr2 = SPRITEOFFSET16(27,heart.currFrame*2) | 1<<12;
    
    shadowOAM[1].attr0 = p1.row | ATTR0_WIDE;
    shadowOAM[1].attr1 = p1.col | ATTR1_SIZE32;
    shadowOAM[1].attr2 = SPRITEOFFSET16(25,0);
    
    shadowOAM[2].attr0 = cursor.row;
    shadowOAM[2].attr1 = cursor.col;
    shadowOAM[2].attr2 = SPRITEOFFSET16(31,0);
    
    DMANow(3,shadowOAM,OAM,512);
    
    if(BUTTON_PRESSED(BUTTON_START)||BUTTON_PRESSED(BUTTON_A)){
        if(cursor.row == 110){
            playSoundA(soundAdventure,SOUNDADVENTURELEN,SOUNDADVENTUREFREQ,1);
            restartLevel();
        }else{
            initSplash();
        }
    }
}

void initSplash(){
    state=SPLASHSCREEN;
        
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
        
    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE;
    REG_BG1CNT = CBB(0) | SBB(30) | BG_SIZE0 | COLOR256;

    loadPalette(backSplashPal);
    DMANow(3,backSplashTiles,&CHARBLOCKBASE[0],backSplashTilesLen/2);
    DMANow(3,backSplashMap,&SCREENBLOCKBASE[30],backSplashMapLen/2);
    
    loadSpritePalette(spriteSheetPal);
    DMANow(3,spriteSheetTiles,&CHARBLOCKBASE[4],spriteSheetTilesLen/2);
    
    cursor.row=75;
    cursor.col=15;
    
    playSoundA(soundAdventure,SOUNDADVENTURELEN,SOUNDADVENTUREFREQ,1);
}

void initInstructions(){
    
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    
    state=INSTRUCTIONSSCREEN;
    
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    
    REG_BG0CNT = CBB(0) | SBB(30) | BG_SIZE0 | COLOR256;

    loadPalette(backInstructionsPal);
    DMANow(3,backInstructionsTiles,&CHARBLOCKBASE[0],backInstructionsTilesLen/2);
    DMANow(3,backInstructionsMap,&SCREENBLOCKBASE[30],backInstructionsMapLen/2);
}

void initPause(){
    
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    
    state=PAUSESCREEN;
    
    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE;
    
    REG_BG1CNT = CBB(0) | SBB(30) | BG_SIZE0 | COLOR256;
    
    loadPalette(backPausePal);
    
    DMANow(3,backPauseTiles,&CHARBLOCKBASE[0],backPauseTilesLen/2);
    DMANow(3,backPauseMap,&SCREENBLOCKBASE[30],backPauseMapLen/2);
    
    cursor.row=86;
    cursor.col=81;
    
}

void initLevel0(){
    
    state = GAMESCREEN;
    
    currLevel = 0;

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
    REG_BG0CNT = CBB(0) | SBB(27) | BG_SIZE0 | COLOR256;
    REG_BG1CNT = CBB(1) | SBB(28) | BG_SIZE1 | COLOR256;
    REG_BG2CNT = CBB(2) | SBB (30) | BG_SIZE0 | COLOR256;

    loadPalette(backLevel0Pal);
    
    DMANow(3,HUDTiles, &CHARBLOCKBASE[0], HUDTilesLen/2);
    DMANow(3,HUDMap, &SCREENBLOCKBASE[27], HUDMapLen/2);
    
    DMANow(3,backLevel0Tiles,&CHARBLOCKBASE[1],backLevel0TilesLen/2);
    DMANow(3,backLevel0Map,&SCREENBLOCKBASE[28],backLevel0MapLen/2);

    DMANow(3,backScene0Tiles,&CHARBLOCKBASE[2],backScene0TilesLen/2);
    DMANow(3,backScene0Map,&SCREENBLOCKBASE[30],backScene0MapLen/2);

    p1.width = 16;
    p1.height = 24;
    p1.rdel = 0;
    p1.cdel = 0;
    p1.row = 80;
    p1.col = 20;
    p1.health = 3;
    p1.isAirborne=1;
    p1.aniCounter = 0;
    p1.aniDelay = 10;
    p1.currFrame = 0;
    p1.aniState = FACERIGHT;
    p1.type = PLAYER;
    
    flag.active=INACTIVE;
    
    int t;
    for(t=0;t<NUM_TURRETS;t++){
        turrets[t].active = INACTIVE;
    }
    
    int eb;
    for(eb=0;eb<MAX_ENEMY_BULLETS;eb++){
        enemyBullets[eb].active = INACTIVE;
    }
    
    hOff=0;
    vOff=80;
    
    MAPWIDTH = 512;
    MAPHEIGHT = 256;
}

void initLevel1(){}

void initLevel2(){
    
    state = GAMESCREEN;
    
    currLevel = 2;

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
    REG_BG1CNT = CBB(0) | SBB(30) | BG_SIZE1 | COLOR256;
    REG_BG0CNT = CBB(1) | SBB(26) | BG_SIZE0 | COLOR256;

    loadPalette(backLevel2Pal);
    DMANow(3,backLevel2Tiles,&CHARBLOCKBASE[0],backLevel2TilesLen/2);
    DMANow(3,backLevel2Map,&SCREENBLOCKBASE[30],backLevel2MapLen/2);

    DMANow(3,HUDTiles, &CHARBLOCKBASE[1], HUDTilesLen/2);
    DMANow(3,HUDMap, &SCREENBLOCKBASE[26], HUDMapLen/2);
    
    p1.width = 16;
    p1.height = 24;
    p1.rdel = 0;
    p1.cdel = 0;
    p1.row = 120;
    p1.col = 20;
    p1.health = 3;
    p1.isAirborne=1;
    p1.aniCounter = 0;
    p1.aniDelay = 10;
    p1.currFrame = 0;
    p1.aniState = FACERIGHT;
    p1.type = PLAYER;
    
    int t;
    for(t=0;t<NUM_TURRETS;t++){
        turrets[t].width=8;
        turrets[t].height = 16;
        turrets[t].type = TURRET;
        turrets[t].aniCounter = 0;
        turrets[t].aniDelay = 10;
        turrets[t].active = INACTIVE;
    }
    
    turrets[0].bigRow = 232;
    turrets[0].bigCol = 480;
    turrets[0].fireRate = 200;
    turrets[0].aniState = FACELEFT;
    turrets[0].active = ACTIVE;
    
    turrets[1].bigRow = 72;
    turrets[1].bigCol = 496;
    turrets[1].fireRate = 100;
    turrets[1].aniState = FACELEFT;
    turrets[1].active = ACTIVE;
    
    turrets[2].bigRow = 152;
    turrets[2].bigCol = 24;
    turrets[2].fireRate = 200;
    turrets[2].aniState = FACERIGHT;
    turrets[2].active = ACTIVE;
    
    int eb;
    for(eb=0;eb<MAX_ENEMY_BULLETS;eb++){
        enemyBullets[eb].active = INACTIVE;
        enemyBullets[eb].damage = 1;
        enemyBullets[eb].height = 4;
        enemyBullets[eb].width = 4;
        enemyBullets[eb].type = BULLET;
    }
    
    hOff=0;
    vOff=95;
    
    MAPWIDTH = 512;
    MAPHEIGHT = 256;
}

void initLevel3(){
        
    state = GAMESCREEN;
    
    currLevel = 3;

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
    REG_BG1CNT = CBB(0) | SBB(27) | BG_SIZE2 | COLOR256;
    REG_BG0CNT = CBB(1) | SBB(30) | BG_SIZE0 | COLOR256;

    loadPalette(backLevel3Pal);
    DMANow(3,backLevel3Tiles,&CHARBLOCKBASE[0],backLevel3TilesLen/2);
    DMANow(3,backLevel3Map,&SCREENBLOCKBASE[27],backLevel3MapLen/2);

    DMANow(3,HUDTiles, &CHARBLOCKBASE[1], HUDTilesLen/2);
    DMANow(3,HUDMap, &SCREENBLOCKBASE[30], HUDMapLen/2);
    
    p1.width = 16;
    p1.height = 24;
    p1.rdel = 0;
    p1.cdel = 0;
    p1.row = 120;
    p1.col = 205;
    p1.health = 3;
    p1.isAirborne=1;
    p1.aniCounter = 0;
    p1.aniDelay = 10;
    p1.currFrame = 0;
    p1.aniState = FACELEFT;
    p1.type = PLAYER;
    
    flag.bigRow = 40;
    flag.bigCol = 161;
    flag.aniCounter = 0;
    flag.aniDelay = 10;
    flag.currFrame = 0;
    flag.active = ACTIVE;
    
    int t;
    for(t=0;t<NUM_TURRETS;t++){
        turrets[t].width=8;
        turrets[t].height = 16;
        turrets[t].type = TURRET;
        turrets[t].aniCounter = 0;
        turrets[t].aniDelay = 10;
        turrets[t].active = INACTIVE;
    }
    
    turrets[0].bigRow = 408;
    turrets[0].bigCol = 224;
    turrets[0].fireRate = 200;
    turrets[0].aniState = FACELEFT;
    turrets[0].active = ACTIVE;
    
    turrets[1].bigRow = 392;
    turrets[1].bigCol = 224;
    turrets[1].fireRate = 200;
    turrets[1].reloadTimer = 20;
    turrets[1].aniState = FACELEFT;
    turrets[1].active = ACTIVE;
    
    turrets[2].bigRow = 328;
    turrets[2].bigCol = 24;
    turrets[2].fireRate = 200;
    turrets[2].aniState = FACERIGHT;
    turrets[2].active = ACTIVE;
    
    turrets[3].bigRow = 312;
    turrets[3].bigCol = 24;
    turrets[3].fireRate = 200;
    turrets[3].reloadTimer = 20;
    turrets[3].aniState = FACERIGHT;
    turrets[3].active = ACTIVE;
    
    turrets[4].bigRow = 248;
    turrets[4].bigCol = 224;
    turrets[4].fireRate = 150;
    turrets[4].aniState = FACELEFT;
    turrets[4].active = ACTIVE;
    
    turrets[5].bigRow = 232;
    turrets[5].bigCol = 224;
    turrets[5].fireRate = 150;
    turrets[5].aniState = FACELEFT;
    turrets[5].active = ACTIVE;
    
    turrets[6].bigRow = 168;
    turrets[6].bigCol = 24;
    turrets[6].fireRate = 100;
    turrets[6].aniState = FACERIGHT;
    turrets[6].active = ACTIVE;
    
    turrets[7].bigRow = 152;
    turrets[7].bigCol = 24;
    turrets[7].fireRate = 100;
    turrets[7].reloadTimer = 10;
    turrets[7].aniState = FACERIGHT;
    turrets[7].active = ACTIVE;
    
    int eb;
    for(eb=0;eb<MAX_ENEMY_BULLETS;eb++){
        enemyBullets[eb].active = INACTIVE;
        enemyBullets[eb].damage = 1;
        enemyBullets[eb].height = 4;
        enemyBullets[eb].width = 4;
        enemyBullets[eb].type = BULLET;
    }
    
    hOff=15;
    vOff=350;
    
    MAPWIDTH = 256;
    MAPHEIGHT = 512;
}

void initWin(){
    
    state=WINSCREEN;
    
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    
    REG_BG0CNT = CBB(0) | SBB(30) | BG_SIZE0 | COLOR256;

    loadPalette(backWinPal);
    DMANow(3,backWinTiles,&CHARBLOCKBASE[0],backWinTilesLen/2);
    DMANow(3,backWinMap,&SCREENBLOCKBASE[30],backWinMapLen/2);
    
    playSoundA(soundVictory,SOUNDVICTORYLEN,SOUNDVICTORYFREQ,0);
}

void initLose(){
    
    state=LOSESCREEN;
    
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
        
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
    
    REG_BG0CNT = CBB(0) | SBB(30) | BG_SIZE0 | COLOR256;

    cursor.row = 110;
    cursor.col = 57;
    
    heart.row = 50;
    heart.col = 112;
    heart.currFrame = 0;
    heart.aniCounter = 0;
    heart.aniDelay = 10;
    
    p1.row = 80;
    p1.col = 108;
    p1.rdel = 1;
    
    loadPalette(backLosePal);
    DMANow(3,backLoseTiles,&CHARBLOCKBASE[0],backLoseTilesLen/2);
    DMANow(3,backLoseMap,&SCREENBLOCKBASE[30],backLoseMapLen/2);
    
    playSoundA(soundLose,SOUNDLOSELEN,SOUNDLOSEFREQ,0);
}

void restartLevel(){
    if(currLevel == 0){
        initLevel0();
    }else if(currLevel == 1){
        initLevel1();
    }else if(currLevel == 2){
        initLevel2();
    }else if(currLevel == 3){
        initLevel3();
    }
}

void hideSprites(){
    int i;
    for(i = 0; i < 128; i++){
         shadowOAM[i].attr0 = ATTR0_HIDE;
    }
}

int winCondition(){
    if(currLevel==0){
        return (BUTTON_PRESSED(BUTTON_UP) && collisionLevel0Bitmap[OFFSET(p1.bigRow,p1.bigCol,MAPWIDTH)]==GREEN);
    }else if(currLevel==1){
        return (BUTTON_PRESSED(BUTTON_UP) && collisionLevel1Bitmap[OFFSET(p1.bigRow,p1.bigCol,MAPWIDTH)]==GREEN);
    }else if(currLevel==2){
        return (BUTTON_PRESSED(BUTTON_UP) && collisionLevel2Bitmap[OFFSET(p1.bigRow,p1.bigCol,MAPWIDTH)]==GREEN);
    }else if(currLevel==3){
        return (BUTTON_PRESSED(BUTTON_UP) && collisionLevel3Bitmap[OFFSET(p1.bigRow,p1.bigCol,MAPWIDTH)]==GREEN);
    }
}

int lossCondition(){
    
    return p1.health==0;
}

int bitCollide(int r,int c){
    
    if(currLevel==0){
        return collisionLevel0Bitmap[OFFSET(r,c,MAPWIDTH)]==BLACK;
    }else if(currLevel==1){
        return collisionLevel1Bitmap[OFFSET(r,c,MAPWIDTH)]==BLACK;
    }else if(currLevel==2){
        return collisionLevel2Bitmap[OFFSET(r,c,MAPWIDTH)]==BLACK;
    }else if(currLevel==3){
        return collisionLevel3Bitmap[OFFSET(r,c,MAPWIDTH)]==BLACK;
    }
}

void move(MOVOBJ* obj){
    int topRow = obj->bigRow;
    int bottomRow = obj->bigRow+obj->height;
    int leftCol = obj->bigCol;
    int rightCol = obj->bigCol+obj->width;
    
    if(obj->rdel>0){                      //Check downward motion
        if((bitCollide(bottomRow+SHIFTDOWN(obj->rdel),leftCol))||
                (bitCollide(bottomRow+SHIFTDOWN(obj->rdel),rightCol-1))){
            int down=0;
            while(!((bitCollide(bottomRow+down,leftCol))
                ||(bitCollide(bottomRow+down,rightCol-1)))){
                down++;
            }
            obj->rdel=SHIFTUP(down);
            obj->isAirborne=0;
        }
        
        if(obj->type==PLAYER){
            if((vOff+SHIFTDOWN(obj->rdel))<(MAPHEIGHT-SCREENHEIGHT) && obj->row>(SCREENHEIGHT/2)){
                vOff+=SHIFTDOWN(obj->rdel);
            }else if((obj->row+obj->height)<(SCREENHEIGHT)){
                obj->row+=SHIFTDOWN(obj->rdel);
            }
        }else{
            obj->bigRow+=obj->rdel;
        }
    }else if(obj->rdel<0){                         //Check upward motion
        int hitRoof=0;
        if((bitCollide(topRow+SHIFTDOWN(obj->rdel),leftCol))
                ||(bitCollide(topRow+SHIFTDOWN(obj->rdel),rightCol-1))){
            int up=0;
            while(!((bitCollide(topRow-up,leftCol))
                ||(bitCollide(topRow-up,rightCol-1)))){
                up++;
            }
            obj->rdel=-SHIFTUP(up);
            hitRoof=1;
        }
        
        if(obj->type==PLAYER){
            if((vOff+SHIFTDOWN(obj->rdel)>0) && obj->row<(SCREENHEIGHT/2)){
                vOff+=SHIFTDOWN(obj->rdel);
            }else if(obj->row>0){
                obj->row+=SHIFTDOWN(obj->rdel);
            }
            if(hitRoof==1){
                obj->rdel=0;
            }
        }else{
            obj->bigRow+=obj->rdel;
        }
    }
    
    if(obj->cdel>0){                      //Check rightward motion
        if(((bitCollide(topRow,rightCol+SHIFTDOWN(obj->cdel)))
                ||(bitCollide(bottomRow-1,rightCol+SHIFTDOWN(obj->cdel))))){
            int right=0;
            while(!((bitCollide(topRow,rightCol+right))
                ||(bitCollide(bottomRow-1,rightCol+right)))){
                right++;
            }
            obj->cdel=SHIFTUP(right);
        }
        
        if(obj->type==PLAYER){
            if((hOff+SHIFTDOWN(obj->cdel))<(MAPWIDTH-SCREENWIDTH) && obj->col>(SCREENWIDTH/2)){
                hOff+=SHIFTDOWN(obj->cdel);
            }else if((obj->col+obj->width)<(SCREENWIDTH)){
                obj->col+=SHIFTDOWN(obj->cdel);
            }
        }else{
            obj->bigCol+=obj->cdel;
        }
    }else if(obj->cdel<0){                         //Check leftward motion
        if(((bitCollide(topRow,leftCol-1+SHIFTDOWN(obj->cdel)))
                ||(bitCollide(bottomRow-1,leftCol-1+SHIFTDOWN(obj->cdel))))){
            int left=0;
            while(!((bitCollide(topRow,leftCol-1-left))
                ||(bitCollide(bottomRow-1,leftCol-1-left)))){
                left++;
            }
            obj->cdel=-SHIFTUP(left);
        }
        
        if(obj->type==PLAYER){
            if((hOff+SHIFTDOWN(obj->cdel)>0) && obj->col<(SCREENWIDTH/2)){
                hOff+=SHIFTDOWN(obj->cdel);
            }else if(obj->col>0){
                obj->col+=SHIFTDOWN(obj->cdel);
            }
        }else{
            obj->bigCol+=obj->cdel;
        }
    }
    
            
    if(obj->aniState==FACEIDLE){          //If idle, uses standing frame.
        obj->currFrame=0;
        obj->aniState=p1.prevAniState;
    }else{
        obj->aniCounter++;                //Otherwise, animate.
    }
}

void updatePlayer(){
    p1.prevAniState=p1.aniState;        // Set previous state to state
    p1.aniState=FACEIDLE;               // Reset p1's state to idle as default
    
    if(p1.aniCounter%p1.aniDelay==0){             //Change frame every 10th iteration
        if(p1.currFrame<6){
            p1.currFrame++;
        }else{
            p1.currFrame=1;
        }
    }

    p1.bigRow=vOff+p1.row;
    p1.bigCol=hOff+p1.col;
    
    if(BUTTON_PRESSED(BUTTON_A) && !p1.isAirborne){
        p1.rdel=-800;
        p1.isAirborne=1;
        playSoundB(soundJump,SOUNDJUMPLEN,SOUNDJUMPFREQ,0);
    }
    
    if(BUTTON_PRESSED(BUTTON_R)){
        p1.invincible *= -1;
        if(p1.invincible<0){
            playSoundB(soundShieldDown,SOUNDSHIELDDOWNLEN,SOUNDSHIELDDOWNFREQ,0);
        }else{
            playSoundB(soundShieldUp,SOUNDSHIELDUPLEN,SOUNDSHIELDUPFREQ,0);
        }
    }
    
    if(BUTTON_HELD(BUTTON_LEFT)){
        p1.aniState=FACELEFT;
        if((p1.cdel - 50) < -SHIFTUP(1)){
            p1.cdel = -SHIFTUP(1);
        }else{
            p1.cdel -= 50;
        }
    }else if(BUTTON_HELD(BUTTON_RIGHT)){
        p1.aniState=FACERIGHT;
        if((p1.cdel + 50) > SHIFTUP(1)){
            p1.cdel = SHIFTUP(1);
        }else{
            p1.cdel += 50;
        }
    }else{
        if(p1.cdel<0){
            if((p1.cdel + 40) > 0){
                p1.cdel = 0;
            }else{
                p1.cdel += 40;
            }
        }else if(p1.cdel>0){
            if((p1.cdel - 40) < 0){
                p1.cdel = 0;
            }else{
                p1.cdel -= 40;
            }
        }
    }
    
    move(&p1);
    checkFall();
}

void updateGravity(){
    if(!((bitCollide((p1.bigRow+p1.height),p1.bigCol))
                ||(bitCollide((p1.bigRow+p1.height),p1.bigCol+p1.width-1)))){
        p1.isAirborne=1;
    }
    if(p1.isAirborne){
        p1.rdel+=40;
        p1.currFrame=7;
    }else{
        p1.rdel=0;
    }
}

void updateBullets(){
    int eb;
    for(eb=0; eb<MAX_ENEMY_BULLETS; eb++){
        if(enemyBullets[eb].active == ACTIVE){
            enemyBullets[eb].bigCol += enemyBullets[eb].cdel;
            if((bitCollide(enemyBullets[eb].bigRow,enemyBullets[eb].bigCol+enemyBullets[eb].cdel))
                ||(bitCollide(enemyBullets[eb].bigRow+enemyBullets[eb].height-1,enemyBullets[eb].bigCol+enemyBullets[eb].cdel))){
                enemyBullets[eb].active = INACTIVE;
            }else{
                enemyBullets[eb].bigCol += enemyBullets[eb].cdel;
            }
            
            if(enemyBullets[eb].bigRow <= (p1.bigRow+p1.height) && (enemyBullets[eb].bigRow+enemyBullets[eb].height) >= p1.bigRow && 
                    enemyBullets[eb].bigCol <= (p1.bigCol+p1.width) && (enemyBullets[eb].bigCol+enemyBullets[eb].width) >= p1.bigCol){
                if(p1.hurtTimer == 0 && p1.invincible <0){
                    if(enemyBullets[eb].cdel < 0){
                        knockBack(-800,&p1);
                    }else{
                        knockBack(800,&p1);
                    }
                    p1.health--;
                    p1.hurtTimer=100;
                    playSoundB(soundHurt,SOUNDHURTLEN,SOUNDHURTFREQ,0);
                }
                enemyBullets[eb].active = INACTIVE;
            }
            
        }
    }
}

void updateOAM(){
    int spr=0;
    
    if(shield.aniCounter%10==0){             //Change frame every 10th iteration
        if(shield.currFrame<2){
            shield.currFrame++;
        }else{
            shield.currFrame=0;
        }
    }
    shield.aniCounter++;
    
    if(p1.invincible == 1){
        shadowOAM[spr].attr0 = p1.row | ATTR0_TALL | ATTR0_4BPP;
        shadowOAM[spr].attr1 = p1.col | ATTR1_SIZE32;
        shadowOAM[spr].attr2 = SPRITEOFFSET16(17,shield.currFrame*2) | 2<<12;
        spr++;
    }
    
    if(p1.hurtTimer%2==0){
        shadowOAM[spr].attr0 = p1.row | ATTR0_TALL | ATTR0_4BPP;
        shadowOAM[spr].attr1 = p1.col | ATTR1_SIZE32;
        shadowOAM[spr].attr2 = SPRITEOFFSET16(p1.aniState*4,p1.currFrame*2);
    }
    if(p1.hurtTimer>0){
        p1.hurtTimer--;
    }
    spr++;
    
    int h;
    for (h=0;h<p1.health;h++){
        shadowOAM[spr].attr0 = 10;
        shadowOAM[spr].attr1 = h*10 + 10;
        shadowOAM[spr].attr2 = SPRITEOFFSET16(8,0) | 1 << 12;
        spr++;
    }
    
    int t;
    for(t=0;t<NUM_TURRETS;t++){
        if(turrets[t].active == ACTIVE && isOnScreen(&(turrets[t]))){
            turrets[t].row = turrets[t].bigRow-vOff;
            turrets[t].col = turrets[t].bigCol-hOff;
            shadowOAM[spr].attr0 = turrets[t].row | ATTR0_TALL ;
            shadowOAM[spr].attr1 = turrets[t].col;
            shadowOAM[spr].attr2 = SPRITEOFFSET16(9+turrets[t].aniState*2,turrets[t].currFrame);
            spr++;
        }
    }
    
    int eb;
    for(eb=0;eb<MAX_ENEMY_BULLETS;eb++){
        if(enemyBullets[eb].active==ACTIVE && isOnScreen(&(enemyBullets[eb]))){
            enemyBullets[eb].row = enemyBullets[eb].bigRow-vOff;
            enemyBullets[eb].col = enemyBullets[eb].bigCol-hOff;
            shadowOAM[spr].attr0 = enemyBullets[eb].row;
            shadowOAM[spr].attr1 = enemyBullets[eb].col;
            shadowOAM[spr].attr2 = SPRITEOFFSET16(8,1);
            spr++;
        }
    }
    
    if(flag.aniCounter%flag.aniDelay==0){             //Change frame every 10th iteration
        if(flag.currFrame<6){
            flag.currFrame++;
        }else{
            flag.currFrame=0;
        }
    }
    flag.aniCounter++;
    
    if(flag.active){
        shadowOAM[spr].attr0 = flag.bigRow-vOff;
        shadowOAM[spr].attr1 = flag.bigCol-hOff | ATTR1_SIZE32;
        shadowOAM[spr].attr2 = SPRITEOFFSET16(13,flag.currFrame*4) | 2<<12;
        spr++;
    }
    
    REG_BG1HOFS = hOff;
    REG_BG1VOFS = vOff;
    
    REG_BG2HOFS = hOff*1/4;
    REG_BG2VOFS = vOff*1/4;
    
    int i;                      //Copy the shadowOAM into the OAM
    for(i=0;i<128;i++){
        OAM[i].attr0=shadowOAM[i].attr0;
        OAM[i].attr1=shadowOAM[i].attr1;
        OAM[i].attr2=shadowOAM[i].attr2;
    }
}

void checkFall(){
    if(currLevel==0){
        if(collisionLevel0Bitmap[OFFSET(p1.bigRow,p1.bigCol,MAPWIDTH)]==RED){
            if(p1.invincible < 0){
                p1.health--;
                playSoundB(soundHurt,SOUNDHURTLEN,SOUNDHURTFREQ,0);
            }
            
            p1.hurtTimer=100;
            p1.row = 80;
            p1.col = 110;
            hOff = 220; 
        }
    }
}

void enemyFire(int e){
    int i;
    for(i=0; i<MAX_ENEMY_BULLETS; i++){
        if(!enemyBullets[i].active){
             enemyBullets[i].active = ACTIVE;
             if(turrets[e].aniState == FACELEFT){
                 enemyBullets[i].bigRow = turrets[e].bigRow+2;
                 enemyBullets[i].bigCol = turrets[e].bigCol;
                 enemyBullets[i].rdel = 0;
                 enemyBullets[i].cdel = -1;
             }else{
                 enemyBullets[i].bigRow = turrets[e].bigRow+2;
                 enemyBullets[i].bigCol = turrets[e].bigCol+turrets[e].width;
                 enemyBullets[i].rdel = 0;
                 enemyBullets[i].cdel = 1;
             }
             return;
        }
    }
}

void shootTurrets(){
    int t;
    for(t=0;t<NUM_TURRETS;t++){
        if(turrets[t].active == ACTIVE){
            if(turrets[t].reloadTimer%turrets[t].fireRate==0){
                enemyFire(t);
            }
            turrets[t].reloadTimer++;
        }
    }
}

void changeState(){
    if(BUTTON_PRESSED(BUTTON_START)){
        initPause();
    }
        
    if(winCondition()){
        if(currLevel == 0){
            initLevel2();
        }else if(currLevel == 1){
            initLevel2();
        }else if(currLevel == 2){
            initLevel3();
        }else if(currLevel == 3){
            initWin();
        }
    }
    
    if(lossCondition()){
        initLose();
    }
}

void knockBack(int force, MOVOBJ* obj){
    obj->rdel-=400;
    p1.isAirborne=1;
    obj->cdel+=force;
}

int isOnScreen(MOVOBJ* obj){
    int tR = obj->bigRow;
    int bR = obj->bigRow+obj->height;
    int lC = obj->bigCol;
    int rC = obj->bigCol+obj->width;
    
    return !((tR < vOff) || (bR > (vOff+SCREENHEIGHT)) || (lC < hOff) || (rC > (hOff+SCREENWIDTH)));
}

void setupSounds(){
        REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | 
                        DSA_OUTPUT_RATIO_100 | 
                        DSA_OUTPUT_TO_BOTH | 
                        DSA_TIMER0 | 
                        DSA_FIFO_RESET |
                        DSB_OUTPUT_RATIO_100 | 
                        DSB_OUTPUT_TO_BOTH | 
                        DSB_TIMER1 | 
                        DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;
}

void playSoundA(const unsigned char* sound, int length, int frequency, int isLooping) {

	
        dma[1].cnt = 0;
        vbCountA = 0;
	
        int interval = 16777216/frequency;
	
        DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);
	
        REG_TM0CNT = 0;
	
        REG_TM0D = -interval;
        REG_TM0CNT = TIMER_ON;
	
        /**
         * MODIFY THIS FUNCTION
         * Store the necessary variables into your SOUND struct here so that we may recall them later.
         */
        soundA.data = sound;
        soundA.frequency = frequency;
        soundA.isPlaying = 1;
        soundA.length = length;
        soundA.loops = isLooping;
        soundA.duration = ((60*length)/frequency)-((length/frequency)*3)-1;
}

void playSoundB(const unsigned char* sound, int length, int frequency, int isLooping) {

        dma[2].cnt = 0;
        vbCountB = 0;

        int interval = 16777216/frequency;

        DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

        REG_TM1CNT = 0;
	
        REG_TM1D = -interval;
        REG_TM1CNT = TIMER_ON;
	
        /**
         * MODIFY THIS FUNCTION
         * Store the necessary variables into your SOUND struct here so that we may recall them later.
         */
        soundB.data = sound;
        soundB.frequency = frequency;
        soundB.isPlaying = 1;
        soundB.length = length;
        soundB.loops = isLooping;
        soundB.duration = ((60*length)/frequency)-((length/frequency)*3)-1;
}

void setupInterrupts(){
	REG_IME = 0;
	REG_INTERRUPT = (unsigned int)interruptHandler;
	REG_IE |= INT_VBLANK;
	REG_DISPSTAT |= INT_VBLANK_ENABLE;
	REG_IME = 1;
}

void interruptHandler(){
	REG_IME = 0;
	if(REG_IF & INT_VBLANK)
	{
            /**
             * MODIFY THIS FUNCTION
             * Place your code to loop and stop sounds here.
             */
            vbCountA++;
            vbCountB++;
            if(vbCountA >= soundA.duration){
                REG_TM0CNT = 0;
                dma[1].cnt = 0;
                if(soundA.loops){
                    playSoundA(soundA.data,soundA.length,soundA.frequency,1);
                }
            }
            
            if(vbCountB >= soundB.duration){
                REG_TM1CNT = 0;
                dma[2].cnt = 0;
                if(soundB.loops){
                    playSoundB(soundB.data,soundB.length,soundB.frequency,1);
                }
            }
            
            REG_IF = INT_VBLANK; 
	}

	REG_IME = 1;
}

void muteSound(){
    REG_SOUNDCNT_X = 0;
}

void unmuteSound(){
    REG_SOUNDCNT_X = SND_ENABLED;
}

void stopSounds(){
    REG_TM0CNT = 0;
    dma[1].cnt = 0;
    REG_TM1CNT = 0;
    dma[2].cnt = 0;
}