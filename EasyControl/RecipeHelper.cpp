//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeView.cpp
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
#include "WrapperBase.h"
#include "RecipeHelper.h"
#include "RecipeFileDialog.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/EasyTime/public/EasyTime.h"
#include "BASE/Utils/public/xml/RecipefileXml.h"
#include "RemoteControl.h"
#include "wmuser.h"

#define ISMODIFIEDFLOAT(id1, id2) (fabs(id1-id2) > 1e-04)
#define XMLFILETYPE "xml"

//************************************************************************************
//************************************************************************************
namespace recipehelper
{
	//************************************************************************************
	//************************************************************************************
	static float32_t OnGetLineSetpoint(void)
	{
		base::LineProcessStatus rStatus;
		REMOTEREF.getLineProcessStatus(rStatus);

		float32_t value = 0.0F;
		REMOTEREF.getLineSetpoint(value);
		return value;
	}

	//************************************************************************************
	//************************************************************************************
	static void OnSetLineSetpoint(const float32_t value)
	{
		REMOTEREF.setLineSetpoint(value);
	}

	//************************************************************************************
	//************************************************************************************
	static BOOL CheckValidConfiguration(const base::utils::CRecipeItemList& recipelist)
	{
		uint32_t sMaxItems = 0;
		// Ueberpruefen, ob die Anlagendimension passt
		const auto lCount = recipelist.GetCount();
		REMOTEREF.getMaxDoseCount(sMaxItems);
		if (lCount != sMaxItems)
		{
			ECMessageBox(IDS_QUERY_RECIPEAKTIVIERENERRORDIMENSION, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		return TRUE;
	}
	//************************************************************************************
	//************************************************************************************
	void FillMaxSetpoint(base::utils::CRecipeItemList& recipelist)
	{
		const auto count = recipelist.GetCount();
		for (uint32_t index = 0; index < count; index++)
		{
			float32_t fMaxSetpoint = 0.0f;
			auto& item = recipelist.GetItem(index);
			if (__ISVALIDDOSETYPE(item.GetDoseType()))
			{
				REMOTEREF.getDoseMaxSetpoint(index, fMaxSetpoint);
			}
			item.SetMaxSetpoint(fMaxSetpoint);
		}
	}
	//************************************************************************************
	//************************************************************************************
	base::utils::CRecipeItem GetDose(const int32_t index)
	{
		base::utils::CRecipeItem aR;
		base::eDoseType type;
		REMOTEREF.getDoseType(index, type);
		aR.SetDoseType(type);

		std::string name;
		REMOTEREF.getDoseName(index, name);
		aR.SetName(name);

		uint64_t qmnumber = 0;
		REMOTEREF.getDoseQMNumber(index, qmnumber);
		aR.SetQMNumber(qmnumber);

		if (! base::__ISVALIDDOSETYPE(type))
		{
			aR.SetRecipePercentage(0.0f);
			aR.SetSetpoint(0.0f);
			aR.SetMaxSetpoint(0.0f);
			aR.SetRegenerat(FALSE);
			aR.SetLocalMode(TRUE);
			aR.SetDosePerformance(0.0F);
			aR.SetRefillMax(0.0F);
			aR.SetRefillMin(0.0F);
			aR.SetRefillAlarmLimit(0.0F);
		}
		else
		{
			float32_t value = 0.0F;
			REMOTEREF.getDosePercentage(index, value);
			aR.SetRecipePercentage(value);
			REMOTEREF.getDoseSetpoint(index, value);
			aR.SetSetpoint(value);

			REMOTEREF.getDoseSetpoint(index, value);
			aR.SetSetpoint(value);
			REMOTEREF.getDoseMaxSetpoint(index, value);
			aR.SetMaxSetpoint(value);
			BOOL regenerat = FALSE;
			REMOTEREF.getDoseRegenerat(index, regenerat);
			aR.SetRegenerat(regenerat);
			base::ProcessStatus rStatus;
			REMOTEREF.getDoseProcessStatus(index, rStatus);
			aR.SetLocalMode(rStatus.flags.lineMode == false);
			REMOTEREF.getDoseDosePerformance(index, 0, value);
			aR.SetDosePerformance(value);
			float32_t fMin = 0.0F;
			float32_t fMax = 0.0F;
			float32_t fMinMin = 0.0F;
			REMOTEREF.getDoseRefillLimit(index, fMin, fMax, fMinMin);
			aR.SetRefillMax(fMax);
			aR.SetRefillMin(fMin);
			aR.SetRefillAlarmLimit(fMinMin);
			float32_t fTotalizer = 0.0F;
			REMOTEREF.getDoseTotalizer(index, fTotalizer);
			aR.SetTotalizer(fTotalizer);
		}
		return aR;
	}

	//************************************************************************************
	//************************************************************************************
	void CreateRecipeList(base::utils::CRecipeItemList& recipelist, const CString& szRecipeName, const uint64_t& QMNmber)
	{
		recipelist.Clear();
		recipelist.SetName(toStdString(szRecipeName));
		recipelist.SetQMNumber(QMNmber);

		float32_t value = OnGetLineSetpoint();
		recipelist.SetLineSetpoint(value);

		uint32_t sMaxItems = 0;
		REMOTEREF.getMaxDoseCount(sMaxItems);
		for (uint32_t index = 0; index < sMaxItems; index++)
		{
			recipelist.AddItem(GetDose(_S32(index)));
		}
		FillMaxSetpoint(recipelist);
		// Recipesumme neuberechnen
		recipelist.Summe();
	}
	//************************************************************************************
//************************************************************************************
	BOOL OnRecipeOpenFinal(base::utils::CRecipeItemList& recipelist, CString& filename)
	{
		BOOL result = FALSE;

		int32_t pos = filename.ReverseFind('.'); //will return the position of the last "." in your string
		CString ext = filename.Right(filename.GetLength() - pos); //will return the substring after the last "."
		ext.MakeUpper();

		base::utils::CRecipeItemList templist;
		if (ext == _T(".XML"))
		{
			base::xml::CRecipefileXml xml;
			result = xml.Load(toStdString(filename));
			templist = xml.Get();
		}
		else if (ext == _T(".RZP"))
		{
			result = templist.Load(toStdString(filename));
		}
		if ( ! result)
		{
			CString szTemp, szTemp1;

			VERIFY(szTemp1.LoadString(IDS_ERROR_RECIPEOPEN));
			szTemp.Format(_T("%s"), LPCTSTR(szTemp1));
			ECMessageBox(szTemp, MB_OK | MB_ICONSTOP);
		}
		else
		{
			// Ueberpruefen, ob die Anlagendimension passt
			result = CheckValidConfiguration(templist);
			if (result)
			{
				recipelist = templist;
				FillMaxSetpoint(recipelist);
				// Recipesumme neuberechnen
				recipelist.Summe();
			}
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeOpen(base::utils::CRecipeItemList& recipelist, CString& filename)
	{
		BOOL result = FALSE;

		if (EASYCONTROLREF.m_RecipePath.empty())
		{
			ECMessageBox(IDS_ERROR_RECIPEOPEN, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		filename = _T("");
		CRecipeFileDialog fdlg(TRUE, filename);
		if ( fdlg.DoModal() == IDOK)
		{
			filename = fdlg.GetPathName();
			result = OnRecipeOpenFinal(recipelist, filename);
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeSaveFinal(base::utils::CRecipeItemList& recipelist, const CString& filename)
	{
		BOOL result = FALSE;

		int32_t pos = filename.ReverseFind('.'); //will return the position of the last "." in your string
		CString ext = filename.Right(filename.GetLength() - pos); //will return the substring after the last "."
		ext.MakeUpper();
		if (ext == _T(".XML"))
		{
			base::xml::CRecipefileXml xml;
			xml.Set(recipelist);
			result = xml.Save(toStdString(filename));
		}
		else if (ext == _T(".RZP"))
		{
			result = recipelist.Save(toStdString(filename));
		}
		if ( ! result)
		{
			CString szTemp1;
			CString szTemp;

			VERIFY(szTemp1.LoadString(IDS_ERROR_RECIPESPEICHERN));
			szTemp.Format(_T("%s"), LPCTSTR(szTemp1));
			ECMessageBox(szTemp, MB_OK | MB_ICONSTOP);
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeSave( base::utils::CRecipeItemList& recipelist, CString& filename)
	{
		if (filename.IsEmpty() )
		{
			return OnRecipeSaveAs(recipelist, filename);
		}
		return OnRecipeSaveFinal(recipelist, filename);
	}
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeSaveAs( base::utils::CRecipeItemList& recipelist, CString& filename)
	{
		BOOL result = TRUE;

		if (EASYCONTROLREF.m_RecipePath.empty())
		{
			ECMessageBox(IDS_ERROR_RECIPESPEICHERN, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		CRecipeFileDialog fdlg(FALSE, filename);
		if ( fdlg.DoModal() == IDOK)
		{
			filename = fdlg.GetPathName();
			result = OnRecipeSaveFinal(recipelist, filename);
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	static BOOL WaitMode(const int32_t sItem, const BOOL newlocal)
	{
		constexpr uint32_t waittime = 5000U;

		base::ProcessStatus rStatus;
		REMOTEREF.getDoseProcessStatus(sItem, rStatus);
		BOOL bLocalMode = ! rStatus.flags.lineMode;
		if (newlocal != bLocalMode)
		{
			if (newlocal)
			{
				REMOTEREF.setDosePBLocal(sItem);
			}
			else
			{
				REMOTEREF.setDosePBInline(sItem);
			}
			const auto dStart = base::time::GetTimeU64();
			const auto dEnd = dStart + waittime;
			while (base::time::GetTimeU64() < dEnd)
			{
				Sleep(1000);
				REMOTEREF.getDoseProcessStatus(sItem, rStatus);
				bLocalMode = !rStatus.flags.lineMode;
				if (bLocalMode == newlocal)
				{
					break;
				}
			}
		}
		return bLocalMode == newlocal;
	}
	//************************************************************************************
	//************************************************************************************
	static BOOL WaitStopped(const int32_t sItem)
	{
		constexpr uint32_t waittime = 5000U;

		base::ProcessStatus rStatus;
		REMOTEREF.getDoseProcessStatus(sItem, rStatus);
		BOOL running = rStatus.flags.started || rStatus.flags.running;
		if (running)
		{
			REMOTEREF.setDosePBStop(sItem);
			const auto dStart = base::time::GetTimeU64();
			const auto dEnd = dStart + waittime;
			while ( base::time::GetTimeU64() < dEnd )
			{
				Sleep(1000);
				REMOTEREF.getDoseProcessStatus(sItem, rStatus);
				running = rStatus.flags.started || rStatus.flags.running;
				if ( ! running)
				{
					break;
				}
			} 
		}
		return !running;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL SetAnlagenModus(const int32_t sItem, const BOOL newlocal)
	{
		constexpr uint32_t waittime = 5000U;
		base::ProcessStatus rStatus;
		REMOTEREF.getDoseProcessStatus(sItem, rStatus);
		const BOOL bLocalMode = ! rStatus.flags.lineMode;
		BOOL result = (newlocal == bLocalMode);
		if ( ! result)
		{
			if ( bLocalMode )
			{
				result = WaitStopped(sItem);
			}
			result = WaitMode(sItem, newlocal);
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL CheckAnlagenkonfiguration(const base::utils::CRecipeItemList& recipelist)
	{
		uint32_t sMaxItems = 0;
		// Ueberpruefen, ob die Anlagendimension passt
		const auto lCount = recipelist.GetCount();
		REMOTEREF.getMaxDoseCount(sMaxItems);
		if (lCount != sMaxItems)
		{
			ECMessageBox(IDS_QUERY_RECIPEAKTIVIERENERRORDIMENSION, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		// Ueberpruefen, ob die Anlagenkonfiguration passt
		BOOL result = TRUE;
		for (uint32_t i = 0; i < lCount; i++)
		{
			base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
			REMOTEREF.getDoseType(i, type);
			const auto& item = recipelist.GetItem(i);
			if (item.GetDoseType() != type)
			{
				result = FALSE;
				break;
			}
		}
		if ( ! result)
		{
			ECMessageBox(IDS_QUERY_RECIPEAKTIVIERENERRORCONFIGURATION, MB_OK | MB_ICONSTOP);
		}
		return result;
	}

	//************************************************************************************
	//************************************************************************************
	BOOL CheckSollwerte(const base::utils::CRecipeItemList& recipelist)
	{
		CString szMessage;
		CString szTemp;

		// Ueberpruefen, ob die Anlagendimension passt
		if ( ! CheckValidConfiguration(recipelist))
		{
			return FALSE;
		}
		CString szFeeder;
		VERIFY(szFeeder.LoadString(IDS_DOSIERER));
		CString szNr;
		VERIFY(szNr.LoadString(IDS_ALARMNR));

		int32_t i = 0;
		const auto lCount = _S32(recipelist.GetCount());
		for (; i < lCount; i++)
		{
			const auto& item = recipelist.GetItem(i);
			assert(_S32(item.GetID()) == i);
			if (! base::__ISVALIDDOSETYPE(item.GetDoseType()))
			{
				continue;
			}
			if (item.GetSetpoint() > item.GetDosePerformance())
			{
				VERIFY(szMessage.LoadString(IDS_SETPOINT_GREATER_DOSEPERFORMANCE));
				szTemp.Format(_T("%s %s%d \n%s"), LPCTSTR(szFeeder), LPCTSTR(szNr), _S32(item.GetID()+1), LPCTSTR(szMessage));
				auto result = ECMessageBox(szTemp, MB_OKCANCEL | MB_ICONEXCLAMATION);
				if (result == IDCANCEL)
				{
					break;
				}
			}
			if (item.GetSetpoint() > item.GetMaxSetpoint())
			{
				VERIFY(szMessage.LoadString(IDS_SETPOINT_GREATER_MAXSETPOINT));
				szTemp.Format(_T("%s %s%d \n%s"), LPCTSTR(szFeeder), LPCTSTR(szNr), _S32(item.GetID()+1), LPCTSTR(szMessage));
				auto result = ECMessageBox(szTemp, MB_OKCANCEL | MB_ICONEXCLAMATION);
				if (result == IDCANCEL)
				{
					break;
				}
			}
		}
		return  BOOL(i >= lCount);
	}
	//************************************************************************************
	//************************************************************************************
	BOOL UpdateParameter(const base::utils::CRecipeItemList& recipelist)
	{
		// Ueberpruefen, ob die Anlagendimension passt
		if ( ! CheckValidConfiguration(recipelist))
		{
			return FALSE;
		}
		const auto lCount = recipelist.GetCount();
		for (uint32_t i = 0; i < lCount; i++)
		{
			const auto& item = recipelist.GetItem(i);
			if ( ! __ISVALIDDOSETYPE(item.GetDoseType()))
			{
				continue;
			}
			REMOTEREF.setDoseName(i, item.GetName());
			REMOTEREF.setDoseQMNumber(i, item.GetQMNumber());

			SetAnlagenModus(i, item.GetLocalMode());

			float32_t fTemp = 0.0f;
			REMOTEREF.getDoseSetpoint(i, fTemp);
			if (ISMODIFIEDFLOAT(fTemp, item.GetSetpoint()))
			{
				REMOTEREF.setDoseSetpoint(i, item.GetSetpoint());
			}
			REMOTEREF.getDosePercentage(i, fTemp);
			if (ISMODIFIEDFLOAT(fTemp, item.GetRecipePercentage()))
			{
				REMOTEREF.setDosePercentage(i, item.GetRecipePercentage());
			}
			REMOTEREF.setDoseRegenerat(i, item.GetRegenerat());

			float32_t fMin = 0.0F, fMax = 0.0F, fMinMin = 0.0F;
			BOOL refillmodify = FALSE;
			REMOTEREF.getDoseRefillLimit(i, fMin, fMax, fMinMin);
			refillmodify  = ISMODIFIEDFLOAT(fMin, item.GetRefillMin());
			refillmodify |= ISMODIFIEDFLOAT(fMax, item.GetRefillMax());
			refillmodify |= ISMODIFIEDFLOAT(fMinMin, item.GetRefillAlarmLimit());
			if ( refillmodify )
			{
				REMOTEREF.setDoseRefillLimit(i, fMin, fMax, fMinMin);
			}
		}
		REMOTEREF.setLineRecipeName(recipelist.GetName());
		REMOTEREF.setLineRecipeQMNumber(recipelist.GetQMNumber());
		REMOTEREF.setLineRecipeSetpoint(recipelist.GetRecipeSetpoint());
		REMOTEREF.setLineRegeneratPercentage(recipelist.GetRegeneratPercentage());

		float32_t fLineSetpoint = OnGetLineSetpoint();
		if ( ISMODIFIEDFLOAT(fLineSetpoint, recipelist.GetLineSetpoint()))
		{
			if (ECMessageBox(IDS_QUERY_LINESETPOINT_OVERWRITE, MB_YESNO) == IDYES)
			{
				OnSetLineSetpoint(recipelist.GetLineSetpoint());
			}
		}
		return TRUE;
	}
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeActivate(const base::utils::CRecipeItemList& recipelist)
	{
		// Ueberpruefen, ob die Anlagendimension passt
		auto result = CheckAnlagenkonfiguration(recipelist);

		// Pruefen ob Sollwerte passen
		if (result)
		{
			result = CheckSollwerte(recipelist);
		}
		if (result)
		{
			result = UpdateParameter(recipelist);
		}
		return result;
	}
	//************************************************************************************
	//************************************************************************************
	void UpdateLineSetpoint(base::utils::CRecipeItemList& recipelist, float32_t& value)
	{
		base::LineProcessStatus rStatus;
		REMOTEREF.getLineProcessStatus(rStatus);

		const BOOL bExtLineSetpoint = rStatus.flags.StateExtlineSetpoint;
		if (bExtLineSetpoint)
		{
			value = OnGetLineSetpoint();
		}
		recipelist.UpdateLineSetpoint(value);
	}

};




