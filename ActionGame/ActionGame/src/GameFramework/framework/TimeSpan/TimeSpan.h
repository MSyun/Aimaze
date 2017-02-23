// 時間変換
// 2017. 2.22	プログラム作成
// author		SyunMizuno


#pragma once


class TimeSpan {
public:
	TimeSpan();
	virtual ~TimeSpan();

	float FromMilliseconds(double value);
	float FromSeconds(double value);
	float FromMinutes(double value);
	float FromHours(double value);

private:

};