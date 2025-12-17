// -----------------------------------------------------------------------------
// GDLDialog.cpp
// -----------------------------------------------------------------------------

#include	"../APIEnvir.h"
#include	"ACAPinc.h"

#include	"GDLDialog.hpp"
#include "../PolygonReducer_Resource.h"


namespace PolygonReducer {

	GDLDialog::GDLDialog() :
		DG::ModalDialog(ACAPI_GetOwnResModule(), GDLPageId, ACAPI_GetOwnResModule()),
		closeButton(GetReference(), DG_OK),
		edit(GetReference(), MultiLineEdit_GDL)
	{
		AttachToAllItems(*this);
	}

	void GDLDialog::setEditText(std::string text) {
		edit.SetText(GS::UniString(text.c_str() )  );
	}

	void GDLDialog::setEditText(GS::UniString text) {
		edit.SetText(text);
	}

	void GDLDialog::ButtonClicked(const DG::ButtonClickEvent& ev)
	{
		if (ev.GetSource() == &closeButton)
			PostCloseRequest(DG::ModalDialog::Accept);
	}
}

