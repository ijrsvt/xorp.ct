// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2005 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/rtrmgr/master_template_tree_node.cc,v 1.1 2004/12/11 21:29:57 mjh Exp $"


#include <glob.h>

#include "rtrmgr_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"

#include "command_tree.hh"
#include "conf_tree.hh"
#include "module_command.hh"
#include "template_commands.hh"
#include "template_tree.hh"
#include "master_template_tree_node.hh"
#include "util.hh"


void
MasterTemplateTreeNode::add_cmd(const string& cmd, TemplateTree& tt)
    throw (ParseError)
{
    BaseCommand* command;
    map<string, BaseCommand*>::iterator iter;

    if (cmd == "%modinfo") {
	iter = _cmd_map.find("%modinfo");
	if (iter == _cmd_map.end()) {
	    command = new ModuleCommand(tt, *this, cmd);
	    _cmd_map[cmd] = command;
	}
    } else if ((cmd == "%create")
	       || (cmd == "%activate")
	       || (cmd == "%update")
	       || (cmd == "%list")
	       || (cmd == "%delete")
	       || (cmd == "%set")
	       || (cmd == "%unset")
	       || (cmd == "%get")
	       || (cmd == "%default")) {
	// If the command already exists, no need to create it again.
	// The command action will simply be added to the existing command.
	if (_cmd_map.find(cmd) == _cmd_map.end()) {
	    command = new Command(*this, cmd);
	    _cmd_map[cmd] = command;
	}
    } else {
	TemplateTreeNode::add_cmd(cmd);
    }
}

void
MasterTemplateTreeNode::add_action(const string& cmd,
				   const list<string>& action_list,
				   const XRLdb& xrldb)
{
    BaseCommand* command;
    map<string, BaseCommand*>::iterator iter;

    if (cmd == "%modinfo") {
	iter = _cmd_map.find("%modinfo");
	XLOG_ASSERT(iter != _cmd_map.end());
	command = iter->second;
	ModuleCommand* module_command = dynamic_cast<ModuleCommand*>(command);
	XLOG_ASSERT(module_command != NULL);
	module_command->add_action(action_list, xrldb);
    } else if ((cmd == "%create")
	       || (cmd == "%activate")
	       || (cmd == "%update")
	       || (cmd == "%list")
	       || (cmd == "%delete")
	       || (cmd == "%set")
	       || (cmd == "%unset")
	       || (cmd == "%get")
	       || (cmd == "%default")) {
	iter = _cmd_map.find(cmd);
	XLOG_ASSERT(iter != _cmd_map.end());
	command = iter->second;
	Command* regular_command = dynamic_cast<Command*>(command);
	regular_command->add_action(action_list, xrldb);
    } else {
	TemplateTreeNode::add_action(cmd, action_list);
    }
}

bool
MasterTemplateTreeNode::check_template_tree(string& errmsg) const
{
    //
    // First check this node, then check recursively all children nodes
    //

    //
    // Check whether all referred variable names are valid
    //
    map<string, BaseCommand *>::const_iterator iter1;
    for (iter1 = _cmd_map.begin(); iter1 != _cmd_map.end(); ++iter1) {
	const Command* command;
	command = dynamic_cast<Command*>(iter1->second);
	if (command) {
	    if (! command->check_referred_variables(errmsg))
		return false;
	}
    }

    //
    // Recursively check all children nodes
    //
    list<TemplateTreeNode*>::const_iterator iter2;
    for (iter2 = _children.begin(); iter2 != _children.end(); ++iter2) {
	const MasterTemplateTreeNode* ttn = 
	    (const MasterTemplateTreeNode*)(*iter2);
	if (ttn->check_template_tree(errmsg) != true)
	    return false;
    }

    return true;
}

