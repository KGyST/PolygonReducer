// -----------------------------------------------------------------------------
// SEttingsDialog.hpp
// -----------------------------------------------------------------------------

#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

// ---------------------------------- Includes ---------------------------------
#include	"DGModule.hpp"
#include "../PolygonReducer_Resource.h"

// --- Class declaration: SEttingsDialog -------------------------------------

class SettingsDialog : 
	public DG::ModalDialog

{
friend class SettingsDialogObserver;

protected:
	enum Controls {
		Button_OKID = 1,
		LogLevID = 4,
		ArcEdgesID = 6
	};

	DG::Button	closeButton;
	DG::PopUp		logLevPopup;
	DG::IntEdit arcEdgesEdit;

public:
	SettingsDialog();
	~SettingsDialog() = default;
};

// --- SettingsDialogObserver -------------------------------------

class SettingsDialogObserver :
	public		DG::PanelObserver,
	public		DG::ButtonItemObserver,
	public		DG::CompoundItemObserver,
  public		DG::PopUpObserver,
  public		DG::IntEditObserver
{
private:
	SettingsDialog* dialog;

protected:
	virtual void ButtonClicked(const DG::ButtonClickEvent& ev) override;
  virtual void PopUpChanged(const DG::PopUpChangeEvent& ev) override;
  virtual void IntEditChanged(const DG::IntEditChangeEvent& ev) override;

public:
	explicit SettingsDialogObserver(SettingsDialog* dialog);
	~SettingsDialogObserver(void);
};


#endif // SETTINGS_DIALOG_HPP

