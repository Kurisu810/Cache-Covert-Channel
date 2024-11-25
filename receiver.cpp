#include"util.hpp"

using namespace std;
using namespace std::chrono;



// Probe the cache line and receive a single bit
int receive_bit(char* buffer) {
	int hit = 0;
	int miss = 0;

	// Probe set 0 of L1 for 9 times
	for (int i = 0; i < 9; i++) {
		int cycles = (int) measure_one_block_access_time((ADDR_PTR) buffer);

		// Busy loop to reduce sampling rate
		for(int j = 0; j < 1000000; j++) {}

		// Record result
		if (cycles >= 1000) {
			continue; // Ignore RAM misses
		}

		if (cycles >= 100) {
			miss++;
		} else {
			hit++;
		}
	}

	return (miss > hit);
}



// Receive 8 bits and convert to ASCII character
string receive_character(char* buffer) {	
	int bit = 0;
	double counter = 0.5;
	double num_ones = 0;
	
	// Calculate initial clock edge
	auto now = high_resolution_clock::now();
    auto current_time_us = duration_cast<microseconds>(now.time_since_epoch()).count();
    auto next_clock = current_time_us - (current_time_us % PERIOD_US) + PERIOD_US;

	// Listen for start bit
	while(true) {
		// Continuously read
		counter += 0.5;
		num_ones += receive_bit(buffer);

		// Check start bit at clock edge
    	if (duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() >= next_clock) {
			next_clock += PERIOD_US;

			// Confirm start bit received
			if (num_ones > counter) {
				break;
			}

			// Reset counters for the next period
            counter = 0.5;
            num_ones = 0;
		}
	}

	// Receive bits
	string binary_string;

	for (int i = 0; i < 8; i++) {
		// Wait till the middle of the clock period
		while (duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() < (next_clock - PERIOD_US/2)) {}

		// Receive a bit and append '0' or '1' to the binary string
        if (receive_bit(buffer)) {
            binary_string += '1';
        } else {
            binary_string += '0';
        }

		// Wait till the clock period ends
		while (duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() < next_clock) {}

		next_clock += PERIOD_US;
	}

	return binary_string_to_string(binary_string);
}



int main(int argc, char **argv)
{
	// Initialize buffer
	void *ptr;
	posix_memalign(&ptr, L1_NUM_SETS * CACHE_LINE_SIZE, sizeof(char));
	char* buffer = (char*)ptr;
	*buffer = 0;
	
	char text_buf[128];
	printf("Press enter to receive bits.");
	fgets(text_buf, sizeof(text_buf), stdin);

	// Receiver loop
	bool listening = true;
	while (listening) {
		string received_message;
        
		// Receive message
        while (true) {
            string received_char = receive_character(buffer);

			// Stop receiving when the null terminator is received
            if (received_char[0] == '\0') {
                break;
            } else {
                received_message += received_char[0];
            }
        }

        // Print the final received message
        printf("> %s\n", received_message.c_str());
	}

	printf("Receiver finished.\n");
	free((void *) buffer);
	return 0;
}


