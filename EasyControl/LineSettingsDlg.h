//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineSettingsDlg
///
/// @file   LineSettingsDlg.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"


class CLineSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineSettingsDlg)

	enum { IDD = IDD_LINE_SETTINGS };

	union eModuleBits
	{
		struct 	
		{
            uint32_t ModuleSetpointRamp		: 1;
            uint32_t ModuleExternalSetpoint : 1;
            uint32_t ModuleLineSetpoint		: 1;
            uint32_t ModuleLocalSetpoint	: 1;
			uint32_t ModuleTotalizerPulse : 1;
		} flags;
        uint32_t allflags = 0;
	};

	eModuleBits m_ModuleBits;

private:
	void SortButtons(void);
	CSize GetDlgItemSize(CWnd* pWnd) const;
	void ActivateDlg(CDialog&);

protected:
	BOOL OnInitDialog() override;

public:
	CLineSettingsDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineSettingsDlg() override = default;

	afx_msg void OnBnClickedLineSettingsExtSetpointBt();
	afx_msg void OnBnClickedLineSettingsRampBt();
	afx_msg void OnBnClickedLineSettingsLineSetpointBt();
	afx_msg void OnBnClickedLineSettingsLocalSetpointBt();
	afx_msg void OnBnClickedLineSettingsTotImpulseBt();

	LRESULT OnPreselect(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};
