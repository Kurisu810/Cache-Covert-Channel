#include"util.hpp"

using namespace std;
using namespace std::chrono;

#define BUFFER_SIZE_FACTOR 2
int num_set_index_bits = log2(LLC_NUM_SETS);
int num_cache_line_bits = log2(CACHE_LINE_SIZE);



// Send a single bit
void send_bit(char* buffer, int bit) {
    if (bit == 1) {
        // Evict all cache lines
        for (int i = 0; i < LLC_NUM_SETS; i++) {
            for (int j = 0; j < LLC_ASSOCIATIVITY * BUFFER_SIZE_FACTOR; j++) {
                // Write to every set (ASSOCIATIVITY * BUFFER_SIZE_FACTOR) times
                ADDR_PTR addr = (ADDR_PTR) &buffer[(j << (num_cache_line_bits + num_set_index_bits)) + (i << num_cache_line_bits)];
                buffer[(j << (num_cache_line_bits + num_set_index_bits)) + (i << num_cache_line_bits)] = 0;	
            }
        }
    } else {
        // Do nothing
    }
}



// Send a string of binary values
void send_character(char* buffer, string bits) {
    // Wait for clock edge
    while (duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() % PERIOD_US != 0) {}

    // Send start bit
    auto start = high_resolution_clock::now();
    auto end = start + microseconds(PERIOD_US);

    while (high_resolution_clock::now() < end) {
        send_bit(buffer, 1);
    }

    // Send bits
    for (char bit : bits) {
        start = high_resolution_clock::now();
        end = start + microseconds(PERIOD_US);

        while (high_resolution_clock::now() < end) {
            send_bit(buffer, bit - '0');
        }
    }
}



int main(int argc, char **argv)
{
    // Initialize buffer
    void *ptr;
    posix_memalign(&ptr, CACHE_LINE_SIZE * LLC_NUM_SETS, LLC_SIZE * BUFFER_SIZE_FACTOR);
	char* buffer = (char*)ptr;

    char text_buf[128];
    printf("Press enter to start sending bits.\n");

    // Sender loop
	bool sending = true;
	while (sending) {
        printf("> ");
        fgets(text_buf, sizeof(text_buf), stdin);

        // Remove the newline character if present
        size_t len = strlen(text_buf);
        if (len > 0 && text_buf[len-1] == '\n') {
            text_buf[len-1] = '\0';
        }

        // Send message
        for (size_t i = 0; i < len; ++i) {
            // Convert each character in the input buffer to ASCII value and send
            send_character(buffer, char_to_binary_string(text_buf[i]));
        }
	}

    // Finish
	printf("Sender finished.\n");
    free((void *) buffer);
	return 0;
}


