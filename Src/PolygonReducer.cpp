// *****************************************************************************
// Source code for the Geometry Test Add-On
// API Development Kit 19; Mac/Win
//
// Namespaces:		Contact person:
//	-None-
//
// [SG compatible] - Yes
// *****************************************************************************

#include "APIEnvir.h"
#define	_Geometry_Test_TRANSL_


// ---------------------------------- Includes ---------------------------------

#include	<stdio.h>
#include	<string.h>

#include	"ACAPinc.h"					// also includes APIdefs.h

#include	"basicgeometry.h"

#include    "APICommon.h"

#include <algorithm>

// ---------------------------------- Types ------------------------------------


// ---------------------------------- Variables --------------------------------


// ---------------------------------- Prototypes -------------------------------



// =============================================================================
//
// Main functions
//
// =============================================================================

bool IsItPolygon(API_Neig p_neig)
{
    if (p_neig.neigID == APINeig_PolyLine
        || p_neig.neigID == APINeig_Hatch)
        return true;

    return false;
}

API_Guid NeigToAPIGuid(API_Neig p_neig)
{
    return p_neig.guid;
}

template <class T>
GSErrCode GetItemsFromNeig(
    API_Neig** p_neigs,
    GS::Array<T>* resultArray,
    bool (*funcFilter)(API_Neig)
    //T(*funcConverter)(API_Neig)
    )
{
    UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(API_Neig);

    API_Neig _an;

    try {
        for (UInt32 ii = 0; ii < nSel; ++ii) {
            _an = (*p_neigs)[ii];

            if (funcFilter(_an))
                resultArray->Push((T)_an);
                //resultArray->Push(funcConverter(_an));
        }
    }
    catch (...) {
        return 1;
    }

    return 0;
}

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


static void		ReducePolygons(void)
{
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    API_Neig**          selNeigs;
    GS::Array<API_Neig> indxs = *new GS::Array<API_Neig>();
    GS::Array<API_Guid> inds = *new GS::Array<API_Guid>();
    GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
    //GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();

    err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

    BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

    if (err == APIERR_NOSEL)
        err = NoError;

    if (err != NoError) {
        BMKillHandle((GSHandle*)&selNeigs);
        return;
    }
    
    err = GetItemsFromNeig<API_Neig>(selNeigs, &indxs, IsItPolygon);

    API_Guid _guid;
    API_ElementMemo _memo;
    //API_Polygon *_poly;

    for ( Int32 i = 0; i < (Int32) indxs.GetSize() && err == NoError; ++i) {
        _guid = indxs[i].guid;
        inds.Push(_guid);

        err = ACAPI_Element_GetMemo(_guid, &_memo);
        memos.Push(_memo);
    }

    //for (Int32 i = 0; i < (Int32) memos.GetSize() && err == NoError; ++i) {
    //    TrackPoly(_poly, &_memo);
    //}


    BMKillHandle((GSHandle*)&selNeigs);

    return;
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

    return APIAddon_Normal;
}		/* CheckEnvironment */


//------------------------------------------------------
// Interface definitions
//------------------------------------------------------
GSErrCode	__ACENV_CALL	RegisterInterface(void)
{
    ACAPI_Register_Menu(32500, 0, MenuCode_UserDef, MenuFlag_Default);

    return NoError;
}		/* RegisterInterface */


//------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
//------------------------------------------------------
GSErrCode	__ACENV_CALL Initialize(void)
{
    GSErrCode err = ACAPI_Install_MenuHandler(32500, MenuCommandHandler);
    if (err != NoError)
        DBPrintf("Geometry_Test:: Initialize() ACAPI_Install_MenuHandler failed\n");

    return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// FreeData
//		called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData(void)
{
    return NoError;
}		// FreeData
