// -----------------------------------------------------------------------------
// SEttingsDialog.hpp
// -----------------------------------------------------------------------------

#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

// ---------------------------------- Includes ---------------------------------
#include	"DGModule.hpp"

// --- Class declaration: SEttingsDialog -------------------------------------

class SettingsDialog : 
	public	DG::ModalDialog,
	public	DG::ButtonItemObserver,
	public	DG::PopUpObserver,
	public	DG::IntEditObserver,
	public	DG::CompoundItemObserver

{
protected:
	DG::Button	closeButton;
	DG::PopUp		logLevPopup;
	DG::IntEdit arcEdgesEdit;

public:
	SettingsDialog();
	~SettingsDialog() = default;

protected:
	virtual void ButtonClicked(const DG::ButtonClickEvent& ev) override;
	virtual void PopUpChanged(const DG::PopUpChangeEvent& ev) override;
	virtual void IntEditChanged(const DG::IntEditChangeEvent& ev) override;
};

#endif // SETTINGS_DIALOG_HPP

