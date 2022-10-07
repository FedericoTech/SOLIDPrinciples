/**
 * Interface Segregation principle.
 * many client specific interfaces are better than one general interface.
 */

#include <vector>
#include <cstdint>
#include <string>
#include <functional>


struct PinDirection {};

//#define BAD_CODE


#ifdef BAD_CODE
	/**
	 * In this block of code we have these two interfaces that expose a number of features
	 * we not always want to implement all the member functions because we only need a few
	 */
	struct Runtime {
		virtual ~Runtime() = default;
		virtual void sendViaI2C(std::vector<uint8_t> bytesToSend) = 0;
		virtual std::vector<uint8_t> readViaI2C(int numberOfBytesToRead) = 0;

		virtual void sendViaUART(std::vector<uint8_t> bytesToSend) = 0;
		virtual std::vector<uint8_t> readViaUART(int numberOfBytesToRead) = 0;

		virtual void setPinDirection(int p, PinDirection d) = 0;
		virtual void setPin(int pin) = 0;
		virtual void clearPin(int pin) = 0;
	};

	/**
	 * but the interface forces me to
	 */
	struct MyUARTRuntime: public Runtime {
		~MyUARTRuntime() = default;

		void sendViaI2C(std::vector<uint8_t> bytesToSend){};		//I nullify this
		std::vector<uint8_t> readViaI2C(int numberOfBytesToRead){	//I nullify this
			return std::vector<uint8_t>();
		}

		//I only need these two
		void sendViaUART(std::vector<uint8_t> bytesToSend) override;
		std::vector<uint8_t> readViaUART(int numberOfBytesToRead) override;

		void setPinDirection(int p, PinDirection d){};	//I nullify this
		void setPin(int pin){};							//I nullify this
		void clearPin(int pin){};						//I nullify this
	};

	/**
	 * In this second example I use all the method of the interface but the interface would be
	 * better off split into two different concerns.
	 */
	struct SerialManager {
		virtual ~SerialManager() = default;
		virtual void registerReceiver(std::function<void(std::string)> receiver) = 0;
		virtual void send(std::string message) = 0;
		virtual void readLine() = 0;
	};

	struct MySerialManager : public SerialManager {
		void registerReceiver(std::function<void(std::string)> receiver) override;
		void send(std::string message) override;
		void readLine() override;
	};
#else

	//we have now these three interfaces split by concerns
	struct RuntimeI2C {
		virtual ~RuntimeI2C() = default;
		virtual void sendViaI2C(std::vector<uint8_t> bytesToSend) = 0;
		virtual std::vector<uint8_t> readViaI2C(int numberOfBytesToRead) = 0;
	};

	struct RuntimeUART {
		virtual ~RuntimeUART() = default;
		virtual void sendViaUART(std::vector<uint8_t> bytesToSend) = 0;
		virtual std::vector<uint8_t> readViaUART(int numberOfBytesToRead) = 0;
	};

	struct RuntimePin {
		virtual ~RuntimePin() = default;
		virtual void setPinDirection(int p, PinDirection d) = 0;
		virtual void setPin(int pin) = 0;
		virtual void clearPin(int pin) = 0;
	};

	//and i only implement the features i need, not all the rest
	struct MyUARTRuntime: public RuntimeUART {
		~MyUARTRuntime() = default;
		//I only need these two
		void sendViaUART(std::vector<uint8_t> bytesToSend) override;
		std::vector<uint8_t> readViaUART(int numberOfBytesToRead) override;
	};

	/**
	 * with these two interfaces also split from the example a above
	 * I demonstrat that i can have the full implementation by implementing them all in the same class if i need to.
	 */

	struct SerialClient {
		virtual ~SerialClient() = default;
		virtual void registerReceiver(std::function<void(std::string)> receiver) = 0;
		virtual void send(std::string message) = 0;
	};

	struct SerialReader {
		virtual ~SerialReader() = default;
		virtual void readLine() = 0;
	};

	struct MySerialManager: public SerialClient, public SerialReader {
		void registerReceiver(std::function<void(std::string)> receiver) override;
		void send(std::string message) override;
		void readLine() override;
	};

#endif

int main()
{
	return 0;
}
