#include "cryptocurrency.h"
#include "curl/curl.h"
#include "json-glib/json-glib.h"

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static size_t
write_json_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size *nmemb;
	currencyData *data = (currencyData *) userp;

	data->json = realloc(data->json, data->size + realsize + 1);
	if(data->json == NULL){
		printf("out of memory");
		return 0;
	}

	memcpy(&(data->json[data->size]), contents, realsize);
	data->size += realsize;
	data->json[data->size] = 0;
	return realsize;
}

void
get_http_data(const char *url, currencyData *data)
{
	CURL *curl;
	CURLcode res;

	data->json = malloc(1);
	data->size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_json_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) data);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}

void
parse_json(appdata_s *ad)
{
	get_http_data("https://api.bithumb.com/public/ticker/ALL", &ad->cd);

	JsonParser *parser = json_parser_new();

	if(json_parser_load_from_data(parser, ad->cd.json, strlen(ad->cd.json), NULL)){
		JsonNode *root = json_parser_get_root(parser);
		JsonObject *rootObject = json_node_get_object(root);
		JsonNode *data = json_object_get_member(rootObject, "data");
		JsonObject *dataObject = json_node_get_object(data);
		char* statusCode = (char *) json_object_get_string_member(rootObject, "status");


		if(strcmp(statusCode, "0000") == 0){
			for(int i = 0; i < 12; i++){
				char* name = ad->currencyList[i];
				JsonNode *currency = json_object_get_member(dataObject, name);
				JsonObject *currencyObject = json_node_get_object(currency);

				currencyPriceData *data = &ad->currencyPriceList[i];
				data->opening_price = (char *) json_object_get_string_member(currencyObject, "opening_price");
				data->closing_price = (char *) json_object_get_string_member(currencyObject, "closing_price");
				data->min_price = (char *) json_object_get_string_member(currencyObject, "min_price");
				data->max_price = (char *) json_object_get_string_member(currencyObject, "max_price");
				data->units_traded = (char *) json_object_get_string_member(currencyObject, "units_traded");
				data->acc_trade_value = (char *) json_object_get_string_member(currencyObject, "acc_trade_value");
				data->prev_closing_price = (char *) json_object_get_string_member(currencyObject, "prev_closing_price");
				data->units_traded_24H = (char *) json_object_get_string_member(currencyObject, "units_traded_24H");
				data->acc_trade_value_24H = (char *) json_object_get_string_member(currencyObject, "acc_trade_value_24H");
				data->fluctate_24H = (char *) json_object_get_string_member(currencyObject, "fluctate_24H");
				data->fluctate_rate_24H = (char *) json_object_get_string_member(currencyObject, "fluctate_rate_24H");
				data->date = json_object_get_int_member(currencyObject, "date");
			}
		}
	}
}

static void
currency_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = event_info;
	elm_list_item_selected_set(item, EINA_FALSE);
}

static Eina_Bool
naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	ui_app_exit();
	return EINA_FALSE;
}

static void
display_data(appdata_s *ad)
{
	parse_json(ad);

	char *buf[256];
	Evas_Object *list = elm_list_add(ad->naviFrame);
	Elm_Object_Item *naviFrameItem;

	evas_object_smart_callback_add(list, "selected", currency_selected_cb, NULL);

	for(int i = 0; i < 12; i++){
		currencyPriceData *data = &ad->currencyPriceList[i];
		naviData *naviData = malloc(sizeof *naviData);
		naviData->ad = ad;
		naviData->index = i;

		sprintf(buf, "%s : %sWon ~ %sWon", ad->currencyList[i], data->min_price, data->max_price);
		elm_list_item_append(list, buf, NULL, NULL, currency_item_clicked, naviData);
	}

	elm_list_go(list);

	 naviFrameItem = elm_naviframe_item_push(ad->naviFrame, "Crypto Currency", NULL, NULL, list, NULL);
	 elm_naviframe_item_pop_cb_set(ad->naviFrame, naviframe_pop_cb, ad->win);
}

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	ad->layout = elm_layout_add(ad->conform);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	evas_object_show(ad->layout);
	elm_object_content_set(ad->conform, ad->layout);

	ad->naviFrame = elm_naviframe_add(ad->layout);
	display_data(ad);
	elm_naviframe_prev_btn_auto_pushed_set(ad->naviFrame, EINA_TRUE);
	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->naviFrame);

	eext_object_event_callback_add(ad->naviFrame, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	eext_object_event_callback_add(ad->naviFrame, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, NULL);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;
	char *currencyList[] = {"BTC", "ETH", "DASH", "LTC", "ETC", "XRP", "BCH", "XMR", "ZEC", "QTUM", "BTG", "EOS"};
	for(int i = 0; i < 12; i++){
		ad->currencyList[i] = currencyList[i];
	}

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/

	int ret;
	char *language;

	ret = app_event_get_language(event_info, &language);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_event_get_language() failed. Err = %d.", ret);
		return;
	}

	if (language != NULL) {
		elm_language_set(language);
		free(language);
	}
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
