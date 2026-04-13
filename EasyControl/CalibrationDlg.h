//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalibrateDlg
///
/// @file   CalibrateDlg.h
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
#include "BASE/include/CalibType.h"

class CCalibrationDlg :
      public CDialog
{
    enum { IDD = IDD_CALIBRATION };

private:
    CInfoButton		    m_DoseperformanceInfoButton;
    CInfoButton		    m_MeasuretimeInfoButton;
    CInfoButton		    m_DriveCommandInfoButton;


    CImageList		m_aImageList;
    CListCtrl       m_aCalibrationList;
    base::calib::CalibType m_cfg;
    const std::map <int32_t, std::function<BOOL()> > c_EditMap;
    const CBrush     c_EditBrush;
    float32_t        m_fCalibDriveCommand;
    float32_t        m_fDosePerformance;
    uint32_t         m_lCalibDuration;
    uint32_t         m_CalibIndex;
    int32_t          m_sItem;


private:
    BOOL OnNotifyCalibrationDrivecommand();
    BOOL OnNotifyCalibrationMeasuretime();
    BOOL OnNotifyCalibrationDosePerformance();

    void SetValue();
    void GetValue() {}
    void BuildHeadLine(CListCtrl& iListCtrl);
    void FillListCtrl(CListCtrl& iListCtrl);
    void Calibrate(const int32_t index);
    void Sort(base::calib::CalibType& cfg);


protected:
    BOOL OnInitDialog() override;
    void DoDataExchange(CDataExchange* pDX) override;
    void OnOK() override;

public:
    CCalibrationDlg(CWnd* pParent, int32_t id);
    ~CCalibrationDlg() override = default;


    DECLARE_MESSAGE_MAP()
    afx_msg void OnClickedCalibrationAdd();
    afx_msg void OnClickedCalibrationCalibrate();
    afx_msg void OnClickedCalibrationCalibrateAll();
    afx_msg void OnClickedCalibrationDelete();
    afx_msg void OnItemactivateCalibrationList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickedCalibrationDrivecommand();
    afx_msg void OnClickedCalibrationMeasuretime();
    afx_msg void OnClickedCalibrationDosePerformance();
    afx_msg void OnClickedCalibrationMeasuretimeInfo();
    afx_msg void OnClickedCalibrationDrivecommandInfo();
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

