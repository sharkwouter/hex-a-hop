#ifndef __HHOP_VIDEO_H__
#define __HHOP_VIDEO_H__

// Uncomment this to check cross-platform compilation compatibility
// #undef WIN32

//#define USE_BBTABLET
//#define USE_OPENGL

#define SCREEN_W 640
#define SCREEN_H 480

// Hacky workaround for MSVC's broken for scoping
//#define for if (0) ; else for

extern SDL_Surface * screen;

#endif
