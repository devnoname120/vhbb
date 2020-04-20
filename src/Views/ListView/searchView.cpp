#include "searchView.h"

void SearchView::startSearch()
{
    IMEView::openIMEView(_ime_search_view_result, "Search", _ime_search_view_result->userText);
    log_printf(DBG_DEBUG, "Opened search dialog");
}

void SearchView::SignalSelected()
{
    log_printf(DBG_DEBUG, "SearchView::SignalSelected");
    if (_ime_search_view_result->status != COMMON_DIALOG_STATUS_RUNNING)
        startSearch();
}

void SearchView::SignalDeselected()
{
    log_printf(DBG_DEBUG, "SearchView::SignalDeselected");
    IMEView::closeIMEView();
}

bool SearchView::IsReadyToShow()
{
    return _ime_search_view_result->status == COMMON_DIALOG_STATUS_FINISHED;
}

int SearchView::Display()
{
    if (_ime_search_view_result->status == COMMON_DIALOG_STATUS_FINISHED)
    {
        log_printf(DBG_DEBUG, "Processing finished search dialog: \"%s\"", _ime_search_view_result->userText.c_str());
        if (lastQuery != _ime_search_view_result->userText)
        {
            auto db = Database::get_instance();
            std::vector<Homebrew> hbs;
            hbs = db->Search(SearchQuery(_ime_search_view_result->userText));
            homebrews = hbs;

            std::fill(listItems.begin(), listItems.end(), nullptr);
            LoadListItems();
            lastQuery = _ime_search_view_result->userText;
        }
        else
        {
            log_printf(DBG_DEBUG, "Ignore search: same filter as before");
        }
        _ime_search_view_result->status = COMMON_DIALOG_STATUS_NONE;
    }
    return ListView::Display();
}
