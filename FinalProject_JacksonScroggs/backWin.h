
//{{BLOCK(backWin)

//======================================================================
//
//	backWin, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 237 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 15168 + 2048 = 17728
//
//	Time-stamp: 2013-12-02, 05:29:56
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKWIN_H
#define GRIT_BACKWIN_H

#define backWinTilesLen 15168
extern const unsigned short backWinTiles[7584];

#define backWinMapLen 2048
extern const unsigned short backWinMap[1024];

#define backWinPalLen 512
extern const unsigned short backWinPal[256];

#endif // GRIT_BACKWIN_H

//}}BLOCK(backWin)
