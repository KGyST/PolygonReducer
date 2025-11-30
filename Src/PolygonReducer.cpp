// *****************************************************************************
// Source code for the Geometry Test Add-On
// API Development Kit 19; Mac/Win
//
// Namespaces:		Contact person:
//	-None-
//
// [SG compatible] - Yes
// *****************************************************************************

//ACAPI_​Element_​ChangeMemo()
//ACAPI_​Element_​Change() 

// ---------------------------------- Includes ---------------------------------

#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "AC27/APICommon.h"

#include <stdio.h>
#include <string>
#include <algorithm>
#include <numeric>

#include "basicgeometry.h"
#include "Utils/Utils.hpp"
#include "Geometry/Polygon.hpp"

#include "PolygonReducer_Resource.h"
#include "Gui/InfoboxPanel.hpp"
#include "PolygonReducer.template.hpp"			//templated functions' definitions
#include "PolygonReducer.hpp"
#include "Logger/Logger.hpp"
#include "Logger/LoglevelStrings.hpp"
#include <boost/format.hpp>


using namespace PolygonReducer;
// MUST NOT BE IN ANY NAMESPACE

// ---------------------------------- Types ------------------------------------


// ---------------------------------- Variables --------------------------------

DGMessageData								cntlDlgData;	//Dummy, unused

int	registrationSuccess = 0;

enum {
    SettingsPanelRefCon = 1,
    PointNrInfoBoxPanelRefCon = 2,
    LengthInfoBoxPanelRefCon = 3,

    SettingsPanelRegistered = 1 << SettingsPanelRefCon,
    PointNrInfoBoxPanelRegistered = 1 << PointNrInfoBoxPanelRefCon,
    LengthInfoBoxPanelRegistered = 1 << LengthInfoBoxPanelRefCon
};

PointNrPanel* pointNrInfoBox = NULL;
LengthPanel* lengthInfoBox = NULL;
Logger logger(COMPANY_NAME, APP_NAME);

// ----------------------------------  --------------------------------

// =============================================================================
//
// Main functions
//
// =============================================================================

static short DGCALLBACK SettingsDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	short result = 0;
	GS::UniString _text{};
	API_PropertyDefinition _def;
	IO::Location sFileLoc;

  LOGGER.Log(boost::format("SettingsDlgCallBack: message = %d, dialID = %d, item = %d") % message % dialID % item, Loglevel::LogLev_TRACE);

  switch (message) {
    case DG_MSG_INIT:
      //GSErrCode err;

      for (const GS::UniString& _s : sLoglevels)
      {
        DGPopUpInsertItem(dialID, PopupControl_LogLev, DG_LIST_BOTTOM);
        DGPopUpSetItemText(dialID, PopupControl_LogLev, DG_LIST_BOTTOM, _s);
      }

      DGPopUpSelectItem(dialID, PopupControl_LogLev, LOGGER.GetLoglevel() + 1);

      DGSetItemValLong(dialID, IntEdit_ArcEdges, SETTINGS.GetMinEdgeCount());

      break;
    case DG_MSG_CLICK:
      switch (item) {
      case DG_CLOSEBOX:
        //case -1:
      case Button_OK:
        result = item;

        break;
      }
      
      break;
    case DG_MSG_CHANGE:
      switch (item) {
      case PopupControl_LogLev:
        LOGGER.SetLoglevel((Loglevel)(DGGetItemValLong(dialID, PopupControl_LogLev) - 1));

        break;
      case IntEdit_ArcEdges:
        SETTINGS.SetMinEdgeCount((UINT)DGGetItemValLong(dialID, IntEdit_ArcEdges));

        break;
      }
  }

  return result;
}

static void		Do_DisplaySettings(void) {
  GSErrCode		err = NoError;

  err = DGModalDialog(ACAPI_GetOwnResModule(), SettingsPageId, ACAPI_GetOwnResModule(), SettingsDlgCallBack, (DGUserData)&cntlDlgData);
}

#if _DEBUG
static void		Do_ReducePolygons(void)
{
  GSErrCode           err;
  API_SelectionInfo   selectionInfo;
  //GS::Array<API_Neig> indxs = *new GS::Array<API_Neig>();
  //GS::Array<API_Guid> inds = *new GS::Array<API_Guid>();
  GS::Array<API_ElementMemo> memos = {};
  GS::Array<API_Coord> coords{};

  GS::Array<API_Neig> selNeigs{};

  err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

  if (err == APIERR_NOSEL)
    return;

  if (err != NoError) {
    return;
  }

  err = ConvertToGSArray<API_Neig, API_ElementMemo>(&selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);

  for (API_ElementMemo _memo : memos)
  {
    err = ConvertToGSArray<API_Coord, API_Coord>(_memo.coords, &coords);
  }

  for (API_Coord _coord : coords)
  {
    WriteReport_Alert("X = %f, Y = %f", _coord.x, _coord.y);
  }

  return;
}
#endif

// -----------------------------------------------------------------------------
// Create tabpage callback function
// -----------------------------------------------------------------------------

static	GSErrCode	__ACENV_CALL	CreatePageCallback(Int32 refCon, const void* tabControl, void* data, void** tabPage)
{
    bool success = false;
    const DG::TabControl* control = reinterpret_cast<const DG::TabControl*>(tabControl);
    DG::TabPage** page = reinterpret_cast<DG::TabPage**>(tabPage);

    switch (refCon) {
        case PointNrInfoBoxPanelRefCon:		if (pointNrInfoBox != NULL)
            success = pointNrInfoBox->CreatePage(*control, reinterpret_cast<TBUI::IAPIToolUIData*>(data), page);
            break;
        }
    return (success ? NoError : (GSErrCode)APIERR_GENERAL);
}


// -----------------------------------------------------------------------------
// Destroy tabpage callback function
// -----------------------------------------------------------------------------

static	GSErrCode	__ACENV_CALL	DestroyPageCallback(Int32 refCon, void* /*tabPage*/)
{
    switch (refCon) {

    case PointNrInfoBoxPanelRefCon:		if (pointNrInfoBox != NULL)
        pointNrInfoBox->DestroyPage();
        break;
    }

    return NoError;
}

// -----------------------------------------------------------------------------
// Entry points to handle ArchiCAD events
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	MenuCommandHandler(const API_MenuParams* params)
{
    return ACAPI_CallUndoableCommand("Reduce Polygons",
        [&]() -> GSErrCode {
            switch (params->menuItemRef.itemIndex) {
            case 1:		Do_DisplaySettings();		    break;
#if _DEBUG
            case 2:		Do_ReducePolygons();				break;
#endif
            }
        
            return NoError;
    });
}		// DoCommand


// =============================================================================
//
// Required functions
//
// =============================================================================


//------------------------------------------------------
// Dependency definitions
//------------------------------------------------------
API_AddonType	__ACENV_CALL	CheckEnvironment(API_EnvirParams* envir)
{
    if (envir->serverInfo.serverApplication != APIAppl_ArchiCADID)
        return APIAddon_DontRegister;

    RSGetIndString(&envir->addOnInfo.name, 32000, 1, ACAPI_GetOwnResModule());
    RSGetIndString(&envir->addOnInfo.description, 32000, 2, ACAPI_GetOwnResModule());

    return APIAddon_Preload;
}		/* CheckEnvironment */


//------------------------------------------------------
// Interface definitions
//------------------------------------------------------
GSErrCode	__ACENV_CALL	RegisterInterface(void)
{
  GSErrCode err = NoError;

  ACAPI_MenuItem_RegisterMenu(32500, 0, MenuCode_UserDef, MenuFlag_Default);

	err = ACAPI_AddOnIntegration_RegisterInfoBoxPanel(PointNrInfoBoxPanelRefCon, API_PolyLineID, PointNr_IBOXPAGE_NAME, PointNrInfoBoxId);
	err = ACAPI_AddOnIntegration_RegisterInfoBoxPanel(PointNrInfoBoxPanelRefCon, API_HatchID, PointNr_IBOXPAGE_NAME, PointNrInfoBoxId);

	err = ACAPI_AddOnIntegration_RegisterInfoBoxPanel(LengthInfoBoxPanelRefCon, API_PolyLineID, Length_IBOXPAGE_NAME, LengthInfoBoxId);
	err = ACAPI_AddOnIntegration_RegisterInfoBoxPanel(LengthInfoBoxPanelRefCon, API_HatchID, Length_IBOXPAGE_NAME, LengthInfoBoxId);

  if (err != NoError) {
    DBPrintf("PolygonReducer add-on: Cannot register info box panel\n");
  }
  else
    registrationSuccess |= PointNrInfoBoxPanelRefCon;
    registrationSuccess |= LengthInfoBoxPanelRefCon;

  return NoError;
}		/* RegisterInterface */


//------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
//------------------------------------------------------
GSErrCode	__ACENV_CALL Initialize(void)
{
    GSErrCode err = NoError;

    err = ACAPI_MenuItem_InstallMenuHandler(IDS_APP_NAME, MenuCommandHandler);
    if (err != NoError)
        DBPrintf("Geometry_Test:: Initialize() ACAPI_Install_MenuHandler failed\n");

    if (registrationSuccess & PointNrInfoBoxPanelRefCon & LengthInfoBoxPanelRefCon) {
        try {
            pointNrInfoBox = new PointNrPanel(PointNrInfoBoxPanelRefCon);
        }
        catch (...) {
            DBPrintf("Panel_Test add-on: infoBoxPanel construction failed\n");
            pointNrInfoBox = NULL;
        }

        try {
          lengthInfoBox = new LengthPanel(LengthInfoBoxPanelRefCon);
        }
        catch (...) {
            DBPrintf("Panel_Test add-on: infoBoxPanel construction failed\n");
            lengthInfoBox = NULL;
        }

        if (pointNrInfoBox != NULL && lengthInfoBox != NULL) {
            err = ACAPI_AddOnIntegration_InstallPanelHandler(pointNrInfoBox->GetRefCon(), CreatePageCallback, DestroyPageCallback);
            if (err != NoError) {
              DBPrintf("Panel_Test add-on: Info box panel handler initialization failed\n");
            }

            err = ACAPI_AddOnIntegration_InstallPanelHandler(lengthInfoBox->GetRefCon(), CreatePageCallback, DestroyPageCallback);
            if (err != NoError) {
                DBPrintf("Panel_Test add-on: Info box panel handler initialization failed\n");
            }
        }
    }

    return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// FreeData
//		called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData(void)
{
    if (pointNrInfoBox != NULL)
        delete pointNrInfoBox;

    return NoError;
}		// FreeData
