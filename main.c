#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include  "lua.h"
#include  "lualib.h"

extern int lua_test_main(char* file_name);

#include <stdlib.h>
#include <stdarg.h>

#define EOS_MATCHER_CHAR	'\f'

static int n_isspace(char c)
{
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}


int pp_sscanf(const char *str, const char *fmt, ...)
{
  int cSuccess = 0;
  const char *rp = str;
  const char *fp = fmt;
  va_list ap;
  char *ep;
  char fc;
  long v;

  va_start(ap, fmt);


  while(*rp && *fp){
    fc = *fp;
#ifdef EOS_MATCHER_CHAR
    if(fc == EOS_MATCHER_CHAR)
      break;
#endif
    if(n_isspace(fc)){
      /* do nothing */
    } else if(fc != '%'){
      while(n_isspace(*rp)) rp++;
      if(*rp == 0)
        break;
      else if(fc != *rp)
        break;
      else
        rp++;
    } else {  /* fc == '%' */
      fc = *++fp;
      if(fc == 'd' || fc == 'x'){
        int *ip = va_arg(ap, int *);
        v = strtol(rp, &ep, fc == 'd' ? 10 : 16);
        if(rp == ep) break;
        rp = ep;
        *ip = v;
        cSuccess++;
      } else if(fc == 'f' || fc == 'g' || fc == 'e'){
        double fv = strtod(rp, NULL);
        if(ep == rp)
          break;
        float *vp = va_arg(ap, float *);
        *vp = fv;
        cSuccess++;
        break;
      }
    }
    fp++;
  }

#ifdef EOS_MATCHER_CHAR
  while(n_isspace(*rp)) rp++;
  if(*rp == 0 && *fp == EOS_MATCHER_CHAR)
    cSuccess++;
#endif
  va_end(ap);
  return cSuccess;
}

double pp_atof(const char* str) {
	double d = 10.0;
	double s = 0.0;
	int flag = 0;
	while ((*str > '9' || *str < '.') && *str != '-') {
		str++;
	}

	if (*str == '-'){
		flag = 1;
		str++;
	}
	if (!(*str>= '0' && *str <='9')) {   //如果一开始并非数字，那么直接退出
	    printf("pp_atof is not a number!");
		return s;
	}
	while (*str >= '0' && *str <= '9' && *str != '.') {  //计算小数点前几位
		s = s * 10 + (*str - '0');
		str++;
	}
	if (*str == '.')  //之后为小数部分
		str++;
	while(*str >='0' && *str <= '9'){
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}
	return s * ((flag==1) ? -1.0 : 1.0);
}



static char *reverse(char *s){
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;

    //交换移动指针，直到p和q交叉
    while(q > p) {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

static char *my_itoa(int n){
    int i = 0,isNegative = 0;
    static char s[100];         //必须为static变量，或者是全局变量
    if((isNegative = n) < 0){ //如果是负数，先转为正数

        n = -n;
    }
    do        //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);

    if(isNegative < 0){    //如果是负数，补上负号
        s[i++] = '-';
    }
    s[i] = '\0';    //最后加上字符串结束符
    return reverse(s);
}


void math_max (void)
{
 int i=1;   /* number of arguments */
 double d, dmax;
 lua_Object o;
 /* the function must get at least one argument */
 if ((o = lua_getparam(i++)) == LUA_NOOBJECT)
   lua_error ("too few arguments to function `max'");
 /* and this argument must be a number */
 if (!lua_isnumber(o))
   lua_error ("incorrect argument to function `max'");
 dmax = lua_getnumber (o);
 /* loops until there is no more arguments */
 while ((o = lua_getparam(i++)) != LUA_NOOBJECT)
 {
  if (!lua_isnumber(o))
    lua_error ("incorrect argument to function `max'");
  d = lua_getnumber (o);
  if (d > dmax) dmax = d;
 }
 /* push the result to be returned */
 lua_pushnumber (dmax);
}
#if 1
int out = 0;
void c_common_msg_handler() {
	int result = 1;
    srand((unsigned)time(NULL));
    out = 1;//12-rand()%25;

	lua_pushnumber(result);
}

static clock_t last_valid_input_time_start = 0;
void c_get_input() {
    int i=1;   /* number of arguments */
    lua_Object o;
    /* the function must get at least one argument */
    if ((o = lua_getparam(i++)) == LUA_NOOBJECT)
        lua_error ("too few arguments to function `c_get_input'");
    /* and this argument must be a number */
    if (!lua_isnumber(o))
        lua_error ("incorrect argument to function `c_get_input'");

    int t;
    t = lua_getnumber(o); // timeout time
 //   printf("c_get_input timeout: %d\n", t);

 #if 0
    clock_t last_valid_input_time_end;
    if (last_valid_input_time_start == 0) {
        last_valid_input_time_start = clock();    }

//    printf("c_get_input is keyputing or playing out = %d!\n", out);
    if (out > 0 || out == -2) { // input key or playing
        last_valid_input_time_start = 0;
        printf("c_get_input is keyputing or playing out = %d!\n", out);
        lua_pushnumber(out);
    } else{ // (out <= 0) no any input
        last_valid_input_time_end = clock();
        // printf("c_get_input start: %d, end: %d\n", last_valid_input_time_start, last_valid_input_time_end);
        if ((double)(last_valid_input_time_end-last_valid_input_time_start) > t*1000) {
            last_valid_input_time_start = 0;
            printf("-------------c_get_input is timeout!\n");
            lua_pushnumber(-1);
        }
    }
#else
    lua_pushnumber(1);
#endif
}

void c_play_sound(void) {
    int i=1;   /* number of arguments */
    lua_Object o;
    /* the function must get at least one argument */
    if ((o = lua_getparam(i++)) == LUA_NOOBJECT)
        lua_error ("too few arguments to function `c_play_sound'");
    /* and this argument must be a number */
    if (!lua_isstring(o))
        lua_error ("incorrect argument to function `c_play_sound'");
    char* path = lua_getstring(o);
    printf("play_sound: %s\n", path);
}
#endif

int main()
{
    printf("Hello world!\n");

    float t;
    char c;
    t = pp_atof("-10.34 c");
  //  pp_sscanf("11.3 d", "%f %c", &t, &c);
    printf("%f\n",t);

    int i = 1;
    char* n = "1.f1a";
    char path[32];
    strcat(path,"/");
    strcat(path,my_itoa(i));
    strcat(path,"/");
    strcat(path,n);

    printf("c_play_sound: %s\n", path);

    lua_register ("c_common_msg_handler", c_common_msg_handler);
    lua_register ("c_play_sound", c_play_sound);
    lua_register ("c_get_input", c_get_input);


    //lua_test_main("d:\\work\\lua_project\\test\\card.lua");
    return 0;
}
