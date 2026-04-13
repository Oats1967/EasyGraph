//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EditCtrl
///
/// @file   EditCtrl.h
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

#include "BASE/types.h"
#include "TKBoardInterface.h"


// CEditCtrl
enum class E_TYPCTRL : uint32_t 
{ 
	E_INTCTRL = 0, 
	E_FLOATCTRL, 
	E_STRINGCTRL, 
	E_UINT64CTRL
};


union DATATYPE
{
	char_t	szBuff[100];
	int32_t		iInt;
	float32_t	fFloat;
	uint64_t	iUInt64;
};



class CEditCtrl : public CEdit
{
	DECLARE_DYNAMIC(CEditCtrl)

protected:
	int32_t     m_ID;
    BOOL    m_bValue;

protected:
	static DATATYPE uVal;
    static int32_t m_cx;
    static int32_t m_cy;

protected:
	virtual void GetValue(void) = 0
    {}

	//------------------------------------------------------------------------------------
	///  @brief   OnKillFocus
	///
	///           This method ....
	///
	///  @param[in] pNewWnd , CWnd*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	//------------------------------------------------------------------------------------
	///  @brief   PreTranslateMessage
	///
	///           This method ....
	///
	///  @param[in] pMsg , MSG*
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL PreTranslateMessage(MSG* pMsg) override;

	//------------------------------------------------------------------------------------
	///  @brief   Init
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void Init(CWnd* pParent);

public:
	CEditCtrl(int32_t id) : m_ID(id), m_bValue(FALSE)
    {}
	virtual ~CEditCtrl();

	static CEditCtrl* m_pEdit;

	//------------------------------------------------------------------------------------
	///  @brief   GetInput
	///
	///           This method ....
	///
	///  @param[in] id , int32_t
	///  @param[in] eCtrl , E_TYPCTRL
	///  @param[in]  pParent , CWnd *
	///  @return CEditCtrl*
	///
	//------------------------------------------------------------------------------------
	static CEditCtrl* GetInput (CWnd *pParent, E_TYPCTRL, int32_t id);

	//------------------------------------------------------------------------------------
	///  @brief   GetLong
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	static int32_t		GetLong      (void);
	static int32_t		GetLongAbs	 (void);
	//------------------------------------------------------------------------------------
	///  @brief   GetFloat
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return float32_t
	///
	//------------------------------------------------------------------------------------
	static float32_t	GetFloat   (void);
	static float32_t	GetFloatAbs(void);

	//------------------------------------------------------------------------------------
	///  @brief   GetString
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return CString
	///
	//------------------------------------------------------------------------------------
	static CString		GetString    ( void);
	//------------------------------------------------------------------------------------
	///  @brief   ShowKeyboard
	///
	///           This method shows the keyboard
	///
	///  @param[in] nLayout , const int32_t
	///  @param[in] id , int32_t
	///  @param[in]  pParent , CWnd *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	static uint64_t		GetUint64(void);

	void	ShowNumericKeyboard (CWnd *pParent, int32_t id);
	void	ShowAlphaKeyboard();

	static void Destroy();

protected:
	DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CStringCtrl : public CEditCtrl
{
	protected:
		//------------------------------------------------------------------------------------
		///  @brief   GetValue
		///
		///           This method initiates a framework call to exchanges data with the controls
		///
		///  @param[in] none
		///  @return none
		///
		//------------------------------------------------------------------------------------
		void GetValue (void) override;
public:
	//------------------------------------------------------------------------------------
	///  @brief   CStringCtrl
	///
	///           Contstructor
	///
	///  @param[in] CWnd *pParent
	///  @param[in] int32_t _id
	///  @return none
	///
	//------------------------------------------------------------------------------------
	CStringCtrl (CWnd *pParent, int32_t id);
	~CStringCtrl(void) override = default;
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CNumericCtrl : public CEditCtrl
{
	protected:
		void GetValue (void) override;
public:
	//------------------------------------------------------------------------------------
	///  @brief   CNumericCtrl
	///
	///           Contstructor
	///
	///  @param[in] CWnd *pParent
	///  @param[in] int32_t _id
	///  @return none
	///
	//------------------------------------------------------------------------------------
	CNumericCtrl (CWnd *pParent, int32_t id);
	~CNumericCtrl (void) override = default;
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CUint64Ctrl : public CNumericCtrl
{
protected:
	void GetValue(void) override;
public:
	//------------------------------------------------------------------------------------
	///  @brief   CUint64Ctrl
	///
	///           Contstructor
	///
	///  @param[in] CWnd *pParent
	///  @param[in] int32_t _id
	///  @return none
	///
	//------------------------------------------------------------------------------------
	CUint64Ctrl(CWnd* pParent, int32_t id) : CNumericCtrl(pParent, id)
	{}
	~CUint64Ctrl(void) override = default;
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CFloatCtrl : public CNumericCtrl
{
	protected:
		//------------------------------------------------------------------------------------
		///  @brief   GetValue
		///
		///           This method initiates a framework call to exchanges data with the controls
		///
		///  @author  Detlef Hafer
		///
		///  @class        CFloatCtrl
		///
		///  @param[in] none
		///  @return none
		///
		//------------------------------------------------------------------------------------
		void GetValue (void);
public:
	//------------------------------------------------------------------------------------
	///  @brief   CFloatCtrl
	///
	///           Contstructor
	///
	///  @param[in] CWnd *pParent
	///  @param[in] int32_t _id
	///  @return none
	///
	//------------------------------------------------------------------------------------
	CFloatCtrl(CWnd* pParent, int32_t id) : CNumericCtrl(pParent, id)
	{}
	~CFloatCtrl (void) override = default;
};



