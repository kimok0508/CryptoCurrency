#include "cryptocurrency.h"

static void
item_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = event_info;
	elm_list_item_selected_set(item, EINA_FALSE);
}

void
defail_list_add(Evas_Object *list, char *format, void *data)
{
	char *buf[256];

	sprintf(buf, format, data);
	elm_list_item_append(list, buf, NULL, NULL, item_selected_cb, NULL);
}

void
currency_item_clicked(void *data, Evas_Object *obj, void *event_info)
{
	naviData *naviData = data;
	const char *currencyName = naviData->ad->currencyList[naviData->index];

	printf("%d", naviData->index);

	Evas_Object *list = elm_list_add(naviData->ad->naviFrame);
	currencyPriceData *priceData = &naviData->ad->currencyPriceList[naviData->index];

	defail_list_add(list, "개장 가격: %s", priceData->opening_price);
	defail_list_add(list, "폐장 가격: %s", priceData->closing_price);
	defail_list_add(list, "최저 가격: %s", priceData->min_price);
	defail_list_add(list, "최고 가격: %s", priceData->max_price);
	defail_list_add(list, "거래량: %s", priceData->units_traded);
	defail_list_add(list, "거래 금액: %s", priceData->acc_trade_value);
	defail_list_add(list, "전일 폐장가: %s", priceData->prev_closing_price);
	defail_list_add(list, "최근 24시간 거래량: %s", priceData->units_traded_24H);
	defail_list_add(list, "최근 24시간 거래 금액: %s", priceData->acc_trade_value_24H);
	defail_list_add(list, "최근 24시간 변동가: %s", priceData->fluctate_24H);
	defail_list_add(list, "최근 24시간 변동률: %s", priceData->fluctate_rate_24H);

	elm_naviframe_item_push(naviData->ad->naviFrame, currencyName, NULL, NULL, list, NULL);
}
