#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include  "lua.h"
#include  "lualib.h"

static const struct option long_options[] = {
		{"root_path",1,NULL,'r'},
		{"global_res_dir",1,NULL,'g'},
		{"output_file",1,NULL,'o'}
};

static char root_path[1024] = {0};
static char script_path[1024] = {0};
static char global_res_path[1024] = {0};

static char cur_dir[32] = {0};
static char global_res_dir[32] = {0};
static const char* default_global_res_dir = "gRes";

static char err_file_path[1024] = {0};

static int input_index = 1;


void c_get_input(void) {
    int i=1;   /* number of arguments */
    lua_Object o;
    /* the function must get at least one argument */
    if ((o = lua_getparam(i++)) == LUA_NOOBJECT)
        err_print ("[ERROR]too few arguments to function `c_get_input'");
    /* and this argument must be a number */
    if (!lua_isnumber(o))
        err_print ("[ERROR]incorrect argument to function `c_get_input'");

    lua_Object inputs = lua_getglobal("test_inputs");

    if ( lua_type(inputs) == LUA_T_NIL || lua_type(inputs) != LUA_T_ARRAY) {
        err_print ("[ERROR]Please add test_inputs for testing in card.lua script!\n");
    } else {
        if ((o = lua_getindexed(inputs, input_index++)) != LUA_NOOBJECT) {
            lua_pushnumber(lua_getnumber(o));
        }
    }
}

bool searchResourceFromDir(char *dir, char* res) {
	DIR		*dp;
	struct	dirent	*entry;
	struct	stat	statbuf;
	bool found = false;

	char path[1024];
	sprintf(path, "%s/%s", root_path, dir);

	if ((dp = opendir(path)) == NULL) {
		err_print("[ERROR] cannot open directory: %s\n", path);
		return false;
	}
	char resExt1[32] = {0};
	char resExt2[32] = {0};
	while ((entry = readdir(dp)) != NULL) {
		stat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {
            continue;
		} else {
		    sprintf(resExt1, "%s.mp3", res);
            sprintf(resExt2, "%s.f1a", res);
			if (!strcmp(entry->d_name, resExt1) || !strcmp(entry->d_name, resExt2)) {
                found = true;
                break;
			}
		}
	}
	closedir(dp);
	return found;
}

void searchResource(char *res) {
    if (!searchResourceFromDir(cur_dir, res) &&
        !searchResourceFromDir(global_res_dir, res)) {
        err_print ("[ERROR]Missing resource: [%s.f1a/mp3].\n", res);
    }
}

void c_play_sound(void) {
    lua_Object id, name;

    id = lua_getparam(1);
    name = lua_getparam(2);

    if (id == LUA_NOOBJECT || name == LUA_NOOBJECT) {
        err_print ("[ERROR]too few arguments to function `c_play_sound'");
    }

    if (!lua_isnumber(id) || !lua_isstring(name)) {
        err_print ("[ERROR]incorrect argument to function `c_play_sound'");
    }

    int i = lua_getnumber(id);
    char* s = lua_getstring(name);

    if (i != atoi(cur_dir)) {
        err_print ("[ERROR]c_play_sound: card_id [%d] is not equal to cur_dir [%s]!\n", i, cur_dir);
        return;
    }

    searchResource(s);
}


void verifyRun(char* path) {
    err_print("verifyRun path = %s\n", path);
    input_index = 1;
    lua_dofile(path);
}


bool scanDir(char *dir, int depth)
{
	DIR		*dp;
	struct	dirent	*entry;
	struct	stat	statbuf;
	bool found = false;

	if ((dp = opendir(dir)) == NULL) {
		err_print("cannot open directory: %s\n", dir);
		return false;
	}
	chdir(dir);				//system call
	while ((entry = readdir(dp)) != NULL) {
		stat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {
			if ( strcmp(".", entry->d_name) == 0 ||
				 strcmp("..", entry->d_name) == 0 ) {
				continue;
			}

			err_print("---------------------------------------------------------------\n");
			err_print("Checking dir [%s] begin.\n",entry->d_name);

			if (scanDir(entry->d_name,depth + 1) == false) {
                err_print("[ERROR] No found \"card.lua\" file in dir [%s]!!!\n", entry->d_name);
                err_print("Checking dir [%s] end.\n",entry->d_name);
                err_print("---------------------------------------------------------------\n\n");
			}
		} else {
			if (!strcmp(entry->d_name, "card.lua") && depth == 1) {
                err_print("[OK] Found \"card.lua\" file.\n");
                sprintf(script_path, "%s/%s/%s", root_path, dir, entry->d_name);
                sprintf(cur_dir, "%s", dir);
                verifyRun(script_path);

                err_print("Checking dir [%s] end.\n",dir);
                err_print("---------------------------------------------------------------\n\n");
                found = true;
                break;
			}
		}
	}
	chdir("..");
	closedir(dp);
	return found;
}


void handleParameter(int argc,char *argv[]) {
	int opt;
	opterr =0;
	while ((opt = getopt_long(argc,argv,"r:g:o:",long_options,NULL)) != EOF) {
   	 	switch (opt) {
			case 'r':
			    strcpy(root_path, optarg);
				break;
			case 'g':
                strcpy(global_res_dir, optarg);
				break;
			case 'o':
			    strcpy(err_file_path, optarg);
				break;
			default:
				printf("Other option is wrong\n");
				return 0;
		}
	}
}

bool init() {
  if (!strcmp(root_path, "")) {
        printf("Please using -r or --root_path set the root dir!\n");
        return false;
    } else {
        if (!strcmp(global_res_dir, "")) {
            strcpy(global_res_dir, default_global_res_dir);
        }

        err_print_init(err_file_path);

        lua_register ("c_play_sound", c_play_sound);
        lua_register ("c_get_input", c_get_input);

        return true;
	}
}

void deinit() {
    if (fp_err != NULL){
        fclose(fp_err);
    }
}

int main(int argc,char *argv[]){
    handleParameter(argc, argv);

    if (init()) {
        scanDir(root_path, 0);
        deinit();
        err_print("Done.\n");
    }

    return 0;
}
