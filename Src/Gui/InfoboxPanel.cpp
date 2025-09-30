#include "InfoboxPanel.hpp"
#include "../PolygonReducer_Resource.h"
#include "../APIEnvir.h"
#include "ACAPinc.h"
#include "GDLDialog.hpp"
#include "SettingsDialog.hpp"
#include "../PolygonReducer.template.hpp"
#include "../Utils/Utils.hpp"
#include "Array.hpp"

namespace PolygonReducer {
	PolygonReducerInfoboxPage::PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* p_uiData)
		:DG::TabPage	(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule)
		,iUIPointNumber	(GetReference(), iUIPointNumberId)
		,iSlider		(GetReference(), iUISliderId)
		,GDLButton		(GetReference(), GDLButtonId)
		,SettingsButton	(GetReference(), SettingsButtonId)
		,uiData			(p_uiData)
	{
		int _iPoint = GetPointNumber();

		iUIPointNumber.SetValue(_iPoint);
		DGSetItemMaxLong(InfoBoxPageId, iUISliderId, _iPoint);
		DGSetItemValLong(InfoBoxPageId, iUISliderId, _iPoint);
	}

	PolygonReducerInfoboxPage::~PolygonReducerInfoboxPage() {
		uiData = NULL;
	}

	void PolygonReducerInfoboxPage::SetCurrentPolygon(S::Polygon* currentPolygon)
	{
		m_currentPolygon = currentPolygon;
	}

	GSErrCode PolygonReducerInfoboxPage::SetPointNumber(int i_iPoint/*, int i_iMax*/)
	{
		GSErrCode   err;
		//API_Neig** selNeigs{} ;
		//GS::Array<API_ElementMemo> memos{};
		GS::Array<API_Guid> guids {};
		//GS::Array<API_Neig> neigs {};
		API_Guid guid{};

		if (S::Polygon * pgon = GetFirstPolygonFromSelection(&guid)) {

			err = ACAPI_CallUndoableCommand("Optimize polygons",
				[&]() -> GSErrCode {
					pgon->setPointCount(i_iPoint);

					API_ElementMemo mem = pgon->getMemo();

					API_Element element, mask;
					BNZeroMemory(&element, sizeof(API_Element));

					element.header.guid = guid;
					err = ACAPI_Element_Get(&element);

					API_Polygon poly;
					poly.nCoords = BMGetHandleSize((GSHandle)mem.coords) / sizeof(API_Coord) - 1;
					poly.nSubPolys = BMGetHandleSize((GSHandle)mem.pends) / sizeof(Int32) - 1;
					poly.nArcs = BMGetHandleSize((GSHandle)mem.parcs) / sizeof(API_PolyArc);

					if (pgon->m_isPolygon)
						element.hatch.poly = poly;
					else
						element.polyLine.poly = poly;

					err = ACAPI_Element_Change(&element, &mask, &mem, APIMemoMask_Polygon, true);

					delete pgon;

					return err;
				});

			return err;
		}
		else {
			return 0; // no polygon found in selection
		}
	}

	int PolygonReducerInfoboxPage::GetPointNumber()
	{
		//----- Userdata handling------------------------------------------------

		//API_ElementUserData userData = {};

		//GSErrCode err = ACAPI_Element_GetUserData(&elementHead, &userData);

		//auto originalMemo = memos[i];

		//if (err == NoError && userData.dataHdl != nullptr)
		//{
		//	originalMemo = *reinterpret_cast<API_ElementMemo*> (*userData.dataHdl);
		//}

		//userData.dataVersion = 1;
		//userData.platformSign = GS::Act_Platform_Sign;
		//userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
		//userData.dataHdl = BMAllocateHandle(sizeof(originalMemo), ALLOCATE_CLEAR, 0);
		//*reinterpret_cast<API_ElementMemo*> (*userData.dataHdl) = originalMemo;

		//err = ACAPI_Element_SetUserData(&elementHead, &userData);

		//-----/Userdata handling------------------------------------------------

		S::Polygon *pgon = GetFirstPolygonFromSelection();

		if (pgon != NULL) {
			UINT16 iPoints = (UINT16)pgon->getPointCount();
			delete pgon;

			return iPoints;
		}
		else {
			return 0; // no polygon found in selection
		}
	}

	S::Polygon* GetFirstPolygonFromSelection(API_Guid * o_guid)
	{
		GSErrCode           err;
		API_SelectionInfo   selectionInfo;
		GS::Array<API_Neig> neigs{};
		API_ElementMemo _memo{};

		err = ACAPI_Selection_Get(&selectionInfo, &neigs, false);

		if (err == APIERR_NOSEL)
			return NULL;

		if (err != NoError) {
			return NULL;
		}

		for (auto& neig : neigs)
		{
			if (IsItPolygon(neig))
			{
				GSErrCode err = ACAPI_Element_GetMemo(neig.guid, &_memo);

				if (o_guid) *o_guid = neig.guid;
				
				return new S::Polygon{ &_memo };
			}
		}

		return NULL;
	}

	// --- PolygonReducerPageObserver -------------------------------------------------

	void PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
		if (fieldMask.GetRegDataChanged()) {
		//	// the regdata has changed
		//	int _n = GetPointNumber();
		//	tabPage->iUIPointNumber.SetValue(_n);
		}
	}

	void PolygonReducerPageObserver::PosIntEditChanged(const DG::PosIntEditChangeEvent& ev)
	{
		if (ev.GetSource() == &m_tabPage->iUIPointNumber)
		{
			int iPointVal = ev.GetSource()->GetValue();
			//int iPointMax = ev.GetSource()->GetMax() ;

			DGSetItemValLong(InfoBoxPageId, m_tabPage->iUISliderId, iPointVal);
			//DGSetItemValLong(InfoBoxPageId, iUISliderId, iPointMax);

			m_tabPage->SetPointNumber(iPointVal/*, iPointMax*/);

		}
	}

	void PolygonReducerPageObserver::BarControlChanged(const DG::BarControlChangeEvent& ev)
	{
		if (ev.GetSource() == &m_tabPage->iSlider)
		{
			int iPointVal = ev.GetSource()->GetValue();

			DGSetItemValLong(InfoBoxPageId, m_tabPage->iUIPointNumberId, iPointVal);

			m_tabPage->SetPointNumber(iPointVal/*, iPointMax*/);
		}
	}

	void PolygonReducerPageObserver::ButtonClicked(const DG::ButtonClickEvent& ev)
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

	// --- PolygonReducerPage ---------------------------------------------------------

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