//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FormatMetric
///
/// @file   FormatMetric.h
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


//***********************************************************************************************************
//***********************************************************************************************************
void DDX_FloatKg(CDataExchange* pDX, int32_t nIDC, float32_t& value);
void DDX_FloatKgHR(CDataExchange* pDX, int32_t nIDC, float32_t& value); // High resoultion
void DDX_FloatKgh(CDataExchange* pDX, int32_t nIDC, float32_t& value);
void DDX_Float(CDataExchange* pDX, int32_t nIDC, float32_t& value);
void DDX_Percentage(CDataExchange* pDX, int32_t nIDC, float32_t& value);
void DDX_FloatHR(CDataExchange* pDX, int32_t nIDC, float32_t& value);
void DDX_TimeH(CDataExchange* pDX, int32_t nIDC, uint32_t& value);
void DDX_TextN(CDataExchange* pDX, int32_t nIDC, CString& value, const int32_t n);
void DDX_ControlEnable(BOOL bEnable, CDataExchange* pDX, int nIDC, CWnd& rControl);


CString FORMATKG(const float32_t value);
CString FORMATKGH(const float32_t value);
CString FORMATKGM(const float32_t value);
CString FORMATPERCENT(const float32_t value);
CString FORMATTIME(const uint32_t value);

//***********************************************************************************************************
//***********************************************************************************************************
CString FormatMetric2(const float32_t value);




