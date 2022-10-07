/**
 * The Open/Close Principle.
 * Classes should be open for extension but closed for modification.
 */

#include <iostream>

enum class SensorModel {
	Good,
	Better
};

//#define BAD_CODE

#ifdef BAD_CODE

	/**
	 * the DistanceSensor class implements a business logic per different model provided upon construction.
	 * Every time that a new sensor model is added we need to modify the class which is also a concretion.
	 */
	struct DistanceSensor {
		DistanceSensor() = delete;
		DistanceSensor(SensorModel model) : mModel{model} {}

		int getDistance()
		{
			switch(mModel) {
			case SensorModel::Good:
				std::cout << "Business logic for \"Good\" model.";
				return 1;
			case SensorModel::Better:
				std::cout << "Business logic for \"Better\" model.";
				return 2;
			}

			return 0;
		}

		SensorModel mModel;
	};

#else //GOOD CODE
	/**
	 * In open/close principle we can make an abstraction of DistanceSensor and
	 * enclose the business logic into concrete classes
	 * and we only instantiate the concrete class depending on the param it's passed to a factory method.
	 * so that, if we need to make new business logic we don't need to change anything but the factory method that instantiate them.
	 */

	struct DistanceSensor {
		virtual ~DistanceSensor() = default;
		virtual int getDistance() = 0;

		static DistanceSensor *getSensor(SensorModel model);
	};

	struct GoodDistanceSensor : public DistanceSensor {
		int getDistance() override {
			std::cout << "Business logic for \"Good\" model.";
			return 1;
		}
	};

	struct BetterDistanceSensor : public DistanceSensor {
		int getDistance() override {
			std::cout << "Business logic for \"Better\" model.";
			return 2;
		}
	};

	//now we implement getSensor factory method.
	DistanceSensor *DistanceSensor::getSensor(SensorModel model){
		switch(model){
		case SensorModel::Good:
			return new GoodDistanceSensor;
		case SensorModel::Better:
			return new BetterDistanceSensor;
		default:
			return nullptr;
		}
	};

#endif //GOOD CODE

int main()
{
	DistanceSensor *ds;

#ifdef BAD_CODE
	ds = new DistanceSensor(SensorModel::Better);
#else
	ds = DistanceSensor::getSensor(SensorModel::Better);
#endif

	ds->getDistance();

	delete ds;

	return 0;
}

