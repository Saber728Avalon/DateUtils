#include "stdafx.h"
#include "DateUtils.h"
#include <time.h>
#include <chrono>

//   static final int PATTERN_ERA                  =  0; // G
//   static final int PATTERN_YEAR                 =  1; // y
//   static final int PATTERN_MONTH                =  2; // M
//   static final int PATTERN_DAY_OF_MONTH         =  3; // d
//   static final int PATTERN_HOUR_OF_DAY1         =  4; // k
//   static final int PATTERN_HOUR_OF_DAY0         =  5; // H
//   static final int PATTERN_MINUTE               =  6; // m
//   static final int PATTERN_SECOND               =  7; // s
//   static final int PATTERN_MILLISECOND          =  8; // S
//   static final int PATTERN_DAY_OF_WEEK          =  9; // E
//   static final int PATTERN_DAY_OF_YEAR          = 10; // D
//   static final int PATTERN_DAY_OF_WEEK_IN_MONTH = 11; // F
//   static final int PATTERN_WEEK_OF_YEAR         = 12; // w
//   static final int PATTERN_WEEK_OF_MONTH        = 13; // W
//   static final int PATTERN_AM_PM                = 14; // a
//   static final int PATTERN_HOUR1                = 15; // h
//   static final int PATTERN_HOUR0                = 16; // K
//   static final int PATTERN_ZONE_NAME            = 17; // z
//   static final int PATTERN_ZONE_VALUE           = 18; // Z
//   static final int PATTERN_WEEK_YEAR            = 19; // Y
//   static final int PATTERN_ISO_DAY_OF_WEEK      = 20; // u
//   static final int PATTERN_ISO_ZONE             = 21; // X
//   static final int PATTERN_STANDALONE_MONTH     = 22; // L
//   static final int PATTERN_STANDALONE_DAY_OF_WEEK = 23; // c

// ensures an int takes up at least 2 spaces
inline std::string to_string_with0(const int i, int nPaddingCount) {
	int nOrigLen = 0;
	std::string strRst = std::to_string(i);
	nOrigLen = strRst.length();
	for (int i = 0; i < nPaddingCount - nOrigLen; i++)
	{
		strRst = '0' + strRst;
	}
	return strRst;
}

inline std::string fullMonthName(const int month) {
	static const std::string names[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	return (0 <= month && month < 12) ? names[month] : "Invalid Month";
}

inline std::string shortMonthName(const int month) {
	static const std::string names[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	return (0 <= month && month < 12) ? names[month] : "Inv";
}

inline int ByteCount(char* pBegin, char ch)
{
	int i = 0;
	for (i = 0; pBegin[i] == ch; i++);
	return i;
}

inline int iCountLeapYearsSinceEpoch(int iYear) {
	return (iYear - iEPOCH_YEAR) / iLEAP_YEAR_DIVIDER;
}


std::map<int, int> DateUtils::mi_days_in_month = DateUtils::InitMonthMap();

//YY-MM-DD hh:mm:ss 
DateUtilsTm DateUtils::StringToDatetime(std::string str, std::string format)
{
	DateUtilsTm tm = { 0 };
	for (std::size_t i = 0; i < format.size(); ) {

		char cmd = format[i]; // character after %
		std::string toIns;
		char* pTmp;
		int nCount = ByteCount(&format[i], cmd);
		switch (cmd) {

		case 'y': // last 2 digits of year
			toIns = str.substr(i, nCount);
			tm.tm_year = strtol(toIns.c_str(), &pTmp, 10);
			break;


		case 'D': // day of the month with preceeding 0, ex: 03
			//toIns = to_string_with0(tm.tm_mday);
			break;

		case 'd': // day of the month
			toIns = str.substr(i, nCount);
			tm.tm_mday = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'H': // hour using 24 hour clock
			toIns = str.substr(i, nCount);
			tm.tm_hour = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'h': // hour using 12 hour clock
			toIns = str.substr(i, nCount);
			tm.tm_hour = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'M': // minute with preceeding 0
			toIns = str.substr(i, nCount);
			tm.tm_mon = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'm': // minutes without preceeding 0
			toIns = str.substr(i, nCount);
			tm.tm_min = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'S': // seconds with preceeding 0
			toIns = str.substr(i, nCount);
			tm.tm_mill = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 's': // seconds without preceeding 0
			toIns = str.substr(i, nCount);
			tm.tm_sec = strtol(toIns.c_str(), &pTmp, 10);
			break;

		case 'p':  // whether it is AM or PM, lowercase
			toIns = (tm.tm_hour > 12) ? "pm" : "am";
			break;

		case 'P': // whether it is AM or PM
			toIns = (tm.tm_hour > 12) ? "PM" : "AM";
			break;

		default:
			toIns = cmd;
			break;

			//default: break;
		}

		i += nCount;
	}

	tm.tm_year -= 1900;
	tm = CalcWDayYDay(tm);
	return tm;                                 // 返回值。 
}


time_t DateUtils::CountDaysAfterEpoch(int iYear, int iMonth, int iDay) {
	int i_leap_days_since_epoch = iCountLeapYearsSinceEpoch(iYear);

	if (iYear % iLEAP_YEAR_DIVIDER == 1 || (iYear % iLEAP_YEAR_DIVIDER == 0 && (iMonth > iLEAP_DATE_MONTH || (iMonth == iLEAP_DATE_MONTH && iDay > iLEAP_DATE_DAY))))
		i_leap_days_since_epoch++;

	int i_days_in_past_months = 0;

	for (int i = 0; i < iMonth - 1; i++) {
		i_days_in_past_months += mi_days_in_month[i];
	}

	return iDAYS_IN_YEAR * (iYear - iEPOCH_YEAR) + i_leap_days_since_epoch + i_days_in_past_months + iDay - 1;
}

std::string DateUtils::DatetimeToString(const DateUtilsTm& tm, std::string format)
{
	std::string strRst;
	for (std::size_t i = 0; i < format.size(); ) {

		char cmd = format[i]; // character after %
		std::string toIns;

		int nCount = ByteCount(&format[i], cmd);
		switch (cmd) {

		case 'y': // last 2 digits of year
			toIns = to_string_with0(tm.tm_year + 1900, nCount);
			break;


		case 'D': // day of the month with preceeding 0, ex: 03
			//toIns = to_string_with0(tm.tm_mday);
			break;

		case 'd': // day of the month
			toIns = to_string_with0(tm.tm_mday, nCount);
			break;

		case 'H': // hour using 24 hour clock
			toIns = to_string_with0(tm.tm_hour, nCount);
			break;

		case 'h': // hour using 12 hour clock
		{
			int nHour = tm.tm_hour - ((tm.tm_hour > 12) ? 12 : 0);
			toIns = to_string_with0(nHour, nCount);
		}
			break;

		case 'M': // minute with preceeding 0
			toIns = to_string_with0(tm.tm_mon, nCount);
			break;

		case 'm': // minutes without preceeding 0
			toIns = to_string_with0(tm.tm_min, nCount);
			break;

		case 'S': // seconds with preceeding 0
			toIns = to_string_with0(tm.tm_mill, nCount);
			break;

		case 's': // seconds without preceeding 0
			toIns = to_string_with0(tm.tm_sec, nCount);
			break;

		case 'p':  // whether it is AM or PM, lowercase
			toIns = (tm.tm_hour > 12) ? "pm" : "am";
			break;

		case 'P': // whether it is AM or PM
			toIns = (tm.tm_hour > 12) ? "PM" : "AM";
			break;

		default:
			toIns = cmd;
			break;

			//default: break;
		}
		strRst.append(toIns);
		i += nCount;
	}

	return strRst;
}

time_t DateUtils::StringToTimestamp(std::string str, std::string strSimpleDateFormat)
{
	DateUtilsTm tm = StringToDatetime(str, strSimpleDateFormat);
	time_t timstamp = Tm2Timestamp(tm);
	return timstamp;
}


//结构体转时间
time_t DateUtils::Tm2Timestamp(DateUtilsTm tm)
{
	tm.tm_year += 1900;
	time_t mills = CountDaysAfterEpoch(tm.tm_year, tm.tm_mon, tm.tm_mday);
	mills = mills * iHOURS_IN_DAY * iMINUTES_IN_HOUR * iSECONDS_IN_MINUTE
		+ (tm.tm_hour) * iMINUTES_IN_HOUR * iSECONDS_IN_MINUTE
		+ (tm.tm_min) * iSECONDS_IN_MINUTE
		+ tm.tm_sec;
	mills *= ONE_MINUTE_MILLS;
	mills += tm.tm_mill;
	return mills;
}


DateUtilsTm DateUtils::CalcWDayYDay(DateUtilsTm tm)
{
	int i_days_in_past_months = 0;

	for (int i = 0; i < tm.tm_mon - 1; i++) {
		i_days_in_past_months += mi_days_in_month[i];
	}

	tm.tm_yday = i_days_in_past_months + tm.tm_mday;

	time_t day = CountDaysAfterEpoch(tm.tm_year, tm.tm_mon, tm.tm_mday);
	
	//1970-01-01 is Thursday
	day = (day + 4) % 7;

	tm.tm_wday = day;
	return tm;
}

DateUtilsTm DateUtils::Timestamp2Tm(time_t time)
{
	DateUtilsTm tm = { 0 };
	tm.tm_mill = time % ONE_MINUTE_MILLS;
	time_t secondQuot = time / ONE_MINUTE_MILLS;

	tm.tm_sec = secondQuot % iSECONDS_IN_MINUTE;
	time_t minuteQuot = secondQuot / iSECONDS_IN_MINUTE;

	tm.tm_min = minuteQuot % iMINUTES_IN_HOUR;
	time_t hourQuot = minuteQuot / iMINUTES_IN_HOUR;

	tm.tm_hour = hourQuot % iHOURS_IN_DAY;
	time_t dayQuot = hourQuot / iHOURS_IN_DAY;

	tm.tm_wday = (dayQuot + 4) % 7;

	//将从1970转换到从1900的年差
	int nPass4year = dayQuot / 1461;//4年1461天
	tm.tm_year = (nPass4year << 4) + 70;// 

	int nPass4yearQuot = dayQuot % 1461;
	tm.tm_yday = nPass4yearQuot % 365;
	for (;;)
	{
		int nHpery = iDAYS_IN_YEAR;
		if ((tm.tm_year & 3) == 0)
		{
			nHpery += 1;
		}

		if (dayQuot < nHpery)
		{
			break;
		}

		tm.tm_year++;
		dayQuot -= nHpery;
	}

	//接下来计算月与天
	int nDay = tm.tm_yday;
	for (tm.tm_mon = 0; mi_days_in_month[tm.tm_mon] < nDay; tm.tm_mon++)
	{
		nDay -= mi_days_in_month[tm.tm_mon];
	}

	tm.tm_mon++;
	tm.tm_mday = nDay + 1;
	return tm;
}


std::string DateUtils::TimestampToString(time_t time, std::string strSimpleDateFormat)
{
	DateUtilsTm tm = Timestamp2Tm(time);
	std::string strDate = DatetimeToString(tm, strSimpleDateFormat);
	return strDate;
}