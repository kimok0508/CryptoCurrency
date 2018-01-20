#ifndef __cryptocurrency_H__
#define __cryptocurrency_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "cryptocurrency"

#if !defined(PACKAGE)
#define PACKAGE "kr.edcan.cryptocurrency"
#endif

typedef struct CurrencyPriceData{
	char *opening_price;
	char *closing_price;
	char *min_price;
	char *max_price;
	char *average_price;
	char *units_traded;
	char *volume_1day;
	char *volume_7day;
	char *buy_price;
	char *sell_price;
	long date;
} currencyPriceData;

typedef struct CurrencyData{
	char *json;
	size_t size;
} currencyData;

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *layout;
	Evas_Object *naviFrame;
	char *currencyList[12];
	currencyData cd;
	currencyPriceData currencyPriceList[12];
} appdata_s;

typedef struct NaviData{
	int index;
	appdata_s *ad;
} naviData;

void
currency_item_clicked(void *data, Evas_Object *obj, void *event_info);

#endif /* __cryptocurrency_H__ */
