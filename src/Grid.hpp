#include <vector>

using namespace std;

template <typename T>
class Grid {
	public:
		Grid(long xSize, long ySize, T initValue) {
			data.resize(xSize);
			for(auto x = 0L; x < xSize; ++x) {
				data[x].resize(ySize);
			}
			setAll(initValue);
		}
		void setAll(T value) {
			for(auto x = 0L; x < data.size(); ++x) {
				for(auto y = 0L; y < data[x].size(); ++y) {
					data[x][y] = value;
				}
			}
		}
		T & operator()(long x, long y) {
			return data[x][y];
		}
	private:
		vector< vector<T> > data;
};
