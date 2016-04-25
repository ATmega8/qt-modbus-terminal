#include "ekf_data.h"

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

class EKF
{

public:
	 EKF(EKFData* ekfData, unsigned int z_len);
	~EKF(void);

	void update(vector<double> z_in);
	void expm(gsl_matrix* m, unsigned int n);
	void vector2matrix(vector<double> v, gsl_matrix* m);
	vector<double> eulerAngles(void);

	gsl_vector* X;
	gsl_matrix* H;
	gsl_matrix* P;

private:
	void matrix2vector(vector<double> v, gsl_matrix* m);
	void printfMatrix(const gsl_matrix* m);

	EKFData* m_ekfData;	

	gsl_matrix* K;
	gsl_matrix* A;
	gsl_matrix* R;
	gsl_matrix* Q;

	gsl_vector* z;
};

