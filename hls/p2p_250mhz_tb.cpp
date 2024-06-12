#include "p2p_250mhz.h"

// #define TEST_DATA_SIZE 8

using namespace std;

// ap_uint<DATA_WIDTH> test_data[TEST_DATA_SIZE] = {
//     0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7
// };

#define TEST_PACKET_BYTES 54

ap_uint<8> test_packet [TEST_PACKET_BYTES] = {
0x72,0x49,0x41,0x68,0x35,0x9b,0x14,0xeb,0xb6,0x0b,0xcb,0xdb,0x08,0x00,0x45,0x00,0x00,0x28,0xe5,0xda,0x40,0x00,0x80,0x06,0x00,0x00,0xc0,0xa8,0x64,0x03,0xc0,0xa8,0x64,0xf3,0xcc,0x91,0x00,0x50,0x82,0x38,0x54,0x45,0xb6,0x84,0x68,0x4f,0x50,0x10,0x04,0x05,0x4a,0x62,0x00,0x00
};

int i = 0;
int tx_adapter_i = 0;
int c2h_engine_i = 0;

int main() {
    int test_data_size = TEST_PACKET_BYTES / 512 +1;

    // Converting the test packet to a 512-bit data array  
    ap_uint<DATA_WIDTH> test_data[test_data_size];
    for (int i = 0; i < test_data_size; i++) {
      test_data[i] = 0;
      int j = 0;

      // Reversed order
      // while (j < DATA_WIDTH/8 && (j+(i*DATA_WIDTH))<TEST_PACKET_BYTES) {
      //   test_data[i] = test_data[i] << 8;
      //   test_data[i] = test_data[i] | test_packet[i*512+j];
      //   j++;
      // }
      // while(j < DATA_WIDTH/8) {
      //   test_data[i] = test_data[i] << 8;
      //   j++;
      // }

      // Normal order
      while (j < DATA_WIDTH/8 && (j+(i*DATA_WIDTH))<TEST_PACKET_BYTES) {
        test_data[i] = test_data[i] + ((ap_uint<DATA_WIDTH>)test_packet[i*512+j] << (8*j));
        j++;
      }
    }
    std::cout << "Test data: " << std::hex << test_data[0] << std::endl;
    std::cout << "Test packet dst addr: " << std::hex << (ap_uint<48>)test_data[0](47,0) << std::endl;

    // Create some test data
    box250_axi_t h2c_engine, tx_adapter;
    box250_axi_t rx_adapter, c2h_engine;

    // Create HLS streams for the input and output data
    box250_axi_stream_t h2c_engine_s, tx_adapter_s;
    box250_axi_stream_t rx_adapter_s, c2h_engine_s;

    while(true){
      if(i < test_data_size) {
        // Fill the test data with some values
        h2c_engine.data = test_data[i];
        h2c_engine.last = 1;
        h2c_engine_s.write(h2c_engine);
        rx_adapter.data = test_data[i];
        rx_adapter.last = 1;
        rx_adapter_s.write(rx_adapter);
      }
      // Call the interface function
      p2p_250mhz(h2c_engine_s, c2h_engine_s, tx_adapter_s, rx_adapter_s);

      if (!tx_adapter_s.empty()) {
        tx_adapter_s.read(tx_adapter);
        std::cout << "Tx adapter read data: " << std::hex << tx_adapter.data << std::endl;
        if (tx_adapter.data != test_data[tx_adapter_i]) {
          std::cout << "Test failed on tx_adapter at index " << i << std::endl;
          return 2;
        }
        tx_adapter_i++;
      }


      if(!c2h_engine_s.empty()) {
        c2h_engine_s.read(c2h_engine);
        std::cout << "C2H engine read data: " << std::hex << c2h_engine.data << std::endl;
        if (c2h_engine.data != test_data[c2h_engine_i]) {
          std::cout << "Test failed on c2h_engine at index " << i << std::endl;
          return 3;
        }
        c2h_engine_i++;
      }
      if (i >= test_data_size && tx_adapter_i >= test_data_size && c2h_engine_i >= test_data_size) {
        break;
      }
      i++;
    }

    if (tx_adapter_i < test_data_size) {
      std::cout << "Test failed on tx_adapter_i" << std::endl;
      std::cout << "tx_adapter_i: " << tx_adapter_i << std::endl;
      return 2;
    }
    if (c2h_engine_i < test_data_size) {
      std::cout << "Test failed on c2h_engine_i" << std::endl;
      std::cout << "c2h_engine_i: " << c2h_engine_i << std::endl;
      return 3;
    }

    std::cout << "Test passed successfully!" << std::endl;
    return 0;
}
