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
	defail_list_add(list, "평균 가격: %s", priceData->average_price);
	defail_list_add(list, "거래량: %s", priceData->units_traded);
	defail_list_add(list, "1일 규모: %s", priceData->volume_1day);
	defail_list_add(list, "7일 규모: %s", priceData->volume_7day);
	defail_list_add(list, "구입 가격: %s", priceData->buy_price);
	defail_list_add(list, "매각 가격: %s", priceData->sell_price);

	elm_naviframe_item_push(naviData->ad->naviFrame, currencyName, NULL, NULL, list, NULL);
}
