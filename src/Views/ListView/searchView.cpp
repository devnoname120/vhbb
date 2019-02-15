#include "searchView.h"


void SearchView::startSearch() {
	try {
		_ime_search_view_result_future = IMEView::openIMEView("Search", lastQuery);
		_ime_search_view_result = std::nullopt;
		listViewOutdated = true;
	} catch (const IMEViewAlreadyRunningException &ex) {
		log_printf(DBG_ERROR, "%s", ex.what());
		return;
	}
	
	log_printf(DBG_DEBUG, "Opened search dialog");
}


void SearchView::SignalSelected() {
	log_printf(DBG_DEBUG, "SearchView::SignalSelected");
	startSearch();
}

void SearchView::SignalDeselected() {
	log_printf(DBG_DEBUG, "SearchView::SignalDeselected");
	IMEView::closeIMEView();
}

bool SearchView::IsReadyToShow() {
	log_printf(DBG_DEBUG, "1");
	UpdateFutureStatus();
	log_printf(DBG_DEBUG, "2");
	
	return _ime_search_view_result.has_value();
}

int SearchView::Display() {
	log_printf(DBG_DEBUG, "3");
	UpdateFutureStatus();
	log_printf(DBG_DEBUG, "4");
	
	if (_ime_search_view_result.has_value() && listViewOutdated) {
		log_printf(DBG_DEBUG, "5");
		IMEViewResult &result = _ime_search_view_result.value();
		
		log_printf(DBG_DEBUG, "Processing finished search dialog: \"%s\"", result.userText.c_str());
		if (lastQuery != result.userText) {
			auto db = Database::get_instance();
			std::vector<Homebrew> hbs;
			hbs = db->Search(SearchQuery(result.userText));
			listItems.clear();
			for (Homebrew hb : hbs) {
				listItems.emplace_back(hb);
			}
			lastQuery = result.userText;
		} else {
			log_printf(DBG_DEBUG, "Ignore search: same filter as before");
		}
		listViewOutdated = false;
	}
	return ListView::Display();
}

void SearchView::UpdateFutureStatus() {
	log_printf(DBG_DEBUG, "6");
	if (!_ime_search_view_result.has_value() && _ime_search_view_result_future.valid()) {
		log_printf(DBG_DEBUG, "7");
		std::future_status status = _ime_search_view_result_future.wait_for(std::chrono::seconds(0));
		log_printf(DBG_DEBUG, "8");
		log_printf(DBG_DEBUG, "%d", status);
		if (status == std::future_status::ready) {
			_ime_search_view_result = _ime_search_view_result_future.get();
		}
	}
}
