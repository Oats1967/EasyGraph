//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmArchiveFrame
///
/// @file   AlarmArchiveFrame.h
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


class CAlarmArchiveFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CAlarmArchiveFrame)

private:

protected:
	//------------------------------------------------------------------------------------
	///  @brief   ActivateFrame
	///
	///           This method ....
	///
	///  @param[in] nCmdShow , int
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void ActivateFrame(int nCmdShow = -1) override;

	//------------------------------------------------------------------------------------
	///  @brief   PreCreateWindow
	///
	///           This method is called by the framework before the creation of the Windows 
	///		      window´attached to this CWnd object
	///
	///  @param[in] cs , CREATESTRUCT&
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int
	///
	//------------------------------------------------------------------------------------
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//------------------------------------------------------------------------------------
	///  @brief   OnDestroy
	///
	///           This method is called by the framework before the window is destroyed
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnDestroy();
public:
	CAlarmArchiveFrame() 
	{}
	~CAlarmArchiveFrame() override = default;

	void OnAlarmArchiveFrameTimeSelect();
	void OnAlarmArchiveFrameTimeSelect(CCmdUI* pCmdUI);


	DECLARE_MESSAGE_MAP()
};



