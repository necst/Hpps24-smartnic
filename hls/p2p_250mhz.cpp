#include "p2p_250mhz.h"

#define DEBUG

#define MAX_PACKET_LEN 1514
#define PREAMBLE_SFD_LEN 0
#define MAC_ADDR_LEN 48

#define BLOCKED_MAC_ADDR 0x9c3568414972
#define ALLOWED_MAC_ADDR 0x9b3568414972


ap_uint<MAC_ADDR_LEN> in_mac_addr_src;
ap_uint<MAC_ADDR_LEN> in_mac_addr_dst;
box250_axi_t from_h2c_engine, to_tx_adapter;

box250_axi_t from_rx_adapter, to_c2h_engine;

void p2p_250mhz(box250_axi_stream_t& s_axis_qdma_h2c, box250_axi_stream_t& m_axis_qdma_c2h, box250_axi_stream_t& m_axis_adap_tx_250mhz, box250_axi_stream_t& s_axis_adap_rx_250mhz){
  #pragma HLS INTERFACE mode=axis port=s_axis_qdma_h2c
  #pragma HLS INTERFACE mode=axis port=m_axis_qdma_c2h
  #pragma HLS INTERFACE mode=axis port=m_axis_adap_tx_250mhz
  #pragma HLS INTERFACE mode=axis port=s_axis_adap_rx_250mhz


  static int in_i = 0;
  static int out_i = 0;

  // Packets leaving the system, going to MAC port
  if(!s_axis_qdma_h2c.empty()) {
    from_h2c_engine = s_axis_qdma_h2c.read();

    // Extract the source and destination MAC addresses, only for the first batch
    if (in_i == 0) {
      // Preamble and SFD are 7+1 bytes long, so we need to skip them
      in_mac_addr_dst = from_h2c_engine.data(PREAMBLE_SFD_LEN+MAC_ADDR_LEN-1, PREAMBLE_SFD_LEN);
      in_mac_addr_src = from_h2c_engine.data(PREAMBLE_SFD_LEN+MAC_ADDR_LEN+MAC_ADDR_LEN-1, PREAMBLE_SFD_LEN+MAC_ADDR_LEN);

      #ifdef DEBUG
      std::cout << "DEBUG - From H2C Destination MAC address: " << std::hex << in_mac_addr_dst << std::endl;
      std::cout << "DEBUG - From H2C Source MAC address: " << std::hex << in_mac_addr_src << std::endl;
      #endif
    }


    if (from_h2c_engine.last) {
      in_i = 0;
    } else {
      in_i++;
    }

    // Send the packet to the MAC port
    if (in_mac_addr_dst == ALLOWED_MAC_ADDR) {
      #ifdef DEBUG
      std::cout << "DEBUG - Sending packet to MAC port" << std::endl;
      #endif
    m_axis_adap_tx_250mhz.write(from_h2c_engine);
    }
  }

  // Packets entering the system, coming from MAC port
  if (!s_axis_adap_rx_250mhz.empty()) {
    from_rx_adapter = s_axis_adap_rx_250mhz.read();

    // Extract the source and destination MAC addresses, only for the first batch
    if (out_i == 0) {
      // Preamble and SFD are 7+1 bytes long, so we need to skip them
      ap_uint<MAC_ADDR_LEN> out_mac_addr_dst = from_rx_adapter.data(PREAMBLE_SFD_LEN+MAC_ADDR_LEN-1, PREAMBLE_SFD_LEN);
      ap_uint<MAC_ADDR_LEN> out_mac_addr_src = from_rx_adapter.data(PREAMBLE_SFD_LEN+MAC_ADDR_LEN+MAC_ADDR_LEN-1, PREAMBLE_SFD_LEN+MAC_ADDR_LEN);
    }

    if (from_rx_adapter.last) {
      out_i = 0;
    } else {
      out_i++;
    }

    // Send the packet to the QDMA port
    m_axis_qdma_c2h.write(from_rx_adapter);
  }
}
