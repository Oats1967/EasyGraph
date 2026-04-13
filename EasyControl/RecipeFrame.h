//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeFrame
///
/// @file   RecipeFrame.h
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


class CRecipeFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CRecipeFrame)
public:
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


protected:
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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	CRecipeFrame();           // Dynamische Erstellung verwendet geschützten Konstruktor
	~CRecipeFrame() override = default;
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

	DECLARE_MESSAGE_MAP()
};



