//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalbrateProgressDlg
///
/// @file   CalbrateProgressDlg.h
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

#include "BASE/include/CalibType.h"

class CCalibrateProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrateProgressDlg)

	// Dialogfelddaten
	enum { IDD = IDD_CALIBRATION_PROGRESS };

private:
	CBCGPCircularProgressIndicatorCtrl	m_wndProgress;
	base::calib::CalibType m_Cfg;
	base::calib::CalibType m_saveCfg;
	CString		  m_szDoseFaktor;
	CString		  m_szDriveCommand;
	UINT_PTR	  m_nTimer;
	uint32_t	  m_lTime;
	uint32_t	  m_ProgressStart;
	uint32_t	  m_ProgressFinish;
	uint32_t	  m_nCurrProgress;

	int32_t		  m_sID;
	int32_t		  m_calibindex;

private:
	BOOL GetCalibState() const;
	void OnUpdateProgress();
	uint32_t GetRunTime() const;


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
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	//------------------------------------------------------------------------------------
	///  @brief   OnInitDialog
	///
	///           This method is called by the framework in response to WM_INITDIALOG message
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnInitDialog() override;

public:
	CCalibrateProgressDlg(CWnd* pParent, const int32_t id);   // Standardkonstruktor
	~CCalibrateProgressDlg() override = default;

	void SetCalibCfg(const base::calib::CalibType& cfg)
	{ m_Cfg = cfg;	}

	const base::calib::CalibType& GetCalibCfg( void) const
	{return m_Cfg;	}

	void SetCalibIndex(const int32_t calibindex)
	{	m_calibindex = calibindex;	}

	int32_t GetCalibIndex(void) const
	{	return m_calibindex;	}

	BOOL IsFinished() const
	{	return (m_nCurrProgress >= m_ProgressFinish);	}

	uint32_t getRuntime() const
	{	return m_nCurrProgress;	}


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	void Stop(void);

	DECLARE_MESSAGE_MAP()

};

