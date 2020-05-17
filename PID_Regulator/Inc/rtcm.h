
#define SPANISH_LANGUAGE

#define RTC_UNKNOWN	"Unknown"

#ifdef ENGLISH_LANGUAJE
/* Days of a Week */
#define RTC_MONDAY "Mon"
#define RTC_TUESDAY "Tue"
#define RTC_WEDNESDAY "Wed"
#define RTC_THURSDAY "Thu"
#define RTC_FRIDAY "Fri"
#define RTC_SATURDAY "Sat"
#define RTC_SUNDAY "Sun"
/* Months of a year */
#define RTC_JAN	"Jan"
#define RTC_FEB "Feb"
#define RTC_MAR "Mar"
#define RTC_APR "Apr"
#define RTC_MAY "May"
#define RTC_JUN "Jun"
#define RTC_JUL "Jul"
#define RTC_AUG "Aug"
#define RTC_SEP "Sep"
#define RTC_OCT "Oct"
#define RTC_NOV "Nov"
#define RTC_DIC "Dec"
#endif

#ifdef SPANISH_LANGUAGE
/* Dias de la semana */
#define RTC_MONDAY "Lun"
#define RTC_TUESDAY "Mar"
#define RTC_WEDNESDAY "Mie"
#define RTC_THURSDAY "Jue"
#define RTC_FRIDAY "Vie"
#define RTC_SATURDAY "Sab"
#define RTC_SUNDAY "Dom"
/* Meses del año */
#define RTC_JAN	"Ene"
#define RTC_FEB "Feb"
#define RTC_MAR "Mar"
#define RTC_APR "Abr"
#define RTC_MAY "May"
#define RTC_JUN "Jun"
#define RTC_JUL "Jul"
#define RTC_AUG "Ago"
#define RTC_SEP "Sep"
#define RTC_OCT "Oct"
#define RTC_NOV "Nov"
#define RTC_DIC "Dic"
#endif

typedef struct rct_cfg_s
{
	  RTC_DateTypeDef sdatestructure;
	  RTC_TimeTypeDef stimestructure;
	  RTC_AlarmTypeDef salarmstructure;
}rct_cfg_t;

typedef struct rct_data_s
{
	  RTC_DateTypeDef sdatestructureget;
	  RTC_TimeTypeDef stimestructureget;
	  const char * date;
	  const char * month;
}rct_data_t;

typedef struct rtc_s
{
	rct_cfg_t config;
	rct_data_t data;
}rtc_t;

extern void RTCM_Init(rtc_t *me, RTC_HandleTypeDef *hrtc, rct_cfg_t *cfg);
extern void RTCM_GetDateTime(rtc_t *me, RTC_HandleTypeDef *hrtc);
extern void RTCM_SetAlarm(rtc_t *me, RTC_HandleTypeDef *hrtc);
