#include <Crypto.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

// The data to test
constexpr uint8_t message[] = "Hello";
const int msg_len = sizeof(message) - 1;

const uint8_t none_pub[] = {};

// Caesar with key = 1
constexpr uint8_t caesar1[] = "Gdkkn";
const uint8_t caesar1_pub[] = { 255 };

// A template callback that validates it against an expected template argument
template <const uint8_t *ex_msg, const int ex_len>
bool Verify(std::vector<uint8_t> data) {
	if (ex_len != data.size()) {
		std::cout
			<< "\tMessage length incorrect:" << std::endl
			<< "\t\tExpected: " << ex_len << std::endl
			<< "\t\tActual: " << data.size() << std::endl;
		return false;
	}

	bool equal = true;
	for (int i = 0; i < ex_len; ++i) {
		if (data[i] != ex_msg[i]) {
			equal = false;
			break;
		}
	}

	if(!equal) {
		std::cout
			<< "\tMessage contents incorrect:" << std::endl
			<< "\t\tExpected: \"";
		std::cout.write((const char*)ex_msg, ex_len);
		std::cout
			<< "\"" << std::endl
			<< "\t\tActual: \"";
		std::cout.write((const char*)data.data(), data.size());
		std::cout
			<< "\"" << std::endl;
		return false;
	}

	return true;
}

// A shorthand for referencing the above template class as a parameter
typedef std::function<bool(std::vector<uint8_t> data)> test;

// A struct that compresses the passing of 3 counting variables
struct TestResults {
public:
	int total = 0;
	int passed = 0;
	int failed = 0;
};

// Calls the static Crypto factory method and tests both the encryption and decryption processes
void doTest(Algorithm op, const char* op_name,
            const uint8_t* pubKey, uint8_t pubLen,
            const uint8_t* priKey, uint8_t priLen,
            test eTest, const uint8_t *msg, uint32_t len,
            test dTest, const uint8_t *ex_msg, uint32_t ex_len,
            TestResults *results)
{
	std::shared_ptr<Crypto> testing;
	std::vector<uint8_t> buffEncrypt;
	std::vector<uint8_t> buffDecrypt;

	testing = Crypto::cryptoFactory(
		[&buffEncrypt](const uint8_t *data, uint32_t len)
		{
			buffEncrypt.insert(buffEncrypt.end(), data, data + len);
		},
		[&buffDecrypt](const uint8_t *data, uint32_t len)
		{
			buffDecrypt.insert(buffDecrypt.end(), data, data + len);
		},
		op);
	testing->setKeys(pubKey, pubLen, priKey, priLen);

	// Test the encrypt function
	++(results->total);
	std::cout << op_name << "::encrypt() - Start" << std::endl;
	if (!testing->encrypt(msg, len)) {
		std::cout << "\tReturned false on message encryption" << std::endl;
		std::cout << op_name << "::encrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else if (!testing->encrypt(msg, 0)) {
		std::cout << "\tReturned false on flush command" << std::endl;
		std::cout << op_name << "::encrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else if (!eTest(buffEncrypt)) {
		std::cout << op_name << "::encrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else {
		std::cout << op_name << "::encrypt() - Passed" << std::endl;
		++(results->passed);
	}
	std::cout << std::endl;

	// Test the decrypt function
	++(results->total);
	std::cout << op_name << "::decrypt() - Start" << std::endl;
	if (!testing->decrypt(ex_msg, ex_len)) {
		std::cout << "\tReturned false on message decryption" << std::endl;
		std::cout << op_name << "::decrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else if (!testing->decrypt(msg, 0)) {
		std::cout << "\tReturned false on flush command" << std::endl;
		std::cout << op_name << "::decrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else if (!dTest(buffDecrypt)) {
		std::cout << op_name << "::decrypt() - Failed" << std::endl;
		++(results->failed);
	}
	else {
		std::cout << op_name << "::decrypt() - Passed" << std::endl;
		++(results->passed);
	}
	std::cout << std::endl;
}

int main() {
	std::cout
		<< "+--------------------------------------------------+" << std::endl
		<< "|  Programming Exercise 1: Cryptography Interface  |" << std::endl
		<< "|                    Unit Tests                    |" << std::endl
		<< "+--------------------------------------------------+" << std::endl
		<< std::endl;

	// Counters across tests
	TestResults results;

	// Test the eNONE encryption class
	doTest(
		Algorithm::eNONE, "CryptoNone",
		none_pub, 0,
		none_pub, 0,
		Verify<message, msg_len>, message, msg_len,
		Verify<message, msg_len>, message, msg_len,
		&results);

	// Test the eCAESAR encryption class
	doTest(
		Algorithm::eCAESAR, "CryptoCaesar",
		caesar1_pub, 1,
		none_pub, 0,
		Verify<caesar1, msg_len>, message, msg_len,
		Verify<message, msg_len>, caesar1, msg_len,
		&results);

	std::cout
		<< "+--------------------------------------------------+" << std::endl
		<< "|                     Summary:                     |" << std::endl
		<< "| Tests Attempted: " << results.total << std::endl
		<< "| Tests Passed: " << results.passed << std::endl
		<< "| Tests Failed: " << results.failed << std::endl
		<< "+--------------------------------------------------+" << std::endl;

	return 0;
}
