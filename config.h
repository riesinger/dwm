/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Cascadia Code:size=11", "JoyPixels:pixelsize=12:antialias=true:autohint=true"  };
static char dmenufont[]             = "Cascadia Code:size=11";
static char normbgcolor[]           = "#282828";
static char normbordercolor[]       = "#ebdbb2";
static char normfgcolor[]           = "#ebdbb2";
static char selfgcolor[]            = "#282828";
static char selbordercolor[]        = "#fb4934";
static char selbgcolor[]            = "#ebdbb2";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"alacritty", "--class", "spterm", "-d", "120 34", NULL };
const char *spcmd2[] = {"alacritty", "--class", "spcalc", "-d", "50 20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class     instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",      NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ "Alacritty", NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,        NULL,       "Event Tester",  0,            0,           0,         1,        -1 },
	{ NULL,       "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,       "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "H[]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,           XK_grave,     spawn,	SHCMD("dmenuunicode") },
	{ MODKEY,           XK_0,          view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0,          tag,		{.ui = ~0 } },
	{ MODKEY,           XK_BackSpace,  spawn,		SHCMD("sysmenu") },
  { MODKEY,           XK_Tab,        view,   {0} },
  { MODKEY,           XK_q,          killclient, {0} },
  { MODKEY,           XK_b,          spawn,    SHCMD("$BROWSER") },
  { MODKEY,           XK_m,          spawn,    SHCMD("alacritty -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
  { MODKEY|ShiftMask, XK_m,          spawn,    SHCMD("alacritty -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
  { MODKEY|ShiftMask, XK_n,          spawn,    SHCMD("alacritty -e sudo nmtui") },
  { MODKEY,           XK_e,          spawn,    SHCMD("alacritty -e lf") },
  { MODKEY|ShiftMask, XK_r,          spawn,    SHCMD("alacritty -e htop") },
  { MODKEY,           XK_t,          setlayout,  {.v = &layouts[0]} }, /* tile */
  { MODKEY|ShiftMask, XK_t,          setlayout,  {.v = &layouts[1]} }, /* bstack */
  { MODKEY,           XK_y,          setlayout,  {.v = &layouts[2]} }, /* spiral */
  { MODKEY|ShiftMask, XK_y,          setlayout,  {.v = &layouts[3]} }, /* dwindle */
  { MODKEY,           XK_u,          setlayout,  {.v = &layouts[4]} }, /* deck */
  { MODKEY|ShiftMask, XK_u,          setlayout,  {.v = &layouts[5]} }, /* monocle */
  { MODKEY,           XK_i,          setlayout,  {.v = &layouts[6]} }, /* centeredmaster */
  { MODKEY|ShiftMask, XK_i,          setlayout,  {.v = &layouts[7]} }, /* centeredfloatingmaster */
  { MODKEY,           XK_o,          incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask, XK_o,          incnmaster,     {.i = -1 } },
  { MODKEY,           XK_backslash,  view,   {0} },
  { MODKEY,           XK_a,          togglegaps, {0} },
  { MODKEY|ShiftMask, XK_a,          defaultgaps,  {0} },
  { MODKEY,           XK_s,          togglesticky, {0} },
  { MODKEY,           XK_d,          spawn,          {.v = dmenucmd } },
  { MODKEY,           XK_f,          togglefullscr,  {0} },
  { MODKEY|ShiftMask, XK_f,          setlayout,  {.v = &layouts[8]} },
  { MODKEY,           XK_g,          shiftview,  { .i = -1 } },
  { MODKEY|ShiftMask, XK_g,          shifttag, { .i = -1 } },
  { MODKEY,           XK_h,          setmfact, {.f = -0.05} },
  { MODKEY,           XK_l,          setmfact,       {.f = +0.05} },
  { MODKEY,           XK_semicolon,  shiftview,  { .i = 1 } },
  { MODKEY|ShiftMask, XK_semicolon,  shifttag, { .i = 1 } },
  { MODKEY,           XK_apostrophe, togglescratch,  {.ui = 1} },
  { MODKEY,           XK_Return,     spawn,    {.v = termcmd } },
  { MODKEY|ShiftMask, XK_Return,     togglescratch,  {.ui = 0} },
  { MODKEY,           XK_z,          incrgaps, {.i = +3 } },
  { MODKEY,           XK_x,          incrgaps, {.i = -3 } },
  { MODKEY,           XK_c,          spawn,    SHCMD("alacritty -e bc -l") },
  { MODKEY|ShiftMask, XK_c,          spawn,    SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
  { MODKEY|ShiftMask, XK_v,          spawn,    SHCMD("{ killall xcompmgr || setsid xcompmgr & } ; xwallpaper --zoom ~/.local/share/bg") },
  { MODKEY|ShiftMask, XK_b,          togglebar,  {0} },
  { MODKEY|ShiftMask, XK_n,          spawn,    SHCMD("alacritty -e newsboat; pkill -RTMIN+6 dwmblocks") },
  { MODKEY,           XK_comma,      focusmon, {.i = -1 } },
  { MODKEY|ShiftMask, XK_comma,      tagmon,   {.i = -1 } },
  { MODKEY,           XK_period,     focusmon, {.i = +1 } },
  { MODKEY|ShiftMask, XK_period,     tagmon,   {.i = +1 } },
  { MODKEY,           XK_Page_Up,    shiftview,  { .i = -1 } },
  { MODKEY|ShiftMask, XK_Page_Up,    shifttag, { .i = -1 } },
  { MODKEY,           XK_Page_Down,  shiftview,  { .i = +1 } },
  { MODKEY|ShiftMask, XK_Page_Down,  shifttag, { .i = +1 } },
  { MODKEY,           XK_Insert,     spawn,    SHCMD("notify-send \"📋 Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },
  { MODKEY,           XK_F3,         spawn,    SHCMD("displayselect") },
  { MODKEY,           XK_F4,         spawn,    SHCMD("alacritty -e pulsemixer; pkill -RTMIN+10 dwmblocks") },
  { MODKEY,           XK_F5,         xrdb,   {.v = NULL } },
  { MODKEY,           XK_F6,         spawn,    SHCMD("torwrap") },
  { MODKEY,           XK_F7,         spawn,    SHCMD("td-toggle") },
  { MODKEY,           XK_F8,         spawn,    SHCMD("mailsync") },
  { MODKEY,           XK_F9,         spawn,    SHCMD("dmenumount") },
  { MODKEY,           XK_F10,        spawn,    SHCMD("dmenuumount") },
  { MODKEY,           XK_F11,        spawn,    SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
  { MODKEY,           XK_F12,        xrdb,   {.v = NULL } },
  { MODKEY,           XK_space,      zoom,   {0} },
  { MODKEY|ShiftMask, XK_space,      togglefloating, {0} },
  /* { 0,       XK_Print, spawn,    SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") }, */
  /* { ShiftMask,     XK_Print, spawn,    SHCMD("maimpick") }, */
  /* { MODKEY,      XK_Print, spawn,    SHCMD("dmenurecord") }, */
  /* { MODKEY|ShiftMask,    XK_Print, spawn,    SHCMD("dmenurecord kill") }, */
  /* { MODKEY,      XK_Delete,  spawn,    SHCMD("dmenurecord kill") }, */
  /* { MODKEY,      XK_Scroll_Lock, spawn,    SHCMD("killall screenkey || screenkey &") }, */
  { 0, XF86XK_AudioMute,          spawn,    SHCMD("volume mute; pkill -RTMIN+10 dwmblocks") },
  { 0, XF86XK_AudioRaiseVolume,   spawn,    SHCMD("volume up; pkill -RTMIN+10 dwmblocks") },
  { 0, XF86XK_AudioLowerVolume,   spawn,    SHCMD("volume down; pkill -RTMIN+10 dwmblocks") },
  { 0, XF86XK_AudioPrev,          spawn,    SHCMD("playerctl previous") },
  { 0, XF86XK_AudioNext,          spawn,    SHCMD("playerctl next") },
  { 0, XF86XK_AudioPause,         spawn,    SHCMD("playerctl pause") },
  { 0, XF86XK_AudioPlay,          spawn,    SHCMD("playerctl play-pause") },
  { 0, XF86XK_AudioStop,          spawn,    SHCMD("playerctl stop") },
  { 0, XF86XK_AudioMicMute,       spawn,    SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
  { 0, XF86XK_PowerOff,           spawn,    SHCMD("sysmenu") },
  { 0, XF86XK_Sleep,              spawn,    SHCMD("sudo -A zzz") },
  { 0, XF86XK_ScreenSaver,        spawn,    SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
  { 0, XF86XK_TaskPane,           spawn,    SHCMD("alacritty -e htop") },
  { 0, XF86XK_Launch1,            spawn,    SHCMD("xset dpms force off") },
  { 0, XF86XK_TouchpadToggle,     spawn,    SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
  { 0, XF86XK_TouchpadOff,        spawn,    SHCMD("synclient TouchpadOff=1") },
  { 0, XF86XK_TouchpadOn,         spawn,    SHCMD("synclient TouchpadOff=0") },
  { 0, XF86XK_MonBrightnessUp,    spawn,    SHCMD("brightness up 10") },
  { 0, XF86XK_MonBrightnessDown,  spawn,    SHCMD("brightness down 10") },

  /* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
  /* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
  /* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
  /* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
  /* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
  /* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
  /* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
  /* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD("st -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
