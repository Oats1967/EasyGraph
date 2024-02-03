//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module wmuser.h
///
/// @file   wmuser.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once


#define WM_EASYGRAPH ( WM_USER + 0xFF)
#define WM_NEWDATE			                    (WM_EASYGRAPH + 1) // Send if application will be closed
#define WM_SETVIEW			                    (WM_EASYGRAPH + 2) // Send if application will be closed
#define WM_LINECOLOR			                (WM_EASYGRAPH + 3) // Send if application will be closed
#define WM_CATEGORY			                    (WM_EASYGRAPH + 4) // Send if application will be closed
#define WM_LINEWIDTH			                (WM_EASYGRAPH + 5) // Send if application will be closed
#define WM_VISIBLE			                    (WM_EASYGRAPH + 6) // Send if application will be closed

