#include "InfoboxPanel.hpp"
#include "PolygonReducer_Resource.h"
#include "APIEnvir.h"
#include "ACAPinc.h"
#include	"GDLDialog.hpp"
#include	"SettingsDialog.hpp"

namespace PolygonReducer {
	PolygonReducerInfoboxPage::PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData) :
		DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule),
		iUIPointNumber	(GetReference(), iUIPointNumberId),
		sUITest			(GetReference(), sUITestId),
		GDLButton		(GetReference(), GDLButtonId),
		SettingsButton	(GetReference(), SettingsButtonId),
		uiData			(puiData)
	{
		int _n = GetPointNumber();
		iUIPointNumber.SetValue( _n);
		sUITest.SetText("teszt") ;
	}

	PolygonReducerInfoboxPage::~PolygonReducerInfoboxPage(void) {
		uiData = NULL;
	}

	void	PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
		if (fieldMask.GetRegDataChanged()) {
		//	// the regdata has changed
		//	int _n = GetPointNumber();
		//	tabPage->iUIPointNumber.SetValue(_n);
		}
	}

	void	PolygonReducerPageObserver::iUIPointNumberChanged(const DG::PosIntEditChangeEvent& ev) {
		UNUSED_PARAMETER(ev);
	}

	void	PolygonReducerPageObserver::ButtonClicked(const DG::ButtonClickEvent& ev)
	{
		if (ev.GetSource() == &tabPage->GDLButton)
		{
			GDLDialog* dialog = new GDLDialog()  ;
			dialog->setEditText("teszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt\nteszt");
			GDLDialogObserver observer(dialog);
			dialog->Invoke();
		}

		if (ev.GetSource() == &tabPage->SettingsButton)
		{
			SettingsDialog* dialog = new SettingsDialog();
			SettingsDialogObserver observer(dialog);
			dialog->Invoke();
		}

		UNUSED_PARAMETER(ev);
	}

	PolygonReducerPageObserver::PolygonReducerPageObserver(PolygonReducerInfoboxPage* testPage) :
		tabPage(testPage)
	{
		AttachToAllItems(*tabPage);
		tabPage->uiData->AttachObserver(this);

		TBUI::APIElemDefaultFieldMask mask;
		mask.SetAll();
		APIElementChanged(mask);
	}

	PolygonReducerPageObserver::~PolygonReducerPageObserver(void) {
		DetachFromAllItems(*tabPage);
		tabPage->uiData->DetachObserver(this);
		tabPage = NULL;
	}


	PolygonReducerPanel::PolygonReducerPanel(Int32 refCon):
		TBUI::APIToolUIPanel(refCon)
	{

	}

	PolygonReducerPanel::~PolygonReducerPanel() {

	}

	bool	PolygonReducerPanel::CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) {
		this->tabPage = new PolygonReducerInfoboxPage(tabControl, data);
		*tabPage = this->tabPage;

		observer = new PolygonReducerPageObserver(dynamic_cast<PolygonReducerInfoboxPage*> (this->tabPage));

		return true;
	}

	void	PolygonReducerPanel::DestroyPage(void) {
		delete observer;		// this must be first
		observer = NULL;

		delete tabPage;			// this must be after destroying the observers
		tabPage = NULL;
	}
}