// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2007 International Computer Science Institute
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

#ident "$XORP: xorp/libxorp/asyncio.cc,v 1.30 2006/10/12 01:24:50 pavlin Exp $"

#include "libxorp_module.h"

#include "libxorp/xorp.h"
#include "libxorp/debug.h"
#include "libxorp/xlog.h"
#include "libxorp/eventloop.hh"

#include <signal.h>

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#include "xorpfd.hh"
#include "asyncio.hh"

#ifdef HOST_OS_WINDOWS
#   define EDGE_TRIGGERED_READ_LATENCY		// IOT_READ may be delayed.
#   define EDGE_TRIGGERED_WRITES		// IOT_WRITE is edge triggered.
#   include "win_dispatcher.hh"
#   include "win_io.h"
#endif


// ----------------------------------------------------------------------------
// Utility

bool
is_pseudo_error(const char* name, XorpFd fd, int error_num)
{
    switch (error_num) {
#ifdef HOST_OS_WINDOWS
    case ERROR_IO_PENDING:
	XLOG_WARNING("%s (fd = %p) got ERROR_IO_PENDING, continuing.", name,
		     (void *)fd);
	return true;
    case WSAEINTR:
	XLOG_WARNING("%s (fd = %p) got WSAEINTR, continuing.", name,
		     (void *)fd);
	return true;
    case WSAEWOULDBLOCK:
	XLOG_WARNING("%s (fd = %p) got WSAEWOULDBLOCK, continuing.", name,
		     (void *)fd);
	return true;
    case WSAEINPROGRESS:
	XLOG_WARNING("%s (fd = %p) got WSAEINPROGRESS, continuing.", name,
		     (void *)fd);
	return true;
#else // ! HOST_OS_WINDOWS
    case EINTR:
	XLOG_WARNING("%s (fd = %d) got EINTR, continuing.", name,
		     XORP_INT_CAST(fd));
	return true;
    case EWOULDBLOCK:
	XLOG_WARNING("%s (fd = %d) got EWOULDBLOCK, continuing.", name,
		     XORP_INT_CAST(fd));
	return true;
#endif // ! HOST_OS_WINDOWS
    }
    return false;
}

// ----------------------------------------------------------------------------
AsyncFileOperator::~AsyncFileOperator()
{
}

// ----------------------------------------------------------------------------
// AsyncFileReader read method and entry hook

void
AsyncFileReader::add_buffer(uint8_t* b, size_t b_bytes, const Callback& cb)
{
    assert(b_bytes != 0);
    _buffers.push_back(BufferInfo(b, b_bytes, cb));
}

void
AsyncFileReader::add_buffer_with_offset(uint8_t*	b, 
					size_t		b_bytes, 
					size_t		off,
					const Callback&	cb) 
{
    assert(off < b_bytes);
    _buffers.push_back(BufferInfo(b, b_bytes, off, cb));
}

#ifdef HOST_OS_WINDOWS
void
AsyncFileReader::disconnect(XorpFd fd, IoEventType type)
{
    assert(type == IOT_DISCONNECT);
    assert(fd == _fd);
    assert(fd.is_valid()); 
    assert(fd.is_socket()); 
    debug_msg("IOT_DISCONNECT close detected (reader side)\n");
    BufferInfo& head = _buffers.front();
    head.dispatch_callback(END_OF_FILE);
}
#endif // HOST_OS_WINDOWS

void
AsyncFileReader::read(XorpFd fd, IoEventType type)
{
#ifdef EDGE_TRIGGERED_READ_LATENCY
    if (_running == false)
	return;
#endif
    assert(type == IOT_READ);
    assert(fd == _fd);
    assert(_buffers.empty() == false);

#if 0
    // XXX: comm_sock_is_connected() is cross-reference to libcomm
    if (_fd.is_socket() && (XORP_OK != comm_sock_is_connected(_fd))) {
	debug_msg("Warning: socket %p may have been closed\n", (HANDLE)_fd);
    }
#endif
    debug_msg("Buffer count %u\n", XORP_UINT_CAST(_buffers.size()));

    BufferInfo& head = _buffers.front();
    ssize_t done = 0;
#ifdef HOST_OS_WINDOWS
    BOOL result = FALSE;

    switch (fd.type()) {
    case XorpFd::FDTYPE_SOCKET:
	done = recv((SOCKET)_fd, (char *)(head._buffer + head._offset),
		    head._buffer_bytes - head._offset, 0);
	if (done == SOCKET_ERROR) {
	    _last_error = WSAGetLastError();
	    done = -1;
	} else if (done == 0) {
	    // Graceful close; make sure complete_transfer() gets this.
	    debug_msg("graceful close detected\n");
	    _last_error = WSAENOTCONN;
	    done = -1;
	}
	break;

    case XorpFd::FDTYPE_PIPE:
	// XXX: Return values need review.
	done = win_pipe_read(_fd, head._buffer + head._offset,
			     head._buffer_bytes - head._offset);
	_last_error = GetLastError();
	break;

    case XorpFd::FDTYPE_CONSOLE:
	// XXX: Return values need review.
	done = win_con_read(_fd, head._buffer + head._offset,
			     head._buffer_bytes - head._offset);
	_last_error = GetLastError();
	break;

    case XorpFd::FDTYPE_FILE:
	result = ReadFile(_fd, (LPVOID)(head._buffer + head._offset),
			       (DWORD)(head._buffer_bytes - head._offset),
			       (LPDWORD)&done, NULL);
	if (result == FALSE) {
	    _last_error = GetLastError();
	    SetLastError(ERROR_SUCCESS);
	}
	break;

    default:
	XLOG_FATAL("Invalid descriptor type.");
	break;
    }
#else // ! HOST_OS_WINDOWS
    errno = 0;
    _last_error = 0;
    done = ::read(_fd, head._buffer + head._offset,
		  head._buffer_bytes - head._offset);
    if (done < 0)
	_last_error = errno;
    errno = 0;
#endif // ! HOST_OS_WINDOWS

    debug_msg("Read %d bytes\n", XORP_INT_CAST(done));

    if (done < 0 && is_pseudo_error("AsyncFileReader", _fd, _last_error)) {
	return;
    }
    complete_transfer(_last_error, done);

#ifdef EDGE_TRIGGERED_READ_LATENCY
    //
    // If there's still data which we can read without blocking,
    // and we didn't fill our buffers, then try to read again
    // without waiting for an IOT_READ dispatch, as it may not come,
    // or be delayed due to latency between the primary thread
    // and the Winsock thread.
    //
    if (_fd.is_socket()) {
	u_long remaining = 0;
	int result = ioctlsocket(_fd, FIONREAD, &remaining);
	if (result != SOCKET_ERROR && remaining > 0) {
	    _deferred_io_task = _eventloop.new_oneoff_task(
		callback(this, &AsyncFileReader::read, _fd, IOT_READ));
	    XLOG_ASSERT(_deferred_io_task.scheduled());
	}
   }
#endif // EDGE_TRIGGERED_READ_LATENCY
}

// transfer_complete() invokes callbacks if necessary and updates buffer
// variables and buffer list.
void
AsyncFileReader::complete_transfer(int err, ssize_t done)
{
    // XXX careful after callback is invoked: "this" maybe deleted, so do
    // not reference any object state after callback.

    if (done > 0) {
	BufferInfo& head = _buffers.front();
	head._offset += done;
	if (head._offset == head._buffer_bytes) {
	    BufferInfo copy = head; 		// copy head
	    _buffers.erase(_buffers.begin());	// remove head
	    if (_buffers.empty()) {
		stop();
	    }
	    copy.dispatch_callback(DATA);
	} else {
	    head.dispatch_callback(DATA);
	}
	return;
    }

    BufferInfo& head = _buffers.front();
    if (err != 0 || done < 0) {
	stop();
	head.dispatch_callback(OS_ERROR);
    } else {
	head.dispatch_callback(END_OF_FILE);
    }
}

bool 
AsyncFileReader::start()
{
    if (_running)
	return true;

    if (_buffers.empty() == true) {
	XLOG_WARNING("Could not start reader - no buffers available");
	return false;
    }

#if 0
    // XXX: comm_sock_is_connected() is cross-reference to libcomm
    if (_fd.is_socket() && (XORP_OK != comm_sock_is_connected(_fd))) {
	debug_msg("Warning: socket %p may have been closed\n", (HANDLE)_fd);
    }
#endif // 0

    EventLoop& e = _eventloop;
    if (e.add_ioevent_cb(_fd, IOT_READ,
			 callback(this, &AsyncFileReader::read),
			 _priority) == false) {
	XLOG_ERROR("AsyncFileReader: Failed to add ioevent callback.");
	return false;
    }

#ifdef HOST_OS_WINDOWS
    // Windows notifies us of disconnections using a separate flag.
    // The file descriptor may no longer be valid when we stop, so
    // mark the IOT_DISCONNECT callback as being added using a boolean.
    _disconnect_added = false;
    if (_fd.is_socket()) {
	_disconnect_added = e.add_ioevent_cb(
	    _fd,
	    IOT_DISCONNECT,
	    callback(this, &AsyncFileReader::disconnect),
	    _priority);
	if (_disconnect_added == false) {
	    XLOG_ERROR("AsyncFileReader: Failed to add ioevent callback.");
	    _eventloop.remove_ioevent_cb(_fd, IOT_READ);
	    return false;
	}
    }
#endif // HOST_OS_WINDOWS

    debug_msg("%p start\n", this);

    _running = true;
    return _running;
}

void
AsyncFileReader::stop()
{
    debug_msg("%p stop\n", this);

    _eventloop.remove_ioevent_cb(_fd, IOT_READ);

#ifdef EDGE_TRIGGERED_WRITES
    _deferred_io_task.unschedule();
#endif
#ifdef HOST_OS_WINDOWS
    if (_disconnect_added == true) {
    	_eventloop.remove_ioevent_cb(_fd, IOT_DISCONNECT);
	_disconnect_added = false;
    }
#endif

    _running = false;
}

void
AsyncFileReader::flush_buffers()
{
    stop();
    while (_buffers.empty() == false) {
	// Copy out buffer so flush buffers can be called re-entrantly (even
	// if we happen to think this is bad coding style :-).
	BufferInfo head = _buffers.front();
	_buffers.erase(_buffers.begin());
	head.dispatch_callback(FLUSHING);
    }
}

// ----------------------------------------------------------------------------
// AsyncFileWriter write method and entry hook

#ifndef MAX_IOVEC
#define MAX_IOVEC 16
#endif

AsyncFileWriter::AsyncFileWriter(EventLoop& e, XorpFd fd, uint32_t coalesce,
				 int priority)
    : AsyncFileOperator(e, fd, priority)
{
    static const uint32_t max_coalesce = 16;
    _coalesce = (coalesce > MAX_IOVEC) ? MAX_IOVEC : coalesce;
    if (_coalesce > max_coalesce) {
	_coalesce = max_coalesce;
    }
    _iov = new iovec[_coalesce];
    _dtoken = new int;
}

AsyncFileWriter::~AsyncFileWriter()
{
    stop();

    delete[] _iov;
}

void
AsyncFileWriter::add_buffer(const uint8_t*	b,
			    size_t		b_bytes,
			    const Callback&	cb)
{
    assert(b_bytes != 0);
    _buffers.push_back(BufferInfo(b, b_bytes, cb));
#ifdef EDGE_TRIGGERED_WRITES
    if (_running == true) {
	_deferred_io_task = _eventloop.new_oneoff_task(
	    callback(this, &AsyncFileWriter::write, _fd, IOT_WRITE));
	XLOG_ASSERT(_deferred_io_task.scheduled());
    }
#endif // EDGE_TRIGGERED_WRITES
}

void
AsyncFileWriter::add_buffer_with_offset(const uint8_t*	b,
					size_t		b_bytes,
					size_t		off,
					const Callback&	cb)
{
    assert(off < b_bytes);
    _buffers.push_back(BufferInfo(b, b_bytes, off, cb));
#ifdef EDGE_TRIGGERED_WRITES
    if (_running == true) {
	_deferred_io_task = _eventloop.new_oneoff_task(
	    callback(this, &AsyncFileWriter::write, _fd, IOT_WRITE));
	XLOG_ASSERT(_deferred_io_task.scheduled());
    }
#endif // EDGE_TRIGGERED_WRITES
}

//
// Different UNIX platforms have different iov.iov_base types which
// we can fix at compile time.  The general idea of writev doesn't
// change much across UNIX platforms.
//
template <typename T, typename U>
static void
iov_place(T*& iov_base, U& iov_len, uint8_t* data, size_t data_len)
{
    static_assert(sizeof(T*) == sizeof(uint8_t*));
    iov_base = reinterpret_cast<T*>(data);
    iov_len  = data_len;
}

#ifdef HOST_OS_WINDOWS
void
AsyncFileWriter::disconnect(XorpFd fd, IoEventType type)
{
    assert(type == IOT_DISCONNECT);
    assert(fd == _fd);
    assert(fd.is_valid()); 
    assert(fd.is_socket()); 
    debug_msg("IOT_DISCONNECT close detected (writer side)\n");
#if 0
    BufferInfo& head = _buffers.front();
    head.dispatch_callback(END_OF_FILE);
#endif // 0
}
#endif // HOST_OS_WINDOWS

void
AsyncFileWriter::write(XorpFd fd, IoEventType type)
{
#ifdef EDGE_TRIGGERED_WRITES
    if (_running == false)
	return;
#endif
#if 0
    // XXX: comm_sock_is_connected() is cross-reference to libcomm
    if (_fd.is_socket() && (XORP_OK != comm_sock_is_connected(_fd))) {
	debug_msg("Warning: socket %p may have been closed\n", (HANDLE)_fd);
    }
#endif // 0

    assert(type == IOT_WRITE);
    assert(fd == _fd);
    assert(_buffers.empty() == false);

    // Coalesce buffers into a group
    uint32_t iov_cnt = 0;
    size_t   total_bytes = 0;
    ssize_t  done = 0;

    list<BufferInfo>::iterator i = _buffers.begin();

    while (i != _buffers.end()) {
	BufferInfo bi = *i;

	uint8_t* u = const_cast<uint8_t*>(bi._buffer + bi._offset);
	size_t   u_bytes = bi._buffer_bytes - bi._offset;
	iov_place(_iov[iov_cnt].iov_base, _iov[iov_cnt].iov_len, u, u_bytes);

	total_bytes += u_bytes;
	assert(total_bytes != 0);
	iov_cnt++;
	if (iov_cnt == _coalesce)
	    break;
	++i;
    }

#ifdef HOST_OS_WINDOWS
    if (fd.is_socket()) {
	// Socket handles take non-blocking writes.
	// WSASend() approximates writev().
	int result = WSASend((SOCKET)_fd, (LPWSABUF)_iov, iov_cnt,
			     (LPDWORD)&done, 0, NULL, NULL);
	_last_error = (result == SOCKET_ERROR) ? WSAGetLastError() : 0;
#if 1
	if (_last_error != 0)
	    debug_msg("writer: winsock error %d\n", _last_error);
#endif // 1
    } else {
	// Non-socket handles take blocking writes.
	// There is no writev() equivalent, so emulate it.
	BOOL result;
	DWORD done2;
	for (uint32_t j = 0; j < iov_cnt; j++) {
	    result = WriteFile(_fd, (LPVOID)_iov[j].iov_base,
			       (DWORD)_iov[j].iov_len, (LPDWORD)&done2, NULL);
	    done += done2;
	    if (result == FALSE)
		 break;
	}
	_last_error = (result == FALSE) ? GetLastError() : 0;
    }	
#else // ! HOST_OS_WINDOWS
    sig_t saved_sigpipe = signal(SIGPIPE, SIG_IGN);

    errno = 0;
    _last_error = 0;
    done = ::writev(_fd, _iov, (int)iov_cnt);
    if (done < 0)
	_last_error = errno;
    errno = 0;

    signal(SIGPIPE, saved_sigpipe);
#endif // ! HOST_OS_WINDOWS

    debug_msg("Wrote %d of %u bytes\n",
	      XORP_INT_CAST(done), XORP_UINT_CAST(total_bytes));

    if (done < 0 && is_pseudo_error("AsyncFileWriter", _fd, _last_error)) {
	XLOG_WARNING("Write error %d\n", _last_error);
	return;
    }

    complete_transfer(done);

#ifdef EDGE_TRIGGERED_WRITES
    if (_buffers.empty() == false) {
	_deferred_io_task = _eventloop.new_oneoff_task(
	    callback(this, &AsyncFileWriter::write, _fd, IOT_WRITE));
	XLOG_ASSERT(_deferred_io_task.scheduled());
    }
#endif // EDGE_TRIGGERED_WRITES
}

// transfer_complete() invokes callbacks if necessary and updates buffer
// variables and buffer list.
void
AsyncFileWriter::complete_transfer(ssize_t sdone)
{
    if (sdone < 0) {
	XLOG_ERROR("Write error %d\n", _last_error);
	stop();
	BufferInfo& head = _buffers.front();
	head.dispatch_callback(OS_ERROR);
	return;
    }

    size_t notified = 0;
    size_t done = (size_t)sdone;

    //
    // This is a trick to detect if the instance of the current object is
    // deleted mid-callback.  If so the method should not touch any part of
    // the instance state afterwards and should just return.  Okay, so how
    // to tell if the current AsyncFileWriter instance is deleted?
    //
    // The key observation is that _dtoken is a reference counted object
    // associated with the current instance.  Another reference is made to it
    // here bumping the reference count from 1 to 2.  If after invoking
    // a callback the instance count is no longer 2 then the AsyncFileWriter
    // instance was deleted in the callback.
    //
    ref_ptr<int> stack_token = _dtoken;

    while (notified != done) {
	assert(notified <= done);
	assert(_buffers.empty() == false);

	BufferInfo& head = _buffers.front();
	assert(head._buffer_bytes >= head._offset);

	size_t bytes_needed = head._buffer_bytes - head._offset;

	if (done - notified >= bytes_needed) {
	    //
	    // All data in this buffer has been written
	    //
	    head._offset += bytes_needed;
	    assert(head._offset == head._buffer_bytes);

	    // Copy, then detach head buffer and update state
	    BufferInfo copy = head;
	    _buffers.pop_front();
	    if (_buffers.empty()) {
		stop();
	    }

	    assert(stack_token.is_only() == false);

	    copy.dispatch_callback(DATA);
	    if (stack_token.is_only() == true) {
		// "this" instance of AsyncFileWriter was deleted by the
		// calback, return immediately.
		return;
	    }
	    notified += bytes_needed;
	    continue;
	} else {
	    //
	    // Not enough data has been written
	    //
	    head._offset += (done - notified);
	    assert(head._offset < head._buffer_bytes);

	    return;
	}
    }
}

bool
AsyncFileWriter::start()
{
    if (_running)
	return true;

    if (_buffers.empty() == true) {
	XLOG_WARNING("Could not start writer - no buffers available");
	return false;
    }

#if 0
    // XXX: comm_sock_is_connected() is cross-reference to libcomm
    if (_fd.is_socket() && (XORP_OK != comm_sock_is_connected(_fd))) {
	debug_msg("Warning: socket %p may have been closed\n", (HANDLE)_fd);
    }
#endif // 0

    EventLoop& e = _eventloop;
    if (e.add_ioevent_cb(_fd, IOT_WRITE,
			 callback(this, &AsyncFileWriter::write),
			 _priority) == false) {
	XLOG_ERROR("AsyncFileWriter: Failed to add I/O event callback.");
	return false;
    }
#ifdef HOST_OS_WINDOWS
    _disconnect_added = false;
    // Disable disconnection notification on write end for now.
    // Don't bitch if we can't add the notification.
    // it's a no-op to see if we pick them up on the write side...
    // XXX: If we fail to add a notification it's possible the socket
    // might already have been closed?
#if 0
    if (_fd.is_socket()) {
	_disconnect_added = e.add_ioevent_cb(
	    _fd,
	    IOT_DISCONNECT,
	    callback(this, &AsyncFileWriter::disconnect),
	    _priority);
#if 0
	if (_disconnect_added == false) {
	    XLOG_ERROR("AsyncFileWriter: Failed to add I/O event callback.");
	    _eventloop.remove_ioevent_cb(_fd, IOT_WRITE);
	    return false;
	}
#endif // 0
    }
#endif // 0
#endif // HOST_OS_WINDOWS

#ifdef EDGE_TRIGGERED_WRITES
    _deferred_io_task = _eventloop.new_oneoff_task(
	callback(this, &AsyncFileWriter::write, _fd, IOT_WRITE));
#endif // EDGE_TRIGGERED_WRITES

    _running = true;
    debug_msg("%p start\n", this);
    return _running;
}

void
AsyncFileWriter::stop()
{
    debug_msg("%p stop\n", this);

#ifdef EDGE_TRIGGERED_WRITES
    _deferred_io_task.unschedule();
#endif // EDGE_TRIGGERED_WRITES
    _eventloop.remove_ioevent_cb(_fd, IOT_WRITE);
#ifdef HOST_OS_WINDOWS
    if (_disconnect_added == true) {
    	_eventloop.remove_ioevent_cb(_fd, IOT_DISCONNECT);
	_disconnect_added = false;
    }
#endif // HOST_OS_WINDOWS
    _running = false;
}

void
AsyncFileWriter::flush_buffers()
{
    stop();
    while (_buffers.empty() == false) {
	// Copy out buffer so flush buffers can be called re-entrantly (even
	// if we happen to think this is bad coding style :-).
	BufferInfo head = _buffers.front();
	_buffers.erase(_buffers.begin());
	head.dispatch_callback(FLUSHING);
    }
}