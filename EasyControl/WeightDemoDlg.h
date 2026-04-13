#ifdef __SIM_WEIGHT_GUI

#pragma once


// CWeightDemoDlg-Dialog

class CWeightDemoDlg : public CDialog
{
	DECLARE_DYNAMIC(CWeightDemoDlg)

	enum { IDD = IDD_WEIGHT_DEMO };


private:
	const CBrush				c_EditBrush;
	CWnd* m_pParent;
	static float32_t m_fWeight;
	static float32_t m_fStep;
	static int32_t  m_sItem;
	static int32_t m_lNr;

private:
	BOOL OnNotifyEditStep();
	BOOL OnNotifyEditWeight();
	BOOL OnNotifyEditItem();
	void SetValue();

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

public:
	CWeightDemoDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CWeightDemoDlg();

	void Create(CWnd* pParent);

	LRESULT OnNotifyEdit(WPARAM id, LPARAM bValue);

	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedRampUp();
	afx_msg void OnBnClickedRampDown();
	afx_msg void OnStnClickedStepEdit();
	afx_msg void OnStnClickedWeightEdit();
	afx_msg void OnStnClickedItemEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	static CWeightDemoDlg* g_WeightDemoDlg;

	static void CreateBox(CWnd* pWnd);



	DECLARE_MESSAGE_MAP()
};
#endif
