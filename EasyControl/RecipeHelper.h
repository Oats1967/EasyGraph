//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeHelper
///
/// @file   RecipeHelper.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "EasyControl.h"
#include "BASE/types.h"
#include "BASE/Utils/public/RecipeItemList.h"

//************************************************************************************
//************************************************************************************
namespace recipehelper
{
	void FillMaxSetpoint(base::utils::CRecipeItemList& recipelist);
	//************************************************************************************
	//************************************************************************************
	void CreateRecipeList(base::utils::CRecipeItemList& recipelist, const CString& szRecipeName, const uint64_t& szQMNmber);
	//************************************************************************************
	//************************************************************************************
	BOOL OnRecipeOpen(base::utils::CRecipeItemList& recipelist, CString& filename);
	//************************************************************************************
	//************************************************************************************
	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeSave
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnRecipeSave(base::utils::CRecipeItemList& recipelist, CString& filename);
	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeSaveAs
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnRecipeSaveAs(base::utils::CRecipeItemList& recipelist, CString& filename);
	//------------------------------------------------------------------------------------
	///  @brief   SetAnlagenModus
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @class        CRecipeView
	///
	///  @param[in] _bLocalMode , BOOL
	///  @param[in] sItem , short
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL SetAnlagenModus(const int32_t sItem, const BOOL newlocal);

	//------------------------------------------------------------------------------------
	///  @brief   CheckAnlagenkonfiguration
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @class        CRecipeView
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL CheckAnlagenkonfiguration(const base::utils::CRecipeItemList& recipelist);

	//------------------------------------------------------------------------------------
	///  @brief   CheckSollwerte
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL CheckSollwerte(const base::utils::CRecipeItemList& recipelist);

	//------------------------------------------------------------------------------------
	///  @brief   UpdateParameter
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @class        CRecipeView
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL UpdateParameter(const base::utils::CRecipeItemList& recipelist);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeActivate
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnRecipeActivate(const base::utils::CRecipeItemList& recipelist);

	void UpdateLineSetpoint(base::utils::CRecipeItemList& recipelist, float32_t& value);

};




