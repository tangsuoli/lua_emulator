#include  <stdio.h>
#include  "lua.h"
#include  "lualib.h"

#include "mem.h"

static char add_func[] =
"function add(x,y)\n\
  return x + y\n\
end\n";


static char prog1[] =
"print(\"prog1 from lua\")\n";

static char prog2[] =
"error(\"prog2 error from lua\")\n";

static char loop[] =
"print(\"it's loop.lua\")\n\
print(\"it's loop2.lua\")\n\
print(\"it's loop3.lua\")\n\
i=0\n\
j=5\n\
print(\"j=\"..j)\n\
print(\"begin loop!\")\n\
repeat\n\
	print(\"i=\"..i)\n\
	print(\"j=\"..j)\n\
	i=i+1\n\
until i>10\n";

static char array[] =
"a = {}\n\
i=0\n\
while i<10 do\n\
	a[i] = i*i\n\
	i=i+1\n\
end\n\
r,v = next(a,nil)\n\
while r ~= nil do\n\
	print (\"array[\"..r..\"] = \"..v)\n\
	r,v = next(a,r)\n\
end\n";

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

static char sort2[] =
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
	n=n+1; x[n]=\"a\"\n\
	n=n+1; x[n]=\"waldemar\"\n\
	n=n+1; x[n]=\"luiz\"\n\
    n=n+1;	x[n]=\"sedrez\"\n\
	print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3])\n\
	quicksort(1,n-1)\n\
	print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3])\n\
	sort (x, n-1)\n\
	print(x[0]..\",\"..x[1]..\",\"..x[2]..\",\"..x[3])\n\
   end\n\
   \
   main()\n";

static char card[] =
"ans = {1, 8, 6, 5, 4}\n\
q_auido_files = {\"q1.fla\",\"q2.fla\",\"q3.fla\",\"q4.fla\",\"q5.fla\"}\n\
q_cnt = 5\n\
q_index = 1\n\
\
function c_common_msg_handler()\n\
  result = 1\n\
  return result\n\
end\n\
\
function c_get_input(type, time)\n\
  result = 1\n\
  return result\n\
end\n\
\
function c_play_sound(path)\n\
  print(\"play_sound:\", path)\n\
end\n\
\
function verify_answer(value, index)\n\
  if (value == ans[index])\n\
  then\n\
	  result = true\n\
  else\n\
	  result = false\n\
  end\n\
\
  if (result)\n\
  then\n\
	  print(\"answer correct\")\n\
	  c_play_sound(\"correct\")\n\
  else\n\
	  c_play_sound(\"wrong\")\n\
	  print(\"answer wrong, continue\")\n\
  end\n\
\
  return result\n\
end\n\
\
function main()\n\
  print(\"questions count:\", q_cnt)\n\
  c_play_sound(q_auido_files[q_index])\n\
  while q_index > 0\n\
  do\n\
	  c_common_msg_handler()\n\
\
	  if (q_index > q_cnt)\n\
	  then\n\
		  print(\"all finished\")\n\
		  c_play_sound(\"finshished\")\n\
	  end\n\
\
	  g_ret = c_get_input(0, 5)\n\
	  if (i_ret == -1)\n\
	  then\n\
		  print(\"input timeout\")\n\
		  c_play_sound(\"timeout\")\n\
	  elseif (g_ret == -2)\n\
	  then\n\
		  print(\"audio playing,do nothing\")\n\
	  else\n\
		  v_ret = verify_answer(g_ret, q_index)\n\
		  if (v_ret)\n\
		  then\n\
			  q_index = q_index+1\n\
			  print(\"play next question\")\n\
			  c_play_sound(q_auido_files[q_index])\n\
		  end\n\
	  end\n\
  end\n\
end\n\
\
main()\n";



int lua_test_main(char* file_name)
{
	printf("lua_test_main run \n");

	lua_dofile(file_name);
	//lua_dostring(card);

	printf("lua_test_main end \n");

	//printf("get_total_mem = %d\n", get_total_mem());

	return 0;
}

