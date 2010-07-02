// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/gui/log.hpp>
#include <boost/gui/event.hpp>
#include <windows.h>
#include <sstream>

#if defined(_DEBUG) || defined(BOOST_GUI_ENABLE_LOG)
#  if !defined(BOOST_GUI_LOG_TO_STREAM)
      std::ofstream boost::gui::log( "boost-gui-log.txt" );
#  endif
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3)
   int boost::detail::demangler::status = 0;
#endif

std::string boost::gui::win::get_message_string( UINT message )
{
   static const char *              szMessages[ WM_USER ] =
   {
      //           0x0000                       0x0001                  0x0002                      0x0003                    0x0004                    0x0005                0x0006                  0x0007                  0x0008                0x0009                0x000A                 0x000B              0x000C                0x000D                     0x000E                    0x000F
      /* 0x0000 */ "WM_NULL",                   "WM_CREATE",            "WM_DESTROY",               "WM_MOVE",                "<unknown>",              "WM_SIZE",            "WM_ACTIVATE",          "WM_SETFOCUS",          "WM_KILLFOCUS",       "<unknown>",          "WM_ENABLE",           "WM_SETREDRAW",     "WM_SETTEXT",         "WM_GETTEXT",              "WM_GETTEXTLENGTH",       "WM_PAINT",
      /* 0x0010 */ "WM_CLOSE",                  "WM_QUERYENDSESSION",   "WM_QUIT",                  "WM_QUERYOPEN",           "WM_ERASEBKGND",          "WM_STSCOLORCHANGE",  "WM_ENDSESSION",        "<unknown>",            "WM_SHOWWINDOW",      "<unknown>",          "WM_SETTINGCHANGE",    "WM_DEVMODECHANGE", "WM_ACTIVATEAPP",     "WM_FONTCHANGE",           "WM_TIMECHANGE",          "WM_CANCELMODE",
      /* 0x0020 */ "WM_SETCURSOR",              "WM_MOUSEACTIVATE",     "WM_CHILDACTIVATE",         "WM_QUEUESYNC",           "WM_GETMINMAXINFO",       "<unknown>",          "WM_PAINTICON",         "WM_ICONERASEBKGND",    "WM_NEXTDLGCTL",      "<unknown>",          "WM_SPOOLERSTATUS",    "WM_DRAWITEM",      "WM_MEASUREITEM",     "WM_DELETEITEM",           "WM_VKEYTOITEM",          "WM_CHARTOITEM",
      /* 0x0030 */ "WM_SETFONT",                "WM_GETFONT",           "WM_SETHOTKEY",             "WM_GETHOTKEY",           "<unknown>",              "<unknown>",          "<unknown>",            "WM_QUERYDRAGICON",     "<unknown>",          "WM_COMPAREITEM",     "<unknown>",           "<unknown>",        "<unknown>",          "WM_GETOBJECT",            "<unknown>",              "<unknown>",
      /* 0x0040 */ "<unknown>",                 "WM_COMPACTING",        "<unknown>",                "<unknown>",              "WM_COMMNOTIFY",          "<unknown>",          "WM_WINDOWPOSCHANGING", "WM_WINDOWPOSCHANGED",  "WM_POWER",           "<unknown>",          "WM_COPYDATA",         "WM_CANCELJOURNAL", "<unknown>",          "<unknown>",               "WM_NOTIFY",              "<unknown>",
      /* 0x0050 */ "WM_INPUTLANGCHANGEREQUEST", "WM_INPUTLANGCHANGE",   "WM_TCARD",                 "WM_HELP",                "WM_USERCHANGED",         "WM_NOTIFYFORMAT",    "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0060 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0070 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "WM_CONTEXTMENU",   "WM_STYLECHANGING",   "WM_STYLECHANGED",         "WM_DISPLAYCHANGE",       "WM_GETICON",
      /* 0x0080 */ "WM_SETICON",                "WM_NCCREATE",          "WM_NCDESTROY",             "WM_NCCALCSIZE",          "WM_NCHITTEST",           "WM_NCPAINT",         "WM_NCACTIVATE",        "WM_GETDLGCODE",        "WM_SYNCPAINT",       "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0090 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x00A0 */ "WM_NCMOUSEMOVE",            "WM_NCLBUTTONDOWN",     "WM_NCLBUTTONUP",           "WM_LBUTTONDBLCLK",       "WM_NCRBUTTONDOWN",       "WM_NCRBUTTONUP",     "WM_NCRBUTTONDBLCLK",   "WM_NCMBUTTONDOWN",     "WM_NCMBUTTONUP",     "WM_NCMBUTTONDBLCLK", "WM_UNINOWN",          "WM_NCXBUTTONDOWN", "WM_NCXBUTTONUP",     "WM_NCXBUTTONDBLCLK",      "<unknown>",              "<unknown>", 
      /* 0x00B0 */ "EM_GETSEL",                 "EM_SETSEL",            "EM_GETRECT",               "EM_SETRECT",             "EM_SETRECTNP",           "EM_SCROLL",          "EM_LINESCROLL",        "EM_SCROLLCARET",       "EM_GETMODIFY",       "EM_SETMODIFY",       "EM_LINECOUNT",        "EM_LINEINDEX",     "EM_SETHANDLE",       "EM_GETHANDLE",            "WM_GETTHUMB",            "<unknown>", 
      /* 0x00C0 */ "<unknown>",                 "EM_LINELENGTH",        "EM_REPLACESEL",            "<unknown>",              "EM_GETLINE",             "EM_SETLIMITTEXT",    "EM_CANUNDO",           "EM_UNDO",              "EM_FMTLINES",        "EM_LINEFROMCHAR",    "<unknown>",           "EM_SETTABSTOPS",   "EM_SETPASSWORDCHAR", "EM_EMPTYUNDOBUFFER",      "EM_GETFIRSTVISIBLELINE", "EM_SETREADONLY",
      /* 0x00D0 */ "EM_SETWORDBREAKPROC",       "EM_GETWORDBREAKPROC",  "EM_GETPASSWORDCHAR",       "EM_SETMARGINS",          "EM_GETMARGINS",          "EM_GETLIMITTEXT",    "EM_POSFROMCHAR",       "EM_CHARFROMPOS",       "EM_SETIMESTATUS",    "EM_GETIMESTATUS",    "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x00E0 */ "SBM_SETPOS",                "SBM_GETPOS",           "SBM_SETRANGE",             "SBM_GETRANGE",           "SBM_ENABLE_ARROWS",      "<unknown>",          "SBM_SETRANGEREDRAW",   "<unknown>",            "<unknown>",          "SBM_SETSCROLLINFO",  "SBM_GETSCROLLINFO",   "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x00F0 */ "BM_GETCHECK",               "BM_SETCHECK",          "BM_GETSTATE",              "BM_SETSTATE",            "BM_SETSTYLE",            "BM_CLICK",           "BM_GETIMAGE",          "BM_SETIMAGE",          "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "WM_INPUT", 
      //           0x0000                       0x0001                  0x0002                      0x0003                    0x0004                    0x0005                0x0006                  0x0007                  0x0008                0x0009                0x000A                 0x000B              0x000C                0x000D                     0x000E                    0x000F
      /* 0x0100 */ "WM_KEYDOWN",                "WM_KEYUP",             "WM_CHAR",                  "WM_DRADCHAR",            "WM_SYSKEYDOWN",          "WM_SYSKEYUP",        "WM_SYSCHAR",           "WM_SYSDEADCHAR",       "<unknown>",          "WM_UNICHAR",         "<unknown>",           "<unknown>",        "<unknown>",          "WM_IME_STARTCOMPOSITION", "WM_IME_ENDCOMPOSITION",  "WM_IME_COMPOSITION",
      /* 0x0110 */ "WM_INITDIALOG",             "WM_COMMAND",           "WM_SYSCOMMAND",            "WM_TIMER",               "WM_HSCROLL",             "WM_VSCROLL",         "WM_INITMENU",          "WM_INITMENUPOPUP",     "WM_SYSTIMER",        "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "WM_MENUSELECT",
      /* 0x0120 */ "WM_MENUCHAR",               "WM_ENTERIDLE",         "WM_MENURBUTTONUP",         "WM_MENUDRAG",            "WM_MENUGETOBJECT",       "WM_UNINITMENUPOPUP", "WM_MENUCOMMAND",       "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "WM_CHANGEUISTATE",        "WM_UPDATEUISTATE",       "WM_QUERYUISTATE",
      /* 0x0130 */ "<unknown>",                 "<unknown>",            "WM_CTLCOLORMSGBOX",        "WM_CTLCOLOREDIT",        "WM_CTLCOLORLISTBOX",     "WM_CTLCOLORBTN",     "WM_CTLCOLORDLG",       "WM_CTLCOLORSCROLLBAR", "WM_CTLCOLORSTATIC",  "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0140 */ "CB_GETEDITSEL",             "CB_LIMITTEXT",         "CB_SETEDITSEL",            "CB_ADDSTRING",           "CB_DELETESTRING",        "CB_DIR",             "CB_GETCOUNT",          "CB_GETCURSEL",         "CB_GETLBTEXT",       "CB_GETLBTEXTLEN",    "CB_INSERTSTRING",     "CB_RESETCONTENT",  "CB_FINDSTRING",      "CB_SELECTSTRING",         "CB_SETCURSEL",           "CB_SHOWDROPDOWN",
      /* 0x0150 */ "CB_GETITEMDATA",            "CB_SETITEMDATA",       "CB_GETDROPPEDCONTROLRECT", "CB_SETITEMHEIGHT",       "CB_GETITEMHEIGHT",       "CB_SETEXTENDEDUI",   "CB_GETEXTENDEDUI",     "CB_GETDROPPEDSTATE",   "CB_FINDSTRINGEXACT", "CB_SETLOCALE",       "CB_GETLOCALE",        "CB_GETTOPINDEX",   "CB_SETTOPINDEX",     "CB_GETHORIZONTALEXTENT",  "CB_SETHORIZONTALEXTENT", "CB_GETDROPPEDWIDTH",
      /* 0x0160 */ "CB_SETDROPPEDWIDTH",        "CB_INITSTORAGE",       "<unknown>",                "CB_MULTIPLEADDSTRING",   "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0170 */ "STM_SETICON",               "STM_GETICON",          "STM_SETIMAGE",             "STM_GETIMAGE",           "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0180 */ "LB_ADDSTRING",              "LB_INSERTSTRING",      "LB_DELETESTRING",          "LB_SELITEMRANGEEX",      "LB_RESETCONTENT",        "LB_SETSEL",          "LB_SETCURSEL",         "LB_GETSEL",            "LB_GETCURSEL",       "LB_GETTEXT",         "LB_GETTEXTLEN",       "LB_GETCOUNT",      "LB_SELECTSTRING",    "LB_DIR",                  "LB_GETTOPINDEX",         "LB_FINDSTRING",
      /* 0x0190 */ "LB_GETSELCOUNT",            "LB_GETSELITEMS",       "LB_SETTABSTOPS",           "LB_GETHORIZONTALEXTENT", "LB_SETHORIZONTALEXTENT", "LB_SETCOLUMNWIDTH",  "LB_ADDFILE",           "LB_SETTOPINDEX",       "LB_GETITEMRECT",     "LB_GETITEMDATA",     "LB_SETITEMDATA",      "LB_SELITEMRANGE",  "LB_SETANCHORINDEX",  "LB_GETANCHORINDEX",       "LB_SETCARETINDEX",       "LB_GETCARETINDEX",
      /* 0x01A0 */ "LB_SETITEMHEIGHT",          "LB_GETITEMHEIGHT",     "LB_FINDSTRINGEXACT",       "<unknown>",              "<unknown>",              "LB_SETLOCALE",       "LB_GETLOCALE",         "LB_SETCOUNT",          "LB_INITSTORAGE",     "LB_ITEMFROMPOINT",   "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x01B0 */ "<unknown>",                 "LB_MULTIPLEADDSTRING", "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x01C0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>",
      /* 0x01D0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x01E0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x01F0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>",
      //           0x0000                       0x0001                  0x0002                      0x0003                    0x0004                    0x0005                0x0006                  0x0007                  0x0008                0x0009                0x000A                 0x000B              0x000C                0x000D                     0x000E                    0x000F
      /* 0x0200 */ "WM_MOUSEMOVE",              "WM_LBUTTONDOWN",       "WM_LBUTTONUP",             "WM_LBUTTONDBLCLK",       "WM_RBUTTONDOWN",         "WM_RBUTTONUP",       "WM_RBUTTONDBLCLK",     "WM_MBUTTONDOWN",       "WM_MBUTTONUP",       "WM_MBUTTONDBLCLK",   "WM_MOUSEWHEEL",       "WM_XBUTTONDOWN",   "WM_XBUTTONUP",       "WM_XBUTTONDBLCLK",        "<unknown>",              "<unknown>", 
      /* 0x0210 */ "WM_PARENTNOTIFY",           "WM_ENTERMENULOOP",     "WM_EXITMENULOOP",          "WM_NEXTMENU",            "WM_SIZING",              "WM_CAPTURECHANGED",  "WM_MOVING",            "<unknown>",            "WM_POWERBROADCAST",  "WM_DEVICECHANGE",    "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0220 */ "WM_MDICREATE",              "WM_MDIDESTROY",        "WM_MDIACTIVATE",           "WM_MDIRESTORE",          "WM_MDINEXT",             "WM_MDIMAXIMIZE",     "WM_MDITILE",           "WM_MDICASCADE",        "WM_MDIICONARRANGE",  "WM_MDIGETACTIVE",    "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0230 */ "WM_MDISETMENU",             "WM_ENTERSIZEMOVE",     "WM_EXITSIZEMOVE",          "WM_DROPFILES",           "WM_MDIREFRESHMENU",      "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0240 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0250 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0260 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0270 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0280 */ "<unknown>",                 "WM_IME_SETCONTEXT",    "WM_IME_NOTIFY",            "WM_IME_CONTROL",         "WM_IME_COMPOSITIONFULL", "WM_IME_REQUEST",     "WM_IME_CHAR",          "<unknown>",            "WM_IME_SELECT",      "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0290 */ "WM_IME_KEYDOWN",            "WM_IME_KEYUP",         "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02A0 */ "WM_NCMOUSEHOVER",           "WM_MOUSEHOVER",        "WM_NCMOUSELEAVE",          "WM_MOUSELEAVE",          "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02B0 */ "<unknown>",                 "WM_WTSSESSION_CHANGE", "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02C0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02D0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02E0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x02F0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      //           0x0000                       0x0001                  0x0002                      0x0003                    0x0004                    0x0005                0x0006                  0x0007                  0x0008                0x0009                0x000A                 0x000B              0x000C                0x000D                     0x000E                    0x000F
      /* 0x0300 */ "WM_CUT",                    "WM_COPY",              "WM_PASTE",                 "WM_CLEAR",               "WM_UNDO",                "WM_RENDERFORMAT",    "WM_RENDERALLFORMATS",  "WM_DESTROYCLIPBOARD",  "WM_DRAWCLIPBOARD",   "WM_PAINTCLIPBOARD",  "WM_VSCROLLCLIPBOARD", "WM_SIZECLIPBOARD", "WM_ASKCBFORMATNAME", "WM_CHANGECBCHAIN",        "WM_HSCROLLCLIPBOARD",    "WM_QUERYNEWPALETTE",
      /* 0x0310 */ "WM_PALETTEISCHANGING",      "WM_PALETTECHANGED",    "WM_HOTKEY",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "WM_PRINT",             "WM_PRINTCLIENT",     "WM_APPCOMMAND",      "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0320 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0330 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0340 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0350 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0360 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0370 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0380 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x0390 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03A0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03B0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03C0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03D0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03E0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      /* 0x03F0 */ "<unknown>",                 "<unknown>",            "<unknown>",                "<unknown>",              "<unknown>",              "<unknown>",          "<unknown>",            "<unknown>",            "<unknown>",          "<unknown>",          "<unknown>",           "<unknown>",        "<unknown>",          "<unknown>",               "<unknown>",              "<unknown>", 
      //           0x0000                       0x0001                  0x0002                      0x0003                    0x0004                    0x0005                0x0006                  0x0007                  0x0008                0x0009                0x000A                 0x000B              0x000C                0x000D                     0x000E                    0x000F
   };

   std::ostringstream oss;

   if( message < WM_USER )
   {
      if( std::strcmp( szMessages[ message ], "<unknown>" ) != 0 )
         oss << szMessages[ message ] << ' ';
   }
   else if( message < WM_APP )
   {
      oss << "WM_USER + ";
      message -= WM_USER;
   }

   oss << "[0x" << std::hex << message << std::dec << "]";
   return oss.str();
}