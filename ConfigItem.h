//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2023 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// ConfigItem.h : interface of the CEasyGraphView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "WorkspaceBar.h"

struct ConfigItem
{
	int32_t m_DefaultLineWidth = 1;
	double m_dblThumbnailFontRatio = 0.3;

	CBCGPChartTheme		m_CustomColorTheme;
	CBCGPChartTheme		m_CustomTexturedTheme;
	double										m_dblChartAnimationTime;	// Seconds
	CBCGPAnimationManager::BCGPAnimationType	m_animationType;
	CBCGPChartSeries::BCGPChartAnimationStyle	m_animationStyle;
	CStringArray								m_arAnimationStyleNames;
	CDemoFeature::Feature						m_Feature = CDemoFeature::Feature::BCGP_ChartElements;
	BOOL										m_bHiColorIcons = FALSE;
	UINT m_nAppLook;

	int32_t GetDefaultLineWidth() const
	{
		return m_DefaultLineWidth;
	}

	const CBCGPChartTheme& GetCustomTheme(int32_t nTheme)
	{
		return nTheme == -2 ? m_CustomColorTheme : m_CustomTexturedTheme;
	}
};

extern ConfigItem theConfig;
