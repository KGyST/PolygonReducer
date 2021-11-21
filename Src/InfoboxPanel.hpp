#pragma once
#include "DGModule.hpp"
#include "IAPIToolUIData.hpp"
#include "APIToolUIPanel.hpp"
#include "PolygonReducer.hpp"

namespace PolygonReducer {

	// --- PolygonReducerPage ---------------------------------------------------------

	class PolygonReducerInfoboxPage : public DG::TabPage {

		friend class	PolygonReducerPageObserver;

	private:
		enum {
			iUIPointNumberId = 2,
			sUITestId = 3
		};

		DG::IntEdit				iUIPointNumber;
		DG::TextEdit			sUITest;
		TBUI::IAPIToolUIData*	uiData;
	public:

		PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData);
		~PolygonReducerInfoboxPage(void);
	};


	// --- PolygonReducerPageObserver -------------------------------------------------

	class PolygonReducerPageObserver : public	DG::TextEditBaseObserver,
		public	DG::IntEditObserver,
		public  DG::CompoundItemObserver,
		public  TBUI::IAPIToolUIDataObserver
	{
	private:
		PolygonReducerInfoboxPage* tabPage;

		//void			GetUserDataFromDefault(WallUserData* data);
		//void			SetUserDataToDefault(WallUserData* sdata);
	protected:
		virtual	void	APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) override;
		virtual void	iUIPointNumberChanged(const DG::PosIntEditChangeEvent& ev);

	public:
		explicit PolygonReducerPageObserver(PolygonReducerInfoboxPage* testPage);
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