#pragma once
#include "DGModule.hpp"
#include "IAPIToolUIData.hpp"
#include "APIToolUIPanel.hpp"


namespace PolygonReducer {

	// --- PolygonReducerPage ---------------------------------------------------------

	class PolygonReducerPage : public DG::TabPage {

		friend class	PolygonReducerPageObserver;

	private:
		//enum {
		//	NameTextEditId = 2,
		//	UValueRealEditId = 4
		//};

		//DG::RealEdit			iUIPointNumber;
		TBUI::IAPIToolUIData*	uiData;
	public:

		PolygonReducerPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData);
		~PolygonReducerPage(void);
	};


	// --- PolygonReducerPageObserver -------------------------------------------------

	class PolygonReducerPageObserver : public	DG::TextEditBaseObserver,
		public	DG::RealEditObserver,
		public  DG::CompoundItemObserver,
		public  TBUI::IAPIToolUIDataObserver
	{
	private:
		PolygonReducerPage* tabPage;

		//void			GetUserDataFromDefault(WallUserData* data);
		//void			SetUserDataToDefault(WallUserData* sdata);
	protected:
		virtual	void	APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) override;
		virtual void	iUIPointNumberChanged(const DG::PosIntEditChangeEvent& ev);

	public:

		explicit PolygonReducerPageObserver(PolygonReducerPage* testPage);
		~PolygonReducerPageObserver(void);

	};

	// --- Wrapper class --------------------------------------------------------

	class PolygonReducerPanel : public TBUI::APIToolUIPanel
	{
	private:
		PolygonReducerPageObserver* observer;

	public:
		PolygonReducerPanel(Int32 refCon);
		virtual		~PolygonReducerPanel();

		virtual		bool	CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) override;
		virtual		void	DestroyPage(void) override;
	};
}