// -----------------------------------------------------------------------------
// SEttingsDialog.hpp
// -----------------------------------------------------------------------------

#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

#include	"DGModule.hpp"

using namespace DG;

class SettingsDialog : 
	public	ModalDialog,
	public	ButtonItemObserver,
	public	PopUpObserver,
	public	IntEditObserver,
	public	CompoundItemObserver

{
protected:
	Button	closeButton;
	PopUp		logLevPopup;
	IntEdit arcEdgesEdit;

public:
	SettingsDialog();
	~SettingsDialog() = default;

protected:
	void ButtonClicked(const ButtonClickEvent& ev) override;
	void PopUpChanged(const PopUpChangeEvent& ev) override;
	void IntEditChanged(const IntEditChangeEvent& ev) override;
};

#endif // SETTINGS_DIALOG_HPP

