//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ChildFrm.h
///
/// @file   ChildFrm.h
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


class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)

// Generierte Funktionen für die Meldungstabellen
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
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	// Implementierung
public:
	CChildFrame() noexcept;
	~CChildFrame() override = default;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dc) const;
#endif
	DECLARE_MESSAGE_MAP()

};

