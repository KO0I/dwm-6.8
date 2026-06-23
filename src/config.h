/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 20;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int minwsz    = 5;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:size=12:style=Bold:antialias=false" };
static const char dmenufont[]       = "Terminus:size=12:antialias=false";
static unsigned int baralpha        = 0xd0;
static unsigned int borderalpha     = OPAQUE;
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff5555";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]      = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]       = { col_gray4, col_cyan,  col_cyan  },
	[SchemeUrg]       = { col_gray4, col_gray1, col_red   },
	[SchemeFloatNorm] = { col_gray3, col_gray1, col_cyan  },
	[SchemeFloatSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static ResourcePref resources[] = {
	{ "dwm.normfgcolor",      "Dwm.Normfgcolor",      &colors[SchemeNorm][ColFg],      NULL },
	{ "dwm.normbgcolor",      "Dwm.Normbgcolor",      &colors[SchemeNorm][ColBg],      NULL },
	{ "dwm.normbordercolor",  "Dwm.Normbordercolor",  &colors[SchemeNorm][ColBorder],  NULL },
	{ "dwm.selfgcolor",       "Dwm.Selfgcolor",       &colors[SchemeSel][ColFg],       NULL },
	{ "dwm.selbgcolor",       "Dwm.Selbgcolor",       &colors[SchemeSel][ColBg],       NULL },
	{ "dwm.selbordercolor",   "Dwm.Selbordercolor",   &colors[SchemeSel][ColBorder],   NULL },
	{ "dwm.urgfgcolor",       "Dwm.Urgfgcolor",       &colors[SchemeUrg][ColFg],       NULL },
	{ "dwm.urgbgcolor",       "Dwm.Urgbgcolor",       &colors[SchemeUrg][ColBg],       NULL },
	{ "dwm.urgbordercolor",   "Dwm.Urgbordercolor",   &colors[SchemeUrg][ColBorder],   NULL },
	{ "dwm.floatfgcolor",     "Dwm.Floatfgcolor",     &colors[SchemeFloatNorm][ColFg], NULL },
	{ "dwm.floatbgcolor",     "Dwm.Floatbgcolor",     &colors[SchemeFloatNorm][ColBg], NULL },
	{ "dwm.floatbordercolor", "Dwm.Floatbordercolor", &colors[SchemeFloatNorm][ColBorder], NULL },
	{ "dwm.floatselbgcolor",  "Dwm.Floatselbgcolor",  &colors[SchemeFloatSel][ColBg],  NULL },
	{ "dwm.floatselbordercolor", "Dwm.Floatselbordercolor", &colors[SchemeFloatSel][ColBorder], NULL },
};

/* tagging */
#define MAX_TAGNAME_LEN 14
#define TAG_PREPEND "%1i:"
#define MAX_TAGLEN 16
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",       NULL,     NULL,       0,            0,           -1 },
	{ "Firefox",    NULL,     NULL,       0,            0,           -1 },
	{ "XTerm",      NULL,     "foo",      0,            1,           -1 },
	{ "Msgcompose", NULL,     NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact    = 0.00; /* factor reserved at bottom of stack area */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|||",      col },     /* first entry is default */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",      deck },
	{ "HHH",      grid },
};

/* key definitions */
//#define MODKEY Mod4Mask
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucalc[] = { "/bin/sh", "-c", "xsel -o | dmenu_run -p Calculate: | xargs echo | bc 2>&1 | dmenu -p Answer: | xsel -i", NULL };
static const char *termcmd[]  = { "/bin/sh", "-c", "$HOME/scripts/colorshuffle", NULL };
//static const char *browscmd[] = SHCMD("/home/$USER/build/surf-git/src/surf/surf duckduckgo.com");
static const char *emailcmd[] = { "thunderbird", NULL };
static const char *lockcmd[]  = { "xscreensaver-command", "-lock", NULL };
static const char *taskcmd[]  = { "/bin/sh", "-c", "/home/$USER/scripts/todo", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
//	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = browscmd } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = emailcmd } },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = taskcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setsmfact,      {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setsmfact,      {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ControlMask,           XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ControlMask,           XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_semicolon, spawn,       {.v = dmenucalc } },
	{ MODKEY,                       XK_F5,     reloadcolors,   {0} },
	{ MODKEY,                       XK_n,      nametag,        {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
