#pragma once

// -----------------------------------------------------------------------------
// SEttingsDialog.hpp
// -----------------------------------------------------------------------------

#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

// ---------------------------------- Includes ---------------------------------
#include	"DGModule.hpp"
#include "PolygonReducer_Resource.h"

// --- Class declaration: SEttingsDialog -------------------------------------

class SettingsDialog : 
	public DG::ModalDialog

{
friend class SettingsDialogObserver;

protected:
	enum Controls {
		TextID = 1,
		SettingsCloseButtonID = 2
	};

	DG::Button	closeButton;

public:
	SettingsDialog();
	~SettingsDialog();
};

// --- SettingsDialogObserver -------------------------------------

class SettingsDialogObserver :
	public		DG::PanelObserver,
	public		DG::ButtonItemObserver,
	public		DG::CompoundItemObserver
{
private:
	SettingsDialog* dialog;

protected:
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev) override;

public:
	explicit SettingsDialogObserver(SettingsDialog* dialog);
	~SettingsDialogObserver(void);
};

#endif // SETTINGS_DIALOG_HPP