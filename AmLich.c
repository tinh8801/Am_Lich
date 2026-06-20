/**
 * This code is converted from javascript version developed by Ho Ngoc Duc
 * Where the license is for non-commercial use. If you want to use it for commercial purpose
 * please contact the original author at
 * http://www.informatik.uni-leipzig.de/~duc/amlich/
 */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define PRINT_DEBUG false
#if PRINT_DEBUG
	#define DEBUG
#endif

#ifdef _WIN32
#include <windows.h>
#endif

	

typedef struct {
	int day;
	int month;
	int year;
} solar_date;


typedef struct {
	int day;
	int month;
	int year;
	int leap;
} lunar_date;

int jd_from_date(int dd, int mm, int yyyy) {
	int a, y, m, jd;
	a = (int) ((14 - mm) / 12);
	y = yyyy + 4800 - a;
	m = mm + 12 * a - 3;
	jd = dd + (int) ((153 * m + 2) / 5) + 365 * y + (int) (y / 4)
			- (int) (y / 100) + (int) (y / 400) - 32045;
	if (jd < 2299161) {
		jd = dd + (int) ((153 * m + 2) / 5) + 365 * y + (int) (y / 4) - 32083;
	}
	#ifdef DEBUG
		printf("jd_from_date: %d\n", jd);
	#endif
	return jd;
}

solar_date *jd_to_date(int jd) {
	int a, b, c, d, e, m, day, month, year;
	solar_date *date = (solar_date *) malloc(sizeof(solar_date));

	if (jd > 2299160) { // After 5/10/1582, Gregorian calendar
		a = jd + 32044;
		b = (int) ((4 * a + 3) / 146097);
		c = a - (int) ((b * 146097) / 4);
	} else {
		b = 0;
		c = jd + 32082;
	}
	d = (int) ((4 * c + 3) / 1461);
	e = c - (int) ((1461 * d) / 4);
	m = (int) ((5 * e + 2) / 153);
	day = e - (int) ((153 * m + 2) / 5) + 1;
	month = m + 3 - 12 * (int) (m / 10);
	year = b * 100 + d - 4800 + (int) (m / 10);
	date->day = day;
	date->month = month;
	date->year = year;
	#ifdef DEBUG
		printf("jd_to_date: %d\n", jd);
	#endif
	return date;
}

int get_new_moon_day(int k, int time_zone) {
	double T, T2, T3, dr, Jd1, M, m_pr, F, C1, deltat, jd_new;
	T = k / 1236.85; // Time in Julian centuries from 1900 January 0.5
	T2 = T * T;
	T3 = T2 * T;
	dr = M_PI / 180;
	Jd1 = 2415020.75933 + 29.53058868 * k + 0.0001178 * T2 - 0.000000155 * T3;
	Jd1 = Jd1 + 0.00033 * sin((166.56 + 132.87 * T - 0.009173 * T2) * dr); // Mean new moon
	M = 359.2242 + 29.10535608 * k - 0.0000333 * T2 - 0.00000347 * T3; // Sun's mean anomaly
	m_pr = 306.0253 + 385.81691806 * k + 0.0107306 * T2 + 0.00001236 * T3; // Moon's mean anomaly
	F = 21.2964 + 390.67050646 * k - 0.0016528 * T2 - 0.00000239 * T3; // Moon's argument of latitude
	C1 = (0.1734 - 0.000393 * T) * sin(M * dr) + 0.0021 * sin(2 * dr * M);
	C1 = C1 - 0.4068 * sin(m_pr * dr) + 0.0161 * sin(dr * 2 * m_pr);
	C1 = C1 - 0.0004 * sin(dr * 3 * m_pr);
	C1 = C1 + 0.0104 * sin(dr * 2 * F) - 0.0051 * sin(dr * (M + m_pr));
	C1 = C1 - 0.0074 * sin(dr * (M - m_pr)) + 0.0004 * sin(dr * (2 * F + M));
	C1 = C1 - 0.0004 * sin(dr * (2 * F - M)) - 0.0006 * sin(dr * (2 * F + m_pr));
	C1 = C1 + 0.0010 * sin(dr * (2 * F - m_pr))
			+ 0.0005 * sin(dr * (2 * m_pr + M));
	if (T < -11) {
		deltat = 0.001 + 0.000839 * T + 0.0002261 * T2 - 0.00000845 * T3
				- 0.000000081 * T * T3;
	} else {
		deltat = -0.000278 + 0.000265 * T + 0.000262 * T2;
	};
	jd_new = Jd1 + C1 - deltat;
	#ifdef DEBUG
		printf("jd_new: %f\n", jd_new);
	#endif
	int result = (int)(jd_new + 0.5 + (time_zone/24.0));
	#ifdef DEBUG
		printf("get_new_moon_day: %d\n", result);
	#endif
	return result;
}

int get_sun_longitude(int jd, int time_zone) {
	double T, T2, dr, M, L0, DL, L, omega;
	T = (jd - 2451545.5 - time_zone/24.0) / 36525.0; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
	T2 = T * T;
	dr = M_PI / 180; // degree to radian
	M = 357.52910 + 35999.05030 * T - 0.0001559 * T2 - 0.00000048 * T * T2; // mean anomaly, degree
	L0 = 280.46645 + 36000.76983 * T + 0.0003032 * T2; // mean longitude, degree
	DL = (1.914600 - 0.004817 * T - 0.000014 * T2) * sin(dr * M);
	DL = DL + (0.019993 - 0.000101 * T) * sin(dr * 2 * M)
			+ 0.000290 * sin(dr * 3 * M);
	L = L0 + DL; // true longitude, degree
	omega = 125.04 - 1934.136 * T;
	L = L - 0.00569 - 0.00478 * sin(omega * dr);
	L = L * dr;
	L = L - M_PI * 2 * ((int) (L / (M_PI * 2))); // Normalize to (0, 2*PI)
	int result = (int)(L / M_PI * 6);
	#ifdef DEBUG
		printf("get_sun_longitude: %d\n", result);
	#endif
	return result;
}

//int get_sun_longitude(int jd, int time_zone) {
	//return (int) (sun_longitude(jd - 0.5 - time_zone / 24) / M_PI * 6);
//}

//int get_new_moon_day(int k, int time_zone) {
//	return (int) (new_moon(k) + 0.5 + time_zone / 24);
//}

int get_lunar_month11(int yyyy, int time_zone) {
	int k, off, nm, sun_long;
	off = jd_from_date(31, 12, yyyy) - 2415021;
	k = (int) (off / 29.530588853);
	nm = get_new_moon_day(k, time_zone);
	sun_long = get_sun_longitude(nm, time_zone); // sun longitude at local midnight
	if (sun_long >= 9) {
		nm = get_new_moon_day(k - 1, time_zone);
	}
	#ifdef DEBUG
		printf("get_lunar_month11: %d\n", nm);
	#endif
	return nm;
}

int get_leap_month_offset(int a11, int time_zone) {
	int k, last, arc, i;
	k = (int) ((a11 - 2415021.076998695) / 29.530588853 + 0.5);
	last = 0;
	i = 1; // We start with the month following lunar month 11
	arc = get_sun_longitude(get_new_moon_day(k + i, time_zone), time_zone);
	do {
		last = arc;
		i++;
		arc = get_sun_longitude(get_new_moon_day(k + i, time_zone), time_zone);
	} while (arc != last && i < 14);
	#ifdef DEBUG
		printf("get_leap_month_offset: %d\n", i - 1);
	#endif
	return i - 1;
}

lunar_date *solar2lunar(int dd, int mm, int yyyy, int time_zone) {
	int k, day_number, month_start, a11, b11, lunar_day, lunar_month, lunar_year,
			lunar_leap, diff, leap_month_diff;

	day_number = jd_from_date(dd, mm, yyyy);

	k = (int) ((day_number - 2415021.076998695) / 29.530588853);
	month_start = get_new_moon_day(k + 1, time_zone);
	if (month_start > day_number) {
		month_start = get_new_moon_day(k, time_zone);
	}
	a11 = get_lunar_month11(yyyy, time_zone);
	b11 = a11;
	if (a11 >= month_start) {
		lunar_year = yyyy;
		a11 = get_lunar_month11(yyyy - 1, time_zone);
	} else {
		lunar_year = yyyy + 1;
		b11 = get_lunar_month11(yyyy + 1, time_zone);
	}
	lunar_day = day_number - month_start + 1;
	diff = (int) ((month_start - a11) / 29);
	lunar_leap = 0;
	lunar_month = diff + 11;
	if (b11 - a11 > 365) {
		leap_month_diff = get_leap_month_offset(a11, time_zone);
		if (diff >= leap_month_diff) {
			lunar_month = diff + 10;
			if (diff == leap_month_diff) {
				lunar_leap = 1;
			}
		}
	}
	if (lunar_month > 12) {
		lunar_month = lunar_month - 12;
	}
	if (lunar_month >= 11 && diff < 4) {
		lunar_year -= 1;
	}
	lunar_date * res;
	res = (lunar_date *) malloc(sizeof(lunar_date));
	res->day = lunar_day;
	res->month = lunar_month;
	res->year = lunar_year;
	res->leap = lunar_leap;
	return res;
}

solar_date *lunar2solar(int lunar_day, int lunar_month, int lunar_year,
		int lunar_leap, int time_zone) {
	int k, a11, b11, off, leap_off, leap_month, month_start;
	solar_date *res;

	if (lunar_month < 11) {
		a11 = get_lunar_month11(lunar_year - 1, time_zone);
		b11 = get_lunar_month11(lunar_year, time_zone);
	} else {
		a11 = get_lunar_month11(lunar_year, time_zone);
		b11 = get_lunar_month11(lunar_year + 1, time_zone);
	}
	k = (int) (0.5 + (a11 - 2415021.076998695) / 29.530588853);
	off = lunar_month - 11;
	if (off < 0) {
		off += 12;
	}
	if (b11 - a11 > 365) {
		leap_off = get_leap_month_offset(a11, time_zone);
		leap_month = leap_off - 2;
		if (leap_month < 0) {
			leap_month += 12;
		}
		if (lunar_leap != 0 && lunar_month != leap_month) {
			res = (solar_date *)malloc(sizeof(solar_date));
			res->day = 0;
			res->month = 0;
			res->year = 0;
			return res;
		} else if (lunar_leap != 0 || off >= leap_off) {
			off += 1;
		}
	}
	month_start = get_new_moon_day(k + off, time_zone);
	res = jd_to_date(month_start + lunar_day - 1);

	return res;
}

int test(){
	#ifdef DEBUG
		printf("%s", "DEBUG Enabled\n\n");
	#else
		printf("%s", "DEBUG Disabled\n\n");
	#endif
	
	#ifdef _WIN32
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
	#endif

	
	solar_date *sd = (solar_date *) malloc(sizeof(solar_date));
	lunar_date *ld = (lunar_date *) malloc(sizeof(lunar_date));
	
	printf("Ngày âm lịch 1/6(nhuận)/2025 là ngày dương lịch 25/7/2025\n");
	sd = lunar2solar(1, 6, 2025, 1, 7);
	printf("lunar2solar(1, 6, 2025, 1, 7): %d-%d-%d\n\n", sd->day, sd->month, sd->year);
	
	printf("Ngày âm lịch 1/6/2025 là ngày dương lịch 25/6/2025\n");
	sd = lunar2solar(1, 6, 2025, 0, 7);
	printf("lunar2solar(1, 6, 2025, 0, 7): %d-%d-%d\n\n", sd->day, sd->month, sd->year);
	
	printf("Ngày dương lịch 6/6/2026 là ngày âm lịch 21/4/2026\n");
	ld = solar2lunar(6, 6, 2026, 7);
	printf("solar2lunar(6, 6, 2026, 7): %d-%d-%d-(leap: %d)\n\n", ld->day, ld->month, ld->year, ld->leap);
	
	printf("Ngày dương lịch 22/3/2023 là ngày âm lịch 1/2(nhuận)/2023\n");
	ld = solar2lunar(22, 3, 2023, 7);
	printf("solar2lunar(22, 3, 2023, 7): %d-%d-%d-(leap: %d)\n\n", ld->day, ld->month, ld->year, ld->leap);
	
	printf("Ngày dương lịch 21/1/2023 là ngày âm lịch 30/12/2022\n");
	ld = solar2lunar(21, 1, 2023, 7);
	printf("solar2lunar(21, 1, 2023, 7): %d-%d-%d-(leap: %d)\n\n", ld->day, ld->month, ld->year, ld->leap);
	
	
	return 0;
}

int main(){
	time_t t = time(NULL);
	struct tm *current_time = localtime(&t);
	int day = current_time->tm_mday;
	int month = current_time->tm_mon+1;
	int year = current_time->tm_year+1900;
	printf("Current system date: %02d-%02d-%04d\n", day, month, year);
	lunar_date *ld = (lunar_date *) malloc(sizeof(lunar_date));
	ld = solar2lunar(day, month, year, 7);
	printf("Lunar date: %02d-%02d-%04d (leap: %d)\n", ld->day, ld->month, ld->year, ld->leap);
	//test();
	return 0;
}