/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 *
 * $XORP: xorp/xrl/targets/mld6igmp_base.hh,v 1.18 2003/11/18 01:47:40 hodson Exp $
 */


#ifndef __XRL_INTERFACES_MLD6IGMP_BASE_HH__
#define __XRL_INTERFACES_MLD6IGMP_BASE_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XrlMld6igmpTarget"

#include "libxorp/xlog.h"
#include "libxipc/xrl_cmd_map.hh"

class XrlMld6igmpTargetBase {
protected:
    XrlCmdMap* _cmds;

public:
    /**
     * Constructor.
     *
     * @param cmds an XrlCmdMap that the commands associated with the target
     *		   should be added to.  This is typically the XrlRouter
     *		   associated with the target.
     */
    XrlMld6igmpTargetBase(XrlCmdMap* cmds = 0);

    /**
     * Destructor.
     *
     * Dissociates instance commands from command map.
     */
    virtual ~XrlMld6igmpTargetBase();

    /**
     * Set command map.
     *
     * @param cmds pointer to command map to associate commands with.  This
     * argument is typically a pointer to the XrlRouter associated with the
     * target.
     *
     * @return true on success, false if cmds is null or a command map has
     * already been supplied.
     */
    bool set_command_map(XrlCmdMap* cmds);

    /**
     * Get Xrl instance name associated with command map.
     */
    inline const string& name() const { return _cmds->name(); }

    /**
     * Get version string of instance.
     */
    inline const char* version() const { return "mld6igmp/0.0"; }

protected:

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get name of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_target_name(
	// Output values,
	string&	name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get version string from Xrl Target
     */
    virtual XrlCmdError common_0_1_get_version(
	// Output values,
	string&	version) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get status of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_status(
	// Output values,
	uint32_t&	status,
	string&	reason) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Request clean shutdown of Xrl Target
     */
    virtual XrlCmdError common_0_1_shutdown() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Process a CLI command.
     *
     *  @param processor_name the processor name for this command.
     *
     *  @param cli_term_name the terminal name the command was entered from.
     *
     *  @param cli_session_id the CLI session ID the command was entered from.
     *
     *  @param command_name the command name to process.
     *
     *  @param command_args the command arguments to process.
     *
     *  @param ret_processor_name the processor name to return back to the CLI.
     *
     *  @param ret_cli_term_name the terminal name to return back.
     *
     *  @param ret_cli_session_id the CLI session ID to return back.
     *
     *  @param ret_command_output the command output to return back.
     */
    virtual XrlCmdError cli_processor_0_1_process_command(
	// Input values,
	const string&	processor_name,
	const string&	cli_term_name,
	const uint32_t&	cli_session_id,
	const string&	command_name,
	const string&	command_args,
	// Output values,
	string&	ret_processor_name,
	string&	ret_cli_term_name,
	uint32_t&	ret_cli_session_id,
	string&	ret_command_output) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Add a new vif.
     *
     *  @param vif_name the name of the new vif.
     *
     *  @param vif_index the index of the new vif.
     */
    virtual XrlCmdError mfea_client_0_1_new_vif(
	// Input values,
	const string&	vif_name,
	const uint32_t&	vif_index) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Delete an existing vif.
     *
     *  @param vif_name the name of the vif to delete.
     */
    virtual XrlCmdError mfea_client_0_1_delete_vif(
	// Input values,
	const string&	vif_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Add an address to a vif.
     *
     *  @param vif_name the name of the vif.
     *
     *  @param addr the unicast address to add.
     *
     *  @param subnet the subnet address to add.
     *
     *  @param broadcast the broadcast address (when applicable).
     *
     *  @param peer the peer address (when applicable).
     */
    virtual XrlCmdError mfea_client_0_1_add_vif_addr4(
	// Input values,
	const string&	vif_name,
	const IPv4&	addr,
	const IPv4Net&	subnet,
	const IPv4&	broadcast,
	const IPv4&	peer) = 0;

    virtual XrlCmdError mfea_client_0_1_add_vif_addr6(
	// Input values,
	const string&	vif_name,
	const IPv6&	addr,
	const IPv6Net&	subnet,
	const IPv6&	broadcast,
	const IPv6&	peer) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Delete an address from a vif.
     *
     *  @param vif_name the name of the vif.
     *
     *  @param addr the unicast address to delete.
     */
    virtual XrlCmdError mfea_client_0_1_delete_vif_addr4(
	// Input values,
	const string&	vif_name,
	const IPv4&	addr) = 0;

    virtual XrlCmdError mfea_client_0_1_delete_vif_addr6(
	// Input values,
	const string&	vif_name,
	const IPv6&	addr) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set flags to a vif.
     *
     *  @param vif_name the name of the vif.
     *
     *  @param is_pim_register true if this is a PIM Register vif.
     *
     *  @param is_p2p true if this is a point-to-point vif.
     *
     *  @param is_loopback true if this is a loopback interface.
     *
     *  @param is_multicast true if the vif is multicast-capable.
     *
     *  @param is_broadcast true if the vif is broadcast-capable.
     *
     *  @param is_up true if the vif is UP and running.
     */
    virtual XrlCmdError mfea_client_0_1_set_vif_flags(
	// Input values,
	const string&	vif_name,
	const bool&	is_pim_register,
	const bool&	is_p2p,
	const bool&	is_loopback,
	const bool&	is_multicast,
	const bool&	is_broadcast,
	const bool&	is_up) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Complete all transactions with vif information.
     */
    virtual XrlCmdError mfea_client_0_1_set_all_vifs_done() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Test if the vif setup is completed.
     *
     *  @param is_completed if true the vif setup is completed.
     */
    virtual XrlCmdError mfea_client_0_1_is_vif_setup_completed(
	// Output values,
	bool&	is_completed) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Receive a protocol message from the MFEA.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param protocol_name the name of the protocol that sends a message.
     *
     *  @param protocol_id the ID of the protocol that sends a message (both
     *  sides must agree on the particular values).
     *
     *  @param vif_name the name of the vif the message was received on.
     *
     *  @param vif_index the index of the vif the message was received on.
     *
     *  @param source_address the address of the sender.
     *
     *  @param dest_address the destination address.
     *
     *  @param ip_ttl the TTL of the received IP packet. If it has a negative
     *  value, it should be ignored.
     *
     *  @param ip_tos the TOS of the received IP packet. If it has a negative
     *  value, it should be ignored.
     *
     *  @param is_router_alert if true, the IP Router Alert option in the IP
     *  packet was set (when applicable).
     *
     *  @param protocol_message the protocol message.
     */
    virtual XrlCmdError mfea_client_0_1_recv_protocol_message4(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index,
	const IPv4&	source_address,
	const IPv4&	dest_address,
	const int32_t&	ip_ttl,
	const int32_t&	ip_tos,
	const bool&	is_router_alert,
	const vector<uint8_t>&	protocol_message) = 0;

    virtual XrlCmdError mfea_client_0_1_recv_protocol_message6(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index,
	const IPv6&	source_address,
	const IPv6&	dest_address,
	const int32_t&	ip_ttl,
	const int32_t&	ip_tos,
	const bool&	is_router_alert,
	const vector<uint8_t>&	protocol_message) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Receive a kernel signal message from the MFEA.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param protocol_name the name of the protocol that sends a message.
     *
     *  @param protocol_id the ID of the protocol that sends a message (both
     *  sides must agree on the particular values).
     *
     *  @param message_type the type of the kernel signal message (TODO:
     *  integer for now: the particular types are well-known by both sides).
     *
     *  @param vif_name the name of the vif the message was received on.
     *
     *  @param vif_index the index of the vif the message was received on.
     *
     *  @param source_address the address of the sender.
     *
     *  @param dest_address the destination address.
     *
     *  @param protocol_message the protocol message.
     */
    virtual XrlCmdError mfea_client_0_1_recv_kernel_signal_message4(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const uint32_t&	message_type,
	const string&	vif_name,
	const uint32_t&	vif_index,
	const IPv4&	source_address,
	const IPv4&	dest_address,
	const vector<uint8_t>&	protocol_message) = 0;

    virtual XrlCmdError mfea_client_0_1_recv_kernel_signal_message6(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const uint32_t&	message_type,
	const string&	vif_name,
	const uint32_t&	vif_index,
	const IPv6&	source_address,
	const IPv6&	dest_address,
	const vector<uint8_t>&	protocol_message) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Add Multicast Routing Information Base information.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param dest_prefix the destination prefix to add.
     *
     *  @param next_hop_router_addr the address of the next-hop router toward
     *  the destination prefix.
     *
     *  @param next_hop_vif_name the name of the vif toward the destination
     *  prefix.
     *
     *  @param next_hop_vif_index the index of the vif toward the destination
     *  prefix.
     *
     *  @param metric_preference the metric preference for this entry.
     *
     *  @param metric the metric for this entry.
     */
    virtual XrlCmdError mfea_client_0_1_add_mrib4(
	// Input values,
	const string&	xrl_sender_name,
	const IPv4Net&	dest_prefix,
	const IPv4&	next_hop_router_addr,
	const string&	next_hop_vif_name,
	const uint32_t&	next_hop_vif_index,
	const uint32_t&	metric_preference,
	const uint32_t&	metric) = 0;

    virtual XrlCmdError mfea_client_0_1_add_mrib6(
	// Input values,
	const string&	xrl_sender_name,
	const IPv6Net&	dest_prefix,
	const IPv6&	next_hop_router_addr,
	const string&	next_hop_vif_name,
	const uint32_t&	next_hop_vif_index,
	const uint32_t&	metric_preference,
	const uint32_t&	metric) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Delete Multicast Routing Information Base information.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param dest_prefix the destination prefix to delete.
     */
    virtual XrlCmdError mfea_client_0_1_delete_mrib4(
	// Input values,
	const string&	xrl_sender_name,
	const IPv4Net&	dest_prefix) = 0;

    virtual XrlCmdError mfea_client_0_1_delete_mrib6(
	// Input values,
	const string&	xrl_sender_name,
	const IPv6Net&	dest_prefix) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Complete a transaction with MRIB information.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     */
    virtual XrlCmdError mfea_client_0_1_set_mrib_done(
	// Input values,
	const string&	xrl_sender_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  A signal that a dataflow-related pre-condition is true.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param source_address the source address of the dataflow.
     *
     *  @param group_address the group address of the dataflow.
     *
     *  @param threshold_interval_sec the number of seconds in the interval
     *  requested for measurement.
     *
     *  @param threshold_interval_usec the number of microseconds in the
     *  interval requested for measurement.
     *
     *  @param measured_interval_sec the number of seconds in the last measured
     *  interval that has triggered the signal.
     *
     *  @param measured_interval_usec the number of microseconds in the last
     *  measured interval that has triggered the signal.
     *
     *  @param threshold_packets the threshold value to trigger a signal (in
     *  number of packets).
     *
     *  @param threshold_bytes the threshold value to trigger a signal (in
     *  bytes).
     *
     *  @param measured_packets the number of packets measured within the
     *  measured interval.
     *
     *  @param measured_bytes the number of bytes measured within the measured
     *  interval.
     *
     *  @param is_threshold_in_packets if true, threshold_packets is valid.
     *
     *  @param is_threshold_in_bytes if true, threshold_bytes is valid.
     *
     *  @param is_geq_upcall if true, the operation for comparison is ">=".
     *
     *  @param is_leq_upcall if true, the operation for comparison is "<=".
     */
    virtual XrlCmdError mfea_client_0_1_recv_dataflow_signal4(
	// Input values,
	const string&	xrl_sender_name,
	const IPv4&	source_address,
	const IPv4&	group_address,
	const uint32_t&	threshold_interval_sec,
	const uint32_t&	threshold_interval_usec,
	const uint32_t&	measured_interval_sec,
	const uint32_t&	measured_interval_usec,
	const uint32_t&	threshold_packets,
	const uint32_t&	threshold_bytes,
	const uint32_t&	measured_packets,
	const uint32_t&	measured_bytes,
	const bool&	is_threshold_in_packets,
	const bool&	is_threshold_in_bytes,
	const bool&	is_geq_upcall,
	const bool&	is_leq_upcall) = 0;

    virtual XrlCmdError mfea_client_0_1_recv_dataflow_signal6(
	// Input values,
	const string&	xrl_sender_name,
	const IPv6&	source_address,
	const IPv6&	group_address,
	const uint32_t&	threshold_interval_sec,
	const uint32_t&	threshold_interval_usec,
	const uint32_t&	measured_interval_sec,
	const uint32_t&	measured_interval_usec,
	const uint32_t&	threshold_packets,
	const uint32_t&	threshold_bytes,
	const uint32_t&	measured_packets,
	const uint32_t&	measured_bytes,
	const bool&	is_threshold_in_packets,
	const bool&	is_threshold_in_bytes,
	const bool&	is_geq_upcall,
	const bool&	is_leq_upcall) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Enable/disable/start/stop a MLD6IGMP vif interface.
     *
     *  @param vif_name the name of the vif to enable/disable/start/stop.
     */
    virtual XrlCmdError mld6igmp_0_1_enable_vif(
	// Input values,
	const string&	vif_name) = 0;

    virtual XrlCmdError mld6igmp_0_1_disable_vif(
	// Input values,
	const string&	vif_name) = 0;

    virtual XrlCmdError mld6igmp_0_1_start_vif(
	// Input values,
	const string&	vif_name) = 0;

    virtual XrlCmdError mld6igmp_0_1_stop_vif(
	// Input values,
	const string&	vif_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Enable/disable/start/stop all MLD6IGMP vif interfaces.
     */
    virtual XrlCmdError mld6igmp_0_1_enable_all_vifs() = 0;

    virtual XrlCmdError mld6igmp_0_1_disable_all_vifs() = 0;

    virtual XrlCmdError mld6igmp_0_1_start_all_vifs() = 0;

    virtual XrlCmdError mld6igmp_0_1_stop_all_vifs() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Enable/disable/start/stop MLD6IGMP protocol and the MLD6IGMP CLI
     *  access.
     */
    virtual XrlCmdError mld6igmp_0_1_enable_mld6igmp() = 0;

    virtual XrlCmdError mld6igmp_0_1_disable_mld6igmp() = 0;

    virtual XrlCmdError mld6igmp_0_1_enable_cli() = 0;

    virtual XrlCmdError mld6igmp_0_1_disable_cli() = 0;

    virtual XrlCmdError mld6igmp_0_1_start_mld6igmp() = 0;

    virtual XrlCmdError mld6igmp_0_1_stop_mld6igmp() = 0;

    virtual XrlCmdError mld6igmp_0_1_start_cli() = 0;

    virtual XrlCmdError mld6igmp_0_1_stop_cli() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Configure MLD6IGMP interface-related metrics. The 'set_foo' XRLs set
     *  the particular values. The 'reset_foo' XRLs reset the metrics to their
     *  default values.
     *
     *  @param vif_name the name of the vif to configure.
     *
     *  @param proto_version the protocol version.
     */
    virtual XrlCmdError mld6igmp_0_1_get_vif_proto_version(
	// Input values,
	const string&	vif_name,
	// Output values,
	uint32_t&	proto_version) = 0;

    virtual XrlCmdError mld6igmp_0_1_set_vif_proto_version(
	// Input values,
	const string&	vif_name,
	const uint32_t&	proto_version) = 0;

    virtual XrlCmdError mld6igmp_0_1_reset_vif_proto_version(
	// Input values,
	const string&	vif_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Enable/disable the MLD6IGMP trace log for all operations.
     *
     *  @param enable if true, then enable the trace log, otherwise disable it.
     */
    virtual XrlCmdError mld6igmp_0_1_log_trace_all(
	// Input values,
	const bool&	enable) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Add/delete a client protocol in the MLD/IGMP protocol.
     *
     *  @param xrl_sender_name the XRL name of the originator of this XRL.
     *
     *  @param protocol_name the name of the protocol to add/delete.
     *
     *  @param protocol_id the ID of the protocol to add/delete (both sides
     *  must agree on the particular values).
     *
     *  @param vif_name the name of the vif the protocol add/delete to apply
     *  to.
     *
     *  @param vif_index the index of the vif the protocol add/delete to apply
     *  to. The added protocol will receive Join/Leave membership information
     *  about same-LAN members for the particular vif.
     */
    virtual XrlCmdError mld6igmp_0_1_add_protocol4(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index) = 0;

    virtual XrlCmdError mld6igmp_0_1_add_protocol6(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index) = 0;

    virtual XrlCmdError mld6igmp_0_1_delete_protocol4(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index) = 0;

    virtual XrlCmdError mld6igmp_0_1_delete_protocol6(
	// Input values,
	const string&	xrl_sender_name,
	const string&	protocol_name,
	const uint32_t&	protocol_id,
	const string&	vif_name,
	const uint32_t&	vif_index) = 0;

private:
    const XrlCmdError handle_common_0_1_get_target_name(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_get_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_get_status(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_shutdown(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_cli_processor_0_1_process_command(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_new_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_delete_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_add_vif_addr4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_add_vif_addr6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_delete_vif_addr4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_delete_vif_addr6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_set_vif_flags(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_set_all_vifs_done(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_is_vif_setup_completed(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_protocol_message4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_protocol_message6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_kernel_signal_message4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_kernel_signal_message6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_add_mrib4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_add_mrib6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_delete_mrib4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_delete_mrib6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_set_mrib_done(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_dataflow_signal4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mfea_client_0_1_recv_dataflow_signal6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_enable_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_disable_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_start_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_stop_vif(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_enable_all_vifs(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_disable_all_vifs(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_start_all_vifs(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_stop_all_vifs(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_enable_mld6igmp(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_disable_mld6igmp(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_enable_cli(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_disable_cli(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_start_mld6igmp(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_stop_mld6igmp(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_start_cli(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_stop_cli(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_get_vif_proto_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_set_vif_proto_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_reset_vif_proto_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_log_trace_all(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_add_protocol4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_add_protocol6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_delete_protocol4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_mld6igmp_0_1_delete_protocol6(const XrlArgs& in, XrlArgs* out);

    void add_handlers()
    {
	if (_cmds->add_handler("common/0.1/get_target_name",
	    callback(this, &XrlMld6igmpTargetBase::handle_common_0_1_get_target_name)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/common/0.1/get_target_name");
	}
	if (_cmds->add_handler("common/0.1/get_version",
	    callback(this, &XrlMld6igmpTargetBase::handle_common_0_1_get_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/common/0.1/get_version");
	}
	if (_cmds->add_handler("common/0.1/get_status",
	    callback(this, &XrlMld6igmpTargetBase::handle_common_0_1_get_status)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/common/0.1/get_status");
	}
	if (_cmds->add_handler("common/0.1/shutdown",
	    callback(this, &XrlMld6igmpTargetBase::handle_common_0_1_shutdown)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/common/0.1/shutdown");
	}
	if (_cmds->add_handler("cli_processor/0.1/process_command",
	    callback(this, &XrlMld6igmpTargetBase::handle_cli_processor_0_1_process_command)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/cli_processor/0.1/process_command");
	}
	if (_cmds->add_handler("mfea_client/0.1/new_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_new_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/new_vif");
	}
	if (_cmds->add_handler("mfea_client/0.1/delete_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_delete_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/delete_vif");
	}
	if (_cmds->add_handler("mfea_client/0.1/add_vif_addr4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_add_vif_addr4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/add_vif_addr4");
	}
	if (_cmds->add_handler("mfea_client/0.1/add_vif_addr6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_add_vif_addr6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/add_vif_addr6");
	}
	if (_cmds->add_handler("mfea_client/0.1/delete_vif_addr4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_delete_vif_addr4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/delete_vif_addr4");
	}
	if (_cmds->add_handler("mfea_client/0.1/delete_vif_addr6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_delete_vif_addr6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/delete_vif_addr6");
	}
	if (_cmds->add_handler("mfea_client/0.1/set_vif_flags",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_set_vif_flags)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/set_vif_flags");
	}
	if (_cmds->add_handler("mfea_client/0.1/set_all_vifs_done",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_set_all_vifs_done)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/set_all_vifs_done");
	}
	if (_cmds->add_handler("mfea_client/0.1/is_vif_setup_completed",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_is_vif_setup_completed)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/is_vif_setup_completed");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_protocol_message4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_protocol_message4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_protocol_message4");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_protocol_message6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_protocol_message6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_protocol_message6");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_kernel_signal_message4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_kernel_signal_message4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_kernel_signal_message4");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_kernel_signal_message6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_kernel_signal_message6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_kernel_signal_message6");
	}
	if (_cmds->add_handler("mfea_client/0.1/add_mrib4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_add_mrib4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/add_mrib4");
	}
	if (_cmds->add_handler("mfea_client/0.1/add_mrib6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_add_mrib6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/add_mrib6");
	}
	if (_cmds->add_handler("mfea_client/0.1/delete_mrib4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_delete_mrib4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/delete_mrib4");
	}
	if (_cmds->add_handler("mfea_client/0.1/delete_mrib6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_delete_mrib6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/delete_mrib6");
	}
	if (_cmds->add_handler("mfea_client/0.1/set_mrib_done",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_set_mrib_done)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/set_mrib_done");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_dataflow_signal4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_dataflow_signal4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_dataflow_signal4");
	}
	if (_cmds->add_handler("mfea_client/0.1/recv_dataflow_signal6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mfea_client_0_1_recv_dataflow_signal6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mfea_client/0.1/recv_dataflow_signal6");
	}
	if (_cmds->add_handler("mld6igmp/0.1/enable_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_enable_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/enable_vif");
	}
	if (_cmds->add_handler("mld6igmp/0.1/disable_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_disable_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/disable_vif");
	}
	if (_cmds->add_handler("mld6igmp/0.1/start_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_start_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/start_vif");
	}
	if (_cmds->add_handler("mld6igmp/0.1/stop_vif",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_stop_vif)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/stop_vif");
	}
	if (_cmds->add_handler("mld6igmp/0.1/enable_all_vifs",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_enable_all_vifs)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/enable_all_vifs");
	}
	if (_cmds->add_handler("mld6igmp/0.1/disable_all_vifs",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_disable_all_vifs)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/disable_all_vifs");
	}
	if (_cmds->add_handler("mld6igmp/0.1/start_all_vifs",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_start_all_vifs)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/start_all_vifs");
	}
	if (_cmds->add_handler("mld6igmp/0.1/stop_all_vifs",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_stop_all_vifs)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/stop_all_vifs");
	}
	if (_cmds->add_handler("mld6igmp/0.1/enable_mld6igmp",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_enable_mld6igmp)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/enable_mld6igmp");
	}
	if (_cmds->add_handler("mld6igmp/0.1/disable_mld6igmp",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_disable_mld6igmp)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/disable_mld6igmp");
	}
	if (_cmds->add_handler("mld6igmp/0.1/enable_cli",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_enable_cli)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/enable_cli");
	}
	if (_cmds->add_handler("mld6igmp/0.1/disable_cli",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_disable_cli)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/disable_cli");
	}
	if (_cmds->add_handler("mld6igmp/0.1/start_mld6igmp",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_start_mld6igmp)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/start_mld6igmp");
	}
	if (_cmds->add_handler("mld6igmp/0.1/stop_mld6igmp",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_stop_mld6igmp)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/stop_mld6igmp");
	}
	if (_cmds->add_handler("mld6igmp/0.1/start_cli",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_start_cli)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/start_cli");
	}
	if (_cmds->add_handler("mld6igmp/0.1/stop_cli",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_stop_cli)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/stop_cli");
	}
	if (_cmds->add_handler("mld6igmp/0.1/get_vif_proto_version",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_get_vif_proto_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/get_vif_proto_version");
	}
	if (_cmds->add_handler("mld6igmp/0.1/set_vif_proto_version",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_set_vif_proto_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/set_vif_proto_version");
	}
	if (_cmds->add_handler("mld6igmp/0.1/reset_vif_proto_version",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_reset_vif_proto_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/reset_vif_proto_version");
	}
	if (_cmds->add_handler("mld6igmp/0.1/log_trace_all",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_log_trace_all)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/log_trace_all");
	}
	if (_cmds->add_handler("mld6igmp/0.1/add_protocol4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_add_protocol4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/add_protocol4");
	}
	if (_cmds->add_handler("mld6igmp/0.1/add_protocol6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_add_protocol6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/add_protocol6");
	}
	if (_cmds->add_handler("mld6igmp/0.1/delete_protocol4",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_delete_protocol4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/delete_protocol4");
	}
	if (_cmds->add_handler("mld6igmp/0.1/delete_protocol6",
	    callback(this, &XrlMld6igmpTargetBase::handle_mld6igmp_0_1_delete_protocol6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://mld6igmp/mld6igmp/0.1/delete_protocol6");
	}
	_cmds->finalize();
    }

    void remove_handlers()
    {
	_cmds->remove_handler("common/0.1/get_target_name");
	_cmds->remove_handler("common/0.1/get_version");
	_cmds->remove_handler("common/0.1/get_status");
	_cmds->remove_handler("common/0.1/shutdown");
	_cmds->remove_handler("cli_processor/0.1/process_command");
	_cmds->remove_handler("mfea_client/0.1/new_vif");
	_cmds->remove_handler("mfea_client/0.1/delete_vif");
	_cmds->remove_handler("mfea_client/0.1/add_vif_addr4");
	_cmds->remove_handler("mfea_client/0.1/add_vif_addr6");
	_cmds->remove_handler("mfea_client/0.1/delete_vif_addr4");
	_cmds->remove_handler("mfea_client/0.1/delete_vif_addr6");
	_cmds->remove_handler("mfea_client/0.1/set_vif_flags");
	_cmds->remove_handler("mfea_client/0.1/set_all_vifs_done");
	_cmds->remove_handler("mfea_client/0.1/is_vif_setup_completed");
	_cmds->remove_handler("mfea_client/0.1/recv_protocol_message4");
	_cmds->remove_handler("mfea_client/0.1/recv_protocol_message6");
	_cmds->remove_handler("mfea_client/0.1/recv_kernel_signal_message4");
	_cmds->remove_handler("mfea_client/0.1/recv_kernel_signal_message6");
	_cmds->remove_handler("mfea_client/0.1/add_mrib4");
	_cmds->remove_handler("mfea_client/0.1/add_mrib6");
	_cmds->remove_handler("mfea_client/0.1/delete_mrib4");
	_cmds->remove_handler("mfea_client/0.1/delete_mrib6");
	_cmds->remove_handler("mfea_client/0.1/set_mrib_done");
	_cmds->remove_handler("mfea_client/0.1/recv_dataflow_signal4");
	_cmds->remove_handler("mfea_client/0.1/recv_dataflow_signal6");
	_cmds->remove_handler("mld6igmp/0.1/enable_vif");
	_cmds->remove_handler("mld6igmp/0.1/disable_vif");
	_cmds->remove_handler("mld6igmp/0.1/start_vif");
	_cmds->remove_handler("mld6igmp/0.1/stop_vif");
	_cmds->remove_handler("mld6igmp/0.1/enable_all_vifs");
	_cmds->remove_handler("mld6igmp/0.1/disable_all_vifs");
	_cmds->remove_handler("mld6igmp/0.1/start_all_vifs");
	_cmds->remove_handler("mld6igmp/0.1/stop_all_vifs");
	_cmds->remove_handler("mld6igmp/0.1/enable_mld6igmp");
	_cmds->remove_handler("mld6igmp/0.1/disable_mld6igmp");
	_cmds->remove_handler("mld6igmp/0.1/enable_cli");
	_cmds->remove_handler("mld6igmp/0.1/disable_cli");
	_cmds->remove_handler("mld6igmp/0.1/start_mld6igmp");
	_cmds->remove_handler("mld6igmp/0.1/stop_mld6igmp");
	_cmds->remove_handler("mld6igmp/0.1/start_cli");
	_cmds->remove_handler("mld6igmp/0.1/stop_cli");
	_cmds->remove_handler("mld6igmp/0.1/get_vif_proto_version");
	_cmds->remove_handler("mld6igmp/0.1/set_vif_proto_version");
	_cmds->remove_handler("mld6igmp/0.1/reset_vif_proto_version");
	_cmds->remove_handler("mld6igmp/0.1/log_trace_all");
	_cmds->remove_handler("mld6igmp/0.1/add_protocol4");
	_cmds->remove_handler("mld6igmp/0.1/add_protocol6");
	_cmds->remove_handler("mld6igmp/0.1/delete_protocol4");
	_cmds->remove_handler("mld6igmp/0.1/delete_protocol6");
    }
};

#endif /* __XRL_INTERFACES_MLD6IGMP_BASE_HH__ */
