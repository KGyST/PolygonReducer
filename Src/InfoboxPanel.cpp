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
		:DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule)
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

	void PolygonReducerInfoboxPage::SetCurrentPolygon(S_Polygon* currentPolygon)
	{
		m_currentPolygon = currentPolygon;
	}

	GSErrCode PolygonReducerInfoboxPage::SetPointNumber(int i_iVal/*, int i_iMax*/)
	{
		UNUSED_PARAMETER(i_iVal);

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
			API_Guid _guid0 = guids[0];
			S_Polygon pgon(&memos[0]);
			API_ElementMemo mem = pgon.getMemo();

			//API_ElementMemo mem2 = memos[0];
			//S::Array<Int32> _sa(mem.pends);
			//S::Array<Int32> _sa2(mem2.pends);
			//UNUSED_VARIABLE(_sa);
			//UNUSED_VARIABLE(_sa2);

			//mem.pends = mem2.pends;

			err = ACAPI_Element_ChangeMemo(_guid0, APIMemoMask_Polygon, &mem);

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

		API_ElementMemo originalMemo;

		err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);

		BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

		if (err == APIERR_NOSEL)
			err = NoError;

		if (err != NoError) {
			BMKillHandle((GSHandle*)&selNeigs);
			return 0;
		}

		API_Elem_Head elementHead = {};

		err = ConvertToGSArray<API_Neig, API_ElementMemo>(selNeigs, &memos, ReturnTrue<API_Neig>, ConvertToMemos);
		err = ConvertToGSArray<API_Neig, API_Guid>(selNeigs, &guids, ReturnTrue<API_Neig>, NeigToAPIGuid);


		for (unsigned int i = 0; i < memos.GetSize(); i++)
		{
			API_Guid _guid = guids[i];
			elementHead.guid = _guid;

			API_Element			elem;
			BNZeroMemory(&elem, sizeof(elem));
			elem.header.guid = _guid;
			err = ACAPI_Element_Get(&elem);

			//----- Userdata handling------------------------------------------------

			API_ElementUserData userData = {};

			GSErrCode err = ACAPI_Element_GetUserData(&elementHead, &userData);

			originalMemo = memos[i];

			if (err == NoError && userData.dataHdl != nullptr)
			{
				originalMemo = *reinterpret_cast<API_ElementMemo*> (*userData.dataHdl);
			}

			userData.dataVersion = 1;
			userData.platformSign = GS::Act_Platform_Sign;
			userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
			userData.dataHdl = BMAllocateHandle(sizeof(originalMemo), ALLOCATE_CLEAR, 0);
			*reinterpret_cast<API_ElementMemo*> (*userData.dataHdl) = originalMemo;

			err = ACAPI_Element_SetUserData(&elementHead, &userData);

			//-----/Userdata handling------------------------------------------------

			err = ConvertToGSArray<API_Coord, API_Coord>(memos[i].coords, &coords);
			err = ConvertToGSArray<INT32, INT32>(memos[i].pends, &pends);
			err = ConvertToGSArray<API_PolyArc, API_PolyArc>(memos[i].parcs, &parcs);
			err = ConvertToGSArray<UInt32, UInt32>(memos[i].vertexIDs, &vertexIDs);

			S_Polygon* _sp = new S_Polygon(&memos[i]);

			SetCurrentPolygon(_sp);

			_sp->updateMemo(&originalMemo);

			//-----------------------------------------------------

			API_Element         element;
			element.header = elementHead;
			element.header.guid = _guid;

			err = ACAPI_Element_Get(&element);
			if (err != NoError)
				return err;

			if (err == NoError) {

				API_ElementMemo tmpMemo;
			
				BNZeroMemory(&tmpMemo, sizeof(API_ElementMemo));
				API_Coord **_coords = ((S::Array<API_Coord>)coords).ToNeigs();
				tmpMemo.coords = _coords;
				GS::Int32** _pends = ((S::Array<Int32>)pends).ToNeigs();
				tmpMemo.pends = _pends;
				API_PolyArc** _parcs = ((S::Array<API_PolyArc>)parcs).ToNeigs();
				tmpMemo.parcs = _parcs;
				UInt32** _vertexIDs = ((S::Array<UInt32>)vertexIDs).ToNeigs();
				tmpMemo.vertexIDs = _vertexIDs;

				API_Guid __guid = *new API_Guid();

				err = ACAPI_Element_ChangeMemo(__guid, APIMemoMask_Polygon, &tmpMemo);
			}
		}

		return coords.GetSize() - pends.GetSize();
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

			//DGSetItemMaxLong(InfoBoxPageId, iUISliderId, iPointVal);
			//DGSetItemValLong(InfoBoxPageId, iUISliderId, iPointMax);

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