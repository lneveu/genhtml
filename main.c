#include <stdio.h>
#include <string.h>
#include <time.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


extern int notInLua;
extern int luaopen_genhtml(lua_State *L); 

int main() {
  notInLua = 1;
  luaopen_genhtml(NULL);
  return 0;
 
}
