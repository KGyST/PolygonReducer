// -----------------------------------------------------------------------------
// SettingsDialog.cpp
// -----------------------------------------------------------------------------

// ---------------------------------- Includes ---------------------------------

#include	"../APIEnvir.h"
#include	"ACAPinc.h"

#include	"SettingsDialog.hpp"
#include "Logger/LoglevelStrings.hpp"
#include "Gui/SettingsSingleton.hpp"
#include "../PolygonReducer_Resource.h"


// --- Class definition: SettingsDialog --------------------------------------

SettingsDialog::SettingsDialog()
	:	DG::ModalDialog(ACAPI_GetOwnResModule(), SettingsPageId, ACAPI_GetOwnResModule())
	, closeButton(GetReference(), Button_OK)
	, logLevPopup(GetReference(), PopupControl_LogLev)
	, arcEdgesEdit(GetReference(), IntEdit_ArcEdges)
{
	for (const GS::UniString& _s : sLoglevels)
	{
    logLevPopup.InsertItem(DG::PopUp::BottomItem);
    logLevPopup.SetItemText(DG::PopUp::BottomItem, _s);
	}

  logLevPopup.SelectItem(LOGGER.GetLoglevel() + 1);

  arcEdgesEdit.SetValue(SETTINGS.GetMinEdgeCount());
}

//SettingsDialog::~SettingsDialog()
//{
//}

SettingsDialogObserver::SettingsDialogObserver(SettingsDialog* dialog):
	dialog(dialog)
{
	//dialog->Attach(*this);
	dialog->closeButton.Attach(*(DG::ButtonItemObserver*) this);
	dialog->logLevPopup.Attach(*this);
  dialog->arcEdgesEdit.Attach(*(DG::IntEditObserver*) this);

	AttachToAllItems(*dialog);
} 

SettingsDialogObserver::~SettingsDialogObserver(void) 
{
	dialog->Detach(*this);
	DetachFromAllItems(*dialog);
} 

void SettingsDialogObserver::ButtonClicked(const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource() == &dialog->closeButton)
		dialog->PostCloseRequest(DG::ModalDialog::Accept);
}

void SettingsDialogObserver::PopUpChanged(const DG::PopUpChangeEvent& ev)
{
	if (ev.GetSource() == &dialog->logLevPopup)
	{
		Int32 selectedLogLev = dialog->logLevPopup.GetSelectedItem() - 1;
		LOGGER.SetLoglevel(static_cast<Loglevel>(selectedLogLev));
	}
}

void SettingsDialogObserver::IntEditChanged(const DG::IntEditChangeEvent& ev)
{
	if (ev.GetSource() == &dialog->arcEdgesEdit)
	{
		UINT minEdgeCount = static_cast<UINT>(dialog->arcEdgesEdit.GetValue());
		SETTINGS.SetMinEdgeCount(minEdgeCount);
	}
}

