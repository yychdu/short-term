#ifndef  GUI_H
#define  GUI_H
#define GUI_COUNTOF(a) (sizeof(a) / sizeof(a[0]))

/**********************************
*
*      Text styles
*
***********************************
*/
#define GUI_TS_NORMAL           (0)
#define GUI_TS_UNDERLINE        (1 << 0)
#define GUI_TS_STRIKETHRU       (1 << 1)
#define GUI_TS_OVERLINE         (1 << 2)

/**********************************
*
*      Line styles
*
***********************************
*/
#define GUI_LS_SOLID        (0)
#define GUI_LS_DASH         (1)
#define GUI_LS_DOT          (2)
#define GUI_LS_DASHDOT      (3)
#define GUI_LS_DASHDOTDOT   (4)


/**********************************
*
*      Pen shapes
*
***********************************
*/
#define GUI_PS_ROUND        (0)
#define GUI_PS_FLAT         (1)
#define GUI_PS_SQUARE       (2)

/*      *********************************
        *
        *      Coordinates
        *
        *********************************
*/
#define GUI_COORD_X 0
#define GUI_COORD_Y 1

/* Text alignment flags, horizontal */
#define GUI_TA_HORIZONTAL  (3<<0)
#define GUI_TA_LEFT        (0<<0)
#define GUI_TA_RIGHT	     (1<<0)
#define GUI_TA_CENTER	     (2<<0)
#define GUI_TA_HCENTER	   GUI_TA_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define GUI_TA_VERTICAL   (3<<2)
#define GUI_TA_TOP	      (0<<2)
#define GUI_TA_BOTTOM	    (1<<2)
#define GUI_TA_BASELINE   (2<<2)
#define GUI_TA_VCENTER    (3<<2)

/*    *********************************
      *
      *     Min/Max coordinates
      *
      *********************************
*/
/* Define minimum and maximum coordinates in x and y */
#define GUI_XMIN -4095
#define GUI_XMAX  4095
#define GUI_YMIN -4095
#define GUI_YMAX  4095

/*********************************************************************
*
*       Support for multitasking systems (locking)
*
**********************************************************************
*/

#define GUI_LOCK()
#define GUI_UNLOCK()
#define GUITASK_INIT()
#define GUITASK_COPY_CONTEXT()

/*********************************************************************
*
*             GUI_CONTEXT
*
**********************************************************************

  This structure is public for one reason only:
  To allow the application to save and restore the context.
*/
#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned long   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */
#define GUI_UNI_PTR
#define GUI_CONST_STORAGE const

typedef struct { I16P x,y; } GUI_POINT;
typedef struct { I16 x0,y0,x1,y1; } LCD_RECT;
typedef struct GUI_FONT GUI_FONT;
typedef LCD_RECT        GUI_RECT;
typedef U32 LCD_COLOR;

typedef enum {
	GUI_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	GUI_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} GUI_COLOR_t;
typedef GUI_COLOR_t GUI_COLOR;

typedef struct {
  LCD_RECT       ClipRect;
  U8             TextStyle;
  U8        PenSize;
  U8        PenShape;
  U8        LineStyle;
  U8        FillStyle;
  const GUI_FONT           GUI_UNI_PTR * pAFont;
  I16P DispPosX, DispPosY;
  I16P TextMode, TextAlign;
	GUI_COLOR DrawColor;
} GUI_CONTEXT;

typedef struct {
  int              NumEntries; 
  char             HasTrans;         
  const LCD_COLOR GUI_UNI_PTR * pPalEntries; 
} LCD_LOGPALETTE; 

typedef LCD_LOGPALETTE  GUI_LOGPALETTE;

typedef struct {
  U16P XSize;
  U16P YSize;
  U16P BytesPerLine;
  U16P BitsPerPixel;
  const U8 GUI_UNI_PTR * pData;
  const GUI_LOGPALETTE GUI_UNI_PTR * pPal;
} GUI_BITMAP;

typedef struct {
  U16 Flags;
  U8 Baseline;
  U8 LHeight;     /* height of a small lower case character (a,x) */
  U8 CHeight;     /* height of a small upper case character (A,X) */
} GUI_FONTINFO;

#define GUI_FONTINFO_FLAG_PROP (1<<0)    /* Is proportional */

typedef struct {
  U8 XSize;
  U8 XDist;
  U8 BytesPerLine;
  const unsigned char GUI_UNI_PTR * pData;
} GUI_CHARINFO;

typedef struct GUI_FONT_INFO {
  U16P First;                        /* first character               */
  U16P Last;                         /* last character                */
  const GUI_CHARINFO* paCharInfo;    /* address of first character    */
  const struct GUI_FONT_INFO* pNext; /* pointer to next */
} GUI_FONT_INFO;

typedef struct GUI_FONT_PROP {
  U16P First;                                /* first character               */
  U16P Last;                                 /* last character                */
  const GUI_CHARINFO GUI_UNI_PTR * paCharInfo;            /* address of first character    */
  const struct GUI_FONT_PROP GUI_UNI_PTR * pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef int  tGUI_GetLineDistX(const char GUI_UNI_PTR *s, int Len);
typedef int  tGUI_GetLineLen(const char GUI_UNI_PTR *s, int MaxLen);
typedef void tGL_DispLine(const char GUI_UNI_PTR *s, int Len);

typedef struct {
  tGUI_GetLineDistX*          pfGetLineDistX;
  tGUI_GetLineLen*            pfGetLineLen;
  tGL_DispLine*               pfDispLine;
} tGUI_ENC_APIList;

#define DECLARE_FONT(Type)                                     \
void GUI##Type##_DispChar    (U16P c);                         \
int  GUI##Type##_GetCharDistX(U16P c);                         \
void GUI##Type##_GetFontInfo (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi); \
char GUI##Type##_IsInFont    (const GUI_FONT GUI_UNI_PTR * pFont, U16 c)

DECLARE_FONT(PROP);
#define GUI_FONTTYPE_PROP_SJIS  \
  GUIPROP_DispChar,             \
	GUIPROP_GetCharDistX,         \
	GUIPROP_GetFontInfo,          \
	GUIPROP_IsInFont,             \
  0

typedef void GUI_DISPCHAR(U16 c);
typedef int  GUI_GETCHARDISTX(U16P c);
typedef void GUI_GETFONTINFO(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi);
typedef char GUI_ISINFONT   (const GUI_FONT GUI_UNI_PTR * pFont, U16 c);

struct GUI_FONT {
  GUI_DISPCHAR*     pfDispChar; 
  GUI_GETCHARDISTX* pfGetCharDistX; 
  GUI_GETFONTINFO*  pfGetFontInfo; 
  GUI_ISINFONT*     pfIsInFont;
  const tGUI_ENC_APIList* pafEncode;
  U8 YSize;
  U8 YDist;
  U8 XMag;
  U8 YMag;
  union {
    const void          GUI_UNI_PTR * pFontData;
//    const GUI_FONT_MONO GUI_UNI_PTR * pMono;
    const GUI_FONT_PROP GUI_UNI_PTR * pProp;
  } p;
	U8 d1;
	U8 d2;
	U8 d3;
};


#ifndef GUI_FLASH 
#define GUI_FLASH 
#endif 

extern GUI_FLASH const GUI_FONT GUI_Font8_ASCII;

#define GUI_DEFAULT_FONT &GUI_Font8_ASCII

extern GUI_CONTEXT        GUI_Context;        /* Thread wide globals */
extern char               GUI_DecChar;

#define COLOR_F (GUI_Context.DrawColor)
#define	COLOR_B ((GUI_COLOR_WHITE == COLOR_F) ? GUI_COLOR_BLACK : GUI_COLOR_WHITE)

/*********************************************************************
*
*             General routines
*
**********************************************************************
*/
int   GUI_Init(void);		// GUI图形库初始化，包括液晶屏初始化
void  GUI_Update(void);		// 屏幕显示刷新
int GUI_GetXSize(void);
int GUI_GetYSize(void);
GUI_COLOR GUI_GetColor(void);
void GUI_SetColor(GUI_COLOR color);

//void         GUI_SetDefault(void);
void GUI_Clear            (void);
void GUI_ClearRect        (int x0, int y0, int x1, int y1);
//void GUI_DrawArc          (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_DrawBitmap       (const GUI_BITMAP GUI_UNI_PTR * pBM, int x0, int y0);
void GUI_DrawCircle       (int x0, int y0, int r);
void GUI_DrawEllipse      (int x0, int y0, int rx, int ry);
void GUI_DrawHLine        (int y0, int x0, int x1);
void GUI_DrawLine         (int x0, int y0, int x1, int y1);
//void GUI_DrawLineTo       (int x, int y);
//void GUI_DrawPie          (int x0, int y0, int r, int a0, int a1, int Type);
void GUI_DrawPixel        (int x, int y);
void GUI_DrawPoint        (int x, int y, GUI_COLOR c);
//void GUI_DrawPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
//void GUI_DrawPolyLine     (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
//void GUI_DrawFocusRect    (const GUI_RECT *pRect, int Dist);
void GUI_DrawRect         (int x0, int y0, int x1, int y1);
void GUI_DrawVLine        (int x0, int y0, int y1);
void GUI_FillCircle       (int x0, int y0, int r);
void GUI_FillEllipse      (int x0, int y0, int rx, int ry);
//void GUI_FillPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GUI_FillRect         (int x0, int y0, int x1, int y1);
//void GUI_InvertRect       (int x0, int y0, int x1, int y1);
//void GUI_MoveTo           (int x, int y);
void      GUI_SetColor     (GUI_COLOR);

/*********************************************************************
*
*              Text related routines
*
**********************************************************************
*/

void  GUI_DispCEOL (void);
void  GUI_DispChar  (U16 c);
//void  GUI_DispChars (U16 c, int Cnt);
void  GUI_DispCharAt(U16 c, I16P x, I16P y);
void  GUI_DispString         (const char GUI_UNI_PTR *s);
void  GUI_DispStringAt       (const char GUI_UNI_PTR *s, int x, int y);
//void  GUI_DispStringAtCEOL   (const char GUI_UNI_PTR *s, int x, int y);
void  GUI_DispStringHCenterAt(const char GUI_UNI_PTR *s, int x, int y);
void  GUI__DispStringInRect  (const char GUI_UNI_PTR *s, GUI_RECT* pRect, int TextAlign, int MaxNumChars);
void  GUI_DispStringInRect   (const char GUI_UNI_PTR *s, GUI_RECT* pRect, int Flags);
void  GUI_DispStringInRectMax(const char GUI_UNI_PTR *s, GUI_RECT* pRect, int TextAlign, int MaxLen); /* Not to be doc. */
void  GUI_DispStringLen      (const char GUI_UNI_PTR *s, int Len);
void  GUI_GetTextExtend(GUI_RECT* pRect, const char GUI_UNI_PTR * s, int Len);
int   GUI_GetYAdjust(void);
int   GUI_GetDispPosX(void);
int   GUI_GetDispPosY(void);
const GUI_FONT GUI_UNI_PTR * GUI_GetFont(void);
int   GUI_GetCharDistX(U16 c);
int   GUI_GetStringDistX(const char GUI_UNI_PTR *s);
int   GUI_GetFontDistY(void);
int   GUI_GetFontSizeY(void);
void  GUI_GetFontInfo   (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO* pfi);
int   GUI_GetYSizeOfFont(const GUI_FONT GUI_UNI_PTR * pFont);
int   GUI_GetYDistOfFont(const GUI_FONT GUI_UNI_PTR * pFont);
int   GUI_GetTextAlign(void);
//int   GUI_GetTextMode(void);
char  GUI_IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c);
int   GUI_SetTextAlign(int Align);
//int   GUI_SetTextMode(int Mode);
//char  GUI_SetTextStyle(char Style);
//void  GUI_SetOrg(int x, int y);
const GUI_FONT GUI_UNI_PTR * GUI_SetFont(const GUI_FONT GUI_UNI_PTR * pNewFont);
char  GUI_GotoXY(int x, int y);
char  GUI_GotoX(int x);
char  GUI_GotoY(int y);
void  GUI_DispNextLine(void);

#define GUI_Delay HAL_Delay 
#define GUI_GetTime HAL_GetTick 
//void GUI_Delay  (int Period);
//int  GUI_GetTime(void);

/**************************************************************
*
*      Defines for constants
*
***************************************************************
*/

#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff


#endif   /* ifdef GUI_H */

/*************************** End of file ****************************/
