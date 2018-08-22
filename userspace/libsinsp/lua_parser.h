#pragma once

#include "lua_parser_api.h"

typedef struct lua_State lua_State;

class lua_parser
{
public:
	lua_parser(lua_filter_factory *factory, lua_State *ls, const char *lua_global_binding);
	~lua_parser();
	lua_parser_filter* get_filter(bool reset_filter = false);

 private:

	void reset();
	lua_filter_factory* m_factory;

	lua_parser_filter* m_filter;

	boolop m_last_boolop;
	bool m_have_rel_expr;
	int32_t m_nest_level;

	lua_State* m_ls;

	friend class lua_parser_cbacks;
};

