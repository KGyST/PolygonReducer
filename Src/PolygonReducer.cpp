﻿// *****************************************************************************
// Source code for the Geometry Test Add-On
// API Development Kit 19; Mac/Win
//
// Namespaces:		Contact person:
//	-None-
//
// [SG compatible] - Yes
// *****************************************************************************

//API_​ElementUserData
//ACAPI_Element_SetUserData() 
//ACAPI_Element_GetUserData()

// ---------------------------------- Includes ---------------------------------

#define ACExtension
#include <stdio.h>
#include <string>
#include <algorithm>
#include <numeric>

#include "RS.hpp"
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "basicgeometry.h"
#include "APICommon.h"

#include "Utils.hpp"
#include "S_Polygon.hpp"
#include "S_Polygon.hpp"

#include "PolygonReducer_Resource.h"
#include "InfoboxPanel.hpp"
#include "PolygonReducer.template.hpp"			//templated functions' definitions
//#include "PolygonReducer.hpp"

using namespace PolygonReducer;
// MUST NOT BE IN NAMESPACE

// ---------------------------------- Types ------------------------------------


// ---------------------------------- Variables --------------------------------

int	registrationSuccess = 0;

enum {
    SettingsPanelRefCon = 1,
    InfoBoxPanelRefCon = 2,

    SettingsPanelRegistered = 1 << SettingsPanelRefCon,
    InfoBoxPanelRegistered = 1 << InfoBoxPanelRefCon
};

PolygonReducerPanel* infoBox = NULL;

// ----------------------------------  --------------------------------

// =============================================================================
//
// Main functions
//
// =============================================================================

static void		ReducePolygons(void)
{
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    API_Neig** selNeigs;
    GS::Array<API_Neig> indxs = *new GS::Array<API_Neig>();
    GS::Array<API_Guid> inds = *new GS::Array<API_Guid>();
    GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
    GS::Array<API_Coord> coords = *new GS::Array<API_Coord>();

    err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

    BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

    if (err == APIERR_NOSEL)
        err = NoError;

    if (err != NoError) {
        BMKillHandle((GSHandle*)&selNeigs);
        return;
    }

    err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);

    for (unsigned int i = 0; i < memos.GetSize(); i++)
    {
        err = ConvertToGSArray<API_Coord, API_Coord>(memos[i].coords, &coords);
    }

    for (unsigned int i = 0; i < coords.GetSize(); i++)
    {
        WriteReport_Alert("X = %f, Y = %f", coords[i].x, coords[i].y);
    }

    BMKillHandle((GSHandle*)&selNeigs);

    return;
}

// -----------------------------------------------------------------------------
// Create tabpage callback function
// -----------------------------------------------------------------------------

static	GSErrCode	__ACENV_CALL	CreatePageCallback(Int32 refCon, const void* tabControl, void* data, void** tabPage)
{
    bool success = false;
    const DG::TabControl* control = reinterpret_cast<const DG::TabControl*>(tabControl);
    DG::TabPage** page = reinterpret_cast<DG::TabPage**>(tabPage);

    switch (refCon) {

    case InfoBoxPanelRefCon:		if (infoBox != NULL)
        success = infoBox->CreatePage(*control, reinterpret_cast<TBUI::IAPIToolUIData*>(data), page);
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

    case InfoBoxPanelRefCon:		if (infoBox != NULL)
        infoBox->DestroyPage();
        break;
    }

    return NoError;
}

// -----------------------------------------------------------------------------
// Entry points to handle ArchiCAD events
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	MenuCommandHandler(const API_MenuParams* params)
{
    switch (params->menuItemRef.itemIndex) {
    case 1:		ReducePolygons();				break;
    }

    return NoError;
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

    ACAPI_Resource_GetLocStr(envir->addOnInfo.name, 32000, 1);
    ACAPI_Resource_GetLocStr(envir->addOnInfo.description, 32000, 2);

    return APIAddon_Preload;
}		/* CheckEnvironment */


//------------------------------------------------------
// Interface definitions
//------------------------------------------------------
GSErrCode	__ACENV_CALL	RegisterInterface(void)
{
    GSErrCode err = NoError;

    ACAPI_Register_Menu(32500, 0, MenuCode_UserDef, MenuFlag_Default);

    err = ACAPI_Register_InfoBoxPanel(InfoBoxPanelRefCon, API_PolyLineID, APIVarId_Generic, IDS_INFOBOXPAGE_NAME, InfoBoxPageId);
    err = ACAPI_Register_InfoBoxPanel(InfoBoxPanelRefCon, API_HatchID, APIVarId_Generic, IDS_INFOBOXPAGE_NAME, InfoBoxPageId);
    if (err != NoError) {
        DBPrintf("Panel_Test add-on: Cannot register info box panel\n");
    }
    else
        registrationSuccess |= InfoBoxPanelRegistered;

    return NoError;
}		/* RegisterInterface */


//------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
//------------------------------------------------------
GSErrCode	__ACENV_CALL Initialize(void)
{
    GSErrCode err = NoError;

    err = ACAPI_Install_MenuHandler(32500, MenuCommandHandler);
    if (err != NoError)
        DBPrintf("Geometry_Test:: Initialize() ACAPI_Install_MenuHandler failed\n");

    if (registrationSuccess & InfoBoxPanelRegistered) {
        try {
            infoBox = new PolygonReducerPanel(InfoBoxPanelRefCon);
        }
        catch (...) {
            DBPrintf("Panel_Test add-on: infoBoxPanel construction failed\n");
            infoBox = NULL;
        }

        if (infoBox != NULL) {
            err = ACAPI_Install_PanelHandler(infoBox->GetRefCon(), CreatePageCallback, DestroyPageCallback);
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
    if (infoBox != NULL)
        delete infoBox;

    return NoError;
}		// FreeData
