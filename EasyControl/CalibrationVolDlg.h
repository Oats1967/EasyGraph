//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalibrationVolDlg
///
/// @file   CalibrationVolDlg.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <functional>
#include "EasyControl.h"
#include "InfoButton.h"
#include "ButtonStartStop.h"

class CCalibrationVolDlg : public CDialog
{
    enum { IDD = IDD_CALIBRATIONVOL };

private:
    CInfoButton		    m_DoseperformanceInfoButton;
    CInfoButton		    m_MeasuretimeInfoButton;
    CInfoButton		    m_WeightInfoButton;
    CInfoButton		    m_DriveCommandInfoButton;
    CButtonStartStop	m_StartBt;


private:
    const std::map <int32_t, std::function<BOOL()> > m_EditMap;
    const CBrush     c_EditBrush;
    float32_t        m_fCalibDriveCommand;
    float32_t        m_fDosePerformance;
    float32_t        m_fWeight;
    uint32_t         m_lCalibDuration;
    BOOL             m_bTerminated;
    int32_t          m_sItem;


private:
    BOOL OnNotifyCalibrationDrivecommand();
    BOOL OnNotifyCalibrationMeasuretime();
    BOOL OnNotifyCalibrationWeight();

    void SetValue();
    void GetValue();
    BOOL OnUpdateControls(void);
    void SetControlStyle(void);

protected:
    BOOL OnInitDialog() override;
    void DoDataExchange(CDataExchange* pDX) override;
    void OnOK() override;

public:
    CCalibrationVolDlg(CWnd* pParent, int32_t id);
    ~CCalibrationVolDlg() override = default;


    DECLARE_MESSAGE_MAP()
    afx_msg void OnClickedCalibrationCalibrate();
    afx_msg void OnClickedCalibrationDrivecommand();
    afx_msg void OnClickedCalibrationMeasuretime();
    afx_msg void OnClickedCalibrationWeight();
    afx_msg void OnClickedCalibrationDrivecommandInfo();
    afx_msg void OnClickedCalibrationMeasuretimeInfo();
    afx_msg void OnClickedCalibrationWeightInfo();
    afx_msg void OnClickedCalibrationDoseperformanceInfo();

    LRESULT OnNotifyEdit(WPARAM w, LPARAM);

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
};

