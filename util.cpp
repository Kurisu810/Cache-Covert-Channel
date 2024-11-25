#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}



// Function to convert a single character to a binary string representing its ASCII value
std::string char_to_binary_string(char input_char) {
    // Convert the character to its ASCII binary representation
    std::bitset<8> bits(input_char);
    return bits.to_string();
}



// Function to convert a binary string back to an ASCII string
std::string binary_string_to_string(const std::string& binary_string) {
    std::string output;

    for (size_t i = 0; i < binary_string.length(); i += 8) {
        // Extract 8 bits from the binary string
        std::string byte_string = binary_string.substr(i, 8);
        // Convert the binary string to a character
        char character = static_cast<char>(std::bitset<8>(byte_string).to_ulong());
        output += character;
    }

    return output;
}