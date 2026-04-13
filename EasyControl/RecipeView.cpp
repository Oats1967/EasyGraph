//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeView
///
/// @file   RecipeView.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "RecipeHelper.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/EasyTime/public/EasyTime.h"
#include "RecipeDoc.h"
#include "RecipeView.h"
#include "RemoteControl.h"
#include "FormatMetric.h"
#include "MFCMacros.h"
#include "DataImage.h"
#include "EditCtrlEx.h"
#include "RecipeEditDlg.h"
#include "ToolTipDlg.h"



#define ISMODIFIEDFLOAT(id1, id2) (fabs(id1-id2) > 1e-04)
#define OFFSETRECIPE(_a) _U32(m_StartPos + (_a))



static_assert(IDC_RECIPE_ANLAGE2 == IDC_RECIPE_ANLAGE1 + 1, "IDC_RECIPE_ANLAGE2 failed");
static_assert(IDC_RECIPE_ANLAGE3 == IDC_RECIPE_ANLAGE1 + 2, "IDC_RECIPE_ANLAGE3 failed");
static_assert(IDC_RECIPE_ANLAGE4 == IDC_RECIPE_ANLAGE1 + 3, "IDC_RECIPE_ANLAGE4 failed");
static_assert(IDC_RECIPE_ANLAGE5 == IDC_RECIPE_ANLAGE1 + 4, "IDC_RECIPE_ANLAGE5 failed");
static_assert(IDC_RECIPE_ANLAGE6 == IDC_RECIPE_ANLAGE1 + 5, "IDC_RECIPE_ANLAGE6 failed");
static_assert(IDC_RECIPE_ANLAGE7 == IDC_RECIPE_ANLAGE1 + 6, "IDC_RECIPE_ANLAGE7 failed");
static_assert(IDC_RECIPE_ANLAGE8 == IDC_RECIPE_ANLAGE1 + 7, "IDC_RECIPE_ANLAGE8 failed");

static_assert(IDC_RECIPE_NAME2 == IDC_RECIPE_NAME1 + 1, "IDC_RECIPE_NAME2 failed");
static_assert(IDC_RECIPE_NAME3 == IDC_RECIPE_NAME1 + 2, "IDC_RECIPE_NAME3 failed");
static_assert(IDC_RECIPE_NAME4 == IDC_RECIPE_NAME1 + 3, "IDC_RECIPE_NAME4 failed");
static_assert(IDC_RECIPE_NAME5 == IDC_RECIPE_NAME1 + 4, "IDC_RECIPE_NAME5 failed");
static_assert(IDC_RECIPE_NAME6 == IDC_RECIPE_NAME1 + 5, "IDC_RECIPE_NAME6 failed");
static_assert(IDC_RECIPE_NAME7 == IDC_RECIPE_NAME1 + 6, "IDC_RECIPE_NAME7 failed");
static_assert(IDC_RECIPE_NAME8 == IDC_RECIPE_NAME1 + 7, "IDC_RECIPE_NAME8 failed");

static_assert(IDC_RECIPE_QMNUMBER2 == IDC_RECIPE_QMNUMBER1 + 1, "IDC_RECIPE_QMNUMBER2 failed");
static_assert(IDC_RECIPE_QMNUMBER3 == IDC_RECIPE_QMNUMBER1 + 2, "IDC_RECIPE_QMNUMBER3 failed");
static_assert(IDC_RECIPE_QMNUMBER4 == IDC_RECIPE_QMNUMBER1 + 3, "IDC_RECIPE_QMNUMBER4 failed");
static_assert(IDC_RECIPE_QMNUMBER5 == IDC_RECIPE_QMNUMBER1 + 4, "IDC_RECIPE_QMNUMBER5 failed");
static_assert(IDC_RECIPE_QMNUMBER6 == IDC_RECIPE_QMNUMBER1 + 5, "IDC_RECIPE_QMNUMBER6 failed");
static_assert(IDC_RECIPE_QMNUMBER7 == IDC_RECIPE_QMNUMBER1 + 6, "IDC_RECIPE_QMNUMBER7 failed");
static_assert(IDC_RECIPE_QMNUMBER8 == IDC_RECIPE_QMNUMBER1 + 7, "IDC_RECIPE_QMNUMBER8 failed");

static_assert(IDC_RECIPE_SETPOINT2 == IDC_RECIPE_SETPOINT1 + 1, "IDC_RECIPE_SETPOINT2 failed");
static_assert(IDC_RECIPE_SETPOINT3 == IDC_RECIPE_SETPOINT1 + 2, "IDC_RECIPE_SETPOINT3 failed");
static_assert(IDC_RECIPE_SETPOINT4 == IDC_RECIPE_SETPOINT1 + 3, "IDC_RECIPE_SETPOINT4 failed");
static_assert(IDC_RECIPE_SETPOINT5 == IDC_RECIPE_SETPOINT1 + 4, "IDC_RECIPE_SETPOINT5 failed");
static_assert(IDC_RECIPE_SETPOINT6 == IDC_RECIPE_SETPOINT1 + 5, "IDC_RECIPE_SETPOINT6 failed");
static_assert(IDC_RECIPE_SETPOINT7 == IDC_RECIPE_SETPOINT1 + 6, "IDC_RECIPE_SETPOINT7 failed");
static_assert(IDC_RECIPE_SETPOINT8 == IDC_RECIPE_SETPOINT1 + 7, "IDC_RECIPE_SETPOINT8 failed");

static_assert(IDC_RECIPE_PERCENTAGE2 == IDC_RECIPE_PERCENTAGE1 + 1, "IDC_RECIPE_PERCENTAGE2 failed");
static_assert(IDC_RECIPE_PERCENTAGE3 == IDC_RECIPE_PERCENTAGE1 + 2, "IDC_RECIPE_PERCENTAGE3 failed");
static_assert(IDC_RECIPE_PERCENTAGE4 == IDC_RECIPE_PERCENTAGE1 + 3, "IDC_RECIPE_PERCENTAGE4 failed");
static_assert(IDC_RECIPE_PERCENTAGE5 == IDC_RECIPE_PERCENTAGE1 + 4, "IDC_RECIPE_PERCENTAGE5 failed");
static_assert(IDC_RECIPE_PERCENTAGE6 == IDC_RECIPE_PERCENTAGE1 + 5, "IDC_RECIPE_PERCENTAGE6 failed");
static_assert(IDC_RECIPE_PERCENTAGE7 == IDC_RECIPE_PERCENTAGE1 + 6, "IDC_RECIPE_PERCENTAGE7 failed");
static_assert(IDC_RECIPE_PERCENTAGE8 == IDC_RECIPE_PERCENTAGE1 + 7, "IDC_RECIPE_PERCENTAGE8 failed");

static_assert(IDC_RECIPE_DOSEPERFORMANCE2 == IDC_RECIPE_DOSEPERFORMANCE1 + 1, "IDC_RECIPE_DOSEPERFORMANCE2 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE3 == IDC_RECIPE_DOSEPERFORMANCE1 + 2, "IDC_RECIPE_DOSEPERFORMANCE3 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE4 == IDC_RECIPE_DOSEPERFORMANCE1 + 3, "IDC_RECIPE_DOSEPERFORMANCE4 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE5 == IDC_RECIPE_DOSEPERFORMANCE1 + 4, "IDC_RECIPE_DOSEPERFORMANCE5 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE6 == IDC_RECIPE_DOSEPERFORMANCE1 + 5, "IDC_RECIPE_DOSEPERFORMANCE6 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE7 == IDC_RECIPE_DOSEPERFORMANCE1 + 6, "IDC_RECIPE_DOSEPERFORMANCE7 failed");
static_assert(IDC_RECIPE_DOSEPERFORMANCE8 == IDC_RECIPE_DOSEPERFORMANCE1 + 7, "IDC_RECIPE_DOSEPERFORMANCE8 failed");

static_assert(IDC_RECIPE_REFILL_MAX2 == IDC_RECIPE_REFILL_MAX1 + 1, "IDC_RECIPE_REFILL_MAX2 failed");
static_assert(IDC_RECIPE_REFILL_MAX3 == IDC_RECIPE_REFILL_MAX1 + 2, "IDC_RECIPE_REFILL_MAX3 failed");
static_assert(IDC_RECIPE_REFILL_MAX4 == IDC_RECIPE_REFILL_MAX1 + 3, "IDC_RECIPE_REFILL_MAX4 failed");
static_assert(IDC_RECIPE_REFILL_MAX5 == IDC_RECIPE_REFILL_MAX1 + 4, "IDC_RECIPE_REFILL_MAX5 failed");
static_assert(IDC_RECIPE_REFILL_MAX6 == IDC_RECIPE_REFILL_MAX1 + 5, "IDC_RECIPE_REFILL_MAX6 failed");
static_assert(IDC_RECIPE_REFILL_MAX7 == IDC_RECIPE_REFILL_MAX1 + 6, "IDC_RECIPE_REFILL_MAX7 failed");
static_assert(IDC_RECIPE_REFILL_MAX8 == IDC_RECIPE_REFILL_MAX1 + 7, "IDC_RECIPE_REFILL_MAX8 failed");

static_assert(IDC_RECIPE_REFILL_MIN2 == IDC_RECIPE_REFILL_MIN1 + 1, "IDC_RECIPE_REFILL_MIN2 failed");
static_assert(IDC_RECIPE_REFILL_MIN3 == IDC_RECIPE_REFILL_MIN1 + 2, "IDC_RECIPE_REFILL_MIN3 failed");
static_assert(IDC_RECIPE_REFILL_MIN4 == IDC_RECIPE_REFILL_MIN1 + 3, "IDC_RECIPE_REFILL_MIN4 failed");
static_assert(IDC_RECIPE_REFILL_MIN5 == IDC_RECIPE_REFILL_MIN1 + 4, "IDC_RECIPE_REFILL_MIN5 failed");
static_assert(IDC_RECIPE_REFILL_MIN6 == IDC_RECIPE_REFILL_MIN1 + 5, "IDC_RECIPE_REFILL_MIN6 failed");
static_assert(IDC_RECIPE_REFILL_MIN7 == IDC_RECIPE_REFILL_MIN1 + 6, "IDC_RECIPE_REFILL_MIN7 failed");
static_assert(IDC_RECIPE_REFILL_MIN8 == IDC_RECIPE_REFILL_MIN1 + 7, "IDC_RECIPE_REFILL_MIN8 failed");


static_assert(IDC_RECIPE_REFILL_ALARMLIMIT2 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 1, "IDC_RECIPE_REFILL_ALARMLIMIT2 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT3 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 2, "IDC_RECIPE_REFILL_ALARMLIMIT3 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT4 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 3, "IDC_RECIPE_REFILL_ALARMLIMIT4 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT5 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 4, "IDC_RECIPE_REFILL_ALARMLIMIT5 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT6 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 5, "IDC_RECIPE_REFILL_ALARMLIMIT6 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT7 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 6, "IDC_RECIPE_REFILL_ALARMLIMIT7 failed");
static_assert(IDC_RECIPE_REFILL_ALARMLIMIT8 == IDC_RECIPE_REFILL_ALARMLIMIT1 + 7, "IDC_RECIPE_REFILL_ALARMLIMIT8 failed");

static_assert(IDC_RECIPE_TOTALIZER2 == IDC_RECIPE_TOTALIZER1 + 1, "IDC_RECIPE_TOTALIZER2 failed");
static_assert(IDC_RECIPE_TOTALIZER3 == IDC_RECIPE_TOTALIZER1 + 2, "IDC_RECIPE_TOTALIZER3 failed");
static_assert(IDC_RECIPE_TOTALIZER4 == IDC_RECIPE_TOTALIZER1 + 3, "IDC_RECIPE_TOTALIZER4 failed");
static_assert(IDC_RECIPE_TOTALIZER5 == IDC_RECIPE_TOTALIZER1 + 4, "IDC_RECIPE_TOTALIZER5 failed");
static_assert(IDC_RECIPE_TOTALIZER6 == IDC_RECIPE_TOTALIZER1 + 5, "IDC_RECIPE_TOTALIZER6 failed");
static_assert(IDC_RECIPE_TOTALIZER7 == IDC_RECIPE_TOTALIZER1 + 6, "IDC_RECIPE_TOTALIZER7 failed");
static_assert(IDC_RECIPE_TOTALIZER8 == IDC_RECIPE_TOTALIZER1 + 7, "IDC_RECIPE_TOTALIZER8 failed");


IMPLEMENT_DYNCREATE(CRecipeView, CFormView)

CRecipeView::CRecipeView()
	: CFormView(CRecipeView::IDD)
	// eColNormal = 0,	eColModified,	eColError,	eColRegenerat,	eColRecipeEdit,	eColRecipeRegenerat,
	, c_CtrlMap{{
				{ -1					, IDC_RECIPE_DOSIERTYP1 },
				{ IDC_RECIPE_NAME_STATIC, IDC_RECIPE_NAME1 },
				{ IDC_RECIPE_QMNUMBER_STATIC, IDC_RECIPE_QMNUMBER1 },
				{ IDC_RECIPE_LINEMODE_STATIC, IDC_RECIPE_ANLAGE1 },
				{ IDC_RECIPE_SETPOINT_STATIC, IDC_RECIPE_SETPOINT1 },
				{ IDC_RECIPE_PERCENTAGE_STATIC, IDC_RECIPE_PERCENTAGE1 }}
		}
	, c_ExtCtrlMap{ {
		{ IDC_RECIPE_REFILL_MAX_STATIC, IDC_RECIPE_REFILL_MAX1 },
		{ IDC_RECIPE_REFILL_MIN_STATIC, IDC_RECIPE_REFILL_MIN1 },
		{ IDC_RECIPE_REFILL_ALARMLIMIT_STATIC, IDC_RECIPE_REFILL_ALARMLIMIT1 },
		{ IDC_RECIPE_TOTALIZER_STATIC, IDC_RECIPE_TOTALIZER1 },
		{ IDC_RECIPE_REGENERAT_STATIC, IDC_RECIPE_REGENERAT1 },
		{ IDC_RECIPE_DOSEPERFORMANCE_STATIC, IDC_RECIPE_DOSEPERFORMANCE1 }}
	}
	, c_BkTextColor{ { { BCCOLNORMAL, TCCOLNORMAL },  { BCCOLMODIFIED, TCCOLMODIFIED}, { BCCOLERROR, TCCOLERROR}, { BCCOLREGENERAT, TCCOLREGENERAT}, { BCCOLRECIPEEDIT, TCCOLRECIPEEDIT } , { BCCOLRECIPEREGENERAT, TCCOLRECIPEREGENERAT } } }
	, c_Brush { BCCOLNORMAL,  BCCOLMODIFIED, BCCOLERROR, BCCOLREGENERAT, BCCOLRECIPEEDIT, BCCOLRECIPEREGENERAT }
	, c_chkRegenerat {  CButtonSwitchCtrl(IDC_RECIPE_REGENERAT1),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT2),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT3),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT4),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT5),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT6),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT7),
					   CButtonSwitchCtrl(IDC_RECIPE_REGENERAT8) }
	, m_RecipeGaugeCtrl()
	, m_szRecipeName(_T(""))
	, m_RecipeQMNumber(0)
	, m_szRecipeSumme(_T(""))
	, m_szFilename("")
	, m_StartPos(0)
	, m_bModifyAllowed(FALSE)
	, m_bOpenAllowed(FALSE)
	, m_bSaveAllowed(FALSE)
	, m_bDeleteAllowed(FALSE)
	, m_bActivateAllowed(FALSE)
	, m_fRecipePercentage(0.0F)
	, m_fRecipeSetpoint(0.0F)
	, m_fLineSetpoint(0.0F)
	, m_fRegeneratPercentage(0.0F)
	, m_bModified(FALSE)
	, m_bRecipeComplete(FALSE)
	, m_bNewRecipe(FALSE)
	, m_RecipeState(eRecipeState::eRecipeStateNormal)
	, m_ProductSaveID{ -1 }
	, m_LayoutChanged{ FALSE }
{
	VERIFY(m_aIndexFont.CreateFont(
		30,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	VERIFY(m_aDescriptionFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename


	for (auto& item : m_Recipes)
	{
		item.Init();
	}
}
//******************************************************************************************************        
//******************************************************************************************************        
void CRecipeView::InitModifyFlags()
{
	ModifyFlags DefaultFlag;
	DefaultFlag.allflags = 0;
	m_ModifyFlags.fill(DefaultFlag);
}
//******************************************************************************************************        
//******************************************************************************************************        
void CRecipeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	int32_t index = 0;
    for (auto& item : m_Recipes)
    {
	    DDX_Text(pDX, IDC_RECIPE_NAME1+ index, item.m_szName);
	    DDX_Text(pDX, IDC_RECIPE_QMNUMBER1+ index, item.m_QMNumber);
	    DDX_Text(pDX, IDC_RECIPE_DOSIERTYP1+ index, item.m_szNr);
	    DDX_Percentage(pDX, IDC_RECIPE_PERCENTAGE1+ index, item.m_fRecipePercentage);
	    DDX_Float(pDX, IDC_RECIPE_SETPOINT1+ index, item.m_fSetpoint);
	    DDX_Float(pDX, IDC_RECIPE_REFILL_ALARMLIMIT1+ index, item.m_fRefillAlarmLimit);
	    DDX_Float(pDX, IDC_RECIPE_REFILL_MIN1+ index, item.m_fRefillMin);
	    DDX_Float(pDX, IDC_RECIPE_REFILL_MAX1+ index, item.m_fRefillMax);
	    DDX_Float(pDX, IDC_RECIPE_DOSEPERFORMANCE1+ index, item.m_fDosePerformance);
		DDX_Float(pDX, IDC_RECIPE_TOTALIZER1 + index, item.m_fTotalizer);
		DDX_Control(pDX, IDC_RECIPE_REGENERAT1 + index, c_chkRegenerat[index]);
		index++;
    }
	DDX_Text(pDX, IDC_RECIPE_LINE_NAME, m_szRecipeName);
	DDX_Text(pDX, IDC_RECIPE_LINE_QMNUMMER, m_RecipeQMNumber);
	DDX_Text(pDX, IDC_RECIPE_LINE_SETPOINT, m_fLineSetpoint);
	DDX_Text(pDX, IDC_RECIPE_LINE_SUM, m_szRecipeSumme);
	DDX_Control(pDX, IDC_RECIPE_MORE, m_MoreButton);
	DDX_Control(pDX, IDC_RECIPE_LESS, m_LessButton);
	DDX_Control(pDX, IDC_RECIPE_NEXT, m_NextButton);
	DDX_Control(pDX, IDC_RECIPE_PREVIOUS, m_PreviousButton);
	DDX_Control(pDX, IDC_RECIPE_NAME_STATIC, m_StRecipeName);
	DDX_Control(pDX, IDC_RECIPE_QMNUMBER_STATIC, m_StRecipeQMNumber);
	DDX_Control(pDX, IDC_RECIPE_LINEMODE_STATIC, m_StLineMode);
	DDX_Control(pDX, IDC_RECIPE_SETPOINT_STATIC, m_StSetpoint);
	DDX_Control(pDX, IDC_RECIPE_PERCENTAGE_STATIC, m_StPercentage);
	DDX_Control(pDX, IDC_RECIPE_DOSEPERFORMANCE_STATIC, m_StDoseperformance);
	DDX_Control(pDX, IDC_RECIPE_REFILL_MAX_STATIC, m_StRefillMax);
	DDX_Control(pDX, IDC_RECIPE_REFILL_MIN_STATIC, m_StRefillMin);
	DDX_Control(pDX, IDC_RECIPE_REFILL_ALARMLIMIT_STATIC, m_StRefillMinMin);
	DDX_Control(pDX, IDC_RECIPE_TOTALIZER_STATIC, m_StTotalizer);
	DDX_Control(pDX, IDC_RECIPE_REGENERAT_STATIC, m_StRegenerat);
	DDX_Control(pDX, IDC_RECIPE_RECIPESUM, m_RecipeGaugeCtrl);

	DDX_Control(pDX, IDC_RECIPE_RECIPESUM_STATIC, m_StLineRecipeSum);
	DDX_Control(pDX, IDC_RECIPE_LINE_NAME_STATIC, m_StLineName);
	DDX_Control(pDX, IDC_RECIPE_LINE_QMNUMMER_STATIC, m_StLineQMNumber);
	DDX_Control(pDX, IDC_RECIPE_LINE_SETPOINT_STATIC, m_StLineSetpoint);
}
//******************************************************************************************************        
//******************************************************************************************************        
BEGIN_MESSAGE_MAP(CRecipeView, CFormView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_NOTIFYEDIT, &CRecipeView::OnNotifyEdit)
	ON_MESSAGE(WM_LOGON, &CRecipeView::OnUserLogOn)
	ON_MESSAGE(WM_LOGOUT, &CRecipeView::OnUserLogOut)
	ON_MESSAGE(WM_ACTIVATE_VIEW, &CRecipeView::OnActivateView)
	ON_MESSAGE(WM_DEACTIVATE_VIEW, &CRecipeView::OnDeactivateView)
	ON_MESSAGE(WM_LAYOUTCHANGED, &CRecipeView::OnLayoutChanged)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_DOSIERTYP1, IDC_RECIPE_DOSIERTYP8, &CRecipeView::OnStnClickedDosierTyp)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_ANLAGE1, IDC_RECIPE_ANLAGE8, &CRecipeView::OnStnClickedRecipeLineMode)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_NAME1, IDC_RECIPE_NAME8, &CRecipeView::OnStnClickedRecipeName)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_QMNUMBER1, IDC_RECIPE_QMNUMBER8, &CRecipeView::OnStnClickedRecipeQMNumber)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_SETPOINT1, IDC_RECIPE_SETPOINT8, &CRecipeView::OnStnClickedFloat)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_PERCENTAGE1, IDC_RECIPE_PERCENTAGE8, &CRecipeView::OnStnClickedFloat)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_DOSEPERFORMANCE1, IDC_RECIPE_DOSEPERFORMANCE8, &CRecipeView::OnStnClickedFloat)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_REFILL_MAX1, IDC_RECIPE_REFILL_MAX8, &CRecipeView::OnStnClickedFloat)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_REFILL_MIN1, IDC_RECIPE_REFILL_MIN8, &CRecipeView::OnStnClickedFloat)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RECIPE_REFILL_ALARMLIMIT1, IDC_RECIPE_REFILL_ALARMLIMIT8, &CRecipeView::OnStnClickedFloat)

	ON_COMMAND(ID_TB_RECIPE_OPEN, &CRecipeView::OnRecipeOpen)
	ON_UPDATE_COMMAND_UI(ID_TB_RECIPE_OPEN, &CRecipeView::OnUpdateRecipeOpen)
	ON_COMMAND(ID_TB_RECIPE_SAVE, &CRecipeView::OnRecipeSave)
	ON_UPDATE_COMMAND_UI(ID_TB_RECIPE_SAVE, &CRecipeView::OnUpdateRecipeSave)
	ON_COMMAND(ID_TB_RECIPE_SAVE_AS, &CRecipeView::OnRecipeSaveAs)
	ON_UPDATE_COMMAND_UI(ID_TB_RECIPE_SAVE_AS, &CRecipeView::OnUpdateRecipeSaveAs)
	ON_COMMAND(ID_TB_RECIPE_ACTIVATE, &CRecipeView::OnRecipeActivate)
	ON_UPDATE_COMMAND_UI(ID_TB_RECIPE_ACTIVATE, &CRecipeView::OnUpdateRecipeActivate)
	ON_COMMAND(ID_TB_RECIPE_ACCEPT, &CRecipeView::OnRecipeAccept)
	ON_UPDATE_COMMAND_UI(ID_TB_RECIPE_ACCEPT, &CRecipeView::OnUpdateRecipeAccept)
	ON_BN_CLICKED(IDC_RECIPE_MORE, &CRecipeView::OnBnClickedRecipeMore)
	ON_BN_CLICKED(IDC_RECIPE_LESS, &CRecipeView::OnBnClickedRecipeLess)
	ON_BN_CLICKED(IDC_RECIPE_NEXT, &CRecipeView::OnBnClickedRecipeNext)
	ON_BN_CLICKED(IDC_RECIPE_PREVIOUS, &CRecipeView::OnBnClickedRecipePrevious)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_RECIPE_LINE_NAME, &CRecipeView::OnStnClickedRecipeLineName)
	ON_STN_CLICKED(IDC_RECIPE_LINE_QMNUMMER, &CRecipeView::OnStnClickedRecipeLineQMNumber)
	ON_STN_CLICKED(IDC_RECIPE_LINE_SETPOINT, &CRecipeView::OnStnClickedRecipeLineSetpoint)
#ifdef __IMAGEBACKGROUND
	ON_WM_ERASEBKGND()
#endif
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_CLICK, OnGaugeClick)
	ON_MESSAGE(WM_PRODUCTKEY, OnNewProductKey)
END_MESSAGE_MAP()


//***************************************************************************************
//***************************************************************************************
LRESULT CRecipeView::OnNewProductKey(WPARAM id, LPARAM )
{
	SetFocus();
	auto pItem = PRODUCTDATABASEREF.GetItemID(_S32(id));
	if (pItem)
	{
		SetRecipeDoseName(m_ProductSaveID, toCString(pItem->GetProductName()));
		SetRecipeDoseQMNumber(m_ProductSaveID, pItem->GetQMNumber());
		SetValue();
	}
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
void CRecipeView::OnSize(UINT  , int  , int  )
{
	m_LayoutChanged = TRUE;
}

//************************************************************************************
//************************************************************************************
LRESULT CRecipeView::OnGaugeClick(WPARAM wp, LPARAM /*lp*/)
{
	UpdateData(TRUE);
	int32_t nID = _S32(wp);
	if (nID >= IDC_RECIPE_REGENERAT1 && nID < IDC_RECIPE_REGENERAT1 + MAXRECIPECOUNT)
	{
		OnBnClickedRecipeRegenerat(nID);
	}
	return 0L;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeLineName()
{
	auto value = CEditCtrl::GetString();
	BOOL bModified = (value != m_szRecipeName);
	if (bModified)
	{
		//m_szRecipeName = value;

		auto& rr = GetRecipeItemList();
		rr.SetName(toStdString(value));
		m_ModifyFlags[0].flags.m_bRecipeLineNameChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeLineQMNumber()
{
	auto value = CEditCtrl::GetUint64();
	BOOL bModified = (value != m_RecipeQMNumber);
	if (bModified)
	{
		//m_RecipeQMNumber = value;
		auto& rr = GetRecipeItemList();
		rr.SetQMNumber(value);
		m_ModifyFlags[0].flags.m_bRecipeLineQMNumberChanged = TRUE;
	}
	return bModified;
}

//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeLineSetpoint()
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	value = __max(value, 0.0F);
	BOOL bModified = (value != m_fLineSetpoint);
	if (bModified)
	{
		//m_fLineSetpoint = value;
		auto& rr = GetRecipeItemList();
		rr.SetLineSetpoint(value);
		SetLineSetpoint(value);
		m_ModifyFlags[0].flags.m_bRecipeLineSetpointChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetRecipeDoseName(const int32_t index, const CString& value)
{
	auto& rr = GetRecipeItemList();
	ASSERT(index < _S32(rr.GetCount()));
	auto& item = rr.GetItem(index);
	item.SetName(toStdString(value));
	m_ModifyFlags[index].flags.m_bNameChanged = TRUE;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeName(const int32_t id, const CString& value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_szName);
	if (bModified)
	{
		SetRecipeDoseName(OFFSETRECIPE(id), value);
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeName(const int32_t id)
{
	auto value = CEditCtrl::GetString();
	return SetRecipeName(id - IDC_RECIPE_NAME1, value);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetRecipeDoseQMNumber(const int32_t index, const uint64_t value)
{
	auto& rr = GetRecipeItemList();
	ASSERT(index < _S32(rr.GetCount()));
	auto& item = rr.GetItem(index);
	item.SetQMNumber(value);
	m_ModifyFlags[index].flags.m_bQMNumberChanged = TRUE;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeQMNumber(const int32_t id, const uint64_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_QMNumber);
	if (bModified)
	{
		SetRecipeDoseQMNumber(OFFSETRECIPE(id), value);
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeQMNumber(const int32_t id)
{
	auto value = CEditCtrl::GetUint64();
	return SetRecipeQMNumber(id - IDC_RECIPE_QMNUMBER1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeSetpoint(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fSetpoint);
	if (bModified)
	{
		const uint32_t index = OFFSETRECIPE(id);
		auto& rr = GetRecipeItemList();
		ASSERT(index < rr.GetCount());
		rr.UpdateSingleSetpoint(index, value);
		m_ModifyFlags[index].flags.m_bSetpointChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeSetpoint(const int32_t index)
{
	auto value = CEditCtrl::GetFloatAbs();
	return SetRecipeSetpoint(index - IDC_RECIPE_SETPOINT1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipePercentage(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fRecipePercentage);
	if (bModified)
	{
		auto& rr = GetRecipeItemList();
		const uint32_t index = OFFSETRECIPE(id);
		ASSERT(index < rr.GetCount());
		rr.UpdateEinzelAnteil(index, value);
		m_ModifyFlags[index].flags.m_bMischungChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipePercentage(const int32_t index)
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	return SetRecipePercentage(index - IDC_RECIPE_PERCENTAGE1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeDosefactor(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fDosePerformance);
	if (bModified)
	{
		const uint32_t index = OFFSETRECIPE(id);
		auto& rr = GetRecipeItemList();
		ASSERT(index < rr.GetCount());
		auto& item = rr.GetItem(index);
		item.SetDosePerformance(value);
		m_ModifyFlags[index].flags.m_bDoseperformanceChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeDosefactor(const int32_t index)
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	return SetRecipeDosefactor(index - IDC_RECIPE_DOSEPERFORMANCE1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeRefillMin(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fRefillMin);
	if (bModified)
	{
		const uint32_t index = OFFSETRECIPE(id);
		auto& rr = GetRecipeItemList();
		ASSERT(index < rr.GetCount());
		auto& item = rr.GetItem(id);
		item.SetRefillMin(value);
		m_ModifyFlags[index].flags.m_bRefillMinChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeRefillMin(const int32_t index)
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	return SetRecipeRefillMin(index - IDC_RECIPE_REFILL_MIN1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeRefillMax(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fRefillMax);
	if (bModified)
	{
		const uint32_t index = OFFSETRECIPE(id);
		auto& rr = GetRecipeItemList();
		ASSERT(index < rr.GetCount());
		auto& item = rr.GetItem(index);
		item.SetRefillMax(value);
		m_ModifyFlags[index].flags.m_bRefillMaxChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeRefillMax(const int32_t index)
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	return SetRecipeRefillMax(index - IDC_RECIPE_REFILL_MAX1, value);
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::SetRecipeRefillMinMin(const int32_t id, const float32_t value)
{
	ASSERT(id >= 0 && id < MAXRECIPECOUNT);
	BOOL bModified = (value != m_Recipes[id].m_fRefillAlarmLimit);
	if (bModified)
	{
		const uint32_t index = OFFSETRECIPE(id);
		auto& rr = GetRecipeItemList();
		ASSERT(index < rr.GetCount());
		auto& item = rr.GetItem(index);
		item.SetRefillAlarmLimit(value);
		m_ModifyFlags[index].flags.m_bRefillMinMinChanged = TRUE;
	}
	return bModified;
}
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnNotifyRecipeRefillMinMin(const int32_t index)
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	return SetRecipeRefillMinMin(index - IDC_RECIPE_REFILL_ALARMLIMIT1, value);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeLineMode(UINT nID)
{
	const uint32_t id = OFFSETRECIPE(nID - IDC_RECIPE_ANLAGE1);
	auto& rr = GetRecipeItemList();
	if (id < rr.GetCount())
	{
		GetValue();
		auto& item = rr.GetItem(id);
		BOOL bLocalMode = item.GetLocalMode();
		rr.UpdateLocalMode(id, !bLocalMode);
		m_bModified = TRUE;
		m_ModifyFlags[id].flags.m_bLineModeChanged = TRUE;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
LRESULT CRecipeView::OnNotifyEdit(WPARAM ID, LPARAM bChanged)
{
	BOOL bModified = FALSE;
	if (bChanged)
	{
		auto& rr = GetRecipeItemList();
		const int32_t lCount = _S32(rr.GetCount());
		const int32_t lID = _S32(ID);
		if (lID == IDC_RECIPE_LINE_NAME)
		{
			bModified = OnNotifyRecipeLineName();
		}
		else if (lID == IDC_RECIPE_LINE_QMNUMMER)
		{
			bModified = OnNotifyRecipeLineQMNumber();
		}
		else if (lID == IDC_RECIPE_LINE_SETPOINT)
		{
			bModified = OnNotifyRecipeLineSetpoint();
		}
		else if ((lID >= IDC_RECIPE_NAME1) && (lID < IDC_RECIPE_NAME1 + lCount))
		{
			bModified = OnNotifyRecipeName(lID);
		}
		else if ((lID >= IDC_RECIPE_QMNUMBER1) && (lID < IDC_RECIPE_QMNUMBER1 + lCount))
		{
			bModified = OnNotifyRecipeQMNumber(lID);
		}
		else if ((lID >= IDC_RECIPE_SETPOINT1) && (lID < IDC_RECIPE_SETPOINT1 + lCount))
		{
			bModified = OnNotifyRecipeSetpoint(lID);
		}
		else if ((lID >= IDC_RECIPE_PERCENTAGE1) && (lID < IDC_RECIPE_PERCENTAGE1 + lCount))
		{
			bModified = OnNotifyRecipePercentage(lID);
		}
		else if ((lID >= IDC_RECIPE_DOSEPERFORMANCE1) && (lID < IDC_RECIPE_DOSEPERFORMANCE1 + lCount))
		{
			bModified = OnNotifyRecipeDosefactor(lID);
		}
		else if ((lID >= IDC_RECIPE_REFILL_MIN1) && (lID < IDC_RECIPE_REFILL_MIN1 + lCount))
		{
			bModified = OnNotifyRecipeRefillMin(lID);
		}
		else if ((lID >= IDC_RECIPE_REFILL_MAX1) && (lID < IDC_RECIPE_REFILL_MAX1 + lCount))
		{
			bModified = OnNotifyRecipeRefillMax(lID);
		}
		else if ((lID >= IDC_RECIPE_REFILL_ALARMLIMIT1) && (lID < IDC_RECIPE_REFILL_ALARMLIMIT1 + lCount))
		{
			bModified = OnNotifyRecipeRefillMinMin(lID);
		}
		else
		{
		}
	}
	SetFocus();
	if (bModified)
	{
		SetValue();
		m_bModified = TRUE;
	}
	return 0L;
}

#ifdef _DEBUG
//------------------------------------------------------------------------------------
///  @brief   AssertValid
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CRecipeView
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void CRecipeView::AssertValid() const
{
	CFormView::AssertValid();
}

//------------------------------------------------------------------------------------
///  @brief   Dump
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CRecipeView
///
///  @param[in] dc , CDumpContext&
///  @return none
///
//------------------------------------------------------------------------------------
void CRecipeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeLineName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_RECIPE_LINE_NAME);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeLineQMNumber()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_UINT64CTRL, IDC_RECIPE_LINE_QMNUMMER);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeLineSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_RECIPE_LINE_SETPOINT);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeName(UINT nID)
{
	const uint32_t index = (nID - IDC_RECIPE_NAME1);

	auto pWnd = GetDlgItem(nID);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	m_ProductSaveID = OFFSETRECIPE(index);
	CRecipeEditDlg::CreateEditDlg(this, CPoint{ aRect.left - 20, aRect.top - 20 }, m_Recipes[index].m_szName, nID);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedRecipeQMNumber(UINT nID)
{
	const uint32_t index = (nID - IDC_RECIPE_QMNUMBER1);

	auto pWnd = GetDlgItem(nID);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	m_ProductSaveID = OFFSETRECIPE(index);
	CRecipeEditDlg::CreateEditDlg(this, CPoint{ aRect.left - 20, aRect.top - 20 }, m_Recipes[index].m_QMNumber, nID);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedDosierTyp(UINT nID)
{
	const uint32_t index = (nID - IDC_RECIPE_DOSIERTYP1);

	auto pWnd = GetDlgItem(nID);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CToolTipDlg::CreateInfo(this, CPoint{ aRect.left - 20, aRect.top - 60 }, m_Recipes[index].m_szDoseType);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnStnClickedFloat(UINT nID)
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, nID);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetValue () // Setzt die Eingabewerte aus der Rezepturliste
{
	const auto& rr = GetRecipeItemList();
	const uint32_t cCount = rr.GetCount();
	m_bRecipeComplete       = rr.CalcRecipeSum();
	m_fRecipePercentage		= ROUND2(rr.GetRecipePercentage());
	m_fRecipeSetpoint		= ROUND2(rr.GetRecipeSetpoint());
	m_fLineSetpoint			= ROUND2(rr.GetLineSetpoint());
	m_fRegeneratPercentage	= rr.GetRegeneratPercentage();
	m_szRecipeName			= toCString(rr.GetName());
	m_RecipeQMNumber		= rr.GetQMNumber();
	m_szRecipeSumme.Format(_T("%3.2f %%"), m_fRecipePercentage);

	ENABLE_SHOW_ID(IDC_RECIPE_LINE_NAME, m_bModifyAllowed, TRUE);
	ENABLE_SHOW_ID(IDC_RECIPE_LINE_QMNUMMER, m_bModifyAllowed, TRUE);
	ENABLE_SHOW_ID(IDC_RECIPE_LINE_SETPOINT, m_bModifyAllowed, TRUE);
	ENABLE_SHOW_ID(IDC_RECIPE_LINE_SUM, m_bModifyAllowed, TRUE);

	for (int32_t i  = 0; i < _S32(m_Recipes.size()); i++)
	{            
		BOOL result = FALSE;
		const uint32_t index = OFFSETRECIPE(i);
		if (index < cCount)
		{
			const auto& rRecipe = rr.GetItem(index);
			if ( base::__ISVALIDDOSETYPE(rRecipe.GetDoseType()))
			{
				result = TRUE;
				m_Recipes[i].m_szName = toCString(rRecipe.GetName());
				m_Recipes[i].m_QMNumber = rRecipe.GetQMNumber();
				m_Recipes[i].m_fDosePerformance = ROUND2(rRecipe.GetDosePerformance());
				m_Recipes[i].m_fRecipePercentage = ROUND2(rRecipe.GetRecipePercentage());
				m_Recipes[i].m_fSetpoint = ROUND2(rRecipe.GetSetpoint());
				m_Recipes[i].m_fRefillMax = ROUND2(rRecipe.GetRefillMax());
				m_Recipes[i].m_fRefillMin = ROUND2(rRecipe.GetRefillMin());
				m_Recipes[i].m_fRefillAlarmLimit = ROUND2(rRecipe.GetRefillAlarmLimit());
				m_Recipes[i].m_fTotalizer = rRecipe.GetTotalizer();

				m_Recipes[i].m_szNr.Format(_T("%d"), _S32(rRecipe.GetID() + 1));
				m_Recipes[i].m_szDoseType = m_Recipes[i].m_szNr + _T(" - ") + toCString(m_DoseTypemap.get(rRecipe.GetDoseType()));

				const BOOL bLWF		= base::__ISLWFTYPE(rRecipe.GetDoseType());
				const BOOL bWBF		= base::__ISWBFTYPE(rRecipe.GetDoseType());
				const BOOL bWBM		= base::__ISWBMTYPE(rRecipe.GetDoseType());
				const BOOL bBatcher = base::__ISBATCHERTYPE(rRecipe.GetDoseType());
				const BOOL bScale	= bLWF | bBatcher;
				const BOOL localMode = rRecipe.GetLocalMode();

				ENABLE_SHOW_ID(IDC_RECIPE_DOSIERTYP1 + i, (m_bModifyAllowed), TRUE);
				ENABLE_SHOW_ID(IDC_RECIPE_ANLAGE1 + i, (m_bModifyAllowed), TRUE);
				ENABLE_SHOW_ID(IDC_RECIPE_SETPOINT1 + i, (m_bModifyAllowed) && (!bWBM), (!bWBM));
				ENABLE_SHOW_ID(IDC_RECIPE_PERCENTAGE1 + i, (m_bModifyAllowed && (!bWBM) && (!localMode)), (!bWBM));
				ENABLE_SHOW_ID(IDC_RECIPE_NAME1 + i, (m_bModifyAllowed), TRUE);
				ENABLE_SHOW_ID(IDC_RECIPE_QMNUMBER1 + i, (m_bModifyAllowed), TRUE);

				ENABLE_SHOW_ID(IDC_RECIPE_REFILL_MAX1 + i,			m_bModifyAllowed, bScale && m_Placement.IsInside(eExtCtrl::eRefillMax));
				ENABLE_SHOW_ID(IDC_RECIPE_REFILL_MIN1 + i,			m_bModifyAllowed, bScale && m_Placement.IsInside(eExtCtrl::eRefillMin));
				ENABLE_SHOW_ID(IDC_RECIPE_REFILL_ALARMLIMIT1 + i,	m_bModifyAllowed, bScale && m_Placement.IsInside(eExtCtrl::eRefillAlarm));
				ENABLE_SHOW_ID(IDC_RECIPE_TOTALIZER1 + i,			m_bModifyAllowed,			 m_Placement.IsInside(eExtCtrl::eTotalizer));
				ENABLE_SHOW_ID(IDC_RECIPE_DOSEPERFORMANCE1 + i,		m_bModifyAllowed && (!bBatcher), (!bBatcher) && m_Placement.IsInside(eExtCtrl::eDoseperformance));
				ENABLE_SHOW_ID(IDC_RECIPE_REGENERAT1 + i,			(m_bModifyAllowed) && (!bWBM), (!bWBM)		 && m_Placement.IsInside(eExtCtrl::eRegenerat));
				SETBITMAP_ID(IDC_RECIPE_ANLAGE1 + i,				(localMode) ? &m_aBitmapLocal : &m_aBitmapLinie);

				c_chkRegenerat[i].SetOn(rRecipe.GetRegenerat());
			}
		}
		if ( ! result)
		{
			HIDE_ID(IDC_RECIPE_DOSIERTYP1 + i);
			HIDE_ID(IDC_RECIPE_ANLAGE1 + i);
			HIDE_ID(IDC_RECIPE_SETPOINT1 + i);
			HIDE_ID(IDC_RECIPE_PERCENTAGE1 + i);
			HIDE_ID(IDC_RECIPE_REFILL_MIN1 + i);
			HIDE_ID(IDC_RECIPE_REFILL_MAX1 + i);
			HIDE_ID(IDC_RECIPE_REFILL_ALARMLIMIT1 + i);
			HIDE_ID(IDC_RECIPE_TOTALIZER1 + i);
			HIDE_ID(IDC_RECIPE_NAME1 + i);
			HIDE_ID(IDC_RECIPE_QMNUMBER1 + i);
			HIDE_ID(IDC_RECIPE_DOSEPERFORMANCE1 + i);
			HIDE_ID(IDC_RECIPE_REGENERAT1 + i);

			m_Recipes[i].Init();
		}
	}
    ENABLE_SHOW_ID(IDC_RECIPE_REGENERAT_STATIC, m_bModifyAllowed, m_bModifyAllowed);
	ENABLE_SHOW_ID(IDC_RECIPE_PREVIOUS, BOOL(m_StartPos > 0), _S32(cCount) > MAXRECIPECOUNT)
	ENABLE_SHOW_ID(IDC_RECIPE_NEXT, BOOL(m_StartPos + MAXRECIPECOUNT < _S32(cCount)), _S32(cCount) > MAXRECIPECOUNT);
	BOOL bAllBatcher = m_DoseStatistics.IsAllBatcher();

	SHOWW_ID(IDC_RECIPE_DOSEPERFORMANCE_STATIC, (!bAllBatcher) && m_Placement.IsInside(eExtCtrl::eDoseperformance));
	SHOWW_ID(IDC_RECIPE_REFILL_MAX_STATIC, m_Placement.IsInside(eExtCtrl::eRefillMax));
	SHOWW_ID(IDC_RECIPE_REFILL_MIN_STATIC, m_Placement.IsInside(eExtCtrl::eRefillMin));
	SHOWW_ID(IDC_RECIPE_REFILL_ALARMLIMIT_STATIC, m_Placement.IsInside(eExtCtrl::eRefillAlarm));
	SHOWW_ID(IDC_RECIPE_TOTALIZER_STATIC, m_Placement.IsInside(eExtCtrl::eTotalizer));
	SHOWW_ID(IDC_RECIPE_REGENERAT_STATIC, m_Placement.IsInside(eExtCtrl::eRegenerat));

	m_RecipeState = (m_bRecipeComplete) ? eRecipeState::eRecipeStateComplete : eRecipeState::eRecipeStateNormal;

	ASSERT(_S32(c_ExtCtrlMap.size()) == _S32(eExtCtrl::eMax));
	BOOL bEnable = BOOL(_S32(m_Placement.eStartExtCtrl) + 1 + m_Placement.ExtElements <= _S32(eExtCtrl::eMax));
	ENABLE_ID(IDC_RECIPE_MORE, bEnable);
	m_MoreButton.Enable(bEnable);
	m_MoreButton.Show();

	bEnable = BOOL(_S32(m_Placement.eStartExtCtrl) > 0 && _S32(m_Placement.eStartExtCtrl) - 1 + m_Placement.ExtElements <= _S32(eExtCtrl::eMax));
	ENABLE_ID(IDC_RECIPE_LESS, bEnable);
	m_LessButton.Enable(bEnable);
	m_LessButton.Show();

	m_RecipeGaugeCtrl.SetPercentage(m_fRecipePercentage);

	Invalidate(FALSE);
	UpdateData(FALSE);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetLineSetpoint (float32_t f)
{
	base::LineProcessStatus rStatus;
	REMOTEREF.getLineProcessStatus(rStatus);

	const BOOL bExtLineSetpoint = rStatus.flags.StateExtlineSetpoint;
	if (bExtLineSetpoint)
	{
		REMOTEREF.getLineSetpoint(f);
	}
	m_fLineSetpoint = f;
	auto& rr = GetRecipeItemList();
	rr.UpdateLineSetpoint(f);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::GetValue() // Übernimmt die Eingabewerte in die Rezepturliste
{
	UpdateData(TRUE);

	auto& rr = GetRecipeItemList();
	rr.SetName(toStdString(m_szRecipeName));
	rr.SetQMNumber(m_RecipeQMNumber);
	rr.SetLineSetpoint(ROUND2(m_fLineSetpoint));

	for (uint32_t i = 0; i < MAXRECIPECOUNT; i++)
	{
		const uint32_t index = OFFSETRECIPE(i);
		if (index >= rr.GetCount())
		{
			break;
		}
		auto& rRecipe = rr.GetItem(index);
		if (! base::__ISVALIDDOSETYPE(rRecipe.GetDoseType()))
		{
			rRecipe.SetName("");
			rRecipe.SetQMNumber(0);
			rRecipe.SetSetpoint(0.0f);
			rRecipe.SetRecipePercentage(0.0f);
		}
		else
		{
			rRecipe.SetName(toStdString(m_Recipes[i].m_szName));
			rRecipe.SetQMNumber(m_Recipes[i].m_QMNumber);
			rRecipe.SetDosePerformance(ROUND2(m_Recipes[i].m_fDosePerformance));
			rRecipe.SetRecipePercentage(ROUND2(m_Recipes[i].m_fRecipePercentage));
			rRecipe.SetSetpoint(ROUND2(m_Recipes[i].m_fSetpoint));
			rRecipe.SetRefillMax(ROUND2(m_Recipes[i].m_fRefillMax));
			rRecipe.SetRefillMin(ROUND2(m_Recipes[i].m_fRefillMin));
			rRecipe.SetRefillAlarmLimit(ROUND2(m_Recipes[i].m_fRefillAlarmLimit));
			rRecipe.SetRegenerat(c_chkRegenerat[i].IsOn());
		}
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetDlgItemText(int32_t id, int32_t text)
{
	CString szTemp;
	VERIFY(szTemp.LoadString(text));
	CFormView::SetDlgItemText(id, szTemp);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::Init()
{
	m_bModifyAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_EDIT);
	m_bActivateAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_ACTIVATE);
	m_bSaveAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_SAVE);
	m_bDeleteAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_DELETE);
	m_bOpenAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_OPEN);

	m_bModified = FALSE;
	InitModifyFlags();

	auto& rr = GetRecipeItemList();
	recipehelper::CreateRecipeList(rr, m_szRecipeName, m_RecipeQMNumber);
	SetLineSetpoint(rr.GetLineSetpoint());
	rr.GetDoseStatistics(m_DoseStatistics);
	if (m_LayoutChanged)
	{
		m_LayoutChanged = FALSE;
		m_StartPos = 0;
		SortCtrlItems(_S32(rr.GetCount()));
	}

	const BOOL bAllBatcher = m_DoseStatistics.IsAllBatcher();
	SetDlgItemText(IDC_RECIPE_SETPOINT_STATIC, (bAllBatcher)?IDS_BATCHSIZE_KG: IDS_SETPOINT_KGH);
	SetDlgItemText(IDC_RECIPE_LINE_SETPOINT_STATIC, (bAllBatcher) ? IDS_LINEBATCHSIZE_KG : IDS_LINESETPOINT_KGH);

	UpdateData(TRUE);
	SetValue();
}

//************************************************************************************
//************************************************************************************
void CRecipeView::CalcPlacements(const int32_t count)
{
	auto& rP = m_Placement;
	rP.cDisplayCount = __min(MAXRECIPECOUNT, count);
	rP.cSpaceBottom = 10;
	rP.cMinEditFieldX = 80;
	rP.cMinEditFieldY = 12;
	rP.cMinStaticFieldX = 180;
	rP.cMinStaticFieldY = 12;
	rP.cMinLeftOffset = (count > 8) ? 180 : 140;
	rP.cMinRightOffset = (count > 4) ? 80 : 40;
	rP.cMinTopOffset = 80;
	rP.BaseElements = 6;

	CRect rRect;
	GetClientRect(rRect);
	rP.dx = rRect.Width();
	rP.dy = rRect.Height();
	rP.ExtElements = (rP.dy < 600) ? 1 : 2;

	const int32_t cTotalElements = rP.BaseElements + rP.ExtElements;
	rP.topoffset = std::max<int32_t>(rP.dy / 10, rP.cMinTopOffset);
	rP.bottomoffset = 0; 
	rP.leftoffset = std::max<int32_t>(rP.dx / 10, rP.cMinLeftOffset);
	rP.rightoffset = std::max<int32_t>(rP.leftoffset / 4, rP.cMinRightOffset);
	rP.ctrlsizeY = std::max<int32_t>(((rP.dy - (rP.bottomoffset + rP.topoffset)) / cTotalElements), rP.cMinEditFieldY);
	rP.ctrlsizeX = std::max<int32_t>(((rP.dx - (rP.leftoffset   + rP.rightoffset + rP.cMinStaticFieldX)) / rP.cDisplayCount), rP.cMinEditFieldX);
	rP.count = count;
}
//************************************************************************************
//************************************************************************************
void CRecipeView::CalcExtControlsPos()
{
	const auto& rP = m_Placement;
	// Extensionparameter
	int32_t y = rP.topoffset + _S32(eCtrl::eMax) * rP.ctrlsizeY;
	int32_t iRows = 0;
	for (int32_t j = 0; (j < _S32(c_ExtCtrlMap.size())); j++)
	{
		int32_t x = rP.leftoffset;
		const auto& rItem		= c_ExtCtrlMap[j];
		const auto& cStaticId	= rItem.first;
		const auto& cEl			= rItem.second;

		if (cStaticId > 0 )
		{
			CENTERPOSY_ID(cStaticId, x, y)
		}
		x += rP.cMinStaticFieldX;
		for (int32_t i = 0; i < rP.cDisplayCount; i++, x += rP.ctrlsizeX)
		{
			CENTERPOS_ID(cEl + i, x, y);
		}
		if (j >= _S32(rP.eStartExtCtrl) &&  (iRows < rP.ExtElements) )
		{
			y += rP.ctrlsizeY;
			iRows++;
		}
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::CalcCtrlPos()
{
	BOOL bAllBatcher = m_DoseStatistics.IsAllBatcher();

	const auto& rP = m_Placement;
	int32_t y = rP.topoffset;
	for (const auto& rItem : c_CtrlMap)
	{
		int32_t x = rP.leftoffset;
		const auto& cStaticId = rItem.first;
		if (bAllBatcher && (cStaticId == IDC_RECIPE_DOSEPERFORMANCE_STATIC))
		{
			continue;
		}
		if (cStaticId > 0)
		{
			CENTERPOSY_ID(cStaticId, x, y)
		}
		x += rP.cMinStaticFieldX;

		const auto& cEl = rItem.second;
		for (int32_t i = 0; i < rP.cDisplayCount; i++, x += rP.ctrlsizeX)
		{
			CENTERPOS_ID(cEl + i, x, y);
		}
		y += rP.ctrlsizeY;
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SortCtrlItems(const int32_t count)
{
	CalcPlacements(count);
	CalcCtrlPos();
	CalcExtControlsPos();

	// Arror next/previous
	const auto& rP = m_Placement;
	{
		int32_t y = (rP.dy - (rP.bottomoffset + rP.topoffset + 110)) / 2;
		int32_t x = rP.leftoffset - 60;
		SETPOS_ID(IDC_RECIPE_PREVIOUS, x, y);
		x = rP.leftoffset + rP.cMinStaticFieldX + rP.cDisplayCount * rP.ctrlsizeX;
		SETPOS_ID(IDC_RECIPE_NEXT, x, y);
	}

	{
		const int32_t c_MinOffsetY = 48;
		const int32_t c_MaxOffsetY = 150;

		int32_t x  = 18;
		int32_t y  = c_MinOffsetY;
		int32_t cx = 80;
		int32_t cy = std::max<int32_t>(rP.dy  - (c_MinOffsetY + c_MaxOffsetY), 0);
		SETPOSSIZE_ID(IDC_RECIPE_RECIPESUM, x,y, cx, cy);
		SETPOS_ID(IDC_RECIPE_LINE_SUM, x +  5,   y + cy + 10);
		SETPOS_ID(IDC_RECIPE_LESS, x  ,   y + cy + 60);
		SETPOS_ID(IDC_RECIPE_MORE, x + 50,   y + cy + 60);
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	for (int32_t i = 0; i < MAXRECIPECOUNT; i++)
	{
		SETFONT_ID(IDC_RECIPE_DOSIERTYP1 + i, &m_aIndexFont );
		SETFONT_ID(IDC_RECIPE_SETPOINT1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_NAME1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_QMNUMBER1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_PERCENTAGE1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_DOSEPERFORMANCE1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_REFILL_MAX1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_REFILL_MIN1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_REFILL_ALARMLIMIT1 + i, &m_aDescriptionFont);
		SETFONT_ID(IDC_RECIPE_TOTALIZER1 + i, &m_aDescriptionFont);

		CENTERTEXT_ID(IDC_RECIPE_DOSIERTYP1 + i);
		CENTERTEXT_ID(IDC_RECIPE_SETPOINT1 + i);
		CENTERTEXT_ID(IDC_RECIPE_NAME1 + i);
		CENTERTEXT_ID(IDC_RECIPE_QMNUMBER1 + i);
		CENTERTEXT_ID(IDC_RECIPE_PERCENTAGE1 + i);
		CENTERTEXT_ID(IDC_RECIPE_DOSEPERFORMANCE1 + i);
		CENTERTEXT_ID(IDC_RECIPE_REFILL_MAX1 + i);
		CENTERTEXT_ID(IDC_RECIPE_REFILL_MIN1 + i);
		CENTERTEXT_ID(IDC_RECIPE_REFILL_ALARMLIMIT1 + i);
		CENTERTEXT_ID(IDC_RECIPE_TOTALIZER1 + i);
	}
	SETFONT_ID(IDC_RECIPE_LINE_NAME, &m_aDescriptionFont);
	SETFONT_ID(IDC_RECIPE_LINE_QMNUMMER, &m_aDescriptionFont);
	SETFONT_ID(IDC_RECIPE_LINE_SETPOINT, &m_aDescriptionFont);
	SETFONT_ID(IDC_RECIPE_LINE_SUM, &m_aDescriptionFont);

	//CENTERTEXT_ID(IDC_RECIPE_LINE_NAME);
	//CENTERTEXT_ID(IDC_RECIPE_LINE_QMNUMMER);
	CENTERTEXT_ID(IDC_RECIPE_LINE_SETPOINT);
	CENTERTEXT_ID(IDC_RECIPE_LINE_SUM);


	m_StRecipeName.SetFont(&m_aDescriptionFont);
	m_StRecipeName.SizeToContent();
	m_StRecipeQMNumber.SetFont(&m_aDescriptionFont);
	m_StRecipeQMNumber.SizeToContent();
	m_StLineMode.SetFont(&m_aDescriptionFont);
	m_StLineMode.SizeToContent();
	m_StSetpoint.SetFont(&m_aDescriptionFont);
	m_StSetpoint.SizeToContent();
	m_StPercentage.SetFont(&m_aDescriptionFont);
	m_StPercentage.SizeToContent();
	m_StDoseperformance.SetFont(&m_aDescriptionFont);
	m_StDoseperformance.SizeToContent();
	m_StRefillMax.SetFont(&m_aDescriptionFont);
	m_StRefillMax.SizeToContent();
	m_StRefillMin.SetFont(&m_aDescriptionFont);
	m_StRefillMin.SizeToContent();
	m_StRefillMinMin.SetFont(&m_aDescriptionFont);
	m_StRefillMinMin.SizeToContent();
	m_StRegenerat.SetFont(&m_aDescriptionFont);
	m_StRegenerat.SizeToContent();
	m_StTotalizer.SetFont(&m_aDescriptionFont);
	m_StTotalizer.SizeToContent();

	m_StLineRecipeSum.SetFont(&m_aDescriptionFont);
	m_StLineRecipeSum.SizeToContent();
	m_StLineName.SetFont(&m_aDescriptionFont);
	m_StLineName.SizeToContent();
	m_StLineQMNumber.SetFont(&m_aDescriptionFont);
	m_StLineQMNumber.SizeToContent();
	m_StLineSetpoint.SetFont(&m_aDescriptionFont);
	m_StLineSetpoint.SizeToContent();

	VERIFY(m_aBitmapLocal.LoadBitmap(IDB_LOCALMODE_FOCUS) != 0);
	VERIFY(m_aBitmapLinie.LoadBitmap(IDB_LINE_FOCUS) != 0);
#ifdef __IMAGEBACKGROUND
	CPngImage image;
	VERIFY(image.Load(IDB_LOGOBRUSH, nullptr));
	VERIFY(m_aBackground.Attach(image.Detach()));
	BITMAP bm;
	VERIFY(m_aBackground.GetBitmap(&bm));
	m_aBackgroundSize = CSize(bm.bmWidth, bm.bmHeight);
#endif

	m_PreviousButton.SizeToContent();
	m_NextButton.SizeToContent();
	m_RecipeGaugeCtrl.Init();

	SetScrollSizes(MM_TEXT, CSize(0, 0));
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnRecipeOpen()
{
	TKB.TK_Show();

	auto& rr = GetRecipeItemList();
	m_szFilename = "";
	auto result = recipehelper::OnRecipeOpen(rr, m_szFilename);
	if ( result )
	{
		m_bModified = FALSE;
		InitModifyFlags();
		SetValue();
	}
	TKB.TK_Hide();
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnUpdateRecipeOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bOpenAllowed);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnRecipeSave()
{
	TKB.TK_Show();
	OnRecipeAccept();
	auto& rr = GetRecipeItemList();
	VERIFY(recipehelper::OnRecipeSave(rr, m_szFilename));
	TKB.TK_Hide();
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnUpdateRecipeSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((! m_szFilename.IsEmpty()) && m_bSaveAllowed);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnRecipeSaveAs()
{
	TKB.TK_Show();
	OnRecipeAccept();
	auto& rr = GetRecipeItemList();
	m_szFilename = toCString(rr.GetName());
	VERIFY(recipehelper::OnRecipeSaveAs(rr, m_szFilename));
	TKB.TK_Hide();
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnUpdateRecipeSaveAs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(	m_bSaveAllowed);
}
//************************************************************************************
//************************************************************************************
void CRecipeView::SetAnlagenModus(int32_t sItem, BOOL _bLocalMode)
{
	BeginWaitCursor();
	auto result = recipehelper::SetAnlagenModus(sItem, _bLocalMode);
	EndWaitCursor();
	if (!result)
	{
		CString szTemp, szTemp1, szTemp2;
		VERIFY(szTemp1.LoadString(IDS_DOSIERER));
		VERIFY(szTemp2.LoadString(IDS_DOSIERERNOSTOPPED));

		szTemp.Format(_T("%s %d %s"), LPCTSTR(szTemp1), int32_t(sItem + 1), LPCTSTR(szTemp2));
		ECMessageBox(szTemp, MB_ICONEXCLAMATION | MB_OK);
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnRecipeActivate()
{
	if ( ECMessageBox(IDS_QUERY_RECIPE_ACTIVATE, MB_YESNO|MB_ICONQUESTION   ) != IDYES)
	{
		return;
	}
	GetValue();
	auto& rr = GetRecipeItemList();
	auto result = recipehelper::OnRecipeActivate(rr);
	if ( result )
	{
		SetValue();
		InitModifyFlags();
		AfxGetMainWnd()->PostMessage(WM_UPDATERECIPE);
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnUpdateRecipeActivate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bRecipeComplete && (!m_bModified) && (m_bActivateAllowed));
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnRecipeAccept()
{
	GetValue();
	m_bModified = FALSE;
	// SetValue();
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnUpdateRecipeAccept(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bRecipeComplete && (m_bActivateAllowed));
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnBnClickedRecipeMore()
{
	ASSERT(_S32(c_ExtCtrlMap.size()) == _S32(eExtCtrl::eMax));
	int32_t StartExtCtrl = _S32(m_Placement.eStartExtCtrl);
	StartExtCtrl++;
	if (StartExtCtrl + m_Placement.ExtElements <= _S32(eExtCtrl::eMax))
	{
		m_Placement.eStartExtCtrl = eExtCtrl(StartExtCtrl);
		CalcExtControlsPos();
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnBnClickedRecipeLess()
{
	ASSERT(_S32(c_ExtCtrlMap.size()) == _S32(eExtCtrl::eMax));
	int32_t StartExtCtrl = _S32(m_Placement.eStartExtCtrl);
	if (StartExtCtrl > 0)
	{
		StartExtCtrl--;
		if (StartExtCtrl + m_Placement.ExtElements <= _S32(eExtCtrl::eMax))
		{
			m_Placement.eStartExtCtrl = eExtCtrl(StartExtCtrl);
			CalcExtControlsPos();
			SetValue();
		}
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnBnClickedRecipeNext()
{
	int32_t temp = m_StartPos;
	const auto& rr = GetRecipeItemList();
	int32_t sMaxItems = _S32(rr.GetCount());
	m_StartPos = __min(m_StartPos + 1, __max(sMaxItems - MAXRECIPECOUNT, 0));
	if (temp != m_StartPos)
	{
		SetValue();
	}
}
//************************************************************************************
//*******************************************Get*****************************************
void CRecipeView::OnBnClickedRecipePrevious()
{
	int32_t temp = m_StartPos;
	m_StartPos = __max(m_StartPos - 1, 0);
	if (temp != m_StartPos)
	{
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CRecipeView::OnBnClickedRecipeRegenerat(UINT )
{
	GetValue();
	auto& rr = GetRecipeItemList();
	rr.UpdateRegenerat();
	m_bModified = TRUE;
	SetValue();
}
//************************************************************************************
//************************************************************************************
HBRUSH CRecipeView::SetColor(CDC* pDC, eColorPair aCol_e)
{
    ASSERT( ( _S32(aCol_e) >= 0) && (_S32(aCol_e) < _S32(eColorPair::eColMax)));
	const auto& rItem = c_BkTextColor[_S32(aCol_e)];
	pDC->SetBkColor(rItem.first);
	pDC->SetTextColor(rItem.second);
    pDC->SetBkMode(OPAQUE);
    return c_Brush[_S32(aCol_e)];
}
//************************************************************************************
//************************************************************************************
HBRUSH CRecipeView::SetItemColor(CDC* pDC, uint32_t id, const BOOL bModified)
{
    eColorPair aCol_e = eColorPair::eColNormal;

	const auto& rr = GetRecipeItemList();
	if (id < rr.GetCount())
	{
		auto& item = rr.GetItem(id);
        if ( item.GetSetpoint() > item.GetMaxSetpoint() ||
            (item.GetSetpoint() > item.GetDosePerformance() ) )
        {
            aCol_e = eColorPair::eColError;

        }
		else if (bModified)
		{
			aCol_e = eColorPair::eColModified;
		}
		else if (item.GetRegenerat() )
        {
            aCol_e = eColorPair::eColRegenerat;
        }
		else
		{
			aCol_e = eColorPair::eColNormal;
		}
	}
	return SetColor(pDC, aCol_e);
}
#ifdef __IMAGEBACKGROUND
//************************************************************************************
//************************************************************************************
BOOL CRecipeView::OnEraseBkgnd(CDC* pDC)
{
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);


	auto pOld = memdc.SelectObject(&m_aBackground);

	CRect rc;
	GetClientRect(&rc);

	const int32_t x = ( rc.Width() -  m_aBackgroundSize.cx) / 2;
	const int32_t y = ( rc.Height() - m_aBackgroundSize.cy) / 2;
	pDC->FillSolidRect(rc, RGB(240, 240, 240));
	pDC->TransparentBlt(x, y, m_aBackgroundSize.cx, m_aBackgroundSize.cy, &memdc, 0, 0, m_aBackgroundSize.cx, m_aBackgroundSize.cy, RGB(255, 255, 255));
	memdc.SelectObject(pOld);
	return TRUE;
}
#endif
//************************************************************************************
//************************************************************************************

HBRUSH CRecipeView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    int32_t lID = pWnd->GetDlgCtrlID();
    switch (lID)
    {
	    case IDC_RECIPE_LINE_NAME:
								{
									const BOOL bModified = m_ModifyFlags[0].flags.m_bRecipeLineNameChanged;
									hbr = SetColor(pDC, (bModified) ? eColorPair::eColModified : eColorPair::eColNormal);
								}
								break;

		case IDC_RECIPE_LINE_QMNUMMER:
								{
									const BOOL bModified = m_ModifyFlags[0].flags.m_bRecipeLineQMNumberChanged;
									hbr = SetColor(pDC, (bModified) ? eColorPair::eColModified : eColorPair::eColNormal);
								}
								break;

		case IDC_RECIPE_DOSIERTYP1:
		case IDC_RECIPE_DOSIERTYP2:
		case IDC_RECIPE_DOSIERTYP3:
		case IDC_RECIPE_DOSIERTYP4:
		case IDC_RECIPE_DOSIERTYP5:
		case IDC_RECIPE_DOSIERTYP6:
		case IDC_RECIPE_DOSIERTYP7:
		case IDC_RECIPE_DOSIERTYP8:
								{
									pDC->SetTextColor(INDEXCOLOR);
									//pDC->SetBkColor(BCCOLNORMAL);
									pDC->SetBkMode(TRANSPARENT);
									//hbr = c_Brush[0];
								}
								break;

        case IDC_RECIPE_NAME1:
		case IDC_RECIPE_NAME2:
        case IDC_RECIPE_NAME3:
        case IDC_RECIPE_NAME4:
        case IDC_RECIPE_NAME5:
        case IDC_RECIPE_NAME6:
        case IDC_RECIPE_NAME7:
        case IDC_RECIPE_NAME8:
							{
								const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_NAME1);
								const BOOL bModified = m_ModifyFlags[id].flags.m_bNameChanged;
								hbr = SetColor(pDC, ( bModified) ? eColorPair::eColModified : eColorPair::eColNormal);
							}
							break;

        case IDC_RECIPE_QMNUMBER1:
        case IDC_RECIPE_QMNUMBER2:
        case IDC_RECIPE_QMNUMBER3:
        case IDC_RECIPE_QMNUMBER4:
        case IDC_RECIPE_QMNUMBER5:
        case IDC_RECIPE_QMNUMBER6:
        case IDC_RECIPE_QMNUMBER7:
        case IDC_RECIPE_QMNUMBER8:
							{
								const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_QMNUMBER1);
								const BOOL bModified = m_ModifyFlags[id].flags.m_bQMNumberChanged;
								hbr = SetColor(pDC, (bModified) ? eColorPair::eColModified : eColorPair::eColNormal);
							}
							break;

        case IDC_RECIPE_LINE_SETPOINT:
                            {
								const BOOL bModified = m_ModifyFlags[0].flags.m_bRecipeLineSetpointChanged;
								hbr = SetColor(pDC, (bModified) ? eColorPair::eColModified : eColorPair::eColNormal);
                            }
                            break;
#if 0
        case IDC_RECIPE_LINE_SUM :
                            {
	                            ASSERT((_S32(m_RecipeState) >= 0) && (_S32(m_RecipeState) < _S32(eRecipeState::eRecipeStateMax)));
                                eColorPair aCol_e = (m_RecipeState == eRecipeState::eRecipeStateNormal) ? eColorPair::eColRecipeEdit : eColorPair::eColRecipeRegenerat;
                                hbr = SetColor(pDC, aCol_e);
                            }
                            break;
#endif
	case IDC_RECIPE_PERCENTAGE1:
	case IDC_RECIPE_PERCENTAGE2:
	case IDC_RECIPE_PERCENTAGE3:
	case IDC_RECIPE_PERCENTAGE4:
	case IDC_RECIPE_PERCENTAGE5:
	case IDC_RECIPE_PERCENTAGE6:
	case IDC_RECIPE_PERCENTAGE7:
	case IDC_RECIPE_PERCENTAGE8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_PERCENTAGE1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bMischungChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;

	case IDC_RECIPE_SETPOINT1:
	case IDC_RECIPE_SETPOINT2:
	case IDC_RECIPE_SETPOINT3:
	case IDC_RECIPE_SETPOINT4:
	case IDC_RECIPE_SETPOINT5:
	case IDC_RECIPE_SETPOINT6:
	case IDC_RECIPE_SETPOINT7:
	case IDC_RECIPE_SETPOINT8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_SETPOINT1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bSetpointChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;
	case IDC_RECIPE_DOSEPERFORMANCE1:
	case IDC_RECIPE_DOSEPERFORMANCE2:
	case IDC_RECIPE_DOSEPERFORMANCE3:
	case IDC_RECIPE_DOSEPERFORMANCE4:
	case IDC_RECIPE_DOSEPERFORMANCE5:
	case IDC_RECIPE_DOSEPERFORMANCE6:
	case IDC_RECIPE_DOSEPERFORMANCE7:
	case IDC_RECIPE_DOSEPERFORMANCE8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_DOSEPERFORMANCE1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bDoseperformanceChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;

	case IDC_RECIPE_REFILL_MAX1:
	case IDC_RECIPE_REFILL_MAX2:
	case IDC_RECIPE_REFILL_MAX3:
	case IDC_RECIPE_REFILL_MAX4:
	case IDC_RECIPE_REFILL_MAX5:
	case IDC_RECIPE_REFILL_MAX6:
	case IDC_RECIPE_REFILL_MAX7:
	case IDC_RECIPE_REFILL_MAX8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_REFILL_MAX1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bRefillMaxChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;
	case IDC_RECIPE_REFILL_MIN1:
	case IDC_RECIPE_REFILL_MIN2:
	case IDC_RECIPE_REFILL_MIN3:
	case IDC_RECIPE_REFILL_MIN4:
	case IDC_RECIPE_REFILL_MIN5:
	case IDC_RECIPE_REFILL_MIN6:
	case IDC_RECIPE_REFILL_MIN7:
	case IDC_RECIPE_REFILL_MIN8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_REFILL_MIN1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bRefillMinChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;

	case IDC_RECIPE_REFILL_ALARMLIMIT1:
	case IDC_RECIPE_REFILL_ALARMLIMIT2:
	case IDC_RECIPE_REFILL_ALARMLIMIT3:
	case IDC_RECIPE_REFILL_ALARMLIMIT4:
	case IDC_RECIPE_REFILL_ALARMLIMIT5:
	case IDC_RECIPE_REFILL_ALARMLIMIT6:
	case IDC_RECIPE_REFILL_ALARMLIMIT7:
	case IDC_RECIPE_REFILL_ALARMLIMIT8:
									{
										const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_REFILL_ALARMLIMIT1);
										const BOOL bModified = m_ModifyFlags[id].flags.m_bRefillMinMinChanged;
										hbr = SetItemColor(pDC, id, bModified);
									}
									break;
	case IDC_RECIPE_TOTALIZER1:
	case IDC_RECIPE_TOTALIZER2:
	case IDC_RECIPE_TOTALIZER3:
	case IDC_RECIPE_TOTALIZER4:
	case IDC_RECIPE_TOTALIZER5:
	case IDC_RECIPE_TOTALIZER6:
	case IDC_RECIPE_TOTALIZER7:
	case IDC_RECIPE_TOTALIZER8:
	{
		const int32_t id = OFFSETRECIPE(lID - IDC_RECIPE_TOTALIZER1);
		hbr = SetItemColor(pDC, id, FALSE);
	}
	break;


	case IDC_RECIPE_NAME_STATIC:
	case IDC_RECIPE_QMNUMBER_STATIC:
	case IDC_RECIPE_LINEMODE_STATIC:
	case IDC_RECIPE_SETPOINT_STATIC:
	case IDC_RECIPE_PERCENTAGE_STATIC:
	case IDC_RECIPE_DOSEPERFORMANCE_STATIC:
	case IDC_RECIPE_REFILL_MAX_STATIC:
	case IDC_RECIPE_REFILL_MIN_STATIC:
	case IDC_RECIPE_REFILL_ALARMLIMIT_STATIC:
	case IDC_RECIPE_REGENERAT_STATIC:
	case IDC_RECIPE_TOTALIZER_STATIC:
	{
											// Set the text color to red
											pDC->SetTextColor(RGB(0, 0, 120));

											// Set the background mode for text to transparent 
											// so background will show thru.
											pDC->SetBkMode(TRANSPARENT);
									}
									break;
	}
    return hbr;
}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		LOGDEBUG("Activate CRecipeView !");
		AfxGetMainWnd()->SendMessage(WM_ACTIVATE_VIEW, WPARAM(eViewSelect::eRecipeView), 0);
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecipeView::OnLayoutChanged(WPARAM, LPARAM)
{
	m_LayoutChanged = TRUE;
	Init();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecipeView::OnUserLogOn(WPARAM, LPARAM)
{
	Init();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecipeView::OnUserLogOut(WPARAM, LPARAM)
{
	Init();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecipeView::OnActivateView(WPARAM, LPARAM)
{
	Init();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecipeView::OnDeactivateView(WPARAM, LPARAM)
{
	return 0L;
}


