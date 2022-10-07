/**
 * Liskov's Substitution principle
 * Parent classes should be easily substituted with their child classes without blowing the application
 */

#include <utility>
#include <iostream>

//#define BAD_CODE

#ifdef BAD_CODE

	/**
	 * In this first version we have these two concrete classes Gyroscope and Accelerometer
	 * that inherit from the base class InertialMeasurementUnit.
	 */

	struct InertialMeasurementUnit {

		virtual ~InertialMeasurementUnit() = default;

		virtual bool setFrequency(double frequency) = 0;
		virtual int getOrientation() const = 0;

	protected:
		double frequency;

	};

	struct Gyroscope: public InertialMeasurementUnit {

		//valid range [0.5, 10]
		bool setFrequency(double frequency) override;

		/**
		 * @return orientation in degrees [0, 360)
		 */
		int getOrientation() const override;
	protected:
		static const double max, min;
	};

	double const Gyroscope::min = .5f;
	double const Gyroscope::max = 10.0f;

	bool Gyroscope::setFrequency(double frequency){
		if(frequency >= min && frequency <= max){
			this->frequency = frequency;
			return true;
		}
		return false;
	}

	int Gyroscope::getOrientation() const {
		return 36; //some value
	}



	struct Accelerometer: public InertialMeasurementUnit
	{
		//valid range [0.1, 100]
		bool setFrequency(double frequency) override;

		/**
		 * @return orientation in degrees [-180, 180)
		 */
		int getOrientation() const override;
	protected:
		static constexpr double min = .1f, max = 100.0f;
	};

	constexpr double Accelerometer::min;
	constexpr double Accelerometer::max;

	bool Accelerometer::setFrequency(double frequency) {
		if(frequency >= min && frequency <= max){
			this->frequency = frequency;
			return true;
		}
		return false;
	}

	int Accelerometer::getOrientation() const {
		return 36; //some value
	}

	 /**
	  * The problem with this approach is that although they share the same interface,
	  * logically are different as they work with different ranges, Gyroscope goes from 0.5 through 10
	  * whereas Accelerometer works with ranges from 0.1 to 100
	  * so, they are actually incompatible an irreplaceable
	  */

#else //GOOD CODE

#include <stdexcept>
	/**
	 * one way to fix this is to through an exception if we try to work with
	 * frequencies that don't go according to the corresponding class.
	 * We also add a method getFrequencyRange so that we can look up what the range for the class is.
	 * so that we have a way to spot the problem.
	 */
	struct InertialMeasurementUnit {
		virtual ~InertialMeasurementUnit() = default;

		/**
		 * Sets the frequency of measurements
		 * @param frequency (in Hertz)
		 * @throw std::out_of_range exception if frequency is invalid
		 */
		virtual void setFrequency(double frequency) = 0;

		virtual int getOrientation() const = 0;

		/**
		 * Provides the valid measurement range
		 * @return <minimun frequency, maximum frequency>
		 */
		virtual std::pair<double, double> getFrequencyRange() const = 0;

	protected:
		double frequency;
	};

	struct Gyroscope: public InertialMeasurementUnit {

		//valid range [0.5, 10]
		void setFrequency(double frequency) override;

		/**
		 * @return orientation in degrees [0, 360]
		 */
		int getOrientation() const override;

		std::pair<double, double> getFrequencyRange() const override;

	protected:
		static constexpr double min = .5f, max = 10.0f;
	};

	constexpr double Gyroscope::min;
	constexpr double Gyroscope::max;

	void Gyroscope::setFrequency(double frequency){
		if(frequency >= min && frequency <= max){
			this->frequency = frequency;
		} else {
			throw std::out_of_range("frequency out of range [0.5, 10]");
		}
	}

	int Gyroscope::getOrientation() const {
		return 36; //some value
	}

	std::pair<double, double> Gyroscope::getFrequencyRange() const {
		return std::pair<double, double>(max, min);
	}

	struct Accelerometer : public InertialMeasurementUnit
	{
		//valid range [0.1, 100]
		void setFrequency(double frequency) override;

		/**
		 * @return orientation in degrees [-180, 180)
		 */
		int getOrientation() const override;

		std::pair<double, double> getFrequencyRange()  const override;

	protected:
		const static double min, max;
	};

	const double Accelerometer::min = .1f;
	const double Accelerometer::max = 100.0f;

	void Accelerometer::setFrequency(double frequency){
		if(frequency >= min && frequency <= max){
			this->frequency = frequency;
		} else {
			throw std::out_of_range("frequency out of range [0.1, 100]");
		}
	}

	int Accelerometer::getOrientation() const {
		return 36; //some value
	}

	std::pair<double, double> Accelerometer::getFrequencyRange() const {
		return std::pair<double, double>(max, min);
	}

#endif //GOOD CODE

int main()
{
	InertialMeasurementUnit *imu = new Gyroscope;

#ifdef BAD_CODE
	//if any error
	if(!imu->setFrequency(500)){
		std::cout << "for some reason it didn't go through" << std::endl;
	}
#else
	//now we can now what's going on.
	try {
		imu->setFrequency(500);
	} catch (std::out_of_range &oor) {
		std::cout << oor.what() << std::endl;
	}

	//const auto &[max, min] = imu->getFrequencyRange(); //c++ 17
	//std::cout << "max: " << max << " min: " << min << std::endl;


	const auto &ello = imu->getFrequencyRange();
	std::cout << "max: " << ello.second << " min: " << ello.first << std::endl;


#endif
	delete imu;

	return 0;
}
