/*
 * Copyright (c) 2001-2005 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/socket6_user_xif.cc,v 1.5 2004/12/16 01:56:52 pavlin Exp $"

#include "socket6_user_xif.hh"

bool
XrlSocket6UserV0p1Client::send_recv_event(
	const char*	the_tgt,
	const string&	sockid,
	const IPv6&	src_host,
	const uint32_t&	src_port,
	const vector<uint8_t>&	data,
	const RecvEventCB&	cb
)
{
    Xrl x(the_tgt, "socket6_user/0.1/recv_event");
    x.args().add("sockid", sockid);
    x.args().add("src_host", src_host);
    x.args().add("src_port", src_port);
    x.args().add("data", data);
    return _sender->send(x, callback(this, &XrlSocket6UserV0p1Client::unmarshall_recv_event, cb));
}


/* Unmarshall recv_event */
void
XrlSocket6UserV0p1Client::unmarshall_recv_event(
	const XrlError&	e,
	XrlArgs*	a,
	RecvEventCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlSocket6UserV0p1Client::send_connect_event(
	const char*	the_tgt,
	const string&	sockid,
	const IPv6&	src_host,
	const uint32_t&	src_port,
	const string&	new_sockid,
	const ConnectEventCB&	cb
)
{
    Xrl x(the_tgt, "socket6_user/0.1/connect_event");
    x.args().add("sockid", sockid);
    x.args().add("src_host", src_host);
    x.args().add("src_port", src_port);
    x.args().add("new_sockid", new_sockid);
    return _sender->send(x, callback(this, &XrlSocket6UserV0p1Client::unmarshall_connect_event, cb));
}


/* Unmarshall connect_event */
void
XrlSocket6UserV0p1Client::unmarshall_connect_event(
	const XrlError&	e,
	XrlArgs*	a,
	ConnectEventCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0);
	return;
    } else if (a && a->size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(1));
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    bool accept;
    try {
	a->get("accept", accept);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    cb->dispatch(e, &accept);
}

bool
XrlSocket6UserV0p1Client::send_error_event(
	const char*	the_tgt,
	const string&	sockid,
	const string&	error,
	const bool&	fatal,
	const ErrorEventCB&	cb
)
{
    Xrl x(the_tgt, "socket6_user/0.1/error_event");
    x.args().add("sockid", sockid);
    x.args().add("error", error);
    x.args().add("fatal", fatal);
    return _sender->send(x, callback(this, &XrlSocket6UserV0p1Client::unmarshall_error_event, cb));
}


/* Unmarshall error_event */
void
XrlSocket6UserV0p1Client::unmarshall_error_event(
	const XrlError&	e,
	XrlArgs*	a,
	ErrorEventCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlSocket6UserV0p1Client::send_close_event(
	const char*	the_tgt,
	const string&	sockid,
	const string&	reason,
	const CloseEventCB&	cb
)
{
    Xrl x(the_tgt, "socket6_user/0.1/close_event");
    x.args().add("sockid", sockid);
    x.args().add("reason", reason);
    return _sender->send(x, callback(this, &XrlSocket6UserV0p1Client::unmarshall_close_event, cb));
}


/* Unmarshall close_event */
void
XrlSocket6UserV0p1Client::unmarshall_close_event(
	const XrlError&	e,
	XrlArgs*	a,
	CloseEventCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}
