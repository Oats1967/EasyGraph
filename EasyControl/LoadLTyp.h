//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LoadLTyp.h
///
/// @file   LoadLTyp.h
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

typedef struct __DOSEPOSITIONSTYP
{
	RECT _aRD;				// Dosierrect für Mausclick
	RECT _aAS;				// Actual Statusrect
	RECT _aNS;				// Nominal Statusrect
	RECT _aBS;				// DoseButton
} DOSEPOSITIONSTYP;



template<uint32_t N>
struct LOADLTYP
{
	uint32_t			 uiAnzahl;
	SIZE				 aBitmapSize;
	DOSEPOSITIONSTYP	 aDose[N];
};

typedef LOADLTYP<1> LOADLTYPBASE;




