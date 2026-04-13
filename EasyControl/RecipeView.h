//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeView.h
///
/// @file   RecipeView.h
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

#include <array>
#include <map>
#include "EasyControl.h"
#include "BASE/include/DoseTypeMap.h"
#include "BASE/include/DoseStatistics.h"
#include "RecipeTypen.h"
#include "MoreButton.h"
#include "LessButton.h"
#include "NextPrevButton.h"
#include "ButtonSwitchCtrl.h"
#include "RecipeGaugeCtrl.h"



class CRecipeDoc;

// CRecipeView-Formularansicht
class CRecipeView : public CFormView
{
	DECLARE_DYNCREATE(CRecipeView)

	enum { IDD = IDD_RECIPE_FORM };

	enum class eCtrl : int32_t
	{
		eDoseType,
		eName,
		eQMNumber,
		eAnlage,
		eSetpoint,
		ePercentage,
		eMax,
	};

	enum class eExtCtrl : int32_t
	{
		eRefillMax,
		eRefillMin,
		eRefillAlarm,
		eTotalizer,
		eRegenerat,
		eDoseperformance,
		eMax,
	};

	struct sCtrlPlacement
	{
		int32_t cDisplayCount = 0;
		int32_t cSpaceBottom = 0;
		int32_t cMinEditFieldX = 0;
		int32_t cMinEditFieldY = 0;
		int32_t cMinStaticFieldX = 0;
		int32_t cMinStaticFieldY = 0;
		int32_t cMinLeftOffset = 0;
		int32_t cMinRightOffset = 0;
		int32_t cMinTopOffset = 0;
		int32_t dx = 0;
		int32_t	dy = 0;
		int32_t	topoffset = 0;
		int32_t	bottomoffset = 0;
		int32_t	leftoffset = 0;
		int32_t	rightoffset = 0;
		int32_t	ctrlsizeY = 0;
		int32_t	ctrlsizeX = 0;
		int32_t	BaseElements = 0;
		int32_t	ExtElements = 0;						// Wieviel zusätzliche Zeilen sollen angezeigt werden
		int32_t	count = 0;								// Anzahl der Dosierer
		eExtCtrl eStartExtCtrl = eExtCtrl::eRefillMax;  // Welches Attribut zuerst

		sCtrlPlacement() = default;

		BOOL IsInside(const eExtCtrl eAttrib) const
		{
			return _S32(eAttrib) >= _S32(eStartExtCtrl) && _S32(eAttrib) < _S32(eStartExtCtrl) + ExtElements;
		}
	};

	union ModifyFlags
	{
		struct
		{
			uint32_t    m_bNameChanged : 1;
			uint32_t    m_bQMNumberChanged : 1;
			uint32_t    m_bLineModeChanged : 1;
			uint32_t    m_bSetpointChanged : 1;
			uint32_t    m_bMischungChanged : 1;
			uint32_t    m_bDoseperformanceChanged : 1;
			uint32_t    m_bRefillMaxChanged : 1;
			uint32_t    m_bRefillMinChanged : 1;
			uint32_t    m_bRefillMinMinChanged : 1;
			uint32_t	m_bRecipeLineNameChanged : 1;
			uint32_t	m_bRecipeLineQMNumberChanged : 1;
			uint32_t	m_bRecipeLineSetpointChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};


	using ColorArray = std::array <std::pair<COLORREF, COLORREF>, _S32(eColorPair::eColMax)>;
	using Bitfield = std::array <ModifyFlags, GLOBALDOSEMAXCOUNT>;

private:
	const ColorArray c_BkTextColor;
	const std::array<CBrush, _S32(eColorPair::eColMax)> c_Brush;
	const base::CDoseTypeMap m_DoseTypemap;
	const std::array< std::pair<int32_t, int32_t>, _S32(eCtrl::eMax)> c_CtrlMap;
	const std::array< std::pair<int32_t, int32_t>, _S32(eExtCtrl::eMax)> c_ExtCtrlMap;
	std::array<CButtonSwitchCtrl, MAXRECIPECOUNT> c_chkRegenerat;
	std::array<RecipeItem_s, MAXRECIPECOUNT> m_Recipes;
	sCtrlPlacement m_Placement;

	base::CDoseStatistics m_DoseStatistics;

	CRecipeGaugeCtrl m_RecipeGaugeCtrl;

	Bitfield		 m_ModifyFlags;
	CNextButton		 m_NextButton;
	CPreviousButton	 m_PreviousButton;
	CLessButton		 m_LessButton;
	CMoreButton		 m_MoreButton;
	CBCGPStatic		 m_StRecipeName;
	CBCGPStatic		 m_StRecipeQMNumber;
	CBCGPStatic		 m_StLineMode;
	CBCGPStatic		 m_StSetpoint;
	CBCGPStatic		 m_StRegenerat;
	CBCGPStatic		 m_StPercentage;
	CBCGPStatic		 m_StDoseperformance;
	CBCGPStatic		 m_StRefillMax;
	CBCGPStatic		 m_StRefillMin;
	CBCGPStatic		 m_StRefillMinMin;
	CBCGPStatic		 m_StTotalizer;
	CBCGPStatic		 m_StLineName;
	CBCGPStatic		 m_StLineQMNumber;
	CBCGPStatic		 m_StLineSetpoint;
	CBCGPStatic		 m_StLineRecipeSum;

	CBitmap			m_aBitmapLocal;
	CBitmap			m_aBitmapLinie;
#ifdef __IMAGEBACKGROUND
	CBitmap			m_aBackground;
	CSize			m_aBackgroundSize;
#endif

	CFont			m_aIndexFont;
	CFont			m_aDescriptionFont;
	CString			m_szRecipeName;
	uint64_t		m_RecipeQMNumber;
	CString			m_szRecipeSumme;
	CString			m_szFilename;
	

	int32_t	m_StartPos;
	BOOL    m_bModifyAllowed;
	BOOL    m_bOpenAllowed;
	BOOL    m_bSaveAllowed;
	BOOL    m_bDeleteAllowed;
	BOOL    m_bActivateAllowed;


	float32_t       m_fRecipePercentage;
	float32_t       m_fRecipeSetpoint;
	float32_t		m_fLineSetpoint;
	float32_t       m_fRegeneratPercentage;
	BOOL			m_bModified;
	BOOL			m_bRecipeComplete;
	BOOL			m_bNewRecipe;
	BOOL			m_LayoutChanged;
	eRecipeState	m_RecipeState;
	int32_t			m_ProductSaveID;

private:
	void Init();
	void SetDlgItemText(int32_t id, int32_t text);
	void InitModifyFlags();
	void SortCtrlItems(const int32_t count);
	void CalcPlacements(const int32_t count);
	void CalcExtControlsPos();
	void CalcCtrlPos();

	//************************************************************************************
	//************************************************************************************

	//------------------------------------------------------------------------------------
	///  @brief   SetValue
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetValue (void);

	//------------------------------------------------------------------------------------
	///  @brief   SetLineSetpoint
	///
	///           This method ....
	///
	///  @param[in] f , float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetLineSetpoint (float32_t f);

	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method initiates a framework call to exchanges data with the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void GetValue (void);

	//------------------------------------------------------------------------------------
	///  @brief   SetAnlagenModus
	///
	///           This method ....
	///
	///  @param[in] _bLocalMode , BOOL
	///  @param[in] sItem , short
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetAnlagenModus(int32_t sItem, BOOL _bLocalMode);

	//------------------------------------------------------------------------------------
	///  @brief   SetColor
	///
	///           This method ....
	///
	///  @param[in] aCol_e , eColorPair
	///  @param[in] pDC , CDC*
	///  @return HBRUSH
	///
	//------------------------------------------------------------------------------------
	HBRUSH SetColor                 (CDC* , eColorPair);

	//------------------------------------------------------------------------------------
	///  @brief   SetItemColor
	///
	///           This method ....
	///
	///  @param[in] id , int32_t
	///  @param[in] pDC , CDC*
	///  @return HBRUSH
	///
	//------------------------------------------------------------------------------------
	HBRUSH SetItemColor             (CDC* , const uint32_t id, const BOOL);
	BOOL SetRecipeName(const int32_t index, const CString& value);
	BOOL SetRecipeQMNumber(const int32_t index, const uint64_t value);
	BOOL SetRecipeSetpoint(const int32_t index, const float32_t value);
	BOOL SetRecipePercentage(const int32_t index, const float32_t value);
	BOOL SetRecipeDosefactor(const int32_t id, const float32_t value);
	BOOL SetRecipeRefillMin(const int32_t id, const float32_t value);
	BOOL SetRecipeRefillMax(const int32_t id, const float32_t value);
	BOOL SetRecipeRefillMinMin(const int32_t id, const float32_t value);
	void SetRecipeDoseName(const int32_t index, const CString& value);
	void SetRecipeDoseQMNumber(const int32_t index, const uint64_t value);

	BOOL OnNotifyRecipeLineName();
	BOOL OnNotifyRecipeLineQMNumber();
	BOOL OnNotifyRecipeLineSetpoint();
	BOOL OnNotifyRecipeName(const int32_t index);
	BOOL OnNotifyRecipeQMNumber(const int32_t index);
	BOOL OnNotifyRecipeSetpoint(const int32_t index);
	BOOL OnNotifyRecipePercentage(const int32_t index);
	BOOL OnNotifyRecipeDosefactor(const int32_t index);
	BOOL OnNotifyRecipeRefillMin(const int32_t index);
	BOOL OnNotifyRecipeRefillMax(const int32_t index);
	BOOL OnNotifyRecipeRefillMinMin(const int32_t index);


protected:
	//------------------------------------------------------------------------------------
	///  @brief   DoDataExchange
	///
	///           Called by the framework to exchange data
	///
	///  @param[in] pDX , CDataExchange*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoDataExchange(CDataExchange* pDX) override;

	//------------------------------------------------------------------------------------
	///  @brief   OnInitialUpdate
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void OnInitialUpdate() override;

	void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;


public:
	CRecipeView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	~CRecipeView() override = default;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CRecipeDoc* GetDocument (void)
	{ return (CRecipeDoc*) CFormView :: GetDocument (); }

	const CRecipeDoc* GetDocument(void) const 
	{	return (CRecipeDoc*)CFormView::GetDocument();	}

	base::utils::CRecipeItemList& GetRecipeItemList()
	{
		auto pDoc = GetDocument();
		ASSERT(pDoc);
		return pDoc->GetRecipeItemList();
	}
	const base::utils::CRecipeItemList& GetRecipeItemList() const 
	{
		const auto pDoc = GetDocument();
		ASSERT(pDoc);
		return pDoc->GetRecipeItemList();
	}

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] _w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT	OnNotifyEdit(WPARAM, LPARAM);

	afx_msg void OnStnClickedRecipeLineName();
	afx_msg void OnStnClickedRecipeLineQMNumber();
	afx_msg void OnStnClickedRecipeLineSetpoint();


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedRecipeLineMode
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedRecipeLineMode(UINT nID);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedRecipeQMNumber
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedRecipeQMNumber(UINT nID);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedRecipeName
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedRecipeName(UINT);
	afx_msg void OnStnClickedDosierTyp(UINT nID);


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedFloat
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedFloat(UINT);

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedRecipeRegenerat
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedRecipeRegenerat(UINT);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeOpen
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnClickNameBt(UINT nID);
	afx_msg void OnClickKeyBt(UINT nID);

	afx_msg void OnRecipeOpen();
	afx_msg void OnUpdateRecipeOpen(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeSave
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------

	afx_msg void OnRecipeSave();
	afx_msg void OnUpdateRecipeSave(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeSaveAs
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnRecipeSaveAs();
	afx_msg void OnUpdateRecipeSaveAs(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeActivate
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnRecipeActivate();
	afx_msg void OnUpdateRecipeActivate(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeAccept
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnRecipeAccept();
	afx_msg void OnUpdateRecipeAccept(CCmdUI* pCmdUI);


	afx_msg void OnBnClickedRecipeMore();
	afx_msg void OnBnClickedRecipeLess();
	afx_msg void OnBnClickedRecipeNext();
	afx_msg void OnBnClickedRecipePrevious();



	//------------------------------------------------------------------------------------
	///  @brief   OnCtlColor
	///
	///           This method is called by the framework when a child control is about to
	///           be drawn
	///
	///  @param[in] nCtlColor , UINT
	///  @param[in] pWnd , CWnd*
	///  @param[in] pDC , CDC*
	///  @return HBRUSH
	///
	//------------------------------------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	LRESULT OnLayoutChanged(WPARAM, LPARAM);

	LRESULT OnUserLogOn(WPARAM, LPARAM);
	LRESULT OnUserLogOut(WPARAM, LPARAM);
	LRESULT OnActivateView(WPARAM, LPARAM);
	LRESULT OnDeactivateView(WPARAM, LPARAM);

#ifdef __IMAGEBACKGROUND
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
#endif
	afx_msg void OnSize(UINT  nType, int  cx, int  cy);


	LRESULT OnGaugeClick(WPARAM wp, LPARAM /*lp*/);
	LRESULT OnNewProductKey(WPARAM id, LPARAM);

	DECLARE_MESSAGE_MAP()
};


