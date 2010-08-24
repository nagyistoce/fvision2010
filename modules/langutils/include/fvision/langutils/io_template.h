#ifndef FVISION_IO_TEMPLATE_H_
#define FVISION_IO_TEMPLATE_H_


#include <vector>
#include <iostream>
#include <fstream>
#include <string>

namespace fvision {

	//===================== Template Functions ===================>

	template <typename T>
	bool save(const char* filename, const T& t) {
		std::ofstream ofs(filename);
		if (ofs.fail()) return false;
		ofs<<t;
		ofs.close();
		return true;
	}

	template <typename T>
	bool save(const std::string& filename, const T& t) {
		std::ofstream ofs(filename.c_str());
		if (ofs.fail()) return false;
		ofs<<t;
		ofs.close();
		return true;
	}

	template <typename T>
	bool load(const std::string& filename, T& t){
		std::ifstream ifs(filename.c_str());
		if (ifs.fail()) return false;
		ifs>>t;
		ifs.close();
		return true;
	}

	/** write a std::vector to a file
	 * format:
	 * n
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void saveV(const std::string& filename, const std::vector<T>& ts) {
		std::ofstream ofs(filename.c_str());
		ofs<<ts.size()<<std::endl;
		for (size_t i = 0; i < ts.size(); i++) {
			ofs<<ts[i]<<std::endl;
		}
		ofs.close();
	}

	/** load a std::vector from a file
	 * format:
	 * n
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void loadV(const std::string& filename, std::vector<T>& ts) {
		std::ifstream ifs(filename.c_str());
		int n;
		ifs>>n;
		for (int i = 0; i < n; i++) {
			T t;
			ifs>>t;
			ts.push_back(t);
			ifs.get();
		}
		ifs.close();
	}

	/** write a std::vector to a file with no number of elements
	 * format:
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void saveVWithNoNumber(const std::string& filename, const std::vector<T>& ts) {
		std::ofstream ofs(filename.c_str());
		for (size_t i = 0; i < ts.size(); i++) {
			ofs<<ts[i]<<std::endl;
		}
		ofs.close();
	}

	/** load a std::vector from a file with no number of elements
	 * format:
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void loadVWithNoNumber(const std::string& filename, std::vector<T>& ts) {
		std::ifstream ifs(filename.c_str());
		while (!ifs.eof()) {
			//skip to next non whitespace character
			while(true) {
				char c = ifs.get();
				if (c == ' ' || c == '\r' || c == '\n') continue;
				else {
					ifs.unget();
					break;
				}
			}

			if (ifs.eof()) break;
			T t;
			ifs>>t;
			ts.push_back(t);
		}
		ifs.close();
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const std::vector<T>& vs) {
		for (size_t i = 0; i < vs.size(); i++) {
            os<<vs[i]<<std::endl;
		}
		return os;
	}

	template<typename Iterator>
	void output(std::ostream& os, Iterator begin, Iterator end) {
		Iterator iter = begin;
        for (; iter != end; iter++) os<<*iter<<std::endl;
	}

	template<typename T>
	void outputToSameLine(std::ostream& os, const std::vector<T>& vs) {
		for (size_t i = 0; i < vs.size(); i++) {
			os<<vs[i]<<" ";
		}
	}

	template<typename T>
    void outputToTwoDimensionVector(std::ostream& os, const std::vector<std::vector<T> >& vss) {
		for (size_t i = 0; i < vss.size(); i++) {
			outputToSameLine(os, vss[i]);
            os<<std::endl;
		}
	}

}
#endif // FVISION_IO_TEMPLATE_H_
