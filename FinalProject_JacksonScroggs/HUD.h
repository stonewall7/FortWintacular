
//{{BLOCK(HUD)

//======================================================================
//
//	HUD, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 6 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 384 + 2048 = 2944
//
//	Time-stamp: 2013-11-22, 14:11:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HUD_H
#define GRIT_HUD_H

#define HUDTilesLen 384
extern const unsigned short HUDTiles[192];

#define HUDMapLen 2048
extern const unsigned short HUDMap[1024];

#define HUDPalLen 512
extern const unsigned short HUDPal[256];

#endif // GRIT_HUD_H

//}}BLOCK(HUD)
