#include <stdio.h>

static int daytab[2][13] {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static const char *month_names[] {
  "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


int day_of_year(int, int, int);
bool month_day(int, int, int *, int *);
const char *postfix_for(int);

int main() {
  int x = day_of_year(2024, 7, 19);
  printf("July 19 is the %d%s day of 2024.\n", x, postfix_for(x));
  x = day_of_year(2023, 7, 19);
  printf("July 19 is the %d%s day of 2023.\n", x, postfix_for(x));
  x = day_of_year(2023, 2, 28);
  printf("Feb 28 is the %d%s day of 2023.\n", x, postfix_for(x));
  x = day_of_year(2024, 2, 29);
  printf("Feb 29 is the %d%s day of 2024.\n", x, postfix_for(x));

  int month, day;
  month_day(2024, 301, &month, &day);
  printf("The 301st day of 2024 is %s %d%s\n", month_names[month], day, postfix_for(day));

  printf("There are only 365 days in a year or 366 in a leap year. It should be impossible to get the month & day from a higher than that.\n");
  month_day(2024, 366, &month, &day);
  printf("The 366th day of 2024 is %s %d%s\n", month_names[month], day, postfix_for(day));
  month_day(2023, 365, &month, &day);
  printf("The 365th day of 2023 is %s %d%s\n", month_names[month], day, postfix_for(day));
  if (month_day(2024, 367, &month, &day)) {
    printf("month_day rejects the 367th day of 2024, as expected.\n");
  } else {
    printf("Oops. month_day doesn't validate properly.\n");
  }
  if (month_day(2023, 366, &month, &day)) {
    printf("month_day rejects the 366th day of 2023, as expected.\n");
  } else {
    printf("Oops. month_day doesn't validate properly.\n");
  }

  if (day_of_year(2024, 13, 1) >= 0) {
    printf("Oops. day_of_year doesn't validate properly.\n");
    return 1;
  } else {
    printf("day_of_year rejects month = 13 as expected.\n");
  }

  if (day_of_year(2024, 2, 30) >= 0) {
    printf("Oops. day_of_year doesn't validate properly.\n");
    return 1;
  } else {
    printf("day_of_year rejects Feb 30 as expected.\n");
  }

  if (day_of_year(2023, 2, 29) >= 0) {
    printf("Oops. day_of_year doesn't validate properly.\n");
    return 1;
  } else {
    printf("day_of_year rejects Feb 29, 2023 as expected.\n");
  }
  return 0;
}

const char *postfix_for(int x) {
  if (x != 11 && x%10 == 1) {
    return "st";
  }
  if (x != 12 && x%10 == 2) {
    return "nd";
  }
  if (x != 13 && x%10 == 3) {
    return "rd";
  }
  return "th";
}

bool is_leap(int year) {
  return year%4 == 0 && year%100 != 0 || year%400 == 0;
}

int day_of_year(int year, int month, int day) {
  if (month > 12) {
    return -1;
  }
  if (day > daytab[is_leap(year)][month]) {
    return -1;
  }
  int i;

  for (i = 1; i < month; i++) {
    day += daytab[is_leap(year)][i];
  }
  return day;
}

bool month_day(int year, int yearday, int *pmonth, int *pday) {
  if (yearday > 366) {
    return true;
  }
  if (yearday == 366 && !is_leap(year)) {
    return true;
  }
  int i;
  bool leap = is_leap(year);

  for (i = 1; yearday > daytab[leap][i]; i++) {
    yearday -= daytab[leap][i];
  }
  *pmonth = i;
  *pday = yearday;
  return false;
}
