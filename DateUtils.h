#pragma once
#include <string>
#include <map>


/*
*����java�� SimpleDateFormat˼·��װ��C++��
*
*
**/

struct DateUtilsTm
{
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [1, 12]
    int tm_year;  // since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_mill;  // millisecond
};

#define iEPOCH_YEAR 1970
#define iEPOCH_DAY 1
#define iEPOCH_MONTH 1

#define iLEAP_DATE_DAY 29
#define iLEAP_DATE_MONTH 2
#define iLEAP_YEAR_DIVIDER 4

#define iSECONDS_IN_MINUTE 60
#define iMINUTES_IN_HOUR 60
#define iHOURS_IN_DAY 24

#define iMONTHS_IN_YEAR 12
#define iDAYS_IN_YEAR 365
#define iDAYS_IN_LEAP_YEAR 366

#define sSTRING_DATE_REPRESENTATION_SEPARATOR string("-")


#define iDAYS_IN_JANUARY 31
#define iDAYS_IN_FEBRUARY 28
#define iDAYS_IN_MARCH 31
#define iDAYS_IN_APRIL 30
#define iDAYS_IN_MAY 31
#define iDAYS_IN_JUNE 30
#define iDAYS_IN_JULY 31
#define iDAYS_IN_AUGUST 31
#define iDAYS_IN_SEPTEMBER 30
#define iDAYS_IN_OCTOBER 31
#define iDAYS_IN_NOVEMBER 30
#define iDAYS_IN_DECEMBER 31

#define ONE_MINUTE_MILLS 1000

#define ONE_YEAR_HOURS 8760
class DateUtils
{
public:

    //�ַ���ת�ṹ��
	static DateUtilsTm StringToDatetime(std::string str, std::string strSimpleDateFormat);

    //�ṹ��ת�ַ���
	static std::string DatetimeToString(const DateUtilsTm& tm, std::string format);

    //�ַ���תtimestamp
    static time_t StringToTimestamp(std::string str, std::string strSimpleDateFormat);

    //ʱ���תtimestamp
    static std::string TimestampToString(time_t time, std::string strSimpleDateFormat);

    //��������ת��timestamp��������ע����������������
    static time_t CountDaysAfterEpoch(int iYear, int iMonth, int iDay);

    //�ṹ��תʱ��
    static time_t Tm2Timestamp(DateUtilsTm tm);

    //ʱ���ת�ṹ��
    static DateUtilsTm Timestamp2Tm(time_t time);

    //����w_day, y_day
    static DateUtilsTm CalcWDayYDay(DateUtilsTm tm);

    //��ʼ����̬��Աmap
    static std::map<int, int> InitMonthMap() {
        std::map<int, int> mapDayInMonth;
        mapDayInMonth[0] = iDAYS_IN_JANUARY;
        mapDayInMonth[1] = iDAYS_IN_FEBRUARY;
        mapDayInMonth[2] = iDAYS_IN_MARCH;
        mapDayInMonth[3] = iDAYS_IN_APRIL;
        mapDayInMonth[4] = iDAYS_IN_MARCH;
        mapDayInMonth[5] = iDAYS_IN_JUNE;
        mapDayInMonth[6] = iDAYS_IN_JULY;
        mapDayInMonth[7] = iDAYS_IN_AUGUST;
        mapDayInMonth[8] = iDAYS_IN_SEPTEMBER;
        mapDayInMonth[9] = iDAYS_IN_OCTOBER;
        mapDayInMonth[10] = iDAYS_IN_NOVEMBER;
        mapDayInMonth[11] = iDAYS_IN_DECEMBER;
        return mapDayInMonth;
    }
private:
    static std::map<int, int> mi_days_in_month;
};

