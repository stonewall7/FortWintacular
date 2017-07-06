
//{{BLOCK(backSplash)

//======================================================================
//
//	backSplash, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 123 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 7872 + 2048 = 10432
//
//	Time-stamp: 2013-11-21, 18:31:31
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKSPLASH_H
#define GRIT_BACKSPLASH_H

#define backSplashTilesLen 7872
extern const unsigned short backSplashTiles[3936];

#define backSplashMapLen 2048
extern const unsigned short backSplashMap[1024];

#define backSplashPalLen 512
extern const unsigned short backSplashPal[256];

#endif // GRIT_BACKSPLASH_H

//}}BLOCK(backSplash)
