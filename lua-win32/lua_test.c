#include  <stdio.h>
#include  <stdlib.h>
#include  "lua.h"
#include  "lualib.h"
#include "string.h"
#include "stdbool.h"
//#include "play_test.h"

static char sort[] =
"function quicksort(r,s)\n\
	if s<=r then return end\n\
	local v, i, j = x[r], r, s+1\n\
		i=i+1; while x[i]<v do i=i+1 end\n\
		j=j-1; while x[j]>v do j=j-1 end\n\
		x[i],x[j]=x[j],x[i]\n\
	while j>i do\n\
		i=i+1; while x[i]<v do i=i+1 end\n\
		j=j-1; while x[j]>v do j=j-1 end\n\
		x[i],x[j]=x[j],x[i]\n\
	end\n\
	x[i],x[j]=x[j],x[i]\n\
	x[j],x[r]=x[r],x[j]\n\
	quicksort(r,j-1)\n\
	quicksort(j+1,s)\n\
end\n\
\
function sort(a,n)\n\
 local i=1\n\
 while i<=n do\n\
  local m, j = i, i+1\n\
  while j<=n do\n\
   if a[j]<a[m] then m=j end\n\
   j=j+1\n\
  end\n\
 a[i],a[m]=a[m],a[i]\n\
 i=i+1\n\
 end\n\
end\n\
\
function main()\n\
 x={}\n\
 n=-1\n\
 n=n+1;	x[n]=\"a\"\n\
 n=n+1;	x[n]=\"waldemar\"\n\
 n=n+1;	x[n]=\"luiz\"\n\
 n=n+1;	x[n]=\"lula\"\n\
 n=n+1;	x[n]=\"peter\"\n\
 n=n+1;	x[n]=\"raquel\"\n\
 n=n+1;	x[n]=\"camilo\"\n\
 n=n+1;	x[n]=\"andre\"\n\
 n=n+1;	x[n]=\"marcelo\"\n\
 n=n+1;	x[n]=\"sedrez\"\n\
 n=n+1;	x[n]=\"z\"\n\
 print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3]..\",\"..x[4]..\",\"..x[5]..\",\"..x[6]..\",\"..x[7]..\",\"..x[8]..\",\"..x[9]..\",\"..x[10])\n\
 quicksort(1,n-1)\n\
 print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3]..\",\"..x[4]..\",\"..x[5]..\",\"..x[6]..\",\"..x[7]..\",\"..x[8]..\",\"..x[9]..\",\"..x[10])\n\
 sort (x, n-1)\n\
 print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3]..\",\"..x[4]..\",\"..x[5]..\",\"..x[6]..\",\"..x[7]..\",\"..x[8]..\",\"..x[9]..\",\"..x[10])\n\
end\n\
\
main()\n";

#include <windows.h>

extern char g_selected_dir[MAX_PATH];
extern int g_key_value;

static bool playing = false;

void stop_play_sound() {
    StopMP3();
    playing = false;
    g_key_value = -1;
    printf("playing finished!!!!\n");
}

bool is_playing() {
    return playing;
}

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
    printf("c_get_input timeout time is: %d\n", t);

    DWORD start,end;
    start= GetTickCount();
    MSG msg;
    while (1){
        if (g_key_value != -1) {
            printf("c_get_input lua_pushnumber g_key_value[%d]\n", g_key_value);
            lua_pushnumber(g_key_value);
            return;
        }
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ) ){
            if (!GetMessage(&msg, NULL, 0, 0 )) {
                return;
            }
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        end= GetTickCount();
        if (end-start > t*1000) {
            printf("c_get_input time cost=%d\n",end-start);
            lua_pushnumber(-1);
            break;
        }
    }

	printf("c_get_input:end!!!\n");
}


void c_play_sound(void) {
    lua_Object id, name;

    id = lua_getparam(1);
    name = lua_getparam(2);

    if (id == LUA_NOOBJECT || name == LUA_NOOBJECT) {
        lua_error ("too few arguments to function `c_play_sound'");
    }

    if (!lua_isnumber(id) || !lua_isstring(name)) {
        lua_error ("incorrect argument to function `c_play_sound'");
    }

    int i = lua_getnumber(id);
    char* n = lua_getstring(name);
    char path[256];
    memset(path, 0x0,sizeof(path));
    sprintf(path, "%s\\%s.mp3", g_selected_dir, n);
    printf("c_play_sound: %s\n", path);

    playing = true;
    PlayMP3(path);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0 ) ){
        TranslateMessage( &msg );
        DispatchMessage( &msg );

        if (!playing)
            break;
    }

	printf("c_play_sound: %s end!!!\n", path);
}

int lua_test_main_file(char* file_name)
{
	printf("lua_test_main_file run \n");

    lua_register ("c_play_sound", c_play_sound);
    lua_register ("c_get_input", c_get_input);

	lua_dofile(file_name);

	printf("lua_test_main_file end \n");

	return 0;
}

int lua_test_main_string(char* string)
{
	printf("lua_test_main_string run \n");

    lua_register ("c_play_sound", c_play_sound);
    lua_register ("c_get_input", c_get_input);

	lua_dostring(string);

	printf("lua_test_main_string end \n");

	return 0;
}

