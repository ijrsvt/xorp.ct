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

// $XORP: xorp/fea/pa_backend.hh,v 1.2 2004/12/17 04:45:21 bms Exp $

#ifndef __FEA_PA_BACKEND_HH__
#define __FEA_PA_BACKEND_HH__

#include "pa_entry.hh"
#include "pa_table.hh"

/* ------------------------------------------------------------------------- */

// Exceptions which can be thrown by PaBackend and member classes.
class PaInvalidBackendException {};

/**
 * @short Packet filter provider interface.
 *
 * Abstract class defining the interface to a packet filtering provider.
 */
class PaBackend {
    friend class Snapshot4;
public:
    PaBackend() throw(PaInvalidBackendException)
        { throw PaInvalidBackendException(); }
    virtual ~PaBackend() {};

public:
    /* --------------------------------------------------------------------- */
    /*
     * @short State snapshot Memento classes.
     *
     * These are provider-specific and abstract. Attempting to instantiate
     * them directly will result in an exception being thrown.
     *
     * Be warned that they might not actually copy all the state in a form
     * which can be marshaled elsewhere. Each provider must implement
     * both of these classes and override the virtuals, and check that
     * snapshots passed to it are its own by using dynamic casts.
     */
    class Snapshot4 {
    public:
	Snapshot4()
	    throw(PaInvalidSnapshotException)
	    { throw PaInvalidSnapshotException(); }
	Snapshot4(const Snapshot4&)
	    throw(PaInvalidSnapshotException)
	    { throw PaInvalidSnapshotException(); }
	virtual ~Snapshot4() {}
    };

#ifdef notyet
    class Snapshot6 {
    public:
	virtual ~Snapshot6() {}
    protected:
	Snapshot6()
	    throw(PaInvalidSnapshotException)
	    { throw PaInvalidSnapshotException(); }
	Snapshot6(const Snapshot6&)
	    throw(PaInvalidSnapshotException)
	    { throw PaInvalidSnapshotException(); }
    };
#endif

public:
    /* --------------------------------------------------------------------- */
    /* General back-end methods. */

    virtual const char* get_name() const = 0;
    virtual const char* get_version() const = 0;

    /* --------------------------------------------------------------------- */
    /* IPv4 ACL back-end methods. */

    virtual bool push_entries4(const PaSnapshot4* snap) = 0;
    virtual bool delete_all_entries4() = 0;
    virtual const Snapshot4* create_snapshot4() = 0;
    virtual bool restore_snapshot4(const Snapshot4* snap) = 0;

#ifdef notyet
    /* --------------------------------------------------------------------- */
    /* IPv6 ACL back-end methods. */

    virtual bool push_entries6(const PaSnapshot6* snap) = 0;
    virtual bool delete_all_entries6() = 0;
    virtual const Snapshot6* create_snapshot6() = 0;
    virtual bool restore_snapshot6(const Snapshot6* snap) = 0;
#endif
};

/* ------------------------------------------------------------------------- */

#endif // __FEA_PA_BACKEND_HH__
