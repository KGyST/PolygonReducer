// *****************************************************************************
// Source code for the Geometry Test Add-On
// API Development Kit 19; Mac/Win
//
// Namespaces:		Contact person:
//	-None-
//
// [SG compatible] - Yes
// *****************************************************************************


#include "PolygonReducer.hpp"

// ---------------------------------- Includes ---------------------------------

#include	<stdio.h>
#include	<string.h>
#include <algorithm>
#include <numeric>

using namespace PolygonReducer;

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




// =============================================================================
//
// Main functions
//
// =============================================================================


template <class T>
GS::Array<T> * ArraySlice(GS::Array<T> p_array, UInt32 start, UInt32 end) {
    GS::Array<T> *result = new GS::Array<T>();
    UInt32 _size = p_array.GetSize();

    if (end > _size) end = _size;

    for (UInt32 i = start; i < end; i++)
    {
        result->Push(p_array[i]);
    }

    return result;
}

template GS::Array<UInt32>* ArraySlice(GS::Array<UInt32>, UInt32, UInt32);
template GS::Array<API_Coord>* ArraySlice(GS::Array<API_Coord>, UInt32, UInt32);

void TrackPoly(const API_Polygon* poly, const API_ElementMemo* memo)
{
    int         j, i, begInd, endInd, arcInd;
    API_Coord   begC, endC;

    for (j = 1; j <= poly->nSubPolys; j++) {
        begInd = (*memo->pends)[j - 1] + 1;
        endInd = (*memo->pends)[j];
        for (i = begInd; i < endInd; i++) {
            begC = (*memo->coords)[i];
            endC = (*memo->coords)[i + 1];
            arcInd = FindArc(*memo->parcs, poly->nArcs, i);
        }

        WriteReport_Alert(&"Teszt"[0]);
    }
}


bool IsItPolygon(API_Neig p_neig)
{
    if  (   p_neig.neigID == APINeig_PolyLine
        ||  p_neig.neigID == APINeig_PolyLineOn
        ||  p_neig.neigID == APINeig_Hatch
        ||  p_neig.neigID == APINeig_HatchOn
        )
        return true;

    return false;
}

API_Guid NeigToAPIGuid(API_Neig p_neig)
{
    return p_neig.guid;
}

// To be removed ------------------------------

template <class inT>
bool ReturnTrue (inT p_inObj)
{
    UNUSED_PARAMETER(p_inObj);

    return true;
}

template bool ReturnTrue< UInt32>(UInt32);
template bool ReturnTrue< Int32>(Int32);
template bool ReturnTrue< API_PolyArc>(API_PolyArc);

template <class T>
T ConvertToTheSame(T p_obj)
{
    return p_obj;
}

template UInt32 ConvertToTheSame< UInt32>(UInt32);
template Int32 ConvertToTheSame< Int32>(Int32);
template API_PolyArc ConvertToTheSame< API_PolyArc>(API_PolyArc);

API_ElementMemo ConvertToMemos(API_Neig p_neig)
{
    API_Guid _guid = p_neig.guid;
    API_ElementMemo _memo;

    GSErrCode err = ACAPI_Element_GetMemo(_guid, &_memo);

    UNUSED_VARIABLE(err);

    return _memo;
}

API_Coord** ConvertToCoords(API_ElementMemo p_memo)
{
    return p_memo.coords;
}

///To be removed ------------------------------

template <class inT, class outT>
GSErrCode ConvertToGSArray(
    inT** p_neigs,
    GS::Array<outT>* resultArray,
    bool (*funcFilter)(inT),
    outT (*funcConverter)(inT)
    )
// FIXME nameless functions as default parameters
{
    UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(inT);

    inT _an;

    try {
        for (UInt32 ii = 0; ii < nSel; ++ii) {
            _an = (*p_neigs)[ii];

            if (funcFilter(_an))
                resultArray->Push(funcConverter(_an));
        }
    }
    catch (...) {
        return 1;
    }

    return 0;
}

template GSErrCode ConvertToGSArray< UInt32, UInt32>(UInt32**, GS::Array<UInt32>*, bool (*funcFilter)(UInt32), UInt32(*funcConverter)(UInt32));
template GSErrCode ConvertToGSArray< API_PolyArc, API_PolyArc>(API_PolyArc**, GS::Array<API_PolyArc>*, bool (*funcFilter)(API_PolyArc), API_PolyArc(*funcConverter)(API_PolyArc));

//------------------------------

static void		ReducePolygons(void)
{
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    API_Neig**          selNeigs;
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

extern int GetPointNumber(void)
{
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    API_Neig** selNeigs;
    GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
    GS::Array<API_Coord> coords = *new GS::Array<API_Coord>();
    GS::Array<INT32> pends = *new GS::Array<INT32>();

    err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

    BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

    if (err == APIERR_NOSEL)
        err = NoError;

    if (err != NoError) {
        BMKillHandle((GSHandle*)&selNeigs);
        return 0;
    }

    err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);

    for (unsigned int i = 0; i < memos.GetSize(); i++)
    {
        err = ConvertToGSArray<API_Coord, API_Coord>(memos[i].coords, &coords);
        err = ConvertToGSArray<INT32, INT32>(memos[i].pends, &pends);
    }

    return coords.GetSize() - pends.GetSize();
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
//
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

