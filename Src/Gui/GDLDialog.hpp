// -----------------------------------------------------------------------------
// GDLDialog.hpp
// -----------------------------------------------------------------------------

#ifndef GDLDIALOG_HPP
#define GDLDIALOG_HPP

#include <string>
#include "DGModule.hpp"

using namespace DG;

namespace PolygonReducer {
	class GDLDialog 
		: public	ModalDialog
		, public	PanelObserver
		, public	ButtonItemObserver
		, public	CompoundItemObserver
	{

	protected:
		Button	closeButton;
		MultiLineEdit edit;

	public:
		GDLDialog();
		~GDLDialog() = default;
		void setEditText(GS::UniString text);
		void setEditText(std::string text);

	protected:
		void	ButtonClicked(const ButtonClickEvent& ev) override;
	};
}


#endif // GDLDIALOG_HPP

