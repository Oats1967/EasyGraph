//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module InfoBoxCtrl
///
/// @file   InfoBoxCtrl.h
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



class CInfoBoxCtrl : public CBCGPInfoBoxCtrl
{
	CRect m_orgrect;
	static const int32_t cMinSize = 240;

public:
	CInfoBoxCtrl() : CBCGPInfoBoxCtrl(), m_orgrect {}
	{ m_nIconIndex = -1; }

	void SizeToContent(BCGP_SIZE_TO_CONTENT_LINES lines = BCGP_SIZE_TO_CONTENT_LINES_AUTO) override;

	void SetWindowOrg()
	{
		GetWindowRect(m_orgrect);
		if (m_orgrect.Width() < cMinSize)
		{
			m_orgrect.right = m_orgrect.left + cMinSize;
		}
	}
};
