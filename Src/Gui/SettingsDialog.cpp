// -----------------------------------------------------------------------------
// SettingsDialog.cpp
// -----------------------------------------------------------------------------

// ---------------------------------- Includes ---------------------------------

#include	"../APIEnvir.h"
#include	"ACAPinc.h"

#include	"SettingsDialog.hpp"

// --- Class definition: SettingsDialog --------------------------------------

SettingsDialog::SettingsDialog() :
	DG::ModalDialog(ACAPI_GetOwnResModule(), SettingsPageId, ACAPI_GetOwnResModule()),
	closeButton(GetReference(), SettingsCloseButtonID) {}

SettingsDialog::~SettingsDialog()
{
}

SettingsDialogObserver::SettingsDialogObserver(SettingsDialog* dialog):
	dialog(dialog)
{
	dialog->Attach(*this);
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

