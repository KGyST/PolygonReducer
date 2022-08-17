#include "InfoboxPanel.hpp"
#include "PolygonReducer_Resource.h"
#include "APIEnvir.h"
#include "ACAPinc.h"
#include "GDLDialog.hpp"
#include "SettingsDialog.hpp"
#include "PolygonReducer.template.hpp"
#include "Utils.hpp"
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
		API_Neig**	selNeigs;
		GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
		GS::Array<API_Guid> guids = *new GS::Array<API_Guid>();
		API_SelectionInfo   selectionInfo;

		err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

		BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

		err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);
		err = ConvertToGSArray<API_Neig, API_Guid>(selNeigs, &guids, ReturnTrue<API_Neig>, NeigToAPIGuid);

		err = ACAPI_CallUndoableCommand("Optimize polygons",
		[&]() -> GSErrCode {
			S::Polygon pgon(&memos[0]);

			//pgon.MoveAllPoints();
			pgon.setPointCount(i_iPoint);

			API_ElementMemo mem = pgon.getMemo();

			API_Element element, mask;
			BNZeroMemory(&element, sizeof(API_Element));
			
			element.header.guid = guids[0];
			err = ACAPI_Element_Get(&element);

			API_Polygon poly ;
			poly.nCoords = BMGetHandleSize((GSHandle)mem.coords) / sizeof(API_Coord) - 1;
			poly.nSubPolys = BMGetHandleSize((GSHandle)mem.pends) / sizeof(Int32) - 1;
			poly.nArcs = BMGetHandleSize((GSHandle)mem.parcs) / sizeof(API_PolyArc);
			
			if (pgon.m_isPolygon)
				element.hatch.poly = poly;
			else
				element.polyLine.poly = poly;

			err =  ACAPI_Element_Change(&element, &mask, &mem, APIMemoMask_Polygon, true);

			return err;
		});

		return err;
	}

	int PolygonReducerInfoboxPage::GetPointNumber()
	{
		GSErrCode           err;
		API_SelectionInfo   selectionInfo;
		API_Neig** selNeigs;
		GS::Array<API_ElementMemo> memos = *new GS::Array<API_ElementMemo>();
		GS::Array<API_Guid> guids = *new GS::Array<API_Guid>();

		GS::Array<API_Coord> coords = *new GS::Array<API_Coord>();
		GS::Array<INT32> pends = *new GS::Array<INT32>();
		GS::Array<API_PolyArc> parcs = *new GS::Array<API_PolyArc>();
		GS::Array<UInt32> vertexIDs = *new GS::Array<UInt32>();

		err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

		////----- Userdata handling------------------------------------------------

		////API_ElementUserData userData = {};

		////GSErrCode err = ACAPI_Element_GetUserData(&elementHead, &userData);

		////auto originalMemo = memos[i];

		////if (err == NoError && userData.dataHdl != nullptr)
		////{
		////	originalMemo = *reinterpret_cast<API_ElementMemo*> (*userData.dataHdl);
		////}

		////userData.dataVersion = 1;
		////userData.platformSign = GS::Act_Platform_Sign;
		////userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
		////userData.dataHdl = BMAllocateHandle(sizeof(originalMemo), ALLOCATE_CLEAR, 0);
		////*reinterpret_cast<API_ElementMemo*> (*userData.dataHdl) = originalMemo;

		////err = ACAPI_Element_SetUserData(&elementHead, &userData);

		////-----/Userdata handling------------------------------------------------

		BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

		if (err == APIERR_NOSEL)
			err = NoError;

		if (err != NoError) {
			BMKillHandle((GSHandle*)&selNeigs);
			return 0;
		}

		API_Elem_Head elementHead;
		BNZeroMemory(&elementHead, sizeof(API_Elem_Head));

		err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);
		err = ConvertToGSArray<API_Neig, API_Guid>(selNeigs, &guids, ReturnTrue<API_Neig>, NeigToAPIGuid);

		UINT16 iPoints = 0;

		for (unsigned int i = 0; i < memos.GetSize(); i++)
		{
			S::Polygon pgon(&memos[i]);

			iPoints += (UINT16)pgon.getPointCount();
		}
		
		return iPoints;
	}

	// --- PolygonReducerPageObserver -------------------------------------------------

	void	PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
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