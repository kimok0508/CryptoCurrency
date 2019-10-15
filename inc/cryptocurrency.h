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


//				"opening_price":"215900",
//				"closing_price":"217900",
//				"min_price":"214000",
//				"max_price":"222600",
//				"units_traded":"64164.88",
//				"acc_trade_value":"14058840173.05",
//				"prev_closing_price":"215800",
//				"units_traded_24H":"66350.04",
//				"acc_trade_value_24H":"14530986261.3",
//				"fluctate_24H":"2,100",
//				"fluctate_rate_24H":"0.97"
typedef struct CurrencyPriceData{
	char *opening_price;
	char *closing_price;
	char *min_price;
	char *max_price;
	char *units_traded;
	char *acc_trade_value;
	char *prev_closing_price;
	char *units_traded_24H;
	char *acc_trade_value_24H;
	char *fluctate_24H;
	char *fluctate_rate_24H;
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
