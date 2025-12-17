// -----------------------------------------------------------------------------
// SettingsDialog.cpp
// -----------------------------------------------------------------------------

#include	"../APIEnvir.h"
#include	"ACAPinc.h"

#include	"SettingsDialog.hpp"
#include "Logger/LoglevelStrings.hpp"
#include "Gui/SettingsSingleton.hpp"
#include "../PolygonReducer_Resource.h"


// --- Class definition: SettingsDialog --------------------------------------

SettingsDialog::SettingsDialog()
	:	DG::ModalDialog(ACAPI_GetOwnResModule(), SettingsPageId, ACAPI_GetOwnResModule())
	, closeButton(GetReference(), DG_OK)
	, logLevPopup(GetReference(), PopupControl_LogLev)
	, arcEdgesEdit(GetReference(), IntEdit_ArcEdges)
{
	AttachToAllItems(*this);

	for (const GS::UniString& _s : sLoglevels)
	{
    logLevPopup.InsertItem(DG::PopUp::BottomItem);
    logLevPopup.SetItemText(DG::PopUp::BottomItem, _s);
	}

  logLevPopup.SelectItem(LOGGER.GetLoglevel() + 1);

  arcEdgesEdit.SetValue(SETTINGS.GetMinEdgeCount());
}

void SettingsDialog::ButtonClicked(const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource() == &closeButton)
		PostCloseRequest(DG::ModalDialog::Accept);
}

void SettingsDialog::PopUpChanged(const DG::PopUpChangeEvent& ev)
{
	if (ev.GetSource() == &logLevPopup)
	{
		Int32 selectedLogLev = logLevPopup.GetSelectedItem() - 1;
		LOGGER.SetLoglevel(static_cast<Loglevel>(selectedLogLev));
	}
}

void SettingsDialog::IntEditChanged(const DG::IntEditChangeEvent& ev)
{
	if (ev.GetSource() == &arcEdgesEdit)
	{
		UINT minEdgeCount = static_cast<UINT>(arcEdgesEdit.GetValue());
		SETTINGS.SetMinEdgeCount(minEdgeCount);
	}
}

