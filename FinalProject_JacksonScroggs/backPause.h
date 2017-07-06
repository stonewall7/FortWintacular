
//{{BLOCK(backPause)

//======================================================================
//
//	backPause, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 83 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5312 + 2048 = 7872
//
//	Time-stamp: 2013-12-02, 09:54:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKPAUSE_H
#define GRIT_BACKPAUSE_H

#define backPauseTilesLen 5312
extern const unsigned short backPauseTiles[2656];

#define backPauseMapLen 2048
extern const unsigned short backPauseMap[1024];

#define backPausePalLen 512
extern const unsigned short backPausePal[256];

#endif // GRIT_BACKPAUSE_H

//}}BLOCK(backPause)
