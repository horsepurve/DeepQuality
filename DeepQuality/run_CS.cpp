#define RUN_CS
#ifdef RUN_CS
#define _SCL_SECURE_NO_WARNINGS

//* note of comments:
//* //* is important comments, explains something
//* /**/ is used for unit testing 
//* //can be deleted, just as a remainder
//* //! is useful cout, for debugging 

#include <iostream>
#include <ctime> 
#include "FileHandler.h"
#include "Utility.h"
#include "CS.h"
#include "Spectrum.h"

int main(int argc, char const *argv[]) {
	if (argc == 1) {
		cout << "DeepSQ v1.0 -- CompressedSensing.exe: This program is used for converting MGF spectra to features.\n";
		cout << "Usage: CompressedSensing.exe config_project_name.JSON\n";
		return 0;
	}
	cout << "config file is: " << argv[1] << endl;

	InitTensorEngine<cpu>();

	Parameter p;
	//read_config("config_default.JSON", p);
	read_config(argv[1], p);

	Spectrum sp(p.min_mz, p.max_mz, p.delta, p.sample);

	time_t start, end;
	if (p.train_only || (!p.train_only && !p.test_only)) {
		string data_csv = p.train_mgf + ".data.csv";

		start = clock();
		cout << "Loading training file, compressed sensing for spectra:\n";
		load_file(p.train_mgf.c_str(), data_csv.c_str(), doline, sp);
		end = clock();
		cout << "Done! costing time: " << end - start << " ms\n";
	}

	if (p.test_only || (!p.train_only && !p.test_only)) {
		string data_csv = p.test_mgf + ".data.csv";

		start = clock();
		cout << "Loading testing file, compressed sensing for spectra:\n";
		load_file(p.test_mgf.c_str(), data_csv.c_str(), doline, sp);
		end = clock();
		cout << "Done! costing time: " << end - start << " ms\n";
	}
	
	ShutdownTensorEngine<cpu>();
	return 0;
}
#endif // RUN_CS
