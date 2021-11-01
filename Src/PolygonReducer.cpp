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

// ---------------------------------- Types ------------------------------------


// ---------------------------------- Variables --------------------------------


// ---------------------------------- Prototypes -------------------------------



// =============================================================================
//
// Main functions
//
// =============================================================================


static void		PolygonReduce (void)
{

	return;
}		/* Do_Test */

// -----------------------------------------------------------------------------
// Entry points to handle ArchiCAD events
//
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	MenuCommandHandler (const API_MenuParams *params)
{
	switch (params->menuItemRef.itemIndex) {
		case 1:		PolygonReduce();				break;
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
API_AddonType	__ACENV_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	if (envir->serverInfo.serverApplication != APIAppl_ArchiCADID)
		return APIAddon_DontRegister;

	ACAPI_Resource_GetLocStr (envir->addOnInfo.name, 32000, 1);
	ACAPI_Resource_GetLocStr (envir->addOnInfo.description, 32000, 2);

	return APIAddon_Normal;
}		/* CheckEnvironment */


//------------------------------------------------------
// Interface definitions
//------------------------------------------------------
GSErrCode	__ACENV_CALL	RegisterInterface (void)
{
	ACAPI_Register_Menu (32500, 0, MenuCode_UserDef, MenuFlag_Default);

	return NoError;
}		/* RegisterInterface */


//------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
//------------------------------------------------------
GSErrCode	__ACENV_CALL Initialize	(void)
{
	GSErrCode err = ACAPI_Install_MenuHandler (32500, MenuCommandHandler);
	if (err != NoError)
		DBPrintf ("Geometry_Test:: Initialize() ACAPI_Install_MenuHandler failed\n");

	return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// FreeData
//		called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData (void)
{
	return NoError;
}		// FreeData
