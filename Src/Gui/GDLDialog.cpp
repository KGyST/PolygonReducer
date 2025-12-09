// -----------------------------------------------------------------------------
// GDLDialog.cpp
// -----------------------------------------------------------------------------

// ---------------------------------- Includes ---------------------------------
#include	"../APIEnvir.h"
#include	"ACAPinc.h"

#include	"GDLDialog.hpp"

// --- Class definition: GDLDialog --------------------------------------
namespace PolygonReducer {

	GDLDialog::GDLDialog() :
		DG::ModalDialog(ACAPI_GetOwnResModule(), GDLPageId, ACAPI_GetOwnResModule()),
		closeButton(GetReference(), ButtonID),
		edit(GetReference(), EditID)
	{
	}

	void GDLDialog::setEditText(std::string text) {
		edit.SetText(GS::UniString(text.c_str() )  );
	}

	void GDLDialog::setEditText(GS::UniString text) {
		edit.SetText(text);
	}

	// --- GDLDialogObserver -------------------------------------

	GDLDialogObserver::GDLDialogObserver(GDLDialog* dialog) :
		dialog(dialog)
	{
		dialog->Attach(*this);
		AttachToAllItems(*dialog);
	}

	GDLDialogObserver::~GDLDialogObserver(void) {
		dialog->Detach(*this);
		DetachFromAllItems(*dialog);
	}

	void GDLDialogObserver::ButtonClicked(const DG::ButtonClickEvent& ev)
	{
		if (ev.GetSource() == &dialog->closeButton)
			dialog->PostCloseRequest(DG::ModalDialog::Accept);
	}
}

