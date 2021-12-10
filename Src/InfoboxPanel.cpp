#include "InfoboxPanel.hpp"
#include "PolygonReducer_Resource.h"
#include "APIEnvir.h"
#include "ACAPinc.h"
#include "GDLDialog.hpp"
#include "SettingsDialog.hpp"
#include "PolygonReducer.template.hpp"
#include "Utils.hpp"

namespace PolygonReducer {
	PolygonReducerInfoboxPage::PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData) :
		DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule),
		iUIPointNumber	(GetReference(), iUIPointNumberId),
		sUITest			(GetReference(), sUITestId),
		GDLButton		(GetReference(), GDLButtonId),
		SettingsButton	(GetReference(), SettingsButtonId),
		uiData			(puiData)
	{
		iUIPointNumber.SetValue(GetPointNumber());

	}

	PolygonReducerInfoboxPage::~PolygonReducerInfoboxPage() {
		uiData = NULL;
	}

	void PolygonReducerInfoboxPage::SetCurrentPolygon(S_Polygon* currentPolygon)
	{
		m_currentPolygon = currentPolygon;
	}

	int PolygonReducerInfoboxPage::GetPointNumber()
	{
		GSErrCode           err;
		API_SelectionInfo   selectionInfo;
		API_Neig** selNeigs;
		GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
		GS::Array<API_Coord> coords = *new GS::Array<API_Coord>();
		GS::Array<INT32> pends = *new GS::Array<INT32>();

		err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

		BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

		if (err == APIERR_NOSEL)
			err = NoError;

		if (err != NoError) {
			BMKillHandle((GSHandle*)&selNeigs);
			return 0;
		}

		err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);

		for (unsigned int i = 0; i < memos.GetSize(); i++)
		{
			err = ConvertToGSArray<API_Coord, API_Coord>(memos[i].coords, &coords);
			err = ConvertToGSArray<INT32, INT32>(memos[i].pends, &pends);

			S_Polygon* _sp = new S_Polygon(&memos[i]);

			SetCurrentPolygon(_sp);
		}

		return coords.GetSize() - pends.GetSize();
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
		if (ev.GetSource() == &m_tabPage->GDLButton)
		{
			GDLDialog* dialog = new GDLDialog()  ;
			std::string _s = m_tabPage->m_currentPolygon->getGDLcode();
			dialog->setEditText(_s);

			GDLDialogObserver observer (dialog);
			dialog->Invoke();
		}

		if (ev.GetSource() == &m_tabPage->SettingsButton)
		{
			SettingsDialog* dialog = new SettingsDialog();
			SettingsDialogObserver observer(dialog);
			dialog->Invoke();
		}

		UNUSED_PARAMETER(ev);
	}

	PolygonReducerPageObserver::PolygonReducerPageObserver(PolygonReducerInfoboxPage* testPage) :
		m_tabPage(testPage)
	{
		AttachToAllItems(*m_tabPage);
		m_tabPage->uiData->AttachObserver(this);

		TBUI::APIElemDefaultFieldMask mask;
		mask.SetAll();
		APIElementChanged(mask);
	}

	PolygonReducerPageObserver::~PolygonReducerPageObserver(void) {
		DetachFromAllItems(*m_tabPage);
		m_tabPage->uiData->DetachObserver(this);
		m_tabPage = NULL;
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