#pragma once

#include <optional>

#include <global_include.h>

#include <database.h>
#include <homebrew.h>
#include <Views/IMEView.h>
#include <Views/ListView/listView.h>

class SearchView : public ListView {
public:
	SearchView() : ListView(Database::get_instance()->homebrews) {};

	void SignalSelected() override;
	void SignalDeselected() override;
	bool IsReadyToShow() override;

	int Display() override;

private:
	bool listViewOutdated = true;
	
	std::optional<IMEViewResult> _ime_search_view_result = std::nullopt;
	std::future<IMEViewResult> _ime_search_view_result_future;
	void startSearch();
	std::string lastQuery = "";
	
	void UpdateFutureStatus();
};
