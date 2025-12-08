#pragma once

// -----------------------------------------------------------------------------
// GDLDialog.hpp
// -----------------------------------------------------------------------------

#ifndef GDLDIALOG_HPP
#define GDLDIALOG_HPP

// ---------------------------------- Includes ---------------------------------
#include <string>
#include "DGModule.hpp"
#include "../PolygonReducer_Resource.h"
#include "IAPIToolUIData.hpp"


// --- Class declaration: GDLDialog -------------------------------------
namespace PolygonReducer {

	class GDLDialog : public DG::ModalDialog
	{
		friend class GDLDialogObserver;

	protected:
		enum Controls {
			EditID = 1,
			ButtonID = 2
		};

		DG::Button	closeButton;
		DG::MultiLineEdit edit;

	public:
		GDLDialog();
		~GDLDialog() = default;
		void setEditText(GS::UniString text);
		void setEditText(std::string text);
	};

	// --- GDLDialogObserver -------------------------------------

	class GDLDialogObserver :
		public		DG::PanelObserver,
		public		DG::ButtonItemObserver,
		public		DG::CompoundItemObserver
	{
	private:
		GDLDialog* dialog;

	protected:
		virtual void	ButtonClicked(const DG::ButtonClickEvent& ev) override;

	public:
		GDLDialogObserver(GDLDialog* dialog);
		~GDLDialogObserver(void);
	};
}


#endif // GDLDIALOG_HPP

