#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <lstate.h>

#include <setjmp.h>

struct user_data_state
{
    jmp_buf panic_jmp;
    void (*logger)(const char* message);
};

static struct user_data_state user_data;

static const luaL_Reg loadedlibs[] =
{
    {LUA_GNAME, luaopen_base},
    {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {NULL, NULL}
};

void openlibs(lua_State *L)
{
    const luaL_Reg *lib;
    
    /* "require" functions from 'loadedlibs' and set results to global table */
    for (lib = loadedlibs; lib->func; lib++)
    {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);  /* remove lib */
    }
}

void* alloc(
    void *user_data,
    void *ptr,
    size_t old_size,
    size_t new_size)
{
    size_t *new_ptr = NULL;

    if (!ptr)
    {
        new_ptr = malloc(new_size + sizeof(size_t));
        if (new_ptr)
        {
            *new_ptr = new_size;
            ++new_ptr;
        }
    }
    else
    {
        // emulating realloc
        size_t *old_ptr = ptr;

        --old_ptr;

        if (new_size != 0)
        {
            if (new_size <= *old_ptr)
            {
                *old_ptr = new_size;
                new_ptr = ++old_ptr;
            }
            else
            {
                new_ptr = malloc(new_size + sizeof(size_t));
                if (new_ptr)
                {
                    *new_ptr = new_size;
                    memcpy(new_ptr + 1, old_ptr + 1, old_size);
                    free(old_ptr);
                    ++new_ptr;
                }
            }
        }
        else
        {
            free(old_ptr);
        }
    }

    return new_ptr;
}

static int panic(lua_State *L)
{
    struct user_data_state *ud = G(L)->ud;
    const char* message = lua_tostring(L, -1);

    printf("Lua panic: %s\n", message);

    longjmp(ud->panic_jmp, 1);
 
    return 0;
}

static void execute(const char* code, size_t code_length)
{
    int error = 0;
    lua_State* l = lua_newstate(alloc, &user_data);

    if (!l)
    {
        return;
    }

    lua_atpanic(l, &panic);
    
    openlibs(l);

    if (setjmp(user_data.panic_jmp) == 0)
    {
        if (luaL_loadbuffer(l,
                            code,
                            code_length,
                            NULL))
        {
            printf("Lua parser error: %s\n", lua_tostring(l, -1));
            return;
        }

        //lua_call(l, 0, LUA_MULTRET);
        error = lua_pcall(l, 0, LUA_MULTRET, 0);

        if (error)
        {
            printf("Lua runtime error: %s\n", lua_tostring(l, -1));
        }
        else if (lua_gettop(l) > 0)
        {
            lua_getglobal(l, "print");
            lua_insert(l, 1);
            lua_pcall(l, lua_gettop(l) - 1, 0, 0);
        }
    }
    else
    {
        puts("Lua panic");
    }
    
    lua_close(l);
}

int main()
{
    const char* code = "print(1/978)";

    execute(code, strlen(code));
}
