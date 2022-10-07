/**
 * Dependency Inversion principle
 * Class should depend on abstraction but not on concretion.
 */
#include <string>

//#define BAD_CODE

#ifdef BAD_CODE

	struct Cloud {
		virtual ~Cloud() = default;

		//this abstraction has now de problem that depends on a concrete detail as it actually belongs to AwCloud
		virtual void uploadToS3Bucket(std::string filepath) = 0;
	};

	struct AwsCloud: public Cloud{
		void uploadToS3Bucket(std::string filepath) override {};
	};

	struct FileUploader {
		//this method depends on a concrete class
		FileUploader(AwsCloud &awsCloud);

		//we can fix this making the method depend on an abstraction
		FileUploader(Cloud &cloud);

		void scheduleUpload(std::string filepath);
	};

#else
	/**
	 * now the interface has a general method upload which doesn't depend in any concrete detail
	 */
	struct Cloud {
		virtual ~Cloud() = default;
		virtual void upload(std::string filepath) = 0;
	};

	/**
	 * AwsCloud concrete class implements the method upload that internally
	 * calls its own uploadToS3Bucket method with it's concrete logic
	 */
	struct AwsCloud: public Cloud{
		void upload(std::string filepath) override {
			uploadToS3Bucket(filepath);
		};

	private:
		void uploadToS3Bucket(std::string filepath);
	};

	struct GoogleCloud: public Cloud{
		void upload(std::string filepath) override {
			uploadTpGoogle(filepath);
		};

	private:
		void uploadTpGoogle(std::string filepath);
	};

	/**
	 * and now FileUploader can accept any Cloud class without needing to know much about their internals
	 */
	struct FileUploader {

		FileUploader(Cloud &cloud);

		void scheduleUpload(std::string filepath);
	};

#endif

int main()
{
	return 0;
}
