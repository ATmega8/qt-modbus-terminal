#include <vector>

//#define vector std::vector

using namespace std;

class EKFData
{

public:
	 EKFData(int stateVariableCount, int observationVariableCount);
	~EKFData(void);

	void setInitialStateVariable(const vector<double> stateVariable);
	void setInitialCovarianceValue(const double cov);
	void setObservationNoiseValue(const vector<double> observationNoise);
	void setProcessNoiseValue(const vector<double> processNoise);

	void setObservationMatrix(const vector<double> observationMatrix);
	void setStateMatrix(const vector<double> stateMatrix);

	vector<double> stateVariable(void) const;
	vector<double> observationNoiseValue(void) const;
	vector<double> processNoiseValue(void) const;
	vector<double> observationMatrix(void) const;
	vector<double> covarianceMatrix(void) const;

private:
	vector<double> m_stateVariable;
	vector<double> m_observationNoise;
	vector<double> m_processNoise;
	vector<double> m_observationMatrix;
	vector<double> m_covarianceMatrix;
	vector<double> m_stateMatrix;

};

