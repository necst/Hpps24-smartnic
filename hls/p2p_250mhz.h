#pragma once

#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include <iostream>

// Define the width of the user data
#define DATA_WIDTH 512
#define USER_WIDTH 16  // t_user_size, t_user_src, t_user_dst

#define AXIS_ENABLE_DATA 0b00000001
#define AXIS_ENABLE_DEST 0b00000010
#define AXIS_ENABLE_ID   0b00000100
#define AXIS_ENABLE_KEEP 0b00001000
#define AXIS_ENABLE_LAST 0b00010000
#define AXIS_ENABLE_STRB 0b00100000
#define AXIS_ENABLE_USER 0b01000000

/*
 * Enabled signals and corresponding map to the opennic interface
 * DATA: tdata
 * USER: t_user_size
 * ID: t_user_src
 * DEST: t_user_dst
 */  
#define ENABLED_SIGNALS ( AXIS_ENABLE_DATA | AXIS_ENABLE_KEEP | AXIS_ENABLE_LAST | AXIS_ENABLE_USER | AXIS_ENABLE_ID | AXIS_ENABLE_DEST )

// Define the AXI4-Stream data type
// using data_t = hls::axis<int, 0, 0, 0, ENABLED_SIGNALS>;
typedef hls::axis<ap_uint<DATA_WIDTH>, USER_WIDTH, USER_WIDTH, USER_WIDTH, ENABLED_SIGNALS> box250_axi_t;
typedef hls::stream<box250_axi_t> box250_axi_stream_t;

void p2p_250mhz(box250_axi_stream_t& s_axis_qdma_h2c, box250_axi_stream_t& m_axis_qdma_c2h, box250_axi_stream_t& m_axis_adap_tx_250mhz, box250_axi_stream_t& s_axis_adap_rx_250mhz);

#define TEST_MULT 2
