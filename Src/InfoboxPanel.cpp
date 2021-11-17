#include "InfoboxPanel.hpp"
#include "PolygonReducer_Resource.h"
#include <ACAPinc.h>

namespace PolygonReducer {
	PolygonReducerPage::PolygonReducerPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData):
		DG::TabPage(tabControl, 1, 100, 100)
		//DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule)
		//iUIPointNumber = DG::RealEdit(tabControl, 0);
	{
		UNUSED_PARAMETER(tabControl);
		UNUSED_PARAMETER(puiData);
	}

	PolygonReducerPage::~PolygonReducerPage(void) {

	}


	void	PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
		UNUSED_PARAMETER(fieldMask);
	}
	void	PolygonReducerPageObserver::iUIPointNumberChanged(const DG::PosIntEditChangeEvent& ev) {
		UNUSED_PARAMETER(ev);
	}

	PolygonReducerPageObserver::PolygonReducerPageObserver(PolygonReducerPage* testPage) {
		UNUSED_PARAMETER(testPage);
	}

	PolygonReducerPageObserver::~PolygonReducerPageObserver(void) {
	}


	PolygonReducerPanel::PolygonReducerPanel(Int32 refCon):
		TBUI::APIToolUIPanel(refCon)
	{
	}

	PolygonReducerPanel::~PolygonReducerPanel() {

	}

	bool	PolygonReducerPanel::CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) {
		UNUSED_PARAMETER(tabControl);
		UNUSED_PARAMETER(data);
		UNUSED_PARAMETER(tabPage);

		return true;
	}

	void	PolygonReducerPanel::DestroyPage(void) {
	}
}