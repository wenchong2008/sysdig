/*
Copyright (C) 2013-2014 Draios inc.

This file is part of sysdig.

sysdig is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

sysdig is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sysdig.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

typedef struct lua_State lua_State;

/*
 * Operators to compare events
 */
enum cmpop {
	CO_NONE = 0,
	CO_EQ = 1,
	CO_NE = 2,
	CO_LT = 3,
	CO_LE = 4,
	CO_GT = 5,
	CO_GE = 6,
	CO_CONTAINS = 7,
	CO_IN = 8,
	CO_EXISTS = 9,
	CO_ICONTAINS = 10,
	CO_STARTSWITH = 11,
	CO_GLOB = 12,
	CO_PMATCH = 13
};

enum boolop
{
	BO_NONE = 0,
	BO_NOT = 1,
	BO_OR = 2,
	BO_AND = 4,

	// obtained by bitwise OR'ing with one of above ops
	BO_ORNOT = 3,
	BO_ANDNOT = 5,
};

class lua_parser_filtercheck
{
public:

	lua_parser_filtercheck() {};
	virtual ~lua_parser_filtercheck() {};

	boolop m_boolop;
	cmpop m_cmpop;

	virtual int32_t parse_field_name(const char* str, bool alloc_state, bool needed_for_filtering) = 0;

	virtual void add_filter_value(const char* str, uint32_t len, uint32_t i = 0 ) = 0;

	virtual void set_check_id(int32_t id) = 0;
};

class lua_parser_filter
{
public:
	lua_parser_filter() {};
	virtual ~lua_parser_filter() {};

	virtual void push_expression(boolop op) = 0;
	virtual void pop_expression() = 0;
	virtual void add_check(lua_parser_filtercheck* chk) = 0;
};

class lua_filter_factory
{
public:

	lua_filter_factory() {};
	virtual ~lua_filter_factory() {};

	// Create a new filter
	virtual lua_parser_filter *new_filter() = 0;

	// Create a new filterchekc
	virtual lua_parser_filtercheck *new_filtercheck(const char *fldname) = 0;
};

class lua_parser_cbacks
{
public:
	// filter.rel_expr(field_name, cmpop, value, index)
	// field_name and cmpop are mandatory
	// value is mandatory unless cmpop=="exists"
	// index is an optional index (integer) that will be written
	// into events matching this expression (internal use).
	static int rel_expr(lua_State *ls);

	// filter.bool_op(op)
	static int bool_op(lua_State *ls);

	// filter.nest()
	static int nest(lua_State *ls);

	// filter.unnest()
	static int unnest(lua_State *ls);
};

