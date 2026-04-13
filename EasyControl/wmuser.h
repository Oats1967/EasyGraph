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


#define WM_CSKEYBOARD        (WM_USER + 192)
#define WM_CSKEYBOARDMOVE    (WM_USER + 193)
#define WM_CSKEYBOARDRESIZE  (WM_USER + 197)

#define WM_EASYCONTROL ( WM_USER + 0xFF)
#define WM_DOSEEND			                    (WM_EASYCONTROL + 1) // Send if application will be closed
#define WM_NEW_DOSEITEM		                    (WM_EASYCONTROL + 2) // Send if new item was focussed, -1 will close window
#define WM_TIMER_REFRESH	                    (WM_EASYCONTROL + 3) // Time 1 second to refresh window
#define WM_LAYOUTCHANGED		                (WM_EASYCONTROL + 4) // Layout changed
#define WM_NEW_RECITEM				            (WM_EASYCONTROL + 5) // Send if new Recorder item was created
#define WM_NEW_LOGITEM				            (WM_EASYCONTROL + 6) // Send if new Log item was created
#define WM_ALARMSHUTDOWN		                (WM_EASYCONTROL + 7)
#define WM_UPDATERECIPE			                (WM_EASYCONTROL + 8)
#define WM_PROPERTYPAGE_CHANGED                 (WM_EASYCONTROL + 9)	// Message to incicate the path changed
#define WM_RECIPENEW			                (WM_EASYCONTROL + 10)	// Message to incicate the path changed
#define WM_DIAGRAMBAR_DIAGRAMTYPE               (WM_EASYCONTROL + 11)	// Message to incicate the path changed
#define WM_DIAGRAMBAR_SELECT                    (WM_EASYCONTROL + 12)	// Message to incicate the path changed
#define WM_DIAGRAMBAR_DOSEITEM                  (WM_EASYCONTROL + 13)	// Message to incicate the path changed
#define WM_DIAGRAMBAR_TIME                      (WM_EASYCONTROL + 14)	// Message to incicate the path changed
#define WM_DIAGRAMBAR_DATE                      (WM_EASYCONTROL + 15)	// Message to incicate the path changed
#define WM_SAMPLETIME                           (WM_EASYCONTROL + 16)
#define WM_MASSFLOWSPEICHERN                    (WM_EASYCONTROL + 17)  // Send by DiagramFrame to active view if toolbar button was pressed
#define WM_VERWIEGUNGCHANGED                    (WM_EASYCONTROL + 18)
#define WM_LOGTIME                              (WM_EASYCONTROL + 19)
#define WM_ACTIVATE_VIEW		                (WM_EASYCONTROL + 20)	// Message to incicate the path changed
#define WM_DEACTIVATE_VIEW		                (WM_EASYCONTROL + 21)	// Message to incicate the path changed
#define WM_SUBDIALOG_CLOSED                     (WM_EASYCONTROL + 22)	// Message to incicate the path changed
#define WM_DATETIMECTRL                         (WM_EASYCONTROL + 23)	// Message to incicate the path changed
#define WM_LOGON                                (WM_EASYCONTROL + 24)	// Message to incicate the path changed
#define WM_LOGOUT                               (WM_EASYCONTROL + 25)	// Message to incicate the path changed
#define WM_INFOBOX_ALARMCLEAR                   (WM_EASYCONTROL + 26)	// Message sent by InfoBox to indicate that message was clicked
#define WM_INFOBOX_FLASHLIGHT                   (WM_EASYCONTROL + 27)	// Message sent by InfoBox to indicate that flash light was clicked
#define WM_SELECT_ITEM                          (WM_EASYCONTROL + 28)	// Message sent by InfoBox to indicate that flash light was clicked
#define WM_CONNECTIONLOST                       (WM_EASYCONTROL + 29)	// 
#define WM_NOTIFY_ALARMERROR                    (WM_EASYCONTROL + 30)	// 
#define WM_ZOOMFACTOR                           (WM_EASYCONTROL + 31)	// 
#define WM_PRODUCTKEY                           (WM_EASYCONTROL + 32)	// 
#define WM_RECORDERERROR                        (WM_EASYCONTROL + 33)	// 
#define WM_SPEEDBOXCLOSED                       (WM_EASYCONTROL + 34)	// 
#define WM_SCALEBOXCLOSED                       (WM_EASYCONTROL + 35)	// 
#define WM_HELPINFOCLOSED                       (WM_EASYCONTROL + 36)	// 
#define WM_REFILLBOXCLOSED                      (WM_EASYCONTROL + 37)	// 
#define WM_IFSSETTINGBOXCLOSED                  (WM_EASYCONTROL + 38)	// 
#define WM_PRODUCTDATABASEBOXCLOSED             (WM_EASYCONTROL + 39)	// 
#define WM_PREPARENEWLAYOUT		                (WM_EASYCONTROL + 40) // Layout changed
#define WM_PRESELECT                            (WM_EASYCONTROL + 41) 
#define WM_NOTIFYSCALEBOX                       (WM_EASYCONTROL + 42) 
#define WM_NOTIFYSPEEDBOX                       (WM_EASYCONTROL + 43) 
#define WM_NOTIFYHELPINFOBOX                    (WM_EASYCONTROL + 44) 
#define WM_NOTIFYEDITBOX                        (WM_EASYCONTROL + 45) 
#define WM_RECIPEEDITLOSED                      (WM_EASYCONTROL + 46)	// 




#define WM_NOTIFYEDIT			                (WM_EASYCONTROL + 55)	// Message to incicate the path changed
#define WM_LICENCECHECK			                (WM_EASYCONTROL + 56)	// Message to incicate the path changed
#define MYWM_POSTINIT                           (WM_EASYCONTROL + 57)	// Message to incicate the path changed
#define WM_DESTROYSCALEBOX                      (WM_EASYCONTROL + 58) 
#define WM_DOSESERVERMESSAGE                    (WM_EASYCONTROL + 60)



